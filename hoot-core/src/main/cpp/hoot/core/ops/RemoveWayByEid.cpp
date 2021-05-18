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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "RemoveWayByEid.h"

// hoot
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/util/Validate.h>

using namespace std;

namespace hoot
{

RemoveWayByEid::RemoveWayByEid(bool removeFully) :
_wayIdToRemove(-std::numeric_limits<int>::max()),
_removeFully(removeFully)
{
}

RemoveWayByEid::RemoveWayByEid(long wId, bool removeFully) :
_wayIdToRemove(wId),
_removeFully(removeFully)
{
}

void RemoveWayByEid::_removeWay(OsmMapPtr& map, long wId) const
{
  if (map->_ways.find(wId) != map->_ways.end())
  {
    LOG_TRACE("Removing way: " << ElementId::way(wId) << "...");
    map->_index->removeWay(map->getWay(wId));
    map->_ways.erase(wId);

    LOG_VART(map->_ways.find(wId) == map->_ways.end());
    LOG_VART(
      map->_index->getElementToRelationMap()->getRelationByElement(ElementId::way(wId)).size());
  }
}

void RemoveWayByEid::_removeWayFully(OsmMapPtr& map, long wId)
{
  // copy the set because we may modify it later.
  set<long> rid =
    map->_index->getElementToRelationMap()->getRelationByElement(ElementId::way(wId));
  for (set<long>::const_iterator it = rid.begin(); it != rid.end(); ++it)
  {
    const long relationId = *it;
    LOG_TRACE("Removing way: " << ElementId::way(wId) << " from relation: " << relationId << "...");
    map->getRelation(relationId)->removeElement(ElementId::way(wId));
  }
  _removeWay(map, wId);
  VALIDATE(map->validate());
}

void RemoveWayByEid::apply(OsmMapPtr& map)
{
  if (_wayIdToRemove == -std::numeric_limits<int>::max())
  {
    throw IllegalArgumentException("No way ID specified for RemoveWayByEid.");
  }

  if (_removeFully)
    _removeWayFully(map, _wayIdToRemove);
  else
    _removeWay(map, _wayIdToRemove);
}

void RemoveWayByEid::removeWay(OsmMapPtr map, long wId)
{
  LOG_VART(wId);
  RemoveWayByEid wayRemover(wId);
  wayRemover.apply(map);
}

void RemoveWayByEid::removeWayFully(OsmMapPtr map, long wId)
{
  LOG_VART(wId);
  RemoveWayByEid wayRemover(wId, true);
  wayRemover.apply(map);
}

}
