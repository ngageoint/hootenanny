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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "CalculateStatsOp.h"

#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/NeedsReviewCriterion.h>
#include <hoot/core/criterion/NoInformationCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/schema/ScriptSchemaTranslatorFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/CalculateAreaForStatsVisitor.h>
#include <hoot/core/visitors/CountUniqueReviewsVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FeatureCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/LengthOfWaysVisitor.h>
#include <hoot/core/visitors/MatchCandidateCountVisitor.h>
#include <hoot/core/visitors/SumNumericTagsVisitor.h>
#include <hoot/core/visitors/SchemaTranslatedTagCountVisitor.h>

#include <math.h>

using namespace std;
namespace bpt = boost::property_tree;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, CalculateStatsOp)

CalculateStatsOp::CalculateStatsOp(QString mapName, bool inputIsConflatedMapOutput)
  : _mapName(mapName),
    _quick(false),
    _inputIsConflatedMapOutput(inputIsConflatedMapOutput),
    _currentStatCalcIndex(1),
    _totalStatCalcs(0),
    // Unknown does not get us a usable element criterion, so skip it.
    _featureTypesToSkip(QStringList("unknown")),
    _numInterpresetStatDataCalls(0),
    _numInterpretStatVisCacheHits(0),
    _numGenerateFeatureStatCalls(0)
{
  setConfiguration(conf());
  _initConflatableFeatureCounts();
  _readGenericStatsConfiguration();
}

CalculateStatsOp::CalculateStatsOp(ElementCriterionPtr criterion, QString mapName,
                                   bool inputIsConflatedMapOutput)
  : _criterion(criterion),
    _mapName(mapName),
    _quick(false),
    _inputIsConflatedMapOutput(inputIsConflatedMapOutput),
    _currentStatCalcIndex(1),
    _totalStatCalcs(0),
    _featureTypesToSkip(QStringList("unknown")),
    _numInterpresetStatDataCalls(0),
    _numInterpretStatVisCacheHits(0),
    _numGenerateFeatureStatCalls(0)
{
  setConfiguration(conf());
  _initConflatableFeatureCounts();
  _readGenericStatsConfiguration();
}

void CalculateStatsOp::setConfiguration(const Settings& conf)
{
  ConfigOptions opts = ConfigOptions(conf);
  _statsFileName = opts.getStatsGenericDataFile();
}

void CalculateStatsOp::_readGenericStatsConfiguration()
{
  // read generic stats from json file
  bpt::ptree propPtree;
  bpt::read_json(_statsFileName.toLatin1().constData(), propPtree );

  _quickStatData.clear();
  _slowStatData.clear();
  QList<StatData>* pCurr = nullptr;
  QHash<QString, StatData::StatCall> enumLookup(
  {
    { "stat", StatData::StatCall::Stat },
    { "min", StatData::StatCall::Min },
    { "max", StatData::StatCall::Max },
    { "average", StatData::StatCall::Average },
    { "infocount", StatData::StatCall::InfoCount },
    { "infomin", StatData::StatCall::InfoMin },
    { "infomax", StatData::StatCall::InfoMax },
    { "infoaverage", StatData::StatCall::InfoAverage },
    { "infodiff", StatData::StatCall::InfoDiff }
  });

  foreach (bpt::ptree::value_type listType, propPtree)
  {
    if (listType.first == "#")
      continue; // skip comments
    else if (listType.first == "quick")
      pCurr = &_quickStatData;
    else if (listType.first == "slow")
      pCurr = &_slowStatData;
    else
    {
      throw HootException("Invalid stats data list name '" + listType.first + "' in " + _statsFileName.toStdString() +
                          ". Allowed: 'quick' or 'slow'.");
    }

    foreach (bpt::ptree::value_type entry, listType.second)
    {
      StatData newStatData;

      foreach (bpt::ptree::value_type data, entry.second)
      {
        const QString key = QString::fromStdString(data.first).toLower().trimmed();
        const QString val = QString::fromStdString(data.second.data()).trimmed();

        if (key == "name")
          newStatData.setName(val);
        else if (key == "visitor")
          newStatData.setVisitor(val);
        else if (key == "criterion")
          newStatData.setCriterion(val);
        else if (key == "filtercriterion")
          newStatData.setFilterCriterion(val);
        else if (key == "statcall")
        {
          QString enumVal = val.toLower();
          if (enumLookup.contains(enumVal))
            newStatData.setStatCall(enumLookup[enumVal]);
          else
            throw HootException("Invalid stats data field '" + val.toStdString() + "' in " + _statsFileName.toStdString());
        }
        else
          throw HootException("Invalid stats data field '" + val.toStdString() + "' in " + _statsFileName.toStdString());
      }
      pCurr->append(newStatData);
    }
  }
}

MatchCreatorPtr CalculateStatsOp::_getMatchCreator(const vector<MatchCreatorPtr>& matchCreators,
                                                   const QString& matchCreatorName,
                                                   CreatorDescription::BaseFeatureType& featureType) const
{
  for (auto matchIt = matchCreators.begin(); matchIt != matchCreators.end(); ++matchIt)
  {
    vector<CreatorDescription> desc = (*matchIt)->getAllCreators();
    for (auto descIt = desc.begin(); descIt != desc.end(); ++descIt)
    {
      QString testName = descIt->getClassName();
      LOG_VART(testName);
      if (0 == matchCreatorName.compare(testName))
      {
        featureType = descIt->getBaseFeatureType();
        LOG_VART(featureType);
        return *matchIt;
      }
    }
  }
  return MatchCreatorPtr(); // empty if not found
}

void CalculateStatsOp::_initConflatableFeatureCounts()
{
  for (auto ft = CreatorDescription::POI;
       ft < CreatorDescription::Unknown;
       ft = CreatorDescription::BaseFeatureType(ft + 1))
  {
    _conflatableFeatureCounts[ft] = 0.0;
  }
}

int CalculateStatsOp::_getNumStatsPassingFilter(const QList<StatData>& stats) const
{
  int numPassingFilter = 0;
  for (const auto& stat : stats)//QList<StatData>::const_iterator it = stats.begin(); it != stats.end(); ++it)
  {
    if (_statPassesFilter(stat))
      numPassingFilter++;
  }
  return numPassingFilter;
}

void CalculateStatsOp::_initStatCalc()
{
  // for debugging only
  _numInterpresetStatDataCalls = 0;
  _numGenerateFeatureStatCalls = 0;

  // Basically, we're trying to count the total calls to _applyVisitor here, which results in a
  // complete pass over the data. Technically the number of stats is larger, since each
  // _applyVisitor call may fuel multiple stats. However, for status reporting purposes we're more
  // concerned with the processing time incurred by _applyVisitor, and the status is easier to
  // update doing it this way. Admittedly, the way the stat total count is being calculated here is
  // a very brittle way to do it but haven't come up with a better way yet. This also could
  // eventually be tied in with progress reporting.

  // TODO: These total stat count is off by a bit (consistently, at least). When run from the stats
  // command, the total is 4 higher than when it should be. When run from the conflate command the
  // total is 2 higher than it should be.

  const int numQuickStatCalcs = _getNumStatsPassingFilter(_quickStatData);
  LOG_VARD(numQuickStatCalcs);
  int numSlowStatCalcs = 0;
  // number of quick stat calcs, each one calling _applyVisitor once
  _totalStatCalcs = numQuickStatCalcs;

  if (!_quick)
  {
    LOG_VARD(_slowStatData.size());
    // number of slow stat calcs, each one calling _applyVisitor once
    numSlowStatCalcs = _getNumStatsPassingFilter(_slowStatData);

    // the number of calls to _applyVisitor outside of a loop and always called
    numSlowStatCalcs += 8;

    // the number of calls to _applyVisitor made during translated tag calc
    if (ConfigOptions().getStatsTranslateScript() != "")
      numSlowStatCalcs += 4;

    LOG_VARD(_inputIsConflatedMapOutput);
    if (_filter.isEmpty() ||
         (_filter.contains(PoiPolygonPoiCriterion::className()) &&
          _filter.contains(PoiPolygonPolyCriterion::className())))
    {
      // extra calls to _applyVisitor made for poi/poly only and for all types of maps
      numSlowStatCalcs += 3;

      // extra calls to _applyVisitor made for poi/poly only and only for input maps
      if (!_inputIsConflatedMapOutput)
        numSlowStatCalcs += 2;
    }

    LOG_VARD(MatchFactory::getInstance().getCreators().size());
    // extra calls to _applyVisitor made for each feature type by _generateFeatureStats
    int numCallsToGenerateFeatureStats = 0;
    int numSizeCalcs = 0;
    for (auto it = _conflatableFeatureCounts.begin(); it != _conflatableFeatureCounts.end(); ++it)
    {
      CreatorDescription::BaseFeatureType featureType = it.key();
      const QString featureTypeStr = CreatorDescription::baseFeatureTypeToString(featureType);
      if (!_featureTypesToSkip.contains(featureTypeStr, Qt::CaseInsensitive) &&
          (_filter.isEmpty() ||
           _filter.contains(CreatorDescription::getElementCriterionName(featureType))))
      {
        numCallsToGenerateFeatureStats++;

        const CreatorDescription::FeatureCalcType calcType =
          CreatorDescription::getFeatureCalcType(featureType);
        if (calcType == CreatorDescription::CalcTypeArea || calcType == CreatorDescription::CalcTypeLength)
          numSizeCalcs += 4;
      }
    }
    LOG_VARD(numCallsToGenerateFeatureStats);
    // _generateFeatureStats calls _applyVisitor multiple times for each feature type.
    numSlowStatCalcs += (7 * numCallsToGenerateFeatureStats);
    // These _applyVisitor calls are made in _generateFeatureStats but are conditional on feature
    // type, so computing the total separately
    numSlowStatCalcs += numSizeCalcs;

    _totalStatCalcs += numSlowStatCalcs;
  }
  LOG_VARD(numSlowStatCalcs);
  LOG_VARD(_totalStatCalcs);

  _currentStatCalcIndex = 1;
}

void CalculateStatsOp::apply(const OsmMapPtr& map)
{
  // We don't want to auto calc search radius unless conflating, b/c it could result in a somewhat
  // expensive rubbersheeting operation. Changing the config glocally works here b/c stats are
  // either calculated by themselves or after a conflation job. As more script conflators implement
  // a search radius function vs using a predefined value, this list could get a little unruly.
  conf().set(ConfigOptions::getPowerLineAutoCalcSearchRadiusKey(), false);
  conf().set(ConfigOptions::getRiverAutoCalcSearchRadiusKey(), false);

  _initStatCalc();

  QString logMsg = "Performing " + QString::number(_totalStatCalcs) + " map statistic calculations";
  if (!_mapName.isEmpty())
    logMsg += " for " + _mapName;
  else if (!map->getName().isEmpty())
    logMsg += " for " + map->getName();
  logMsg += "...";
  LOG_STATUS(logMsg);

  MapProjector::projectToPlanar(map);

  _constMap = map;

  for (const auto& d : _quickStatData)
    _interpretStatData(_constMap, d);

  if (!_quick)
  {
    for (const auto& d : _slowStatData)
      _interpretStatData(_constMap, d);
  }

  if (!_quick)
  {
    // There are some stats here that could be moved to the generic stats file if they weren't
    // needed to calculate derivative stats. In order to prevent calculating them more than once,
    // they haven't been moved to the generic stats.

    const double featureCount = _getApplyVisitor(std::make_shared<FeatureCountVisitor>(), "Total Features");
    _addStat("Total Features", featureCount);

    double conflatedFeatureCount =
      _applyVisitor(
        std::make_shared<StatusCriterion>(Status::Conflated),
        std::make_shared<FeatureCountVisitor>(), "Conflated Feature Count");

    // We're tailoring the stats to whether the map being examined is the input to a conflation job
    // or the output from a conflation job. When the stats option is called from the conflate
    // command, this is accomplished by allowing the conflate command to notify this op which kind
    // of map is being examined. When the stats command is called by itself there is no mechanism
    // for doing that, so we're assuming if there are any conflated features in the map that the
    // map should be considered the output of a conflation job. This logic, of course, breaks down
    // if the same data is conflated more than once. In that case, it may be wise to let the stats
    // command pass in a variable stating whether the map is input/output, like the conflate command
    // does.
    if (conflatedFeatureCount > 0)
      _inputIsConflatedMapOutput = true;

    vector<MatchCreatorPtr> matchCreators = MatchFactory::getInstance().getCreators();
    boost::any matchCandidateCountsData;
    double conflatableFeatureCount =
      _applyVisitor(
        FilteredVisitor(
          std::make_shared<ChainCriterion>(
            std::make_shared<NotCriterion>(std::make_shared<StatusCriterion>(Status::Conflated)),
            std::make_shared<NotCriterion>(std::make_shared<NeedsReviewCriterion>(_constMap))),
          std::make_shared<MatchCandidateCountVisitor>(matchCreators)),
        matchCandidateCountsData,
        "Conflatable Feature Count");
    LOG_VARD(matchCreators.size());

    double poisMergedIntoPolys = 0.0;
    double poisMergedIntoPolysFromMap1 = 0.0;
    double poisMergedIntoPolysFromMap2 = 0.0;
    if (_filter.isEmpty() ||
        (_filter.contains(PoiPolygonPoiCriterion::className()) &&
         _filter.contains(PoiPolygonPolyCriterion::className())))
    {
      poisMergedIntoPolys =
        _getApplyVisitor(
          std::make_shared<SumNumericTagsVisitor>(
            QStringList(MetadataTags::HootPoiPolygonPoisMerged())),
          "POIs Merged Into Polygons");
      poisMergedIntoPolysFromMap1 =
        _getApplyVisitor(
          std::make_shared<SumNumericTagsVisitor>(
            QStringList(MetadataTags::HootPoiPolygonPoisMerged1())),
          "Map 1 POIs Merged Into Polygons");
      poisMergedIntoPolysFromMap2 =
        _getApplyVisitor(
          std::make_shared<SumNumericTagsVisitor>(
            QStringList(MetadataTags::HootPoiPolygonPoisMerged2())),
          "Map 2 POIs Merged Into Polygons");
      // We need to add any pois that may have been merged into polygons by poi/poly into the total
      // Conflated feature count.
      conflatedFeatureCount += poisMergedIntoPolys;
    }

    // The conflatable stat has no meaning on a conflated output map, since everything in the
    // output is either conflated, passed through, marked for review, or left unconflated.
    if (_inputIsConflatedMapOutput)
      conflatableFeatureCount = 0.0;
    _addStat("Total Conflatable Features", conflatableFeatureCount);
    _addStat("Percentage of Total Features Conflatable",
             (conflatableFeatureCount / featureCount) * 100.0);
    const double numFeaturesMarkedForReview =
      _applyVisitor(
        std::make_shared<NeedsReviewCriterion>(_constMap), std::make_shared<FeatureCountVisitor>(),
        "Reviewable Feature Count");

    const double numReviewsToBeMade =
      _getApplyVisitor(std::make_shared<CountUniqueReviewsVisitor>(), "Number of Reviews");

    const double untaggedFeatureCount =
      _applyVisitor(
        std::make_shared<NoInformationCriterion>(), std::make_shared<FeatureCountVisitor>(),
        "No Information Feature Count");
    _addStat("Untagged Features", untaggedFeatureCount);
    double unconflatableFeatureCount = -1.0;
    if (!_inputIsConflatedMapOutput)
      unconflatableFeatureCount = fmax((featureCount - untaggedFeatureCount - conflatableFeatureCount), 0.0);
    else  // This stat has no meaning on a conflated output map (see total feature conflatable stat comment).
      unconflatableFeatureCount = 0.0;

    _addStat("Total Unconflatable Features", unconflatableFeatureCount);
    _addStat("Percentage of Total Features Unconflatable",
             (unconflatableFeatureCount / featureCount) * 100.0);

    _addStat("Match Creators", static_cast<double>(matchCreators.size()));

    const QMap<QString, long> matchCandidateCountsByMatchCreator =
      boost::any_cast<QMap<QString, long>>(matchCandidateCountsData);
    LOG_VARD(matchCandidateCountsByMatchCreator.size());
    LOG_VARD(matchCandidateCountsByMatchCreator);
    for (auto it = matchCandidateCountsByMatchCreator.begin(); it != matchCandidateCountsByMatchCreator.end(); ++it)
    {
      const QString matchCreatorName = it.key();
      LOG_VARD(matchCreatorName);
      CreatorDescription::BaseFeatureType featureType = CreatorDescription::Unknown;
      /*shared_ptr<MatchCreator> matchCreator =*/
      _getMatchCreator(matchCreators, it.key(), featureType);

      double conflatableFeatureCountForFeatureType = 0.0;
      if (!_inputIsConflatedMapOutput)
      {
        conflatableFeatureCountForFeatureType = static_cast<double>(matchCandidateCountsByMatchCreator[matchCreatorName]);
        LOG_VARD(conflatableFeatureCountForFeatureType);
      }

      // Poi/poly conflation is a special case in that it conflates two different types of features,
      // so it must be treated with different logic than the rest of the single feature type
      // conflation logic here and in MatchCandidateCountVisitor.
      if (featureType == CreatorDescription::Polygon)
      {
        double conflatablePolyCount = 0.0;
        if (!_inputIsConflatedMapOutput)
        {
          // See comment in _generateFeatureStats as to why ElementCountVisitor is used here
          // instead of FeatureCountVisitor.
          conflatablePolyCount =
            _applyVisitor(
              std::make_shared<PoiPolygonPolyCriterion>(), std::make_shared<ElementCountVisitor>(),
              "Conflatable Polygon Count");
        }
        _addStat("Polygons Conflatable by: " + matchCreatorName, conflatablePolyCount);
        _conflatableFeatureCounts[CreatorDescription::Polygon] += conflatablePolyCount;

        double conflatablePoiPolyPoiCount = 0.0;
        if (!_inputIsConflatedMapOutput)
        {
          // See comment in _generateFeatureStats as to why ElementCountVisitor is used here
          // instead of FeatureCountVisitor.
          conflatablePoiPolyPoiCount =
            _applyVisitor(
              std::make_shared<PoiPolygonPoiCriterion>(), std::make_shared<ElementCountVisitor>(),
              "Conflatable POI/Polygon Count");
        }
        _addStat("POIs Conflatable by: " + matchCreatorName, conflatablePoiPolyPoiCount);
        _conflatableFeatureCounts[CreatorDescription::PoiPolygonPOI] += conflatablePoiPolyPoiCount;
      }
      _addStat(
        "Features Conflatable by: " + matchCreatorName, conflatableFeatureCountForFeatureType);
      _conflatableFeatureCounts[featureType] += conflatableFeatureCountForFeatureType;
    }

    _addStat("Total Conflated Features", conflatedFeatureCount);
    _addStat("Percentage of Total Features Conflated",
              (conflatedFeatureCount / featureCount) * 100.0);
    _addStat("Total Features Marked for Review", numFeaturesMarkedForReview);
    _addStat("Percentage of Total Features Marked for Review",
             (numFeaturesMarkedForReview / featureCount) * 100.0);
    _addStat("Total Reviews to be Made", numReviewsToBeMade);
    const double unconflatedFeatureCountFromMap1 =
      _applyVisitor(
        std::make_shared<StatusCriterion>(Status::Unknown1),
        std::make_shared<FeatureCountVisitor>(), "Unconflated Feature Count Map 1");
    const double unconflatedFeatureCountFromMap2 =
      _applyVisitor(
        std::make_shared<StatusCriterion>(Status::Unknown2),
        std::make_shared<FeatureCountVisitor>(), "Unconflated Feature Count Map 2");
    const double totalUnconflatedFeatureCount =
      _applyVisitor(
        std::make_shared<NotCriterion>(std::make_shared<StatusCriterion>(Status::Conflated)),
        std::make_shared<FeatureCountVisitor>(), "Unconflated Feature Count");
    _addStat("Total Unmatched Features", totalUnconflatedFeatureCount);
    _addStat("Percentage of Total Features Unmatched",
             (totalUnconflatedFeatureCount / featureCount) * 100.0);
    _addStat("Total Unmatched Features From Map 1", unconflatedFeatureCountFromMap1);
    _addStat("Percentage of Total Features Unmatched From Map 1",
             (unconflatedFeatureCountFromMap1 / featureCount) * 100.0);
    _addStat("Total Unmatched Features From Map 2", unconflatedFeatureCountFromMap2);
    _addStat("Percentage of Total Features Unmatched From Map 2",
             (unconflatedFeatureCountFromMap2 / featureCount) * 100.0);

    for (auto it = _conflatableFeatureCounts.begin(); it != _conflatableFeatureCounts.end(); ++it)
    {
      CreatorDescription::BaseFeatureType featureType = it.key();
      const QString featureTypeStr = CreatorDescription::baseFeatureTypeToString(featureType);
      if (!_featureTypesToSkip.contains(featureTypeStr, Qt::CaseInsensitive) &&
          (_filter.isEmpty() ||
           _filter.contains(CreatorDescription::getElementCriterionName(featureType))))
      {
        _generateFeatureStats(featureType, static_cast<float>(it.value()),
                              CreatorDescription::getFeatureCalcType(featureType),
                              CreatorDescription::getElementCriterion(featureType, map),
                              static_cast<long>(poisMergedIntoPolys),
                              static_cast<long>(poisMergedIntoPolysFromMap1),
                              static_cast<long>(poisMergedIntoPolysFromMap2));
      }
    }

    LOG_DEBUG("config script: " + ConfigOptions().getStatsTranslateScript());
    if (ConfigOptions().getStatsTranslateScript() != "")
    {
      if (!_schemaTranslator)
      {
        assert(!ConfigOptions().getStatsTranslateScript().isEmpty());
        _schemaTranslator =
          ScriptSchemaTranslatorFactory::getInstance().createTranslator(
            ConfigOptions().getStatsTranslateScript());
        _schemaTranslator->setErrorTreatment(StrictOff);
      }
      SchemaTranslatedTagCountVisitor tcv(_schemaTranslator);
      _applyVisitor(&tcv, "Translated Tag Count");
      // This stat could be made generic, but since the ones following it cannot currently we'll
      // leave it non-generic so it shows up next to them in the list.
      _addStat("Translated Populated Tag Percent", tcv.getStat());
      _addStat("Translated Populated Tags", static_cast<double>(tcv.getPopulatedCount()));
      _addStat("Translated Default Tags", static_cast<double>(tcv.getDefaultCount()));
      _addStat("Translated Null Tags", static_cast<double>(tcv.getNullCount()));
    }
    else
    {
      LOG_DEBUG("Skipping stats translation");
    }
  }

  logMsg = QString::number(_totalStatCalcs) + " map statistics calculated";
  if (!_mapName.isEmpty())
    logMsg += " for " + _mapName;
  logMsg += ".";
  LOG_DEBUG(logMsg);

  LOG_VARD(_numInterpresetStatDataCalls);
  LOG_VARD(_numInterpretStatVisCacheHits);
  LOG_VARD(_numGenerateFeatureStatCalls);
}

void CalculateStatsOp::_addStat(const QString& name, double value)
{
  _stats.append(SingleStat(name, value));
}

bool CalculateStatsOp::_statPassesFilter(const StatData& statData) const
{
  return _filter.isEmpty() || statData.getFilterCriterion().isEmpty() ||
         _filter.contains(statData.getFilterCriterion());
}

void CalculateStatsOp::_interpretStatData(const shared_ptr<const OsmMap>& constMap, const StatData& d)
{
  // Check against the applied filter to see whether this stat should be generated.
  LOG_VART(_filter);
  LOG_VART(d.getFilterCriterion());
  if (!_statPassesFilter(d))
    return;

  // Don't collect translation related stats if no translation script has been specified.
  if (d.getVisitor() == SchemaTranslatedTagCountVisitor::className() &&
      ConfigOptions().getStatsTranslateScript().isEmpty())
  {
    return;
  }

  ElementCriterionPtr pCrit;

  if (d.getCriterion().length() > 0)
  {
    if (_criterionCache.contains(d.getCriterion()))
      pCrit = _criterionCache[d.getCriterion()];
    else
    {
      try
      {
        // create criterion
        pCrit = Factory::getInstance().constructObject<ElementCriterion>(d.getCriterion());
        // make sure the map is set before we use it
        ConstOsmMapConsumer* pMapConsumer = dynamic_cast<ConstOsmMapConsumer*>(pCrit.get());
        if (pMapConsumer)
          pMapConsumer->setOsmMap(constMap.get());
      }
      catch (...)
      {
        throw HootException(
          QString("Unable to construct criterion '%1' in stats data entry '%2'")
            .arg(d.getCriterion(), d.getName()) );
      }

      _criterionCache[d.getCriterion()] = pCrit;
    }
  }

  if (d.getVisitor().length() > 0)
  {
    double val = 0;

    if (pCrit)
    {
      // if a criterion is specified, we apply a FilteredVisitor
      // this is not cached as there is only one value and it's unlikely to be repeated
      ConstElementVisitorPtr pCriterionVisitor;

      try
      {
        // create visitor to set in FilteredVisitor
        pCriterionVisitor =
          std::static_pointer_cast<ConstElementVisitor>(
            Factory::getInstance().constructObject<ElementVisitor>(d.getVisitor()));
      }
      catch (...)
      {
        throw HootException(
          QString("Unable to construct visitor '%1' in stats data entry '%2'")
            .arg(d.getVisitor(), d.getName()) );
      }

      if (d.getVisitor() == SchemaTranslatedTagCountVisitor::className())
      {
        if (!_schemaTranslator)
        {
          assert(!ConfigOptions().getStatsTranslateScript().isEmpty());
          _schemaTranslator =
            ScriptSchemaTranslatorFactory::getInstance().createTranslator(
              ConfigOptions().getStatsTranslateScript());
          _schemaTranslator->setErrorTreatment(StrictOff);
        }
        shared_ptr<SchemaTranslatedTagCountVisitor> translatedVis =
          std::dynamic_pointer_cast<SchemaTranslatedTagCountVisitor>(pCriterionVisitor);
        translatedVis->setTranslator(_schemaTranslator);
      }

      FilteredVisitor filteredVisitor(pCrit, ConstElementVisitorPtr(pCriterionVisitor));
      val = _applyVisitor(filteredVisitor, d.getName(), d.getStatCall());
      _numInterpresetStatDataCalls++;
    }
    else
    {
      shared_ptr<ConstElementVisitor> pVisitor;

      if (_appliedVisitorCache.contains(d.getVisitor()))
      {  
        pVisitor = _appliedVisitorCache[d.getVisitor()];

        // Even though this is cached, and its a freebie runtime-wise, we'll still log status to
        // ensure the stat calc index remains acurate.
        LOG_STATUS(
          "Calculating statistic: " << d.getName() << " (" << _currentStatCalcIndex << "/" <<
          _totalStatCalcs << ") ...");

        _currentStatCalcIndex++;
        _numInterpretStatVisCacheHits++;
      }
      else
      {
        try
        {
          pVisitor =
            std::static_pointer_cast<ConstElementVisitor>(
              Factory::getInstance().constructObject<ElementVisitor>(d.getVisitor()));
        }
        catch (...)
        {
          throw HootException(
            QString("Unable to construct visitor '%1' in stats data entry '%2'")
              .arg(d.getVisitor(), d.getName()));
        }

        // without criterion, apply the visitor directly and interpret as NumericStatistic
        _applyVisitor(pVisitor.get(), d.getName());
        _appliedVisitorCache[d.getVisitor()] = pVisitor;
        _numInterpresetStatDataCalls++;
      }

      val = _getRequestedStatValue(pVisitor.get(), d.getStatCall());
    }

    _addStat(d.getName(), val);
  }
}

double CalculateStatsOp::_getRequestedStatValue(const ElementVisitor* pVisitor, StatData::StatCall call) const
{
  if (call == StatData::StatCall::Stat)
  {
    const SingleStatistic* ss = dynamic_cast<const SingleStatistic*>(pVisitor);
    return ss ? ss->getStat() : 0;
  }

  const NumericStatistic* ns = dynamic_cast<const NumericStatistic*>(pVisitor);

  if (!ns)
    return 0;

  switch (call)
  {
    case StatData::StatCall::Stat:        return ns->getStat();
    case StatData::StatCall::Min:         return ns->getMin();
    case StatData::StatCall::Max:         return ns->getMax();
    case StatData::StatCall::Average:     return ns->getAverage();
    case StatData::StatCall::InfoCount:   return static_cast<double>(ns->getInformationCount());
    case StatData::StatCall::InfoMin:     return static_cast<double>(ns->getInformationMin());
    case StatData::StatCall::InfoMax:     return static_cast<double>(ns->getInformationMax());
    case StatData::StatCall::InfoAverage: return ns->getInformationAverage();
    case StatData::StatCall::InfoDiff:    return static_cast<double>(ns->getInformationCountDiff());
    default:                              return 0;
  }
}

bool CalculateStatsOp::_matchDescriptorCompare(const CreatorDescription& m1, const CreatorDescription& m2)
{
  return m1.getClassName() > m2.getClassName();
}

double CalculateStatsOp::_applyVisitor(const ElementCriterion& pCrit, ElementVisitor& pVis,
                                       const QString& statName, StatData::StatCall call)
{
  return _applyVisitor(FilteredVisitor(pCrit, pVis), statName, call);
}

double CalculateStatsOp::_applyVisitor(const ElementCriterionPtr& pCrit, ConstElementVisitorPtr pVis,
                                       const QString& statName, StatData::StatCall call)
{
  return _applyVisitor(FilteredVisitor(pCrit, pVis), statName, call);
}

double CalculateStatsOp::_applyVisitor(const FilteredVisitor& v, const QString& statName, StatData::StatCall call)
{
  boost::any emptyVisitorData;
  return _applyVisitor(v, emptyVisitorData, statName, call);
}

double CalculateStatsOp::_applyVisitor(const FilteredVisitor& v, boost::any& visitorData, const QString& statName,
                                       StatData::StatCall call)
{
  LOG_STATUS(
    "Calculating statistic: " << statName << " (" << _currentStatCalcIndex << "/" <<
    _totalStatCalcs << ") ...");

  // this is a hack to let C++ pass v as a temporary
  FilteredVisitor* fv = const_cast<FilteredVisitor*>(&v);
  shared_ptr<FilteredVisitor> critFv;
  if (_criterion)
  {
    critFv = std::make_shared<FilteredVisitor>(*_criterion, *fv);
    fv = critFv.get();
  }

  ElementVisitor& childVisitor = v.getChildVisitor();

  // There's a possible optimization that could be made here (and in the _applyVisitor below) to
  // check the criterion used by FilteredVisitor, and if it inherits from GeometryTypeCriterion
  // then call visitWaysRo, etc. Not sure if its worth the effort, though.

  _constMap->visitRo(*fv);

  const DataProducer* dataProducer = dynamic_cast<DataProducer*>(&childVisitor);
  if (dataProducer != nullptr)
    visitorData = dataProducer->getData();

  _currentStatCalcIndex++;
  return _getRequestedStatValue(&childVisitor, call);
}

void CalculateStatsOp::_applyVisitor(ConstElementVisitorPtr v, const QString& statName)
{
  LOG_STATUS(
    "Calculating statistic: " << statName << " (" << _currentStatCalcIndex << "/" <<
    _totalStatCalcs << ") ...");

  shared_ptr<FilteredVisitor> critFv;
  if (_criterion)
  {
    critFv = std::make_shared<FilteredVisitor>(*_criterion, *v);
    v = critFv;
  }
  _constMap->visitRo(*v);

  _currentStatCalcIndex++;
}

void CalculateStatsOp::_applyVisitor(ConstElementVisitor* v, const QString& statName)
{
  LOG_STATUS(
    "Calculating statistic: " << statName << " (" << _currentStatCalcIndex << "/" <<
    _totalStatCalcs << ") ...");

  shared_ptr<FilteredVisitor> critFv;
  if (_criterion)
  {
    critFv = std::make_shared<FilteredVisitor>(*_criterion, *v);
    v = critFv.get();
  }
  _constMap->visitRo(*v);

  _currentStatCalcIndex++;
}

double CalculateStatsOp::_getApplyVisitor(ConstElementVisitorPtr v, const QString& statName)
{
  _applyVisitor(v, statName);
  std::shared_ptr<SingleStatistic> ss = std::dynamic_pointer_cast<SingleStatistic>(v);
  if (!ss)
    throw HootException(v->getName() + " does not implement SingleStatistic.");

  return ss->getStat();
}

double CalculateStatsOp::getSingleStat(const QString& n) const
{
  for (const auto& stat : _stats)
  {
    if (stat.name == n)
      return stat.value;
  }
  throw InternalErrorException("Could not find the specified stat: " + n);
}

bool CalculateStatsOp::hasSingleStat(const QString& n) const
{
  for (const auto& stat : _stats)
  {
    if (stat.name == n)
      return true;
  }
  return false;
}

long CalculateStatsOp::indexOfSingleStat(const QString& n) const
{
  for (int i = 0; i < _stats.size(); i++)
  {
    if (_stats[i].name == n)
      return i;
  }
  return -1;
}

ConstElementVisitorPtr CalculateStatsOp::_getElementVisitorForFeatureType(const CreatorDescription::BaseFeatureType& featureType) const
{
  // Poi/poly doesn't have the restriction that an element have a tag information count > 0 to
  // be considered for conflation.
  if (featureType == CreatorDescription::PoiPolygonPOI || featureType == CreatorDescription::Polygon)
    return std::make_shared<ElementCountVisitor>();
  else
    return std::make_shared<FeatureCountVisitor>();
}

void CalculateStatsOp::_generateFeatureStats(const CreatorDescription::BaseFeatureType& featureType, const float conflatableCount,
                                             const CreatorDescription::FeatureCalcType& type, ElementCriterionPtr criterion,
                                             const long poisMergedIntoPolys, const long poisMergedIntoPolysFromMap1,
                                             const long poisMergedIntoPolysFromMap2)
{
  LOG_VARD(criterion->className());
  LOG_VARD(poisMergedIntoPolys);
  const QString description = CreatorDescription::baseFeatureTypeToString(featureType);
  LOG_VARD(description);
  LOG_VARD(type);

  ConstOsmMapConsumer* mapConsumer = dynamic_cast<ConstOsmMapConsumer*>(criterion.get());
  if (mapConsumer != nullptr)
    mapConsumer->setOsmMap(_constMap.get());

  double totalFeatures = 0.0;
  totalFeatures =
    _applyVisitor(
      FilteredVisitor(criterion->clone(), _getElementVisitorForFeatureType(featureType)),
      "Total Feature Count: " + description);
  LOG_VARD(totalFeatures);
  _addStat(QString("%1s").arg(description), totalFeatures);
  _addStat(QString("Conflatable %1s").arg(description), conflatableCount);

  double conflatedFeatureCount =
    _applyVisitor(
      FilteredVisitor(
        std::make_shared<ChainCriterion>(
          std::make_shared<StatusCriterion>(Status::Conflated), criterion->clone()),
      _getElementVisitorForFeatureType(featureType)), "Conflated Feature Count: " + description);
  LOG_VARD(conflatedFeatureCount);
  if (featureType == CreatorDescription::PoiPolygonPOI)
  {
    // We need to add any pois that may have been merged into polygons by poi/poly into the
    // conflated feature count for this feature type.
    conflatedFeatureCount += static_cast<double>(poisMergedIntoPolys);
    LOG_VARD(conflatedFeatureCount);
  }

  const double featuresMarkedForReview =
    _applyVisitor(
      FilteredVisitor(
        std::make_shared<ChainCriterion>(
          std::make_shared<NeedsReviewCriterion>(_constMap),
          criterion->clone()),
      _getElementVisitorForFeatureType(featureType)), "Reviewable Feature Count: " + description);
  _addStat(QString("Conflated %1s").arg(description), conflatedFeatureCount);
  _addStat(QString("%1s Marked for Review").arg(description), featuresMarkedForReview);
  const double numFeatureReviewsToBeMade =
    _applyVisitor(
       FilteredVisitor(
         criterion->clone(),
         std::make_shared<CountUniqueReviewsVisitor>()),
      "Reviews To Be Made Count: " + description);
  _addStat(QString("%1 Reviews to be Made").arg(description), numFeatureReviewsToBeMade);

  double unconflatedFeatureCountMap1 =
    _applyVisitor(
      FilteredVisitor(
        std::make_shared<ChainCriterion>(
          std::make_shared<StatusCriterion>(Status::Unknown1),
          criterion->clone()),
      _getElementVisitorForFeatureType(featureType)),
      "Unconflated Feature Count Map 1: " + description);
  LOG_VARD(unconflatedFeatureCountMap1);
  double unconflatedFeatureCountMap2 =
    _applyVisitor(
      FilteredVisitor(
        std::make_shared<ChainCriterion>(
          std::make_shared<StatusCriterion>(Status::Unknown2),
          criterion->clone()),
      _getElementVisitorForFeatureType(featureType)),
      "Unconflated Feature Count Map 2: " + description);
  LOG_VARD(unconflatedFeatureCountMap2);
  double totalUnconflatedFeatureCount =
    _applyVisitor(
      FilteredVisitor(
        std::make_shared<ChainCriterion>(
          std::make_shared<NotCriterion>(std::make_shared<StatusCriterion>(Status::Conflated)),
          criterion->clone()),
      _getElementVisitorForFeatureType(featureType)),
      "Total Unconflated Feature Count: " + description);
  LOG_VARD(totalUnconflatedFeatureCount);
  if (featureType == CreatorDescription::PoiPolygonPOI)
  {
    // We need to subtract any pois that may have been merged into polygons by poi/poly from the
    // unconflated feature count for this feature type.
    totalUnconflatedFeatureCount -= static_cast<double>(poisMergedIntoPolys);
    LOG_VARD(totalUnconflatedFeatureCount);
    unconflatedFeatureCountMap1 -= static_cast<double>(poisMergedIntoPolysFromMap1);
    LOG_VARD(unconflatedFeatureCountMap1);
    unconflatedFeatureCountMap2 -= static_cast<double>(poisMergedIntoPolysFromMap2);
    LOG_VARD(unconflatedFeatureCountMap2);
  }
  _addStat(QString("Unmatched %1s").arg(description), totalUnconflatedFeatureCount);
  _addStat(QString("Unmatched %1s From Map 1").arg(description), unconflatedFeatureCountMap1);
  _addStat(QString("Unmatched %1s From Map 2").arg(description), unconflatedFeatureCountMap2);

  if (type == CreatorDescription::CalcTypeLength)
  {
    _addStat(QString("Total Meters of %1s").arg(description),
      _applyVisitor(
        FilteredVisitor(
          criterion->clone(),
          std::make_shared<LengthOfWaysVisitor>()),
        "Total Meters: " + description));
    _addStat(QString("Meters of Conflated %1s").arg(description),
      _applyVisitor(
        FilteredVisitor(
          std::make_shared<ChainCriterion>(
            std::make_shared<StatusCriterion>(Status::Conflated), criterion->clone()),
          std::make_shared<LengthOfWaysVisitor>()),
        "Meters Conflated: " + description));
    _addStat(QString("Meters of Unmatched %1s From Map 1").arg(description),
      _applyVisitor(
        FilteredVisitor(
          std::make_shared<ChainCriterion>(
            std::make_shared<StatusCriterion>(Status::Unknown1), criterion->clone()),
          std::make_shared<LengthOfWaysVisitor>()),
        "Meters Unmatched Map 1: " + description));
    _addStat(QString("Meters of Unmatched %1s From Map 2").arg(description),
      _applyVisitor(
        FilteredVisitor(
          std::make_shared<ChainCriterion>(
            std::make_shared<StatusCriterion>(Status::Unknown2), criterion->clone()),
          std::make_shared<LengthOfWaysVisitor>()),
        "Meters Unmatched Map 2: " + description));
    _numGenerateFeatureStatCalls++;
  }
  else if (type == CreatorDescription::CalcTypeArea)
  {
    _addStat(QString("Total Square Meters of %1s").arg(description),
      _applyVisitor(
        FilteredVisitor(criterion->clone(), std::make_shared<LengthOfWaysVisitor>()),
        "Total Square Meters: " + description));
    _addStat(QString("Square Meters of Conflated %1s").arg(description),
      _applyVisitor(
        FilteredVisitor(
          std::make_shared<ChainCriterion>(
            std::make_shared<StatusCriterion>(Status::Conflated), criterion->clone()),
          std::make_shared<CalculateAreaForStatsVisitor>()),
        "Area Conflated: " + description));
    _addStat(QString("Square Meters of Unmatched %1s From Map 1").arg(description),
      _applyVisitor(
        FilteredVisitor(
          std::make_shared<ChainCriterion>(
            std::make_shared<StatusCriterion>(Status::Unknown1), criterion->clone()),
          std::make_shared<CalculateAreaForStatsVisitor>()),
        "Area Unmatched Map 1: " + description));
    _addStat(QString("Square Meters of Unmatched %1s From Map 2").arg(description),
      _applyVisitor(
        FilteredVisitor(
          std::make_shared<ChainCriterion>(
            std::make_shared<StatusCriterion>(Status::Unknown2), criterion->clone()),
          std::make_shared<CalculateAreaForStatsVisitor>()),
        "Area Unmatched Map 2: " + description));
    _numGenerateFeatureStatCalls++;
  }

  double percentageOfTotalFeaturesConflated = 0.0;
  if (totalFeatures > 0.0)
    percentageOfTotalFeaturesConflated = (conflatedFeatureCount / totalFeatures) * 100.0;

  LOG_VARD(percentageOfTotalFeaturesConflated);
  _addStat(QString("Percentage of %1s Conflated").arg(description), percentageOfTotalFeaturesConflated);
  double percentageOfTotalFeaturesMarkedForReview = 0.0;
  if (totalFeatures > 0.0)
    percentageOfTotalFeaturesMarkedForReview = (featuresMarkedForReview / totalFeatures) * 100.0;

  _addStat(
    QString("Percentage of %1s Marked for Review").arg(description),
    percentageOfTotalFeaturesMarkedForReview);
  double percentageOfMap1FeaturesUnconflated = 0.0;
  if (totalFeatures > 0.0)
    percentageOfMap1FeaturesUnconflated = (unconflatedFeatureCountMap1 / totalFeatures) * 100.0;

  LOG_VARD(percentageOfMap1FeaturesUnconflated);
  _addStat(
    QString("Percentage of Unmatched %1s From Map 1").arg(description),
    percentageOfMap1FeaturesUnconflated);
  double percentageOfMap2FeaturesUnconflated = 0.0;
  if (totalFeatures > 0.0)
    percentageOfMap2FeaturesUnconflated = (unconflatedFeatureCountMap2 / totalFeatures) * 100.0;

  LOG_VARD(percentageOfMap2FeaturesUnconflated);
  _addStat(
    QString("Percentage of Unmatched %1s From Map 2").arg(description),
    percentageOfMap2FeaturesUnconflated);
  double percentageOfTotalFeaturesUnconflated = 0.0;
  if (totalFeatures > 0.0)
    percentageOfTotalFeaturesUnconflated = (totalUnconflatedFeatureCount / totalFeatures) * 100.0;

  LOG_VARD(percentageOfTotalFeaturesUnconflated);
  _addStat(
    QString("Percentage of Unmatched %1s").arg(description), percentageOfTotalFeaturesUnconflated);

  _numGenerateFeatureStatCalls += 6;
}

}
