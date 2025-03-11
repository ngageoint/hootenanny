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
 * @copyright Copyright (C) 2015-2025 Maxar (http://www.maxar.com/)
 */
#include "FindIntersectionsOp.h"

// Hoot
#include <hoot/core/algorithms/splitter/DualHighwaySplitter.h>
#include <hoot/core/algorithms/splitter/IntersectionSplitter.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/ElementIdCriterion.h>
#include <hoot/core/criterion/NodeCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/RelationCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/criterion/WayCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/DuplicateWayRemover.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/ops/SuperfluousWayRemover.h>
#include <hoot/core/ops/UnlikelyIntersectionRemover.h>
#include <hoot/core/ops/VisitorOp.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/visitors/FindIntersectionsVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
 
namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, FindHighwayIntersectionsOp)
HOOT_FACTORY_REGISTER(OsmMapOperation, FindRailwayIntersectionsOp)

void FindIntersectionsOp::apply(std::shared_ptr<OsmMap>& map)
{
  // remove all relations
  LOG_INFO(QString("%1 Relations found.").arg(map->getRelationCount()));
  std::shared_ptr<RemoveElementsVisitor> removeRelationsVis = std::make_shared<RemoveElementsVisitor>();
  removeRelationsVis->addCriterion(std::make_shared<RelationCriterion>());
  VisitorOp(removeRelationsVis).apply(map);
  LOG_INFO(QString("%1 Relations found, after removal").arg(map->getRelationCount()));

  // pragmatically remove "bad" data in OSM afghanistan
  // TODO: this really shouldn't be here
  std::shared_ptr<TagCriterion> pCrit = std::make_shared<TagCriterion>("source", "AIMS");
  RemoveElementsVisitor::removeWays(map, pCrit);

  // reproject into a planar projection centered in the middle of bounding box.
  MapProjector::projectToPlanar(map);

  DuplicateWayRemover::removeDuplicates(map);
  SuperfluousWayRemover::removeWays(map);
  // split ways up on intersections. This must come before DualHighwaySplitter. The
  // DualHighwaySplitter assumes that all intersections are on end nodes.
  IntersectionSplitter::splitIntersections(map);
  UnlikelyIntersectionRemover::removeIntersections(map);
  LOG_INFO("Assuming drives on right.");

  // find all intersections
  std::shared_ptr<FindIntersectionsVisitor> v(createVisitor());
  VisitorOp(v).apply(map);
  LOG_INFO(QString("%1 Intersections found.").arg(v->getIntersections().size()));

  // remove all ways first
  std::shared_ptr<RemoveElementsVisitor> removeWaysVis = std::make_shared<RemoveElementsVisitor>();
  removeWaysVis->addCriterion(std::make_shared<WayCriterion>());
  VisitorOp(removeWaysVis).apply(map);

  // remove anything that is not a node and in the list of intersections found
  std::vector<long> intersectionIds = v->getIntersections();
  std::set<long> intersectionIdsSet(intersectionIds.begin(), intersectionIds.end());
  std::shared_ptr<NotCriterion> intersectionCrit =
    std::make_shared<NotCriterion>(
      std::make_shared<ChainCriterion>(
        std::make_shared<ElementIdCriterion>(ElementType::Node, intersectionIdsSet),
        std::make_shared<NodeCriterion>()));
  std::shared_ptr<RemoveElementsVisitor> removeIntersectionsVis =
    std::make_shared<RemoveElementsVisitor>();
  removeIntersectionsVis->addCriterion(intersectionCrit);
  VisitorOp(removeIntersectionsVis).apply(map);

  MapCleaner().apply(map);
}

std::shared_ptr<FindIntersectionsVisitor> FindHighwayIntersectionsOp::createVisitor()
{
  return std::make_shared<FindHighwayIntersectionsVisitor>();
}

std::shared_ptr<FindIntersectionsVisitor> FindRailwayIntersectionsOp::createVisitor()
{
  return std::make_shared<FindRailwayIntersectionsVisitor>();
}

}
