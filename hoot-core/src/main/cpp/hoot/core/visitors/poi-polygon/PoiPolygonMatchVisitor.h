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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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
#include <hoot/core/conflate/poi-polygon/PoiPolygonInfoCache.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

/**
 * Searches maps for any poi/polygon match potentials
 */
class PoiPolygonMatchVisitor : public ConstElementVisitor
{

public:

  PoiPolygonMatchVisitor(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& result,
                         ElementCriterionPtr filter = ElementCriterionPtr());
  PoiPolygonMatchVisitor(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& result,
                         ConstMatchThresholdPtr threshold,
                         std::shared_ptr<PoiPolygonRfClassifier> rf, PoiPolygonInfoCachePtr infoCache,
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

  virtual std::string getClassName() const { return className(); }

private:

  // white box testing
  friend class PoiPolygonMatchVisitorTest;

  const ConstOsmMapPtr& _map;
  std::vector<ConstMatchPtr>& _result;
  std::set<ElementId> _empty;
  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  size_t _maxGroupSize;
  ConstMatchThresholdPtr _threshold;

  // used for finding surrounding polys
  std::shared_ptr<Tgs::HilbertRTree> _polyIndex;
  std::deque<ElementId> _polyIndexToEid;

  std::shared_ptr<PoiPolygonRfClassifier> _rf;

  double _reviewDistanceThreshold;

  long _numElementsVisited;
  long _numMatchCandidatesVisited;
  int _taskStatusUpdateInterval;

  PoiPolygonPoiCriterion _poiCrit;
  PoiPolygonPolyCriterion _polyCrit;
  ElementCriterionPtr _filter;

  PoiPolygonInfoCachePtr _infoCache;

  QElapsedTimer _timer;

  void _checkForMatch(const std::shared_ptr<const Element>& e,
                      const std::set<ElementId>& surroundingPolyIds);
  std::set<ElementId> _collectSurroundingPolyIds(const std::shared_ptr<const Element>& e);
  Meters _getSearchRadius(const std::shared_ptr<const Element>& e) const;

  std::shared_ptr<Tgs::HilbertRTree>& _getPolyIndex();

  ConstOsmMapPtr _getMap() { return _map; }
};

}

#endif // POIPOLYGONMATCHVISITOR_H
