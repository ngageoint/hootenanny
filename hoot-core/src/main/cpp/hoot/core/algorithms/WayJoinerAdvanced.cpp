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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "WayJoinerAdvanced.h"

//  Hoot
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/conflate/highway/HighwayUtils.h>
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/BridgeCriterion.h>
#include <hoot/core/criterion/CriterionUtils.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

#include <unordered_set>
#include <vector>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(WayJoiner, WayJoinerAdvanced)

WayJoinerAdvanced::WayJoinerAdvanced() :
WayJoiner::WayJoiner(),
_callingClass(className())
{
}

void WayJoinerAdvanced::joinWays(const OsmMapPtr& map)
{
  WayJoinerAdvanced wayJoiner;
  wayJoiner.join(map);
}

void WayJoinerAdvanced::join(const OsmMapPtr& map)
{
  //  Call base class implementation
  WayJoiner::join(map);

  // Try to find ref roads that possibly didn't match during the matching phase but can still be
  // joined up.
  if (ConfigOptions().getAttributeConflationAggressiveHighwayJoining())
  {
    _joinUnsplitWaysAtNode();
    OsmMapWriterFactory::writeDebugMap(map, "after-way-joiner-join-unsplit-ways");
  }
}

void WayJoinerAdvanced::_joinParentChild()
{
  LOG_INFO("\tJoining parent ways to children...");

  WayMap ways = _map->getWays();
  _totalWays = ways.size();
  vector<long> ids;
  //  Find all ways that have a split parent id
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    if (_hasPid(way))
    {
      LOG_TRACE(
        "Adding " << way->getElementId() << " with split parent id: " << _getPid(way) << "...");
      ids.push_back(way->getId());
    }
  }
  //  Sort the ids so that the smallest is first (i.e. largest negative id is the last one allocated)
  sort(ids.begin(), ids.end());
  LOG_VART(ids);
  //  Iterate all of the ids
  for (vector<long>::const_iterator it = ids.begin(); it != ids.end(); ++it)
  {
    WayPtr way = ways[*it];
    if (way)
    {
      LOG_VART(way->getElementId());
    }
    long parent_id = way->getPid();
    LOG_VART(parent_id);
    WayPtr parent = ways[parent_id];
    Tags parentTags;
    if (parent)
    {
      LOG_VART(parent->getElementId());
      parentTags = parent->getTags();
    }
    else
    {
      LOG_TRACE(
        "Parent: " << ElementId(ElementType::Way, parent_id) <<
        " does not exist. Skipping join with " << way->getElementId());
    }

    // don't try to join if there are explicitly conflicting names; fix for #2888
    Tags wayTags = way->getTags();
    // TODO: use TagUtils::nameConflictExists here instead
    const bool strictNameMatch = ConfigOptions().getWayJoinerAdvancedStrictNameMatch();
    if (parent && parentTags.hasName() && wayTags.hasName() &&
        !Tags::haveMatchingName(parentTags, wayTags, strictNameMatch))
    {
      // TODO: move this check down to _joinWays?
      LOG_TRACE(
        "Conflicting name tags between " << way->getElementId() << " and " <<
        parent->getElementId() << ".  Skipping parent/child join.");
      continue;
    }
    else
    {
      //  Join this way to the parent
      _callingMethod = "_joinParentChild";
      _joinWays(parent, way);
    }

    if (_numProcessed % (_taskStatusUpdateInterval / 10) == 0)
    {
      PROGRESS_INFO(
        "\tRejoined " << StringUtils::formatLargeNumber(_numJoined) << " pairs of ways / " <<
        StringUtils::formatLargeNumber(_totalWays) << " total ways.");
    }
  }
}

bool WayJoinerAdvanced::_hasPid(const ConstWayPtr& way) const
{
  return way->hasPid();
}

long WayJoinerAdvanced::_getPid(const ConstWayPtr& way) const
{
  return way->getPid();
}

void WayJoinerAdvanced::_joinAtNode()
{
  LOG_TRACE("\tJoining ways at node...");

  unordered_set<long> ids;
  unordered_set<long>::size_type currentNumSplitParentIds = ids.max_size();
  int numIterations = 0;

  // keep iterating until we're no longer joining any ways; fix for #2867
  while (currentNumSplitParentIds > 0)
  {
    LOG_TRACE("joinAtNode iteration: " << numIterations + 1);

    WayMap ways = _map->getWays();
    ids.clear();
    //  Find all ways that have a split parent id
    for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
    {
      WayPtr way = it->second;
      if (_hasPid(way))
      {
        LOG_TRACE(
          "Adding " << way->getElementId() << " with split parent id: " << _getPid(way) << "...");
        ids.insert(way->getId());
      }
    }
    LOG_VART(ids.size());

    LOG_VART(currentNumSplitParentIds);
    // If we didn't reduce the number of ways from the previous iteration, or there are none left
    // to reduce, then exit out.
    if (currentNumSplitParentIds == ids.size() || ids.size() == 0)
    {
      break;
    }
    currentNumSplitParentIds = ids.size();
    LOG_VART(currentNumSplitParentIds);

    // This needs to be called within the loop, since we're modifying the map.  It looks like the
    // index gets updated with any changes we make to the map, but is the node to way map itself
    // being updated as well?
    std::shared_ptr<NodeToWayMap> nodeToWayMap = _map->getIndex().getNodeToWayMap();
    //  Iterate all of the nodes and check for compatible ways to join them to
    for (unordered_set<long>::iterator it = ids.begin(); it != ids.end(); ++it)
    {
      WayPtr way = ways[*it];
      LOG_VART(way->getElementId());

      if (way->getNodeCount() < 1)
        continue;

      Tags pTags = way->getTags();
      // Ignoring length here during the parent/child tag equals check, since differing values in
      // that field can cause us to miss a way join.  We'll add that value up after joining the
      // combined way - change for #2867
      pTags.remove(MetadataTags::Length());
      //  Check each of the endpoints for ways to merge
      vector<long> endpoints({ way->getFirstNodeId(), way->getLastNodeId() });
      LOG_VART(endpoints.size());
      LOG_VART(endpoints);

      for (vector<long>::const_iterator e = endpoints.begin(); e != endpoints.end(); ++e)
      {
        //  Find all ways connected to this node
        const set<long>& way_ids = nodeToWayMap->getWaysByNode(*e);
        LOG_VART(way_ids.size());
        LOG_VART(way_ids);
        for (set<long>::const_iterator w = way_ids.begin(); w != way_ids.end(); ++w)
        {
          LOG_VART(way->getElementId());
          WayPtr child = _map->getWay(*w);
          if (child)
          {
            LOG_VART(child->getElementId());
          }
          LOG_VART(way->getId() != child->getId());
          LOG_VART(_areJoinable(way, child));
          LOG_VART(way->getStatus());
          LOG_VART(child->getStatus());
          if (child && way->getId() != child->getId() && _areJoinable(way, child))
          {
            Tags cTags = child->getTags();
            // change for #2867
            cTags.remove(MetadataTags::Length());

            // don't try to join if there are explicitly conflicting names; fix for #2888
            const bool parentHasName = pTags.hasName();
            const bool childHasName = cTags.hasName();
            // TODO: use TagUtils::nameConflictExists here instead
            const bool strictNameMatch = ConfigOptions().getWayJoinerAdvancedStrictNameMatch();
            if ((!parentHasName && childHasName) || (!childHasName && parentHasName) ||
                Tags::haveMatchingName(pTags, cTags, strictNameMatch))
            {
              _callingMethod = "_joinAtNode";
              _joinWays(way, child);
              break;
            }
            else
            {
              LOG_TRACE(
                "Ways " << way->getElementId() << " and " << child->getElementId() <<
                " had conflicting names.  Not joining.");
              LOG_VART(pTags);
              LOG_VART(cTags);
            }
          }
        }
      }

      if (_numProcessed % (_taskStatusUpdateInterval / 10) == 0)
      {
        PROGRESS_INFO(
          "\tRejoined " << StringUtils::formatLargeNumber(_numJoined) << " pairs of ways / " <<
          StringUtils::formatLargeNumber(_totalWays) << " total ways.");
      }
    }
    numIterations++;
  }

  LOG_TRACE("Num joinAtNode iterations: " << numIterations);
}

void WayJoinerAdvanced::_rejoinSiblings(deque<long>& way_ids)
{
  LOG_TRACE("\tRejoining siblings...");
  LOG_VART(way_ids);

  WayMap ways = _map->getWays();
  WayPtr start;
  WayPtr end;
  size_t failure_count = 0;
  deque<long> sorted;
  //  Try sorting the ways into a connected way by adding other ways before or after
  while (!way_ids.empty() && failure_count < way_ids.size())
  {
    long id = way_ids[0];
    way_ids.pop_front();
    WayPtr way = ways[id];

    if (!way)
    {
      LOG_TRACE(ElementId(ElementType::Way, id) << " does not exist.");
      continue;
    }
    else
    {
      LOG_VART(way->getElementId());
    }

    if (sorted.empty())
    {
      //  The first time through the loop, just use that way as the base
      sorted.push_back(id);
      start = way;
      end = way;
    }
    else
    {
      LOG_VART(way->getElementId());
      LOG_VART(start->getElementId());
      LOG_VART(end->getElementId());
      LOG_VART(way->getFirstNodeId());
      LOG_VART(way->getLastNodeId());
      LOG_VART(start->getFirstNodeId());
      LOG_VART(end->getLastNodeId());

      OneWayCriterion oneWayCrit;

      //  Check if the road is contiguous with the sorted roads
      if (end->getLastNodeId() == way->getFirstNodeId())
      {
        //  Matching up end-to-start works for all roads (one-way and not)
        sorted.push_back(id);
        end = way;
        failure_count = 0;
      }
      else if (start->getFirstNodeId() == way->getLastNodeId())
      {
        //  Matching up start-to-end works for all roads (one-way and not)
        sorted.push_front(id);
        start = way;
        failure_count = 0;
      }
      else if (!oneWayCrit.isSatisfied(end) && !oneWayCrit.isSatisfied(start) &&
               !oneWayCrit.isSatisfied(way))
      {
        //  Roads that aren't one way can be reversed but still be valid
        if (start->getFirstNodeId() == way->getFirstNodeId())
        {
          LOG_TRACE("Reversing " << way->getElementId() << "...");
          way->reverseOrder();
          sorted.push_front(id);
          start = way;
          failure_count = 0;
        }
        else if (end->getLastNodeId() == way->getLastNodeId())
        {
          LOG_TRACE("Reversing " << way->getElementId() << "...");
          way->reverseOrder();
          sorted.push_back(id);
          end = way;
          failure_count = 0;
        }
        else
        {
          //  Requeue the way and up the failure count
          LOG_TRACE(ElementId(ElementType::Way, id) << " cannot be rejoined (1).");
          way_ids.push_back(id);
          failure_count++;
        }
      }
      else
      {
        //  Requeue the way and up the failure count
        LOG_TRACE(ElementId(ElementType::Way, id) << " cannot be rejoined (2).");
        way_ids.push_back(id);
        failure_count++;
      }
    }
  }
  LOG_VART(sorted);

  //  Iterate the sorted ways and merge them
  if (sorted.size() > 1)
  {
    WayPtr parent = ways[sorted[0]];
    if (parent)
    {
      LOG_VART(parent->getElementId());
    }
    for (size_t i = 1; i < sorted.size(); ++i)
    {
      WayPtr child = ways[sorted[i]];
      // don't try to join if there are explicitly conflicting names; fix for #2888
      bool childHasName = false;
      Tags childTags;
      if (child)
      {
        LOG_VART((child->getElementId()));
        childTags = child->getTags();
        childHasName = childTags.hasName();
      }
      else
      {
        break;
      }
      const Tags parentTags = parent->getTags();
      const bool parentHasName = parentTags.hasName();
      // TODO: use TagUtils::nameConflictExists here instead
      const bool strictNameMatch = ConfigOptions().getWayJoinerAdvancedStrictNameMatch();
      if ((!parentHasName && childHasName) || (!childHasName && parentHasName) ||
          Tags::haveMatchingName(parentTags, childTags, strictNameMatch))
      {
        _callingMethod = "_rejoinSiblings";
        _joinWays(parent, child);
      }
      else
      {
        LOG_TRACE(
          "Ways " << parent->getElementId() << " and " << child->getElementId() <<
          " had conflicting names.  Not joining:");
        LOG_VART(parentTags);
        LOG_VART(childTags);
      }
    }

    // Remove the parent id tag from both of the ways, joinWays() gets the child, do the parent here
    parent->resetPid();
  }
}

bool WayJoinerAdvanced::_joinWays(const WayPtr& parent, const WayPtr& child)
{
  _numProcessed++;

  if (!parent || !child)
    return false;

  LOG_VART(_callingClass);
  LOG_VART(_callingMethod);
  LOG_VART(parent->getElementId());
  LOG_VART(child->getElementId());
  LOG_VART(parent->getStatus());
  LOG_VART(child->getStatus());

  //  Check if the two ways are able to be joined back up

  //  Make sure that there are nodes in the ways
  if (parent->getNodeIds().size() == 0 || child->getNodeIds().size() == 0)
  {
    LOG_TRACE(
      "One or more of the ways: " << parent->getElementId() << " and " << child->getElementId() <<
      " to be joined are empty. Skipping join.");
    return false;
  }

  WayPtr wayWithIdToKeep;
  WayPtr wayWithIdToLose;
  _determineKeeperFeatureForId(parent, child, wayWithIdToKeep, wayWithIdToLose);
  LOG_VART(wayWithIdToKeep);
  LOG_VART(wayWithIdToLose);

  // make sure tags go in the right direction
  WayPtr wayWithTagsToKeep;
  WayPtr wayWithTagsToLose;
  _determineKeeperFeatureForTags(parent, child, wayWithTagsToKeep, wayWithTagsToLose);
  LOG_VART(wayWithTagsToKeep);
  LOG_VART(wayWithTagsToLose);

  // Reverse the way if way to remove is one way and the two ways aren't in similar directions.
  // This needs to be done before we check the merge type.
  const bool keeperWasReversed = _handleOneWayStreetReversal(wayWithTagsToKeep, wayWithTagsToLose);

  vector<long> parent_nodes = wayWithIdToKeep->getNodeIds();
  LOG_VART(parent_nodes.size());
  LOG_VART(parent_nodes);
  vector<long> child_nodes = wayWithIdToLose->getNodeIds();
  LOG_VART(child_nodes.size());
  LOG_VART(child_nodes);
  //  make sure that they share the same node
  JoinAtNodeMergeType joinType;
  LOG_VART(child_nodes[0]);
  LOG_VART(parent_nodes[parent_nodes.size() - 1]);
  LOG_VART(child_nodes[child_nodes.size() - 1]);
  LOG_VART(parent_nodes[0]);
  if (child_nodes[0] == parent_nodes[parent_nodes.size() - 1])
  {
    joinType = JoinAtNodeMergeType::ParentFirst;
  }
  else if (child_nodes[child_nodes.size() - 1] == parent_nodes[0])
  {
    joinType = JoinAtNodeMergeType::ParentLast;
  }
  else
  {
    LOG_TRACE(
      "No join type found for " << wayWithIdToKeep->getElementId() << " and " <<
      wayWithIdToLose->getElementId() << ".");

    if (keeperWasReversed)
    {
      LOG_TRACE("Reverting reversed keeper way...");
      wayWithTagsToKeep->reverseOrder();
    }

    return false;
  }
  LOG_VART(joinType);

  //  Don't join area ways
  AreaCriterion areaCrit;
  if (areaCrit.isSatisfied(wayWithIdToKeep) || areaCrit.isSatisfied(wayWithIdToLose))
  {
    LOG_TRACE(
      "One or more of the ways: " << wayWithIdToKeep->getElementId() << " and " <<
      wayWithIdToLose->getElementId() << " to be joined are areas. Skipping join.");
    return false;
  }

  // deal with bridges
  std::vector<ConstElementPtr> elements;
  elements.push_back(wayWithTagsToKeep);
  elements.push_back(wayWithTagsToLose);
  const bool onlyOneIsABridge =
    CriterionUtils::containsSatisfyingElements<BridgeCriterion>(elements, OsmMapPtr(), 1, true);
  if (ConfigOptions().getAttributeConflationAllowRefGeometryChangesForBridges() &&
      onlyOneIsABridge)
  {
    LOG_TRACE(
      "Only one of the features: " << wayWithIdToKeep->getElementId() << " and " <<
      wayWithIdToLose->getElementId() << " to be joined is a bridge. Skipping join.");
    return false;
  }

  // don't try to join streets with conflicting one way info
  if (HighwayUtils::oneWayConflictExists(wayWithTagsToKeep, wayWithTagsToLose))
  {
    LOG_TRACE(
      "Conflicting one way street tags between " << wayWithIdToKeep->getElementId() << " and " <<
      wayWithIdToLose->getElementId() << ".  Skipping join.");
    return false;
  }

  // If two roads disagree in highway type and aren't generic, don't join back up.
  HighwayCriterion highwayCrit(_map);
  if (highwayCrit.isSatisfied(wayWithTagsToKeep) && highwayCrit.isSatisfied(wayWithTagsToLose) &&
      HighwayUtils::nonGenericHighwayConflictExists(wayWithTagsToKeep, wayWithTagsToLose))
  {
    LOG_TRACE(
      "Conflicting highway type tags between " << wayWithIdToKeep->getElementId() << " and " <<
      wayWithIdToLose->getElementId() << ".  Skipping join.")
    return false;
  }

  // Checks done, now we're ready to merge.

  //  Remove the split parent id
  wayWithIdToLose->resetPid();

  //  Merge the tags

  // #2888 fix
  Tags tags1 = wayWithTagsToKeep->getTags();
  Tags tags2 = wayWithTagsToLose->getTags();

  // If each of these has a length tag, then we need to add up the new value for the joined ways.
  // This logic should possibly be a part of the default tag merging instead of being done here
  // and should also add the possibility for multiple options for the length tag key...leaving this
  // as is for now (fix for #2867)
  const double totalLength = _getTotalLengthFromTags(tags1, tags2);

  Tags mergedTags = TagMergerFactory::mergeTags(tags1, tags2, ElementType::Way);
  if (totalLength != -1.0)
  {
    mergedTags.set(MetadataTags::Length(), QString::number(totalLength));
  }

  wayWithIdToKeep->setTags(mergedTags);

  //  Remove the duplicate node id of the overlap
  if (joinType == JoinAtNodeMergeType::ParentFirst)
  {
    //  Remove the first node of the child and append to parent
    child_nodes.erase(child_nodes.begin());
    wayWithIdToKeep->addNodes(child_nodes);
  }
  else if (joinType == JoinAtNodeMergeType::ParentLast)
  {
    //  Remove the last of the children and prepend them to the parent
    child_nodes.pop_back();
    wayWithIdToKeep->setNodes(child_nodes);
    wayWithIdToKeep->addNodes(parent_nodes);
  }

  //  Keep the conflated status in the parent if the child being merged is conflated
  if ((wayWithIdToKeep->getStatus() == Status::Conflated ||
       wayWithIdToLose->getStatus() == Status::Conflated) ||
      (wayWithIdToKeep->getStatus() == Status::Unknown1 &&
       wayWithIdToLose->getStatus() == Status::Unknown2) ||
      (wayWithIdToKeep->getStatus() == Status::Unknown2 &&
       wayWithIdToLose->getStatus() == Status::Unknown1))
    wayWithIdToKeep->setStatus(Status::Conflated);

  _joinedWayIdMappings[wayWithIdToLose->getId()] = wayWithIdToKeep->getId();

  //  Update any relations that contain the child to use the parent and remove the child.
  ReplaceElementOp(wayWithIdToLose->getElementId(), wayWithIdToKeep->getElementId()).apply(_map);
  wayWithIdToLose->getTags().clear();
  RecursiveElementRemover(wayWithIdToLose->getElementId()).apply(_map);

  LOG_TRACE(
    "Joined ways: " << wayWithIdToKeep->getElementId() << " and " <<
    wayWithIdToLose->getElementId() << "; way kept: " << wayWithIdToKeep << "; way removed: " <<
    wayWithIdToLose);
  _wayKeptAfterJoin = wayWithIdToKeep;

  _numJoined++;

  return true;
}

void WayJoinerAdvanced::_joinUnsplitWaysAtNode()
{
  // This could possibly be done inside of _joinAtNode to avoid an additional pass over the way
  // data by relaxing restrictions on what can be joined.

  LOG_TRACE("Joining unsplit ways at node...");

  HighwayCriterion highwayCrit(_map);
  OneWayCriterion oneWayCrit;
  std::shared_ptr<NodeToWayMap> nodeToWayMap = _map->getIndex().getNodeToWayMap();
  const WayMap ways = _map->getWays();
  int joinAttempts = 0;
  int successfulJoins = 0;
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    WayPtr wayToJoin = wayItr->second;
    if (wayToJoin->getTags().get("highway") == "road" && !wayToJoin->getTags().hasName() &&
        !oneWayCrit.isSatisfied(wayToJoin))
    {
      const vector<long> endpoints({ wayToJoin->getFirstNodeId(), wayToJoin->getLastNodeId() });
      for (vector<long>::const_iterator endpointItr = endpoints.begin();
           endpointItr != endpoints.end(); ++endpointItr)
      {
        const set<long>& connectedWaysIds = nodeToWayMap->getWaysByNode(*endpointItr);
        LOG_VART(connectedWaysIds);
        for (set<long>::const_iterator connectedItr = connectedWaysIds.begin();
             connectedItr != connectedWaysIds.end(); ++connectedItr)
        {
          WayPtr connectedWay = _map->getWay(*connectedItr);
          // Not sure how the connected way could be empty...
          if (connectedWay && highwayCrit.isSatisfied(connectedWay))
          {
            LOG_TRACE("_joinUnsplitWaysAtNode wayToJoin: " << wayToJoin);
            LOG_TRACE("_joinUnsplitWaysAtNode connected way: " << connectedWay);
            const QString roadVal = connectedWay->getTags().get("highway").trimmed();

            // Since this is basically an unmarked, non-oneway road, let's check both the regular
            // and reversed versions of the way we want to join.
            WayPtr reversedWayToJoinCopy(new Way(*wayToJoin));
            reversedWayToJoinCopy->reverseOrder();

            if (!roadVal.isEmpty() && roadVal != "road" && connectedWay->getTags().hasName() &&
                (DirectionFinder::isSimilarDirection2(_map, wayToJoin, connectedWay) ||
                 DirectionFinder::isSimilarDirection2(_map, reversedWayToJoinCopy, connectedWay)))
            {
              LOG_TRACE(
                "Attempting unsplit join on unsplit way: " << wayToJoin->getElementId() <<
                " and connected way: " << connectedWay->getElementId() << "...");
              joinAttempts++;
              if (_joinWays(connectedWay, wayToJoin))
              {
                successfulJoins++;
                LOG_TRACE(
                  "Successfully joined unsplit way: " << wayToJoin->getElementId() <<
                  " and connected way: " << connectedWay->getElementId());
                break;
              }
              else
              {
                LOG_TRACE(
                  "Unable to join unsplit way: " << wayToJoin->getElementId() <<
                  " and connected way: " << connectedWay->getElementId());
              }
            }
          }
        }
      }
    }

    if (_numProcessed % (_taskStatusUpdateInterval / 10) == 0)
    {
      PROGRESS_INFO(
        "\tRejoined " << StringUtils::formatLargeNumber(_numJoined) << " pairs of ways / " <<
        StringUtils::formatLargeNumber(_totalWays) << " total ways.");
    }
  }
  LOG_TRACE(
    "Successfully joined " << successfulJoins << " unsplit ways on " << joinAttempts <<
    " attempts.");
}

void WayJoinerAdvanced::_determineKeeperFeatureForTags(WayPtr parent, WayPtr child, WayPtr& keeper,
                                                       WayPtr& toRemove) const
{
  // This logic is kind of a mess but works.

  const QString tagMergerClassName = ConfigOptions().getTagMergerDefault();
  LOG_VART(tagMergerClassName);
  if (parent->getStatus() == Status::Unknown1)
  {
    if (tagMergerClassName == "hoot::OverwriteTagMerger" ||
        tagMergerClassName == "hoot::OverwriteTag2Merger")
    {
      keeper = child;
      toRemove = parent;
    }
    else if (tagMergerClassName == "hoot::OverwriteTag1Merger")
    {
      keeper = parent;
      toRemove = child;
    }
    else
    {
      keeper = parent;
      toRemove = child;
    }
  }
  else if (child->getStatus() == Status::Unknown1 ||
           (parent->getStatus() == Status::Conflated && child->getStatus() == Status::Conflated))
  {
    if (tagMergerClassName == "hoot::OverwriteTagMerger" ||
        tagMergerClassName == "hoot::OverwriteTag2Merger")
    {
      keeper = parent;
      toRemove = child;
    }
    else if (tagMergerClassName == "hoot::OverwriteTag1Merger")
    {
      keeper = child;
      toRemove = parent;
    }
    else
    {
      keeper = parent;
      toRemove = child;
    }
  }
  // does this make sense??
  else
  {
    keeper = parent;
    toRemove = child;
  }
}

void WayJoinerAdvanced::_determineKeeperFeatureForId(WayPtr parent, WayPtr child, WayPtr& keeper,
                                                     WayPtr& toRemove) const
{
  keeper = parent;
  toRemove = child;
}

bool WayJoinerAdvanced::_handleOneWayStreetReversal(WayPtr wayWithTagsToKeep,
                                                    ConstWayPtr wayWithTagsToLose)
{
  OneWayCriterion oneWayCrit;
  if (oneWayCrit.isSatisfied(wayWithTagsToLose) &&
      !oneWayCrit.isSatisfied(wayWithTagsToKeep) &&
      // note the use of an alternative isSimilarDirection method
      !DirectionFinder::isSimilarDirection2(
        _map->shared_from_this(), wayWithTagsToKeep, wayWithTagsToLose))
  {
    LOG_TRACE("Reversing order of " << wayWithTagsToKeep->getElementId());
    // make sure this reversal gets done before checking the join type later on
    wayWithTagsToKeep->reverseOrder();
    return true;
  }
  return false;
}

double WayJoinerAdvanced::_getTotalLengthFromTags(const Tags& tags1, const Tags& tags2) const
{
  double totalLength = -1.0;
  const bool eitherTagsHaveLength =
    tags1.contains(MetadataTags::Length()) || tags2.contains(MetadataTags::Length());
  if (eitherTagsHaveLength)
  {
    double length1 = 0.0;
    double length2 = 0.0;
    if (tags1.contains(MetadataTags::Length()))
    {
      bool ok = false;
      length1 = tags1.get(MetadataTags::Length()).toDouble(&ok);
      if (!ok)
      {
        length1 = 0.0;
      }
    }
    if (tags2.contains(MetadataTags::Length()))
    {
      bool ok = false;
      length2 = tags2.get(MetadataTags::Length()).toDouble(&ok);
      if (!ok)
      {
        length2 = 0.0;
      }
    }
    totalLength = length1 + length2;
  }
  return totalLength;
}

}
