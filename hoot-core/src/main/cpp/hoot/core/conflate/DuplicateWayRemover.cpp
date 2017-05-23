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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "DuplicateWayRemover.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/algorithms/LongestCommonNodeString.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/ops/RemoveWayOp.h>

// Standard
#include <iostream>
#include <map>
#include <vector>

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

namespace hoot
{
using namespace std;

HOOT_FACTORY_REGISTER(OsmMapOperation, DuplicateWayRemover)

DuplicateWayRemover::DuplicateWayRemover()
{
  setStrictTagMatching(ConfigOptions().getDuplicateWayRemoverStrictTagMatching());
}

void DuplicateWayRemover::apply(OsmMapPtr& map)
{
  _map = map;

  // create a map from nodes to ways
  boost::shared_ptr<NodeToWayMap> n2wp = _map->getIndex().getNodeToWayMap();
  NodeToWayMap& n2w = *n2wp;

  WayMap wm = _map->getWays();

  // go through each way and remove duplicate nodes in one way
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    const WayPtr& w = it->second;
    vector<long> newNodes;
    const vector<long>& nodes = w->getNodeIds();
    for (size_t i = 0; i < nodes.size(); i++)
    {
      if (newNodes.size() == 0 || newNodes[newNodes.size() - 1] != nodes[i])
        newNodes.push_back(nodes[i]);
    }

    if (newNodes.size() < nodes.size())
      w->setNodes(newNodes);
  }

  OsmSchema& schema = OsmSchema::getInstance();
  // go through each way
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    long key = it->first;
    const WayPtr& w = it->second;
    // if the way isn't in the map anymore (deleted as part of this process) or the way is an
    // area type (different treatment).
    if (_map->containsWay(key) == false || !schema.isLinear(*w))
      continue;

    // create a map of all the ways that share nodes with this way and the number of nodes shared
    std::map<long, int> nodesSharedCount;
    const vector<long>& nodes = w->getNodeIds();
    for (size_t i = 0; i < nodes.size(); i++)
    {
      const set<long>& ways = n2w[nodes[i]];
      for (set<long>::iterator wit = ways.begin(); wit != ways.end(); ++wit)
      {
        if (*wit != w->getId() && _isCandidateWay(w))
          nodesSharedCount[*wit]++;
      }
    }

    for (std::map<long, int>::iterator wit = nodesSharedCount.begin();
      wit != nodesSharedCount.end(); ++wit)
    {
      // if a way shares 2 or more nodes
      if (wit->second >= 2 && _map->containsWay(wit->first) && _map->containsWay(w->getId()))
      {
        // remove duplicates between the ways
        WayPtr w2 = _map->getWay(wit->first);

        // if this is a candidate for de-duping
        if (_isCandidateWay(w2))
        {
          LOG_TRACE("candidate way tags:");
          LOG_VART(w->getTags());
          LOG_VART(w2->getTags());

          bool nonNameTagsIdentical = false;
          if (_strictTagMatching)
          {
            nonNameTagsIdentical =
              TagComparator::getInstance().nonNameTagsExactlyMatch(w->getTags(), w2->getTags());
          }

          if (nonNameTagsIdentical || !_strictTagMatching)
          {
            LOG_TRACE("Ways have exact non-name tag match or strict tag matching is disabled.");

            if (w->getNodeCount() > w2->getNodeCount())
              _splitDuplicateWays(w, w2);
            else
              _splitDuplicateWays(w2, w);
          }
        }
      }
    }
  }
}

bool DuplicateWayRemover::_isCandidateWay(const ConstWayPtr& w) const
{
  // is this a linear way
  return (OsmSchema::getInstance().isLinear(*w) &&
      // if this is not part of a relation
      _map->getIndex().getParents(w->getElementId()).size() == 0);
}


void DuplicateWayRemover::_splitDuplicateWays(WayPtr w1, WayPtr w2, bool rev1, bool rev2)
{
  // If the ways have any common geometry, then merge their tags.
  LongestCommonNodeString lcs(w1, w2);
  int length = lcs.apply();
  if (length > 1)
  {
    const Tags mergedTags =
      TagMergerFactory::getInstance().mergeTags(w1->getTags(), w2->getTags(), ElementType::Way);
    const vector<long>& nodes1 = w1->getNodeIds();
    const vector<long>& nodes2 = w2->getNodeIds();
    //  _splitDuplicateWays is always called where num_nodes(w1) >= num_nodes(w2) so the following logic works
    if (nodes1.size() == nodes2.size() && nodes1.size() == static_cast<vector<long>::size_type>(length))
    {
      //  Merge the two ways' tags
      w1->setTags(mergedTags);
      //  Remove w2
      _replaceMultiple(w2, vector<WayPtr>());
    }
    else if (nodes2.size() == static_cast<vector<long>::size_type>(length))
    {
      //  Way 2 is completely engulfed in way 1, shrink way 1 and leave way 2 intact
      vector<WayPtr> ways1 = _splitWay(w1, lcs.getW1Index(), length, false);
      ways1.push_back(w2);
      //  Merge the tags into w2
      w2->setTags(mergedTags);
      //  Replace w1 with both w1 and w2
      _replaceMultiple(w1, ways1);
    }
    else
    {
      vector<long> newNodes(nodes1.begin() + lcs.getW1Index(), nodes1.begin() + lcs.getW1Index() + length);
      //  Split w1 with all new ids for the results
      vector<WayPtr> ways1 = _splitWay(w1, lcs.getW1Index(), length, true);
      //  Split w2 using the old id and new id(s)
      vector<WayPtr> ways2 = _splitWay(w2, lcs.getW2Index(), length, false);
      //  Once w1 is split, update the original to have the merged tags
      w1->setTags(mergedTags);
      w1->setNodes(newNodes);
      //  Add w1 to the list of ways to replace w1 with
      ways1.push_back(w1);
      _replaceMultiple(w1, ways1);
      //  Add w1 to the list of ways to replace w2 with
      ways2.push_back(w1);
      _replaceMultiple(w2, ways2);
    }
  }
  else if (!rev1 && !rev2)
  {
    //  Reverse one of the ways and try again
    if (OsmSchema::getInstance().isOneWay(*w1) == false)
    {
      w1->reverseOrder();
      _splitDuplicateWays(w1, w2, true, false);
    }
    else if (OsmSchema::getInstance().isOneWay(*w2) == false)
    {
      w2->reverseOrder();
      _splitDuplicateWays(w1, w2, false, true);
    }
  }
  else
  {
    // if we didn't find any matches then reverse it back to the original direction
    if (rev1)
      w1->reverseOrder();
    else
      w2->reverseOrder();
  }
}

void DuplicateWayRemover::removeDuplicates(OsmMapPtr map)
{
  DuplicateWayRemover a;
  a.apply(map);
}

vector<WayPtr> DuplicateWayRemover::_splitWay(WayPtr w, int start, int length, bool newIds)
{
  LOG_TRACE("Ways have common node(s)");
  const std::vector<long>& nodes = w->getNodeIds();
  vector<WayPtr> results;
  // if we're not deleting all the nodes
  if (length != (int)nodes.size())
  {
    // If the start is at the beginning of the way
    if (start == 0)
    {
      vector<long> newNodes(nodes.begin() + length - 1, nodes.end());
      results.push_back(_getUpdatedWay(w, newNodes, newIds));
    }
    // if the end as at the end of the way
    else if (start + length == (int)nodes.size())
    {
      vector<long> newNodes(nodes.begin(), nodes.begin() + start + 1);
      results.push_back(_getUpdatedWay(w, newNodes, newIds));
    }
    // If the section to remove is in the middle of the way
    else
    {
      //  Can only keep one way the same, the first one needs a new id
      vector<long> newNodes1(nodes.begin() + start + length - 1, nodes.end());
      results.push_back(_getUpdatedWay(w, newNodes1, true));
      //  Potentially update the second or create a new one (newIds)
      vector<long> newNodes2(nodes.begin(), nodes.begin() + start + 1);
      results.push_back(_getUpdatedWay(w, newNodes2, newIds));
    }
  }
  return results;
}

WayPtr DuplicateWayRemover::_getUpdatedWay(WayPtr way, const vector<long>& nodes, bool newIds)
{
  if (newIds)
  {
    //  Create a new way, update it, and add it to the map
    WayPtr newWay;
    newWay.reset(new Way(way->getStatus(), _map->createNextWayId(), way->getRawCircularError()));
    newWay->addNodes(nodes);
    newWay->setTags(way->getTags());
    _map->addWay(newWay);
    return newWay;
  }
  else
  {
    //  Update the current way
    way->setNodes(nodes);
    return way;
  }
}

void DuplicateWayRemover::_replaceMultiple(const ConstWayPtr& oldWay,
  const std::vector<WayPtr>& ways)
{
  if (ways.size() < 1)
  {
    RemoveWayOp::removeWay(_map, oldWay->getId());
    return;
  }

  boost::shared_ptr<RelationData::Entry[]> newValues(new RelationData::Entry[ways.size()]);
  for (vector<WayPtr>::size_type i = 0; i < ways.size(); ++i)
    newValues.get()[i].setElementId(ways[i]->getElementId());

  RelationData::Entry old(oldWay->getElementId());

  //  Make a copy just in case the index changes while we're replacing elements.
  set<long> rids = _map->getIndex().getElementToRelationMap()->getRelationByElement(oldWay.get());
  for (set<long>::const_iterator it = rids.begin(); it != rids.end(); ++it)
  {
    const RelationPtr& r = _map->getRelation(*it);
    if (r)
      r->replaceElements(old, newValues.get(), newValues.get() + ways.size());
  }
}

}
