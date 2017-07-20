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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONMATCH_H
#define POIPOLYGONMATCH_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/conflate/Match.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/MatchDetails.h>
#include <hoot/core/conflate/MatchClassification.h>
#include <hoot/core/util/Configurable.h>

#include "PoiPolygonRfClassifier.h"

namespace hoot
{

/**
 * This is a additive, rule based mechanism for matching POIs to polygons. See "POI to
 * Polygon Conflation" in the Hootenanny Algorithms document for more details.
 */
class PoiPolygonMatch : public Match, public MatchDetails, public Configurable
{

public:

  static const unsigned int MATCH_EVIDENCE_THRESHOLD;
  static const unsigned int REVIEW_EVIDENCE_THRESHOLD;

  PoiPolygonMatch(const ConstOsmMapPtr& map, ConstMatchThresholdPtr threshold,
    boost::shared_ptr<const PoiPolygonRfClassifier> rf,
    const std::set<ElementId>& polyNeighborIds = std::set<ElementId>(),
    const std::set<ElementId>& poiNeighborIds = std::set<ElementId>());

  virtual void setConfiguration(const Settings& conf);

  void calculateMatch(const ElementId& eid1, const ElementId& eid2);
  void calculateMatchWeka(const ElementId& eid1, const ElementId& eid2);

  virtual const MatchClassification& getClassification() const { return _class; }

  virtual MatchMembers getMatchMembers() const { return MatchMembers::Poi | MatchMembers::Polygon; }

  virtual QString getMatchName() const { return _matchName; }

  virtual std::set< std::pair<ElementId, ElementId> > getMatchPairs() const;

  virtual double getProbability() const { return _class.getMatchP(); }

  virtual bool isConflicting(const Match& /*other*/, const ConstOsmMapPtr& /*map*/) const
  { return false; }

  virtual bool isWholeGroup() const { return true; }

  virtual QString toString() const;

  virtual std::map<QString, double> getFeatures(const ConstOsmMapPtr& m) const;

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

  void setMatchDistanceThreshold(double distance);
  void setReviewDistanceThreshold(double distance);
  void setNameScoreThreshold(double threshold);
  void setTypeScoreThreshold(double threshold);
  void setReviewIfMatchedTypes(const QStringList& types);
  void setEnableAdvancedMatching(bool enabled) { _enableAdvancedMatching = enabled; }
  void setEnableReviewReduction(bool enabled) { _enableReviewReduction = enabled; }

private:

  static QString _matchName;

  ConstOsmMapPtr _map;

  ElementId _eid1;
  ElementId _eid2;
  ConstElementPtr _poi;
  ConstElementPtr _poly;
  boost::shared_ptr<geos::geom::Geometry> _poiGeom;
  boost::shared_ptr<geos::geom::Geometry> _polyGeom;
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
  QStringList _reviewIfMatchedTypes;

  double _nameScore;
  double _nameScoreThreshold;

  double _addressScore;

  //These are only used by PoiPolygonCustomRules and PoiPolygonDistance
  std::set<ElementId> _polyNeighborIds;
  std::set<ElementId> _poiNeighborIds;

  MatchClassification _class;

  bool _enableAdvancedMatching;
  bool _enableReviewReduction;

  boost::shared_ptr<const PoiPolygonRfClassifier> _rf;

  void _categorizeElementsByGeometryType(const ElementId& eid1, const ElementId& eid2);

  unsigned int _calculateEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getDistanceEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getConvexPolyDistanceEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getTypeEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getNameEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getAddressEvidence(ConstElementPtr poi, ConstElementPtr poly);

  bool _featureHasReviewIfMatchedType(ConstElementPtr element) const;
};

}

#endif // POIPOLYGONMATCH_H
