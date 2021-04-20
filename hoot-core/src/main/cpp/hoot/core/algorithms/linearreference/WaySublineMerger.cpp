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
#include <hoot/core/algorithms/linearreference/WaySubline.h>
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/core/conflate/merging/LinearMergerFactory.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

WaySubline WaySublineMerger::mergeSublines(
  const WaySubline& subline1, const WaySubline& subline2, const ConstOsmMapPtr& map)
{
  if (!subline1.isValid() || !subline2.isValid())
  {
    LOG_WARN("Invalid subline(s).");
    return WaySubline();
  }

  const long cachedId = subline1.getWay()->getId();
  LOG_VART(cachedId);

  LOG_TRACE("Cloning sublines: " << subline1 << " and " << subline2 << " to temp map...");
  OsmMapPtr tempMap = std::make_shared<OsmMap>();
  CopyMapSubsetOp mapCopier(
    map, subline1.getWay()->getElementId(), subline2.getWay()->getElementId());
  mapCopier.apply(tempMap);
  MapProjector::projectToPlanar(tempMap);
  WayPtr way1 = tempMap->getWay(subline1.getWay()->getElementId());
  way1->setStatus(Status::Unknown2);
  LOG_VART(way1->getElementId());
  LOG_VART(way1->getNodeCount());
  WayPtr way2 = tempMap->getWay(subline2.getWay()->getElementId());
  way2->setStatus(Status::Unknown1);
  LOG_VART(way2->getElementId());
  LOG_VART(way2->getNodeCount());
  LOG_VART(tempMap->getWayCount());

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

  LOG_TRACE("Merging ways: " << way1->getElementId() << " and " << way2->getElementId() << "...");
  std::set<std::pair<ElementId, ElementId>> pairs;
  pairs.insert(std::pair<ElementId, ElementId>(way1->getElementId(), way2->getElementId()));
  MergerPtr merger = LinearMergerFactory::getMerger(pairs, sublineMatcher);
  LOG_VART(merger->getName());
  std::vector<std::pair<ElementId, ElementId>> replaced;
  merger->apply(tempMap, replaced);
  LOG_VART(tempMap->getWayCount());
  if (tempMap->getWayCount() == 0)
  {
    return WaySubline();
  }

  WayPtr mergedWay;
  const WayMap& ways = tempMap->getWays();
  for (WayMap::const_iterator itr = ways.begin(); itr != ways.end(); ++itr)
  {
    mergedWay = itr->second;
    mergedWay->setId(cachedId);
    LOG_VART(mergedWay.get());
    LOG_VART(mergedWay->getElementId());
    LOG_VART(mergedWay->getNodeCount());
  }

  LOG_TRACE("Returning subline for merged way: " << mergedWay->getElementId() << "...");
  return WaySubline(mergedWay, tempMap);
}

}
