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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "ReuseNodeIdsOnWayOp.h"

// hoot
#include <hoot/core/conflate/IdSwap.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/IdSwapOp.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, ReuseNodeIdsOnWayOp)

ReuseNodeIdsOnWayOp::ReuseNodeIdsOnWayOp(ElementId from, ElementId to)
  : _from(from),
    _to(to)
{
}

void ReuseNodeIdsOnWayOp::addElement(const ConstElementPtr& e)
{
  if (_from.isNull())
    _from = e->getElementId();
  else if (_to.isNull())
    _to = e->getElementId();
  else
    throw IllegalArgumentException("Error adding element. Only two elements can be added 'from' and 'to'.");
}

void ReuseNodeIdsOnWayOp::apply(const std::shared_ptr<OsmMap>& map)
{
  //  There must be two ways to work with
  if (_from.isNull() || _to.isNull())
    throw IllegalArgumentException("You must specify a valid 'from' and 'to' element ID.");
  //  Check both from and to and make sure that they are in the map
  if (!map->containsElement(_from))
  {
    LOG_TRACE(_from << " doesn't exist in map.");
    return;
  }
  if (!map->containsElement(_to))
  {
    LOG_TRACE(_to << " doesn't exist in map.");
    return;
  }
  //  Both elements must be ways in order to reuse node IDs
  if (_from.getType() != ElementType::Way)
  {
    LOG_TRACE(_from << " isn't a way.");
    return;
  }
  if (_to.getType() != ElementType::Way)
  {
    LOG_TRACE(_to << " isn't a way.");
    return;
  }
  //  Don't try to reuse node IDs from UNKNOWN2 because UNKNOWN1 node IDs take precident
  WayPtr from = map->getWay(_from);
  WayPtr to = map->getWay(_to);
  if (from->getStatus() != Status::Unknown1)
  {
    LOG_TRACE(from << " has status " << from->getStatus());
    return;
  }
  //  By now "to" should have been set to conflated
  if (to->getStatus() != Status::Conflated)
  {
    LOG_TRACE(to << " has status " << to->getStatus());
    return;
  }
  LOG_TRACE("Replacing node IDs in " << from << " with " << to << "...");
  //  Only replace node IDs of non-shared nodes
  std::shared_ptr<NodeToWayMap> nodeToWayMap = map->getIndex().getNodeToWayMap();
  //  Swap all of the node IDs at once
  std::shared_ptr<IdSwap> swap = std::make_shared<IdSwap>();
  //  For closed ways the use the unique node count
  size_t toNodeCount = to->getNodeCount();
  if (toNodeCount > 1 && to->getNodeId(0) == to->getNodeId(static_cast<int>(toNodeCount) - 1))
    toNodeCount--;
  size_t fromNodeCount = from->getNodeCount();
  if (fromNodeCount > 1 && from->getNodeId(0) == from->getNodeId(static_cast<int>(fromNodeCount) - 1))
    fromNodeCount--;
  size_t toIndex = 0;
  //  Iterate all of the "from" way's node IDs
  for (size_t fromIndex = 0; fromIndex < fromNodeCount; ++fromIndex)
  {
    //  Make sure not to iterate off of the end of the "to" way
    if (toIndex >= toNodeCount)
      break;
    long fromNodeId = from->getNodeId(static_cast<int>(fromIndex));
    //  Skip any shared node in the from way
    if (nodeToWayMap->getWaysByNode(fromNodeId).size() == 1)
    {
      long toNodeId = to->getNodeId(static_cast<int>(toIndex));
      //  Skip any shared node in the to way
      while (nodeToWayMap->getWaysByNode(toNodeId).size() > 1 && toIndex < toNodeCount)
      {
        toIndex++;
        toNodeId = to->getNodeId(toIndex);
      }
      //  Make sure that both the to and from node IDs are valid before modifying the nodes
      if (toIndex < toNodeCount)
      {
        swap->add(ElementId(ElementType::Node, fromNodeId), ElementId(ElementType::Node, toNodeId));
        toIndex++;
      }
    }
  }
  //  Setup the ID swap so that all the swaps happen together
  IdSwapOp(swap).apply(map);
}

}
