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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "WayUtils.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/NodeUtils.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/algorithms/Distance.h>
#include <hoot/core/algorithms/WayDiscretizer.h>
#include <hoot/core/algorithms/linearreference/LocationOfPoint.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/CollectionUtils.h>

// GEOS
#include <geos/geom/Coordinate.h>

// Std
#include <float.h>

namespace hoot
{

QString WayUtils::getWayNodesDetailedString(const ConstWayPtr& way, const ConstOsmMapPtr& map)
{
  return NodeUtils::nodeCoordsToString(NodeUtils::nodeIdsToNodes(way->getNodeIds(), map));
}

std::set<long> WayUtils::getContainingWayIdsByNodeId(const long nodeId,
                                                     const ConstOsmMapPtr& map,
                                                     const ElementCriterionPtr& wayCriterion)
{
  LOG_VART(nodeId);
  std::set<long> containingWayIds;

  const std::set<long>& idsOfWaysContainingNode =
    map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId);
  LOG_VART(idsOfWaysContainingNode);
  for (std::set<long>::const_iterator containingWaysItr = idsOfWaysContainingNode.begin();
       containingWaysItr != idsOfWaysContainingNode.end(); ++containingWaysItr)
  {
    const long containingWayId = *containingWaysItr;
    LOG_VART(containingWayId);
    //LOG_VART(map->getWay(containingWayId));

    if (wayCriterion)
    {
      LOG_VART(typeid(*wayCriterion).name());
      std::shared_ptr<ChainCriterion> chainCrit =
        std::dynamic_pointer_cast<ChainCriterion>(wayCriterion);
      if (chainCrit)
      {
        LOG_VART(chainCrit->toString());
      }
      LOG_VART(wayCriterion->isSatisfied(map->getWay(containingWayId)));
    }

    if (!wayCriterion || wayCriterion->isSatisfied(map->getWay(containingWayId)))
    {
      containingWayIds.insert(containingWayId);
    }
  }

  LOG_VART(containingWayIds);
  return containingWayIds;
}

std::vector<ConstWayPtr> WayUtils::getContainingWaysByNodeId(
  const long nodeId, const ConstOsmMapPtr& map,
  const ElementCriterionPtr& wayCriterion)
{
  std::vector<ConstWayPtr> containingWays;
  const std::set<long> containingWayIds = getContainingWayIdsByNodeId(nodeId, map, wayCriterion);
  for (std::set<long>::const_iterator containingWayIdsItr = containingWayIds.begin();
       containingWayIdsItr != containingWayIds.end(); ++containingWayIdsItr)
  {
    ConstWayPtr containingWay = map->getWay(*containingWayIdsItr);
    if (containingWay)
    {
      containingWays.push_back(containingWay);
    }
  }
  return containingWays;
}

QSet<long> WayUtils::getConnectedWays(const long wayId, const ConstOsmMapPtr& map)
{
  QSet<long> connectedWayIds;

  ConstWayPtr way = map->getWay(wayId);
  if (way)
  {
    const std::vector<long>& wayNodeIds = way->getNodeIds();
    for (size_t i = 0; i < wayNodeIds.size(); i++)
    {
      const long wayNodeId = wayNodeIds.at(i);
      const QSet<long>& idsOfWaysContainingNode =
        CollectionUtils::stdSetToQSet(map->getIndex().getNodeToWayMap()->getWaysByNode(wayNodeId));
      connectedWayIds = connectedWayIds.unite(idsOfWaysContainingNode);
    }
  }

  return connectedWayIds;
}

int WayUtils::getNumberOfConnectedWays(const long wayId, const ConstOsmMapPtr& map)
{
  return getConnectedWays(wayId, map).size();
}

bool WayUtils::hasConnectedWays(const long wayId, const ConstOsmMapPtr& map)
{
  return getNumberOfConnectedWays(wayId, map) > 0;
}

bool WayUtils::endWayNodeIsCloserToNodeThanStart(const ConstNodePtr& node,
                                                 const ConstWayPtr& way,
                                                 const ConstOsmMapPtr& map)
{
  if (way->isSimpleLoop())
  {
    return false;
  }
  const double distanceToStartNode =
    Distance::euclidean(node->toCoordinate(), map->getNode(way->getFirstNodeId())->toCoordinate());
  const double distanceToEndNode =
    Distance::euclidean(node->toCoordinate(), map->getNode(way->getLastNodeId())->toCoordinate());
  return distanceToEndNode < distanceToStartNode;
}

geos::geom::Coordinate WayUtils::closestWayCoordToNode(
  const ConstNodePtr& node, const ConstWayPtr& way, double& distance,
  const double discretizationSpacing, const ConstOsmMapPtr& map)
{
  // split the way up into coords
  std::vector<geos::geom::Coordinate> discretizedWayCoords;
  WayDiscretizer wayDiscretizer(map, way);
  if (!wayDiscretizer.discretize(discretizationSpacing, discretizedWayCoords))
  {
    return geos::geom::Coordinate();
  }

  // add the first and last coords in (one or both could already be there, but it won't hurt if
  // they're duplicated)
  ConstNodePtr firstNode = map->getNode(way->getFirstNodeId());
  ConstNodePtr lastNode = map->getNode(way->getLastNodeId());
  if (!firstNode || !lastNode)
  {
    return geos::geom::Coordinate();
  }
  discretizedWayCoords.insert(discretizedWayCoords.begin(), firstNode->toCoordinate());
  discretizedWayCoords.push_back(lastNode->toCoordinate());
  LOG_VART(discretizedWayCoords.size());
  LOG_VART(discretizedWayCoords);

  // determine which end of the way is closer to our input node (to handle ways looping back on
  // themselves)
  if (endWayNodeIsCloserToNodeThanStart(node, way, map))
  {
    std::reverse(discretizedWayCoords.begin(), discretizedWayCoords.end());
  }
  LOG_VART(discretizedWayCoords.size());

  // find the closest coord to the input node
  double shortestDistance = DBL_MAX;
  double lastDistance = DBL_MAX;
  geos::geom::Coordinate closestWayCoordToNode;
  for (size_t i = 0; i < discretizedWayCoords.size(); i++)
  {
    const geos::geom::Coordinate wayCoord = discretizedWayCoords[i];
    const double distanceBetweenNodeAndWayCoord = wayCoord.distance(node->toCoordinate());
    // Since we're going in node order and started at the closest end of the way, if we start
    // seeing larger distances, then we're done.
    if (distanceBetweenNodeAndWayCoord > lastDistance)
    {
      break;
    }
    if (distanceBetweenNodeAndWayCoord < shortestDistance)
    {
      closestWayCoordToNode = wayCoord;
      shortestDistance = distanceBetweenNodeAndWayCoord;
    }
  }
  distance = shortestDistance;

  LOG_VART(distance);
  LOG_VART(closestWayCoordToNode);

  return closestWayCoordToNode;
}

long WayUtils::closestWayNodeIdToNode(const ConstNodePtr& node, const ConstWayPtr& way,
                                      const ConstOsmMapPtr& map)
{
  double shortestDistance = DBL_MAX;
  long closestWayNodeId = 0;

  const std::vector<long>& wayNodeIds = way->getNodeIds();
  for (size_t i = 0; i < wayNodeIds.size(); i++)
  {
    ConstNodePtr wayNode = map->getNode(wayNodeIds[i]);
    const double distanceFromNodeToWayNode =
      Distance::euclidean(node->toCoordinate(), wayNode->toCoordinate());
    if (distanceFromNodeToWayNode < shortestDistance)
    {
      shortestDistance = distanceFromNodeToWayNode;
      closestWayNodeId = wayNode->getId();
    }
  }
  LOG_VART(shortestDistance);

  LOG_VART(closestWayNodeId);
  return closestWayNodeId;
}

long WayUtils::closestWayNodeIndexToNode(
  const ConstNodePtr& node, const ConstWayPtr& way, const ConstOsmMapPtr& map)
{
  LOG_VART(way->getNodeCount());

  const long nodeId = closestWayNodeIdToNode(node, way, map);
  LOG_VART(nodeId);
  const long index = way->getNodeIndex(nodeId);
  LOG_VART(index);
  return index;
}

long WayUtils::closestWayNodeInsertIndex(
  const ConstNodePtr& node, const ConstWayPtr& way, const ConstOsmMapPtr& map)
{
  // get the way location of the node
  const WayLocation nodeLoc = LocationOfPoint::locate(map, way, node->toCoordinate());
  if (!nodeLoc.isValid())
  {
    return -1;
  }

  // get the closest way node index to it and get its way location
  const long closestWayNodeIndex = closestWayNodeIndexToNode(node, way, map);
  LOG_VART(closestWayNodeIndex);
  if (closestWayNodeIndex == -1)
  {
    return -1;
  }
  ConstNodePtr closestWayNode = map->getNode(way->getNodeId(closestWayNodeIndex));
  LOG_VART(closestWayNode->getElementId());
  if (!closestWayNode)
  {
    return -1;
  }
  const WayLocation closestNodeLoc =
    LocationOfPoint::locate(map, way, closestWayNode->toCoordinate());
  if (!closestNodeLoc.isValid())
  {
    return -1;
  }

  // if the closest index way location is after the node's way location we want to insert before
  // the index, so return the index
  LOG_VART(closestNodeLoc >= nodeLoc);
  if (closestNodeLoc >= nodeLoc)
  {
    return closestWayNodeIndex;
  }
  else
  {
    // otherwise, if the closest index way location is before the node's way location we want to
    // insert after the index, so return the index + 1
    return closestWayNodeIndex + 1;
  }
}

bool WayUtils::nodesAreContainedInTheSameWay(const long nodeId1, const long nodeId2,
                                             const ConstOsmMapPtr& map)
{
  const std::set<long>& waysContainingNode1 =
    map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId1);
  LOG_VART(waysContainingNode1);

  const std::set<long>& waysContainingNode2 =
    map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId2);
  LOG_VART(waysContainingNode2);

  std::set<long> commonNodesBetweenWayGroups;
  std::set_intersection(
    waysContainingNode1.begin(), waysContainingNode1.end(),
    waysContainingNode2.begin(), waysContainingNode2.end(),
    std::inserter(commonNodesBetweenWayGroups, commonNodesBetweenWayGroups.begin()));
  LOG_VART(commonNodesBetweenWayGroups);

  return commonNodesBetweenWayGroups.size() != 0;
}

bool WayUtils::nodesAreContainedInTheExactSameWays(
  const long nodeId1, const long nodeId2, const ConstOsmMapPtr& map)
{
  const std::set<long>& waysContainingNode1 =
    map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId1);
  LOG_VART(waysContainingNode1);

  const std::set<long>& waysContainingNode2 =
    map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId2);
  LOG_VART(waysContainingNode2);

  return waysContainingNode1 == waysContainingNode2;
}

int WayUtils::getNumberOfWaysContainingNode(const long nodeId, const ConstOsmMapPtr& map)
{
  return map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId).size();
}

bool WayUtils::nodeContainedByAnyWay(const long nodeId, const ConstOsmMapPtr& map)
{
  return map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId).size() > 0;
}

bool WayUtils::nodeContainedByAnyWay(const long nodeId, const std::set<long> wayIds,
                                     const ConstOsmMapPtr& map)
{
  std::set<long> waysContainingNode = map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId);
  std::set<long> commonWayIds;
  std::set_intersection(
    waysContainingNode.begin(), waysContainingNode.end(), wayIds.begin(), wayIds.end(),
    std::inserter(commonWayIds, commonWayIds.begin()));
  return commonWayIds.size() > 0;
}

bool WayUtils::nodeContainedByMoreThanOneWay(const long nodeId, const ConstOsmMapPtr& map)
{
  return map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId).size() > 1;
}

std::set<long> WayUtils::getIntersectingWayIds(const long wayId, const OsmMapPtr& map)
{
  std::set<long> intersectingWayIds;
  ConstWayPtr way = map->getWay(wayId);
  if (way)
  {
    std::vector<long> nearbyWayIds = map->getIndex().findWays(way->getEnvelopeInternal(map));
    for (std::vector<long>::const_iterator it = nearbyWayIds.begin(); it != nearbyWayIds.end();
         ++it)
    {
      const long otherWayId = *it;
      ConstWayPtr otherWay = map->getWay(otherWayId);
      if (otherWay && way->hasSharedNode(*otherWay))
      {
        intersectingWayIds.insert(otherWayId);
      }
    }
  }
  return intersectingWayIds;
}

std::vector<WayPtr> WayUtils::getIntersectingWays(const long wayId, const OsmMapPtr& map)
{
  std::vector<WayPtr> intersectingWays;
  ConstWayPtr way = map->getWay(wayId);
  if (way)
  {
    std::vector<long> nearbyWayIds = map->getIndex().findWays(way->getEnvelopeInternal(map));
    for (std::vector<long>::const_iterator it = nearbyWayIds.begin(); it != nearbyWayIds.end();
         ++it)
    {
      const long otherWayId = *it;
      WayPtr otherWay = map->getWay(otherWayId);
      if (otherWay && way->hasSharedNode(*otherWay))
      {
        intersectingWays.push_back(otherWay);
      }
    }
  }
  return intersectingWays;
}

bool WayUtils::wayIntersectsWithWayHavingKvp(
  const long wayId, const QString& kvp, const OsmMapPtr& map)
{
  const std::set<long> intersectingWayIds = getIntersectingWayIds(wayId, map);
  for (std::set<long>::const_iterator it = intersectingWayIds.begin();
       it != intersectingWayIds.end(); ++it)
  {
    ConstWayPtr intersectingWay = map->getWay(*it);
    if (intersectingWay && intersectingWay->getTags().hasKvp(kvp))
    {
      return true;
    }
  }
  return false;
}

bool WayUtils::nodeContainedByWaySharingNodesWithAnotherWay(
  const long nodeId, const long wayId, const OsmMapPtr& map)
{
  ConstWayPtr way = map->getWay(wayId);
  if (!way)
  {
    return false;
  }

  const std::vector<ConstWayPtr> waysContainingNode =
    getContainingWaysByNodeId(nodeId, map);
  for (std::vector<ConstWayPtr>::const_iterator containingWayItr = waysContainingNode.begin();
       containingWayItr != waysContainingNode.end(); ++containingWayItr)
  {
    ConstWayPtr containingWay = *containingWayItr;
    if (containingWay)
    {
      if (containingWay->hasSharedNode(*way))
      {
        return true;
      }
    }
  }
  return false;
}

}
