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

#include "NodeMatcher.h"

// Hoot
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/RiverCriterion.h>
#include <hoot/core/criterion/PowerLineCriterion.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>


// Tgs
#include <tgs/Statistics/Normal.h>

// Qt


using namespace std;
using namespace Tgs;

namespace hoot
{

int NodeMatcher::logWarnCount = 0;
QList<std::shared_ptr<ElementCriterion>> NodeMatcher::_networkFeatureTypeCriteria;

NodeMatcher::NodeMatcher() :
_strictness(ConfigOptions().getNodeMatcherStrictness()),
_delta(ConfigOptions().getNodeMatcherAngleCalcDelta())
{
}

QStringList NodeMatcher::getNetworkCriterionClassNames()
{
  // TODO: Should LinearCriterion be added here?
  QStringList critClasses;
  critClasses.append(HighwayCriterion::className());
  critClasses.append(RiverCriterion::className());
  critClasses.append(PowerLineCriterion::className());
  critClasses.append(RailwayCriterion::className());
  return critClasses;
}

bool NodeMatcher::isNetworkFeatureType(ConstElementPtr element)
{
  if (_networkFeatureTypeCriteria.isEmpty())
  {
    const QStringList critClasses = getNetworkCriterionClassNames();
    for (int i = 0; i < critClasses.size(); i++)
    {
      _networkFeatureTypeCriteria.append(
        Factory::getInstance().constructObject<ElementCriterion>(critClasses.at(i)));
    }
  }

  for (int i = 0; i < _networkFeatureTypeCriteria.size(); i++)
  {
    if (_networkFeatureTypeCriteria.at(i)->isSatisfied(element))
    {
      LOG_TRACE(
        element->getElementId() << " recognized as network feature types by " <<
        _networkFeatureTypeCriteria.at(i)->toString() << ".");
      return true;
    }
  }
  return false;
}

vector<Radians> NodeMatcher::calculateAngles(const OsmMap* map, long nid,
                                             const set<long>& wids, Meters delta)
{
  vector<Radians> result;
  result.reserve(wids.size());

  LOG_VART(nid);
  ConstNodePtr node = map->getNode(nid);
  LOG_VART(node);
  QSet<long> badWayIds;
  for (set<long>::const_iterator it = wids.begin(); it != wids.end(); ++it)
  {
    const ConstWayPtr& w = map->getWay(*it);
    LOG_VART(w->getId());
    LOG_VART(w->getLastNodeId());
    LOG_VART(w->getNodeId(0));

    if (!isNetworkFeatureType(w))
    {
      // if this isn't a feature from a specific list, then don't consider it.
      LOG_TRACE("calculateAngles skipping feature...");
    }
    else if (w->getNodeId(0) == nid)
    {
      LOG_TRACE("Start node: " << nid);
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
      LOG_VART(heading);
      result.push_back(heading);
    }
    else if (w->getLastNodeId() == nid)
    {
      LOG_TRACE("End node: " << nid);
      WayLocation wl(map->shared_from_this(), w, w->getNodeCount() - 1, 1.0);
      Radians heading = WayHeading::calculateHeading(wl, delta);
      LOG_VART(heading);
      result.push_back(heading);
    }
    else
    {
      // count this as a bad spot. If we find some valid spots and some bad spots then that is an
      // error condition
      badWayIds.insert(w->getId());
    }
  }

  LOG_VART(badWayIds.size());
  LOG_VART(result.size());
  if (!result.empty() && !badWayIds.empty())
  {
    LOG_TRACE(
      "Found " << badWayIds.size() << " bad spot(s) in NodeMatcher when calculating angles " <<
      "with node: " << nid);
    LOG_TRACE("wids: " << badWayIds);
    for (QSet<long>::const_iterator it = badWayIds.begin(); it != badWayIds.end(); ++it)
    {
      const ConstWayPtr& w = map->getWay(*it);
      LOG_VART(w->getId());
      LOG_VART(w->getTags().get("REF1"));
      LOG_VART(w->getTags().get("REF2"));
      LOG_VART(w->getNodeIndex(nid));
      LOG_VART(w->getNodeId(0));
      LOG_VART(w->getLastNodeId());
    }

    if (ConfigOptions().getNodeMatcherFailOnBadAngleSpots())
    {
      throw HootException(
        QString("NodeMatcher::calculateAngles was called with a node that was not a start or ") +
        QString("end node on the specified way."));
    }
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

int NodeMatcher::getDegree(ElementId nid) const
{
  return (int)_map->getIndex().getNodeToWayMap()->at(nid.getId()).size();
}

double NodeMatcher::scorePair(long nid1, long nid2)
{
  ConstNodePtr n1 = _map->getNode(nid1);
  LOG_VART(n1->getElementId());
  ConstNodePtr n2 = _map->getNode(nid2);
  LOG_VART(n2->getElementId());

  const set<long>& wids1 = _map->getIndex().getNodeToWayMap()->at(nid1);
  LOG_VART(wids1);
  const set<long>& wids2 = _map->getIndex().getNodeToWayMap()->at(nid2);
  LOG_VART(wids2);

  double acc = 0;
  for (set<long>::const_iterator it = wids1.begin(); it != wids1.end(); ++it)
  {
    acc = max(acc, _map->getWay(*it)->getCircularError());
  }
  for (set<long>::const_iterator it = wids2.begin(); it != wids2.end(); ++it)
  {
    acc = max(acc, _map->getWay(*it)->getCircularError());
  }
  LOG_VART(acc);

  vector<Radians> theta1 = calculateAngles(_map.get(), nid1, wids1, _delta);
  LOG_VART(theta1);
  vector<Radians> theta2 = calculateAngles(_map.get(), nid2, wids2, _delta);
  LOG_VART(theta2);

  int s1 = theta1.size();
  LOG_VART(s1);
  int s2 = theta2.size();
  LOG_VART(s2);

  if (s1 < 3 || s2 < 3)
  {
    return 0.0;
  }

  double d = n1->toCoordinate().distance(n2->toCoordinate());

  // TODO: this isnt right; Talk to mike
  double distanceScore = 1 - (Normal::phi(d, acc * 1.5) - 0.5) * 2.0;
  LOG_VART(nid1);
  LOG_VART(nid2);
  LOG_VART(distanceScore);
  LOG_VART(acc);
  LOG_VART(d);
  LOG_VART(Normal::phi(d, acc * 1.5));
  LOG_VART(Normal::phi(d, acc / 2.0));

  if (theta1.size() < theta2.size())
  {
    swap(theta1, theta2);
  }

  double thetaScore;
  // this is very unsual and will slow things down.
  if (theta1.size() > 6 && theta2.size() > 6)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_DEBUG(
        "Greater than seven intersections at one spot? Odd.  Giving it a high angle score.");
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_DEBUG(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
    LOG_VART(nid1);
    LOG_VART(nid2);
    LOG_VART(wids1);
    LOG_VART(wids2);
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
  int diff = abs(s1 - s2);

  double result = (min(s1, s2) - diff) * thetaScore * distanceScore;

  LOG_VART(result);
  return result;
}

}

