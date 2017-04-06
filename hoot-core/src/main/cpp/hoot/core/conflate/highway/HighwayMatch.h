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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HIGHWAYMATCH_H
#define HIGHWAYMATCH_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/linearreference/WaySublineMatchString.h>
#include <hoot/core/conflate/MatchClassification.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/Match.h>
#include <hoot/core/conflate/MatchDetails.h>

// Qt
#include <QHash>

// tgs
#include <tgs/SharedPtr.h>

namespace Tgs
{
  class RandomForest;
}

namespace hoot
{
using namespace Tgs;

class HighwayClassifier;
class SublineStringMatcher;
class ElementId;

/**
 * Matches two highway elements.
 */
class HighwayMatch : public Match, public MatchDetails
{
public:
  HighwayMatch(const boost::shared_ptr<HighwayClassifier>& classifier,
               const boost::shared_ptr<SublineStringMatcher>& sublineMatcher,
               const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
               ConstMatchThresholdPtr mt);

  virtual QString explain() const;

  virtual const MatchClassification& getClassification() const { return _c; }

  virtual map<QString, double> getFeatures(const boost::shared_ptr<const OsmMap>& m) const;

  virtual QString getMatchName() const { return getHighwayMatchName(); }
  static QString getHighwayMatchName() { return _matchName; }

  virtual double getProbability() const;

  virtual double getScore() const { return _score; }

  const boost::shared_ptr<SublineStringMatcher>& getSublineMatcher() const { return _sublineMatcher; }

  virtual bool isConflicting(const Match& other, const ConstOsmMapPtr& map) const;

  /**
   * Simply returns the two elements that were matched.
   */
  virtual set< pair<ElementId, ElementId> > getMatchPairs() const;

  const WaySublineMatchString& getSublineMatch() const { return _sublineMatch; }

  virtual QString toString() const;

private:

  boost::shared_ptr<HighwayClassifier> _classifier;
  ElementId _eid1, _eid2;
  boost::shared_ptr<SublineStringMatcher> _sublineMatcher;
  MatchClassification _c;
  double _minSplitSize;
  double _score;
  WaySublineMatchString _sublineMatch;
  mutable QHash<ElementId, bool> _conflicts;
  QString _explainText;
  static QString _matchName;
  static QString _noMatchingSubline;

  double _calculateExpertProbability(const ConstOsmMapPtr& map) const;

  bool _isOrderedConflicting(const ConstOsmMapPtr& map, ElementId sharedEid,
    ElementId other1, ElementId other2) const;

};

}

#endif // HIGHWAYMATCH_H
