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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonMatchCreator.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/filters/ArbitraryCriterion.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/NotImplementedException.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/IndexElementsVisitor.h>

#include "PoiPolygonMatch.h"
#include "filters/PoiPolygonPolyCriterion.h"
#include "filters/PoiPolygonPoiCriterion.h"

// Standard
#include <fstream>

// Boost
#include <boost/bind.hpp>

// tgs
#include <tgs/RandomForest/RandomForest.h>
#include <tgs/RStarTree/IntersectionIterator.h>
#include <tgs/RStarTree/MemoryPageStore.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, PoiPolygonMatchCreator)

using namespace Tgs;

/**
 * Searches the specified map for any poi/polygon match potentials
 */
class PoiPolygonMatchVisitor : public ElementVisitor
{

public:

  PoiPolygonMatchVisitor(const ConstOsmMapPtr& map, vector<const Match*>& result,
                         ConstMatchThresholdPtr threshold, shared_ptr<PoiPolygonRfClassifier> rf) :
    _map(map),
    _result(result),
    _threshold(threshold),
    _rf(rf)
  {
    _neighborCountMax = -1;
    _neighborCountSum = 0;
    _elementsEvaluated = 0;
  }

  ~PoiPolygonMatchVisitor()
  {
  }

  void checkForMatch(const shared_ptr<const Element>& e)
  {
    auto_ptr<Envelope> env(e->getEnvelope(_map));
    env->expandBy(getSearchRadius(e));

    // find other nearby candidates
    set<ElementId> neighbors = IndexElementsVisitor::findNeighbors(*env,
                                                                   getIndex(),
                                                                   _indexToEid,
                                                                   getMap());
    ElementId from(e->getElementType(), e->getId());

    _elementsEvaluated++;
    int neighborCount = 0;

    for (set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
    {
      if (from != *it)
      {
        const shared_ptr<const Element>& n = _map->getElement(*it);

        if (n->isUnknown() && PoiPolygonMatch::isPoly(*n))
        {
          // score each candidate and push it on the result vector
          PoiPolygonMatch* m =
            new PoiPolygonMatch(
              _map, from, *it, _threshold, _rf, _surroundingPolyIds, _surroundingPoiIds);

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

  void collectSurroundingPolyIds(const shared_ptr<const Element>& e)
  {
    _surroundingPolyIds.clear();
    auto_ptr<Envelope> env(e->getEnvelope(_map));
    env->expandBy(getSearchRadius(e));

    // find other nearby candidates
    set<ElementId> neighbors = IndexElementsVisitor::findNeighbors(*env,
                                                                   getPolyIndex(),
                                                                   _polyIndexToEid,
                                                                   getMap());
    ElementId from(e->getElementType(), e->getId());

    for (set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
    {
      if (from != *it)
      {
        const shared_ptr<const Element>& n = _map->getElement(*it);

        if (n->isUnknown() && PoiPolygonMatch::isPoly(*n))
        {
          _surroundingPolyIds.insert(*it);
        }
      }
    }
  }

  void collectSurroundingPoiIds(const shared_ptr<const Element>& e)
  {
    _surroundingPoiIds.clear();
    auto_ptr<Envelope> env(e->getEnvelope(_map));
    env->expandBy(getSearchRadius(e));

    // find other nearby candidates
    set<ElementId> neighbors = IndexElementsVisitor::findNeighbors(*env,
                                                                   getPoiIndex(),
                                                                   _poiIndexToEid,
                                                                   getMap());
    ElementId from(e->getElementType(), e->getId());

    for (set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
    {
      if (from != *it)
      {
        const shared_ptr<const Element>& n = _map->getElement(*it);

        if (n->isUnknown() && PoiPolygonMatch::isPoi(*n))
        {
          _surroundingPoiIds.insert(*it);
        }
      }
    }
  }

  Meters getSearchRadius(const shared_ptr<const Element>& e) const
  {
    return e->getCircularError() + ConfigOptions().getPoiPolygonReviewDistanceThreshold();
  }

  virtual void visit(const ConstElementPtr& e)
  {
    if (isMatchCandidate(e))
    {
      //Technically, the density based density matches depends on this data too, but since that
      //code has been disabled, this check is good enough.
      if (ConfigOptions().getPoiPolygonEnableAdvancedMatching() ||
          ConfigOptions().getPoiPolygonEnableReviewReduction())
      {
        collectSurroundingPolyIds(e);
        collectSurroundingPoiIds(e);
      }
      checkForMatch(e);
    }
  }

  static bool isMatchCandidate(ConstElementPtr element)
  {
    return element->isUnknown() && PoiPolygonMatch::isPoi(*element);
  }

  shared_ptr<HilbertRTree>& getIndex()
  {
    if (!_index)
    {
      // No tuning was done, I just copied these settings from OsmMapIndex.
      // 10 children - 368
      shared_ptr<MemoryPageStore> mps(new MemoryPageStore(728));
      _index.reset(new HilbertRTree(mps, 2));

      shared_ptr<PoiPolygonPolyCriterion> crit(new PoiPolygonPolyCriterion());

      // Instantiate our visitor
      IndexElementsVisitor v(_index,
                             _indexToEid,
                             crit,
                             boost::bind(&PoiPolygonMatchVisitor::getSearchRadius, this, _1),
                             getMap());

      getMap()->visitWaysRo(v);
      getMap()->visitRelationsRo(v);
      v.finalizeIndex();
    }

    return _index;
  }

  shared_ptr<HilbertRTree>& getPolyIndex()
  {
    if (!_polyIndex)
    {
      shared_ptr<MemoryPageStore> mps(new MemoryPageStore(728));
      _polyIndex.reset(new HilbertRTree(mps, 2));

      shared_ptr<PoiPolygonPolyCriterion> crit(new PoiPolygonPolyCriterion());

      IndexElementsVisitor v(_polyIndex,
                             _polyIndexToEid,
                             crit,
                             boost::bind(&PoiPolygonMatchVisitor::getSearchRadius, this, _1),
                             getMap());

      getMap()->visitWaysRo(v);
      getMap()->visitRelationsRo(v);
      v.finalizeIndex();
    }

    return _polyIndex;
  }

  shared_ptr<HilbertRTree>& getPoiIndex()
  {
    if (!_poiIndex)
    {
      shared_ptr<MemoryPageStore> mps(new MemoryPageStore(728));
      _poiIndex.reset(new HilbertRTree(mps, 2));

      shared_ptr<PoiPolygonPoiCriterion> crit(new PoiPolygonPoiCriterion());

      IndexElementsVisitor v(_poiIndex,
                             _poiIndexToEid,
                             crit,
                             boost::bind(&PoiPolygonMatchVisitor::getSearchRadius, this, _1),
                             getMap());

      getMap()->visitNodesRo(v);
      v.finalizeIndex();
    }

    return _poiIndex;
  }

  ConstOsmMapPtr getMap() { return _map; }

private:

  const ConstOsmMapPtr& _map;
  vector<const Match*>& _result;
  set<ElementId> _empty;
  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  size_t _maxGroupSize;
  ConstMatchThresholdPtr _threshold;

  shared_ptr<HilbertRTree> _index; // Used for finding neighbors
  deque<ElementId> _indexToEid;
  shared_ptr<HilbertRTree> _polyIndex; // used for finding surrounding polys
  deque<ElementId> _polyIndexToEid;
  set<ElementId> _surroundingPolyIds;
  shared_ptr<HilbertRTree> _poiIndex; // used for finding surrounding poi's
  deque<ElementId> _poiIndexToEid;
  set<ElementId> _surroundingPoiIds;

  shared_ptr<PoiPolygonRfClassifier> _rf;
};

PoiPolygonMatchCreator::PoiPolygonMatchCreator()
{
}

Match* PoiPolygonMatchCreator::createMatch(const ConstOsmMapPtr& map, ElementId eid1,
                                           ElementId eid2)
{
  Match* result = 0;

  if (eid1.getType() != eid2.getType())
  {
    ConstElementPtr e1 = map->getElement(eid1);
    ConstElementPtr e2 = map->getElement(eid2);
    bool foundPoi = PoiPolygonMatch::isPoi(*e1) || PoiPolygonMatch::isPoi(*e2);
    bool foundPoly = PoiPolygonMatch::isPoly(*e1) || PoiPolygonMatch::isPoly(*e2);

    if (foundPoi && foundPoly)
    {
      result = new PoiPolygonMatch(map, eid1, eid2, getMatchThreshold(), _getRf());
    }
  }

  return result;
}

void PoiPolygonMatchCreator::createMatches(const ConstOsmMapPtr& map, vector<const Match*>& matches,
                                           ConstMatchThresholdPtr threshold)
{
  PoiPolygonMatch::resetMatchDistanceInfo();

  PoiPolygonMatchVisitor v(map, matches, threshold, _getRf());
  map->visitRo(v);

  if (ConfigOptions().getPoiPolygonPrintMatchDistanceTruth())
  {
    PoiPolygonMatch::printMatchDistanceInfo();
  }
}

vector<MatchCreator::Description> PoiPolygonMatchCreator::getAllCreators() const
{
  vector<Description> result;

  result.push_back(
    Description(className(), "POI to Polygon Match Creator", MatchCreator::POI, true));

  return result;
}

bool PoiPolygonMatchCreator::isMatchCandidate(ConstElementPtr element,
                                              const ConstOsmMapPtr& /*map*/)
{
  return PoiPolygonMatchVisitor::isMatchCandidate(element);
}

shared_ptr<MatchThreshold> PoiPolygonMatchCreator::getMatchThreshold()
{
  if (!_matchThreshold.get())
  {
    ConfigOptions config;
    _matchThreshold.reset(
      new MatchThreshold(config.getPoiPolygonMatchThreshold(), config.getPoiPolygonMissThreshold(),
                         config.getPoiPolygonReviewThreshold()));
  }
  return _matchThreshold;
}

shared_ptr<PoiPolygonRfClassifier> PoiPolygonMatchCreator::_getRf()
{
  if (!_rf)
  {
    _rf.reset(new PoiPolygonRfClassifier());
  }

  return _rf;
}

}
