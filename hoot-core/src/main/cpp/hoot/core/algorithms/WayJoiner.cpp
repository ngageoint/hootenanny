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

#include "WayJoiner.h"

//  Hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/ConfigOptions.h>

#include <unordered_set>
#include <vector>

using namespace std;

namespace hoot
{

WayJoiner::WayJoiner() :
_leavePid(false),
_numJoined(0)
{
}

void WayJoiner::join(const OsmMapPtr& map)
{
  _map = map;

  //  Join back any ways with parent ids
  _joinParentChild();
  OsmMapWriterFactory::writeDebugMap(map, "after-way-joiner-join-parent-child-1");

  //  Join any siblings that have the same parent id but the parent isn't connected
  _joinSiblings();
  OsmMapWriterFactory::writeDebugMap(map, "after-way-joiner-join-siblings");

  //  Rejoin any ways that are now connected to their parents
  _joinParentChild();
  OsmMapWriterFactory::writeDebugMap(map, "after-way-joiner-join-parent-child-2");

  //  Run one last join on ways that share a node and have a parent id
  _joinAtNode();
  OsmMapWriterFactory::writeDebugMap(map, "after-way-joiner-join-at-node");

  //  Clear out any remaining unjoined parent ids
  _resetParents();
}

void WayJoiner::_joinParentChild()
{
  LOG_INFO("Joining parent ways to children...");

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

  //  Iterate all of the ids
  for (vector<long>::const_iterator it = ids.begin(); it != ids.end(); ++it)
  {
    WayPtr way = ways[*it];
    long parent_id = way->getPid();
    WayPtr parent = ways[parent_id];
    //  Join this way to the parent
    _joinWays(parent, way);
  }
}

void WayJoiner::_joinSiblings()
{
  LOG_INFO("Joining way siblings...");

  WayMap ways = _map->getWays();
  // Get a list of ways that still have a parent
  map<long, deque<long>> w;
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
  for (map<long, deque<long>>::iterator map_it = w.begin(); map_it != w.end(); ++map_it)
  {
    deque<long>& way_ids = map_it->second;
    LOG_VART(way_ids);
    while (way_ids.size() > 1)
      _rejoinSiblings(way_ids);
  }
}

void WayJoiner::_joinAtNode()
{
  LOG_INFO("Joining ways at shared nodes...");

  WayMap ways = _map->getWays();
  unordered_set<long> ids;
  //  Find all ways that have a split parent id
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    if (way->hasPid())
      ids.insert(way->getId());
  }
  std::shared_ptr<NodeToWayMap> nodeToWayMap = _map->getIndex().getNodeToWayMap();
  //  Iterate all of the nodes and check for compatible ways to join them to
  for (unordered_set<long>::iterator it = ids.begin(); it != ids.end(); ++it)
  {
    WayPtr way = ways[*it];
    Tags pTags = way->getTags();
    //  Check each of the endpoints for ways to merge
    vector<long> endpoints({ way->getFirstNodeId(), way->getLastNodeId() });
    for (vector<long>::const_iterator e = endpoints.begin(); e != endpoints.end(); ++e)
    {
      //  Find all ways connected to this node
      const set<long>& way_ids = nodeToWayMap->getWaysByNode(*e);
      for (set<long>::const_iterator ways = way_ids.begin(); ways != way_ids.end(); ++ways)
      {
        WayPtr child = _map->getWay(*ways);
        if (child && way->getId() != child->getId() && _areJoinable(way, child))
        {
          Tags cTags = child->getTags();
          //  Check for equivalent tags
          if (pTags == cTags || pTags.dataOnlyEqual(cTags))
          {
            _joinWays(way, child);
            break;
          }
        }
      }
    }
  }
}

bool WayJoiner::_areJoinable(const WayPtr& w1, const WayPtr& w2) const
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

void WayJoiner::_resetParents()
{
  LOG_VARD(_leavePid);
  if (_leavePid)
    return;
  WayMap ways = _map->getWays();
  //  Find all ways that have a split parent id and reset them
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    if (way->hasPid())
      way->resetPid();
  }
}

void WayJoiner::_rejoinSiblings(deque<long>& way_ids)
{
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
      continue;
    if (sorted.empty())
    {
      //  The first time through the loop, just use that way as the base
      sorted.push_back(id);
      start = way;
      end = way;
    }
    else
    {
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
          way->reverseOrder();
          sorted.push_front(id);
          start = way;
          failure_count = 0;
        }
        else if (end->getLastNodeId() == way->getLastNodeId())
        {
          way->reverseOrder();
          sorted.push_back(id);
          end = way;
          failure_count = 0;
        }
        else
        {
          //  Requeue the way and up the failure count
          way_ids.push_back(id);
          failure_count++;
        }
      }
      else
      {
        //  Requeue the way and up the failure count
        way_ids.push_back(id);
        failure_count++;
      }
    }
  }
  //  Iterate the sorted ways and merge them
  if (sorted.size() > 1)
  {
    WayPtr parent = ways[sorted[0]];
    for (size_t i = 1; i < sorted.size(); ++i)
      _joinWays(parent, ways[sorted[i]]);
  }
}

bool WayJoiner::_joinWays(const WayPtr &parent, const WayPtr &child)
{
  if (!parent || !child)
    return false;

  //  Don't join area ways
  AreaCriterion areaCrit;
  if (areaCrit.isSatisfied(parent) || areaCrit.isSatisfied(child))
    return false;
  //  Check if the two ways are able to be joined back up
  vector<long> child_nodes = child->getNodeIds();
  vector<long> parent_nodes = parent->getNodeIds();
  //  Make sure that there are nodes in the ways
  if (parent_nodes.size() == 0 || child_nodes.size() == 0)
    return false;
  //  First make sure that they begin or end at the same node
  bool parentFirst;
  if (child_nodes[0] == parent_nodes[parent_nodes.size() - 1])
    parentFirst = true;
  else if (child_nodes[child_nodes.size() - 1] == parent_nodes[0])
    parentFirst = false;
  else
    return false;
  //  Remove the split parent id
  child->resetPid();
  //  Merge the tags
  Tags pTags = parent->getTags();
  Tags cTags = child->getTags();
  Tags tags = TagMergerFactory::mergeTags(pTags, cTags, ElementType::Way);
  parent->setTags(tags);
  //  Remove the duplicate node id of the overlap
  if (parentFirst)
  {
    //  Remove the first node of the child and append to parent
    child_nodes.erase(child_nodes.begin());
    parent->addNodes(child_nodes);
  }
  else
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

  _numJoined++;

  return true;
}

}
