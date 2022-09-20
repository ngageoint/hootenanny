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

#include "RoadCrossingPolyMarker.h"

// Hoot
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/CriterionUtils.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/geometry/GeometricRelationship.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/SpatialIndexer.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RoadCrossingPolyMarker)

RoadCrossingPolyMarker::RoadCrossingPolyMarker()
  : _addValidationTags(false),
    _numRoads(0),
    _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
  setConfiguration(conf());
}

void RoadCrossingPolyMarker::setConfiguration(const Settings& conf)
{
  _crossingRulesFile = ConfigOptions(conf).getHighwayCrossingPolyRules().trimmed();
}

void RoadCrossingPolyMarker::apply(const OsmMapPtr& map)
{
  _numAffected = 0; // roads we mark for review
  _numProcessed = 0; // all ways
  _numRoads = 0; // all roads
  _map = map;
  _markedRoads.clear();

  // If there are no polygons in the input, then skip initialization of the road crossing poly
  // rules, since the road index creation can be expensive when calling conflation in a loop.
  ElementCriterionPtr polyCrit = std::make_shared<PolygonCriterion>(_map);
  ElementCriterionPtr tagCrit = std::make_shared<NotCriterion>(std::make_shared<TagKeyCriterion>("highway"));
  ElementCriterionPtr crit = std::make_shared<ChainCriterion>(polyCrit, tagCrit);
  if (FilteredVisitor::getStat(crit, std::make_shared<ElementCountVisitor>(), _map) == 0)
  {
    LOG_DEBUG("No polygons found in input map. Skipping marking roads crossing polygons.");
    return;
  }

  _crossingRules = RoadCrossingPolyRule::readRules(_crossingRulesFile, _map);

  ReviewMarker reviewMarker;
  const WayMap& ways = _map->getWays();
  LOG_VARD(ways.size());
  HighwayCriterion highwayCrit(_map);
  for (auto waysItr = ways.begin(); waysItr != ways.end(); ++waysItr)
  {
    WayPtr way = waysItr->second;
    LOG_VART(way->getElementId());

    // for each road
    if (highwayCrit.isSatisfied(way))
    {
      // for each road crossing poly rule
      for (const auto& rule : qAsConst(_crossingRules))
      {
        // If we haven't already marked this road for review, its not in another review, and it
        // isn't allowed to cross the type of poly specified by the current rule... (actually, now
        // not sure that skipping the road if its already involved in another review is the best
        // behavior)
        if (!_markedRoads.contains(way->getElementId()) &&
            !ReviewMarker::isNeedsReview(_map, way) &&
            (!rule.getAllowedRoadTagFilter() || !rule.getAllowedRoadTagFilter()->isSatisfied(way)))
        {
          std::shared_ptr<geos::geom::Envelope> env(way->getEnvelope(_map));
          LOG_VART(env);

          // Get all nearby polys to the road that pass our poly filter.
          const std::set<ElementId> neighborIdsSet =
            SpatialIndexer::findNeighbors(*env, rule.getIndex(), rule.getIndexToEid(), _map, ElementType::Way, false);
          LOG_VART(neighborIdsSet.size());

          // for each nearby poly
          for (const auto& neighborId : neighborIdsSet)
          {
            LOG_VART(neighborId);
            ConstElementPtr neighbor = _map->getElement(neighborId);

            // If the road intersects the poly, mark it.
            if (neighbor && rule.getPolyFilter()->isSatisfied(neighbor) &&
                ElementGeometryUtils::haveGeometricRelationship(
                  way, neighbor, GeometricRelationship::Crosses, _map))
            {
              LOG_TRACE("Marking " << way->getElementId() << "...");
              LOG_VART(way);
              LOG_VART(rule.getPolyFilterString());
              LOG_VART(rule.getAllowedRoadTagFilterString());

              const QString description =
                "Road crossing polygon: " + rule.getPolyFilterString() + "; " + rule.getAllowedRoadTagFilterString();
              if (!_addValidationTags)
                reviewMarker.mark(_map, way, description, MetadataTags::HootReviewRoadCrossingPolygon(), 1.0);
              else
              {
                way->setTag(MetadataTags::HootValidationError(), description);
                way->setTag(MetadataTags::HootValidationSource(), "Hootenanny");
              }
              _markedRoads.insert(way->getElementId());
              _numAffected++;
            }
          }
        }
      }
      _numRoads++;
    }

    _numProcessed++;
    if (_numProcessed % (_taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "\tMarked " << StringUtils::formatLargeNumber(_numAffected) << " crossing roads out of " <<
        StringUtils::formatLargeNumber(_numRoads) << " total roads and " <<
        StringUtils::formatLargeNumber(ways.size())  << " total ways.");
    }
  }
  LOG_VARD(_numProcessed);
}

QStringList RoadCrossingPolyMarker::getCriteria() const
{
  QStringList criteria;
  criteria.append(HighwayCriterion::className());
  return criteria;
}

QString RoadCrossingPolyMarker::getValidationErrorMessage() const
{
  return _numAffected == 0 ? "" : "Roads crossing polygons errors: " + QString::number(_numAffected);
}

}
