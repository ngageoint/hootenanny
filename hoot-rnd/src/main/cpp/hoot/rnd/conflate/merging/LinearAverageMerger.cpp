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
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/algorithms/linearreference/WaySublineCollection.h>
#include <hoot/core/algorithms/splitter/MultiLineStringSplitter.h>
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/elements/ElementComparer.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/IdSwapOp.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/ops/RemoveElementByEid.h>
#include <hoot/core/ops/RemoveReviewsByEidOp.h>
#include <hoot/core/ops/RelationMemberSwapper.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/util/Validate.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/visitors/WaysVisitor.h>
#include <hoot/core/conflate/merging/WayNodeCopier.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/NoInformationCriterion.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/algorithms/WayAverager.h>

// Qt
#include <QSet>

namespace hoot
{

// ONLY ENABLE THIS DURING DEBUGGING; We don't want to tie it to debug.maps.write, as it may
// produce a very large number of files.
const bool LinearAverageMerger::WRITE_DETAILED_DEBUG_MAPS = false;

HOOT_FACTORY_REGISTER(Merger, LinearAverageMerger)

int LinearAverageMerger::logWarnCount = 0;

LinearAverageMerger::LinearAverageMerger() :
LinearMergerAbstract(),
_matchedBy(HighwayMatch::MATCH_NAME)
{
}

LinearAverageMerger::LinearAverageMerger(
  const std::set<std::pair<ElementId, ElementId>>& pairs,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher) :
_sublineMatcher(sublineMatcher),
_matchedBy(HighwayMatch::MATCH_NAME)
{
  _pairs = pairs;
  LOG_VART(_pairs);
}

void LinearAverageMerger::apply(const OsmMapPtr& /*map*/,
                                std::vector<std::pair<ElementId, ElementId>>& /*replaced*/)
{
  LOG_TRACE("Applying LinearAverageMerger...");
  LOG_VART(hoot::toString(_pairs));
  //LOG_VART(hoot::toString(replaced));


}

bool LinearAverageMerger::_mergePair(
  const OsmMapPtr& /*map*/, ElementId /*eid1*/, ElementId /*eid2*/,
  std::vector<std::pair<ElementId, ElementId>>& /*replaced*/)
{


  return false;
}

}
