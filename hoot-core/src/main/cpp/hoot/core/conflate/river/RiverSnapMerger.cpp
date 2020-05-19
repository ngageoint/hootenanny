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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RiverSnapMerger.h"

// hoot
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/ElementConverter.h>
//#include <hoot/core/criterion/LinearWaterwayCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(Merger, RiverSnapMerger)

RiverSnapMerger::RiverSnapMerger() :
HighwaySnapMerger()
{
  setConfiguration(conf());
}

RiverSnapMerger::RiverSnapMerger(
  const std::set<std::pair<ElementId, ElementId>>& pairs,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher2) :
HighwaySnapMerger(pairs, sublineMatcher),
_sublineMatcher2(sublineMatcher2)
{
  setConfiguration(conf());
}

void RiverSnapMerger::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  _nodeCountThreshold = opts.getWaterwayLongWayNodeCountThreshold();
  _lengthThreshold = opts.getWaterwayLongWayLengthThreshold();
}

WaySublineMatchString RiverSnapMerger::_matchSubline(OsmMapPtr map, ElementPtr e1, ElementPtr e2)
{
  if (e1->getElementType() == ElementType::Way && e2->getElementType() == ElementType::Way &&
      isLongWayPair(
        map, std::dynamic_pointer_cast<const Way>(e1), std::dynamic_pointer_cast<const Way>(e2)))
  {
    LOG_TRACE("Matching long river sublines for merging...");
    return _sublineMatcher2->findMatch(map, e1, e2);
  }
  else
  {
    LOG_TRACE("Matching river sublines for merging...");
    return _sublineMatcher->findMatch(map, e1, e2);
  }
}

bool RiverSnapMerger::isLongWayPair(ConstOsmMapPtr map, ConstWayPtr way1, ConstWayPtr way2)
{
  ElementConverter lengthCalc(map);
  LOG_VART(lengthCalc.calculateLength(way1) + lengthCalc.calculateLength(way2));
  LOG_VART(way1->getNodeCount() + way2->getNodeCount());
  return
    (lengthCalc.calculateLength(way1) + lengthCalc.calculateLength(way2)) > _lengthThreshold ||
    (int)(way1->getNodeCount() + way2->getNodeCount()) > _nodeCountThreshold;
}

}
