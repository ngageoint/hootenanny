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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONMATCHVISITOR_H
#define POIPOLYGONMATCHVISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/OsmMap.h>
#include "../PoiPolygonRfClassifier.h"

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
                         ConstMatchThresholdPtr threshold,
                         boost::shared_ptr<PoiPolygonRfClassifier> rf);
  ~PoiPolygonMatchVisitor();

  /**
   * Determines whether an element is a Poi/Polygon conflation match candidate and, if so,
   * attempts to match it with another feature
   *
   * @param e element to examine
   */
  virtual void visit(const ConstElementPtr& e);

  virtual QString getDescription() const { return ""; }

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

  boost::shared_ptr<Tgs::HilbertRTree> _polyIndex; // used for finding surrounding polys
  std::deque<ElementId> _polyIndexToEid;
  std::set<ElementId> _surroundingPolyIds;
  boost::shared_ptr<Tgs::HilbertRTree> _poiIndex; // used for finding surrounding poi's
  std::deque<ElementId> _poiIndexToEid;
  std::set<ElementId> _surroundingPoiIds;

  boost::shared_ptr<PoiPolygonRfClassifier> _rf;

  bool _enableAdvancedMatching;
  bool _enableReviewReduction;

  double _reviewDistanceThreshold;

  void _checkForMatch(const boost::shared_ptr<const Element>& e);
  void _collectSurroundingPolyIds(const boost::shared_ptr<const Element>& e);
  void _collectSurroundingPoiIds(const boost::shared_ptr<const Element>& e);
  Meters _getSearchRadius(const boost::shared_ptr<const Element>& e) const;

  static bool _isMatchCandidate(ConstElementPtr element);

  boost::shared_ptr<Tgs::HilbertRTree>& _getPolyIndex();
  boost::shared_ptr<Tgs::HilbertRTree>& _getPoiIndex();

  ConstOsmMapPtr _getMap() { return _map; }
};

}

#endif // POIPOLYGONMATCHVISITOR_H
