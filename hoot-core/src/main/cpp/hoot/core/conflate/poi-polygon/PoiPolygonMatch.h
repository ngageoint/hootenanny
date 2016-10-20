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

class PoiPolygonEvidenceScorer;

/**
 * This is a additive, rules based mechanism for matching POIs to polygons. See "POI to
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
    const set<ElementId>& areaIds, const set<ElementId>& poiIds);

  PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
    ConstMatchThresholdPtr threshold, shared_ptr<const PoiPolygonRfClassifier> rf,
    double matchDistance, double reviewDistance, double nameScoreThreshold,
    double typeScoreThreshold);

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

  static void printMatchDistanceInfo();
  static void resetMatchDistanceInfo();

private:

  //TODO: can probably get rid of these two eid's
  ElementId _eid1;
  ElementId _eid2;
  shared_ptr<const PoiPolygonRfClassifier> _rf;
  static QString _matchName;
  //ElementId _poiEid, _polyEid;
  ConstElementPtr _poi;
  ConstElementPtr _poly;
  shared_ptr<Geometry> _poiGeom;
  shared_ptr<Geometry> _polyGeom;
  MatchClassification _class;
  int _badGeomCount;
  ConstOsmMapPtr _map;
  bool _e1IsPoi;

  double _distance;
  double _nameScore;
  bool _nameMatch;
  bool _exactNameMatch;
  double _typeScore;
  bool _typeMatch;

  double _matchDistance;
  double _reviewDistance;
  double _nameScoreThreshold;
  double _typeScoreThreshold;

  static QString _testUuid;
  bool _testFeatureFound;
  static QMultiMap<QString, double> _poiMatchRefIdsToDistances;
  static QMultiMap<QString, double> _poiReviewRefIdsToDistances;
  static QMultiMap<QString, double> _polyMatchRefIdsToDistances;
  static QMultiMap<QString, double> _polyReviewRefIdsToDistances;

  set<ElementId> _areaNeighborIds;
  set<ElementId> _poiNeighborIds;

  void _separateElementsByGeometryType(const ElementId& eid1, const ElementId& eid2);

  bool _parseGeometries();

  void _calculateMatch(const ElementId& eid1, const ElementId& eid2);

  void _recordDistanceTruth(const PoiPolygonEvidenceScorer& evidenceScorer);

  static void _printMatchDistanceInfo(const QString matchType,
                                      const QMultiMap<QString, double>& distanceInfo);

};

}

#endif // POIPOLYGONMATCH_H
