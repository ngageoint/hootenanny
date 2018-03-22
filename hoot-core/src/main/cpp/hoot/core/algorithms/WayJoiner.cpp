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
#include <hoot/core/schema/TagMergerFactory.h>

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
    joinWays(parent, way);
  }
}

void WayJoiner::joinWays(const WayPtr &parent, const WayPtr &child)
{
  if (!parent || !child)
    return;

  Tags pTags = parent->getTags();
  Tags cTags = child->getTags();

  //  Remove the split split tag
  cTags.remove(MetadataTags::HootSplitParentId());

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
  //  Second check the tags to make sure that they are still compatible
  if (pTags != cTags)
  {
    if (!pTags.dataOnlyEqual(cTags))
    {
      //  Tags aren't compatible, don't join
      return;
    }
  }

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
