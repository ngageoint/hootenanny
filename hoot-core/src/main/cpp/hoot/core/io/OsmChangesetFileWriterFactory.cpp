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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "OsmChangesetFileWriterFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/OsmChangesetFileWriter.h>
#include <hoot/core/io/OsmApiDbSqlChangesetFileWriter.h>
#include <hoot/core/util/ConfigOptions.h>


namespace hoot
{

OsmChangesetFileWriterFactory& OsmChangesetFileWriterFactory::getInstance()
{
  //  Local static singleton instance
  static OsmChangesetFileWriterFactory instance;
  return instance;
}

std::shared_ptr<OsmChangesetFileWriter> OsmChangesetFileWriterFactory::createWriter(
  const QString& url, const QString& osmApiDbUrl) const
{
  LOG_VARD(url);
  LOG_VARD(osmApiDbUrl);

  std::vector<QString> names =
    Factory::getInstance().getObjectNamesByBase(OsmChangesetFileWriter::className());
  std::shared_ptr<OsmChangesetFileWriter> writer;
  for (size_t i = 0; i < names.size() && !writer; ++i)
  {
    LOG_VARD(names[i]);
    writer = Factory::getInstance().constructObject<OsmChangesetFileWriter>(names[i]);
    if (writer->isSupported(url))
    { LOG_DEBUG("Using changeset output writer: " << names[i]);

      Configurable* c = dynamic_cast<Configurable*>(writer.get());
      if (c != nullptr)
      {
        c->setConfiguration(conf());
      }

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
    else
    {
      writer.reset();
    }
  }

  if (!writer)
  {
    throw IllegalArgumentException("A valid writer could not be found for the URL: " + url);
  }

  return writer;
}

}
