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
#include "WaySubline.h"

// GEOS
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/FindNodesInWayFactory.h>
#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{

WaySubline::WaySubline()
{

}

WaySubline::WaySubline(const WaySubline& from) :
  _start(from.getStart()),
  _end(from.getEnd())
{

}

WaySubline::WaySubline(const WaySubline& from, const ConstOsmMapPtr& newMap)
{
  if (from.isValid())
  {
    ConstWayPtr oldWay = from.getStart().getWay();
    ConstWayPtr newWay = newMap->getWay(oldWay->getId());
    _start = WayLocation(newMap, newWay,
      from.getStart().getSegmentIndex(), from.getStart().getSegmentFraction());
    _end = WayLocation(newMap, newWay,
      from.getEnd().getSegmentIndex(), from.getEnd().getSegmentFraction());
  }
}

WaySubline::WaySubline(const WayLocation& start, const WayLocation& end) :
  _start(start),
  _end(end)
{
  assert(_start.getWay() == _end.getWay());
}

WaySubline& WaySubline::operator=(const WaySubline& from)
{
  _start = from.getStart();
  _end = from.getEnd();
  return *this;
}

Meters WaySubline::calculateLength() const
{
  return fabs(_start.calculateDistanceOnWay() - _end.calculateDistanceOnWay());
}

bool WaySubline::contains(const WayLocation& wl) const
{
  return wl >= getFormer() && wl <= getLatter();
}

bool WaySubline::contains(const WaySubline& other) const
{
  return getStart().getWay() == other.getStart().getWay() &&
      other.getStart() >= getStart() && other.getEnd() <= getEnd();
}

WaySubline WaySubline::expand(Meters d) const
{
  return WaySubline(_start.move(-d), _end.move(d));
}

Meters WaySubline::getLength() const
{
  return fabs(_end.calculateDistanceOnWay() - _start.calculateDistanceOnWay());
}

bool operator==(const WaySubline& a, const WaySubline& b)
{
  return a.getStart() == b.getStart() && a.getEnd() == b.getEnd();
}

bool WaySubline::overlaps(const WaySubline& other) const
{
  bool overlaps = true;

  if (other.getWay() != getWay() ||
      getStart() >= other.getEnd() ||
      other.getStart() >= getEnd())
  {
    overlaps = false;
  }

  return overlaps;
}

WaySubline WaySubline::reverse(const ConstWayPtr& reversedWay) const
{
  WaySubline result;

  // sanity check to make sure they're actually reversed, this isn't conclusive but should help
  // if there is a major goof.
  assert(reversedWay->getNodeCount() == getWay()->getNodeCount());
  assert(reversedWay->getNodeId(0) == getWay()->getLastNodeId());

  double l = ElementConverter(getMap()).convertToLineString(getWay())->getLength();

  result._start = WayLocation(getMap(), reversedWay, l - getEnd().calculateDistanceOnWay());
  result._end = WayLocation(getMap(), reversedWay, l - getStart().calculateDistanceOnWay());

  return result;
}

QString WaySubline::toString() const
{
  return "start: " + getStart().toString() + " end: " + getEnd().toString();
}

WayPtr WaySubline::toWay(const OsmMapPtr& map, GeometryConverter::NodeFactory* nf) const
{
  ConstWayPtr way = _start.getWay();

  auto_ptr<GeometryConverter::NodeFactory> nfPtr;
  if (nf == 0)
  {
    nf = new FindNodesInWayFactory(way);
    // delete it automatically.
    nfPtr.reset(nf);
  }

  Meters ce = way->getRawCircularError();

  WayPtr result(new Way(way->getStatus(), map->createNextWayId(), ce));
  result->setTags(way->getTags());

  int includedStartIndex = _start.getSegmentIndex();
  if (_start.getSegmentFraction() > 0.0)
  {
    includedStartIndex += 1;
  }
  int includedEndIndex = _end.getSegmentIndex();
  if (_end.getSegmentFraction() >= 1.0)
  {
    includedEndIndex += 1;
  }

  if (!_start.isNode())
  {
    Coordinate c = _start.getCoordinate();
    boost::shared_ptr<Node> n = nf->createNode(map, c, way->getStatus(), ce);
    map->addNode(n);
    result->addNode(n->getId());
  }

  for (int i = includedStartIndex; i <= includedEndIndex; i++)
  {
    result->addNode(way->getNodeId(i));
  }

  if (!_end.isNode())
  {
    Coordinate c = _end.getCoordinate();
    boost::shared_ptr<Node> n = nf->createNode(map, c, way->getStatus(), ce);
    map->addNode(n);
    result->addNode(n->getId());
  }

  return result;
}

bool WaySubline::touches(const WaySubline& other) const
{
  bool touches = true;

  if (other.getWay() != getWay() ||
      getStart() > other.getEnd() ||
      other.getStart() > getEnd())
  {
    touches = false;
  }

  return touches;
}

void WaySubline::visitRo(const ElementProvider& ep, ElementVisitor& visitor) const
{
  visitor.visit(_start.getWay());

  for (int i = getFormer().getSegmentIndex(); i <= getLatter().getSegmentIndex(); ++i)
  {
    visitor.visit(ep.getNode(_start.getWay()->getNodeId(i)));
  }
}

}
