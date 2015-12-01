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
#ifndef CUSTOMPOIMATCH_H
#define CUSTOMPOIMATCH_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/conflate/Match.h>
#include <hoot/core/conflate/MatchDetails.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/point/PoiRfClassifier.h>

namespace hoot
{

/**
 * This is loosely based on the "PLACES" way of doing things. It uses a set of rules to assign a
 * "probability" score.
 */
class CustomPoiMatch : public Match, public MatchDetails
{
public:

  static QString epsilonDistanceKey() { return "places.poi.epsilon.distance"; }

  CustomPoiMatch(const ConstOsmMapPtr& map, const PoiRfClassifierPtr &rf, const ElementId& eid1,
    const ElementId& eid2, ConstMatchThresholdPtr mt);

  virtual const MatchClassification& getClassification() const { return _p; }

  virtual map<QString, double> getFeatures(const shared_ptr<const OsmMap>& m) const;

  virtual QString getMatchName() const { return _matchName; }

  virtual double getProbability() const;

  /**
   * CustomPoi matches never conflict other POI matches, but conflict with everything else.
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
  PoiRfClassifierPtr _rf;

  void _calculateClassification(const ConstOsmMapPtr& map);

  bool _isExactMatch(const ConstNodePtr &n1, const ConstNodePtr &n2) const;

  void _classify(const ConstNodePtr& n1, const ConstNodePtr& n2, MatchClassification& p) const;

  bool _isDistanceMatch(const ConstNodePtr &n1, const ConstNodePtr &n2) const;

  bool _isNameMatch(const ConstNodePtr &n1, const ConstNodePtr &n2) const;

};

}

#endif // CUSTOMPOIMATCH_H
