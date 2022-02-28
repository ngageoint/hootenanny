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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "PoiPolygonMatchVisitor.h"

// hoot
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/SpatialIndexer.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/MemoryUsageChecker.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

// Standard
#include <functional>
using namespace std;

namespace hoot
{

PoiPolygonMatchVisitor::PoiPolygonMatchVisitor(
  const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& result, ElementCriterionPtr filter) :
_map(map),
_result(result),
_filter(filter)
{
  _timer.start();
}

PoiPolygonMatchVisitor::PoiPolygonMatchVisitor(
  const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& result, ConstMatchThresholdPtr threshold,
  PoiPolygonInfoCachePtr infoCache, ElementCriterionPtr filter) :
_map(map),
_result(result),
_neighborCountMax(-1),
_neighborCountSum(0),
_elementsEvaluated(0),
_threshold(threshold),
_numElementsVisited(0),
_numMatchCandidatesVisited(0),
_filter(filter),
_infoCache(infoCache)
{
  ConfigOptions opts = ConfigOptions();
  _reviewDistanceThreshold = opts.getPoiPolygonAdditionalSearchDistance();
  _taskStatusUpdateInterval = opts.getTaskStatusUpdateInterval();
  _memoryCheckUpdateInterval = opts.getMemoryUsageCheckerInterval();
  LOG_VART(_infoCache.get());
  _timer.start();
}

void PoiPolygonMatchVisitor::_checkForMatch(const std::shared_ptr<Element const>& e,
                                            const std::set<ElementId>& surroundingPolyIds)
{
  LOG_TRACE("Checking for match with POI: " << e->getElementId());
  LOG_VART(surroundingPolyIds.size());

  LOG_TRACE("Attempting to match poly neighbors with POI: " << e->getElementId() << "...");
  const ElementId poiId(e->getElementType(), e->getId());
  _elementsEvaluated++;
  int neighborCount = 0;

  for (std::set<ElementId>::const_iterator it = surroundingPolyIds.begin();
       it != surroundingPolyIds.end(); ++it)
  {
    const ElementId polyId = *it;
    LOG_VART(polyId);
    if (poiId != polyId)
    {
      const std::shared_ptr<Element const>& poly = _map->getElement(polyId);
      LOG_VART(poly.get());
      LOG_VART(poly->getElementId());
      if (poly->isUnknown() && _polyCrit.isSatisfied(poly))
      {
        // score each candidate and push it on the result vector
        LOG_TRACE(
          "Calculating match between: " << poiId << " and " << poly->getElementId() << "...");
        std::shared_ptr<PoiPolygonMatch> m =
          std::make_shared<PoiPolygonMatch>(_map, _threshold, _infoCache, surroundingPolyIds);
        m->setConfiguration(conf());
        m->calculateMatch(poiId, polyId);

        // if we're confident this is a miss
        if (m->getType() != MatchType::Miss)
        {
          _result.push_back(m);
          neighborCount++;
        }
      }
    }
  }

  _neighborCountSum += neighborCount;
  _neighborCountMax = std::max(_neighborCountMax, neighborCount);
}

std::set<ElementId> PoiPolygonMatchVisitor::_collectSurroundingPolyIds(
  const std::shared_ptr<Element const>& e)
{
  LOG_TRACE("Collecting surrounding poly IDs for: " << e->getElementId());

  std::set<ElementId> surroundingPolyIds;

  std::shared_ptr<geos::geom::Envelope> env(e->getEnvelope(_map));
  env->expandBy(_getSearchRadius(e));

  // find other nearby candidates
  LOG_TRACE("Searching for neighbors...");
  const std::set<ElementId> neighbors =
    SpatialIndexer::findNeighbors(*env, _getPolyIndex(), _polyIndexToEid, _getMap());
  LOG_VART(neighbors.size());

  LOG_TRACE("Processing neighbors...");
  ElementId from(e->getElementType(), e->getId());
  for (std::set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
  {
    ElementId neighboringElementId = *it;
    if (from != neighboringElementId)
    {
      const std::shared_ptr<Element const>& poly = _map->getElement(neighboringElementId);
      LOG_VART(neighboringElementId);
      LOG_VART(poly->isUnknown());
      if (poly->isUnknown())
      {
        LOG_TRACE("Found neighboring poly: " << neighboringElementId);
        surroundingPolyIds.insert(neighboringElementId);
      }
    }
  }

  LOG_VART(surroundingPolyIds.size());
  return surroundingPolyIds;
}

Meters PoiPolygonMatchVisitor::_getSearchRadius(const std::shared_ptr<Element const>& e) const
{
  const Meters searchRadius = e->getCircularError() + _reviewDistanceThreshold;
  //LOG_VART(searchRadius);
  return searchRadius;
}

void PoiPolygonMatchVisitor::visit(const ConstElementPtr& e)
{
  // See if the element is a POI as defined by poi/poly conflation.
  if (isMatchCandidate(e))
  {
    // Now, let's try to match all polys in the search radius with this POI (both as defined by
    // poi/poly conflation).

    // If we are doing advanced matching or review reduction, let's collect all polys that surround
    // the POI and also all POIs that surround it. If you're doing performance analysis of
    // poi/poly getting the neighbor polys is definitely a bottleneck, but at this point not sure
    // anything can be done about it other than running with a smaller search radius.

    _checkForMatch(e, _collectSurroundingPolyIds(e));

    _numMatchCandidatesVisited++;
    if (_numMatchCandidatesVisited % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_DEBUG(
        "\tProcessed " << StringUtils::formatLargeNumber(_numMatchCandidatesVisited) <<
        " match candidates / " << StringUtils::formatLargeNumber(_map->getNodeCount()) <<
        " nodes.");
    }
  }

  // poi/poly matching can be a little slow at times compared to the other types of conflation, so
  // throttle the log update interval accordingly.
  if (_timer.elapsed() > 3000 && _taskStatusUpdateInterval >= 10)
  {
    _taskStatusUpdateInterval /= 10;
  }
  else if (_timer.elapsed() < 250 && _taskStatusUpdateInterval < 10000)
  {
    _taskStatusUpdateInterval *= 10;
  }

  _numElementsVisited++;
  if (_numElementsVisited % _taskStatusUpdateInterval == 0)
  {
    PROGRESS_STATUS(
      "\tProcessed " << StringUtils::formatLargeNumber(_numElementsVisited) << " of " <<
      StringUtils::formatLargeNumber(_map->getNodeCount()) << " nodes.");
    _timer.restart();
  }
  if (_numElementsVisited % _memoryCheckUpdateInterval == 0)
  {
    MemoryUsageChecker::getInstance().check();
  }
}

bool PoiPolygonMatchVisitor::isMatchCandidate(ConstElementPtr element) const
{
  if (_filter && !_filter->isSatisfied(element))
  {
    return false;
  }

  // Its simpler logic to just examine each POI and check for surrounding polys, rather than check
  // both POIs and their surrounding polys and polys and their surrounding POIs; note that this is
  // different than PoiPolygonMatchCreator::isMatchCandidate, which is looking at both to appease
  // the stats.
  return element->isUnknown() && PoiPolygonPoiCriterion().isSatisfied(element);
}

std::shared_ptr<Tgs::HilbertRTree>& PoiPolygonMatchVisitor::_getPolyIndex()
{
  if (!_polyIndex)
  {
    LOG_INFO("Creating POI/Polygon feature index...");

    // tune this? - see #3054
    _polyIndex =
      std::make_shared<Tgs::HilbertRTree>(std::make_shared<Tgs::MemoryPageStore>(728), 2);

    std::shared_ptr<PoiPolygonPolyCriterion> crit = std::make_shared<PoiPolygonPolyCriterion>();

    SpatialIndexer v(_polyIndex,
                     _polyIndexToEid,
                     crit,
                     std::bind(&PoiPolygonMatchVisitor::_getSearchRadius, this, placeholders::_1),
                     _getMap());
    _getMap()->visitWaysRo(v);
    _getMap()->visitRelationsRo(v);
    v.finalizeIndex();

    LOG_INFO(
      "POI/Polygon feature index created with " << StringUtils::formatLargeNumber(v.getSize()) <<
      " elements.");
  }
  return _polyIndex;
}

}
