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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "WayJoiner2.h"

//  Hoot
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/criterion/BridgeCriterion.h>

#include <unordered_set>
#include <vector>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(WayJoiner, WayJoiner2)

WayJoiner2::WayJoiner2() :
_leavePid(false),
_numJoined(0)
{
}

void WayJoiner2::join(const OsmMapPtr& map)
{
  _map = map;

  //  Join back any ways with parent ids
  _joinParentChild();
  OsmMapWriterFactory::writeDebugMap(map);

  //  Join any siblings that have the same parent id but the parent isn't connected
  _joinSiblings();
  OsmMapWriterFactory::writeDebugMap(map);

  //  Rejoin any ways that are now connected to their parents
  _joinParentChild();
  OsmMapWriterFactory::writeDebugMap(map);

  //  Run one last join on ways that share a node and have a parent id
  _joinAtNode();
  OsmMapWriterFactory::writeDebugMap(map);

  //  Clear out any remaining unjoined parent ids
  _resetParents();
  OsmMapWriterFactory::writeDebugMap(map);
}

void WayJoiner2::_resetParents()
{
  LOG_TRACE("Resetting parents...");

  if (_leavePid)
    return;
  WayMap ways = _map->getWays();
  //  Find all ways that have a split parent id and reset them
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    if (way->hasPid())
    {
      way->resetPid();
    }
  }
}

void WayJoiner2::_joinParentChild()
{
  LOG_TRACE("Joining parents to children...");

  WayMap ways = _map->getWays();
  vector<long> ids;
  //  Find all ways that have a split parent id
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    if (way->hasPid())
      ids.push_back(way->getId());
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
      LOG_TRACE("Parent with ID: " << parent_id << " does not exist.");
    }

    // don't try to join if there are explicitly conflicting names; fix for #2888
    Tags wayTags = way->getTags();
    if (parent && parentTags.hasName() && wayTags.hasName() &&
        !Tags::haveMatchingName(parentTags, wayTags))
    {
      // TODO: move this check down to _joinWays?
      LOG_TRACE("Conflicting name tags.  Skipping parent/child join.");
      continue;
    }
    else
    {
      //  Join this way to the parent
      _joinWays(parent, way);
    }
  }
}

void WayJoiner2::_joinSiblings()
{
  LOG_TRACE("Joining siblings...");

  WayMap ways = _map->getWays();
  // Get a list of ways that still have a parent
  map<long, deque<long> > w;
  //  Find all ways that have a split parent id
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    if (way->hasPid())
    {
      long parent_id = way->getPid();
      w[parent_id].push_back(way->getId());
    }
  }
  //  Rejoin any sibling ways where the parent id no longer exists
  for (map<long, deque<long> >::iterator map_it = w.begin(); map_it != w.end(); ++map_it)
  {
    deque<long>& way_ids = map_it->second;
    LOG_VART(way_ids);
    while (way_ids.size() > 1)
      _rejoinSiblings(way_ids);
  }
}

void WayJoiner2::_joinAtNode()
{
  LOG_TRACE("Joining at node...");

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
      if (way->hasPid())
        ids.insert(way->getId());
    }

    LOG_VART(currentNumSplitParentIds);
    // If we didn't reduce the number of ways from the previous iteration or there are none left
    // to reduce, exit out.
    if (currentNumSplitParentIds == ids.size() || ids.size() == 0)
    {
      break;
    }
    currentNumSplitParentIds = ids.size();
    LOG_VART(currentNumSplitParentIds);

    boost::shared_ptr<NodeToWayMap> nodeToWayMap = _map->getIndex().getNodeToWayMap();
    //  Iterate all of the nodes and check for compatible ways to join them to
    for (unordered_set<long>::iterator it = ids.begin(); it != ids.end(); ++it)
    {
      WayPtr way = ways[*it];
      LOG_VART(way->getElementId());

      Tags pTags = way->getTags();
      // Ignoring length here during the parent/child tag equals check, since differing values in
      // that field can cause us to miss a way join.  We'll add that value
      // up after joining the combined way.
      // change for #2867
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
        for (set<long>::const_iterator ways = way_ids.begin(); ways != way_ids.end(); ++ways)
        {
          LOG_VART(way->getElementId());
          WayPtr child = _map->getWay(*ways);
          if (child)
          {
            LOG_VART(child->getElementId());
          }
          if (child && way->getId() != child->getId() && _areJoinable(way, child))
          {
            Tags cTags = child->getTags();
            // change for #2867
            cTags.remove(MetadataTags::Length());

            // don't try to join if there are explicitly conflicting names; fix for #2888
            const bool parentHasName = pTags.hasName();
            const bool childHasName = cTags.hasName();
            if ((!parentHasName && childHasName) || (!childHasName && parentHasName) ||
                Tags::haveMatchingName(pTags, cTags))
            {
              _joinWays(way, child);
              break;
            }
            else
            {
              LOG_TRACE("Ways had conflicting names.  Not joining:");
              LOG_VART(pTags);
              LOG_VART(cTags);
            }
          }
        }
      }
    }
    numIterations++;
  }

  LOG_TRACE("Num joinAtNode iterations: " << numIterations);
}

bool WayJoiner2::_areJoinable(const WayPtr& w1, const WayPtr& w2)
{
  return
      //  Same status => Joinable
      w1->getStatus() == w2->getStatus() ||
      //  One (or both) are conflated => Joinable
      (w1->getStatus() == Status::Conflated || w2->getStatus() == Status::Conflated) ||
      //  Don't know if this is the case but one (or both) are invalid => Joinable
      (w1->getStatus() == Status::Invalid || w2->getStatus() == Status::Invalid);
      //  What isn't joinable is one is UNKNOWN1 and the other is UNKNOWN2 or vice-a-versa
}

void WayJoiner2::_rejoinSiblings(deque<long>& way_ids)
{
  LOG_TRACE("Rejoining siblings...");
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
      LOG_TRACE("Way with ID: " << id << " does not exist.");
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
          LOG_TRACE("Way with ID: " << id << " cannot be rejoined (1).");
          way_ids.push_back(id);
          failure_count++;
        }
      }
      else
      {
        //  Requeue the way and up the failure count
        LOG_TRACE("Way with ID: " << id << " cannot be rejoined (2).");
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
      if ((!parentHasName && childHasName) || (!childHasName && parentHasName) ||
          Tags::haveMatchingName(parentTags, childTags))
      {
        _joinWays(parent, child);
      }
    }

    //  Remove the parent id tag from both of the ways, joinWays() gets the child, do the parent here
    parent->resetPid();
  }
}

void WayJoiner2::_joinWays(const WayPtr& parent, const WayPtr& child)
{
  if (!parent || !child)
    return;

  LOG_VART(parent->getId());
  LOG_VART(child->getId());
  LOG_VART(parent->getStatus());
  LOG_VART(child->getStatus());

  //  Check if the two ways are able to be joined back up

  //  Make sure that there are nodes in the ways
  if (parent->getNodeIds().size() == 0 || child->getNodeIds().size() == 0)
  {
    LOG_TRACE("One or more of the ways to be joined are empty. Skipping join.");
    return;
  }

  //  Don't join area ways
  AreaCriterion areaCrit;
  if (areaCrit.isSatisfied(parent) || areaCrit.isSatisfied(child))
  {
    LOG_TRACE("One or more of the ways to be joined are areas. Skipping join.");
    return;
  }

  // make sure tags go in the right direction (TODO: this is a mess)
  WayPtr wayWithTagsToKeep;
  WayPtr wayWithTagsToLose;
  const QString tagMergerClassName = ConfigOptions().getTagMergerDefault();
  LOG_VART(tagMergerClassName);
  if (parent->getStatus() == Status::Unknown1)
  {
    if (tagMergerClassName == "hoot::OverwriteTagMerger" ||
        tagMergerClassName == "hoot::OverwriteTag2Merger")
    {
      wayWithTagsToKeep = child;
      wayWithTagsToLose = parent;
    }
    else if (tagMergerClassName == "hoot::OverwriteTag1Merger")
    {
      wayWithTagsToKeep = parent;
      wayWithTagsToLose = child;
    }
    else
    {
      wayWithTagsToKeep = parent;
      wayWithTagsToLose = child;
    }
  }
  else if (child->getStatus() == Status::Unknown1 ||
           (parent->getStatus() == Status::Conflated && child->getStatus() == Status::Conflated))
  {
    if (tagMergerClassName == "hoot::OverwriteTagMerger" ||
        tagMergerClassName == "hoot::OverwriteTag2Merger")
    {
      wayWithTagsToKeep = parent;
      wayWithTagsToLose = child;
    }
    else if (tagMergerClassName == "hoot::OverwriteTag1Merger")
    {
      wayWithTagsToKeep = child;
      wayWithTagsToLose = parent;
    }
    else
    {
      wayWithTagsToKeep = parent;
      wayWithTagsToLose = child;
    }
  }
  // does this make sense??
  else
  {
    wayWithTagsToKeep = parent;
    wayWithTagsToLose = child;
  }
  LOG_VART(wayWithTagsToKeep);
  LOG_VART(wayWithTagsToLose);

  // deal with bridges

  BridgeCriterion isBridge;
  const bool e1IsBridge = isBridge.isSatisfied(wayWithTagsToKeep);
  const bool e2IsBridge = isBridge.isSatisfied(wayWithTagsToLose);
  const bool onlyOneIsABridge = (e1IsBridge && !e2IsBridge) || (e2IsBridge && !e1IsBridge);
  if (onlyOneIsABridge)
  {
    LOG_DEBUG("Only one of the features to be joined is a bridge. Skipping join.");
    return;
  }

  // deal with one way streets

  OneWayCriterion oneWayCrit;
  LOG_VART(oneWayCrit.isSatisfied(wayWithTagsToKeep));
  LOG_VART(oneWayCrit.isSatisfied(wayWithTagsToLose));

  // TODO: use Tags::isFalse here instead
  const bool keepElementExplicitlyNotAOneWayStreet =
    wayWithTagsToKeep->getTags().get("oneway") == "no";
  const bool removeElementExplicitlyNotAOneWayStreet =
    wayWithTagsToLose->getTags().get("oneway") == "no";
  LOG_VART(keepElementExplicitlyNotAOneWayStreet);
  LOG_VART(removeElementExplicitlyNotAOneWayStreet);
  if ((oneWayCrit.isSatisfied(wayWithTagsToKeep) &&
       removeElementExplicitlyNotAOneWayStreet) ||
      (oneWayCrit.isSatisfied(wayWithTagsToLose) &&
       keepElementExplicitlyNotAOneWayStreet))
  {
    LOG_TRACE("Conflicting one way street tags.  Skipping join.");
    return;
  }

  if (oneWayCrit.isSatisfied(wayWithTagsToLose) &&
      !oneWayCrit.isSatisfied(wayWithTagsToKeep) &&
      // note the use of an alternative isSimilarDirection method
      !DirectionFinder::isSimilarDirection2(
        _map->shared_from_this(), wayWithTagsToKeep, wayWithTagsToLose))
  {
    LOG_TRACE("Reversing order of " << wayWithTagsToKeep->getElementId());
    // make sure this reversal gets done before checking the join type later on
    wayWithTagsToKeep->reverseOrder();
  }

  vector<long> parent_nodes = parent->getNodeIds();
  LOG_VART(parent_nodes.size());
  LOG_VART(parent_nodes);
  vector<long> child_nodes = child->getNodeIds();
  LOG_VART(child_nodes.size());
  LOG_VART(child_nodes);

  //  First make sure that they share the same node
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
    LOG_TRACE("No join type found.");
    return;
  }
  LOG_VART(joinType);

  //  Remove the split parent id
  child->resetPid();

  //  Merge the tags

  // #2888 fix
  Tags tags1 = wayWithTagsToKeep->getTags();
  Tags tags2 = wayWithTagsToLose->getTags();

  // If each of these has a length tag, then we need to add up the new value for the joined ways.
  // This logic should possibly be a part of the default tag merging instead of being done here
  // and should also add the possibility for multiple options for the length tag key...leaving this
  // as is for now (fix for #2867)
  double totalLength = 0.0;
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

  Tags mergedTags = TagMergerFactory::mergeTags(tags1, tags2, ElementType::Way);
  if (eitherTagsHaveLength)
  {
    mergedTags.set(MetadataTags::Length(), QString::number(totalLength));
  }
  parent->setTags(mergedTags);

  //  Remove the duplicate node id of the overlap
  if (joinType == JoinAtNodeMergeType::ParentFirst)
  {
    //  Remove the first node of the child and append to parent
    child_nodes.erase(child_nodes.begin());
    parent->addNodes(child_nodes);
  }
  else if (joinType == JoinAtNodeMergeType::ParentLast)
  {
    //  Remove the last of the children and prepend them to the parent
    child_nodes.pop_back();
    parent->setNodes(child_nodes);
    parent->addNodes(parent_nodes);
  }

  //  Keep the conflated status in the parent if the child being merged is conflated
  if (parent->getStatus() == Status::Conflated || child->getStatus() == Status::Conflated)
    parent->setStatus(Status::Conflated);

  //  Update any relations that contain the child to use the parent
  ReplaceElementOp(child->getElementId(), parent->getElementId()).apply(_map);
  child->getTags().clear();
  RecursiveElementRemover(child->getElementId()).apply(_map);

  LOG_VART(parent);

  _numJoined++;
}

}
