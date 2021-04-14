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
#include "LinearKeepRef1Merger.h"

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
const bool LinearKeepRef1Merger::WRITE_DETAILED_DEBUG_MAPS = true;

HOOT_FACTORY_REGISTER(Merger, LinearKeepRef1Merger)

int LinearKeepRef1Merger::logWarnCount = 0;

LinearKeepRef1Merger::LinearKeepRef1Merger() :
LinearMergerAbstract()
{
}

LinearKeepRef1Merger::LinearKeepRef1Merger(
  const std::set<std::pair<ElementId, ElementId>>& pairs,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher) :
_sublineMatcher(sublineMatcher)
{
  _pairs = pairs;
  LOG_VART(_pairs);
}

void LinearKeepRef1Merger::apply(
  const OsmMapPtr& map, std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  LOG_TRACE("Applying LinearKeepRef1Merger...");
  LOG_VART(hoot::toString(_pairs));
  LOG_VART(hoot::toString(replaced));
  LOG_VART(MapProjector::isPlanar(map));

  for (std::set<std::pair<ElementId, ElementId>>::const_iterator it = _pairs.begin();
       it != _pairs.end(); ++it)
  {
    ElementId eid1 = it->first;
    ElementId eid2 = it->second;

    for (size_t i = 0; i < replaced.size(); i++)
    {
      //LOG_VART(eid1);
      //LOG_VART(eid2);
      //LOG_VART(replaced[i].first);
      //LOG_VART(replaced[i].second);

      //LOG_TRACE("e1 before replacement check: " << map->getElement(eid1));
      //LOG_TRACE("e2 before replacement check: " << map->getElement(eid2));

      if (eid1 == replaced[i].first)
      {
        LOG_TRACE("Changing " << eid1 << " to " << replaced[i].second << "...");
        eid1 = replaced[i].second;
      }
      if (eid2 == replaced[i].first)
      {
        LOG_TRACE("Changing " << eid2 << " to " << replaced[i].second << "...");
        eid2 = replaced[i].second;
      }

      //LOG_TRACE("e1 after replacement check: " << map->getElement(eid1));
      //LOG_TRACE("e2 after replacement check: " << map->getElement(eid2));
    }

    _mergePair(map, eid1, eid2, replaced);
  }
}

bool LinearKeepRef1Merger::_mergePair(
  const OsmMapPtr& map, ElementId eid1, ElementId eid2,
  std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  ElementPtr e1 = map->getElement(eid1);
  ElementPtr e2 = map->getElement(eid2);
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

  WaySublineMatchString match;
  try
  {
    match = _sublineMatcher->findMatch(map, way1, way2);
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

  // Get the portion of the ref way that matched.
  WaySublineMatchString::MatchCollection matches = match.getMatches();
  LOG_VART(matches.size());
  WaySubline subline1 = matches.at(0).getSubline1();
  LOG_VART(subline1.getWay()->getElementId());
  LOG_VART(subline1);
  LOG_VART(subline1.getWay() == way1);
  LOG_VART(subline1.getWay()->getElementId() == way1->getElementId());

  std::vector<ElementId> newWayIds;
  if (subline1.getWay()->getElementId() == way1->getElementId()) // TODO: necessary?
  {
    WayLocation start(subline1.getStart());
    WayLocation end(subline1.getEnd());
    // If the matching subline runs the entire length of the ref way, just remove the entire ref
    // way.
    if (start.isExtreme() && end.isExtreme())
    {
      LOG_TRACE(
        "Subline matches covers entire ref way; removing entire ref way: " << way1->getElementId());
      RecursiveElementRemover(way1->getElementId()).apply(map);
    }
    else
    {
      // Otherwise, remove only the portion of the ref way that matched with the sec way.

      LOG_TRACE("Removing subline 1 from " << way1->getElementId() << "...");

      newWayIds = WaySublineRemover::remove(way1, start, end, map);
      LOG_VART(newWayIds);
      // Do bookkeeping to show the new ways that replaced the old ref way.
      if (!newWayIds.empty())
      {
        replaced.emplace_back(way1->getElementId(), newWayIds.at(0));
        ReplaceElementOp(way1->getElementId(), newWayIds.at(0), true).apply(map);
      }
      else
      {
        LOG_TRACE("No subline removed for " << way1->getElementId() << ".");
      }
    }
    if (WRITE_DETAILED_DEBUG_MAPS)
    {
      OsmMapWriterFactory::writeDebugMap(
        map, "after-merge-" + eid1.toString() + "-" + eid2.toString());
    }
  }

  // Remove the sec way.
  LOG_TRACE("Removing sec way: " << way2->getElementId() << "...");
  RecursiveElementRemover(way2->getElementId()).apply(map);

  LOG_VART(replaced);
  return false;
}

}
