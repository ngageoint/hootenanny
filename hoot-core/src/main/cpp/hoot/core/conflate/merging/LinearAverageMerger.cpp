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
#include "LinearAverageMerger.h"

// geos
//#include <geos/geom/CoordinateSequence.h>
//#include <geos/geom/CoordinateSequenceFactory.h>
//#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>

// hoot
//#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
//#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/algorithms/WayAverager.h>
#include <hoot/core/algorithms/subline-matching/MaximalNearestSubline.h>
#include <hoot/core/ops/RemoveWayByEid.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/schema/TagMergerFactory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(Merger, LinearAverageMerger)

// ONLY ENABLE THIS DURING DEBUGGING; We don't want to tie it to debug.maps.write, as it may
// produce a very large number of files.
const bool LinearAverageMerger::WRITE_DETAILED_DEBUG_MAPS = false;

LinearAverageMerger::LinearAverageMerger() :
LinearMergerAbstract()
{
  _matchedBy = HighwayMatch::MATCH_NAME;
}

LinearAverageMerger::LinearAverageMerger(
  const std::set<std::pair<ElementId, ElementId>>& pairs) :
LinearMergerAbstract()
{
  _pairs = pairs;
  LOG_VART(_pairs);
  _matchedBy = HighwayMatch::MATCH_NAME;
}

bool LinearAverageMerger::_mergePair(
  const OsmMapPtr& map, ElementId eid1, ElementId eid2,
  std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  if (LinearMergerAbstract::_mergePair(map, eid1, eid2, replaced))
  {
    return true;
  }

  WayPtr way1 = std::dynamic_pointer_cast<Way>(map->getElement(eid1));
  WayPtr way2 = std::dynamic_pointer_cast<Way>(map->getElement(eid2));

  if (!way1 || !way2)
  {
    return false;
  }

  // TODO: refactor

  // Determine the split size.
  ElementToGeometryConverter geomConverter(map);
  Meters minSplitSize = ConfigOptions().getWayMergerMinSplitSize();
  std::shared_ptr<geos::geom::LineString> lineString1 = geomConverter.convertToLineString(way1);
  std::shared_ptr<geos::geom::LineString> lineString2 = geomConverter.convertToLineString(way2);
  if (!lineString1 || !lineString2)
  {
    return false;
  }
  LOG_VART(lineString1->getLength());
  LOG_VART(lineString2->getLength());
  const double lengthMultiplier = ConfigOptions().getAverageConflationMinSplitSizeMultiplier();
  minSplitSize = std::min(minSplitSize, lineString1->getLength() * lengthMultiplier);
  minSplitSize = std::min(minSplitSize, lineString2->getLength() * lengthMultiplier);
  LOG_VART(minSplitSize);

  // Split left into its maximal nearest sublines.
  MaximalNearestSubline mns1(
    map, way1, way2, minSplitSize, way1->getCircularError() + way2->getCircularError());
  int mnsLeftIndex = -1;
  std::vector<WayPtr> splitsLeft = mns1.splitWay(map, mnsLeftIndex);
  LOG_VART(splitsLeft.size());
  if (splitsLeft.size() == 0)
  {
    return false;
  }
  assert(mnsLeftIndex > -1);
  WayPtr mnsLeft = splitsLeft[mnsLeftIndex];

  // Split right into its maximal nearest sublines.
  MaximalNearestSubline mns2(
    map, way2, mnsLeft, minSplitSize, way1->getCircularError() + way2->getCircularError());
  int mnsRightIndex = -1;
  std::vector<WayPtr> splitsRight = mns2.splitWay(map, mnsRightIndex);
  LOG_VART(splitsRight.size());
  if (splitsRight.size() == 0)
  {
    return false;
  }
  assert(mnsRightIndex > -1);
  WayPtr mnsRight = splitsRight[mnsRightIndex];

  // Add the way splits (?).
  for (size_t i = 0; i < splitsLeft.size(); i++)
  {
    map->addWay(splitsLeft[i]);
  }
  for (size_t i = 0; i < splitsRight.size(); i++)
  {
    map->addWay(splitsRight[i]);
  }

  // Average the two maximal nearest sublines.
  WayPtr averagedWay = WayAverager::average(map, mnsRight, mnsLeft);
  LOG_VART(averagedWay.get());
  if (!averagedWay)
  {
    return false;
  }
  LOG_VART(averagedWay->getElementId());

  // Merge the tags.
  LOG_VART(averagedWay->getTags().size());
  Tags mergedTags =
    TagMergerFactory::mergeTags(averagedWay->getTags(), way1->getTags(), ElementType::Way);
  averagedWay->setTags(mergedTags);
  LOG_VART(averagedWay->getTags().size());
  mergedTags =
    TagMergerFactory::mergeTags(averagedWay->getTags(), way2->getTags(), ElementType::Way);
  averagedWay->setTags(mergedTags);
  LOG_VART(averagedWay->getTags().size());
  averagedWay->setStatus(Status::Conflated);

  // Remove the original ways.
  RemoveWayByEid::removeWay(map, way1->getId());
  RemoveWayByEid::removeWay(map, way2->getId());

  // Add the merged way.
  map->addWay(averagedWay);

  // Adding this ID replacement wreaks havoc on the output. The original average merging was created
  // before the concept of updating the "replaced" list, although it still feels like this update
  // should be made in order to keep IDs consistent as multiple mergers are applied...but maybe
  // not?? The output is bad with this added even if you disable the original way removal above.
//  replaced.push_back(
//    std::pair<ElementId, ElementId>(way1->getElementId(), averagedWay->getElementId()));
//  replaced.push_back(
//    std::pair<ElementId, ElementId>(way2->getElementId(), averagedWay->getElementId()));
//  LOG_VART(replaced);

  return false;
}

}
