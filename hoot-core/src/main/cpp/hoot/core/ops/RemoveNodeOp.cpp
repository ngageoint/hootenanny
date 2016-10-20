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
#include "RemoveNodeOp.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/util/Validate.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RemoveNodeOp)

RemoveNodeOp::RemoveNodeOp(bool doCheck, bool removeFully):
  _doCheck(doCheck),
  _removeFully(removeFully)
{
}

RemoveNodeOp::RemoveNodeOp(long nId, bool doCheck, bool removeFully):
  _nodeIdToRemove(nId),
  _doCheck(doCheck),
  _removeFully(removeFully)
{
}

void RemoveNodeOp::_removeNodeNoCheck(shared_ptr<OsmMap>& map, long nId)
{
  map->_index->removeNode(map->getNode(_nodeIdToRemove));
  map->_nodes.erase(nId);
}

void RemoveNodeOp::_removeNode(shared_ptr<OsmMap>& map, long nId)
{
  const shared_ptr<NodeToWayMap>& n2w = map->getIndex().getNodeToWayMap();
  const set<long>& ways = n2w->getWaysByNode(nId);
  if (ways.size() > 0)
  {
    throw HootException("Removing a node, but it is still part of one or more ways.");
  }
  _removeNodeNoCheck(map, nId);
}

void RemoveNodeOp::_removeNodeFully(shared_ptr<OsmMap>& map, long nId)
{
  // copy the set because we may modify it later.
  set<long> rid = map->getIndex().getElementToRelationMap()->
      getRelationByElement(ElementId::way(nId));

  for (set<long>::const_iterator it = rid.begin(); it != rid.end(); ++it)
  {
    map->getRelation(*it)->removeElement(ElementId::node(nId));
  }

  const shared_ptr<NodeToWayMap>& n2w = map->getIndex().getNodeToWayMap();
  const set<long> ways = n2w->getWaysByNode(nId);

  for (set<long>::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    map->getWay(*it)->removeNode(nId);
  }

  _removeNodeNoCheck(map, nId);

  VALIDATE(map->validate());
}

void RemoveNodeOp::apply(shared_ptr<OsmMap>& map)
{
  if (_removeFully)
    _removeNodeFully(map, _nodeIdToRemove);
  else if (!_doCheck)
    _removeNodeNoCheck(map, _nodeIdToRemove);
  else
    _removeNode(map, _nodeIdToRemove);
}

} // end namespace hoot

