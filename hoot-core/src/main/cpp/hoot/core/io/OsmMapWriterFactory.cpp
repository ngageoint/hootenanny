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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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
#include <hoot/core/conflate/network/DebugNetworkMapCreator.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

unsigned int OsmMapWriterFactory::_debugMapCount = 1;

std::shared_ptr<OsmMapWriter> OsmMapWriterFactory::createWriter(const QString& url)
{
  LOG_VART(url);

  QString writerOverride = ConfigOptions().getOsmMapWriterFactoryWriter();
  LOG_VART(writerOverride);

  std::shared_ptr<OsmMapWriter> writer;
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

QString OsmMapWriterFactory::getWriterName(const QString& url)
{
  LOG_VARD(url);
  vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(OsmMapWriter::className());
  LOG_VARD(names.size());
  std::shared_ptr<OsmMapWriter> writer;
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

bool OsmMapWriterFactory::isSupportedFormat(const QString& url)
{
  return !getWriterName(url).trimmed().isEmpty();
}

bool OsmMapWriterFactory::hasElementOutputStream(const QString& url)
{
  bool result = false;
  std::shared_ptr<OsmMapWriter> writer = createWriter(url);
  std::shared_ptr<ElementOutputStream> streamWriter =
    std::dynamic_pointer_cast<ElementOutputStream>(writer);
  if (streamWriter)
  {
    result = true;
  }

  return result;
}

void OsmMapWriterFactory::write(const std::shared_ptr<OsmMap>& map, const QString& url,
                                const bool silent, const bool is_debug)
{
  bool skipEmptyMap = map->isEmpty() && ConfigOptions().getOsmMapWriterSkipEmptyMap();

  if (!silent)
  {
    LOG_INFO(
      (skipEmptyMap ? "Map is empty. Not writing to " : "Writing map to ") << url << "...");
  }

  if (!skipEmptyMap)
  {
    QElapsedTimer timer;
    timer.start();

    MapProjector::projectToWgs84(map);

    std::shared_ptr<OsmMapWriter> writer = createWriter(url);
    writer->setIsDebugMap(is_debug);
    writer->open(url);
    // We could pass a progress in here to get more granular write status feedback.
    writer->write(map);
    LOG_STATUS(
      "Wrote " << StringUtils::formatLargeNumber(map->getElementCount()) <<
      " elements to output in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
  }
}

void OsmMapWriterFactory::writeDebugMap(const ConstOsmMapPtr& map, const QString& title,
                                        NetworkMatcherPtr matcher)
{
  if (ConfigOptions().getDebugMapsWrite())
  {
    QString debugMapFileName = ConfigOptions().getDebugMapsFilename();
    if (!debugMapFileName.toLower().endsWith(".osm"))
    {
      throw IllegalArgumentException("Debug maps must be written to an .osm file.");
    }

    LOG_VARD(StringUtils::formatLargeNumber(map->getElementCount()));
    LOG_VARD(StringUtils::formatLargeNumber(map->getNodeCount()));
    LOG_VARD(StringUtils::formatLargeNumber(map->getWayCount()));
    LOG_VARD(StringUtils::formatLargeNumber(map->getRelationCount()));

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
    LOG_INFO("Writing debug output to: ..." << debugMapFileName.right(30));
    OsmMapPtr copy(new OsmMap(map));

    if (matcher)
    {
      DebugNetworkMapCreator()
        .addDebugElements(copy, matcher->getAllEdgeScores(), matcher->getAllVertexScores());
    }

    MapProjector::projectToWgs84(copy);
    if (ConfigOptions().getDebugMapsRemoveMissingElements())
    {
      // Don't remove elements recursively here. You can end up with a map unreadable in JOSM if
      // you don't remove missing elements here. However, in some cases (like debugging cut and
      // replace), you want to see them in the raw output to know things are working the way they
      // should be.
      RemoveMissingElementsVisitor missingElementsRemover;
      copy->visitRw(missingElementsRemover);
    }
    write(copy, debugMapFileName, true, true);
    _debugMapCount++;
  }
}

}
