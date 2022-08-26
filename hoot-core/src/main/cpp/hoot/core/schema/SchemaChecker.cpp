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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "SchemaChecker.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

int SchemaChecker::logWarnCount = 0;

SchemaChecker::SchemaChecker(const OsmSchema& osmSchema)
  : _schemaVertexList(osmSchema.getAllTags())
{
}

void SchemaChecker::checkUnknownVertexType() const
{
  for (const auto& schemaVertex : _schemaVertexList)
  {
    if (schemaVertex.isValid() == false)
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Warning: unknown type. " << schemaVertex.getName());
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
  }
}

void SchemaChecker::check() const
{
  checkUnknownVertexType();
}

}
