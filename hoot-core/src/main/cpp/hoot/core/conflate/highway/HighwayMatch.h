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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef HIGHWAYMATCH_H
#define HIGHWAYMATCH_H

// hoot
#include <hoot/core/algorithms/linearreference/WaySublineMatchString.h>
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchDetails.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

class HighwayClassifier;
class SublineStringMatcher;
class ElementId;

/**
 * Matches two road elements
 */
class HighwayMatch : public Match, public MatchDetails
{
public:

  static QString className() { return "HighwayMatch"; }

  static const QString MATCH_NAME;

  HighwayMatch() = default;
  HighwayMatch(const std::shared_ptr<HighwayClassifier>& classifier,
               const std::shared_ptr<SublineStringMatcher>& sublineMatcher,
               const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
               ConstMatchThresholdPtr mt);
  ~HighwayMatch() = default;

  /**
   * Simply returns the two elements that were matched.
   */
  std::set<std::pair<ElementId, ElementId>> getMatchPairs() const override;
  double getProbability() const override;
  double getScore() const override { return _score; }
  QString explain() const override { return _explainText; }
  const MatchClassification& getClassification() const override { return _c; }
  MatchMembers getMatchMembers() const override { return MatchMembers::Polyline; }

  bool isConflicting(const ConstMatchPtr& other, const ConstOsmMapPtr& map,
    const QHash<QString, ConstMatchPtr>& matches = QHash<QString, ConstMatchPtr>()) const override;

  std::map<QString, double> getFeatures(const ConstOsmMapPtr& m) const override;

  const WaySublineMatchString& getSublineMatch() const { return _sublineMatch; }
  const std::shared_ptr<SublineStringMatcher>& getSublineMatcher() const { return _sublineMatcher; }

  QString toString() const override;

  QString getName() const override { return getHighwayMatchName(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Matches roads with the 2nd Generation (Unifying) Algorithm"; }

  static QString getHighwayMatchName() { return MATCH_NAME; }

private:

  std::shared_ptr<HighwayClassifier> _classifier;
  MatchClassification _c;
  double _score;
  QString _explainText;

  std::shared_ptr<SublineStringMatcher> _sublineMatcher;
  double _minSplitSize;
  WaySublineMatchString _sublineMatch;
  static QString _noMatchingSubline;

  mutable QHash<ElementId, bool> _conflicts;

  double _calculateExpertProbability(const ConstOsmMapPtr& map) const;

  bool _isOrderedConflicting(
    const ConstOsmMapPtr& map, const ElementId& sharedEid, const ElementId& other1,
    const ElementId& other2) const;

  void _updateNonMatchDescriptionBasedOnGeometricProperties(
    QStringList& description, const ConstOsmMapPtr& map, const ConstElementPtr e1,
    const ConstElementPtr e2) const;
};

}

#endif // HIGHWAYMATCH_H
