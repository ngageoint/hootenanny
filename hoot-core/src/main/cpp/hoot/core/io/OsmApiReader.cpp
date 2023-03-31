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
 * @copyright Copyright (C) 2019-2023 Maxar (http://www.maxar.com/)
 */

#include "OsmApiReader.h"

// Hoot
#include <hoot/core/criterion/ChildElementCriterion.h>
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
  setBounds(Boundable::loadBounds(config));
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
  //  Check if it is an XML Overpass URL
  if (isOverpassXml(url))
    return true;
  QStringList validPrefixes = supportedFormats().split(";");
  const QString checkString(url.toLower());
  //  Check the OSM API endpoint
  if (!checkString.endsWith(OsmApiEndpoints::OSM_API_PATH_MAP))
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

void OsmApiReader::open(const QString& url)
{
  //  Base class open
  OsmXmlReader::open(url);
  //  Check if the URL is an Overpass URL
  setOverpassUrl(url);
}

void OsmApiReader::read(const OsmMapPtr& map)
{
  _map = map;
  _map->appendSource(_url);

  //  Initialize the object
  initializePartial();

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
  if (_getIsBoundsPoly())
  {
    size_t element_count = _map->getElementCount();
    //  Remove any elements that don't meet the criterion
    RemoveElementsVisitor v(true);
    v.addCriterion(_polyCriterion);
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
  _setIsBoundsPoly(bounds ? !bounds->isRectangle() : false);
  _setBoundingPoly(bounds);
  _polyCriterion = std::make_shared<InBoundsCriterion>(false);
  _polyCriterion->setBounds(bounds);
  Boundable::setBounds(bounds);
}

void OsmApiReader::setBounds(const geos::geom::Envelope& bounds)
{
  std::shared_ptr<geos::geom::Geometry> geometry(std::shared_ptr<geos::geom::Geometry>(geos::geom::GeometryFactory::getDefaultInstance()->toGeometry(&bounds).release()));
  //  An envelope isn't a poly bounds
  _setIsBoundsPoly(false);
  _setBoundingPoly(geometry);
  _polyCriterion = std::make_shared<InBoundsCriterion>(false);
  _polyCriterion->setBounds(geometry);
  Boundable::setBounds(bounds);
}

void OsmApiReader::initializePartial()
{
  LOG_VART(_status);
  LOG_VART(_useDataSourceId);
  LOG_VART(_useFileStatus);
  LOG_VART(_keepStatusTag);
  LOG_VART(_preserveAllTags);

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

  if (_missingElements.empty())
  {
    //  Set the bounds once we begin the read if setBounds() hasn't already been called
    if (_bounds == nullptr)
      _loadBounds();

    if (_bounds == nullptr)
      throw IllegalArgumentException("OsmApiReader requires rectangular bounds");

    //  Spin up the threads and use the envelope of the bounds to throw away any non-retangular bounds
    //  that may have been passed.
    beginRead(_url, *(_bounds->getEnvelopeInternal()));
  }
  else
  {
    //  Spin up the threads and use the missing elements to run separate queries
    beginRead(_url);
  }
}

bool OsmApiReader::hasMoreElements()
{
  //  Check if there is anything to read and parse
  if (!hasMoreResults() && _streamReader.atEnd())
    return false;
  //  Check if there is an open buffer to read from
  if (!_xmlBuffer.isOpen())
  {
    finalizePartial();
    if (!_elementConverter)
      _elementConverter = std::make_shared<ElementToGeometryConverter>(_getElementProvider());

    QString xmlResult;
    //  Get one XML string to parse
    while (!getSingleResult(xmlResult))
    {
      if (!hasMoreResults())
        return false;
      _sleep();
    }

    if (_xmlBuffer.isOpen())
      _xmlBuffer.close();
    _xmlBuffer.setData(xmlResult.toUtf8());
    _xmlBuffer.open(QBuffer::ReadOnly);
    _streamReader.setDevice(&_xmlBuffer);

    //  check for a valid osm header as soon as the file is opened
    while (!_foundOsmHeaderXmlStartElement() && !_streamReader.atEnd())
      _streamReader.readNext();

    if (!_osmFound)
      throw HootException("Unable to parse XML result from " + _url);
  }

  //  chew up tokens until we find a node/way/relation start element or read to the end of the file
  while (!_foundOsmElementXmlStartElement() && !_streamReader.atEnd())
    _streamReader.readNext();

  if ((_streamReader.isEndElement() && _streamReader.name().toString() == "osm") || _streamReader.atEnd())
  {
    //  Close the buffer
    _xmlBuffer.close();
    //  Check if there are more buffers coming in
    if (hasMoreResults())
      return hasMoreElements();
    else
      return false;
  }
  //  Read the next element and is available in _element
  return true;
}

ElementPtr OsmApiReader::readNextElement()
{
  //  Grab the next element from the OsmXmlReader
  ElementPtr result = OsmXmlReader::readNextElement();
  //  Make sure that the element hasn't been read before, this can happen because elements
  //  that span more than one bounding box will be returned from the API in each of the bounding
  //  boxes.  Write the first, ignore the subsequent ones
  while (!_canUseElement(result))
  {
    //  Check for the next element and read it, otherwise return an empty element
    if (hasMoreElements())
      result = OsmXmlReader::readNextElement();
    else
      return ElementPtr();
  }
  //  Add the element to the set
  ConstElementPtr e = std::const_pointer_cast<const Element>(result);
  _elementCache->addElement(e);
  //  Return the element read
  return result;
}

bool OsmApiReader::_loadBounds()
{
  setBounds(Boundable::loadBounds());
  //  Return true if the bounds object was created
  return _bounds != nullptr;
}

bool OsmApiReader::_canUseElement(const ElementPtr& element) const
{
  if (!element)
    return false;
  else if (_elementCache->containsElement(element->getElementId()))
    return false;
  else if (_getIsBoundsPoly() && element->getElementType() != ElementType::Node && !_isInPolyBounds(element))
    return false;
  else
    return true;
}

bool OsmApiReader::_isInPolyBounds(const ElementPtr& element) const
{
  std::shared_ptr<geos::geom::Geometry> e = _elementConverter->convertToGeometry(element);
  std::shared_ptr<geos::geom::Geometry> b = _getBoundingPoly();
  return (e && b && e->intersects(b.get()));
}

}
