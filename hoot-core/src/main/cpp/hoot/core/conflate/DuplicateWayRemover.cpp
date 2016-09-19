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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "DuplicateWayRemover.h"

// Hoot
#include <hoot/core/Factory.h>
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

void DuplicateWayRemover::apply(shared_ptr<OsmMap>& map)
{
  LOG_INFO("Removing duplicate ways...");
  _map = map;

  // create a map from nodes to ways
  shared_ptr<NodeToWayMap> n2wp = _map->getIndex().getNodeToWayMap();
  NodeToWayMap& n2w = *n2wp;

  WayMap wm = _map->getWays();

  // go through each way and remove duplicate nodes in one way
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); it++)
  {
    const shared_ptr<Way>& w = it->second;
    vector<long> newNodes;
    const vector<long>& nodes = w->getNodeIds();
    for (size_t i = 0; i < nodes.size(); i++)
    {
      if (newNodes.size() == 0 || newNodes[newNodes.size() - 1] != nodes[i])
      {
        newNodes.push_back(nodes[i]);
      }
    }

    if (newNodes.size() < nodes.size())
    {
      w->setNodes(newNodes);
    }
  }

  OsmSchema& schema = OsmSchema::getInstance();
  // go through each way
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); it++)
  {
    long key = it->first;
    const shared_ptr<Way>& w = it->second;
    // if the way isn't in the map anymore (deleted as part of this process) or the way is an
    // area type (different treatment).
    if (_map->containsWay(key) == false || !schema.isLinear(*w))
    {
      continue;
    }

    // create a map of all the ways that share nodes with this way and the number of nodes shared
    std::map<long, int> nodesSharedCount;
    const vector<long>& nodes = w->getNodeIds();
    for (size_t i = 0; i < nodes.size(); i++)
    {
      const set<long>& ways = n2w[nodes[i]];
      for (set<long>::iterator wit = ways.begin(); wit != ways.end(); wit++)
      {
        if (*wit != w->getId() && _isCandidateWay(w))
        {
          nodesSharedCount[*wit]++;
        }
      }
    }

    for (std::map<long, int>::iterator wit = nodesSharedCount.begin();
      wit != nodesSharedCount.end(); wit++)
    {
      // if a way shares 2 or more nodes
      if (wit->second >= 2 && _map->containsWay(wit->first) && _map->containsWay(w->getId()))
      {
        // remove duplicates between the ways
        shared_ptr<Way> w2 = _map->getWay(wit->first);

        // if this is a candidate for de-duping
        if (_isCandidateWay(w2))
        {
          //LOG_DEBUG("candidate way tags:");
          //LOG_VARD(w->getTags());
          //LOG_VARD(w2->getTags());

          bool nonNameTagsIdentical = false;
          if (_strictTagMatching)
          {
            nonNameTagsIdentical =
              TagComparator::getInstance().nonNameTagsExactlyMatch(w->getTags(), w2->getTags());
          }

          if (nonNameTagsIdentical || !_strictTagMatching)
          {
            //LOG_DEBUG("Ways have exact non-name tag match or strict tag matching is disabled.");

            if (w->getNodeCount() > w2->getNodeCount())
            {
              _removeDuplicateNodes(w, w2);
            }
            else
            {
              _removeDuplicateNodes(w2, w);
            }
          }
        }
      }
    }
  }
}

bool DuplicateWayRemover::_isCandidateWay(const ConstWayPtr& w) const
{
  bool result = false;

  const OsmMapIndex& index = _map->getIndex();
  OsmSchema& schema = OsmSchema::getInstance();

  // is this a linear way
  if (schema.isLinear(*w) &&
      // if this is not part of a relation
      index.getParents(w->getElementId()).size() == 0)
  {
    result = true;
  }

  return result;
}


void DuplicateWayRemover::_removeDuplicateNodes(shared_ptr<Way> w1, shared_ptr<Way> w2)
{
  LongestCommonNodeString lcs(w1, w2);

  //If the ways have any common geometry, then merge their tags.

  int length = lcs.apply();
  if (length > 1)
  {
    const Tags mergedTags =
      TagMergerFactory::getInstance().mergeTags(w1->getTags(), w2->getTags(), ElementType::Way);
    //w1->setTags(mergedTags);
    w2->setTags(mergedTags);
    _removeNodes(w1, lcs.getW1Index(), length);
  }
  else
  {
    bool rev1 = false, rev2 = false;
    if (OsmSchema::getInstance().isOneWay(*w1) == false)
    {
      w1->reverseOrder();
      rev1 = true;
    }
    else if (OsmSchema::getInstance().isOneWay(*w2) == false)
    {
      w2->reverseOrder();
      rev2 = true;
    }

    int length = lcs.apply();
    if (length > 1)
    {
      const Tags mergedTags =
        TagMergerFactory::getInstance().mergeTags(w1->getTags(), w2->getTags(), ElementType::Way);
      //w1->setTags(mergedTags);
      w2->setTags(mergedTags);
      _removeNodes(w1, lcs.getW1Index(), length);
    }
    else
    {
      // if we didn't find any matches then reverse it back to the original direction
      if (rev1)
      {
        w1->reverseOrder();
      }
      if (rev2)
      {
        w2->reverseOrder();
      }
    }
  }
}

void DuplicateWayRemover::removeDuplicates(shared_ptr<OsmMap> map)
{
  DuplicateWayRemover a;
  a.apply(map);
}

void DuplicateWayRemover::_removeNodes(shared_ptr<const Way> w, int start, int length)
{
  //LOG_DEBUG("Ways have common node(s)");

  const std::vector<long>& nodes = w->getNodeIds();

  Meters ce = w->getRawCircularError();

  // if we're not deleting all the nodes
  if (length != (int)nodes.size())
  {
    // If the start is at the beginning of the way
    if (start == 0)
    {
      vector<long> newNodes(nodes.begin() + length - 1, nodes.end());
      shared_ptr<Way> newWay(new Way(w->getStatus(), _map->createNextWayId(), ce));
      newWay->addNodes(newNodes);
      newWay->setTags(w->getTags());

      _map->replace(w, newWay);
    }
    // if the end as at the end of the way
    else if (start + length == (int)nodes.size())
    {
      vector<long> newNodes(nodes.begin(), nodes.begin() + start + 1);
      shared_ptr<Way> newWay(new Way(w->getStatus(), _map->createNextWayId(), ce));
      newWay->addNodes(newNodes);
      newWay->setTags(w->getTags());

      _map->replace(w, newWay);
    }
    // If the section to remove is in the middle of the way
    else
    {
      vector<long> newNodes1(nodes.begin(), nodes.begin() + start + 1);
      shared_ptr<Way> newWay1(new Way(w->getStatus(), _map->createNextWayId(), ce));
      newWay1->addNodes(newNodes1);
      newWay1->setTags(w->getTags());

      vector<long> newNodes2(nodes.begin() + start + length - 1, nodes.end());
      shared_ptr<Way> newWay2(new Way(w->getStatus(), _map->createNextWayId(), ce));
      newWay2->addNodes(newNodes2);
      newWay2->setTags(w->getTags());

      _replaceMultiple(w, newWay1, newWay2);
    }
  }
  // if we're removing all the nodes, then just remove the way.
  else
  {
    RemoveWayOp::removeWayFully(_map, w->getId());
  }
}

void DuplicateWayRemover::_replaceMultiple(const shared_ptr<const Way>& oldWay,
  const shared_ptr<Way>& newWay1, const shared_ptr<Way>& newWay2)
{
  RelationData::Entry newValues[2];
  newValues[0].setElementId(ElementId::way(newWay1->getId()));
  newValues[1].setElementId(ElementId::way(newWay2->getId()));

  RelationData::Entry old(oldWay->getElementId());

  _map->addWay(newWay1);
  _map->addWay(newWay2);

  // make a copy just in case the index changes while we're replacing elements.
  set<long> rids = _map->getIndex().getElementToRelationMap()->getRelationByElement(oldWay.get());
  for (set<long>::const_iterator it = rids.begin(); it != rids.end(); it++)
  {
    const shared_ptr<Relation>& r = _map->getRelation(*it);
    if (r)
    {
      r->replaceElements(old, newValues, newValues + 2);
    }
  }

  RemoveWayOp::removeWay(_map, oldWay->getId());
}

}
