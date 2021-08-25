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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
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
#include <hoot/core/conflate/poi-polygon/PoiPolygonInfoCache.h>

namespace hoot
{

/**
 * This is an additive, rule based mechanism for matching POIs to polygons. See "POI to
 * Polygon Conflation" in the Hootenanny Algorithms document for more details.
 *
 * This class could definitely use some refactoring.
 */
class PoiPolygonMatch : public Match, public MatchDetails, public Configurable
{

public:

  static QString className() { return "PoiPolygonMatch"; }

  static const QString MATCH_NAME;

  PoiPolygonMatch() = default;
  PoiPolygonMatch(const ConstOsmMapPtr& map, ConstMatchThresholdPtr threshold,
    std::shared_ptr<const PoiPolygonRfClassifier> rf,
    PoiPolygonInfoCachePtr infoCache = PoiPolygonInfoCachePtr(),
    const std::set<ElementId>& polyNeighborIds = std::set<ElementId>());
  ~PoiPolygonMatch() = default;

  void calculateMatch(const ElementId& eid1, const ElementId& eid2);

  static QString getPoiPolygonMatchName() { return MATCH_NAME; }

  /**
   * Pass through to the same method in PoiPolygonDistanceTruthRecorder
   */
  static void printMatchDistanceInfo();
  /**
   * Pass through to the same method in PoiPolygonDistanceTruthRecorder
   */
  static void resetMatchDistanceInfo();

  void setConfiguration(const Settings& conf) override;

  const MatchClassification& getClassification() const override { return _class; }
  MatchMembers getMatchMembers() const override
  { return MatchMembers::Poi | MatchMembers::Polygon; }
  std::set<std::pair<ElementId, ElementId>> getMatchPairs() const override;
  double getProbability() const override { return _class.getMatchP(); }
  bool isConflicting(
    const ConstMatchPtr& /*other*/, const ConstOsmMapPtr& /*map*/,
    const QHash<QString, ConstMatchPtr>& /*matches*/) const override { return false; }
  bool isWholeGroup() const override { return true; }
  std::map<QString, double> getFeatures(const ConstOsmMapPtr& m) const override;
  QString explain() const override { return _explainText; }
  MatchType getType() const override;

  QString getName() const override { return getPoiPolygonMatchName(); }
  QString getClassName() const override { return className(); }
  QString toString() const override;
  QString getDescription() const override { return "Matches POIs with polygons"; }

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
  void setDisableIntradatasetConflation1(const bool disabled)
  { _disableIntradatasetConflation1 = disabled; }
  void setDisableIntradatasetConflation2(const bool disabled)
  { _disableIntradatasetConflation2 = disabled; }
  void setReviewMultiUseBuildings(const bool review) { _reviewMultiUseBuildings = review; }
  void setAddressMatchingEnabled(const bool enabled) { _addressMatchEnabled = enabled; }

  // summary of match types found; assumes one invocation of this class per executed process; would
  // like to handle these in a different way
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
  friend class PoiPolygonMatchTest;

  ConstOsmMapPtr _map;

  PoiPolygonInfoCachePtr _infoCache;

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

  // prevents elements with the same status from matching (same input dataset); this is a simpler
  // way to prevent intra-dataset matches than using _disableSameSourceConflation
  bool _disableIntradatasetConflation1;
  bool _disableIntradatasetConflation2;

  bool _reviewMultiUseBuildings;

  std::shared_ptr<const PoiPolygonRfClassifier> _rf;

  QString _explainText;

  static std::shared_ptr<ToEnglishTranslator> _translator;

  PoiPolygonPoiCriterion _poiCrit;
  PoiPolygonPolyCriterion _polyCrit;

  int _timingThreshold;

  // this constructor added primarily for testing purposes
  PoiPolygonMatch(ConstMatchThresholdPtr threshold);

  void _categorizeElementsByGeometryType();

  bool _inputFeaturesHaveSameSource() const;

  unsigned int _calculateEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getDistanceEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getConvexPolyDistanceEvidence(ConstElementPtr poi, ConstElementPtr poly) const;
  unsigned int _getTypeEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getNameEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getAddressEvidence(ConstElementPtr poi, ConstElementPtr poly);
  unsigned int _getPhoneNumberEvidence(ConstElementPtr poi, ConstElementPtr poly);

  bool _featureHasReviewIfMatchedType(ConstElementPtr element) const;

  bool _skipForReviewTypeDebugging() const;

  void _clearCache() const;
};

}

#endif // POIPOLYGONMATCH_H
