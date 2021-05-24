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
#include "MaximalNearestSublineMatcher.h"

// geos
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/algorithms/subline-matching/MaximalNearestSubline.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/Factory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(SublineMatcher, MaximalNearestSublineMatcher)

WaySublineMatchString MaximalNearestSublineMatcher::findMatch(const ConstOsmMapPtr& map,
  const ConstWayPtr& way1, const ConstWayPtr& way2, double& score, Meters maxRelevantDistance) const
{
  LOG_VART(way1->getElementId());
  LOG_VART(way2->getElementId());

  score = 0;
  Meters mrd =
    maxRelevantDistance == -1 ? way1->getCircularError() + way2->getCircularError() :
    maxRelevantDistance;

  OsmMapPtr mapCopy(new OsmMap());
  CopyMapSubsetOp(map, way1->getElementId(), way2->getElementId()).apply(mapCopy);

  WayPtr way1NonConst = mapCopy->getWay(way1->getId());
  WayPtr way2NonConst = mapCopy->getWay(way2->getId());
  LOG_VART(way1NonConst->getNodeIds());
  LOG_VART(way2NonConst->getNodeIds());

  MaximalNearestSubline mns1(
    mapCopy, way1NonConst, way2NonConst, _minSplitSize, mrd, _maxRelevantAngle, _headingDelta);

  // Use the maximal nearest subline code to find the best subline.
  std::vector<WayLocation> interval1 = mns1.getInterval();
  if (!interval1[0].isValid() || !interval1[1].isValid() || interval1[0] == interval1[1])
  {
    // If the interval isn't valid, then return an invalid result.
    LOG_TRACE("Returning invalid result...");
    return WaySublineMatchString();
  }
  _snapToEnds(map, interval1);
  WayPtr subline1 = WaySubline(interval1[0], interval1[1]).toWay(mapCopy);
  LOG_VART(subline1->getNodeIds());

  MaximalNearestSubline mns2(mapCopy, way2NonConst, subline1, _minSplitSize, -1, -1, _headingDelta);
  std::vector<WayLocation> interval2 = mns2.getInterval();
  if (!interval2[0].isValid() || !interval2[1].isValid() || interval2[0] == interval2[1])
  {
    LOG_TRACE("Returning invalid result...");
    return WaySublineMatchString();
  }
  _snapToEnds(map, interval2);

  WaySublineMatch match =
    WaySublineMatch(
      WaySubline(interval1[0], interval1[1]), WaySubline(interval2[0], interval2[1]));
  LOG_VART(match);

  if (subline1->getNodeCount() > 1)
  {
    std::shared_ptr<LineString> ls =
      ElementToGeometryConverter(mapCopy).convertToLineString(subline1);
    if (ls && ls->isValid())
    {
      score = ls->getLength();
    }
  }
  LOG_VART(score);

  vector<WaySublineMatch> v;
  // Switch the subline match to reference a different map.
  v.emplace_back(match, map);

  return WaySublineMatchString(v);
}

void MaximalNearestSublineMatcher::_snapToEnds(const ConstOsmMapPtr &map,
  vector<WayLocation>& wl) const
{
  if (wl[0].calculateDistanceOnWay() <= _minSplitSize)
  {
    wl[0] = WayLocation(map, wl[0].getWay(), 0, 0);
  }
  if (wl[1].calculateDistanceFromEnd() <= _minSplitSize)
  {
    wl[1] = WayLocation::createAtEndOfWay(map, wl[1].getWay());
  }
}

}
