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
#ifndef CALCULATESTATSOP_H
#define CALCULATESTATSOP_H

// hoot
#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/info/SingleStat.h>
#include <hoot/core/info/StatData.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/schema/ScriptSchemaTranslator.h>

namespace hoot
{
class FilteredVisitor;

/**
 * Calcs the set of stats that feeds the stats command.
 *
 * Statistics data definitions which can be defined generically have moved to the json file
 * identified by ConfigOptions.getStatsGenericDataFile(). The json file contains two StatData struct
 * lists containing the slow and quick generic stat definitions. See the CalculateStatsOp::StatCall
 * and CalculateStatsOp::StatData comments below for an explanation of the entries.
 */
class CalculateStatsOp : public ConstOsmMapOperation, public Configurable
{
public:

  static QString className() { return "hoot::CalculateStatsOp"; }

  CalculateStatsOp(QString mapName = "", bool inputIsConflatedMapOutput = false);
  CalculateStatsOp(
    ElementCriterionPtr criterion, QString mapName = "", bool inputIsConflatedMapOutput = false);
  ~CalculateStatsOp() = default;

  double getSingleStat(const QString& n) const;
  bool hasSingleStat(const QString& n) const;
  long indexOfSingleStat(const QString& n) const;

  void apply(const OsmMapPtr& map) override;

  void setConfiguration(const Settings& conf) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Calculates map statistics"; }

  QList<SingleStat> getStats() const { return _stats; }

  void setFilter(const QSet<QString>& filter) { _filter = filter; }
  void setQuickSubset(bool quick) { _quick = quick; }

private:

  friend class CalculateStatsOpTest;

  ElementCriterionPtr _criterion;
  // simple map name string for logging purposes
  QString _mapName;
  std::shared_ptr<const OsmMap> _constMap;
  bool _quick;
  // We differentiate between maps that are the input to a conflation job vs those that are the
  // output of a conflation job.  Another option would be to refactor this class for both maps
  // meant to be input to a conflation job and those that are output from a conflation job.
  bool _inputIsConflatedMapOutput;
  QString _statsFileName;
  QList<SingleStat> _stats;
  // list of GeometryTypeCriterion class names used to control which statistics are generated; see
  // ConflateExecutor and SuperfluousConflateOpRemover
  QSet<QString> _filter;

  std::shared_ptr<ScriptSchemaTranslator> _schemaTranslator;

  QHash<QString, std::shared_ptr<ElementCriterion>> _criterionCache;
  QHash<QString, std::shared_ptr<ConstElementVisitor>> _appliedVisitorCache;

  QList<StatData> _quickStatData;
  QList<StatData> _slowStatData;
  // the index of the stat calculation we are currently doing
  int _currentStatCalcIndex;
  // total number of stat calculations made with a visitor; basically any time _applyVisitor is
  // called; any time a stat is added that results in another manual call to _applyVisitor (done
  // outside of _interpretStatData), this total needs to be incremented.
  int _totalStatCalcs;
  QStringList _featureTypesToSkip;

  // These are here for debugging purposes only.
  int _numInterpresetStatDataCalls;
  int _numInterpretStatVisCacheHits;
  int _numGenerateFeatureStatCalls;

  QMap<CreatorDescription::BaseFeatureType, double> _conflatableFeatureCounts;

  void _initStatCalc();
  void _initConflatableFeatureCounts();
  void _readGenericStatsConfiguration();
  void _addStat(const QString& name, double value);
  bool _statPassesFilter(const StatData& statData) const;
  int _getNumStatsPassingFilter(const QList<StatData>& stats) const;
  void _interpretStatData(const std::shared_ptr<const OsmMap>& constMap, const StatData& d);
  double _getRequestedStatValue(const ElementVisitor* pVisitor, StatData::StatCall call) const;
  void _generateFeatureStats(
    const CreatorDescription::BaseFeatureType& featureType, const float conflatableCount,
    const CreatorDescription::FeatureCalcType& type, ElementCriterionPtr criterion,
    const long poisMergedIntoPolys, const long poisMergedIntoPolysFromMap1,
    const long poisMergedIntoPolysFromMap2);

  /**
   * @brief getMatchCreator finds the match creator (in the supplied vector) by name
   * @param [in]  matchCreators vector of matchCreators to search
   * @param [in]  matchCreatorName name for which to search
   * @param [out] featureType base feature type for the found matchCreator
   * @return ptr to match creator, if found, otherwise std::shared_ptr to null
   */
  std::shared_ptr<MatchCreator> _getMatchCreator(
    const std::vector<std::shared_ptr<MatchCreator>>& matchCreators,
    const QString &matchCreatorName, CreatorDescription::BaseFeatureType& featureType) const;
  static bool _matchDescriptorCompare(
    const CreatorDescription& m1, const CreatorDescription& m2);

  double _applyVisitor(
    const hoot::FilteredVisitor& v, const QString& statName,
    StatData::StatCall call = StatData::StatCall::Stat);
  double _applyVisitor(
    const hoot::FilteredVisitor& v, boost::any& visitorData, const QString& statName,
    StatData::StatCall call = StatData::StatCall::Stat);
  double _applyVisitor(
    const ElementCriterion& pCrit, ElementVisitor& pVis, const QString& statName,
    StatData::StatCall call = StatData::StatCall::Stat);
  double _applyVisitor(
    const std::shared_ptr<ElementCriterion> pCrit, std::shared_ptr<ConstElementVisitor> pVis,
    const QString& statName, StatData::StatCall call = StatData::StatCall::Stat);
  void _applyVisitor(std::shared_ptr<ConstElementVisitor> v, const QString& statName);
  void _applyVisitor(ConstElementVisitor* v, const QString& statName);
  double _getApplyVisitor(std::shared_ptr<ConstElementVisitor> v, const QString& statName);
  ConstElementVisitorPtr _getElementVisitorForFeatureType(
    const CreatorDescription::BaseFeatureType& featureType) const;
};

}

#endif // CALCULATESTATSOP_H
