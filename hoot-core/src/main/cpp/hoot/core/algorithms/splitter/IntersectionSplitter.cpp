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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "IntersectionSplitter.h"

// Hoot
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/algorithms/splitter/WaySplitter.h>
#include <hoot/core/criterion/NetworkTypeCriterion.h>
#include <hoot/core/elements/ElementIdUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/RelationMemberUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QDebug>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, IntersectionSplitter)

IntersectionSplitter::IntersectionSplitter(const std::shared_ptr<OsmMap>& map) :
_map(map)
{
}

QStringList IntersectionSplitter::getCriteria() const
{
  return NodeMatcher::getNetworkCriterionClassNames();
}

void IntersectionSplitter::apply(std::shared_ptr<OsmMap> &map)
{
  splitIntersections(map);
}

void IntersectionSplitter::splitIntersections(const std::shared_ptr<OsmMap>& map)
{
  IntersectionSplitter is(map);
  return is.splitIntersections();
}

void IntersectionSplitter::splitIntersections()
{
  _numAffected = 0;
  // Make a map of nodes to ways.
  _mapNodesToWays();

  // Go through all the nodes.
  int numProcessed = 0;
  const int taskStatusUpdateInterval = ConfigOptions().getTaskStatusUpdateInterval();
  while (_todoNodes.isEmpty() == false)
  {
    long nodeId = *_todoNodes.begin();
    //  Remove the node first in case it needs to be reprocessed later
    _todoNodes.remove(nodeId);
    numProcessed++;

    if (numProcessed % (taskStatusUpdateInterval * 10) == 0 && !_todoNodes.empty())
    {
      PROGRESS_INFO(
        "\tCreated  " <<  StringUtils::formatLargeNumber(numProcessed) <<
        " intersection splits. " << StringUtils::formatLargeNumber(_todoNodes.size()) <<
        " remaining nodes to process.");
    }

    // If the node is part of two or more ways,
    if (_nodeToWays.count(nodeId) >= 2)
    {
      // evaluate each way for splitting.
      QList<long> ways = _nodeToWays.values(nodeId);
      for (QList<long>::const_iterator way = ways.begin(); way != ways.end(); ++way)
      {
        _splitWay(*way, nodeId);
      }
    }
  }
}

void IntersectionSplitter::_mapNodesToWay(const std::shared_ptr<Way>& way)
{
  long wId = way->getId();

  const std::vector<long>& nodes = way->getNodeIds();
  for (size_t i = 0; i < nodes.size(); i++)
  {
    _nodeToWays.insert(nodes[i], wId);
    if (_nodeToWays.count(nodes[i]) > 1)
    {
      _todoNodes.insert(nodes[i]);
    }
  }
}

void IntersectionSplitter::_mapNodesToWays()
{
  _nodeToWays.clear();

  const WayMap& ways = _map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    std::shared_ptr<Way> w = it->second;
    const bool isNetworkType = NetworkTypeCriterion(_map).isSatisfied(w);

    LOG_VART(w->getElementId());
    LOG_VART(isNetworkType);
    if (isNetworkType)
    {
      _mapNodesToWay(w);
    }
  }
}

void IntersectionSplitter::_removeWayFromMap(const std::shared_ptr<Way>& way)
{
  LOG_TRACE("Removing " << way->getElementId() << " from map...");

  long wId = way->getId();
  const std::vector<long>& nodes = way->getNodeIds();
  for (size_t i = 0; i < nodes.size(); i++)
  {
    _nodeToWays.remove(nodes[i], wId);
  }
}

void IntersectionSplitter::_splitWay(long wayId, long nodeId)
{
  std::shared_ptr<Way> way = _map->getWay(wayId);
  if (way == nullptr)
  {
    LOG_TRACE("way at " << wayId << " does not exist.");
    return;
  }

  LOG_TRACE(
    "Attempting to split way: " << way->getElementId() << " at node: " <<
    ElementId(ElementType::Node, nodeId));

  // Find the first index of the split node that isn't an endpoint.
  int firstIndex = -1;
  const std::vector<long>& nodeIds = way->getNodeIds();
  for (size_t i = 1; i < nodeIds.size() - 1; i++)
  {
    if (nodeIds[i] == nodeId)
    {
      firstIndex = i;
      break;
    }
  }
  LOG_VART(firstIndex);

  // if the first index wasn't an endpoint
  if (firstIndex != -1)
  {
    QList<long> ways = _nodeToWays.values(nodeId);
    LOG_VART(ways);
    int concurrent_count = 0;
    int otherWays_count = ways.count() - 1;
    for (QList<long>::const_iterator it = ways.begin(); it != ways.end(); ++it)
    {
      long compWayId = *it;
      LOG_VART(compWayId);
      //  Don't compare it against itself.
      if (wayId == compWayId)
        continue;

      std::shared_ptr<Way> comp = _map->getWay(compWayId);
      if (!comp)
      {
        continue;
      }
      const std::vector<long>& compIds = comp->getNodeIds();
      long idx = comp->getNodeIndex(nodeId);
      LOG_VART(idx);

      //  The endpoints of the other way should be split.
      if (idx < 1 || idx > (long)compIds.size() - 1)
        continue;

      //  Check both in forward and reverse for shared nodes in the way
      if ((nodeIds[firstIndex - 1] == compIds[idx - 1] && nodeIds[firstIndex + 1] == compIds[idx + 1]) ||
          (nodeIds[firstIndex - 1] == compIds[idx + 1] && nodeIds[firstIndex + 1] == compIds[idx - 1]))
        concurrent_count++;
    }

    LOG_VART(concurrent_count);
    //  A split point is found when there is at least one non-concurrent way at this node.
    if (concurrent_count < otherWays_count)
    {
      // Split the way and remove it from the map.
      WayLocation wl(_map, way, firstIndex, 0.0);
      LOG_TRACE("Splitting " << way->getElementId() << " at " << wl << "...");
      vector<std::shared_ptr<Way>> splits = WaySplitter::split(_map, way, wl);
      LOG_VART(splits);

      // If a split occurred:
      if (splits.size() > 1)
      {
        _numAffected++;

        LOG_VART(way->getElementId());
        LOG_VART(way->getStatus());
        LOG_VART(splits[0].get());
        LOG_VART(splits[0]->getElementId());

        const ElementId splitWayId = way->getElementId();

        QList<ElementPtr> newWays;
        foreach (const std::shared_ptr<Way>& w, splits)
        {
          newWays.append(w);
        }

        // Ensure that relation member order for the replaced ways are preserved.
        _preserveWayRelationMemberOrder(splitWayId, newWays);

        // Make sure any ways that are part of relations continue to be part of those relations
        // after they're split.
        _map->replace(way, newWays);
        if (ConfigOptions().getDebugMapsWrite() && ConfigOptions().getDebugMapsWriteDetailed())
        {
          OsmMapWriterFactory::writeDebugMap(
            _map, "IntersectionSplitter-after-replace-" + way->getElementId().toString());
        }

        _removeWayFromMap(way);
        if (ConfigOptions().getDebugMapsWrite() && ConfigOptions().getDebugMapsWriteDetailed())
        {
          OsmMapWriterFactory::writeDebugMap(
            _map, "IntersectionSplitter-after-remove-" + way->getElementId().toString());
        }

        // Go through all the resulting splits.
        for (size_t i = 0; i < splits.size(); i++)
        {
          // Add the new ways nodes just in case the way was self-intersecting.
          _mapNodesToWay(splits[i]);
        }

        LOG_VART(_map->containsElement(splitWayId));
      }
    }
  }
}

bool IntersectionSplitter::_determineSplitWaysOrdering(
  const QList<ElementPtr>& splitWays, const ElementId& splitWayId, WayPtr& splitWay,
  WayPtr& addedWay) const
{
  assert(splitWays.size() == 2);
  bool firstNewWayIsSplitWay = false;

  QList<ElementPtr>::const_iterator itr = splitWays.begin();
  WayPtr firstWay = std::dynamic_pointer_cast<Way>(*itr);
  itr++;
  WayPtr secondWay = std::dynamic_pointer_cast<Way>(*itr);
  if (firstWay->getElementId() == splitWayId)
  {
    splitWay = firstWay;
    addedWay = secondWay;
    firstNewWayIsSplitWay = true;
  }
  else
  {
    splitWay = secondWay;
    addedWay = firstWay;
  }

  LOG_VART(firstNewWayIsSplitWay);
  LOG_VART(splitWay->getElementId());
  LOG_VART(addedWay->getElementId());

  return firstNewWayIsSplitWay;
}

QList<ElementId> IntersectionSplitter::_getAdjoiningRelationMembers(
  const ElementId& wayId, const ConstRelationPtr& containingRelation,
  ConstWayPtr& adjoiningWayMemberIndexedBefore, ConstWayPtr& adjoiningWayMemberIndexedAfter) const
{
  const QList<ElementId> adjoiningMemberIds =
    containingRelation->getAdjoiningMemberIds(wayId);
  LOG_VART(adjoiningMemberIds);

  if (adjoiningMemberIds.size() == 2)
  {
    int ctr = 0;
    for (QList<ElementId>::const_iterator itr = adjoiningMemberIds.begin();
         itr != adjoiningMemberIds.end(); ++itr)
    {
      if (ctr == 0)
      {
        adjoiningWayMemberIndexedBefore = _map->getWay(*itr);
      }
      else
      {
        adjoiningWayMemberIndexedAfter = _map->getWay(*itr);
      }
      ctr++;
    }
    LOG_VART(adjoiningWayMemberIndexedBefore.get());
    LOG_VART(adjoiningWayMemberIndexedBefore->getElementId());
    LOG_VART(adjoiningWayMemberIndexedAfter.get());
    LOG_VART(adjoiningWayMemberIndexedAfter->getElementId());
  }
  else if (adjoiningMemberIds.size() == 1)
  {
    if (containingRelation->isFirstMember(wayId))
    {
      // The only entry in adjoiningMemberIds is the ID of the way member after it.
      adjoiningWayMemberIndexedAfter = _map->getWay(*(adjoiningMemberIds.begin()));
      LOG_VART(adjoiningWayMemberIndexedAfter->getElementId());
    }
    else if (containingRelation->isLastMember(wayId))
    {
      // The only entry in adjoiningMemberIds is the ID of the way member before it.
      adjoiningWayMemberIndexedBefore = _map->getWay(*(adjoiningMemberIds.begin()));
      LOG_VART(adjoiningWayMemberIndexedBefore->getElementId());
    }
  }

  LOG_VART(adjoiningMemberIds);
  return adjoiningMemberIds;
}

void IntersectionSplitter::_preserveWayRelationMemberOrder(
  const ElementId& splitWayId, QList<ElementPtr>& newWays) const
{
  LOG_VART(splitWayId);
  LOG_VART(newWays);

  // newWays consists of the way that was split and the new way created as a result of the
  // splitting. Determine which is which.
  int fromIndex = -1;
  // Sanity check here. Currently, both these conditions should always be true.
  if (newWays.size() == 2 && ElementIdUtils::containsElementId(splitWayId, newWays, fromIndex))
  {
    int addedWayIndex = 0;
    if (fromIndex == 0)
    {
      addedWayIndex = 1;
    }
    LOG_VART(addedWayIndex);
    ConstWayPtr addedWay = std::dynamic_pointer_cast<const Way>(newWays.at(addedWayIndex));
    LOG_VART(addedWay->getElementId());

    // Get all the relations that the split way belongs to.
    const std::vector<ConstRelationPtr> containingRelations =
      RelationMemberUtils::getContainingRelationsConst(splitWayId, _map, true);
    LOG_VART(containingRelations.size());
    if (containingRelations.size() > 0)
    {
      // Arbitrarily just looking at the first one here. Not sure what to do in the case of
      // membership to multiple relations yet.
      ConstRelationPtr containingRelation = containingRelations.front();
      LOG_VART(containingRelation->getElementId());

      // Get the members adjoining the split way from the relation.
      ConstWayPtr adjoiningWayMemberIndexedBefore;
      ConstWayPtr adjoiningWayMemberIndexedAfter;
      const QList<ElementId> adjoiningMemberIds =
        _getAdjoiningRelationMembers(
          splitWayId, containingRelation, adjoiningWayMemberIndexedBefore,
          adjoiningWayMemberIndexedAfter);

      // If we have two adjoining member IDs, that means the split way is neither the first nor
      // last member in the relation. (We may eventually be able to combine the cases for
      // adjoiningMemberIds.size() == 1 and adjoiningMemberIds.size() == 2 together here).
      bool reverseNewWays = false;
      if (adjoiningMemberIds.size() == 2)
      {
        assert(adjoiningWayMemberIndexedBefore != nullptr);
        LOG_VART(addedWay->hasSharedEndNode(*adjoiningWayMemberIndexedBefore));
        // If the new way created by the split shares an endpoint with the member indexed before the
        // split way being replaced, reverse the copied way list previously created to preserve
        // relation membership order. NOTE: not sure yet if this is catching all test cases
        // correctly.
        if (addedWay->hasSharedEndNode(*adjoiningWayMemberIndexedBefore))
        {  
          reverseNewWays = true;
        }
      }
      // If only one adjoining member ID was returned, the way being split is either the first or
      // last member in the relation.
      else if (adjoiningMemberIds.size() == 1)
      {
        assert(
          adjoiningWayMemberIndexedBefore != nullptr || adjoiningWayMemberIndexedAfter != nullptr);

        WayPtr splitWay;
        WayPtr addedWay;
        const bool firstNewWayIsSplitWay =
          _determineSplitWaysOrdering(newWays, splitWayId, splitWay, addedWay);

        LOG_VART(containingRelation->isFirstMember(splitWayId));
        LOG_VART(containingRelation->isLastMember(splitWayId));

        // If the way that was split was the first relation member and its ID is first in the
        // newWays list,
        if (adjoiningWayMemberIndexedAfter && firstNewWayIsSplitWay &&
            containingRelation->isFirstMember(splitWayId))
        {
          LOG_VART(addedWay->hasSharedEndNode(*adjoiningWayMemberIndexedAfter));
          // ...and the added way shares an end node with the member indexed after, then we need to
          // reverse newWays. The ordering of ways in newWays should always match the relation
          // member order, so if the first way is the split way but the added way is connected to
          // the way member after the split way, we need to reverse the ordering of newWays.
          if (!addedWay->hasSharedEndNode(*adjoiningWayMemberIndexedAfter))
          {
            reverseNewWays = true;
          }
        }
        // This is similar logic to above but handles when the split way is the last relation
        // member.
        else if (adjoiningWayMemberIndexedBefore && !firstNewWayIsSplitWay &&
                 containingRelation->isLastMember(splitWayId))
        {
          LOG_VART(addedWay->hasSharedEndNode(*adjoiningWayMemberIndexedBefore));
          if (!addedWay->hasSharedEndNode(*adjoiningWayMemberIndexedBefore))
          {
            reverseNewWays = true;
          }
        }
      }

      if (reverseNewWays)
      {
        LOG_TRACE("Reversing new ways...");
        std::reverse(newWays.begin(), newWays.end());
      }
    }
  }
  LOG_VART(newWays);
}

}
