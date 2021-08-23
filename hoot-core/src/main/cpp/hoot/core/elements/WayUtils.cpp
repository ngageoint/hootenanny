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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "WayUtils.h"

// Hoot
#include <hoot/core/algorithms/Distance.h>
#include <hoot/core/algorithms/WayDiscretizer.h>
#include <hoot/core/algorithms/linearreference/LocationOfPoint.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/NodeUtils.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/util/HootException.h>


// GEOS


// Std
#include <float.h>

namespace hoot
{

QString WayUtils::getWayNodesDetailedString(const ConstWayPtr& way, const ConstOsmMapPtr& map)
{
  return NodeUtils::nodeCoordsToString(NodeUtils::nodeIdsToNodes(way->getNodeIds(), map));
}

std::set<long> WayUtils::getContainingWayIds(
  const long nodeId, const ConstOsmMapPtr& map, const ElementCriterionPtr& wayCriterion)
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

std::vector<ConstWayPtr> WayUtils::getContainingWaysConst(
  const long nodeId, const ConstOsmMapPtr& map,
  const ElementCriterionPtr& wayCriterion)
{
  std::vector<ConstWayPtr> containingWays;
  const std::set<long> containingWayIds = getContainingWayIds(nodeId, map, wayCriterion);
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

std::vector<WayPtr> WayUtils::getContainingWays(
  const long nodeId, const ConstOsmMapPtr& map,
  const ElementCriterionPtr& wayCriterion)
{
  std::vector<WayPtr> waysToReturn;
  std::vector<ConstWayPtr> ways = getContainingWaysConst(nodeId, map, wayCriterion);
  for (std::vector<ConstWayPtr>::const_iterator itr = ways.begin(); itr != ways.end(); ++itr)
  {
    waysToReturn.push_back(std::const_pointer_cast<Way>(*itr));
  }
  return waysToReturn;
}

std::set<QString> WayUtils::getContainingWaysMostSpecificTypes(
  const long nodeId, const ConstOsmMapPtr& map)
{
  std::set<QString> uniqueTypes;
  std::vector<ConstWayPtr> containingWays = getContainingWaysConst(nodeId, map);
  std::sort(containingWays.begin(), containingWays.end());
  OsmSchema& schema = OsmSchema::getInstance();
  for (std::vector<ConstWayPtr>::const_iterator containingWaysItr = containingWays.begin();
       containingWaysItr != containingWays.end(); ++containingWaysItr)
  {
    const QString mostSpecificType = schema.mostSpecificType((*containingWaysItr)->getTags());
    if (!mostSpecificType.isEmpty())
    {
      uniqueTypes.insert(mostSpecificType);
    }
  }
  return uniqueTypes;
}

std::set<QString> WayUtils::getContainingWaysMostSpecificTypeKeys(
  const long nodeId, const ConstOsmMapPtr& map)
{
  std::set<QString> uniqueTypeKeys;
  const std::set<QString> uniqueTypes = getContainingWaysMostSpecificTypes(nodeId, map);
  for (std::set<QString>::const_iterator uniqueTypesItr = uniqueTypes.begin();
       uniqueTypesItr != uniqueTypes.end(); ++uniqueTypesItr)
  {
    uniqueTypeKeys.insert(Tags::kvpToKey(*uniqueTypesItr));
  }
  return uniqueTypeKeys;
}

QSet<long> WayUtils::getConnectedWays(const long wayId, const ConstOsmMapPtr& map)
{
  QSet<long> connectedWayIds;

  ConstWayPtr way = map->getWay(wayId);
  if (way)
  {
    LOG_VART(way->getElementId());
    const std::vector<long>& wayNodeIds = way->getNodeIds();
    for (size_t i = 0; i < wayNodeIds.size(); i++)
    {
      const long wayNodeId = wayNodeIds.at(i);
      LOG_VART(wayNodeId);
      const QSet<long>& idsOfWaysContainingNode =
        CollectionUtils::stdSetToQSet(map->getIndex().getNodeToWayMap()->getWaysByNode(wayNodeId));
      connectedWayIds.unite(idsOfWaysContainingNode);
    }
  }

  // Don't include the way we're examining.
  connectedWayIds.remove(wayId);
  LOG_VART(connectedWayIds);
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

bool WayUtils::endWayNodeIsCloserToNodeThanStart(
  const ConstNodePtr& node, const ConstWayPtr& way, const ConstOsmMapPtr& map)
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

  // Add the first and last coords in. One or both could already be there, but it won't hurt if
  // they're duplicated.
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

long WayUtils::closestWayNodeIdToNode(
  const ConstNodePtr& node, const ConstWayPtr& way, const ConstOsmMapPtr& map)
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

  // If the closest index way location is after the node's way location we want to insert before
  // the index, so return the index.
  LOG_VART(closestNodeLoc >= nodeLoc);
  if (closestNodeLoc >= nodeLoc)
  {
    return closestWayNodeIndex;
  }
  else
  {
    // Otherwise, if the closest index way location is before the node's way location we want to
    // insert after the index, so return the index + 1.
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

  return !commonNodesBetweenWayGroups.empty();
}

bool WayUtils::nodeContainedByAnyWay(const long nodeId, const ConstOsmMapPtr& map)
{
  return !map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId).empty();
}

bool WayUtils::nodeContainedByAnyWay(const long nodeId, const std::set<long>& wayIds,
                                     const ConstOsmMapPtr& map)
{
  std::set<long> waysContainingNode = map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId);
  std::set<long> commonWayIds;
  std::set_intersection(
    waysContainingNode.begin(), waysContainingNode.end(), wayIds.begin(), wayIds.end(),
    std::inserter(commonWayIds, commonWayIds.begin()));
  return !commonWayIds.empty();
}

bool WayUtils::nodeContainedByMoreThanOneWay(const long nodeId, const ConstOsmMapPtr& map)
{
  return map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId).size() > 1;
}

std::vector<long> WayUtils::getIntersectingWayIds(const long wayId, const OsmMapPtr& map)
{
  std::vector<long> intersectingWayIds;
  WayPtr way = map->getWay(wayId);
  if (way)
  {
    std::vector<long> nearbyWayIds = map->getIndex().findWays(way->getEnvelopeInternal(map));
    for (std::vector<long>::const_iterator it = nearbyWayIds.begin(); it != nearbyWayIds.end();
         ++it)
    {
      const long otherWayId = *it;
      if (otherWayId != wayId)
      {
        WayPtr otherWay = map->getWay(otherWayId);
        if (otherWay && way->hasSharedNode(*otherWay))
        {
          intersectingWayIds.push_back(otherWay->getId());
        }
      }
    }
  }
  return intersectingWayIds;
}

std::vector<WayPtr> WayUtils::getIntersectingWays(const long wayId, const OsmMapPtr& map)
{
  std::vector<WayPtr> intersectingWays;
  const std::vector<long> intersectingWayIds = getIntersectingWayIds(wayId, map);
  for (std::vector<long>::const_iterator itr = intersectingWayIds.begin();
       itr != intersectingWayIds.end(); ++itr)
  {
    const long intersectingWayId = *itr;
    WayPtr way = map->getWay(intersectingWayId);
    if (way)
    {
      intersectingWays.push_back(way);
    }
  }
  return intersectingWays;
}

std::vector<long> WayUtils::getIntersectingWayIdsConst(const long wayId, const ConstOsmMapPtr& map)
{
  std::vector<long> intersectingWayIds;
  ConstWayPtr way = map->getWay(wayId);
  if (way)
  {
    std::vector<long> nearbyWayIds = map->getIndex().findWays(way->getEnvelopeInternal(map));
    for (std::vector<long>::const_iterator it = nearbyWayIds.begin(); it != nearbyWayIds.end();
         ++it)
    {
      const long otherWayId = *it;
      if (otherWayId != wayId)
      {
        ConstWayPtr otherWay = map->getWay(otherWayId);
        if (otherWay && way->hasSharedNode(*otherWay))
        {
          intersectingWayIds.push_back(otherWay->getId());
        }
      }
    }
  }
  return intersectingWayIds;
}

std::vector<ConstWayPtr> WayUtils::getIntersectingWaysConst(
  const long wayId, const ConstOsmMapPtr& map)
{
  std::vector<ConstWayPtr> intersectingWays;
  const std::vector<long> intersectingWayIds = getIntersectingWayIdsConst(wayId, map);
  for (std::vector<long>::const_iterator itr = intersectingWayIds.begin();
       itr != intersectingWayIds.end(); ++itr)
  {
    const long intersectingWayId = *itr;
    ConstWayPtr way = map->getWay(intersectingWayId);
    if (way)
    {
      intersectingWays.push_back(way);
    }
  }
  return intersectingWays;
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
    getContainingWaysConst(nodeId, map);
  for (std::vector<ConstWayPtr>::const_iterator containingWayItr = waysContainingNode.begin();
       containingWayItr != waysContainingNode.end(); ++containingWayItr)
  {
    ConstWayPtr containingWay = *containingWayItr;
    if (containingWay && containingWay->hasSharedNode(*way))
    {
      return true;
    }
  }
  return false;
}

}
