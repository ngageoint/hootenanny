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
#include <hoot/core/algorithms/linearreference//WaySublineRemover.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

// ONLY ENABLE THIS DURING DEBUGGING; We don't want to tie it to debug.maps.write, as it may
// produce a very large number of output files.
const bool LinearDiffMerger::WRITE_DETAILED_DEBUG_MAPS = true;

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

  std::vector<ElementId> newWayIds = WaySublineRemover::removeSubline(way2, subline2, _map);
  if (!newWayIds.empty())
  {
    // Do bookkeeping to show the new ways that replaced the old ref way. Arbitrarily use the
    // first ID, since we can't map the modified sec way to more than one new way.
    replaced.emplace_back(way2->getElementId(), newWayIds.at(0));
  }
  if (WRITE_DETAILED_DEBUG_MAPS)
  {
    OsmMapWriterFactory::writeDebugMap(
      _map, "after-merge-" + eid1.toString() + "-" + eid2.toString());
  }
  LOG_VART(replaced);

  return false;
}

}
