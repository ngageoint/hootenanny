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
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/linearreference/WaySubline.h>

namespace hoot
{

WaySubline WaySublineMerger::mergeSublines(const WaySubline& subline1, const WaySubline& subline2)
{
  if (!subline1.isValid() || !subline2.isValid())
  {
    LOG_WARN("Invalid subline.");
    return WaySubline();
  }

  LOG_TRACE("Merging sublines: " << subline1 << " and " << subline2 << "...");

  LOG_VART(subline1.getWay()->getId() == subline2.getWay()->getId());

  const long cachedId = subline1.getWay()->getId();
  LOG_VART(cachedId);

  OsmMapPtr map = std::make_shared<OsmMap>();
  MapProjector::projectToPlanar(map);
  WayPtr way1 = subline1.toWay(map/*, nullptr, true*/);
  //WayPtr way1 = std::dynamic_pointer_cast<Way>(ElementPtr(subline1.getWay()->clone()));
  //way1->setId(map->createNextWayId());
  LOG_VART(way1->getElementId());
  LOG_VART(way1->getNodeCount());
  map->addWay(way1);
  WayPtr way2 = subline2.toWay(map/*, nullptr, true*/);
  //WayPtr way2 = std::dynamic_pointer_cast<Way>(ElementPtr(subline2.getWay()->clone()));
  //way2->setId(map->createNextWayId());
  LOG_VART(way2->getElementId());
  LOG_VART(way2->getNodeCount());
  map->addWay(way2);
  LOG_VART(map->getWayCount());
  MapProjector::projectToPlanar(map);
  OsmMapWriterFactory::writeDebugMap(
    map,
    "merge-sublines-temp-map-" + way1->getElementId().toString() + "-" +
      way2->getElementId().toString());

  std::set<std::pair<ElementId, ElementId>> pairs;
  pairs.insert(std::pair<ElementId, ElementId>(way1->getElementId(), way2->getElementId()));

  std::shared_ptr<SublineStringMatcher> sublineMatcher;
  sublineMatcher.reset(
    Factory::getInstance().constructObject<SublineStringMatcher>(
      ConfigOptions().getWaySublineStringMatcher()));
  LOG_VART(sublineMatcher->getName());
  LOG_VART(sublineMatcher->getSublineMatcherName());
  Settings settings = conf();
  settings.set("way.matcher.max.angle", ConfigOptions().getHighwayMatcherMaxAngle());
  settings.set("way.subline.matcher", ConfigOptions().getHighwaySublineMatcher());
  settings.set("way.matcher.heading.delta", ConfigOptions().getHighwayMatcherHeadingDelta());
  sublineMatcher->setConfiguration(settings);

  MergerPtr merger = LinearMergerFactory::getMerger(pairs, sublineMatcher);
  LOG_VART(merger->getName());
  std::vector<std::pair<ElementId, ElementId>> replaced;
  merger->apply(map, replaced);
  LOG_VART(map->getWayCount());
  if (map->getWayCount() == 0)
  {
    return WaySubline();
  }

  WayPtr mergedWay;
  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator itr = ways.begin(); itr != ways.end(); ++itr)
  {
    mergedWay = itr->second;
    mergedWay->setId(cachedId);
    LOG_VART(mergedWay.get());
    LOG_VART(mergedWay->getElementId());
    LOG_VART(mergedWay->getNodeCount());
  }

  return WaySubline(mergedWay, map);
}

}
