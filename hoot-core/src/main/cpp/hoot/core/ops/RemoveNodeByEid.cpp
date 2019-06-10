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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RemoveNodeByEid.h"

// hoot
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/util/Validate.h>

using namespace std;

namespace hoot
{

RemoveNodeByEid::RemoveNodeByEid(bool doCheck, bool removeFully) :
_nodeIdToRemove(-std::numeric_limits<int>::max()),
_doCheck(doCheck),
_removeFully(removeFully),
_removeOnlyUnused(false)
{
}

RemoveNodeByEid::RemoveNodeByEid(long nId, bool doCheck, bool removeFully, bool removeOnlyUnused) :
_nodeIdToRemove(nId),
_doCheck(doCheck),
_removeFully(removeFully),
_removeOnlyUnused(removeOnlyUnused)
{
}

void RemoveNodeByEid::_removeNodeNoCheck(OsmMapPtr& map, long nId)
{
  map->_index->removeNode(map->getNode(_nodeIdToRemove));
  map->_nodes.erase(nId);
}

void RemoveNodeByEid::_removeNode(OsmMapPtr& map, long nId)
{
  const std::shared_ptr<NodeToWayMap>& n2w = map->getIndex().getNodeToWayMap();
  const set<long>& ways = n2w->getWaysByNode(nId);
  if (ways.size() > 0)
  {
    if (_removeOnlyUnused)
      return;
    else
      throw HootException("Removing a node, but it is still part of one or more ways.");
  }
  _removeNodeNoCheck(map, nId);
}

void RemoveNodeByEid::_removeNodeFully(OsmMapPtr& map, long nId)
{
  // copy the set because we may modify it later.
  set<long> rid = map->getIndex().getElementToRelationMap()->
      getRelationByElement(ElementId::way(nId));

  for (set<long>::const_iterator it = rid.begin(); it != rid.end(); ++it)
  {
    map->getRelation(*it)->removeElement(ElementId::node(nId));
  }

  const std::shared_ptr<NodeToWayMap>& n2w = map->getIndex().getNodeToWayMap();
  const set<long> ways = n2w->getWaysByNode(nId);

  for (set<long>::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    map->getWay(*it)->removeNode(nId);
  }

  _removeNodeNoCheck(map, nId);

  VALIDATE(map->validate());
}

void RemoveNodeByEid::apply(OsmMapPtr& map)
{
  if (_nodeIdToRemove == -std::numeric_limits<int>::max())
  {
    throw IllegalArgumentException("No node ID specified for RemoveNodeByEid.");
  }

  if (_removeFully)
    _removeNodeFully(map, _nodeIdToRemove);
  else if (!_doCheck)
    _removeNodeNoCheck(map, _nodeIdToRemove);
  else
    _removeNode(map, _nodeIdToRemove);
}

void RemoveNodeByEid::removeNode(OsmMapPtr map, long nId, bool removeOnlyUnused)
{
  RemoveNodeByEid nodeRemover(nId, true, false, removeOnlyUnused);
  nodeRemover.apply(map);
}

void RemoveNodeByEid::removeNodeNoCheck(OsmMapPtr map, long nId)
{
  RemoveNodeByEid nodeRemover(nId, false);
  nodeRemover.apply(map);
}

void RemoveNodeByEid::removeNodeFully(OsmMapPtr map, long nId)
{
  RemoveNodeByEid nodeRemover(nId, true, true);
  nodeRemover.apply(map);
}

}

