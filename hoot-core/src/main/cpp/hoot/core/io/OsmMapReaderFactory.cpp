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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmMapReaderFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Validate.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

bool OsmMapReaderFactory::hasElementInputStream(const QString& url)
{
  bool result = false;
  std::shared_ptr<OsmMapReader> reader = createReader(url, true, Status::Unknown1);
  std::shared_ptr<ElementInputStream> eis =
    std::dynamic_pointer_cast<ElementInputStream>(reader);
  if (eis)
  {
    result = true;
  }

  return result;
}

bool OsmMapReaderFactory::hasElementInputStream(const QStringList& inputs)
{
  for (int i = 0; i < inputs.size(); i++)
  {
    if (!hasElementInputStream(inputs.at(i)))
    {
      LOG_INFO(
        "Unable to stream Inputs due to input: " << inputs.at(i).right(25) << ". Loading entire map...");
      return false;
    }
  }
  return true;
}

bool OsmMapReaderFactory::hasPartialReader(const QString& url)
{
  bool result = false;
  std::shared_ptr<OsmMapReader> reader = createReader(url, true, Status::Unknown1);
  std::shared_ptr<PartialOsmMapReader> pr =
    std::dynamic_pointer_cast<PartialOsmMapReader>(reader);
  if (pr)
  {
    result = true;
  }

  return result;
}

std::shared_ptr<OsmMapReader> OsmMapReaderFactory::_createReader(const QString& url)
{
  QString readerOverride = ConfigOptions().getMapFactoryReader();

  std::shared_ptr<OsmMapReader> reader;
  if (readerOverride != "")
  {
    reader.reset(Factory::getInstance().constructObject<OsmMapReader>(readerOverride));
    LOG_DEBUG("Using reader: " << readerOverride);
  }

  vector<QString> names = Factory::getInstance().getObjectNamesByBase(OsmMapReader::className());
  for (size_t i = 0; i < names.size() && !reader; ++i)
  {
    LOG_TRACE("Checking input " << url << " with reader " << names[i]);
    reader.reset(Factory::getInstance().constructObject<OsmMapReader>(names[i]));
    if (reader->isSupported(url))
    {
      LOG_DEBUG("Using input reader: " << names[i]);
    }
    else
    {
      reader.reset();
    }
  }

  if (!reader)
  {
    throw HootException("A valid reader could not be found for the URL: " + url);
  }

  reader->setConfiguration(conf());

  return reader;
}

std::shared_ptr<OsmMapReader> OsmMapReaderFactory::createReader(const QString& url,
                                                                bool useDataSourceIds,
                                                                Status defaultStatus)
{
  LOG_VART(url);
  LOG_VART(useDataSourceIds);
  LOG_VART(defaultStatus);

  std::shared_ptr<OsmMapReader> reader = _createReader(url);
  reader->setUseDataSourceIds(useDataSourceIds);
  reader->setDefaultStatus(defaultStatus);
  return reader;
}

std::shared_ptr<OsmMapReader> OsmMapReaderFactory::createReader(bool useDataSourceIds,
                                                                bool useFileStatus,
                                                                const QString& url)
{
  LOG_VART(url);
  LOG_VART(useDataSourceIds);
  LOG_VART(useFileStatus);

  std::shared_ptr<OsmMapReader> reader = _createReader(url);
  reader->setUseDataSourceIds(useDataSourceIds);
  reader->setUseFileStatus(useFileStatus);
  return reader;
}

QString OsmMapReaderFactory::getReaderName(const QString& url)
{
  LOG_VARD(url);
  vector<QString> names = Factory::getInstance().getObjectNamesByBase(OsmMapReader::className());
  LOG_VARD(names.size());
  std::shared_ptr<OsmMapReader> writer;
  for (size_t i = 0; i < names.size(); i++)
  {
    const QString name = names[i];
    LOG_VART(name);
    writer.reset(Factory::getInstance().constructObject<OsmMapReader>(name));
    LOG_VART(url);
    LOG_VART(writer->isSupported(url));
    if (writer->isSupported(url))
    {
      return name;
    }
  }
  return "";
}

bool OsmMapReaderFactory::isSupportedFormat(const QString& url)
{
  return !getReaderName(url).trimmed().isEmpty();
}

void OsmMapReaderFactory::read(const OsmMapPtr& map, const QString& url, bool useDataSourceIds,
                               Status defaultStatus)
{
  LOG_INFO("Loading map from ..." << url.right(50) << "...");
  std::shared_ptr<OsmMapReader> reader = createReader(url, useDataSourceIds, defaultStatus);
  _read(map, reader, url);
}

void OsmMapReaderFactory::read(const OsmMapPtr& map, bool useDataSourceIds,
                               bool useFileStatus, const QString& url)
{
  LOG_INFO("Loading map from " << url.right(50) << "...");
  std::shared_ptr<OsmMapReader> reader = createReader(url, useDataSourceIds, useFileStatus);
  _read(map, reader, url);
}

void OsmMapReaderFactory::_read(const OsmMapPtr& map,
                                const std::shared_ptr<OsmMapReader>& reader, const QString& url)
{
  std::shared_ptr<Boundable> boundable = std::dynamic_pointer_cast<Boundable>(reader);
  if (!ConfigOptions().getBounds().trimmed().isEmpty() && !boundable.get())
  {
    throw IllegalArgumentException(
      ConfigOptions::getBoundsKey() +
      " configuration option used with unsupported reader for data source: " + url);
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
  LOG_INFO(
    "Read " << StringUtils::formatLargeNumber(map->getElementCount()) <<
    " elements from input in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
}

}
