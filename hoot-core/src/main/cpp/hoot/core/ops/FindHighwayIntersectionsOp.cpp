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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "FindHighwayIntersectionsOp.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/ops/DuplicateWayRemover.h>
#include <hoot/core/ops/VisitorOp.h>
#include <hoot/core/ops/SuperfluousWayRemover.h>
#include <hoot/core/ops/UnlikelyIntersectionRemover.h>
#include <hoot/core/algorithms/splitter/DualWaySplitter.h>
#include <hoot/core/algorithms/splitter/IntersectionSplitter.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/visitors/FindHighwayIntersectionsVisitor.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/conflate/cleaning/MapCleaner.h>
#include <hoot/core/criterion/ElementInIdListCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/NodeCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, FindHighwayIntersectionsOp)

FindHighwayIntersectionsOp::FindHighwayIntersectionsOp()
{
}

void FindHighwayIntersectionsOp::apply(boost::shared_ptr<OsmMap> &map)
{
  // remove all relations
  LOG_INFO(QString("%1 Relations found.").arg(map->getRelations().size()));
  boost::shared_ptr<ElementTypeCriterion> relationCrit(
    new ElementTypeCriterion(ElementType::Relation));
  VisitorOp(new RemoveElementsVisitor(relationCrit)).apply(map);
  LOG_INFO(QString("%1 Relations found, after removal").arg(map->getRelations().size()));

  /// @todo move this to a config file.
  // pragmatically remove "bad" data in OSM afghanistan
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

  // find all intersections
  boost::shared_ptr<FindHighwayIntersectionsVisitor> v(new FindHighwayIntersectionsVisitor());
  VisitorOp(v).apply(map);
  LOG_INFO(QString("%1 Intersections found.").arg(v->getIntersections().size()));

  // remove all ways first
  boost::shared_ptr<ElementTypeCriterion> wayCrit(new ElementTypeCriterion(ElementType::Way));
  VisitorOp(new RemoveElementsVisitor(wayCrit)).apply(map);

  // remove anything that is not a node and in the list of intersections found
  boost::shared_ptr<NotCriterion> intersectionCrit(
    new NotCriterion(
      new ChainCriterion(
        boost::shared_ptr<ElementInIdListCriterion>(
          new ElementInIdListCriterion(v->getIntersections())),
        boost::shared_ptr<NodeCriterion>(new NodeCriterion()))));
  VisitorOp(new RemoveElementsVisitor(intersectionCrit)).apply(map);

  MapCleaner().apply(map);
}

}
