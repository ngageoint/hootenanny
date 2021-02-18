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

#include "WaySublineRemover.h"

// Hoot
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/algorithms/splitter/WaySplitter.h>
#include <hoot/core/ops/RecursiveElementRemover.h>

// Standard
#include <vector>

namespace hoot
{

void WaySublineRemover::remove(
  WayPtr way, WayLocation& start, WayLocation& end, const OsmMapPtr& map)
{
  LOG_TRACE(
    "Removing subline for: " << way->getElementId() << " starting at: " << start <<
    " and ending at: " << end << "...");

  WaySplitter splitter(map, way);
  // split at start
  splitter.split(start);
  // split at end
  const std::vector<WayPtr> splitWays = splitter.split(end);
  LOG_VART(splitWays);

  // remove middle section
  RecursiveElementRemover(splitWays.at(0)->getElementId()).apply(map);
}

}
