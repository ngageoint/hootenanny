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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "MaximalNearestSubline.h"

// GEOS
#include <geos/geom/LineString.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <geos/operation/distance/DistanceOp.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/linearreference/WaySubline.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/algorithms/FindNodesInWayFactory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

Meters MaximalNearestSubline::_headingDelta = -1;

MaximalNearestSubline::MaximalNearestSubline(const ConstOsmMapPtr& map,
                                             ConstWayPtr a,
                                             ConstWayPtr b, Meters minSplitSize,
                                             Meters maxRelevantDistance,
                                             Radians maxRelevantAngle,
                                             Meters headingDelta) :
  _a(a), _b(b),
  _aPtLocator(map, a),
  _map(map)
{
  _a = a;
  _b = b;
  _maxInterval.resize(2);
  _minSplitSize = minSplitSize;
  _maxRelevantDistance = maxRelevantDistance;
  _maxRelevantAngle = maxRelevantAngle;
  _headingDelta = headingDelta;
}

Meters MaximalNearestSubline::_calculateIntervalLength()
{
  Meters result = -1;
  if (_maxInterval[0].isValid() && _maxInterval[1].isValid())
  {
    Meters d0 = _maxInterval[0].calculateDistanceOnWay();
    Meters d1 = _maxInterval[1].calculateDistanceOnWay();
    result = max(d0, d1) - min(d0, d1);
  }

  return result;
}

void MaximalNearestSubline::_expandInterval(WayLocation& loc)
{
  // expand maximal interval if this point is outside it
  if (_maxInterval[0].isValid() == false || loc.compareTo(_maxInterval[0]) < 0)
  {
    _maxInterval[0] = loc;
  }

  if (_maxInterval[1].isValid() == false || loc.compareTo(_maxInterval[1]) > 0)
  {
    _maxInterval[1] = loc;
  }
}

WayPtr MaximalNearestSubline::getMaximalNearestSubline(const OsmMapPtr& map,
    ConstWayPtr a, ConstWayPtr b, Meters minSplitSize, Meters maxRelevantDistance)
{
  MaximalNearestSubline mns(map, a, b, minSplitSize, maxRelevantDistance);

  vector<WayLocation> interval = mns.getInterval();

  if (interval[0].isValid() && interval[1].isValid())
  {
    return WaySubline(interval[0], interval[1]).toWay(map);
  }
  else
  {
    return WayPtr();
  }
}

const vector<WayLocation>& MaximalNearestSubline::getInterval()
{
  vector<WayLocation> testPoints;

  // clear the working interval
  _maxInterval[0] = WayLocation();
  _maxInterval[1] = WayLocation();

  /**
   * The basic strategy is to pick test points on B and find their nearest point on A.
   * The interval containing these nearest points is approximately the MaximalNeareastSubline of A.
   */
  // Heuristic #1: use every vertex of B as a test point
  for (size_t ib = 0; ib < _b->getNodeCount(); ib++)
  {
    Coordinate bPt = _map->getNode(_b->getNodeId(ib))->toCoordinate();
    WayLocation nearestLocationOnA = _aPtLocator.locate(bPt);
    if (nearestLocationOnA.isValid())
    {
      testPoints.push_back(nearestLocationOnA);
    }
  }

  /**
   * Heuristic #2:
   *
   * Find the nearest point on B to all vertices of A and use those points of B as test points.
   * For efficiency use only vertices of A outside current max interval.
   */
  // find all the b points using heuristic 2
  LocationOfPoint bPtLocator(_map, _b);
  for (size_t ia = 0; ia < _a->getNodeCount(); ia++)
  {
    WayLocation bLoc = bPtLocator.locate(_map->getNode(_a->getNodeId(ia))->toCoordinate());
    Coordinate bPt = bLoc.getCoordinate();
    WayLocation nearestLocationOnA = _aPtLocator.locate(bPt);
    if (nearestLocationOnA.isValid())
    {
      testPoints.push_back(nearestLocationOnA);
    }
  }

  // sort the locations
  sort(testPoints.begin(), testPoints.end());

  std::shared_ptr<LineString> bls = ElementToGeometryConverter(_map).convertToLineString(_b);
  double bestLength = -1;
  vector<WayLocation> bestInterval;
  bestInterval.resize(2);

  // start at the beginning and look for the longest contiguous match
  for (size_t i = 0; i < testPoints.size(); i++)
  {
    // if the point is within the specified distance and angle
    if (_isInBounds(testPoints[i], bls))
    {
      // expand the interval
      _expandInterval(testPoints[i]);
    }
    // if the point is outside the specified distance
    else
    {
      Meters l = _calculateIntervalLength();
      // if this interval is the best & it is at least _minSplitSize in length
      if (l > bestLength)
      {
        // save the interval
        bestLength = l;
        bestInterval = _maxInterval;
      }
      // clear the working interval
      _maxInterval[0] = WayLocation();
      _maxInterval[1] = WayLocation();
    }
  }

  Meters l = _calculateIntervalLength();
  // if this interval is the best & it is at least _minSplitSize in length
  if (l > bestLength)
    bestInterval = _maxInterval;

  _maxInterval = bestInterval;

  return _maxInterval;
}

bool MaximalNearestSubline::_isInBounds(const WayLocation& wl,
                                        const std::shared_ptr<LineString>& ls)
{
  // calculate the distance from the test point to b
  std::shared_ptr<Point> tp(GeometryFactory::getDefaultInstance()->createPoint(
      wl.getCoordinate()));

  geos::operation::distance::DistanceOp dop(tp.get(), ls.get());
  bool result = true;

  if (_maxRelevantDistance > 0)
  {
    Meters d = dop.distance();
    if (d > _maxRelevantDistance)
    {
      result = false;
    }
  }

  if (result && _maxRelevantAngle >= 0)
  {
    std::shared_ptr<CoordinateSequence> cs(dop.nearestPoints());

    LocationOfPoint loc(_map, _b);
    WayLocation wl2 = loc.locate(cs->getAt(1));

    Radians h1 = WayHeading::calculateHeading(wl, _headingDelta);
    Radians h2 = WayHeading::calculateHeading(wl2, _headingDelta);

    if (WayHeading::deltaMagnitude(h1, h2) > _maxRelevantAngle)
    {
      result = false;
    }
  }

  return result;
}

bool MaximalNearestSubline::_isOutsideInterval(int ia)
{
  if (ia <= _maxInterval[0].getSegmentIndex())
    return true;
  if (ia > _maxInterval[1].getSegmentIndex())
    return true;
  return false;
}

vector<WayPtr> MaximalNearestSubline::splitWay(OsmMapPtr map, int& mnsIndex)
{
  vector<WayPtr> result;
  _map = map;

  vector<WayLocation> interval = getInterval();
  WayLocation start = interval[0];
  WayLocation end = interval[1];

  // if we didn't find an applicable subline (due to distance constraint?)
  if (start.getSegmentIndex() < 0 || end.getSegmentIndex() < 0)
  {
    mnsIndex = -1;
    // return an empty result.
    return result;
  }

  // There are four possible scenarios ('x' is a split point):
  // a. x---x----
  // b. --x---x--
  // c. ----x---x
  // d. x-------x

  std::shared_ptr<FindNodesInWayFactory> nf(new FindNodesInWayFactory(_a));

  // if this is b or c
  if (start.getSegmentIndex() != 0 || start.getSegmentFraction() > 0.0)
  {
    WayLocation wl(map, _a, 0, 0.0);
    WayPtr way1 = WaySubline(wl, start).toWay(map, nf.get());

    double l = ElementToGeometryConverter(map).convertToLineString(way1)->getLength();
    // if the way is too short, round to the first way.
    if (l < _minSplitSize)
    {
      start = WayLocation(map, _a, 0, 0.0);
    }
    else
    {
      result.push_back(way1);
    }
  }

  // if this is a or b
  if (end.getSegmentIndex() < (int)_a->getNodeCount() - 1 || end.getSegmentFraction() < 1.0)
  {
    WayPtr way3 = WaySubline(end, WayLocation(map, _a, _a->getNodeCount() - 1, 0.0)).
      toWay(map, nf.get());

    double l = ElementToGeometryConverter(map).convertToLineString(way3)->getLength();
    // if the way is too short, round to the first way.
    if (l < _minSplitSize)
    {
      end = WayLocation(map, _a, _a->getNodeCount() - 1, 0.0);
    }
    else
    {
      result.push_back(way3);
    }
  }

  // in all cases we add the middle line.
  WayPtr way2 = WaySubline(start, end).toWay(map, nf.get());
  double l = ElementToGeometryConverter(map).convertToLineString(way2)->getLength();
  // if the way is big enough then add it on.
  if (l > _minSplitSize)
  {
    mnsIndex = result.size();
    result.push_back(way2);
  }
  else
  {
    result.clear();
  }

  // if we didn't split the way, return the original way.
  if (result.size() <= 1)
  {
    mnsIndex = 0;
    result.clear();
    result.push_back(map->getWay(_a->getId()));
  }

  return result;
}

}
