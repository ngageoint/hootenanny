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
#include "MaximalSublineMatcher.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/Units.h>

#include "MaximalSubline.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(SublineMatcher, MaximalSublineMatcher)

MaximalSublineMatcher::MaximalSublineMatcher()
{
  _minSplitSize = -1.0;
  _maxAngle = -1.0;
}

WaySublineMatchString MaximalSublineMatcher::findMatch(const ConstOsmMapPtr &map,
  const ConstWayPtr& way1, const ConstWayPtr& way2, double &score, Meters maxRelevantDistance) const
{
  Meters mrd = maxRelevantDistance == -1 ? way1->getCircularError() + way2->getCircularError() :
    maxRelevantDistance;
//  LOG_WARN("min split size: " << _minSplitSize << " _maxAngle " << _maxAngle << " mrd: " << mrd);
//  assert(_minSplitSize >= 0.0 && _maxAngle >= 0.0 && mrd >= 0.0);

  MaximalSubline::ThresholdMatchCriteria* threshold =
    new MaximalSubline::ThresholdMatchCriteria(mrd, _maxAngle);
  // This should use the _minSplitSize rather than mrd, but that causes some tests to fail. We
  // should look into the problem and solve it. See #6159
  MaximalSubline ms(threshold, mrd);

  vector<WaySublineMatch> matches = ms.findAllMatches(map, way1, way2, score);

  return WaySublineMatchString(matches);
}

void MaximalSublineMatcher::setConfiguration(const Settings &conf)
{
  ConfigOptions co(conf);
  _maxAngle = toRadians(co.getWayMatcherMaxAngle());
  _minSplitSize = co.getWayMergerMinSplitSize();
}

}
