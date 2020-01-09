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
#ifndef POIPOLYGONMATCH_H
#define POIPOLYGONMATCH_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/MatchDetails.h>
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonRfClassifier.h>
#include <hoot/core/algorithms/extractors/AddressScoreExtractor.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonNameScoreExtractor.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonPhoneNumberScoreExtractor.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonTypeScoreExtractor.h>
#include <hoot/core/language/ToEnglishTranslator.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonCache.h>

namespace hoot
{

/**
 * This is an additive, rule based mechanism for matching POIs to polygons. See "POI to
 * Polygon Conflation" in the Hootenanny Algorithms document for more details.
 *
 * This class could use some refactoring.
 */
class PoiPolygonMatch : public Match, public MatchDetails, public Configurable
{

public:

  static std::string className() { return "hoot::PoiPolygonMatch"; }

  PoiPolygonMatch();
  // this constructor added primarily for testing purposes
  PoiPolygonMatch(ConstMatchThresholdPtr threshold);
  PoiPolygonMatch(const ConstOsmMapPtr& map, ConstMatchThresholdPtr threshold,
    std::shared_ptr<const PoiPolygonRfClassifier> rf,
    PoiPolygonCachePtr infoCache = PoiPolygonCachePtr(),
    const std::set<ElementId>& polyNeighborIds = std::set<ElementId>());

  virtual void setConfiguration(const Settings& conf) override;

  void calculateMatch(const ElementId& eid1, const ElementId& eid2);

  virtual const MatchClassification& getClassification() const override { return _class; }

  virtual MatchMembers getMatchMembers() const override
  { return MatchMembers::Poi | MatchMembers::Polygon; }

  virtual QString getMatchName() const override { return _matchName; }

  virtual std::set<std::pair<ElementId, ElementId>> getMatchPairs() const override;

  virtual double getProbability() const override { return _class.getMatchP(); }

  // Is the right implementation for this?
  virtual bool isConflicting(const ConstMatchPtr& /*other*/,
                             const ConstOsmMapPtr& /*map*/) const override
  { return false; }

  virtual bool isWholeGroup() const override { return true; }

  virtual QString toString() const override;

  virtual std::map<QString, double> getFeatures(const ConstOsmMapPtr& m) const override;

  /**
   * Pass through to the same method in PoiPolygonDistanceTruthRecorder
   */
  static void printMatchDistanceInfo();

  /**
   * Pass through to the same method in PoiPolygonDistanceTruthRecorder
   */
  static void resetMatchDistanceInfo();

  virtual QString explain() const override { return _explainText; }

  virtual QString getDescription() const override { return "Matches POIs with polygons"; }

  virtual MatchType getType() const override;

  void setMatchDistanceThreshold(const double distance);
  void setReviewDistanceThreshold(const double distance);
  void setNameScoreThreshold(const double threshold);
  void setTypeScoreThreshold(const double threshold);
  void setReviewIfMatchedTypes(const QStringList& types);
  void setEnableReviewReduction(const bool enabled) { _enableReviewReduction = enabled; }
  void setMatchEvidenceThreshold(const int threshold) { _matchEvidenceThreshold = threshold; }
  void setReviewEvidenceThreshold(const int threshold) { _reviewEvidenceThreshold = threshold; }
  void setDisableSameSourceConflation(const bool disabled)
  { _disableSameSourceConflation = disabled; }
  void setDisableSameSourceConflationMatchTagKeyPrefixOnly(const bool disabled)
  { _disableSameSourceConflationMatchTagKeyPrefixOnly = disabled; }
  void setSourceTagKey(const QString& key) { _sourceTagKey = key; }
  void setReviewMultiUseBuildings(const bool review) { _reviewMultiUseBuildings = review; }
  void setAddressMatchingEnabled(const bool enabled) { _addressMatchEnabled = enabled; }

  //summary of match types found; assumes one invocation of this class per executed process; would
  //like to handle these in a different way
  static long matchesProcessed;
  static long distanceMatches;
  static long typeMatches;
  static long noTypeFoundCount;
  static long nameMatches;
  static long namesProcessed;
  static long nameMatchCandidates;
  static long addressMatches;
  static long addressesProcessed;
  static long addressMatchCandidates;
  static long phoneNumberMatches;
  static long phoneNumbersProcesed;
  static long phoneNumberMatchCandidates;
  static long convexPolyDistanceMatches;
  static long numReviewReductions;

private:

  friend class PoiPolygonMergerCreatorTest;

  static QString _matchName;

  ConstOsmMapPtr _map;

  ElementId _eid1;
  ElementId _eid2;
  ConstNodePtr _poi;
  ConstElementPtr _poly;
  std::shared_ptr<geos::geom::Geometry> _poiGeom;
  std::shared_ptr<geos::geom::Geometry> _polyGeom;
  bool _e1IsPoi;

  //min number evidences pieces required to classify a match
  unsigned int _matchEvidenceThreshold;
  //min number evidences pieces required to classify a review; should be less than
  //_matchEvidenceThreshold
  unsigned int _reviewEvidenceThreshold;

  //measured distance between the two elements
  double _distance;
  //max distance allowed between the elements where they can be considered a distance match
  double _matchDistanceThreshold;
  // max distance allowed between the elements where they can be considered for review; note: this
  // possibly should be renamed to search distance
  double _reviewDistanceThreshold;
  double _reviewDistancePlusCe;
  //true if the distance between the elements, given CE, is within the review distance; absolute
  //requirement for matching
  bool _closeDistanceMatch;

  // should be able to shrink some of this scorer code down with some abstraction

  std::shared_ptr<PoiPolygonTypeScoreExtractor> _typeScorer;
  double _typeScore;
  double _typeScoreThreshold;
  QStringList _reviewIfMatchedTypes;

  PoiPolygonNameScoreExtractor _nameScorer;
  double _nameScore;
  double _nameScoreThreshold;

  AddressScoreExtractor _addressScorer;
  double _addressScore;
  bool _addressMatchEnabled;

  PoiPolygonPhoneNumberScoreExtractor _phoneNumberScorer;
  double _phoneNumberScore;
  bool _phoneNumberMatchEnabled;

  //These is used by PoiPolygonReviewReducer
  // all the polys within the search radius of the POI being matched
  std::set<ElementId> _polyNeighborIds;

  MatchClassification _class;

  bool _enableReviewReduction;

  bool _disableSameSourceConflation;
  bool _disableSameSourceConflationMatchTagKeyPrefixOnly;
  QString _sourceTagKey;

  bool _reviewMultiUseBuildings;

  std::shared_ptr<const PoiPolygonRfClassifier> _rf;

  QString _explainText;

  static std::shared_ptr<ToEnglishTranslator> _translator;

  PoiPolygonPoiCriterion _poiCrit;
  PoiPolygonPolyCriterion _polyCrit;

  PoiPolygonCachePtr _infoCache;

  int _timingThreshold;

  void _categorizeElementsByGeometryType();

  bool _inputFeaturesHaveSameSource() const;

  unsigned int _calculateEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getDistanceEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getConvexPolyDistanceEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getTypeEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getNameEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getAddressEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getPhoneNumberEvidence(ConstElementPtr poi, ConstElementPtr poly);

  bool _featureHasReviewIfMatchedType(ConstElementPtr element) const;

  bool _skipForReviewTypeDebugging() const;
};

}

#endif // POIPOLYGONMATCH_H
