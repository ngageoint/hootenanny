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
#include <hoot/core/conflate/MatchDetails.h>
#include <hoot/core/util/Configurable.h>

#include "PoiPolygonRfClassifier.h"

namespace hoot
{

/**
 * This is a very simple mechanism for matching POIs to polygons. See "Building to POI Conflation"
 * in the Hootenanny Algorithms document for more details.
 */
class PoiPolygonMatch : public Match, public MatchDetails
{
public:

  PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
    ConstMatchThresholdPtr threshold, shared_ptr<const PoiPolygonRfClassifier> rf);

  PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
    ConstMatchThresholdPtr threshold, shared_ptr<const PoiPolygonRfClassifier> rf,
    double matchDistance, double reviewDistance, double nameScoreThreshold,
    double typeScoreThreshold);

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

  double _matchDistance;
  double _reviewDistance;
  double _nameScoreThreshold;
  double _typeScoreThreshold;

  QMap<QString, shared_ptr<TagAncestorDifferencer> > _tagAncestorDifferencers;

  //static QString _testUuid;

  void _calculateMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
                       const ElementId& eid2);

  /**
   * Returns a score from 0 to 1 representing the similarity of the names. A score of -1 means one
   * or both of the features have no names.
   */
  double _calculateNameScore(ConstElementPtr e1, ConstElementPtr e2) const;

  /**
   * Returns true if the tag similarity score is greater than or equal to
   * poi.polygon.min.tag.score
   */
  bool _calculateTypeMatch(const ConstOsmMapPtr& map, ConstElementPtr e1, ConstElementPtr e2);

  double _getTagScore(ConstElementPtr e1, ConstElementPtr e2) const;
  QStringList _getRelatedTags(const Tags& tags) const;

};

}

#endif // POIPOLYGONMATCH_H
