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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "FrechetSublineMatcher.h"
// geos
#include <geos/geom/LineString.h>
// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/Units.h>
#include <hoot/core/algorithms/FrechetDistance.h>
#include <hoot/core/ops/CopySubsetOp.h>
#include <hoot/core/util/ElementConverter.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(SublineMatcher, FrechetSublineMatcher)

FrechetSublineMatcher::FrechetSublineMatcher()
{
}

WaySublineMatchString FrechetSublineMatcher::findMatch(const ConstOsmMapPtr& map, const ConstWayPtr& way1,
  const ConstWayPtr& way2, double& score, Meters maxRelevantDistance) const
{
  score = 0;
  Meters mrd = maxRelevantDistance == -1 ? way1->getCircularError() + way2->getCircularError() :
    maxRelevantDistance;
  //  Create a copy of the map and the two ways
  OsmMapPtr mapCopy(new OsmMap());
  CopySubsetOp(map,
               way1->getElementId(),
               way2->getElementId()).apply(mapCopy);
  //  Calculate the Frechet subline
  FrechetDistance fd(map, way1, way2, _maxAngle);
  frechet_subline max_subline = fd.maxSubline(mrd);
  //  Make sure that there is a valid subline
  if (max_subline.size() <= 1)
  {
    return WaySublineMatchString();
  }
  //  Create the way sublines
  WaySubline subline1(WayLocation(mapCopy, way1, max_subline[0].first, 0),
                      WayLocation(mapCopy, way1, max_subline[max_subline.size() - 1].first, 0));
  WaySubline subline2(WayLocation(mapCopy, way2, max_subline[0].second, 0),
                      WayLocation(mapCopy, way2, max_subline[max_subline.size() - 1].second, 0));
  //  Convert the sublines to ways
  WayPtr sub1 = subline1.toWay(mapCopy);
  WayPtr sub2 = subline2.toWay(mapCopy);
  //  Calculate the score (max length of both sublines)
  if (sub1->getNodeCount() > 1 && sub2->getNodeCount() > 1)
  {
    shared_ptr<LineString> ls1 = ElementConverter(mapCopy).convertToLineString(sub1);
    shared_ptr<LineString> ls2 = ElementConverter(mapCopy).convertToLineString(sub2);
    if (ls1->isValid() && ls2->isValid())
    {
      score = min(ls1->getLength(), ls2->getLength());
    }
  }
  //  Create the match and match string
  WaySublineMatch match = WaySublineMatch(subline1, subline2);
  vector<WaySublineMatch> v;
  v.push_back(WaySublineMatch(match, map));
  return WaySublineMatchString(v);
}


void FrechetSublineMatcher::setConfiguration(const Settings &conf)
{
  ConfigOptions co(conf);
  _maxAngle = toRadians(co.getWayMatcherMaxAngle());
}

}
