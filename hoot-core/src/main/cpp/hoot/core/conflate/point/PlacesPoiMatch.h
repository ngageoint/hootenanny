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
#ifndef POIMATCH_H
#define POIMATCH_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/conflate/Match.h>
#include <hoot/core/conflate/MatchThreshold.h>

namespace hoot
{

/**
 * This is loosely based on the "PLACES" way of doing things. It uses a set of rules to assign a
 * "probability" score.
 */
class PlacesPoiMatch : public Match
{
public:

  static QString epsilonDistanceKey() { return "places.poi.epsilon.distance"; }
  static QString goodnessThresholdKey() { return "places.goodness.threshold"; }
  static QString distanceWeightKey() { return "places.distance.weight"; }
  static QString stringWeightKey() { return "places.string.weight"; }
  static QString minimumEditSimilarityKey() { return "places.minimum.edit.similarity"; }

  PlacesPoiMatch(const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
    ConstMatchThresholdPtr threshold);

  virtual const MatchClassification& getClassification() const { return _p; }

  virtual MatchMembers getMatchMembers() const { return MatchMembers::Poi; }

  virtual QString getMatchName() const { return _matchName; }

  virtual double getProbability() const;

  /**
   * PlacesPoi matches never conflict other POI matches, but conflict with everything else.
   */
  virtual bool isConflicting(const Match& other, const ConstOsmMapPtr& map) const;

  /**
   * Simply returns the two elements that were matched.
   */
  virtual set< pair<ElementId, ElementId> > getMatchPairs() const;

  virtual QString toString() const;

private:

  ElementId _eid1, _eid2;
  static QString _matchName;
  MatchClassification _p;
  double _goodnessThreshold;

  void _calculateClassification(const ConstOsmMapPtr& map);

  double _calculateGoodness(const ConstNodePtr &n1, const ConstNodePtr &n2) const;

  bool _isExactMatch(const ConstNodePtr &n1, const ConstNodePtr &n2) const;

  bool _isNameMatch(const ConstNodePtr &n1, const ConstNodePtr &n2) const;

  bool _isDistanceMatch(const ConstNodePtr &n1, const ConstNodePtr &n2) const;
};

}

#endif // POIMATCH_H
