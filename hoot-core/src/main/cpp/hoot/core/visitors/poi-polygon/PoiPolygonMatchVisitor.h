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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONMATCHVISITOR_H
#define POIPOLYGONMATCHVISITOR_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonRfClassifier.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonCache.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>

namespace hoot
{

/**
 * Searches maps for any poi/polygon match potentials
 */
class PoiPolygonMatchVisitor : public ConstElementVisitor
{

public:

  PoiPolygonMatchVisitor(const ConstOsmMapPtr& map, std::vector<const Match*>& result,
                         ElementCriterionPtr filter = ElementCriterionPtr());
  PoiPolygonMatchVisitor(const ConstOsmMapPtr& map, std::vector<const Match*>& result,
                         ConstMatchThresholdPtr threshold,
                         std::shared_ptr<PoiPolygonRfClassifier> rf, PoiPolygonCachePtr infoCache,
                         ElementCriterionPtr filter = ElementCriterionPtr());
  ~PoiPolygonMatchVisitor();

  /**
   * Determines whether an element is a Poi/Polygon conflation match candidate and, if so,
   * attempts to match it with another feature
   *
   * @param e element to examine
   */
  virtual void visit(const ConstElementPtr& e) override;

  bool isMatchCandidate(ConstElementPtr element);

  virtual QString getDescription() const override { return ""; }

  long getNumMatchCandidatesFound() const { return _numMatchCandidatesVisited; }

private:

  // white box testing
  friend class PoiPolygonMatchVisitorTest;

  const ConstOsmMapPtr& _map;
  std::vector<const Match*>& _result;
  std::set<ElementId> _empty;
  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  size_t _maxGroupSize;
  ConstMatchThresholdPtr _threshold;

  std::shared_ptr<Tgs::HilbertRTree> _polyIndex; // used for finding surrounding polys
  std::deque<ElementId> _polyIndexToEid;
  std::set<ElementId> _surroundingPolyIds;
  std::shared_ptr<Tgs::HilbertRTree> _poiIndex; // used for finding surrounding poi's
  std::deque<ElementId> _poiIndexToEid;
  std::set<ElementId> _surroundingPoiIds;

  std::shared_ptr<PoiPolygonRfClassifier> _rf;

  bool _enableAdvancedMatching;
  bool _enableReviewReduction;

  double _reviewDistanceThreshold;

  long _numElementsVisited;
  long _numMatchCandidatesVisited;
  int _taskStatusUpdateInterval;

  PoiPolygonPoiCriterion _poiCrit;
  PoiPolygonPolyCriterion _polyCrit;
  ElementCriterionPtr _filter;

  PoiPolygonCachePtr _infoCache;

  void _checkForMatch(const std::shared_ptr<const Element>& e);
  void _collectSurroundingPolyIds(const std::shared_ptr<const Element>& e);
  void _collectSurroundingPoiIds(const std::shared_ptr<const Element>& e);
  Meters _getSearchRadius(const std::shared_ptr<const Element>& e) const;

  std::shared_ptr<Tgs::HilbertRTree>& _getPolyIndex();
  std::shared_ptr<Tgs::HilbertRTree>& _getPoiIndex();

  ConstOsmMapPtr _getMap() { return _map; }
};

}

#endif // POIPOLYGONMATCHVISITOR_H
