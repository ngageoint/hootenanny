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
 * @copyright Copyright (C) 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "PoiPolygonMatchVisitor.h"

// hoot
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/IndexElementsVisitor.h>
#include <hoot/core/util/StringUtils.h>

// Boost
#include <boost/bind.hpp>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

namespace hoot
{

PoiPolygonMatchVisitor::PoiPolygonMatchVisitor(const ConstOsmMapPtr& map,
                                               std::vector<const Match*>& result,
                                               ElementCriterionPtr filter) :
_map(map),
_result(result),
_filter(filter)
{
}

PoiPolygonMatchVisitor::PoiPolygonMatchVisitor(const ConstOsmMapPtr& map,
                                               std::vector<const Match*>& result,
                                               ConstMatchThresholdPtr threshold,
                                               boost::shared_ptr<PoiPolygonRfClassifier> rf,
                                               ElementCriterionPtr filter) :
_map(map),
_result(result),
_neighborCountMax(-1),
_neighborCountSum(0),
_elementsEvaluated(0),
_threshold(threshold),
_rf(rf),
_numElementsVisited(0),
_numMatchCandidatesVisited(0),
_filter(filter)
{
  ConfigOptions opts = ConfigOptions();
  _enableAdvancedMatching = opts.getPoiPolygonEnableAdvancedMatching();
  _enableReviewReduction = opts.getPoiPolygonEnableReviewReduction();
  _reviewDistanceThreshold = opts.getPoiPolygonReviewDistanceThreshold();
  _taskStatusUpdateInterval = opts.getTaskStatusUpdateInterval();
}

PoiPolygonMatchVisitor::~PoiPolygonMatchVisitor()
{
}

void PoiPolygonMatchVisitor::_checkForMatch(const boost::shared_ptr<const Element>& e)
{
  boost::shared_ptr<geos::geom::Envelope> env(e->getEnvelope(_map));
  env->expandBy(_getSearchRadius(e));

  // find other nearby candidates
  std::set<ElementId> neighbors = IndexElementsVisitor::findNeighbors(*env,
                                                                      _getPolyIndex(),
                                                                      _polyIndexToEid,
                                                                      _getMap());
  ElementId from(e->getElementType(), e->getId());

  _elementsEvaluated++;
  int neighborCount = 0;

  for (std::set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
  {
    if (from != *it)
    {
      const boost::shared_ptr<const Element>& n = _map->getElement(*it);

      if (n->isUnknown() && _polyCrit.isSatisfied(n))
      {
        // score each candidate and push it on the result vector
        PoiPolygonMatch* m =
          new PoiPolygonMatch(_map, _threshold, _rf, _surroundingPolyIds, _surroundingPoiIds);
        m->setConfiguration(conf());
        m->calculateMatch(from, *it);

        // if we're confident this is a miss
        if (m->getType() == MatchType::Miss)
        {
          delete m;
        }
        else
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

void PoiPolygonMatchVisitor::_collectSurroundingPolyIds(const boost::shared_ptr<const Element>& e)
{
  _surroundingPolyIds.clear();
  boost::shared_ptr<geos::geom::Envelope> env(e->getEnvelope(_map));
  env->expandBy(_getSearchRadius(e));

  // find other nearby candidates
  std::set<ElementId> neighbors = IndexElementsVisitor::findNeighbors(*env,
                                                                      _getPolyIndex(),
                                                                      _polyIndexToEid,
                                                                      _getMap());
  ElementId from(e->getElementType(), e->getId());

  for (std::set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
  {
    if (from != *it)
    {
      const boost::shared_ptr<const Element>& n = _map->getElement(*it);

      // TODO: Aren't we already filtering by poly when we create the index?  Check this.  Also,
      // maybe could make the unknown part of the criteria to being with.
      if (n->isUnknown() && _polyCrit.isSatisfied(n))
      {
        _surroundingPolyIds.insert(*it);
      }
    }
  }
}

void PoiPolygonMatchVisitor::_collectSurroundingPoiIds(const boost::shared_ptr<const Element>& e)
{
  _surroundingPoiIds.clear();
  boost::shared_ptr<geos::geom::Envelope> env(e->getEnvelope(_map));
  env->expandBy(_getSearchRadius(e));

  // find other nearby candidates
  std::set<ElementId> neighbors = IndexElementsVisitor::findNeighbors(*env,
                                                                      _getPoiIndex(),
                                                                      _poiIndexToEid,
                                                                      _getMap());
  ElementId from(e->getElementType(), e->getId());

  for (std::set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
  {
    if (from != *it)
    {
      const boost::shared_ptr<const Element>& n = _map->getElement(*it);

      // TODO: Aren't we already filtering by poi when we create the index?  Check this.  Also,
      // maybe could make the unknown part of the criteria to begin with.
      if (n->isUnknown() && _poiCrit.isSatisfied(n))
      {
        _surroundingPoiIds.insert(*it);
      }
    }
  }
}

Meters PoiPolygonMatchVisitor::_getSearchRadius(const boost::shared_ptr<const Element>& e) const
{
  const Meters searchRadius = e->getCircularError() + _reviewDistanceThreshold;
  LOG_VART(searchRadius);
  return searchRadius;
}

void PoiPolygonMatchVisitor::visit(const ConstElementPtr& e)
{
  if (isMatchCandidate(e))
  {
    // Technically, the density based matches depends on this data too, but since that code has
    // been disabled, this check is good enough.
    if (_enableAdvancedMatching || _enableReviewReduction)
    {
      _collectSurroundingPolyIds(e);
      _collectSurroundingPoiIds(e);
    }
    _checkForMatch(e);

    _numMatchCandidatesVisited++;
    if (_numMatchCandidatesVisited % (_taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_DEBUG(
        "Processed " << StringUtils::formatLargeNumber(_numMatchCandidatesVisited) <<
        " match candidates / " << StringUtils::formatLargeNumber(_map->getElementCount()) <<
        " total elements.");
    }
  }

  _numElementsVisited++;
  if (_numElementsVisited % (_taskStatusUpdateInterval * 10) == 0)
  {
    PROGRESS_INFO(
      "Processed " << StringUtils::formatLargeNumber(_numElementsVisited) << " / " <<
      StringUtils::formatLargeNumber(_map->getElementCount()) << " elements.");
  }
}

bool PoiPolygonMatchVisitor::isMatchCandidate(ConstElementPtr element)
{
  if (_filter && !_filter->isSatisfied(element))
  {
    return false;
  }

  //simpler logic to just examine each POI and check for surrounding polys, rather than check both
  //POIs and their surrounding polys and polys and their surrounding POIs; note that this is
  //different than PoiPolygonMatchCreator::isMatchCandidate, which is looking at both to appease
  //the stats
  return element->isUnknown() && PoiPolygonPoiCriterion().isSatisfied(element);
}

boost::shared_ptr<Tgs::HilbertRTree>& PoiPolygonMatchVisitor::_getPolyIndex()
{
  if (!_polyIndex)
  {
    // TODO: tune this? - see #3054
    boost::shared_ptr<Tgs::MemoryPageStore> mps(new Tgs::MemoryPageStore(728));
    _polyIndex.reset(new Tgs::HilbertRTree(mps, 2));

    boost::shared_ptr<PoiPolygonPolyCriterion> crit(new PoiPolygonPolyCriterion());

    IndexElementsVisitor v(_polyIndex,
                           _polyIndexToEid,
                           crit,
                           boost::bind(&PoiPolygonMatchVisitor::_getSearchRadius, this, _1),
                           _getMap());

    _getMap()->visitWaysRo(v);
    _getMap()->visitRelationsRo(v);
    v.finalizeIndex();
  }
  return _polyIndex;
}

boost::shared_ptr<Tgs::HilbertRTree>& PoiPolygonMatchVisitor::_getPoiIndex()
{
  if (!_poiIndex)
  {
    // TODO: tune this? - see #3054
    boost::shared_ptr<Tgs::MemoryPageStore> mps(new Tgs::MemoryPageStore(728));
    _poiIndex.reset(new Tgs::HilbertRTree(mps, 2));

    boost::shared_ptr<PoiPolygonPoiCriterion> crit(new PoiPolygonPoiCriterion());

    IndexElementsVisitor v(_poiIndex,
                           _poiIndexToEid,
                           crit,
                           boost::bind(&PoiPolygonMatchVisitor::_getSearchRadius, this, _1),
                           _getMap());

    _getMap()->visitNodesRo(v);
    v.finalizeIndex();
  }
  return _poiIndex;
}

}
