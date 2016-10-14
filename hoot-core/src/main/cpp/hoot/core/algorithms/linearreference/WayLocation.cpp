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

#include "WayLocation.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/ElementConverter.h>

// Standard
#include <iomanip>

namespace hoot {

Coordinate c;

const double WayLocation::SLOPPY_EPSILON = 1e-10;

WayLocation::WayLocation()
{
  _segmentIndex = -1;
  _segmentFraction = -1;
}

WayLocation::WayLocation(ConstOsmMapPtr map, ConstWayPtr way, double distance) :
  _map(map)
{
  double d = 0.0;

  _segmentIndex = -1;
  _segmentFraction = -1;

  _way = way;
  double length = ElementConverter(map).convertToLineString(way)->getLength();

  if (distance <= 0)
  {
    _segmentIndex = 0.0;
    _segmentFraction = 0;
  }
  else if (distance >= length)
  {
    _segmentIndex = _way->getNodeCount() - 1;
    _segmentFraction = 0.0;
  }
  else
  {
    Coordinate last = _map->getNode(way->getNodeId(0))->toCoordinate();

    _segmentIndex = way->getNodeCount() - 1;
    _segmentFraction = 0;

    for (size_t i = 1; i < way->getNodeCount(); i++)
    {
      shared_ptr<const Node> n = _map->getNode(_way->getNodeId(i));
      Coordinate next = n->toCoordinate();
      double delta = next.distance(last);
      last = next;

      if (d <= distance && d + delta > distance)
      {
        _segmentIndex = i - 1;
        _segmentFraction = (distance - d) / delta;
        // this can sometimes happen due to rounding errors.
        if (_segmentFraction >= 1.0)
        {
          _segmentFraction = 0.0;
          _segmentIndex++;
        }
        _way = way;
        break;
      }
      d += delta;
    }
  }
  assert(_segmentFraction < 1.0);
  assert((size_t)_segmentIndex <= _way->getNodeCount() - 1);
}

WayLocation::WayLocation(ConstOsmMapPtr map, ConstWayPtr way, int segmentIndex,
  double segmentFraction) :
  _map(map)
{
  _way = way;
  _segmentIndex = segmentIndex;
  _segmentFraction = segmentFraction;

  if (_segmentFraction == 1.0)
  {
    _segmentIndex++;
    _segmentFraction = 0.0;
  }

  if (_segmentFraction < 0.0 || _segmentFraction >= 1.0)
  {
    throw HootException("Segment Fraction is out of range.");
  }

  if (_segmentIndex < 0)
  {
    _segmentIndex = 0;
    _segmentFraction = 0.0;
  }

  if (_segmentIndex >= (int)_way->getNodeCount() - 1)
  {
    _segmentIndex = _way->getNodeCount() - 1;
    _segmentFraction = 0.0;
  }
}

Meters WayLocation::calculateDistanceFromEnd() const
{
  /// @todo this could be made more efficient by using the nodes directly
  return ElementConverter(getMap()).convertToLineString(getWay())->getLength() -
      calculateDistanceOnWay();
}

Meters WayLocation::calculateDistanceOnWay() const
{
  Meters result = 0.0;
  Coordinate last = _map->getNode(_way->getNodeId(0))->toCoordinate();
  for (int i = 1; i < (int)_way->getNodeCount() && i <= _segmentIndex; i++)
  {
    shared_ptr<const Node> n = _map->getNode(_way->getNodeId(i));
    Coordinate next = n->toCoordinate();
    result += next.distance(last);
    last = next;
  }

  if (_segmentIndex < (int)_way->getNodeCount() - 1)
  {
    shared_ptr<const Node> n = _map->getNode(_way->getNodeId(_segmentIndex + 1));
    Coordinate next = n->toCoordinate();
    Meters d = next.distance(last);
    result += d * _segmentFraction;
  }

  return result;
}

int WayLocation::compareLocationValues(int segmentIndex0, double segmentFraction0,
                                       int segmentIndex1, double segmentFraction1)
{
  // compare segments
  if (segmentIndex0 < segmentIndex1) return -1;
  if (segmentIndex0 > segmentIndex1) return 1;
  // same segment, so compare segment fraction
  if (segmentFraction0 < segmentFraction1) return -1;
  if (segmentFraction0 > segmentFraction1) return 1;
  // same location
  return 0;
}

int WayLocation::compareTo(const WayLocation& other) const
{
  if (!(_segmentFraction < 1.0 && other._segmentFraction < 1.0))
  {
    LOG_WARN(_segmentFraction << " other: " << other._segmentFraction);
  }
  assert(_segmentFraction < 1.0 && other._segmentFraction < 1.0);
  // compare way ids
  if (_way->getId() < other._way->getId()) return -1;
  if (_way->getId() > other._way->getId()) return 1;
  // compare segments
  if (_segmentIndex < other._segmentIndex) return -1;
  if (_segmentIndex > other._segmentIndex) return 1;
  // same segment, so compare segment fraction
  if (_segmentFraction < other._segmentFraction) return -1;
  if (_segmentFraction > other._segmentFraction) return 1;
  // same location
  return 0;
}

WayLocation WayLocation::createAtEndOfWay(const ConstOsmMapPtr& map, const ConstWayPtr way)
{
  return WayLocation(map, way, way->getNodeCount() - 1, 0.0);
}

const Coordinate WayLocation::getCoordinate() const
{
  shared_ptr<const Node> p0 = _map->getNode(_way->getNodeId(_segmentIndex));
  if (_segmentFraction <= 0.0)
  {
    return p0->toCoordinate();
  }
  else
  {
    shared_ptr<const Node> p1 = _map->getNode(_way->getNodeId(_segmentIndex + 1));
    return pointAlongSegmentByFraction(p0->toCoordinate(), p1->toCoordinate(), _segmentFraction);
  }
}

ConstNodePtr WayLocation::getNode(double epsilon) const
{
  if (!isNode(epsilon))
  {
    throw IllegalArgumentException("getNode() is only valid if WayLocation is on a node.");
  }

  // Round to the appropriate segment index. This may be necessary due to floating point errors.
  if (_segmentFraction >= 0.5)
  {
    return _map->getNode(getWay()->getNodeId(_segmentIndex + 1));
  }
  else
  {
    return _map->getNode(getWay()->getNodeId(_segmentIndex));
  }
}

bool WayLocation::isLast(double epsilon) const
{
  bool result = false;
  if (_segmentIndex == (int)_way->getNodeCount() - 1)
  {
    result = true;
  }
  else if (_segmentIndex == (int)_way->getNodeCount() - 2 && _segmentFraction >= 1 - epsilon)
  {
    result = true;
  }

  return result;
}

WayLocation WayLocation::move(Meters distance) const
{
  WayLocation result(*this);
  Coordinate last = result.getCoordinate();

  // This odd statement avoid us adding irrelevantly small distances.
  while (1 + distance > 1)
  {
    // if we're at the end of the way
    if (result.isLast())
    {
      return result;
    }

    shared_ptr<const Node> n = _map->getNode(_way->getNodeId(result.getSegmentIndex() + 1));
    Coordinate next = n->toCoordinate();

    double delta = last.distance(next);

    // if the next node is too far
    if (distance - delta < 0)
    {
      // figure out what distance we need to move along the segment.
      Coordinate lastSegment = _map->getNode(_way->getNodeId(result.getSegmentIndex()))->
          toCoordinate();
      double segmentLength = lastSegment.distance(next);
      result._segmentFraction += (distance / segmentLength);

      // this can happen due to floating point errors when the new location is very close to a
      // node.
      if (result._segmentFraction >= 1.0)
      {
        result._segmentFraction = 0.0;
        result._segmentIndex = result._segmentIndex + 1;
      }

      distance = 0;
    }
    // if we need to go past the next node
    else
    {
      distance -= delta;
      last = next;
      result._segmentIndex = result.getSegmentIndex() + 1;
      result._segmentFraction = 0.0;
    }
  }

  // This odd statement avoid us subtracting irrelevantly small distances.
  while (1 + distance < 1)
  {
    // if we're at the end of the way
    if (result.isFirst())
    {
      return result;
    }

    if (result._segmentFraction > 0)
    {
      Coordinate next = _map->getNode(_way->getNodeId(result.getSegmentIndex()))->toCoordinate();

      Meters delta = last.distance(next);
      if (distance + delta > 0)
      {
        // figure out what distance we need to move along the segment.
        Coordinate last = _map->getNode(_way->getNodeId(result._segmentIndex + 1))->toCoordinate();
        double segmentLength = last.distance(next);
        result._segmentFraction += (distance / segmentLength);

        // this can happen due to floating point errors when the new location is very close to a
        // node.
        if (result._segmentFraction >= 1.0)
        {
          result._segmentFraction = 0.0;
          result._segmentIndex = result._segmentIndex + 1;
        }

        distance = 0;
      }
      else
      {
        result._segmentFraction = 0;
        distance += delta;
        last = next;
      }
    }
    else
    {
      Coordinate next = _map->getNode(_way->getNodeId(result.getSegmentIndex() - 1))->
          toCoordinate();

      Meters delta = last.distance(next);
      if (distance + delta > 0)
      {
        // figure out what distance we need to move along the segment.
        Coordinate last = _map->getNode(_way->getNodeId(result.getSegmentIndex()))->toCoordinate();
        double segmentLength = last.distance(next);
        result._segmentFraction = 1.0 + (distance / segmentLength);
        // if we're suffering from a floating point issue.
        if (result._segmentFraction >= 1.0)
        {
          // make sure the floating point issue is within the expected bounds.
          assert(result._segmentFraction < 1.0 + 1e-14);
          result._segmentFraction = 0;
        }
        else
        {
          result._segmentIndex = result.getSegmentIndex() - 1;
        }
        distance = 0;
      }
      else
      {
        result._segmentIndex--;
        distance += delta;
        last = next;
      }
    }
  }

  assert(result._segmentFraction < 1.0 && result._segmentFraction >= 0);
  assert(result._segmentIndex >= 0 && result._segmentIndex < (int)result.getWay()->getNodeCount());

  return result;
}

const Coordinate WayLocation::pointAlongSegmentByFraction(const Coordinate& p0,
                                                          const Coordinate& p1,
                                                          double frac)
{
  if (frac <= 0.0) return p0;
  if (frac >= 1.0) return p1;

  double x = (p1.x - p0.x) * frac + p0.x;
  double y = (p1.y - p0.y) * frac + p0.y;
  return Coordinate(x, y);
}

QString WayLocation::toString() const
{
  if (isValid())
  {
    return QString("way: %1 index: %2 fraction: %3").arg(_way->getId()).arg(_segmentIndex).
        arg(_segmentFraction, 0, 'g', 15);
  }
  else
  {
    return QString("way: <null> index: %1 fraction: %2").arg(_segmentIndex).arg(_segmentFraction);
  }
}

}
