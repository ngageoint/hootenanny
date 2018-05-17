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
 * @copyright Copyright (C) 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonMatchVisitor.h"

// hoot
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/IndexElementsVisitor.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/OsmSchema.h>

#include "../PoiPolygonMatch.h"
#include "../filters/PoiPolygonPolyCriterion.h"
#include "../filters/PoiPolygonPoiCriterion.h"
#include "../PoiPolygonTagIgnoreListReader.h"

// Boost
#include <boost/bind.hpp>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>


namespace hoot
{

 PoiPolygonMatchVisitor::PoiPolygonMatchVisitor(const ConstOsmMapPtr& map,
                                                std::vector<const Match*>& result,
                                                ConstMatchThresholdPtr threshold,
                                                boost::shared_ptr<PoiPolygonRfClassifier> rf) :
_map(map),
_result(result),
_threshold(threshold),
_rf(rf)
{
  _neighborCountMax = -1;
  _neighborCountSum = 0;
  _elementsEvaluated = 0;

  ConfigOptions opts = ConfigOptions();
  _enableAdvancedMatching = opts.getPoiPolygonEnableAdvancedMatching();
  _enableReviewReduction = opts.getPoiPolygonEnableReviewReduction();
  _reviewDistanceThreshold = opts.getPoiPolygonReviewDistanceThreshold();
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

      if (n->isUnknown() &&
          OsmSchema::getInstance().isPoiPolygonPoly(
            n, PoiPolygonTagIgnoreListReader::getInstance().getPolyTagIgnoreList()))
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

      if (n->isUnknown() &&
          OsmSchema::getInstance().isPoiPolygonPoly(
            n, PoiPolygonTagIgnoreListReader::getInstance().getPolyTagIgnoreList()))
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

      if (n->isUnknown() &&
          OsmSchema::getInstance().isPoiPolygonPoi(
            n, PoiPolygonTagIgnoreListReader::getInstance().getPoiTagIgnoreList()))
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
  if (_isMatchCandidate(e))
  {
    //Technically, the density based density matches depends on this data too, but since that
    //code has been disabled, this check is good enough.
    if (_enableAdvancedMatching || _enableReviewReduction)
    {
      _collectSurroundingPolyIds(e);
      _collectSurroundingPoiIds(e);
    }
    _checkForMatch(e);
  }
}

bool PoiPolygonMatchVisitor::_isMatchCandidate(ConstElementPtr element)
{
  //simpler logic to just examine each POI and check for surrounding polys, rather than check both
  //POIs and their surrounding polys and polys and their surrounding POIs; note that this is
  //different than PoiPolygonMatchCreator::isMatchCandidate, which is looking at both to appease
  //the stats
  return element->isUnknown() &&
         OsmSchema::getInstance().isPoiPolygonPoi(
         element, PoiPolygonTagIgnoreListReader::getInstance().getPoiTagIgnoreList());
}

boost::shared_ptr<Tgs::HilbertRTree>& PoiPolygonMatchVisitor::_getPolyIndex()
{
  if (!_polyIndex)
  {
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
