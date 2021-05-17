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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef BUILDINGMATCH_H
#define BUILDINGMATCH_H

// hoot
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchDetails.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

class BuildingRfClassifier;

/**
 * Matches two building elements. If there is a many to many match it will be resolved by the
 * conflation routines and then merged properly with the BuildingMerger.
 */
class BuildingMatch : public Match, public MatchDetails
{
public:

  static QString className() { return "hoot::BuildingMatch"; }

  static const QString MATCH_NAME;

  BuildingMatch() = default;
  BuildingMatch(const ConstMatchThresholdPtr& mt);
  BuildingMatch(const ConstOsmMapPtr& map, const std::shared_ptr<const BuildingRfClassifier>& rf,
                const ElementId& eid1, const ElementId& eid2, const ConstMatchThresholdPtr& mt);
  ~BuildingMatch() = default;

  const MatchClassification& getClassification() const override { return _p; }

  std::map<QString, double> getFeatures(const ConstOsmMapPtr& m) const override;

  MatchMembers getMatchMembers() const override { return MatchMembers::Polygon; }

  QString getName() const override { return MATCH_NAME; }

  QString getClassName() const override { return className(); }

  double getProbability() const override;

  /**
   * Building matches never conflict other building matches, but conflict with everything else.
   */
  bool isConflicting(const ConstMatchPtr& other, const ConstOsmMapPtr& map,
    const QHash<QString, ConstMatchPtr>& matches = QHash<QString, ConstMatchPtr>()) const override;

  /**
   * Simply returns the two elements that were matched.
   */
  std::set<std::pair<ElementId, ElementId>> getMatchPairs() const override;

  QString toString() const override;

  QString explain() const override { return _explainText; }
  void setExplain(const QString& explainText) override { _explainText = explainText; }

  QString getDescription() const override { return "Matches buildings"; }

private:

  friend class PoiPolygonMergerCreatorTest;

  MatchClassification _p;
  std::shared_ptr<const BuildingRfClassifier> _rf;
  QString _explainText;

  // tag key to determine a feature's timestamp
  QString _dateTagKey;
  // format of a feature's timestamp
  QString _dateFormat;

  void _calculateClassification(const ConstOsmMapPtr& map);
  QStringList _getNonMatchDescription(const ConstOsmMapPtr& map, const MatchType& type,
                                      const ConstElementPtr& element1,
                                      const ConstElementPtr& element2);
  QStringList _createReviewIfSecondaryFeatureNewer(const ConstElementPtr& element1,
                                                   const ConstElementPtr& element2);
};

}

#endif // BUILDINGMATCH_H
