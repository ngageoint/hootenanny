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

#include "UnlikelyIntersectionRemover.h"

// Hoot
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/conflate/ConflateUtils.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

// Standard
#include <iostream>

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, UnlikelyIntersectionRemover)

void UnlikelyIntersectionRemover::apply(std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;
  _result = map;

  NodeToWayMap n2w(*_result);

  // go through each node
  for (NodeToWayMap::const_iterator it = n2w.begin(); it != n2w.end(); ++it)
  {
    // if the node is part of multiple ways
    if (it->second.size() > 1)
    {
      // evaluate and split the intersection
      _evaluateAndSplit(it->first, it->second);
    }
  }
}

void UnlikelyIntersectionRemover::_evaluateAndSplit(long intersectingNode, const set<long>& wayIds)
{
  // This evaluate and split method uses a simple heuristic for finding a split. It also assumes
  // that there are at most two intersections at a given node. While this is reasonable for most
  // real world scenarios there may be some edge cases where this falls apart (e.g. an overpass
  // over a ground level road, over a tunnel).
  //
  // The split heuristic simply looks at the first way vs. all the other ways. In complex
  // situations with more than two ways this may be non-optimal, but again, for most real world
  // scenarios this should be just fine.

  // Create two groups for the ways.
  vector<std::shared_ptr<Way>> g1, g2;

  LOG_VART(intersectingNode);
  LOG_VART(wayIds);

  // Put the first valid way in the first group.
  set<long>::const_iterator it = wayIds.begin();
  std::shared_ptr<Way> first;
  while (!first && it != wayIds.end())
  {
    WayPtr way = _result->getWay(*it);
    if (way)
    {
      g1.push_back(way);
      first = g1[0];
    }
    ++it;
  }
  LOG_VART(first.get());
  if (!first)
  {
    return;
  }

  // Go through all the other ways.
  for (set<long>::const_iterator way_it = wayIds.begin(); way_it != wayIds.end(); ++way_it)
  {
    std::shared_ptr<Way> w = _result->getWay(*way_it);
    _numProcessed++;
    if (!w)
    {
      continue;
    }
    // Since this class operates on elements with generic types, an additional check must be
    // performed here during conflation to enure we don't modify any element not associated with
    // an active conflate matcher in the current conflation configuration.
    else if (_conflateInfoCache &&
             !_conflateInfoCache->elementCanBeConflatedByActiveMatcher(w, className()))
    {
      LOG_TRACE(
        "Skipping processing of " << w->getElementId() << ", as it cannot be conflated by any " <<
        "actively configured conflate matcher...");
      continue;
    }

    if (w && w->getElementId() != first->getElementId())
    {
      const double p = _pIntersection(intersectingNode, first, w);
      // If this is a likely intersection with the first way,
      if (p > 0.5)
      {
        // put it in the first group.
        g1.push_back(w);
      }
      // If this is an unlikely intersection with the first way,
      else
      {
        // put it in the second group.
        g2.push_back(w);
      }
    }
  }

  // If all ways aren't in the first group, split the intersection into two groups
  if (!g2.empty())
  {
    _numAffected = g2.size();
    LOG_TRACE("Splitting intersection for ways: " << g2 << " at node " << intersectingNode);
    _splitIntersection(intersectingNode, g2);
  }
}

double UnlikelyIntersectionRemover::_pIntersection(long intersectingNode,
                                                   const std::shared_ptr<Way>& w1,
                                                   const std::shared_ptr<Way>& w2)
{
  // presume it is a valid intersection
  double p = 1.0;

  LOG_VART(intersectingNode);
  LOG_VART(w1->getElementId());
  LOG_VART(w2->getElementId());

  int i1 = w1->getNodeIndex(intersectingNode);
  int i2 = w2->getNodeIndex(intersectingNode);
  LOG_VART(i1);
  LOG_VART(i2);
  // if either node is within the way (not an end node)
  if ((i1 != 0 && i1 != (int)w1->getNodeCount() - 1) ||
      (i2 != 0 && i2 != (int)w2->getNodeCount() - 1))
  {
    // if bridge or tunnel doesn't match
    if (w1->getTags()["bridge"] != w2->getTags()["bridge"] ||
        w1->getTags()["tunnel"] != w2->getTags()["tunnel"])
    {
      p *= .2;
      LOG_VART(p);
    }
    // if one is a motorway and the other isn't
    if ((w1->getTags()["highway"] == "motorway") != (w2->getTags()["highway"] == "motorway"))
    {
      // if one is a motorway and the other is a motorway_link
      if (w1->getTags()["highway"].startsWith("motorway") !=
          w2->getTags()["highway"].startsWith("motorway"))
      {
        p *= .4;
        LOG_VART(p);
      }
    }
  }
  LOG_VART(p);

  Radians h1 = WayHeading::calculateHeading(WayLocation(_result, w1, i1, 0.0));
  LOG_VART(h1);
  Radians h2 = WayHeading::calculateHeading(WayLocation(_result, w2, i2, 0.0));
  LOG_VART(h2);
  Degrees d = toDegrees(WayHeading::deltaMagnitude(h1, h2));
  d = min(fabs(d - 180), d);
  LOG_VART(d);
  // if angle is > 45deg
  if (d > 45)
  {
    // if bridge or tunnel doesn't match
    if (w1->getTags()["bridge"] != w2->getTags()["bridge"] ||
        w1->getTags()["tunnel"] != w2->getTags()["tunnel"])
    {
      p *= .2;
      LOG_VART(p);
    }
    // if one is a motorway and the other isn't
    if ((w1->getTags()["highway"] == "motorway") != (w2->getTags()["highway"] == "motorway"))
    {
      // if one is a motorway and the other is a motorway_link
      if (w1->getTags()["highway"].startsWith("motorway") !=
          w2->getTags()["highway"].startsWith("motorway"))
      {
        p *= .4;
        LOG_VART(p);
      }
    }
  }

  LOG_VART(p);
  return p;
}

void UnlikelyIntersectionRemover::removeIntersections(std::shared_ptr<OsmMap> map)
{
  UnlikelyIntersectionRemover uir;
  uir.apply(map);
}

void UnlikelyIntersectionRemover::_splitIntersection(long intersectingNode,
                                                     const vector<std::shared_ptr<Way>>& g2)
{
  LOG_VART(intersectingNode);
  LOG_VART(g2.size());

  NodePtr oldNode = _result->getNode(intersectingNode);
  LOG_VART(oldNode.get());
  if (!oldNode)
  {
    return;
  }
  LOG_VART(oldNode->getElementId());
  // create a copy of the intersecting node
  NodePtr newNode(
    new Node(
      oldNode->getStatus(), _result->createNextNodeId(), oldNode->toCoordinate(),
      oldNode->getCircularError()));
  newNode->setTags(oldNode->getTags());
  LOG_VART(newNode->getElementId());
  _result->addNode(newNode);

  // all ways in group one are unchanged

  // all ways in group 2 get the intersecting node replaced by the new node.
  for (size_t i = 0; i < g2.size(); i++)
  {
    LOG_VART(g2[i]->getElementId());
    g2[i]->replaceNode(intersectingNode, newNode->getId());
  }
}

QStringList UnlikelyIntersectionRemover::getCriteria() const
{
  return QStringList(LinearCriterion::className());
}

}
