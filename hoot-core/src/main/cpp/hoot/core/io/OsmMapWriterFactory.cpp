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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmMapWriterFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/OsmMapWriter.h>
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/io/ElementOutputStream.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/MapProjector.h>

using namespace std;

namespace hoot
{

unsigned int OsmMapWriterFactory::_debugMapCount = 1;

boost::shared_ptr<OsmMapWriter> OsmMapWriterFactory::createWriter(QString url)
{
  LOG_VART(url);

  QString writerOverride = ConfigOptions().getOsmMapWriterFactoryWriter();
  LOG_VART(writerOverride);

  boost::shared_ptr<OsmMapWriter> writer;
  if (writerOverride != "" && url != ConfigOptions().getDebugMapsFilename())
  {
    writer.reset(Factory::getInstance().constructObject<OsmMapWriter>(writerOverride));
  }

  vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(OsmMapWriter::className());
  for (size_t i = 0; i < names.size() && !writer; ++i)
  {
    LOG_VART(names[i]);
    writer.reset(Factory::getInstance().constructObject<OsmMapWriter>(names[i]));
    if (writer->isSupported(url))
    {
      LOG_DEBUG("Using output writer: " << names[i]);
    }
    else
    {
      writer.reset();
    }
  }

  if (!writer)
  {
    throw HootException("A valid writer could not be found for the URL: " + url);
  }

  return writer;
}

QString OsmMapWriterFactory::getWriterName(const QString url)
{
  LOG_VARD(url);
  vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(OsmMapWriter::className());
  LOG_VARD(names.size());
  boost::shared_ptr<OsmMapWriter> writer;
  for (size_t i = 0; i < names.size(); i++)
  {
    const std::string name = names[i];
    LOG_VART(name);
    writer.reset(Factory::getInstance().constructObject<OsmMapWriter>(name));
    if (writer->isSupported(url))
    {
      return QString::fromStdString(name);
    }
  }
  return "";
}

bool OsmMapWriterFactory::isSupportedFormat(const QString url)
{
  return !getWriterName(url).trimmed().isEmpty();
}

bool OsmMapWriterFactory::hasElementOutputStream(QString url)
{
  bool result = false;
  boost::shared_ptr<OsmMapWriter> writer = createWriter(url);
  boost::shared_ptr<ElementOutputStream> streamWriter =
    boost::dynamic_pointer_cast<ElementOutputStream>(writer);
  if (streamWriter)
  {
    result = true;
  }

  return result;
}

void OsmMapWriterFactory::write(const boost::shared_ptr<const OsmMap>& map, QString url,
                                const bool silent)
{
  bool skipEmptyMap = map->isEmpty() && ConfigOptions().getOsmMapWriterSkipEmptyMap();

  if (!silent)
  {
    LOG_INFO((skipEmptyMap ? "Map is empty. Not writing to " : "Writing map to ") << url << "...");
  }

  if (!skipEmptyMap)
  {
    boost::shared_ptr<OsmMapWriter> writer = createWriter(url);
    writer->open(url);
    writer->write(map);
  }
}

void OsmMapWriterFactory::writeDebugMap(const ConstOsmMapPtr& map, const QString title)
{
  if (ConfigOptions().getDebugMapsWrite())
  {
    QString debugMapFileName = ConfigOptions().getDebugMapsFilename();
    if (!debugMapFileName.toLower().endsWith(".osm"))
    {
      throw IllegalArgumentException("Debug maps must be written to a .osm file.");
    }
    const QString fileNumberStr = StringUtils::getNumberStringPaddedWithZeroes(_debugMapCount, 3);
    if (!title.isEmpty())
    {
      debugMapFileName =
        debugMapFileName.replace(".osm", "-" + fileNumberStr + "-" + title + ".osm");
    }
    else
    {
      debugMapFileName = debugMapFileName.replace(".osm", "-" + fileNumberStr + ".osm");
    }
    LOG_DEBUG("Writing debug output to " << debugMapFileName);
    OsmMapPtr copy(new OsmMap(map));
    MapProjector::projectToWgs84(copy);
    write(copy, debugMapFileName, true);
    _debugMapCount++;
  }
}

}
