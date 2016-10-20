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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RemoveWayOp.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/util/Validate.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RemoveWayOp)

RemoveWayOp::RemoveWayOp(bool removeFully):
  _removeFully(removeFully)
{
}

RemoveWayOp::RemoveWayOp(long wId, bool removeFully):
  _wayIdToRemove(wId),
  _removeFully(removeFully)
{
}

void RemoveWayOp::_removeWay(shared_ptr<OsmMap> &map, long wId)
{
  if (map->_ways.find(wId) != map->_ways.end())
  {
    map->_index->removeWay(map->getWay(wId));
    map->_ways.erase(wId);
  }
}

void RemoveWayOp::_removeWayFully(shared_ptr<OsmMap> &map, long wId)
{
  // copy the set because we may modify it later.
  set<long> rid = map->_index->getElementToRelationMap()->
      getRelationByElement(ElementId::way(wId));

  for (set<long>::const_iterator it = rid.begin(); it != rid.end(); ++it)
  {
    map->getRelation(*it)->removeElement(ElementId::way(wId));
  }
  _removeWay(map, wId);
  VALIDATE(map->validate());
}

void RemoveWayOp::apply(shared_ptr<OsmMap>& map)
{
  if (_removeFully)
    _removeWayFully(map, _wayIdToRemove);
  else
    _removeWay(map, _wayIdToRemove);
}

} // end namespace hoot
