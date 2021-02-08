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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "DualHighwayMarker.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/SpatialIndexer.h>
#include <hoot/core/criterion/ArbitraryCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/algorithms/extractors/ParallelScoreExtractor.h>
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/elements/WayUtils.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

// Boost
#include <boost/bind.hpp>

// Standard
#include <functional>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, DualHighwayMarker)

DualHighwayMarker::DualHighwayMarker() :
_minParallelScore(0.9),
_markCrossingRoads(false),
_numCrossing(0),
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
}

void DualHighwayMarker::setConfiguration(const Settings& conf)
{
  ConfigOptions confOpts(conf);
  _minParallelScore = confOpts.getDualHighwayMarkerParallelScoreThreshold();
  _markCrossingRoads = confOpts.getDualHighwayMarkerMarkCrossingRoads();
  _maxCrossingRoadsParallelScore =
    confOpts.getDualHighwayMarkerCrossingRoadsParallelScoreThreshold();
}

Meters DualHighwayMarker::_getSearchRadius(const ConstElementPtr& e) const
{
  return e->getCircularError();
}

bool DualHighwayMarker::_isMatchCandidate(ConstElementPtr element) const
{
  if (!element)
  {
    return false;
  }
  // TODO
  return OneWayCriterion().isSatisfied(element) && HighwayCriterion(_map).isSatisfied(element);
}

void DualHighwayMarker::_createIndex()
{
  LOG_DEBUG("Creating spatial index...");

  // No tuning done - see #3054
  std::shared_ptr<Tgs::MemoryPageStore> mps(new Tgs::MemoryPageStore(728));
  _index.reset(new Tgs::HilbertRTree(mps, 2));

  // Only index elements that satisfy isMatchCandidate.
  std::function<bool (ConstElementPtr e)> f =
    std::bind(&DualHighwayMarker::_isMatchCandidate, this, std::placeholders::_1);
  std::shared_ptr<ArbitraryCriterion> pCrit(new ArbitraryCriterion(f));

  SpatialIndexer v(
    _index, _indexToEid, pCrit,
    std::bind(&DualHighwayMarker::_getSearchRadius, this, std::placeholders::_1), _map);
  _map->visitRo(v);
  v.finalizeIndex();
}

void DualHighwayMarker::apply(const OsmMapPtr& map)
{
  _numAffected = 0;
  _numProcessed = 0;
  _map = map;
  _createIndex();
  _elementInfo.reset(new ConflateInfoCache(map));

  const WayMap& ways = _map->getWays();
  for (WayMap::const_iterator waysItr = ways.begin(); waysItr != ways.end(); ++waysItr)
  {
    WayPtr road = waysItr->second;
    LOG_VART(road->getElementId());

    if (!road || !_isMatchCandidate(road))
    {
      continue;
    }

    std::shared_ptr<geos::geom::Envelope> env(road->getEnvelope(_map));
    LOG_VART(env);
    const std::set<ElementId> neighborIds =
      SpatialIndexer::findNeighbors(*env, _index, _indexToEid, _map, ElementType::Way, false);
    LOG_VART(neighborIds.size());

    for (std::set<ElementId>::const_iterator neighborsItr = neighborIds.begin();
         neighborsItr != neighborIds.end(); ++neighborsItr)
    {
      LOG_VART(*neighborsItr);
      WayPtr neighborRoad = std::dynamic_pointer_cast<Way>(_map->getElement(*neighborsItr));
      // No match candidate check needed here, as the index takes care of that for us.
      if (neighborRoad)
      {
        const double parallelScore = ParallelScoreExtractor().extract(*_map, road, neighborRoad);
        LOG_VART(parallelScore);
        const bool roadsInSameDirection =
          DirectionFinder::isSimilarDirection2(_map, road, neighborRoad);
        LOG_VART(roadsInSameDirection);

        // TODO: explain
        if (parallelScore >= _minParallelScore && !roadsInSameDirection)
        {
          if (road->getTags()[MetadataTags::HootDualHighway()] != "yes")
          {
            road->getTags().set(MetadataTags::HootDualHighway(), "yes");
            _numAffected++;
          }
          if (neighborRoad->getTags()[MetadataTags::HootDualHighway()] != "yes")
          {
            neighborRoad->getTags().set(MetadataTags::HootDualHighway(), "yes");
            _numAffected++;
          }

          if (_markCrossingRoads && road->getTags()[MetadataTags::HootDualHighway()] == "yes" &&
              neighborRoad->getTags()[MetadataTags::HootDualHighway()] == "yes")
          {
            // TODO
            _markRoadsCrossingDividedRoads(road, neighborRoad);
            _markRoadsCrossingDividedRoads(neighborRoad, road);
          }
        }
      }
    }
  }
  LOG_VARD(_numAffected);
  LOG_VARD(_numCrossing);
}

void DualHighwayMarker::_markRoadsCrossingDividedRoads(
  const ConstWayPtr& divRoad1, const ConstWayPtr& divRoad2)
{
  LOG_TRACE(
    "Attempting to remove roads crossing over divided roads " << divRoad1->getElementId() <<
    " and " << divRoad2->getElementId() << "...")

  // Get all the roads connected to the first of the divided road pair.
  const QSet<long> connectedWaysToRoad1 = WayUtils::getConnectedWays(divRoad1->getId(), _map);
  LOG_VART(connectedWaysToRoad1);
  for (QSet<long>::const_iterator connectedWaysToRoad1Itr = connectedWaysToRoad1.begin();
       connectedWaysToRoad1Itr != connectedWaysToRoad1.end(); ++connectedWaysToRoad1Itr)
  {
    WayPtr wayConnectedToRoad1 = _map->getWay(*connectedWaysToRoad1Itr);
    if (wayConnectedToRoad1)
    {
      LOG_VART(wayConnectedToRoad1->getElementId());
    }
    // Make sure its a road and hasn't already been marked.
    if (wayConnectedToRoad1 &&
        wayConnectedToRoad1->getTags()[MetadataTags::HootDualHighwayCrossing()] != "yes" &&
        HighwayCriterion(_map).isSatisfied(wayConnectedToRoad1))
    {
      // Keep only those which have a single end connected to the first in the divided road pair.
      long connectedEndId = 0;
      long oppositeEndId = 0;
      const long firstNodeEndId = wayConnectedToRoad1->getFirstNodeId();
      const long lastNodeEndId = wayConnectedToRoad1->getLastNodeId();
      const bool road1ContainsFirstNode = divRoad1->containsNodeId(firstNodeEndId);
      const bool road1ContainsLastNode = divRoad1->containsNodeId(lastNodeEndId);

      LOG_VART(firstNodeEndId);
      LOG_VART(lastNodeEndId);
      LOG_VART(road1ContainsFirstNode);
      LOG_VART(road1ContainsLastNode);

      if (road1ContainsFirstNode && road1ContainsLastNode)
      {
        continue;
      }
      else if (road1ContainsFirstNode)
      {
        connectedEndId = firstNodeEndId;
        oppositeEndId = lastNodeEndId;
      }
      else if (road1ContainsLastNode)
      {
        connectedEndId = lastNodeEndId;
        oppositeEndId = firstNodeEndId;
      }
      LOG_VART(connectedEndId);
      LOG_VART(oppositeEndId);
      if (connectedEndId == 0)
      {
        continue;
      }

      // Keep only those whose opposite end is closer to the other divided road in the pair than
      // the one in the pair its connected to (in between the two roads).
      ConstNodePtr connectedNodeEnd = _map->getNode(connectedEndId);
      ConstNodePtr oppositeNodeEnd = _map->getNode(oppositeEndId);
      if (connectedNodeEnd && oppositeNodeEnd)
      {
        const double distanceBetweenOppositeEndAndOtherDivRoad =
          _elementInfo->getDistance(oppositeNodeEnd, divRoad2);
        LOG_VART(distanceBetweenOppositeEndAndOtherDivRoad);
        const double distanceBetweenConnectedEndAndDivRoad =
          _elementInfo->getDistance(connectedNodeEnd, divRoad1);
        LOG_VART(distanceBetweenConnectedEndAndDivRoad);
        if (distanceBetweenOppositeEndAndOtherDivRoad >= distanceBetweenConnectedEndAndDivRoad)
        {
          continue;
        }
      }

      // Keep only those that are mostly perpendicular with the two div roads.
      const double parallelScoreWithRoad1 =
        ParallelScoreExtractor().extract(*_map, divRoad1, wayConnectedToRoad1);
      LOG_VART(parallelScoreWithRoad1);
      const double parallelScoreWithRoad2 =
        ParallelScoreExtractor().extract(*_map, divRoad2, wayConnectedToRoad1);
      LOG_VART(parallelScoreWithRoad2);
      if (parallelScoreWithRoad1 <= _maxCrossingRoadsParallelScore &&
          parallelScoreWithRoad2 <= _maxCrossingRoadsParallelScore)
      {
        LOG_TRACE(
          "Marking " << wayConnectedToRoad1->getElementId() << " as crossing dual highway...");
        wayConnectedToRoad1->getTags().set(MetadataTags::HootDualHighwayCrossing(), "yes");
        _numCrossing++;
      }
    }
  }
}

QStringList DualHighwayMarker::getCriteria() const
{
  QStringList criteria;
  criteria.append(HighwayCriterion::className());
  return criteria;
}

}
