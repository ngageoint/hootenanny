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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "FindIntersectionsOp.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/conflate/DuplicateNameRemover.h>
#include <hoot/core/conflate/DuplicateWayRemover.h>
#include <hoot/core/conflate/ImpliedDividedMarker.h>
#include <hoot/core/conflate/SmallWayMerger.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/ops/VisitorOp.h>
#include <hoot/core/conflate/SuperfluousWayRemover.h>
#include <hoot/core/conflate/UnlikelyIntersectionRemover.h>
#include <hoot/core/conflate/splitter/DualWaySplitter.h>
#include <hoot/core/conflate/splitter/IntersectionSplitter.h>
#include <hoot/core/filters/TagCriterion.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/visitors/RemoveDuplicateAreaVisitor.h>
#include <hoot/core/visitors/RemoveEmptyAreasVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/FindIntersectionsVisitor.h>
#include <hoot/core/filters/IntersectionFilter.h>
#include <hoot/core/filters/ElementTypeCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, FindIntersectionsOp)

FindIntersectionsOp::FindIntersectionsOp()
{
}

void FindIntersectionsOp::apply(boost::shared_ptr<OsmMap>& map)
{
  // remove all relations
  LOG_INFO(QString("%1 Relations found.").arg(map->getRelations().size()));
  boost::shared_ptr<ElementTypeCriterion> rFilter(new ElementTypeCriterion(ElementType::Relation));
  VisitorOp(new RemoveElementsVisitor(rFilter)).apply(map);
  LOG_INFO(QString("%1 Relations found, after removal").arg(map->getRelations().size()));

  /// @todo move this to a config file.
  // pragmatically remove "bad" data in OSM afghanistan
  //map->removeWays(TagFilter(Filter::FilterMatches, "source", "AIMS"));
  boost::shared_ptr<TagCriterion> pCrit(new TagCriterion("source", "AIMS"));
  RemoveElementsVisitor::removeWays(map, pCrit);

  // reproject into a planar projection centered in the middle of bounding box.
  MapProjector::projectToPlanar(map);

  DuplicateWayRemover::removeDuplicates(map);
  SuperfluousWayRemover::removeWays(map);
  // split ways up on intersections. This must come before DualWaySplitter. The DualWaySplitter
  // assumes that all intersections are on end nodes.
  IntersectionSplitter::splitIntersections(map);
  UnlikelyIntersectionRemover::removeIntersections(map);
  LOG_INFO("Assuming drives on right.");
//  map = DualWaySplitter::splitAll(map, DualWaySplitter::Right, 12.5);
//  map = ImpliedDividedMarker::markDivided(map);

//  LOG_INFO("removeDuplicates()");
//  DuplicateNameRemover::removeDuplicates(map);
//  LOG_INFO("SmallWayMerger::mergeWays()");
//  SmallWayMerger::mergeWays(map, 15.0);

//  LOG_INFO("RemoveEmptyAreasVisitor()");
//  VisitorOp(new RemoveEmptyAreasVisitor()).apply(map);
//  LOG_INFO("RemoveDuplicateAreaVisitor()");
//  VisitorOp(new RemoveDuplicateAreaVisitor()).apply(map);

  // find all intersections
//  LOG_INFO("FindIntersectionsVisitor()");
  boost::shared_ptr<FindIntersectionsVisitor> v(new FindIntersectionsVisitor());
  VisitorOp(v).apply(map);
  LOG_INFO(QString("%1 Intersections found.").arg(v->getIntersections().size()));

  // remove all ways first
  boost::shared_ptr<ElementTypeCriterion> wayFilter(new ElementTypeCriterion(ElementType::Way));
  VisitorOp(new RemoveElementsVisitor(wayFilter)).apply(map);

  // then remove everything except for the intersection that we found
  boost::shared_ptr<IntersectionFilter> intersectionFilter(new IntersectionFilter(v->getIntersections()));
  VisitorOp(new RemoveElementsVisitor(intersectionFilter)).apply(map);


  // Apply any user specified operations.
  NamedOp(ConfigOptions().getMapCleanerTransforms()).apply(map);
}

}
