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

#include "WaySublineRemover.h"

// Hoot
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/algorithms/splitter/WaySplitter.h>
#include <hoot/core/ops/RecursiveElementRemover.h>

// Standard
#include <vector>

namespace hoot
{

std::vector<ElementId> WaySublineRemover::remove(
  WayPtr way, WayLocation& start, WayLocation& end, const OsmMapPtr& map)
{
  LOG_DEBUG(
    "Removing subline for: " << way->getElementId() << " starting at: " << start <<
    " and ending at: " << end << "...");

  std::vector<ElementId> newWayIds;

  WaySplitter splitter(map, way);
  // split at start
  const std::vector<WayPtr> splitWays1 = splitter.split(start);
  LOG_VARD(splitWays1);
  if (splitWays1.size() == 1)
  {
    RecursiveElementRemover(splitWays1.at(0)->getElementId()).apply(map);
    return newWayIds;
  }
  // split at end
  const std::vector<WayPtr> splitWays2 = splitter.split(end);
  LOG_VARD(splitWays2);
  if (splitWays2.size() == 1)
  {
    RecursiveElementRemover(splitWays2.at(0)->getElementId()).apply(map);
    return newWayIds;
  }

  // remove middle section
  RecursiveElementRemover(splitWays2.at(0)->getElementId()).apply(map);

  newWayIds.push_back(splitWays1.at(0)->getElementId());
  //if (splitWays2.size() > 0)
  //{
    newWayIds.push_back(splitWays2.at(1)->getElementId());
  //}
  LOG_VARD(newWayIds);
  return newWayIds;
}

}
