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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Maxar (http://www.maxar.com/)
 */
#include "WaySubline.h"

// GEOS
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/algorithms/FindNodesInWayFactory.h>
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/visitors/ConstElementVisitor.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

WaySubline::WaySubline(const WaySubline& from)
  : _start(from.getStart()),
    _end(from.getEnd())
{
}

WaySubline::WaySubline(const WaySubline& from, const ConstOsmMapPtr& newMap)
{
  if (from.isValid())
  {
    ConstWayPtr oldWay = from.getStart().getWay();
    ConstWayPtr newWay = newMap->getWay(oldWay->getId());
    _start = WayLocation(newMap, newWay, from.getStart().getSegmentIndex(), from.getStart().getSegmentFraction());
    _end = WayLocation(newMap, newWay, from.getEnd().getSegmentIndex(), from.getEnd().getSegmentFraction());
  }
}

WaySubline::WaySubline(const WayLocation& start, const WayLocation& end)
  : _start(start),
    _end(end)
{
}

WaySubline::WaySubline(WaySubline && subline) noexcept
  : _start(subline._start),
    _end(subline._end)
{
  subline._start = WayLocation();
  subline._end = WayLocation();
}


WaySubline& WaySubline::operator=(const WaySubline& from)
{
  if (this != &from)
  {
    _start = from.getStart();
    _end = from.getEnd();
  }
  return *this;
}

bool operator==(const WaySubline& a, const WaySubline& b)
{
  return a.getStart() == b.getStart() && a.getEnd() == b.getEnd();
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

bool WaySubline::overlaps(const WaySubline& other) const
{
  bool overlaps = true;
  if (other.getWay() != getWay() || getStart() >= other.getEnd() || other.getStart() >= getEnd())
    overlaps = false;
  return overlaps;
}

WaySubline WaySubline::reverse(const ConstWayPtr& reversedWay) const
{
  WaySubline result;

  // sanity check to make sure they're actually reversed, this isn't conclusive but should help
  // if there is a major goof.
  assert(reversedWay->getNodeCount() == getWay()->getNodeCount());

  double l = ElementToGeometryConverter(getMap()).convertToLineString(getWay())->getLength();

  result._start = WayLocation(getMap(), reversedWay, l - getEnd().calculateDistanceOnWay());
  result._end = WayLocation(getMap(), reversedWay, l - getStart().calculateDistanceOnWay());

  return result;
}

QString WaySubline::toString() const
{
  return "start: " + getStart().toString() + " end: " + getEnd().toString();
}

WayPtr WaySubline::toWay(const OsmMapPtr& map, std::shared_ptr<GeometryToElementConverter::NodeFactory> nf,
                         bool reuse) const
{
  ConstWayPtr way = _start.getWay();
  LOG_VART(way->getElementId());

  if (!nf)
    nf = std::make_shared<FindNodesInWayFactory>(way);

  Meters ce = way->getRawCircularError();

  long way_id = way->getId();
  if (!reuse)
    way_id = map->createNextWayId();
  LOG_VART(way_id);
  WayPtr result = std::make_shared<Way>(way->getStatus(), way_id, ce);
  result->setPid(way->getPid());
  result->setVersion(way->getVersion());
  result->setTimestamp(way->getTimestamp());
  result->setTags(way->getTags());
  LOG_VART(result->getElementId());

  int includedStartIndex = _start.getSegmentIndex();
  if (_start.getSegmentFraction() > 0.0)
    includedStartIndex += 1;
  int includedEndIndex = _end.getSegmentIndex();
  if (_end.getSegmentFraction() >= 1.0)
    includedEndIndex += 1;

  if (!_start.isNode())
  {
    Coordinate c = _start.getCoordinate();
    NodePtr n = nf->createNode(map, c, way->getStatus(), ce);
    map->addNode(n);
    result->addNode(n->getId());
  }

  for (int i = includedStartIndex; i <= includedEndIndex; i++)
    result->addNode(way->getNodeId(i));

  LOG_VART(_end.isNode());
  if (!_end.isNode())
  {
    Coordinate c = _end.getCoordinate();
    LOG_VART(c);
    NodePtr n = nf->createNode(map, c, way->getStatus(), ce);
    LOG_VART(n->getElementId());
    map->addNode(n);
    result->addNode(n->getId());
  }

  LOG_VART(result);
  return result;
}

bool WaySubline::touches(const WaySubline& other) const
{
  bool touches = true;
  if (other.getWay() != getWay() || getStart() > other.getEnd() || other.getStart() > getEnd())
    touches = false;
  return touches;
}

void WaySubline::visitRo(const ElementProvider& ep, ConstElementVisitor& visitor) const
{
  visitor.visit(_start.getWay());

  for (int i = getFormer().getSegmentIndex(); i <= getLatter().getSegmentIndex(); ++i)
    visitor.visit(ep.getNode(_start.getWay()->getNodeId(i)));
}

}
