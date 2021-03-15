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
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/geometry/ElementToGeometryConverter.h>
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

  _map = map;

  WayPtr way1 = std::dynamic_pointer_cast<Way>(_map->getElement(eid1));
  WayPtr way2 = std::dynamic_pointer_cast<Way>(_map->getElement(eid2));

  // Not sure what needs to be done for this with relations.
  if (!way1 || !way2)
  {
    return false;
  }

  // Remove any ways that span both inputs.
  _removeSpans(_map, way1, way2);

  // Determine the split size.
  const double minSplitSize = _getMinSplitSize(way1, way2);
  if (minSplitSize == 0.0)
  {
    return false;
  }

  // Split left into its maximal nearest sublines.
  std::vector<WayPtr> splitsLeft;
  WayPtr mnsLeft = _getMaximalNearestSubline(way1, way2, minSplitSize, splitsLeft);
  LOG_VART(mnsLeft.get());
  if (!mnsLeft || splitsLeft.size() == 0)
  {
    return false;
  }

  // Split right into its maximal nearest sublines.
  std::vector<WayPtr> splitsRight;
  WayPtr mnsRight = _getMaximalNearestSubline(way2, mnsLeft, minSplitSize, splitsRight);
  LOG_VART(mnsRight.get());
  if (!mnsRight || splitsRight.size() == 0)
  {
    return false;
  }

  // Add the sublines to the map, as that's required in order to average them.
  _map->addWay(mnsLeft);
  _map->addWay(mnsRight);

  // Average the two maximal nearest sublines.
  WayPtr averagedWay = WayAverager::replaceWaysWithAveragedWay(_map, mnsRight, mnsLeft);
  LOG_VART(averagedWay.get());
  if (!averagedWay)
  {
    // Remove the sublines that would have automatically been removed if averaging was successful.
    RemoveWayByEid::removeWay(_map, mnsLeft->getId());
    RemoveWayByEid::removeWay(_map, mnsRight->getId());
    return false;
  }
  LOG_VART(averagedWay->getElementId());
  const long pid = Way::getPid(way1, way2);
  // If the the parent IDs for both matched ways are empty, we won't write the empty ID to the
  // averaged portion to possibly avoid overwriting a pre-existing valid parent ID.
  if (pid != WayData::PID_EMPTY)
  {
    averagedWay->setPid(pid);
    LOG_TRACE("Set PID: " << pid << " on: " << averagedWay->getElementId());
  }

  // Remove the averaged ways.
  RemoveWayByEid::removeWay(_map, way1->getId());
  RemoveWayByEid::removeWay(_map, way2->getId());

  // Add the averaged way (don't understand why this needs to be done, since WayAverager is already
  // doing it).
  averagedWay->setStatus(Status::Conflated);
  _map->addWay(averagedWay);

  // Do bookkeeping on the ways being replaced.
  replaced.push_back(
    std::pair<ElementId, ElementId>(averagedWay->getElementId(), way1->getElementId()));
  replaced.push_back(
    std::pair<ElementId, ElementId>(averagedWay->getElementId(), way2->getElementId()));
  LOG_VART(replaced);

  return false;
}

double LinearAverageMerger::_getMinSplitSize(const ConstWayPtr& way1, const ConstWayPtr& way2) const
{
  ElementToGeometryConverter geomConverter(_map);
  // TODO: play with this value
  Meters minSplitSize = ConfigOptions().getWayMergerMinSplitSize();
  std::shared_ptr<geos::geom::LineString> lineString1 = geomConverter.convertToLineString(way1);
  std::shared_ptr<geos::geom::LineString> lineString2 = geomConverter.convertToLineString(way2);
  if (!lineString1 || !lineString2)
  {
    return 0.0;
  }
  LOG_VART(lineString1->getLength());
  LOG_VART(lineString2->getLength());
  // TODO: play with this value
  const double lengthMultiplier = ConfigOptions().getAverageConflationMinSplitSizeMultiplier();
  minSplitSize = std::min(minSplitSize, lineString1->getLength() * lengthMultiplier);
  minSplitSize = std::min(minSplitSize, lineString2->getLength() * lengthMultiplier);
  LOG_VART(minSplitSize);
  return minSplitSize;
}

WayPtr LinearAverageMerger::_getMaximalNearestSubline(
  const ConstWayPtr& way1, const ConstWayPtr& way2, const double minSplitSize,
  std::vector<WayPtr>& splits)
{
  MaximalNearestSubline maximalNearestSubline(
    _map, way1, way2, minSplitSize, way1->getCircularError() + way2->getCircularError());
  int index = -1;
  splits = maximalNearestSubline.splitWay(_map, index);
  LOG_VART(index);
  LOG_VART(splits.size());
  if (splits.size() == 0)
  {
    return WayPtr();
  }
  assert(index > -1);
  return splits[index];
}

void LinearAverageMerger::_mergeTags(
  const WayPtr& averagedWay, const WayPtr& originalWay1, const WayPtr& originalWay2)
{
  LOG_VART(averagedWay->getTags().size());
  Tags mergedTags =
    TagMergerFactory::mergeTags(averagedWay->getTags(), originalWay1->getTags(), ElementType::Way);
  averagedWay->setTags(mergedTags);
  LOG_VART(averagedWay->getTags().size());
  mergedTags =
    TagMergerFactory::mergeTags(averagedWay->getTags(), originalWay2->getTags(), ElementType::Way);
  averagedWay->setTags(mergedTags);
  LOG_VART(averagedWay->getTags().size());
  averagedWay->setStatus(Status::Conflated);
}

}
