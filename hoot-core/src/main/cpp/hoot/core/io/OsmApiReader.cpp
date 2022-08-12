/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "OsmApiReader.h"

// Hoot
#include <hoot/core/criterion/ChildElementCriterion.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/OsmApiUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/ReportMissingElementsVisitor.h>

// Qt
#include <QBuffer>

//  Geos
#include <geos/geom/GeometryFactory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapReader, OsmApiReader)

OsmApiReader::OsmApiReader()
{
  setConfiguration(conf());
  //  When reading in from the OSM API there won't be duplicates unless we are
  //  dividing up the bounds into smaller quadrants that fit below the 0.25 degrees
  //  squared or 50k element max limits, when we do it is safe to ignore duplicate
  //  elements
  setIgnoreDuplicates(true);
  //  Because this data is coming from an OSM API we MUST use the data source IDs
  setUseDataSourceIds(true);
  //  Because the data may come in a different order, add the `nd` and `ref` members
  //  even if they don't exist in the sub-area that is currently being parsed
  setAddChildRefsWhenMissing(true);
  //  Set the data type to Osm Xml for debugging/output purposes
  _dataType = DataType::OsmXml;
}

OsmApiReader::~OsmApiReader()
{
  close();
}

void OsmApiReader::setConfiguration(const Settings& conf)
{
  ConfigOptions config(conf);
  setMaxThreads(config.getReaderHttpBboxThreadCount());
  setCoordGridSize(config.getReaderHttpBboxMaxSize());
  setMaxGridSize(config.getReaderHttpBboxMaxDownloadSize());
  _boundsString = config.getBounds();
  _boundsFilename = config.getBoundsInputFile();
}

void OsmApiReader::setUseDataSourceIds(bool /*useDataSourceIds*/)
{
  //  No matter what, we want to always use the data source's IDs
  _useDataSourceId = true;
}

QString OsmApiReader::supportedFormats() const
{
  return "http://;https://";
}

bool OsmApiReader::isSupported(const QString& url) const
{
  QStringList validPrefixes = supportedFormats().split(";");
  const QString checkString(url.toLower());
  //  Check the OSM API endpoint
  if (!checkString.endsWith(OsmApiEndpoints::API_PATH_MAP))
    return false;
  //  Support HTTP and HTTPS URLs to OSM API servers
  for (const auto& prefix : qAsConst(validPrefixes))
  {
    if (checkString.startsWith(prefix))
      return true;
  }
  //  If we fall out of loop, no dice
  return false;
}

void OsmApiReader::read(const OsmMapPtr& map)
{
  LOG_VART(_status);
  LOG_VART(_useDataSourceId);
  LOG_VART(_useFileStatus);
  LOG_VART(_keepStatusTag);
  LOG_VART(_preserveAllTags);

  //  Set the bounds once we begin the read if setBounds() hasn't already been called
  if (_bounds == nullptr && _loadBounds())
    setBounds(_boundingPoly);

  if (_bounds == nullptr)
    throw IllegalArgumentException("OsmApiReader requires rectangular bounds");

  //  Reusing the reader for multiple reads has two options, the first is the
  //  default where the reader is reset and duplicates error out.  The second
  //  is where duplicates are ignored in the same dataset and across datasets
  //  so the ID maps aren't reset
  if (!_ignoreDuplicates)
  {
    _nodeIdMap.clear();
    _relationIdMap.clear();
    _wayIdMap.clear();

    _numRead = 0;
    finalizePartial();
  }
  _map = map;
  _map->appendSource(_url);

  //  Spin up the threads. Use the envelope of the boundsto throw away any non-retangular bounds
  // that may have been  passed.
  beginRead(_url, *(_bounds->getEnvelopeInternal()));

  //  Iterate all of the XML results
  while (hasMoreResults())
  {
    QString xmlResult;
    //  Get one XML string at a time
    if (getSingleResult(xmlResult))
    {
      //  Use an XML reader and the base class overloads
      QXmlSimpleReader reader;
      reader.setContentHandler(this);
      reader.setErrorHandler(this);
      //  Convert the string to a buffer to parse
      QBuffer buffer;
      buffer.setData(xmlResult.toUtf8());
      //  Do the actual parsing
      QXmlInputSource xmlInputSource(&buffer);
      if (reader.parse(xmlInputSource) == false)
        throw HootException(_errorString);
    }
    else
      _sleep();
  }

  if (isError())
    throw HootException("Error reading OSM API");

  LOG_VARD(StringUtils::formatLargeNumber(_map->getElementCount()));

  ReportMissingElementsVisitor visitor;
  LOG_INFO("\t" << visitor.getInitStatusMessage());
  _map->visitRw(visitor);
  LOG_DEBUG("\t" << visitor.getCompletedStatusMessage());

  //  Filter the map based on the polygon
  if (_isPolygon)
  {
    size_t element_count = _map->getElementCount();
    std::shared_ptr<InBoundsCriterion> crit = std::make_shared<InBoundsCriterion>(false);
    crit->setBounds(_boundingPoly);
    //  Remove any elements that don't meet the criterion
    RemoveElementsVisitor v(true);
    v.addCriterion(crit);
    v.setRecursive(true);
    v.setOsmMap(_map.get());
    _map->visitRw(v);
    size_t elements_filtered = element_count - _map->getElementCount();
    if (elements_filtered > 0)
      LOG_STATUS("Filtered " << elements_filtered << " out of bounds elements from OSM API.");
 }
  _map.reset();
}

void OsmApiReader::close()
{
  finalizePartial();
  _map.reset();
}

void OsmApiReader::setBounds(std::shared_ptr<geos::geom::Geometry> bounds)
{
  //  Check if the bounds are rectangular
  _isPolygon = bounds ? !bounds->isRectangle() : false;
  _boundingPoly = bounds;
  Boundable::setBounds(bounds);
}

void OsmApiReader::setBounds(const geos::geom::Envelope& bounds)
{
  //  An envelope isn't a poly bounds
  _isPolygon = false;
  _boundingPoly.reset(geos::geom::GeometryFactory::getDefaultInstance()->toGeometry(&bounds).release());
  Boundable::setBounds(bounds);
}

bool OsmApiReader::_loadBounds()
{
  bool isEnvelope = false;
  //  Try to read the bounds from the `bounds` string
  if (_boundsString != ConfigOptions::getBoundsDefaultValue())
  {
    _boundingPoly = GeometryUtils::boundsFromString(_boundsString, isEnvelope);
    _bounds = _boundingPoly;
    _isPolygon = !isEnvelope;
    return true;
  }
  //  Try to read the bounds from the `bounds.input.file` file
  else if (_boundsFilename != ConfigOptions::getBoundsInputFileDefaultValue())
  {
    _boundingPoly = GeometryUtils::readBoundsFromFile(_boundsFilename, isEnvelope);
    _bounds = _boundingPoly;
    _isPolygon = !isEnvelope;
    return true;
  }
  return false;
}

}
