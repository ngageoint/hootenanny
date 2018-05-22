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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmMapReaderFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Validate.h>
#include <hoot/core/ops/Boundable.h>
#include <hoot/core/util/Log.h>

using namespace std;

namespace hoot
{

boost::shared_ptr<OsmMapReaderFactory> OsmMapReaderFactory::_theInstance;

OsmMapReaderFactory::OsmMapReaderFactory()
{
}

OsmMapReaderFactory& OsmMapReaderFactory::getInstance()
{
  if (!_theInstance.get())
  {
    _theInstance.reset(new OsmMapReaderFactory());
  }
  return *_theInstance;
}

bool OsmMapReaderFactory::hasElementInputStream(QString url)
{
  bool result = false;
  boost::shared_ptr<OsmMapReader> reader = createReader(url, true, Status::Unknown1);
  boost::shared_ptr<ElementInputStream> eis = boost::dynamic_pointer_cast<ElementInputStream>(reader);
  if (eis)
  {
    result = true;
  }

  return result;
}

bool OsmMapReaderFactory::hasPartialReader(QString url)
{
  bool result = false;
  boost::shared_ptr<OsmMapReader> reader = createReader(url, true, Status::Unknown1);
  boost::shared_ptr<PartialOsmMapReader> pr = boost::dynamic_pointer_cast<PartialOsmMapReader>(reader);
  if (pr)
  {
    result = true;
  }

  return result;
}

boost::shared_ptr<OsmMapReader> OsmMapReaderFactory::_createReader(const QString url)
{
  QString readerOverride = ConfigOptions().getOsmMapReaderFactoryReader();

  /// @todo hack - the OsmApiDbAwareHootApiDbReader should always be reading from hoot api
  /// databases, but by using the factory override during conflation it won't - see #781 for
  /// potential fix task
  if (readerOverride == "hoot::OsmApiDbAwareHootApiDbReader" && url.startsWith("osmapidb"))
  {
    readerOverride = "";
  }

  boost::shared_ptr<OsmMapReader> reader;
  if (readerOverride != "")
  {
    reader.reset(Factory::getInstance().constructObject<OsmMapReader>(readerOverride));
    LOG_DEBUG("Using reader: " << readerOverride);
  }

  vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(OsmMapReader::className());
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

  return reader;
}

boost::shared_ptr<OsmMapReader> OsmMapReaderFactory::createReader(QString url,
                                                                  bool useDataSourceIds,
                                                                  Status defaultStatus)
{
  LOG_VART(url);
  LOG_VART(useDataSourceIds);
  LOG_VART(defaultStatus);

  boost::shared_ptr<OsmMapReader> reader = _createReader(url);
  reader->setUseDataSourceIds(useDataSourceIds);
  reader->setDefaultStatus(defaultStatus);
  return reader;
}

boost::shared_ptr<OsmMapReader> OsmMapReaderFactory::createReader(bool useDataSourceIds,
                                                                  bool useFileStatus, QString url)
{
  LOG_VART(url);
  LOG_VART(useDataSourceIds);
  LOG_VART(useFileStatus);

  boost::shared_ptr<OsmMapReader> reader = _createReader(url);
  reader->setUseDataSourceIds(useDataSourceIds);
  reader->setUseFileStatus(useFileStatus);
  return reader;
}

QString OsmMapReaderFactory::getReaderName(const QString url)
{
  LOG_VARD(url);
  vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(OsmMapReader::className());
  LOG_VARD(names.size());
  boost::shared_ptr<OsmMapReader> writer;
  for (size_t i = 0; i < names.size(); i++)
  {
    const std::string name = names[i];
    LOG_VART(name);
    writer.reset(Factory::getInstance().constructObject<OsmMapReader>(name));
    if (writer->isSupported(url))
    {
      return QString::fromStdString(name);
    }
  }
  return "";
}

bool OsmMapReaderFactory::isSupportedFormat(const QString url)
{
  return !getReaderName(url).trimmed().isEmpty();
}

void OsmMapReaderFactory::read(boost::shared_ptr<OsmMap> map, QString url, bool useDataSourceIds,
                               Status defaultStatus)
{
  LOG_INFO("Loading map from " << url << "...");
  boost::shared_ptr<OsmMapReader> reader =
    getInstance().createReader(url, useDataSourceIds, defaultStatus);
  _read(map, reader, url);
}

void OsmMapReaderFactory::read(boost::shared_ptr<OsmMap> map, bool useDataSourceIds,
                               bool useFileStatus, QString url)
{
  LOG_INFO("Loading map from " << url << "...");
  boost::shared_ptr<OsmMapReader> reader =
    getInstance().createReader(url, useDataSourceIds, useFileStatus);
  _read(map, reader, url);
}

void OsmMapReaderFactory::_read(boost::shared_ptr<OsmMap> map,
                                boost::shared_ptr<OsmMapReader> reader, const QString url)
{
  boost::shared_ptr<Boundable> boundable = boost::dynamic_pointer_cast<Boundable>(reader);
  if (!ConfigOptions().getConvertBoundingBox().trimmed().isEmpty() && !boundable.get())
  {
    throw IllegalArgumentException(
      ConfigOptions::getConvertBoundingBoxKey() +
      " configuration option used with unsupported reader for data source: " + url);
  }
  reader->open(url);
  reader->read(map);
  VALIDATE(map->validate(true));
}

}
