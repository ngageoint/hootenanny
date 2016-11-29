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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONMATCH_H
#define POIPOLYGONMATCH_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/conflate/Match.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/MatchDetails.h>
#include <hoot/core/util/Configurable.h>

#include "PoiPolygonRfClassifier.h"

namespace hoot
{

/**
 * This is a additive, rule based mechanism for matching POIs to polygons. See "POI to
 * Polygon Conflation" in the Hootenanny Algorithms document for more details.
 */
class PoiPolygonMatch : public Match, public MatchDetails
{

public:

  static const unsigned int MATCH_EVIDENCE_THRESHOLD;
  static const unsigned int REVIEW_EVIDENCE_THRESHOLD;

  PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
    ConstMatchThresholdPtr threshold, shared_ptr<const PoiPolygonRfClassifier> rf);

  PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
    ConstMatchThresholdPtr threshold, shared_ptr<const PoiPolygonRfClassifier> rf,
    const set<ElementId>& polyNeighborIds, const set<ElementId>& poiNeighborIds);

  PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
    ConstMatchThresholdPtr threshold, shared_ptr<const PoiPolygonRfClassifier> rf,
    double matchDistance, double reviewDistance, double nameScoreThreshold,
    double typeScoreThreshold, double addressScoreThreshold);

  virtual const MatchClassification& getClassification() const { return _class; }

  virtual MatchMembers getMatchMembers() const { return MatchMembers::Poi | MatchMembers::Polygon; }

  virtual QString getMatchName() const { return _matchName; }

  virtual set< pair<ElementId, ElementId> > getMatchPairs() const;

  virtual double getProbability() const { return _class.getMatchP(); }

  virtual bool isConflicting(const Match& /*other*/, const ConstOsmMapPtr& /*map*/) const
  { return false; }

  virtual bool isWholeGroup() const { return true; }

  virtual QString toString() const;

  virtual map<QString, double> getFeatures(const shared_ptr<const OsmMap>& m) const;

  /**
   * Determines criteria for a feature to be considered a polygon for matching by
   * PoiPolygonMatch
   *
   * @param element to be evaluated
   * @return true if the element meets the criteria; false otherwise
   */
  static bool isPoly(const Element& e);

  /**
   * @Determines criteria for a feature to be considered a POI for matching by
   * PoiPolygonMatch
   *
   * @param element to be evaluated
   * @return true if the element meets the criteria; false otherwise
   */
  static bool isPoi(const Element& e);

  /**
   * Determines criteria for a feature to be considered an area by
   * PoiPolygonMatch
   *
   * @param element to be evaluated
   * @return true if the element meets the criteria; false otherwise
   */
  static bool isArea(const Element& e);

  /**
   * Pass through to the same method in PoiPolygonDistanceTruthRecorder
   */
  static void printMatchDistanceInfo();

  /**
   * Pass through to the same method in PoiPolygonDistanceTruthRecorder
   */
  static void resetMatchDistanceInfo();

  double getDistance() const { return _distance; }
  double getMatchDistanceThreshold() const { return _matchDistanceThreshold; }
  double getReviewDistanceThreshold() const { return _reviewDistanceThreshold; }
  bool getCloseMatch() const { return _closeMatch; }
  double getTypeScore() const { return _typeScore; }
  double getTypeScoreThreshold() const { return _typeScoreThreshold; }
  double getNameScore() const { return _nameScore; }
  double getNameScoreThreshold() const { return _nameScoreThreshold; }
  double getAddressScore() const { return _addressScore; }

private:

  static QString _matchName;

  ConstOsmMapPtr _map;

  ElementId _eid1;
  ElementId _eid2;
  ConstElementPtr _poi;
  ConstElementPtr _poly;
  shared_ptr<Geometry> _poiGeom;
  shared_ptr<Geometry> _polyGeom;
  bool _e1IsPoi;

  //measured distance between the two elements
  double _distance;
  //max distance allowed between the elements where they can be considered a distance match
  double _matchDistanceThreshold;
  //max distance allowed between the elements where they can be considered for review
  double _reviewDistanceThreshold;
  //true if the distance between the elements, given CE, is within the review distance; absolute
  //requirement for matching
  bool _closeMatch;

  double _typeScore;
  double _typeScoreThreshold;

  double _nameScore;
  double _nameScoreThreshold;

  double _addressScore;
  double _addressScoreThreshold;

  //These are only used by PoiPolygonCustomRules and PoiPolygonDistance
  set<ElementId> _polyNeighborIds;
  set<ElementId> _poiNeighborIds;

  MatchClassification _class;

  shared_ptr<const PoiPolygonRfClassifier> _rf;

  void _calculateMatch(const ElementId& eid1, const ElementId& eid2);
  void _calculateMatchWeka(const ElementId& eid1, const ElementId& eid2);

  void _categorizeElementsByGeometryType(const ElementId& eid1, const ElementId& eid2);

  unsigned int _calculateEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getDistanceEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getConvexPolyDistanceEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getTypeEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getNameEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getAddressEvidence(ConstElementPtr poi, ConstElementPtr poly);

};

}

#endif // POIPOLYGONMATCH_H
