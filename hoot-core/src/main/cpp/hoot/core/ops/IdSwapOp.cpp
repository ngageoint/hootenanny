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
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "IdSwapOp.h"

//  Hoot
#include <hoot/core/conflate/IdSwap.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, IdSwapOp)

IdSwapOp::IdSwapOp(const IdSwapPtr& idSwap) :
_idSwap(idSwap)
{
}

IdSwapOp::IdSwapOp(ElementId e1, ElementId e2) :
_idSwap(std::make_shared<IdSwap>(e1, e2))
{
}

void IdSwapOp::apply(const std::shared_ptr<hoot::OsmMap>& map)
{
  //  Validate the swap information
  IdSwapPtr swap;
  if (_idSwap)
    swap = _idSwap;
  else if (map->getIdSwap())
    swap = map->getIdSwap();
  else
    return;
  //  Iterate all of the entries to swap
  for (IdSwap::iterator it = swap->begin(); it != swap->end(); ++it)
  {
    ElementPtr element1 = map->getElement(it->first);
    ElementPtr element2 = map->getElement(it->second);
    //  All matches are processed but not all are affected
    _numProcessed++;
    //  Can't swap non-existant elements
    if (!element1 || !element2)
      continue;
    //  Don't swap elements that aren't the same type
    if (element1->getElementType() != element2->getElementType())
      continue;
    //  Each type of element potentially has a something extra to swap
    //  Both are ensured to have the same type at this point
    if (element1->getElementType() == ElementType::Way)
    {
      //  This is tricky because element1a has replaced element1 as the actual object
      //  Create a copy of element elements
      ElementPtr element1a(element1->clone());
      ElementPtr element2a(element2->clone());
      //  Replace element 2 with an empty ID temporarily
      element2a->setId(0);
      map->replace(element2, element2a);
      //  Replace element 1 with element 2's ID
      element1a->setId(element2->getId());
      map->replace(element1, element1a);
      //  Replace element 2a with element 1's ID
      element2->setId(element1->getId());
      map->replace(element2a, element2);
      //  Swap the PIDs also
      WayPtr way1 = std::dynamic_pointer_cast<Way>(element1a);
      WayPtr way2 = std::dynamic_pointer_cast<Way>(element2);
      long temp = way1->getPid();
      way1->setPid(way2->getPid());
      way2->setPid(temp);
    }
    else if (element1->getElementType() == ElementType::Node)
    {
      //  Nodes cannot use OsmMap::replace() or OsmMap::replaceNode()
      //  because it deletes the node being replaced
      NodePtr node1 = std::dynamic_pointer_cast<Node>(element1);
      NodePtr node2 = std::dynamic_pointer_cast<Node>(element2);
      //  Remove the nodes from the map index but don't remove them fully
      RemoveNodeByEid(node1->getId(), false).apply(map);
      RemoveNodeByEid(node2->getId(), false).apply(map);
      //  Swap the node IDs in the nodes themselves
      long nodeId1 = node1->getId();
      long nodeId2 = node2->getId();
      node1->setId(0);
      node2->setId(nodeId1);
      node1->setId(nodeId2);
      //  Add the new swapped nodes back to the map
      map->addNode(node1);
      map->addNode(node2);
      //  Swap the node IDs in any way that they are associated with
      swapNodeIdInWay(map, nodeId1, 0);
      swapNodeIdInWay(map, nodeId2, nodeId1);
      swapNodeIdInWay(map, 0, nodeId2);
    }
    //  Update the number affected
    _numAffected++;
  }
}

void IdSwapOp::swapNodeIdInWay(const std::shared_ptr<OsmMap>& map, long nodeId, long swapId) const
{
  std::shared_ptr<NodeToWayMap> nodeToWayMap = map->getIndex().getNodeToWayMap();
  std::set<long> ways = nodeToWayMap->getWaysByNode(nodeId);
  //  Iterate all of the ways that contain this node
  for (std::set<long>::iterator it = ways.begin(); it != ways.end(); ++it)
  {
    long wayId = *it;
    WayPtr way = map->getWay(wayId);
    if (!way)
      continue;
    //  Get a list of all nodes in the way
    std::vector<long> nodes = way->getNodeIds();
    //  Iterate all nodes in the way replacing nodeId with swapId as many times as it occurs
    for (size_t i = 0; i < nodes.size(); i++)
    {
      if (nodes[i] == nodeId)
        nodes[i] = swapId;
    }
    //  Replace the vector of node IDs in the way with the new list
    way->setNodes(nodes);
  }
}

}
