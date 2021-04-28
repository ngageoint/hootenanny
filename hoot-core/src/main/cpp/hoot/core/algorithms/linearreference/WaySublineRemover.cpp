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
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/algorithms/splitter/WaySplitter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/RemoveElementByEid.h>
//#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/algorithms/linearreference/WaySubline.h>

// Standard
#include <vector>

namespace hoot
{

std::vector<ElementId> WaySublineRemover::removeSubline(
  const WayPtr& way, const WaySubline& subline, OsmMapPtr& map,
  const bool removeWholeWayRecursively)
{
  if (!way || !subline.isValid())
  {
    LOG_WARN("Invalid way or subline.");
    return std::vector<ElementId>();
  }

  LOG_TRACE("Removing subline: " << subline << " from: " << way->getElementId() << "...");

  std::vector<ElementId> newWayIds;

  // If the matching subline runs the entire length of the way, just remove the entire way.
  if (subline.getStart().isExtreme() && subline.getEnd().isExtreme())
  {
    LOG_TRACE(
      "Subline matches covers entire way. Removing entire way: " << way->getElementId() << "...");

    if (removeWholeWayRecursively)
    {
      // remove it from parents and the map
      RemoveElementByEid(way->getElementId()).apply(map);
      // remove its children
      RecursiveElementRemover(way->getElementId()).apply(map);
    }
    else
    {
      // Use RemoveElementByEid here instead of RecursiveElementRemover so that the way is removed
      // from its parent before its removal. We also don't want to remove RecursiveElementRemover here
      // b/c the way's nodes may still belong to one the temp relations we created during splitting.
      RemoveElementByEid(way->getElementId()).apply(map);
    }

    if (ConfigOptions().getDebugMapsWrite() && ConfigOptions().getDebugMapsWriteDetailed())
    {
      OsmMapWriterFactory::writeDebugMap(
        map, "WaySublineRemover-after-entire-way-removal-" + way->getElementId().toString());
    }
  }
  else
  {
    // Otherwise, remove only the portion of the way that overlaps.

    LOG_TRACE("Removing subline from " << way->getElementId() << "...");

    newWayIds = removeSubline(way, subline.getStart(), subline.getEnd(), map);
    LOG_VART(newWayIds);
    if (!newWayIds.empty())
    {
      // Update references to the modified way.
      ReplaceElementOp(way->getElementId(), newWayIds.at(0), true).apply(map);

      if (ConfigOptions().getDebugMapsWrite() && ConfigOptions().getDebugMapsWriteDetailed())
      {
        OsmMapWriterFactory::writeDebugMap(
          map, "WaySublineRemover-after-way-ref-update-" + way->getElementId().toString());
      }
    }
    else
    {
      LOG_TRACE("No subline removed for " << way->getElementId() << ".");
    }
  }

  return newWayIds;
}

std::vector<ElementId> WaySublineRemover::removeSubline(
  const WayPtr& way, const WayLocation& start, const WayLocation& end, OsmMapPtr& map)
{
  if (!MapProjector::isPlanar(map))
  {
    throw IllegalArgumentException("Map must be in a planar projection.");
  }
  if (!start.isValid() || !end.isValid())
  {
    return std::vector<ElementId>();
  }
  // TODO: We may be able to use the same optimization in the other removeSubline method here where
  // we just remove the whole way if the start/end points are the ends.

  LOG_TRACE(
    "Removing subline for: " << way->getElementId() << " starting at: " << start <<
    " and ending at: " << end << "...");
  LOG_TRACE("Input way length: " << ElementGeometryUtils::calculateLength(way, map));

  // Make a copy of the input way so that the first split doesn't affect calculation of the second
  // one.
  WayPtr wayCopy1 = std::dynamic_pointer_cast<Way>(ElementPtr(way->clone()));
  // Thought about trying to preserve the original way ID on the first segment here but had problems
  // making it work.
  wayCopy1->setId(map->createNextWayId());
  LOG_VART(wayCopy1->getElementId());
  map->addWay(wayCopy1);
  // We copied the way, so we need to also copy the loc.
  WayLocation startCopy(map, wayCopy1, start.getSegmentIndex(), start.getSegmentFraction());

  WayPtr wayCopy2 = std::dynamic_pointer_cast<Way>(ElementPtr(way->clone()));
  wayCopy2->setId(map->createNextWayId());
  LOG_VART(wayCopy2->getElementId());
  map->addWay(wayCopy2);
  WayLocation endCopy(map, wayCopy2, end.getSegmentIndex(), end.getSegmentFraction());

  if (ConfigOptions().getDebugMapsWrite() && ConfigOptions().getDebugMapsWriteDetailed())
  {
    OsmMapWriterFactory::writeDebugMap(map, "WaySublineRemover-after-copied-ways-added");
  }

  // Split the way at the beginning of the section we want to remove. If a split is performed, the
  // copied input is automatically removed from the map and resulting newly created segments are
  // automatically added to the map.
  std::vector<ElementId> newWayIds1 = _split(wayCopy1, startCopy, map, true);
  const bool split1Performed = !newWayIds1.empty();
  LOG_VART(split1Performed);
  if (newWayIds1.empty())
  {
    // If no split was actually made, then we need to remove the copied way to avoid duplication in
    // the output.
    LOG_TRACE("Removing " << wayCopy1->getElementId() << "...");
    RecursiveElementRemover(wayCopy1->getElementId()).apply(map);
  }
  if (ConfigOptions().getDebugMapsWrite() && ConfigOptions().getDebugMapsWriteDetailed())
  {
    OsmMapWriterFactory::writeDebugMap(map, "WaySublineRemover-after-split-1-performed");
  }

  // Split the way at the end of the section we want to remove.
  std::vector<ElementId> newWayIds2 = _split(wayCopy2, endCopy, map, false);
  const bool split2Performed = !newWayIds2.empty();
  LOG_VART(split2Performed);
  if (newWayIds2.empty())
  {
    LOG_TRACE("Removing " << wayCopy2->getElementId() << "...");
    RecursiveElementRemover(wayCopy2->getElementId()).apply(map);
  }
  if (ConfigOptions().getDebugMapsWrite() && ConfigOptions().getDebugMapsWriteDetailed())
  {
    OsmMapWriterFactory::writeDebugMap(map, "WaySublineRemover-after-split-2-performed");
  }

  std::vector<ElementId> newWayIds = newWayIds1;
  newWayIds.insert(newWayIds.end(), newWayIds2.begin(), newWayIds2.end());
  LOG_VART(newWayIds.size());

  // Remove the original way from the map to avoid duplication, but only if any splitting was
  // actually done.
  if (!newWayIds.empty())
  {
    LOG_TRACE("Removing " << way->getElementId() << "...");
    RecursiveElementRemover(way->getElementId()).apply(map);
    QString outputLengthsStr =
      QString::number(ElementGeometryUtils::calculateLength(map->getElement(newWayIds.at(0)), map));
    if (newWayIds.size() > 1)
    {
      outputLengthsStr +=
        QString::number(
          ElementGeometryUtils::calculateLength(map->getElement(newWayIds.at(1)), map));
    }
    LOG_TRACE("Output way length(s): " << outputLengthsStr);

    if (ConfigOptions().getDebugMapsWrite() && ConfigOptions().getDebugMapsWriteDetailed())
    {
      OsmMapWriterFactory::writeDebugMap(
        map, "WaySublineRemover-after-original-way-removed-" + way->getElementId().toString());
    }
  }
  else
  {
    LOG_TRACE(way->getElementId() << " was not split.")
  }

  LOG_TRACE(map->containsElement(way->getElementId()));
  LOG_TRACE(map->containsElement(wayCopy1->getElementId()));
  LOG_TRACE(map->containsElement(wayCopy2->getElementId()));

  return newWayIds;
}

std::vector<ElementId> WaySublineRemover::_split(
  const WayPtr& way, WayLocation& splitLocation, OsmMapPtr& map, const bool keepFirstSegment)
{
  std::vector<ElementId> newWayIds;
  LOG_VART(splitLocation.isExtreme());
  // Don't split anything if we're splitting at the beginning or end of the way.
  if (!splitLocation.isExtreme())
  {
    WaySplitter splitter(map, way);
    // split the way
    const std::vector<WayPtr> splitWays = splitter.split(splitLocation);
    LOG_VART(splitWays.size());
    assert(splitWays.size() == 2);
    const int keepIndex = keepFirstSegment ? 0 : 1;
    const int removeIndex = keepFirstSegment ? 1 : 0;
    // remove the segment we don't want
    LOG_TRACE("Removing " << splitWays.at(removeIndex)->getElementId());
    RecursiveElementRemover(splitWays.at(removeIndex)->getElementId()).apply(map);
    // record the kept segment
    newWayIds.push_back(splitWays.at(keepIndex)->getElementId());
  }
  return newWayIds;
}

}
