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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef BUILDINGMATCH_H
#define BUILDINGMATCH_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/Match.h>
#include <hoot/core/conflate/MatchDetails.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/MatchClassification.h>

// tgs
#include <tgs/SharedPtr.h>

namespace Tgs
{
  class RandomForest;
}

namespace hoot
{

using namespace Tgs;

class BuildingRfClassifier;

/**
 * Matches two building elements. If there is a many to many match it will be resolved by the
 * conflation routines and then merged properly with the BuildingMerge.
 */
class BuildingMatch : public Match, public MatchDetails
{
public:
  BuildingMatch(const ConstOsmMapPtr& map, boost::shared_ptr<const BuildingRfClassifier> rf,
                const ElementId& eid1, const ElementId& eid2,
                ConstMatchThresholdPtr mt);

  virtual const MatchClassification& getClassification() const { return _p; }

  virtual map<QString, double> getFeatures(const boost::shared_ptr<const OsmMap>& m) const;

  virtual MatchMembers getMatchMembers() const { return MatchMembers::Polygon; }

  virtual QString getMatchName() const { return _matchName; }

  virtual double getProbability() const;

  /**
   * Building matches never conflict other building matches, but conflict with everything else.
   */
  virtual bool isConflicting(const Match& other, const ConstOsmMapPtr& map) const;

  /**
   * Simply returns the two elements that were matched.
   */
  virtual set< pair<ElementId, ElementId> > getMatchPairs() const;

  virtual QString toString() const;

  virtual QString explain() const { return _explainText; }

private:

  ElementId _eid1, _eid2;
  static QString _matchName;
  MatchClassification _p;
  boost::shared_ptr<const BuildingRfClassifier> _rf;
  QString _explainText;

  void _calculateClassification(const ConstOsmMapPtr& map);
};

}

#endif // BUILDINGMATCH_H
