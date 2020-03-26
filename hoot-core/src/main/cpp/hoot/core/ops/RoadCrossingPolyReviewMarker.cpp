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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RoadCrossingPolyReviewMarker.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/SpatialIndexer.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/GeometricRelationship.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RoadCrossingPolyReviewMarker)

RoadCrossingPolyReviewMarker::RoadCrossingPolyReviewMarker() :
_numRoads(0),
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
}

void RoadCrossingPolyReviewMarker::setConfiguration(const Settings& conf)
{
  _crossingRulesFile = ConfigOptions(conf).getHighwayCrossingPolyRules().trimmed();
}

void RoadCrossingPolyReviewMarker::apply(const OsmMapPtr& map)
{
  _numAffected = 0;
  _numProcessed = 0;
  _numRoads = 0;
  _map = map;
  _markedRoads.clear();
  _crossingRules = RoadCrossingPolyRule::readRules(_crossingRulesFile, _map);

  ReviewMarker reviewMarker;
  const WayMap& ways = _map->getWays();
  for (WayMap::const_iterator waysItr = ways.begin(); waysItr != ways.end(); ++waysItr)
  {
    WayPtr way = waysItr->second;
    LOG_VART(way->getElementId());

    // for each road
    if (HighwayCriterion(_map).isSatisfied(way))
    {
      // for each road crossing poly rule
      for (QList<RoadCrossingPolyRule>::const_iterator rulesItr = _crossingRules.begin();
           rulesItr != _crossingRules.end(); ++rulesItr)
      {
        RoadCrossingPolyRule rule = *rulesItr;

        // if we haven't already marked this road for review, its not in another review, and this
        // it isn't allowed to cross the type of poly specified by the current rule
        if (!_markedRoads.contains(way->getElementId()) &&
            !ReviewMarker::isNeedsReview(_map, way) &&
            (!rule.getAllowedRoadTagFilter() || !rule.getAllowedRoadTagFilter()->isSatisfied(way)))
        {
          std::shared_ptr<geos::geom::Envelope> env(way->getEnvelope(_map));
          LOG_VART(env);

          // get all nearby polys to the road that pass our poly filter
          const std::set<ElementId> neighborIdsSet =
            SpatialIndexer::findNeighbors(
                *env, rule.getIndex(), rule.getIndexToEid(), _map, ElementType::Way, false);
          LOG_VART(neighborIdsSet.size());

          // for each nearby poly
          for (std::set<ElementId>::const_iterator neighborIdsItr = neighborIdsSet.begin();
               neighborIdsItr != neighborIdsSet.end(); ++neighborIdsItr)
          {
            const ElementId neighborId = *neighborIdsItr;
            LOG_VART(neighborId);
            ConstElementPtr neighbor = _map->getElement(neighborId);

            // if the road intersects the poly, flag it for review
            if (neighbor && rule.getPolyFilter()->isSatisfied(neighbor) &&
                OsmUtils::haveGeometricRelationship(
                  way, neighbor, GeometricRelationship::Crosses, _map))
            {
              LOG_TRACE("Marking " << way->getElementId() << " for review...");
              LOG_VART(way);
              LOG_VART(rule.getPolyFilterString());
              LOG_VART(rule.getAllowedRoadTagFilterString());
//              LOG_VART(
//                OsmUtils::haveGeometricRelationship(
//                  way, neighbor, GeometricRelationship::Overlaps, _map));
//              LOG_VART(
//                OsmUtils::haveGeometricRelationship(
//                  way, neighbor, GeometricRelationship::Crosses, _map));
//              LOG_VART(
//                OsmUtils::haveGeometricRelationship(
//                  way, neighbor, GeometricRelationship::Overlaps, _map));
//              LOG_VART(
//                OsmUtils::haveGeometricRelationship(
//                  way, neighbor, GeometricRelationship::Touches, _map));

              reviewMarker.mark(
                _map, way,
                "Road crossing polygon: " + rule.getPolyFilterString() + "; " +
                  rule.getAllowedRoadTagFilterString(),
                MetadataTags::HootReviewRoadCrossingPolygon(), 1.0);
              _markedRoads.insert(way->getElementId());
              _numAffected++;
            }
          }
        }
      }
      _numRoads++;
    }

    _numProcessed++;
    if (_numProcessed % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "\tMarked " << StringUtils::formatLargeNumber(_numAffected) << " crossing roads for " <<
        "review out of " << StringUtils::formatLargeNumber(_numRoads) << " total roads and " <<
        StringUtils::formatLargeNumber(_numProcessed)  << " total ways.");
    }
  }
}

}
