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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "RemoveRelationByEid.h"

// hoot
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/Validate.h>

using namespace std;

namespace hoot
{

RemoveRelationByEid::RemoveRelationByEid()
  : _rIdToRemove(-std::numeric_limits<int>::max())
{
}

RemoveRelationByEid::RemoveRelationByEid(long rId)
  : _rIdToRemove(rId)
{
}

void RemoveRelationByEid::apply(OsmMapPtr& map)
{
  if (_rIdToRemove == -std::numeric_limits<int>::max())
    throw IllegalArgumentException("No relation ID specified for RemoveRelationByEid.");

  if (map->_relations.find(_rIdToRemove) != map->_relations.end())
  {
    // determine if this relation is a part of any other relations
    // make a copy of the rids in case the index gets changed.
    const set<long> rids =
      map->_index->getElementToRelationMap()->getRelationByElement(ElementId::relation(_rIdToRemove));

    // remove this relation from all other parent relations.
    for (auto parentRelationId : rids)
    {
      LOG_TRACE("Removing relation: " << _rIdToRemove << " from relation: " << parentRelationId);
      map->getRelation(parentRelationId)->removeElement(ElementId::relation(_rIdToRemove));
    }

    LOG_TRACE("Removing relation: " << _rIdToRemove);
    map->_index->removeRelation(map->getRelation(_rIdToRemove));
    map->_relations.erase(_rIdToRemove);
    VALIDATE(map->validate());
  }
}

void RemoveRelationByEid::removeRelation(OsmMapPtr map, long rId)
{
  RemoveRelationByEid relationRemover(rId);
  relationRemover.apply(map);
}

}

