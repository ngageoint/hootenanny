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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "WayJoiner.h"

//  Hoot
#include <hoot/core/ops/RemoveWayOp.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagMergerFactory.h>

#include <unordered_set>
#include <vector>

using namespace std;

namespace hoot
{

bool idCompare(long lhs, long rhs)
{
  return lhs < rhs;
}

WayJoiner::WayJoiner(const OsmMapPtr &map)
  : _map(map)
{
}

void WayJoiner::join(const OsmMapPtr &map)
{
  WayJoiner joiner(map);
  joiner.join();
}

void WayJoiner::join()
{
  //  Join back any ways with parent ids
  joinParentChild();
  //  Join any siblings that have the same parent id but the parent isn't connected
  joinSiblings();
  //  Rejoin any ways that are now connected to their parents
  joinParentChild();
  //  Run one last join on ways that share a node and have a parent id
  joinAtNode();
}

void WayJoiner::joinParentChild()
{
  WayMap ways = _map->getWays();
  vector<long> ids;
  //  Find all ways that have a split parent id
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    if (way->getTags().contains(MetadataTags::HootSplitParentId()))
      ids.push_back(way->getId());
  }
  if (ids.size() == 0)
    return;
  //  Sort the ids so that the smallest is first (i.e. largest negative id is the last one allocated)
  sort(ids.begin(), ids.end());

  //  Iterate all of the ids
  for (vector<long>::const_iterator it = ids.begin(); it != ids.end(); ++it)
  {
    WayPtr way = ways[*it];
    long parent_id = way->getTags().get(MetadataTags::HootSplitParentId()).toLong();
    WayPtr parent = ways[parent_id];
    //  Join this way to the parent
    joinWays(parent, way);
  }
}

void WayJoiner::joinSiblings()
{
  WayMap ways = _map->getWays();
  // Get a list of ways that still have a parent
  map<long, deque<long> > w;
  //  Find all ways that have a split parent id
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    Tags tags = way->getTags();
    if (tags.contains(MetadataTags::HootSplitParentId()))
    {
      long parent_id = tags[MetadataTags::HootSplitParentId()].toLong();
      w[parent_id].push_back(way->getId());
    }
  }
  //  Rejoin any sibling ways where the parent id no longer exists
  for (map<long, deque<long> >::iterator map_it = w.begin(); map_it != w.end(); ++map_it)
  {
    deque<long>& way_ids = map_it->second;
    while (way_ids.size() > 1)
      rejoinSiblings(way_ids);
  }
}

void WayJoiner::joinAtNode()
{
  return;
/*
  WayMap ways = _map->getWays();
  unordered_set<long> ids;
  //  Find all ways that have a split parent id
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    if (way->getTags().contains(MetadataTags::HootSplitParentId()))
      ids.insert(way->getId());
  }

  //  Iterate all of the nodes and check for compatible ways to join them to
  for (unordered_set<long>::iterator it = ids.begin(); it != ids.end(); ++it)
  {
    WayPtr way = ways[*id];
    long start_id = way->getFirstNodeId();
    vector<long> way_ids = _map->

    long end_id = way->getLastNodeId();



    if (pTags != cTags)
    {
      if (!pTags.dataOnlyEqual(cTags))
      {
        //  Tags aren't compatible, don't join
        return;
      }
    }
  }
*/
}

void WayJoiner::rejoinSiblings(deque<long>& way_ids)
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
      else if (!OsmSchema::getInstance().isOneWay(*end) &&
               !OsmSchema::getInstance().isOneWay(*start) &&
               !OsmSchema::getInstance().isOneWay(*way))
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
      joinWays(parent, ways[sorted[i]]);
    //  Remove the parent id tag from both of the ways, joinWays() gets the child, do the parent here
    parent->getTags().remove(MetadataTags::HootSplitParentId());
  }
}

void WayJoiner::joinWays(const WayPtr &parent, const WayPtr &child)
{
  if (!parent || !child)
    return;

  Tags pTags = parent->getTags();
  Tags cTags = child->getTags();

  //  Check if the two ways are able to be joined back up
  //  First make sure that they begin or end at the same node
  vector<long> child_nodes = child->getNodeIds();
  vector<long> parent_nodes = parent->getNodeIds();
  bool parentFirst;
  if (child_nodes[0] == parent_nodes[parent_nodes.size() - 1])
    parentFirst = true;
  else if (child_nodes[child_nodes.size() - 1] == parent_nodes[0])
    parentFirst = false;
  else
    return;

  //  Remove the split split tag
  cTags.remove(MetadataTags::HootSplitParentId());

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

  //  Remove the child
  RemoveWayOp::removeWay(_map, child->getId());
}

}
