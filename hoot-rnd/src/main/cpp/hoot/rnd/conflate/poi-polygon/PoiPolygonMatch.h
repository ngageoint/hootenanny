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
#include <hoot/core/schema/TagAncestorDifferencer.h>
#include <hoot/core/schema/TagCategoryDifferencer.h>
#include <hoot/core/conflate/MatchDetails.h>

#include "PoiPolygonRfClassifier.h"

namespace hoot
{

/**
 * TODO: update
 *
 * This is a very simple mechanism for matching POIs to polygons. The following rules are used:
 *
 * - Match - If the names are fairly similar or non-existant & the point is inside the polygon
 * - Review - If the point is inside the polygon, but the names are different OR
 *            If the point is close to the polygon and the names are similar
 * - Miss - Everything else
 *
 * This supports intra-dataset and inter-dataset conflation.
 *
 * If there are multiple overlapping matches then they will all get marked as needing review in
 * PoiPolygonMergerCreator.
 */
class PoiPolygonMatch : public Match, public MatchDetails
{
public:

  PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
    ConstMatchThresholdPtr threshold);
  PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
    ConstMatchThresholdPtr threshold, shared_ptr<const PoiPolygonRfClassifier> rf);

  virtual const MatchClassification& getClassification() const { return _c; }

  virtual MatchMembers getMatchMembers() const { return MatchMembers::Poi | MatchMembers::Polygon; }

  virtual QString getMatchName() const { return _matchName; }

  virtual set< pair<ElementId, ElementId> > getMatchPairs() const;

  virtual double getProbability() const { return _c.getMatchP(); }

  static bool isBuildingIsh(ConstElementPtr e);

  virtual bool isConflicting(const Match& /*other*/, const ConstOsmMapPtr& /*map*/) const
  { return false; }

  static bool isPoiIsh(ConstElementPtr e);

  virtual bool isWholeGroup() const { return true; }

  virtual QString toString() const;

  virtual map<QString, double> getFeatures(const shared_ptr<const OsmMap>& m) const;

private:

  ElementId _eid1;
  ElementId _eid2;
  shared_ptr<const PoiPolygonRfClassifier> _rf;

  static QString _matchName;
  ElementId _poiEid, _polyEid;
  MatchClassification _c;

  static QString _testUuid;

  QString _uuid1;
  QString _uuid2;
  bool _typeMatch;
  QString _typeMatchAttributeKey;
  QString _typeMatchAttributeValue;
  double _nameScore;
  bool _nameMatch;
  QString _names1;
  QString _names2;
  double _circularError1;
  double _circularError2;
  double _ce;
  double _distance;
  double _reviewDistance;
  bool _closeMatch;
  int _evidence;

  bool _ancestorTypeMatch;
  double _ancestorDistance;

  bool _addressMatch;

  bool _exactNameMatch;

  QMap<QString, shared_ptr<TagAncestorDifferencer> > _tagAncestorDifferencers;
  QMap<QString, shared_ptr<TagCategoryDifferencer> > _tagCategoryDifferencers;

  void _calculateMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
                       const ElementId& eid2);
  void _calculateMatchWeka(const ConstOsmMapPtr& map, const ElementId& eid1,
                           const ElementId& eid2);
  void _calculateMatchWekaGetFeatures(const ConstOsmMapPtr& map, const ElementId& eid1,
                                      const ElementId& eid2);

  /**
   * Returns a score from 0 to 1 representing the similarity of the names. A score of -1 means one
   * or both of the features have no names.
   */
  double _calculateNameScore(ConstElementPtr e1, ConstElementPtr e2) const;

  /**
   * Returns true if at least one POI tag is an exact match between the two elements. E.g.
   * amenity=cafe in e1 and in e2.
   */
  bool _calculateTypeMatch(ConstElementPtr e1, ConstElementPtr e2) /*const*/;

  bool _calculateAncestorTypeMatch(const ConstOsmMapPtr& map, ConstElementPtr e1,
                                   ConstElementPtr e2) /*const*/;

  double _getTagDistance(const QString type, const QString kvp, ConstOsmMapPtr map,
                         ConstElementPtr e1, ConstElementPtr e2);
  double _getTagDistance(const QString kvp, ConstElementPtr e1, ConstElementPtr e2) const;
  QStringList _getRelatedTags(const QString relateToKvp, const Tags& tags) const;
  QStringList _getTagsByCategory(const QString category, const Tags& tags) const;
  bool _oneGeneric(ConstElementPtr e1, ConstElementPtr e2) const;

  bool _getAddressMatch(ConstElementPtr e1, ConstElementPtr e2);

  double _getReviewDistance(ConstElementPtr element);
  double _getMatchDistance(ConstElementPtr element);

};

}

#endif // POIPOLYGONMATCH_H
