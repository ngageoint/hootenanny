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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "OsmChangeWriterFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/OsmChangeWriter.h>
#include <hoot/core/util/ConfigOptions.h>

using namespace std;

namespace hoot
{

OsmChangeWriterFactory& OsmChangeWriterFactory::getInstance()
{
  //  Local static singleton instance
  static OsmChangeWriterFactory instance;
  return instance;
}

std::shared_ptr<OsmChangeWriter> OsmChangeWriterFactory::createWriter(
  const QString& url, const QString& elementPayloadFormat) const
{
  LOG_VART(url);
  LOG_VART(elementPayloadFormat);

  vector<QString> names =
    Factory::getInstance().getObjectNamesByBase(OsmChangeWriter::className());
  std::shared_ptr<OsmChangeWriter> writer;
  for (size_t i = 0; i < names.size() && !writer; ++i)
  {
    LOG_VART(names[i]);
    writer.reset(Factory::getInstance().constructObject<OsmChangeWriter>(names[i]));
    if (writer->isSupported(url))
    {
      writer->setElementPayloadFormat(elementPayloadFormat);
      LOG_DEBUG("Using changeset output writer: " << names[i]);
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

bool OsmChangeWriterFactory::isSupported(const QString& output) const
{
  return output.endsWith(".xml") || output.endsWith(".json");
}

}
