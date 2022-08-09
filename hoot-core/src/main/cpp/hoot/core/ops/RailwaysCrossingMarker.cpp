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

#include "RailwaysCrossingMarker.h"

// Hoot
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/criterion/ArbitraryCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/geometry/GeometricRelationship.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/SpatialIndexer.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

// Boost
#include <boost/bind.hpp>

// Standard
#include <functional>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RailwaysCrossingMarker)

RailwaysCrossingMarker::RailwaysCrossingMarker()
  : _markIntraDatasetCrossings(false),
    _numRailways(0),
    _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
}

void RailwaysCrossingMarker::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setTagExcludeFilter(configOptions.getRailwaysCrossingMarkerIgnoreTypes());
  setMarkIntraDatasetCrossings(configOptions.getRailwaysCrossingMarkerMarkIntraDatasetCrossings());
}

void RailwaysCrossingMarker::apply(const OsmMapPtr& map)
{
  _numAffected = 0; // rails we mark for review
  _numProcessed = 0; // all ways
  _numRailways = 0; // all rails
  _map = map;
  _markedRailways.clear();

  // If there are no railways in the input, then skip a potentially expensive initialization of
  // the railway index.
  if (FilteredVisitor::getStat(
        std::make_shared<NotCriterion>(std::make_shared<RailwayCriterion>()),
        std::make_shared<ElementCountVisitor>(), _map) == 0)
  {
    LOG_DEBUG("No railways found in input map. Skipping marking crossing railways.");
    return;
  }

  _createIndex();

  ReviewMarker reviewMarker;
  const WayMap& ways = _map->getWays();
  LOG_VARD(ways.size());
  for (auto waysItr = ways.begin(); waysItr != ways.end(); ++waysItr)
  {
    ConstWayPtr way = waysItr->second;
    LOG_VART(way->getElementId());

    // For each rail,
    if (_isMatchCandidate(way))
    {
      // and for each rail that is nearby the rail,
      std::shared_ptr<geos::geom::Envelope> env(way->getEnvelope(_map));
      const std::set<ElementId> neighborIdsSet =
        SpatialIndexer::findNeighbors(*env, _index, _indexToEid, _map, ElementType::Way, false);
      LOG_VART(neighborIdsSet.size());
      for (const auto& neighborId : neighborIdsSet)
      {
        LOG_VART(neighborId);
        ConstElementPtr neighbor = _map->getElement(neighborId);

        // if the rail crosses the nearby rail, mark it.
        if (neighbor)
        {
          // Create a unique ID string made from the two feature ID's. If we've already processed
          // this pair, then skip.
          const QString idStr1 = QString("%1;%2").arg(way->getElementId().toString(), neighbor->getElementId().toString());
          const QString idStr2 = QString("%1;%2").arg(neighbor->getElementId().toString(), way->getElementId().toString());
          const bool pairNotProcessed =
            !_markedRailways.contains(idStr1) && !_markedRailways.contains(idStr2);

          // This tells us if the two features were from the same input dataset *and* unmodified. If
          // either was conflated, there's not a way to tell if they were from the same dataset by
          // this point (maybe a way around that; but not sure we need it).
          const bool haveSameStatus =
            way->getStatus() != Status::Conflated && neighbor->getStatus() != Status::Conflated &&
            way->getStatus() == neighbor->getStatus();

          // Was either feature involved in a one-to-many match?
          const bool oneToManyMatch =
            way->getTags().contains(MetadataTags::HootRailwayOneToManyMatchSecondary()) ||
            neighbor->getTags().contains(MetadataTags::HootRailwayOneToManyMatchSecondary());

          LOG_VART(way->getElementId());
          LOG_VART(neighborId);
          LOG_VART(pairNotProcessed);
          LOG_VART(haveSameStatus);
          LOG_VART(oneToManyMatch);

          // If we haven't already processed the pair, we're either allowing intra-dataset features
          // to be marked as crossing OR we aren't and they don't have the same status, neither
          // feature was involved in a one-to-many match, and they cross, then mark the pair for
          // review. We skip anything involved in a one-to-many match since we know that in that
          // case the secondary feature will not have been geometrically merged into the reference
          // feature. Due to that, there's no way conflation could have created a new crossing pair
          // of rails in that situation.
          if (pairNotProcessed && (_markIntraDatasetCrossings || !haveSameStatus) &&
              !oneToManyMatch &&
              ElementGeometryUtils::haveGeometricRelationship(
                way, neighbor, GeometricRelationship::Crosses, _map))
          {
            // TODO: It would be helpful if we'd could also mark the points at which they cross here
            // and tag those for debugging purposes.
            LOG_TRACE(
              "Marking for review: " << way->getElementId() << " and " << neighborId << "...");
            reviewMarker.mark(
              _map, way, neighbor, "Crossing railways", MetadataTags::HootReviewCrossingRailways(),
              1.0);
            _markedRailways.insert(idStr1);
            _markedRailways.insert(idStr2);
            _numAffected++;
          }
        }
      }
      _numRailways++;
    }

    _numProcessed++;
    if (_numProcessed % (_taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "\tMarked " << StringUtils::formatLargeNumber(_numAffected) << " crossing railways " <<
        "out of " << StringUtils::formatLargeNumber(_numRailways) << " total railways and " <<
        StringUtils::formatLargeNumber(ways.size())  << " total ways.");
    }
  }
}

void RailwaysCrossingMarker::setTagExcludeFilter(const QStringList& excludeTags)
{
  if (excludeTags.empty())
    return;

  _tagExcludeFilter = std::make_shared<ChainCriterion>();
  for (const auto& kvp : excludeTags)
  {
    const QStringList kvpParts = Tags::kvpToParts(kvp);
    if (!kvpParts.empty())
    {
      _tagExcludeFilter->addCriterion(
        std::make_shared<NotCriterion>(std::make_shared<TagCriterion>(kvpParts[0], kvpParts[1])));
    }
    else
    {
      LOG_WARN("Invalid exclude tag KVP: " << kvp);
    }
  }
}

void RailwaysCrossingMarker::_createIndex()
{
  LOG_INFO("\tCreating railways index...");

  // Create an index for all rails within the default search radius.

  // No tuning was done, just copied these settings from OsmMapIndex. 10 children - 368 - see #3054
  _index = std::make_shared<Tgs::HilbertRTree>(std::make_shared<Tgs::MemoryPageStore>(728), 2);

  // Only index elements that satisfy isMatchCandidate.
  std::function<bool (ConstElementPtr e)> f =
    std::bind(&RailwaysCrossingMarker::_isMatchCandidate, this, std::placeholders::_1);
  std::shared_ptr<ArbitraryCriterion> pCrit = std::make_shared<ArbitraryCriterion>(f);

  SpatialIndexer v(
    _index, _indexToEid, pCrit,
    std::bind(&RailwaysCrossingMarker::_getSearchRadius, this, std::placeholders::_1), _map);
  _map->visitRo(v);
  v.finalizeIndex();

  LOG_DEBUG(
    "\tRailways feature index created with " << StringUtils::formatLargeNumber(v.getSize()) <<
    " elements.");
}

Meters RailwaysCrossingMarker::_getSearchRadius(const ConstElementPtr& e) const
{
  return e->getCircularError();
}

bool RailwaysCrossingMarker::_isMatchCandidate(ConstElementPtr element) const
{
  return
    RailwayCriterion().isSatisfied(element) &&
    (!_tagExcludeFilter || _tagExcludeFilter->isSatisfied(element));
}

QStringList RailwaysCrossingMarker::getCriteria() const
{
  QStringList criteria;
  criteria.append(RailwayCriterion::className());
  return criteria;
}

}
