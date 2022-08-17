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
 * @copyright Copyright (C) 2015, 2017, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "TagFilteredDifferencer.h"

#include <hoot/core/schema/OsmSchema.h>

using namespace std;

namespace hoot
{

double TagFilteredDifferencer::diff(const ConstOsmMapPtr&, const ConstElementPtr& e1, const ConstElementPtr& e2) const
{
  const OsmSchema& schema = OsmSchema::getInstance();

  vector<SchemaVertex> v1 = schema.getUniqueSchemaVertices(e1->getTags());
  vector<SchemaVertex> v2 = schema.getUniqueSchemaVertices(e2->getTags());

  double result = 1;

  for (const auto& vertex1 : v1)
  {
    if (_isValidTag(vertex1))
    {
      for (const auto& vertex2 : v2)
      {
        if (_isValidTag(vertex2))
          result = min(result, 1 - schema.score(vertex1, vertex2));
      }
    }
  } 
  return result;
}

}
