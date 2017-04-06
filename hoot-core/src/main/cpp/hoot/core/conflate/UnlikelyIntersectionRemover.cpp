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

#include "UnlikelyIntersectionRemover.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>

// Standard
#include <iostream>

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

#include "NodeToWayMap.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, UnlikelyIntersectionRemover)

UnlikelyIntersectionRemover::UnlikelyIntersectionRemover()
{

}

void UnlikelyIntersectionRemover::_evaluateAndSplit(long intersectingNode, const set<long>& wayIds)
{
  //
  // This evaluate and split method uses a simple heuristic for finding a split. It also assumes
  // that there are at most two intersections at a given node. While this is reasonable for most
  // real world scenarios there may be some edge cases where this falls apart (e.g. an overpass
  // over a ground level road, over a tunnel).
  //
  // The split heuristic simply looks at the first way vs. all the other ways. In complex
  // situations with more than two ways this may be non-optimal, but again, for most real world
  // scenarios this should be just fine.
  //

  // create two groups for the ways
  vector< boost::shared_ptr<Way> > g1, g2;

  // put the first way in the first group
  g1.push_back(_result->getWay(*wayIds.begin()));

  boost::shared_ptr<Way> first = g1[0];
  // go through all the other ways
  for (set<long>::iterator it = wayIds.begin(); it != wayIds.end(); ++it)
  {
    boost::shared_ptr<Way> w = _result->getWay(*it);
    double p = _pIntersection(intersectingNode, first, w);

    // if this is a likely intersection with the first way
    if (p > 0.5)
    {
      // put it in the first group
      g1.push_back(w);
    }
    // if this is an unlikely intersection with the first way
    else
    {
      // put it in the second group
      g2.push_back(w);
    }
  }

  // if all ways are in the first group, we're done.
  if (g2.size() == 0)
  {
    // pass
  }
  // otherwise split the intersection into two groups.
  else
  {
    _splitIntersection(intersectingNode, g2);
  }
}

double UnlikelyIntersectionRemover::_pIntersection(long intersectingNode, boost::shared_ptr<Way> w1,
                                                 boost::shared_ptr<Way> w2)
{
  // pressume it is a valid intersection
  double p = 1.0;

  int i1 = w1->getNodeIndex(intersectingNode);
  int i2 = w2->getNodeIndex(intersectingNode);
  // if either node is within the way (not an end node)
  if ((i1 != 0 && i1 != (int)w1->getNodeCount() - 1) ||
      (i2 != 0 && i2 != (int)w2->getNodeCount() - 1))
  {
    // if bridge or tunnel doesn't match
    if (w1->getTags()["bridge"] != w2->getTags()["bridge"] ||
        w1->getTags()["tunnel"] != w2->getTags()["tunnel"])
    {
      p *= .2;
    }
    // if one is a motorway and the other isn't
    if ((w1->getTags()["highway"] == "motorway") !=
        (w2->getTags()["highway"] == "motorway"))
    {
      // if one is a motorway and the other is a motorway_link
      if (w1->getTags()["highway"].startsWith("motorway") !=
        w2->getTags()["highway"].startsWith("motorway"))
      {
        p *= .4;
      }
    }
  }

  Radians h1 = WayHeading::calculateHeading(WayLocation(_result, w1, i1, 0.0));
  Radians h2 = WayHeading::calculateHeading(WayLocation(_result, w2, i2, 0.0));
  Degrees d = toDegrees(WayHeading::deltaMagnitude(h1, h2));
  d = min(fabs(d - 180), d);
  // if angle is > 45deg
  if (d > 45)
  {
    // if bridge or tunnel doesn't match
    if (w1->getTags()["bridge"] != w2->getTags()["bridge"] ||
        w1->getTags()["tunnel"] != w2->getTags()["tunnel"])
    {
      p *= .2;
    }
    // if one is a motorway and the other isn't
    if ((w1->getTags()["highway"] == "motorway") != (w2->getTags()["highway"] == "motorway"))
    {
      // if one is a motorway and the other is a motorway_link
      if (w1->getTags()["highway"].startsWith("motorway") !=
        w2->getTags()["highway"].startsWith("motorway"))
      {
        p *= .4;
      }
    }
  }

  return p;
}

void UnlikelyIntersectionRemover::removeIntersections(boost::shared_ptr<OsmMap> map)
{
  UnlikelyIntersectionRemover uir;
  uir.apply(map);
}

void UnlikelyIntersectionRemover::_splitIntersection(long intersectingNode,
                                                     const vector< boost::shared_ptr<Way> >& g2)
{
  boost::shared_ptr<Node> oldNode = _result->getNode(intersectingNode);
  // create a copy of the intersecting node
  boost::shared_ptr<Node> newNode(new Node(oldNode->getStatus(), _result->createNextNodeId(),
    oldNode->toCoordinate(), oldNode->getCircularError()));
  newNode->setTags(oldNode->getTags());
  _result->addNode(newNode);

  // all ways in group one are unchanged

  // all ways in group 2 get the intersecting node replaced by the new node.
  for (size_t i = 0; i < g2.size(); i++)
  {
    g2[i]->replaceNode(intersectingNode, newNode->getId());
  }
}

void UnlikelyIntersectionRemover::apply(boost::shared_ptr<OsmMap>& map)
{
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

}
