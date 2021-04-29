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

// Qt
#include <QHash>

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

  static QString className() { return "hoot::HighwayMatch"; }

  static const QString MATCH_NAME;

  HighwayMatch() = default;
  HighwayMatch(const std::shared_ptr<HighwayClassifier>& classifier,
               const std::shared_ptr<SublineStringMatcher>& sublineMatcher,
               const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
               ConstMatchThresholdPtr mt);
  ~HighwayMatch() = default;

  QString explain() const override;

  const MatchClassification& getClassification() const override { return _c; }

  MatchMembers getMatchMembers() const override { return MatchMembers::Polyline; }

  std::map<QString, double> getFeatures(const ConstOsmMapPtr& m) const override;

  QString getName() const override { return getHighwayMatchName(); }
  static QString getHighwayMatchName() { return MATCH_NAME; }
  QString getClassName() const override { return className(); }

  double getProbability() const override;

  double getScore() const override { return _score; }

  const std::shared_ptr<SublineStringMatcher>& getSublineMatcher() const
  { return _sublineMatcher; }

  bool isConflicting(const ConstMatchPtr& other, const ConstOsmMapPtr& map,
    const QHash<QString, ConstMatchPtr>& matches = QHash<QString, ConstMatchPtr>()) const override;

  /**
   * Simply returns the two elements that were matched.
   */
  std::set<std::pair<ElementId, ElementId>> getMatchPairs() const override;

  const WaySublineMatchString& getSublineMatch() const { return _sublineMatch; }

  QString toString() const override;

  QString getDescription() const override
  { return "Matches roads with the 2nd Generation (Unifying) Algorithm"; }

private:

  std::shared_ptr<HighwayClassifier> _classifier;
  std::shared_ptr<SublineStringMatcher> _sublineMatcher;
  MatchClassification _c;
  double _minSplitSize;
  double _score;
  WaySublineMatchString _sublineMatch;
  mutable QHash<ElementId, bool> _conflicts;
  QString _explainText;
  static QString _noMatchingSubline;

  double _calculateExpertProbability(const ConstOsmMapPtr& map) const;

  bool _isOrderedConflicting(
    const ConstOsmMapPtr& map, ElementId sharedEid, ElementId other1, ElementId other2) const;

  void _updateNonMatchDescriptionBasedOnGeometricProperties(
    QStringList& description, const ConstOsmMapPtr& map, const ConstElementPtr e1,
    const ConstElementPtr e2);
};

}

#endif // HIGHWAYMATCH_H
