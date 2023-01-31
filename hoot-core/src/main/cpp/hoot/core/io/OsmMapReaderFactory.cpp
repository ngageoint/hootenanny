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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Maxar (http://www.maxar.com/)
 */
#include "OsmMapReaderFactory.h"

// hoot
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Validate.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

bool OsmMapReaderFactory::supportsPartialReading(const QString& url)
{
  std::shared_ptr<OsmMapReader> reader = createReader(url, true, Status::Unknown1);
  std::shared_ptr<PartialOsmMapReader> pr = std::dynamic_pointer_cast<PartialOsmMapReader>(reader);
  return pr.operator bool();
}

std::shared_ptr<OsmMapReader> OsmMapReaderFactory::_createReader(const QString& url)
{
  std::shared_ptr<OsmMapReader> reader;

  const QString readerOverride = ConfigOptions().getMapFactoryReader();
  if (readerOverride != "")
  {
    reader = Factory::getInstance().constructObject<OsmMapReader>(readerOverride);
    LOG_DEBUG("Using reader: " << readerOverride);
  }

  vector<QString> names = Factory::getInstance().getObjectNamesByBase(OsmMapReader::className());
  for (const auto& name : names)
  {
    LOG_TRACE("Checking input " << url << " with reader " << name);
    reader = Factory::getInstance().constructObject<OsmMapReader>(name);
    if (reader->isSupported(url))
    {
      LOG_DEBUG("Using input reader: " << name);
      break;
    }
    else
      reader.reset();
  }

  if (!reader)
    throw HootException("A valid reader could not be found for the URL: " + url);

  reader->setConfiguration(conf());

  return reader;
}

std::shared_ptr<OsmMapReader> OsmMapReaderFactory::createReader(const QString& url, bool useDataSourceIds, Status defaultStatus, bool cropOnReadIfBounded)
{
  LOG_VART(url);
  LOG_VART(useDataSourceIds);
  LOG_VART(defaultStatus);

  std::shared_ptr<OsmMapReader> reader = _createReader(url);
  reader->setUseDataSourceIds(useDataSourceIds);
  reader->setDefaultStatus(defaultStatus);
  std::shared_ptr<Boundable> bounded = std::dynamic_pointer_cast<Boundable>(reader);
  if (bounded)
    bounded->setCropOnReadIfBounded(cropOnReadIfBounded);
  return reader;
}

std::shared_ptr<OsmMapReader> OsmMapReaderFactory::createReader(bool useDataSourceIds, bool useFileStatus, const QString& url, bool cropOnReadIfBounded)
{
  LOG_VART(url);
  LOG_VART(useDataSourceIds);
  LOG_VART(useFileStatus);

  std::shared_ptr<OsmMapReader> reader = _createReader(url);
  reader->setUseDataSourceIds(useDataSourceIds);
  reader->setUseFileStatus(useFileStatus);
  std::shared_ptr<Boundable> bounded = std::dynamic_pointer_cast<Boundable>(reader);
  if (bounded)
    bounded->setCropOnReadIfBounded(cropOnReadIfBounded);
  return reader;
}

QString OsmMapReaderFactory::getReaderName(const QString& url)
{
  LOG_VART(url);
  vector<QString> names = Factory::getInstance().getObjectNamesByBase(OsmMapReader::className());
  LOG_VART(names.size());
  std::shared_ptr<OsmMapReader> reader;
  for (const auto& name : names)
  {
    LOG_VART(name);
    reader = Factory::getInstance().constructObject<OsmMapReader>(name);
    LOG_VART(url);
    LOG_VART(reader->isSupported(url));
    if (reader->isSupported(url))
    {
      LOG_TRACE("Supported reader: " << name << " for: " << url);
      return name;
    }
  }
  return "";
}

void OsmMapReaderFactory::read(const OsmMapPtr& map, const QString& url, bool useDataSourceIds, Status defaultStatus, bool cropOnReadIfBounded)
{
  LOG_INFO("Loading map from ..." << FileUtils::toLogFormat(url, 50) << "...");
  std::shared_ptr<OsmMapReader> reader = createReader(url, useDataSourceIds, defaultStatus, cropOnReadIfBounded);
  _read(map, reader, url);
}

void OsmMapReaderFactory::read(const OsmMapPtr& map, bool useDataSourceIds, bool useFileStatus, const QString& url, bool cropOnReadIfBounded)
{
  LOG_INFO("Loading map from " << FileUtils::toLogFormat(url, 50) << "...");
  LOG_VART(useFileStatus);
  std::shared_ptr<OsmMapReader> reader = createReader(useDataSourceIds, useFileStatus, url, cropOnReadIfBounded);
  _read(map, reader, url);
}

void OsmMapReaderFactory::_read(const OsmMapPtr& map, const std::shared_ptr<OsmMapReader>& reader, const QString& url)
{
  std::shared_ptr<Boundable> boundable = std::dynamic_pointer_cast<Boundable>(reader);
  if (!ConfigOptions().getBounds().trimmed().isEmpty() && !boundable.get())
  {
    throw IllegalArgumentException(
      QString("%1 configuration option used with unsupported reader for data source: %2").arg(ConfigOptions::getBoundsKey(), url));
  }

  QElapsedTimer timer;
  timer.start();

  reader->open(url);
  reader->read(map);
  if (map->size() == 0)
  {
    const QString msg = "No data in map: " + url + ".";
    if (ConfigOptions().getLogWarningsForEmptyInputMaps())
    {
      LOG_WARN(msg);
    }
    else
    {
      LOG_INFO(msg);
    }
  }
  VALIDATE(map->validate(true));
  LOG_STATUS(
    "Read " << StringUtils::formatLargeNumber(map->getElementCount()) <<
    " elements from input in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
}

}
