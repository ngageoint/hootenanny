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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "DataQualityMetricTagger.h"

// hoot
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/DisconnectedWayCriterion.h>
#include <hoot/core/criterion/EmptyWayCriterion.h>
#include <hoot/core/criterion/ElementIdCriterion.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/criterion/WayEndNodeCriterion.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/ops/DuplicateElementMarker.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, DataQualityMetricTagger)

DataQualityMetricTagger::DataQualityMetricTagger()
  : _orphanedNodes(0),
    _disconnectedWays(0),
    _emptyWays(0),
    _duplicateElementPairs(0),
    _numWayEndNodes(0)
{
}

void DataQualityMetricTagger::apply(OsmMapPtr& map)
{
  LOG_STATUS("Tagging features with quality issues...");

  std::shared_ptr<SetTagValueVisitor> tagVis;
  std::shared_ptr<FilteredVisitor> filteredVis;
  std::shared_ptr<ElementCriterion> crit;

  // We're only guaranteeing output data quality for the data in the task grid cells actually
  // replaced. Any data outside of the replacement grid may end up with quality issues that can't be
  // fixed until its replaced. So, restrict each of these quality checks to be in the replacement
  // AOI.

  tagVis = std::make_shared<SetTagValueVisitor>(MetadataTags::HootSuperfluous(), "yes");
  crit = std::make_shared<ElementIdCriterion>(ElementType::Node, SuperfluousNodeRemover::collectSuperfluousNodeIds(map, false, _bounds));
  filteredVis = std::make_shared<FilteredVisitor>(crit, tagVis);
  map->visitRo(*filteredVis);
  _orphanedNodes = static_cast<int>(tagVis->getNumFeaturesAffected());
  LOG_STATUS("Tagged " << StringUtils::formatLargeNumber(_orphanedNodes) << " orphaned nodes in output.");

  // SuperfluousNodeRemover took in a bounds above, but the remaining quality checks do not, so
  // combine their criteria with an InBoundsCriterion to make sure we only count elements within the
  // replacement bounds.
  std::shared_ptr<InBoundsCriterion> inBoundsCrit = std::make_shared<InBoundsCriterion>(true);
  inBoundsCrit->setBounds(_bounds);
  inBoundsCrit->setOsmMap(map.get());
  inBoundsCrit->setTreatWayNodesAsPartOfWays(false);

  tagVis = std::make_shared<SetTagValueVisitor>(MetadataTags::HootDisconnected(), "yes");
  crit = std::make_shared<ChainCriterion>(std::make_shared<DisconnectedWayCriterion>(map), inBoundsCrit);
  filteredVis = std::make_shared<FilteredVisitor>(crit, tagVis);
  map->visitRo(*filteredVis);
  _disconnectedWays = static_cast<int>(tagVis->getNumFeaturesAffected());
  LOG_STATUS("Tagged " << StringUtils::formatLargeNumber(_disconnectedWays) << " disconnected ways in output.");

  tagVis = std::make_shared<SetTagValueVisitor>(MetadataTags::HootEmptyWay(), "yes");
  crit = std::make_shared<ChainCriterion>(std::make_shared<EmptyWayCriterion>(), inBoundsCrit);
  filteredVis = std::make_shared<FilteredVisitor>(crit, tagVis);
  map->visitRo(*filteredVis);
  _emptyWays = static_cast<int>(tagVis->getNumFeaturesAffected());
  LOG_STATUS("Tagged " << StringUtils::formatLargeNumber(_emptyWays) << " empty ways in output.");

  tagVis = std::make_shared<SetTagValueVisitor>(MetadataTags::HootWayEndNode(), "yes");
  crit = std::make_shared<ChainCriterion>(std::make_shared<WayEndNodeCriterion>(map, false), inBoundsCrit);
  filteredVis = std::make_shared<FilteredVisitor>(crit, tagVis);
  map->visitRo(*filteredVis);
  _numWayEndNodes = static_cast<int>(tagVis->getNumFeaturesAffected());
  LOG_STATUS("Tagged " << StringUtils::formatLargeNumber(_numWayEndNodes) << " way end nodes in output.");

  DuplicateElementMarker dupeMarker;
  dupeMarker.setCoordinateComparisonSensitivity(8);
  dupeMarker.apply(map);
  _duplicateElementPairs = static_cast<int>(dupeMarker.getNumFeaturesAffected());
  LOG_STATUS("Tagged " << StringUtils::formatLargeNumber(_duplicateElementPairs) << " duplicate feature pairs in output.");
  LOG_STATUS("Containing way types for duplicate way nodes: " << dupeMarker.getContainingWayTypes());
}

}
