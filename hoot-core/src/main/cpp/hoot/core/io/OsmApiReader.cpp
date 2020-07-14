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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmApiReader.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/OsmApiUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/ReportMissingElementsVisitor.h>

// Qt
#include <QBuffer>

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
  ConfigOptions configOptions(conf);
  //  Get the bounds of the query
  setBounds(GeometryUtils::envelopeFromConfigString(configOptions.getConvertBoundingBox()));
  setMaxThreads(configOptions.getReaderHttpBboxThreadCount());
  setCoordGridSize(configOptions.getReaderHttpBboxMaxSize());
  setMaxGridSize(configOptions.getReaderHttpBboxMaxDownloadSize());
}

void OsmApiReader::setUseDataSourceIds(bool /*useDataSourceIds*/)
{
  //  No matter what, we want to always use the data source's IDs
  _useDataSourceId = true;
}

QString OsmApiReader::supportedFormats()
{
  return "http://;https://";
}

bool OsmApiReader::isSupported(const QString& url)
{
  QStringList validPrefixes = supportedFormats().split(";");
  const QString checkString(url.toLower());
  //  Support HTTP and HTTPS URLs to OSM API servers
  for (int i = 0; i < validPrefixes.size(); ++i)
  {
    if (checkString.startsWith(validPrefixes[i]) && checkString.endsWith(OsmApiEndpoints::API_PATH_MAP))
      return true;
  }
  //  If we fall out of loop, no dice
  return false;
}

void OsmApiReader::open(const QString& url)
{
  //  Save off the URL
  OsmMapReader::open(url);
}

void OsmApiReader::read(const OsmMapPtr& map)
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
  _map = map;
  _map->appendSource(_url);

  //  Spin up the threads
  beginRead(_url, _bounds);

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
      {
        throw HootException(_errorString);
      }
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

  _map.reset();
}

void OsmApiReader::close()
{
  finalizePartial();

  _map.reset();
}

}
