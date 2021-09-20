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
#ifndef SCRIPTMATCHCREATOR_H
#define SCRIPTMATCHCREATOR_H

// Hoot
#include <hoot/core/conflate/SearchRadiusProvider.h>
#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/core/criterion/ElementCriterion.h>

#include <hoot/js/PluginContext.h>

namespace hoot
{

class ScriptMatchVisitor;

/**
 * Match creator for all generic conflation scripts
 *
 * @sa ScriptMatch
 * @todo This class could use some refactoring after recent changes.
 */
class ScriptMatchCreator : public MatchCreator, public SearchRadiusProvider
{
public:

  static QString className() { return "ScriptMatchCreator"; }

  static const QString POINT_POLYGON_SCRIPT_NAME;

  ScriptMatchCreator() = default;
  ~ScriptMatchCreator() override = default;

  /**
   * @see SearchRadiusProvider
   */
  void init(const ConstOsmMapPtr& map) override;

  /**
   * @see SearchRadiusProvider
   */
  Meters calculateSearchRadius(const ConstOsmMapPtr& map, const ConstElementPtr& e) override;

  /**
   * @see MatchCreator
   */
  MatchPtr createMatch(const ConstOsmMapPtr&, ElementId, ElementId) override;

  /**
   * Search the provided map for POI matches and add the matches to the matches vector.
   */
  void createMatches(
    const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& matches,
    ConstMatchThresholdPtr threshold) override;

  /**
   * @see MatchCreator
   */
  std::vector<CreatorDescription> getAllCreators() const override;

  /**
   * @see MatchCreator
   */
  void setArguments(const QStringList& args) override;

  /**
   * Determines whether an element is a candidate for matching for this match creator
   *
   * @param element element to determine the match candidate status of
   * @param map the map the element whose candidacy is being determined belongs to
   * @return true if the element is a match candidate; false otherwise
   */
  bool isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map) override;

  /**
   * @see MatchCreator
   */
  std::shared_ptr<MatchThreshold> getMatchThreshold() override;

  /**
   * @see MatchCreator
   */
  QString getName() const override;

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  /**
   * @brief setRunOneToManyRailMatching toggles activation of the one to many rail matching routine
   * when this instance runs Railway Conflation (uses Railway.js).
   * @param runMatching turns one to many rail matching on/off
   * @param scriptInfo determines which feature type is being conflated by this match creator
   * @param identifyingKeys tags used to identify secondary one to many matching eligible rail
   * features (only one must be satisfied)
   * @param transferKeys
   */
  void setRunOneToManyRailMatching(
    const bool runMatching, const CreatorDescription::BaseFeatureType& scriptInfo,
    const QStringList& identifyingKeys, const QStringList& transferKeys) const;

private:

  friend class ScriptMatchCreatorTest;

  std::shared_ptr<PluginContext> _script;
  QString _scriptPath;
  CreatorDescription _scriptInfo;

  std::shared_ptr<ScriptMatchVisitor> _cachedScriptVisitor;
  std::shared_ptr<MatchThreshold> _matchThreshold;
  QMap<QString, Meters> _cachedCustomSearchRadii;
  QMap<QString, double> _candidateDistanceSigmaCache;
  QMap<QString, CreatorDescription> _descriptionCache;

  ElementCriterionPtr _pointPolyPolyCrit;
  ElementCriterionPtr _pointPolyPointCrit;

  CreatorDescription _getScriptDescription(QString path) const;

  std::shared_ptr<ScriptMatchVisitor> _getCachedVisitor(const ConstOsmMapPtr& map);
};

}

#endif // SCRIPTMATCHCREATOR_H
