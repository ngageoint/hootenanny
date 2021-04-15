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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "WaySublineMerger.h"

// hoot
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/core/conflate/merging/LinearMergerFactory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/linearreference/WaySubline.h>

namespace hoot
{

WaySubline WaySublineMerger::mergeSublines(const WaySubline& subline1, const WaySubline& subline2)
{
  OsmMapPtr map = std::make_shared<OsmMap>();
  WayPtr way1 = subline1.toWay(map);
  map->addWay(way1);
  WayPtr way2 = subline2.toWay(map);
  map->addWay(way2);
  LOG_TRACE(map->getWayCount());

  std::set<std::pair<ElementId, ElementId>> pairs;
  pairs.insert(std::pair<ElementId, ElementId>(way1->getElementId(), way2->getElementId()));

  std::shared_ptr<SublineStringMatcher> sublineMatcher;
  sublineMatcher.reset(
    Factory::getInstance().constructObject<SublineStringMatcher>(
      ConfigOptions().getWaySublineStringMatcher()));
  Settings settings = conf();
  settings.set("way.matcher.max.angle", ConfigOptions().getHighwayMatcherMaxAngle());
  settings.set("way.subline.matcher", ConfigOptions().getHighwaySublineMatcher());
  settings.set("way.matcher.heading.delta", ConfigOptions().getHighwayMatcherHeadingDelta());
  sublineMatcher->setConfiguration(settings);

  MergerPtr merger = LinearMergerFactory::getMerger(pairs, sublineMatcher);
  std::vector<std::pair<ElementId, ElementId>> replaced;
  merger->apply(map, replaced);
  LOG_TRACE(map->getWayCount());

  WayPtr mergedWay;
  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator itr = ways.begin(); itr != ways.end(); ++itr)
  {
    mergedWay = itr->second;
  }

  return WaySubline(mergedWay, map);
}

}
