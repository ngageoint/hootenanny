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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "MaximalSublineMatcher.h"

// hoot
#include <hoot/core/algorithms/subline-matching/MaximalSubline.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Units.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(SublineMatcher, MaximalSublineMatcher)

MaximalSublineMatcher::MaximalSublineMatcher() :
_maxRecursions(-1)
{
}

WaySublineMatchString MaximalSublineMatcher::findMatch(const ConstOsmMapPtr& map,
  const ConstWayPtr& way1, const ConstWayPtr& way2, double& score, Meters maxRelevantDistance) const
{
  Meters mrd =
    maxRelevantDistance == -1 ? way1->getCircularError() + way2->getCircularError() :
    maxRelevantDistance;
  LOG_VART(maxRelevantDistance);
  LOG_VART(_minSplitSize);
  LOG_VART(_maxRelevantAngle);

  std::shared_ptr<MaximalSubline::ThresholdMatchCriteria> threshold =
    std::make_shared<MaximalSubline::ThresholdMatchCriteria>(mrd, _maxRelevantAngle);
  // This should use _minSplitSize rather than mrd, but that causes some tests to fail. We should
  // look into the problem and solve it. See #169.
  MaximalSubline ms(threshold, mrd);
  // See MaximalSubline::_maxRecursions
  LOG_VART(_maxRecursions);
  ms.setMaxRecursions(_maxRecursions);

  vector<WaySublineMatch> matches = ms.findAllMatches(map, way1, way2, score);
  LOG_VART(ms.getBestMatchesRecursionCount());
  return WaySublineMatchString(matches);
}

}
