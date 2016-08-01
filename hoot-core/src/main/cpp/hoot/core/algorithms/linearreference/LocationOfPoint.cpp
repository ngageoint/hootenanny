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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "LocationOfPoint.h"

// GEOS
#include <geos/geom/LineSegment.h>
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/util/Assert.h>
#include <hoot/core/util/ElementConverter.h>

// Standard
#include <limits>
#include <numeric>

namespace hoot {

LocationOfPoint::LocationOfPoint(const ConstOsmMapPtr& map, shared_ptr<const Way> way) :
  _map(map),
  _way(way)
{
  _length = -1;
}

double distance(const Node& n1, const Node& n2)
{
  double dx = n1.getX() - n2.getX();
  double dy = n1.getY() - n2.getY();
  return sqrt(dx * dx + dy * dy);
}

Coordinate LocationOfPoint::locate(double d)
{
  if (_length == -1)
  {
    _length = ElementConverter(_map).convertToLineString(_way)->getLength();
  }

  if (d <= 0)
  {
    return _map->getNode(_way->getNodeId(0))->toCoordinate();
  }
  else if (d >= _length)
  {
    return _map->getNode(_way->getNodeId(_way->getNodeCount() - 1))->toCoordinate();
  }

  double running = 0.0;
  double step = 0.0;
  int i = -1;
  do
  {
    i++;
    running += step;
    step = distance(*_map->getNode(_way->getNodeId(i)), *_map->getNode(_way->getNodeId(i + 1)));
  }
  while (step + running < d);

  const Node& n1 = *_map->getNode(_way->getNodeId(i));
  const Node& n2 = *_map->getNode(_way->getNodeId(i + 1));
  double dx = n2.getX() - n1.getX();
  double dy = n2.getY() - n1.getY();

  double p = (d - running) / step;

  Coordinate result;
  result.x = n1.getX() + dx * p;
  result.y = n1.getY() + dy * p;

  return result;
}

WayLocation LocationOfPoint::locate(const ConstOsmMapPtr& map, shared_ptr<const Way> way,
  const Coordinate& inputPt)
{
  LocationOfPoint locater(map, way);
  return locater.locate(inputPt);
}

WayLocation LocationOfPoint::locate(const Coordinate& inputPt) const
{
  double minDistance = std::numeric_limits<double>().max();
  int minIndex = 0;
  double minFrac = -1.0;

  LineSegment seg;

  if (_length == -1)
  {
    _length = ElementConverter(_map).convertToLineString(_way)->getLength();
  }

  if (_length <= 0.0)
  {
    return WayLocation(_map, _way, 0, 0);
  }

  if (_way->getNodeCount() >= 1)
  {
    // this little bit of fanciness makes the function ~30% faster.
    Coordinate lastCoord = _map->getNode(_way->getNodeId(0))->toCoordinate();
    for (size_t i = 0; i < _way->getNodeCount() - 1; i++) {
      Coordinate nextCoord = _map->getNode(_way->getNodeId(i + 1))->toCoordinate();
      seg.p0 = lastCoord;
      seg.p1 = nextCoord;
      lastCoord = nextCoord;
      double segDistance = seg.distance(inputPt);
      double segFrac = segmentFraction(seg, inputPt);

      if (segDistance < minDistance) {
        minIndex = i;
        minFrac = segFrac;
        minDistance = segDistance;
      }
    }
  }

  return WayLocation(_map, _way, minIndex, minFrac);
}

WayLocation LocationOfPoint::locateAfter(const Coordinate& inputPt, const WayLocation& minLocation)
    const
{
  if (minLocation.isValid() == false)
  {
    return locate(inputPt);
  }

  assert(minLocation.getWay() == _way);

  double minDistance = std::numeric_limits<double>().max();
  WayLocation nextClosestLocation = minLocation;

  LineSegment seg;
  size_t startIndex = 0;
  startIndex = (size_t)minLocation.getSegmentIndex();

  for (size_t i = startIndex; i < _way->getNodeCount() - 1; i++)
  {
    seg.p0 = _map->getNode(_way->getNodeId(i))->toCoordinate();
    seg.p1 = _map->getNode(_way->getNodeId(i + 1))->toCoordinate();

    if (i == startIndex)
    {
      seg.p0 = minLocation.getCoordinate();
    }

    double segDistance = seg.distance(inputPt);
    double segFrac = segmentFraction(seg, inputPt);

    if (segDistance < minDistance)
    {
      // if this is the first case (a partial line segment)
      if (i == startIndex)
      {
        // recalculate the segFrac in terms of the whole line segment.
        segFrac = minLocation.getSegmentFraction() +
            (1 - minLocation.getSegmentFraction()) * segFrac;
      }
      nextClosestLocation = WayLocation(_map, _way, i, segFrac);
      minDistance = segDistance;
    }
  }
  // Return the minDistanceLocation found.
  // This will not be null, since it was initialized to minLocation
  Assert::isFalse(nextClosestLocation >= minLocation,
                "computed location is before specified minimum location");

  return nextClosestLocation;
}

bool LocationOfPoint::isGreater(int i, double segFrac, const WayLocation& loc) const
{
  return WayLocation::compareLocationValues(i, segFrac,
      loc.getSegmentIndex(), loc.getSegmentFraction()) > 0;
}

double LocationOfPoint::segmentFraction(const LineSegment& seg, const Coordinate& inputPt)
{
  double segFrac;
  if (seg.p0.equals2D(seg.p1))
  {
    segFrac = 0.0;
  }
  else
  {
    segFrac = seg.projectionFactor(inputPt);
    if (segFrac < 0.000001)
    {
      segFrac = 0.0;
    }
    else if (segFrac > 0.999999)
    {
      segFrac = 1.0;
    }
  }
  return segFrac;
}

}
