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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "LinearDiffMerger.h"

// hoot
#include <hoot/core/algorithms/splitter/WaySublineRemover.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

// ONLY ENABLE THIS DURING DEBUGGING; We don't want to tie it to debug.maps.write, as it may
// produce a very large number of output files.
const bool LinearDiffMerger::WRITE_DETAILED_DEBUG_MAPS = true;
//QHash<ElementId, WayPtr> LinearDiffMerger::_originalWays;

HOOT_FACTORY_REGISTER(Merger, LinearDiffMerger)

LinearDiffMerger::LinearDiffMerger() :
LinearMergerAbstract()
{
}

LinearDiffMerger::LinearDiffMerger(
  const std::set<std::pair<ElementId, ElementId>>& pairs,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher) :
_sublineMatcher(sublineMatcher)
{
  _pairs = pairs;
}

bool LinearDiffMerger::_mergePair(
  ElementId eid1, ElementId eid2, std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  ElementPtr e1 = _map->getElement(eid1);
  ElementPtr e2 = _map->getElement(eid2);
  LOG_VART(e1.get());
  LOG_VART(e2.get());
  if (!e1 || !e2)
  {
    return false;
  }
  WayPtr way1 = std::dynamic_pointer_cast<Way>(e1);
  WayPtr way2 = std::dynamic_pointer_cast<Way>(e2);
  LOG_VART(way1.get());
  LOG_VART(way2.get());
  if (!way1 || !way2)
  {
    return false;
  }
  LOG_VART(eid1);
  LOG_VART(eid2);

//  WayPtr secWay;
//  if (_originalWays.contains(eid2))
//  {
//    secWay = _originalWays[eid2];
//  }
//  else
//  {
//    secWay = way2;
//  }

  WaySublineMatchString match;
  try
  {
    match = _sublineMatcher->findMatch(_map, way1, way2);
  }
  catch (const NeedsReviewException& e)
  {
    // TODO: Do something here?
    LOG_VART(e.getWhat());
  }
  LOG_VART(match.isValid());
  if (!match.isValid())
  {
    return false;
  }

  // Get the portion of the sec way that matched the ref way.
  WaySublineMatchString::MatchCollection matches = match.getMatches();
  LOG_VART(matches.size());
  WaySubline subline2 = matches.at(0).getSubline2();
  LOG_VART(subline2.getWay()->getElementId());
  LOG_VART(subline2);
  LOG_VART(subline2.getWay() == way2);
  LOG_VART(subline2.getWay()->getElementId() == way2->getElementId());

  std::vector<ElementId> newWayIds = removeSubline(way2, subline2, _map);
  if (!newWayIds.empty())
  {
    replaced.emplace_back(way2->getElementId(), newWayIds.at(0));
  }
  if (WRITE_DETAILED_DEBUG_MAPS)
  {
    OsmMapWriterFactory::writeDebugMap(
      _map, "after-merge-" + eid1.toString() + "-" + eid2.toString());
  }

  //LOG_VART(replaced);
  return false;
}

std::vector<ElementId> LinearDiffMerger::removeSubline(
  const WayPtr& way, const WaySubline& subline, const OsmMapPtr& map)
{
  if (!way)
  {
    return std::vector<ElementId>();
  }

  std::vector<ElementId> newWayIds;
  WayLocation start(subline.getStart());
  WayLocation end(subline.getEnd());

  // If the sec matching subline runs the entire length of the sec way, just remove the entire sec
  // way.
  if (start.isExtreme() && end.isExtreme())
  {
    LOG_TRACE(
      "Subline matches covers entire sec way; removing entire sec way: " << way->getElementId());
    RecursiveElementRemover(way->getElementId()).apply(map);
  }
  else
  {
    // Otherwise, remove only the portion of the sec way that matched with the ref way.

    LOG_TRACE("Removing subline from " << way->getElementId() << "...");

    newWayIds = WaySublineRemover::remove(way, start, end, map);
    LOG_VART(newWayIds);
    if (!newWayIds.empty())
    {
      // Do bookkeeping to show the new ways that replaced the old ref way. Arbitrarily use the
      // first ID, since we can't map the modified sec way to more than one new way.
      //replaced.emplace_back(way2->getElementId(), newWayIds.at(0));
      // Update references to the sec way.
      ReplaceElementOp(way->getElementId(), newWayIds.at(0), true).apply(map);
    }
    else
    {
      LOG_TRACE("No subline removed for " << way->getElementId() << ".");
    }
  }

  return newWayIds;
}

}
