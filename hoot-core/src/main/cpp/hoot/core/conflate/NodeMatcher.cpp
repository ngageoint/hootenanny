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

#include "NodeMatcher.h"

// Hoot
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/util/OsmUtils.h>

// Tgs
#include <tgs/Statistics/Normal.h>

namespace hoot
{

NodeMatcher::NodeMatcher()
{
  _strictness = ConfigOptions().getNodeMatcherStrictness();
}

vector<Radians> NodeMatcher::calculateAngles(const OsmMap* map, long nid, const set<long>& wids, Meters delta)
{
  vector<Radians> result;
  result.reserve(wids.size());

  int badSpots = 0;

  for (set<long>::const_iterator it = wids.begin(); it != wids.end(); ++it)
  {
    const shared_ptr<const Way>& w = map->getWay(*it);

    if (OsmSchema::getInstance().isLinearHighway(w->getTags(), w->getElementType()) == false &&
        OsmSchema::getInstance().isLinearWaterway(*w) == false)
    {
      // if this isn't a highway or waterway, then don't consider it.
      //LOG_DEBUG("calculateAngles skipping feature");
    }
    else if (w->getNodeId(0) == nid)
    {
      WayLocation wl(map->shared_from_this(), w, 0, 0.0);
      Radians heading = WayHeading::calculateHeading(wl, delta);
      // This is the first node so the angle is an inbound angle, reverse the value.
      if (heading < 0.0)
      {
        heading += M_PI;
      }
      else
      {
        heading -= M_PI;
      }
      result.push_back(heading);
    }
    else if (w->getLastNodeId() == nid)
    {
      WayLocation wl(map->shared_from_this(), w, w->getNodeCount() - 1, 1.0);
      result.push_back(WayHeading::calculateHeading(wl, delta));
    }
    else
    {
      LOG_VARD(w->getNodeId(0));
      LOG_VARD(w->getLastNodeId());
      // count this as a bad spot. If we find some valid spots and some bad spots then that is an
      // error condition
      badSpots++;
    }
  }

  if (result.size() > 0 && badSpots > 0)
  {
    LOG_WARN("nid: " << nid);
    LOG_WARN(map->getNode(nid)->toString());
    LOG_WARN("wids: " << wids);
    for (set<long>::const_iterator it = wids.begin(); it != wids.end(); ++it)
    {
      LOG_WARN(map->getWay(*it)->toString());
    }

    //shared_ptr<OsmMap> copy(new OsmMap(*map));
    //MapProjector::reprojectToWgs84(copy);
    //OsmUtils::saveMap(copy, "/data/river-data/NodeMatcherMap-temp.osm");

    throw HootException("calculateAngles was called with a node that was not a start or end node"
                        " on the specified way.");
  }

  return result;
}

double NodeMatcher::_calculateAngleScore(const vector<Radians>& theta1,
  const vector<Radians>& theta2, vector<bool>& exclude, size_t depth, bool debug)
{
  assert(theta1.size() <= theta2.size());
  if (depth == theta1.size())
  {
    return 1.0;
  }

  double max = 0;
  for (int j = 0; j < (int)theta2.size(); j++)
  {
    if (exclude[j] == false)
    {
      Radians m = WayHeading::deltaMagnitude(theta1[depth], theta2[j]);
      double r = 0;
      if (m < M_PI / 2.0)
      {
        r = pow(cos(WayHeading::deltaMagnitude(theta1[depth], theta2[j])), _strictness);
      }
      exclude[j] = true;
      double v = r * _calculateAngleScore(theta1, theta2, exclude, depth + 1, debug);
      exclude[j] = false;
      if (v > max)
      {
        max = v;
      }
    }
  }
  return max;
}

int NodeMatcher::getDegree(ElementId nid)
{
  return (int)_map->getIndex().getNodeToWayMap()->at(nid.getId()).size();
}

double NodeMatcher::scorePair(long nid1, long nid2)
{
  shared_ptr<const Node> n1 = _map->getNode(nid1);
  shared_ptr<const Node> n2 = _map->getNode(nid2);

  const set<long>& wids1 = _map->getIndex().getNodeToWayMap()->at(nid1);
  const set<long>& wids2 = _map->getIndex().getNodeToWayMap()->at(nid2);

  double acc = 0;
  for (set<long>::const_iterator it = wids1.begin(); it != wids1.end(); ++it)
  {
    acc = max(acc, _map->getWay(*it)->getCircularError());
  }
  for (set<long>::const_iterator it = wids2.begin(); it != wids2.end(); ++it)
  {
    acc = max(acc, _map->getWay(*it)->getCircularError());
  }

  vector<Radians> theta1 = calculateAngles(_map.get(), nid1, wids1);
  vector<Radians> theta2 = calculateAngles(_map.get(), nid2, wids2);

  int s1 = theta1.size();
  int s2 = theta2.size();

  if (s1 < 3 || s2 < 3)
  {
    return 0.0;
  }

  double d = n1->toCoordinate().distance(n2->toCoordinate());

  /// @todo this isnt right; Talk to mike
  double distanceScore = 1 - (Normal::phi(d, acc * 1.5) - 0.5) * 2.0;
  LOG_VARD(nid1);
  LOG_VARD(nid2);
  LOG_VARD(distanceScore);
  LOG_VARD(acc);
  LOG_VARD(d);
  LOG_VARD(Normal::phi(d, acc * 1.5));
  LOG_VARD(Normal::phi(d, acc / 2.0));

  if (theta1.size() < theta2.size())
  {
    swap(theta1, theta2);
  }

  double thetaScore;
  // this is very unsual and will slow things down.
  if (theta1.size() > 6 && theta2.size() > 6)
  {
    LOG_WARN("7 intersections at one spot? Odd. Giving it a high angleScore.");
    LOG_VAR(nid1);
    LOG_VAR(nid2);
    LOG_VAR(wids1);
    LOG_VAR(wids2);
    thetaScore = 1.0;
  }
  else
  {
    if (theta2.size() < theta1.size())
    {
      vector<bool> exclude(theta2.size(), false);
      thetaScore = _calculateAngleScore(theta2, theta1, exclude, 0);
    }
    else
    {
      vector<bool> exclude(theta1.size(), false);
      thetaScore = _calculateAngleScore(theta1, theta2, exclude, 0);
    }
  }

  // simple stupid heuristic. Replace w/ some cosine fanciness later.
  int diff = abs((int)s1 - (int)s2);

  double result = (min(s1, s2) - diff) * thetaScore * distanceScore;

  LOG_VARD(result);
  return result;
}

}

