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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmChangesetFileWriterFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/OsmChangesetFileWriter.h>
#include <hoot/core/io/OsmApiDbSqlChangesetFileWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

std::shared_ptr<OsmChangesetFileWriterFactory> OsmChangesetFileWriterFactory::_theInstance;

OsmChangesetFileWriterFactory::OsmChangesetFileWriterFactory()
{
}

OsmChangesetFileWriterFactory& OsmChangesetFileWriterFactory::getInstance()
{
  if (!_theInstance.get())
  {
    _theInstance.reset(new OsmChangesetFileWriterFactory());
  }
  return *_theInstance;
}

std::shared_ptr<OsmChangesetFileWriter> OsmChangesetFileWriterFactory::createWriter(
  const QString& url, const QString& osmApiDbUrl)
{
  LOG_VART(url);

  std::vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(OsmChangesetFileWriter::className());
  std::shared_ptr<OsmChangesetFileWriter> writer;
  for (size_t i = 0; i < names.size() && !writer; ++i)
  {
    LOG_VART(names[i]);
    writer.reset(Factory::getInstance().constructObject<OsmChangesetFileWriter>(names[i]));
    if (writer->isSupported(url))
    {
      LOG_DEBUG("Using changeset output writer: " << names[i]);
      if (url.endsWith(".osc.sql"))
      {
        if (osmApiDbUrl.isEmpty())
        {
          throw IllegalArgumentException(
            "No OSM API database URL specified for OsmApiDbSqlChangesetFileWriter input.");
        }
        else
        {
          std::shared_ptr<OsmApiDbSqlChangesetFileWriter> dbWriter =
            std::dynamic_pointer_cast<OsmApiDbSqlChangesetFileWriter>(writer);
          assert(dbWriter.get());
          dbWriter->setOsmApiDbUrl(osmApiDbUrl);
        }
      }
    }
  }

  if (!writer)
  {
    throw IllegalArgumentException("A valid writer could not be found for the URL: " + url);
  }

  return writer;
}

}
