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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmMapWriterFactory.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/io/OsmMapWriter.h>
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/io/ElementOutputStream.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

shared_ptr<OsmMapWriterFactory> OsmMapWriterFactory::_theInstance;

OsmMapWriterFactory::OsmMapWriterFactory()
{
}

OsmMapWriterFactory& OsmMapWriterFactory::getInstance()
{
  if (!_theInstance.get())
  {
    _theInstance.reset(new OsmMapWriterFactory());
  }
  return *_theInstance;
}

shared_ptr<OsmMapWriter> OsmMapWriterFactory::createWriter(QString url)
{
  QString writerOverride = ConfigOptions().getOsmMapWriterFactoryWriter();

  shared_ptr<OsmMapWriter> writer;
  if (writerOverride != "" && url != ConfigOptions().getDebugMapFilename())
  {
    writer.reset(Factory::getInstance().constructObject<OsmMapWriter>(writerOverride));
  }

  vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(OsmMapWriter::className());
  for (size_t i = 0; i < names.size() && !writer; ++i)
  {
    writer.reset(Factory::getInstance().constructObject<OsmMapWriter>(names[i]));
    if (writer->isSupported(url))
    {
      LOG_DEBUG("Using writer: " << names[i]);
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

bool OsmMapWriterFactory::hasElementOutputStream(QString url)
{
  bool result = false;
  shared_ptr<OsmMapWriter> writer = createWriter(url);
  shared_ptr<ElementOutputStream> streamWriter = dynamic_pointer_cast<ElementOutputStream>(writer);
  if (streamWriter)
  {
    result = true;
  }

  return result;
}


bool OsmMapWriterFactory::hasPartialWriter(QString url)
{
  bool result = false;
  shared_ptr<OsmMapWriter> writer = createWriter(url);
  shared_ptr<PartialOsmMapWriter> streamWriter = dynamic_pointer_cast<PartialOsmMapWriter>(writer);
  if (streamWriter)
  {
    result = true;
  }

  return result;
}

bool OsmMapWriterFactory::hasWriter(QString url)
{
  vector<std::string> names = Factory::getInstance().getObjectNamesByBase(
    OsmMapWriter::className());

  bool result = false;
  for (size_t i = 0; i < names.size() && !result; ++i)
  {
    shared_ptr<OsmMapWriter> writer(Factory::getInstance().constructObject<OsmMapWriter>(names[i]));
    if (writer->isSupported(url))
    {
      result = true;
    }
  }

  return result;
}

void OsmMapWriterFactory::write(const shared_ptr<const OsmMap>& map, QString url)
{
  shared_ptr<OsmMapWriter> writer = getInstance().createWriter(url);
  writer->open(url);
  writer->write(map);
}


}
