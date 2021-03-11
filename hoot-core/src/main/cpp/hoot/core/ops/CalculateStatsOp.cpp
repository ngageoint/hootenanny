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
#include "CalculateStatsOp.h"

#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/LinearWaterwayCriterion.h>
#include <hoot/core/criterion/NeedsReviewCriterion.h>
#include <hoot/core/criterion/NoInformationCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>
#include <hoot/core/schema/ScriptSchemaTranslator.h>
#include <hoot/core/schema/ScriptSchemaTranslatorFactory.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/visitors/CalculateAreaVisitor.h>
#include <hoot/core/visitors/CountUniqueReviewsVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FeatureCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/LengthOfWaysVisitor.h>
#include <hoot/core/visitors/LongestTagVisitor.h>
#include <hoot/core/visitors/MatchCandidateCountVisitor.h>
#include <hoot/core/visitors/SumNumericTagsVisitor.h>
#include <hoot/core/visitors/SchemaTranslatedTagCountVisitor.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/criterion/PowerLineCriterion.h>
#include <hoot/core/criterion/AreaCriterion.h>

#include <math.h>

using namespace std;
namespace bpt = boost::property_tree;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, CalculateStatsOp)

CalculateStatsOp::CalculateStatsOp(QString mapName, bool inputIsConflatedMapOutput) :
  _pConf(&conf()),
  _mapName(mapName),
  _quick(false),
  _inputIsConflatedMapOutput(inputIsConflatedMapOutput),
  _currentStatCalcIndex(1),
  _totalStatCalcs(0),
  // Unknown does not get us a usable element criterion, so skip it
  _featureTypesToSkip(QStringList("unknown")),
  _numInterpresetStatDataCalls(0),
  _numInterpretStatVisCacheHits(0),
  _numGenerateFeatureStatCalls(0)
{
  _initConflatableFeatureCounts();
  _readGenericStatsData();
}

CalculateStatsOp::CalculateStatsOp(ElementCriterionPtr criterion, QString mapName,
                                   bool inputIsConflatedMapOutput) :
  _pConf(&conf()),
  _criterion(criterion),
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
  LOG_VART(_inputIsConflatedMapOutput);

  _initConflatableFeatureCounts();
  _readGenericStatsData();
}

void CalculateStatsOp::setConfiguration(const Settings& conf)
{
  _pConf = &conf;
}

void CalculateStatsOp::_readGenericStatsData()
{
  // read generic stats from json file
  ConfigOptions opts = ConfigOptions(*_pConf);
  QString statsFileName = opts.getStatsGenericDataFile();
  bpt::ptree propPtree;
  bpt::read_json(statsFileName.toLatin1().constData(), propPtree );

  _quickStatData.clear();
  _slowStatData.clear();
  QList<StatData>* pCurr = nullptr;
  QHash<QString,StatCall> enumLookup({ {"stat", Stat},
                                       {"min", Min},
                                       {"max", Max},
                                       {"average", Average},
                                       {"infocount", InfoCount},
                                       {"infomin", InfoMin},
                                       {"infomax", InfoMax},
                                       {"infoaverage", InfoAverage},
                                       {"infodiff", InfoDiff} });

  foreach (bpt::ptree::value_type listType, propPtree)
  {
    if (listType.first == "quick") pCurr = &_quickStatData;
    else if (listType.first == "slow") pCurr = &_slowStatData;
    else
    {
      throw HootException(
        "Invalid stats data list name '" + listType.first + "' in " + statsFileName.toStdString() +
        ". Allowed: 'quick' or 'slow'.");
    }

    foreach (bpt::ptree::value_type entry, listType.second)
    {
      StatData newStatData;

      foreach (bpt::ptree::value_type data, entry.second)
      {
        QString key = QString::fromStdString(data.first).toLower();
        QString val = QString::fromStdString(data.second.data());

        if (key == "name") newStatData.name = val;
        else if (key == "visitor") newStatData.visitor = val;
        else if (key == "criterion") newStatData.criterion = val;
        else if (key == "statcall")
        {
          QString enumVal = val.toLower();
          if (enumLookup.contains(enumVal))
          {
            newStatData.statCall = enumLookup[enumVal];
          }
          else
          {
            throw HootException(
              "Invalid stats data field '" + val.toStdString() + "' in " +
              statsFileName.toStdString());
          }
        }
        else
        {
          throw HootException(
            "Invalid stats data field '" + val.toStdString() + "' in " +
            statsFileName.toStdString());
        }
      }

      pCurr->append(newStatData);
    }
  }
}

shared_ptr<MatchCreator> CalculateStatsOp::getMatchCreator(
  const vector<shared_ptr<MatchCreator>>& matchCreators,
  const QString& matchCreatorName,
  CreatorDescription::BaseFeatureType& featureType)
{
  for (vector<shared_ptr<MatchCreator>>::const_iterator matchIt = matchCreators.begin();
       matchIt != matchCreators.end(); ++matchIt)
  {
    vector<CreatorDescription> desc = (*matchIt)->getAllCreators();
    for (vector<CreatorDescription>::const_iterator descIt = desc.begin();
         descIt != desc.end(); ++descIt)
    {
      QString testName = descIt->className;
      LOG_VART(testName);
      if (0 == matchCreatorName.compare(testName))
      {
        featureType = descIt->baseFeatureType;
        LOG_VART(featureType);
        return (*matchIt);
      }
    }
  }
  return shared_ptr<MatchCreator>(); // empty if not found
}

void CalculateStatsOp::_initConflatableFeatureCounts()
{
  for (CreatorDescription::BaseFeatureType ft = CreatorDescription::POI;
       ft < CreatorDescription::Unknown;
       ft = CreatorDescription::BaseFeatureType(ft + 1))
  {
    _conflatableFeatureCounts[ft] = 0.0;
  }
}

void CalculateStatsOp::_initStatCalc()
{
  // for debugging only
  _numInterpresetStatDataCalls = 0;
  _numGenerateFeatureStatCalls = 0;

  // Basically, we're trying to count the total calls to _applyVisitor here, which results in a
  // pass over the data. Technically the number of stats is larger, since each _applyVisitor call
  // may fuel multiple stats. However, for status reporting purposes we're more concerned with the
  // processing time incurred by _applyVisitor, and the status is easier to update doing it this
  // way. Admittedly, the way the stat total count is being calculated here is a very brittle way to
  // do it but haven't come up with a better way yet. This also could eventually be tied in with
  // progress reporting.

  const int numQuickStatCalcs = _quickStatData.size();
  LOG_VARD(numQuickStatCalcs);
  int numSlowStatCalcs = 0;
  // number of quick stat calcs, each one calling _applyVisitor once
  _totalStatCalcs = numQuickStatCalcs;

  if (!_quick)
  {
    LOG_VARD(_slowStatData.size());
    // number of slow stat calcs, each one calling _applyVisitor once
    numSlowStatCalcs = _slowStatData.size();

    // the number of calls to _applyVisitor outside of a loop and always called
    numSlowStatCalcs += 11;

    if (ConfigOptions().getStatsTranslateScript() != "")
    {
      // the number of calls to _applyVisitor made during translated tag calc
      numSlowStatCalcs += 10;
    }

    LOG_VARD(_inputIsConflatedMapOutput);
    if (!_inputIsConflatedMapOutput)
    {
      // extra calls to _applyVisitor made for poi/poly only and only for input maps
      numSlowStatCalcs += 2;
    }

    LOG_VARD(MatchFactory::getInstance().getCreators().size());
    // extra calls to _applyVisitor made for each feature type by _generateFeatureStats
    int numCallsToGenerateFeatureStats = 0;
    int numSizeCalcs = 0;
    for (QMap<CreatorDescription::BaseFeatureType, double>::const_iterator it =
           _conflatableFeatureCounts.begin(); it != _conflatableFeatureCounts.end(); ++it)
    {
      CreatorDescription::BaseFeatureType featureTypeEnum = it.key();
      const QString featureType = CreatorDescription::baseFeatureTypeToString(featureTypeEnum);
      if (!_featureTypesToSkip.contains(featureType, Qt::CaseInsensitive))
      {
        numCallsToGenerateFeatureStats++;

        const CreatorDescription::FeatureCalcType calcType =
          CreatorDescription::getFeatureCalcType(featureTypeEnum);
        if (calcType == CreatorDescription::CalcTypeArea ||
            calcType == CreatorDescription::CalcTypeLength)
        {
          numSizeCalcs += 4;
        }
      }
    }
    LOG_VARD(numCallsToGenerateFeatureStats);
    // _generateFeatureStats calls _applyVisitor multiple times for each feature type
    numSlowStatCalcs += (7 * numCallsToGenerateFeatureStats);
    // these _applyVisitor calls are made in _generateFeatureStats but are conditional on feature
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
  // either calculated by themselves or after a conflation job. As more script conflators
  // implement a search radius function vs using a predefined value, this list could get a little
  // unruly.
  conf().set(ConfigOptions::getPowerLineAutoCalcSearchRadiusKey(), false);
  conf().set(ConfigOptions::getWaterwayAutoCalcSearchRadiusKey(), false);

  _initStatCalc();

  QString logMsg = "Performing " + QString::number(_totalStatCalcs) + " map statistic calculations";
  if (!_mapName.isEmpty())
  {
    logMsg += " for " + _mapName;
  }
  else if (!map->getName().isEmpty())
  {
    logMsg += " for " + map->getName();
  }
  logMsg += "...";
  LOG_STATUS(logMsg);

  MapProjector::projectToPlanar(map);

  _constMap = map;

  for (StatData d : _quickStatData) _interpretStatData(_constMap, d);
  if (!_quick) for (StatData d : _slowStatData) _interpretStatData(_constMap, d);

  if (!_quick)
  {
    const long featureCount = _getApplyVisitor(new FeatureCountVisitor(), "Feature Count");
    LOG_VART(featureCount);

    double conflatedFeatureCount =
      _applyVisitor(
        new StatusCriterion(Status::Conflated), new FeatureCountVisitor(),
        "Conflated Feature Count");

    // We're tailoring the stats to whether the map being examined is the input to a conflation job
    // or the output from a conflation job.  When the stats option is called from the conflate
    // command, this is accomplished by allowing the conflate command to notify this op which kind
    // of map is being examined.  When the stats command is called by itself, there is no mechanism
    // for doing that, so we're assuming if there are any conflated features in the map, that the
    // map should be considered an the output of a conflation job.  This logic, of course, breaks
    // down if the same data is conflated more than once.  In that case, it may be wise to let the
    // stats command pass in a variable stating whether the map is input/output, like the conflate
    // command does.
    if (conflatedFeatureCount > 0)
    {
      _inputIsConflatedMapOutput = true;
    }
    vector<shared_ptr<MatchCreator>> matchCreators = MatchFactory::getInstance().getCreators();
    boost::any matchCandidateCountsData;
    double conflatableFeatureCount =
      _applyVisitor(
        FilteredVisitor(
          ChainCriterion(
            ElementCriterionPtr(
              new NotCriterion(new StatusCriterion(Status::Conflated))),
            ElementCriterionPtr(
              new NotCriterion(new NeedsReviewCriterion(_constMap)))),
          ConstElementVisitorPtr(new MatchCandidateCountVisitor(matchCreators))),
        matchCandidateCountsData,
        "Conflatable Feature Count");
    LOG_VARD(matchCreators.size());

    const double poisMergedIntoPolys =
      _getApplyVisitor(
        new SumNumericTagsVisitor(QStringList(MetadataTags::HootPoiPolygonPoisMerged())),
        "POIs Merged Into Polygons");
    const double poisMergedIntoPolysFromMap1 =
      _getApplyVisitor(
        new SumNumericTagsVisitor(QStringList(MetadataTags::HootPoiPolygonPoisMerged1())),
        "Map 1 POIs Merged Into Polygons");
    const double poisMergedIntoPolysFromMap2 =
      _getApplyVisitor(
        new SumNumericTagsVisitor(QStringList(MetadataTags::HootPoiPolygonPoisMerged2())),
        "Map 2 POIs Merged Into Polygons");
    //we need to add any pois that may have been merged into polygons by poi/poly into the total
    //conflated feature count
    conflatedFeatureCount += poisMergedIntoPolys;

    if (_inputIsConflatedMapOutput)
    {
      //The conflatable stat has no meaning on a conflated output map, since everything in the
      //output is either conflated, passed through, marked for review, or left unconflated.
      conflatableFeatureCount = 0.0;
    }
    _addStat("Total Conflatable Features", conflatableFeatureCount);
    _addStat("Percentage of Total Features Conflatable",
             (conflatableFeatureCount / (double)featureCount) * 100.0);
    const double numFeaturesMarkedForReview =
      _applyVisitor(
        new NeedsReviewCriterion(_constMap), new FeatureCountVisitor(), "Reviewable Feature Count");

    const double numReviewsToBeMade =
      _getApplyVisitor(new CountUniqueReviewsVisitor(), "Number of Reviews");

    const double untaggedFeatureCount =
      _applyVisitor(
        new NoInformationCriterion(), new FeatureCountVisitor(), "No Information Feature Count");
    _addStat("Untagged Features", untaggedFeatureCount);
    long unconflatableFeatureCount = -1.0;
    if (!_inputIsConflatedMapOutput)
    {
      unconflatableFeatureCount =
        fmax((featureCount - untaggedFeatureCount - conflatableFeatureCount), (long)0);
    }
    else
    {
      //this stat has no meaning on a conflated output map (see total feature conflatable stat
      //comment)
      unconflatableFeatureCount = 0.0;
    }
    _addStat("Total Unconflatable Features", unconflatableFeatureCount);
    _addStat("Percentage of Total Features Unconflatable",
             ((double)unconflatableFeatureCount / (double)featureCount) * 100.0);

    _addStat("Match Creators", matchCreators.size());

    const QMap<QString, long> matchCandidateCountsByMatchCreator =
      boost::any_cast<QMap<QString, long>>(matchCandidateCountsData);
    LOG_VARD(matchCandidateCountsByMatchCreator.size());
    LOG_VARD(matchCandidateCountsByMatchCreator);
    for (QMap<QString, long >::const_iterator iterator = matchCandidateCountsByMatchCreator.begin();
         iterator != matchCandidateCountsByMatchCreator.end(); ++iterator)
    {
      const QString matchCreatorName = iterator.key();
      LOG_VARD(matchCreatorName);
      CreatorDescription::BaseFeatureType featureType = CreatorDescription::Unknown;
      /*shared_ptr<MatchCreator> matchCreator =*/
        getMatchCreator(matchCreators, iterator.key(), featureType);

      double conflatableFeatureCountForFeatureType = 0.0;
      if (!_inputIsConflatedMapOutput)
      {
        conflatableFeatureCountForFeatureType =
          matchCandidateCountsByMatchCreator[matchCreatorName];
        LOG_VARD(conflatableFeatureCountForFeatureType);
      }

      //Poi/poly is a special case in that it conflates two different types of features, so it must
      //be treated with different logic than the rest of the single feature type conflation logic
      //here and in MatchCandidateCountVisitor.
      if (featureType == CreatorDescription::Polygon)
      {
        double conflatablePolyCount = 0.0;
        if (!_inputIsConflatedMapOutput)
        {
          //see comment in _generateFeatureStats as to why ElementCountVisitor is used here
          //instead of FeatureCountVisitor
          conflatablePolyCount =
            _applyVisitor(
              new PoiPolygonPolyCriterion(), new ElementCountVisitor(),
              "Conflatable Polygon Count");
        }
        _addStat("Polygons Conflatable by: " + matchCreatorName, conflatablePolyCount);
        _conflatableFeatureCounts[CreatorDescription::Polygon] += conflatablePolyCount;

        double conflatablePoiPolyPoiCount = 0.0;
        if (!_inputIsConflatedMapOutput)
        {
          //see comment in _generateFeatureStats as to why ElementCountVisitor is used here
          //instead of FeatureCountVisitor
          conflatablePoiPolyPoiCount =
            _applyVisitor(
              new PoiPolygonPoiCriterion(), new ElementCountVisitor(),
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
              (conflatedFeatureCount / (double)featureCount) * 100.0);
    _addStat("Total Features Marked for Review", numFeaturesMarkedForReview);
    _addStat("Percentage of Total Features Marked for Review",
             (numFeaturesMarkedForReview / (double)featureCount) * 100.0);
    _addStat("Total Reviews to be Made", numReviewsToBeMade);
    const double unconflatedFeatureCountFromMap1 =
      _applyVisitor(
        new StatusCriterion(Status::Unknown1), new FeatureCountVisitor(),
        "Unconflated Feature Count Map 1");
    const double unconflatedFeatureCountFromMap2 =
      _applyVisitor(
        new StatusCriterion(Status::Unknown2), new FeatureCountVisitor(),
        "Unconflated Feature Count Map 2");
    const double totalUnconflatedFeatureCount =
      _applyVisitor(
        new NotCriterion(new StatusCriterion(Status::Conflated)), new FeatureCountVisitor(),
        "Unconflated Feature Count");
    _addStat("Total Unmatched Features", totalUnconflatedFeatureCount);
    _addStat("Percentage of Total Features Unmatched",
             (totalUnconflatedFeatureCount / (double)featureCount) * 100.0);
    _addStat("Total Unmatched Features From Map 1", unconflatedFeatureCountFromMap1);
    _addStat("Percentage of Total Features Unmatched From Map 1",
             (unconflatedFeatureCountFromMap1 / (double)featureCount) * 100.0);
    _addStat("Total Unmatched Features From Map 2", unconflatedFeatureCountFromMap2);
    _addStat("Percentage of Total Features Unmatched From Map 2",
             (unconflatedFeatureCountFromMap2 / (double)featureCount) * 100.0);

    for (QMap<CreatorDescription::BaseFeatureType, double>::const_iterator it =
           _conflatableFeatureCounts.begin(); it != _conflatableFeatureCounts.end(); ++it)
    {
      const QString featureType = CreatorDescription::baseFeatureTypeToString(it.key());
      if (!_featureTypesToSkip.contains(featureType, Qt::CaseInsensitive))
      {
        _generateFeatureStats(it.key(), it.value(),
                              CreatorDescription::getFeatureCalcType(it.key()),
                              CreatorDescription::getElementCriterion(it.key(), map),
                              poisMergedIntoPolys, poisMergedIntoPolysFromMap1,
                              poisMergedIntoPolysFromMap2);
      }
    }

    _addStat("Longest Tag", _getApplyVisitor(new LongestTagVisitor(), "Longest Tag"));

    // TODO: this should be moved into _generateFeatureStats?
    LOG_DEBUG("config script: " + ConfigOptions().getStatsTranslateScript());
    if (ConfigOptions().getStatsTranslateScript() != "")
    {
      shared_ptr<ScriptSchemaTranslator> st(
        ScriptSchemaTranslatorFactory::getInstance().createTranslator(
          ConfigOptions().getStatsTranslateScript()));
      st->setErrorTreatment(StrictOff);
      SchemaTranslatedTagCountVisitor tcv(st);
      _applyVisitor(&tcv, "Translated Tag Count");
      _addStat("Translated Populated Tag Percent", tcv.getStat());
      _addStat("Translated Populated Tags", tcv.getPopulatedCount());
      _addStat("Translated Default Tags", tcv.getDefaultCount());
      _addStat("Translated Null Tags", tcv.getNullCount());

      // TODO: make this more extensible
      _addStat("Area Translated Populated Tag Percent",
        _applyVisitor(
          new AreaCriterion(map), new SchemaTranslatedTagCountVisitor(st),
          "Translated Areas Count"));
      _addStat("Building Translated Populated Tag Percent",
        _applyVisitor(
          new BuildingCriterion(map), new SchemaTranslatedTagCountVisitor(st),
          "Translated Buildings Count"));
      _addStat("Road Translated Populated Tag Percent",
        _applyVisitor(
          new HighwayCriterion(map), new SchemaTranslatedTagCountVisitor(st),
          "Translated Roads Count"));
      _addStat("POI Translated Populated Tag Percent",
        _applyVisitor(
          new PoiCriterion(), new SchemaTranslatedTagCountVisitor(st), "Translated POIs Count"));
      _addStat("Polygon Conflatable POI Translated Populated Tag Percent",
        _applyVisitor(
          new PoiPolygonPoiCriterion(), new SchemaTranslatedTagCountVisitor(st),
          "Translated POI/Polygon POIs Count"));
      _addStat("Polygon Translated Populated Tag Percent",
        _applyVisitor(
          new PoiPolygonPolyCriterion(), new SchemaTranslatedTagCountVisitor(st),
          "Translated POI/Polygon Polygons Count"));
      _addStat("Power Line Translated Populated Tag Percent",
        _applyVisitor(
          new PowerLineCriterion(), new SchemaTranslatedTagCountVisitor(st),
          "Translated Power Lines Count"));
      _addStat("Railway Translated Populated Tag Percent",
        _applyVisitor(
          new RailwayCriterion(), new SchemaTranslatedTagCountVisitor(st),
          "Translated Railways Count"));
      _addStat("Waterway Translated Populated Tag Percent",
        _applyVisitor(
          new LinearWaterwayCriterion(), new SchemaTranslatedTagCountVisitor(st),
          "Translated Waterways Count"));
    }
    else
    {
      LOG_DEBUG("Skipping stats translation");
    }
  }

  logMsg = QString::number(_totalStatCalcs) + " map statistics calculated";
  if (!_mapName.isEmpty())
  {
    logMsg += " for " + _mapName;
  }
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

void CalculateStatsOp::_addStat(const char* name, double value)
{
  _stats.append(SingleStat(QString(name), value));
}

void CalculateStatsOp::_interpretStatData(shared_ptr<const OsmMap>& constMap, StatData& d)
{
  shared_ptr<ElementCriterion> pCrit;

  if (d.criterion.length() > 0)
  {
    if (_criterionCache.contains(d.criterion))
    {
      pCrit = _criterionCache[d.criterion];
    }
    else
    {
      try
      {
        // create criterion
        pCrit =
          shared_ptr<ElementCriterion>(Factory::getInstance().constructObject<ElementCriterion>(d.criterion));

        // make sure the map is set before we use it
        ConstOsmMapConsumer* pMapConsumer = dynamic_cast<ConstOsmMapConsumer*>(pCrit.get());
        if (pMapConsumer) pMapConsumer->setOsmMap(constMap.get());
      }
      catch (...)
      {
        throw HootException(
          QString("Unable to construct criterion '%1' in stats data entry '%2'")
            .arg(d.criterion, d.name) );
      }

      _criterionCache[d.criterion] = pCrit;
    }
  }

  if (d.visitor.length() > 0)
  {
    double val = 0;

    if (pCrit)
    {
      // if a criterion is specified, we apply a FilteredVisitor
      // this is not cached as there is only one value and it's unlikely to be repeated
      shared_ptr<ConstElementVisitor> pCriterionVisitor;

      try
      {
        // create visitor to set in FilteredVisitor
        pCriterionVisitor =
          shared_ptr<ConstElementVisitor>(
            static_cast<ConstElementVisitor *>(
              Factory::getInstance().constructObject<ElementVisitor>(d.visitor)));
      }
      catch (...)
      {
        throw HootException(
          QString("Unable to construct visitor '%1' in stats data entry '%2'")
            .arg(d.visitor, d.name) );
      }

      FilteredVisitor filteredVisitor =
        FilteredVisitor(pCrit, ConstElementVisitorPtr(pCriterionVisitor));
      val = _applyVisitor(filteredVisitor, d.name, d.statCall);
      _numInterpresetStatDataCalls++;
    }
    else
    {
      shared_ptr<ConstElementVisitor> pVisitor;

      if (_appliedVisitorCache.contains(d.visitor))
      {  
        pVisitor = _appliedVisitorCache[d.visitor];

        // Even though this is cached, and its a freebie runtime-wise, we'll still log status to
        // ensure the stat calc index remains acurate.
        LOG_STATUS(
          "Calculating statistic: " << d.name << " (" << _currentStatCalcIndex << "/" <<
          _totalStatCalcs << ") ...");

        _currentStatCalcIndex++;
        _numInterpretStatVisCacheHits++;
      }
      else
      {
        try
        {
          pVisitor =
            shared_ptr<ConstElementVisitor>(
              static_cast<ConstElementVisitor *>(
                Factory::getInstance().constructObject<ElementVisitor>(d.visitor)));
        }
        catch (...)
        {
          throw HootException(
            QString("Unable to construct visitor '%1' in stats data entry '%2'")
              .arg(d.visitor, d.name) );
        }

        // without criterion, apply the visitor directly and interpret as NumericStatistic
        _applyVisitor(pVisitor.get(), d.name);
        _appliedVisitorCache[d.visitor] = pVisitor;
        _numInterpresetStatDataCalls++;
      }

      val = GetRequestedStatValue(pVisitor.get(), d.statCall);
    }

    _addStat(d.name, val);
  }
}

double CalculateStatsOp::GetRequestedStatValue(const ElementVisitor* pVisitor, StatCall call)
{
  if (call == Stat)
  {
    const SingleStatistic* ss = dynamic_cast<const SingleStatistic*>(pVisitor);
    return ss ? ss->getStat() : 0;
  }

  const NumericStatistic* ns = dynamic_cast<const NumericStatistic*>(pVisitor);

  if (!ns) return 0;

  switch (call)
  {
    case Stat: return ns->getStat();
    case Min: return ns->getMin();
    case Max: return ns->getMax();
    case Average: return ns->getAverage();
    case InfoCount: return ns->getInformationCount();
    case InfoMin: return ns->getInformationMin();
    case InfoMax: return ns->getInformationMax();
    case InfoAverage: return ns->getInformationAverage();
    case InfoDiff: return ns->getInformationCountDiff();
    default: return 0;
  }
}

bool CalculateStatsOp::_matchDescriptorCompare(const CreatorDescription& m1,
                                               const CreatorDescription& m2)
{
  return m1.className > m2.className;
}

double CalculateStatsOp::_applyVisitor(ElementCriterion* pCrit,
                                       ConstElementVisitor* pVis, const QString& statName,
                                       StatCall call)
{
  return _applyVisitor(FilteredVisitor(pCrit, pVis), statName, call);
}

double CalculateStatsOp::_applyVisitor(const FilteredVisitor& v, const QString& statName,
                                       StatCall call)
{
  boost::any emptyVisitorData;
  return _applyVisitor(v, emptyVisitorData, statName, call);
}

double CalculateStatsOp::_applyVisitor(const FilteredVisitor& v, boost::any& visitorData,
                                       const QString& statName, StatCall call)
{
  LOG_STATUS(
    "Calculating statistic: " << statName << " (" << _currentStatCalcIndex << "/" <<
    _totalStatCalcs << ") ...");

  // this is a hack to let C++ pass v as a temporary
  FilteredVisitor* fv = const_cast<FilteredVisitor*>(&v);
  shared_ptr<FilteredVisitor> critFv;
  if (_criterion)
  {
    critFv.reset(new FilteredVisitor(*_criterion, *fv));
    fv = critFv.get();
  }

  ElementVisitor& childVisitor = v.getChildVisitor();

  // There's a possible optimization that could be made here (and in the _applyVisitor below) to
  // check the criterion used by FilteredVisitor, and if it inherits from GeometryTypeCriterion
  // then call visitWaysRo, etc. Not sure if its worth the effort, though.

  _constMap->visitRo(*fv);

  DataProducer* dataProducer = dynamic_cast<DataProducer*>(&childVisitor);
  if (dataProducer != nullptr)
  {
    visitorData = dataProducer->getData();
  }

  _currentStatCalcIndex++;
  return GetRequestedStatValue(&childVisitor, call);
}

void CalculateStatsOp::_applyVisitor(ConstElementVisitor* v, const QString& statName)
{
  LOG_STATUS(
    "Calculating statistic: " << statName << " (" << _currentStatCalcIndex << "/" <<
    _totalStatCalcs << ") ...");

  shared_ptr<FilteredVisitor> critFv;
  if (_criterion)
  {
    critFv.reset(new FilteredVisitor(*_criterion, *v));
    v = critFv.get();
  }
  _constMap->visitRo(*v);

  _currentStatCalcIndex++;
}

double CalculateStatsOp::_getApplyVisitor(ConstElementVisitor* v, const QString& statName)
{
  _applyVisitor(v, statName);
  const SingleStatistic* ss = dynamic_cast<const SingleStatistic*>(v);
  if (v == nullptr)
  {
    throw HootException(v->getName() + " does not implement SingleStatistic.");
  }
  return ss->getStat();
}

double CalculateStatsOp::getSingleStat(const QString& n) const
{
  for (int i = 0; i < _stats.size(); i++)
  {
    if (_stats[i].name == n)
    {
      return _stats[i].value;
    }
  }
  throw InternalErrorException("Could not find the specified stat: " + n);
}

bool CalculateStatsOp::hasSingleStat(const QString& n) const
{
  for (int i = 0; i < _stats.size(); i++)
  {
    if (_stats[i].name == n)
    {
      return true;
    }
  }
  return false;
}

long CalculateStatsOp::indexOfSingleStat(const QString& n) const
{
  for (int i = 0; i < _stats.size(); i++)
  {
    if (_stats[i].name == n)
    {
      return i;
    }
  }
  return -1;
}

ConstElementVisitorPtr CalculateStatsOp::_getElementVisitorForFeatureType(
  const CreatorDescription::BaseFeatureType& featureType)
{
  if (featureType == CreatorDescription::PoiPolygonPOI ||
      featureType == CreatorDescription::Polygon)
  {
    //Poi/poly doesn't have the restriction that an element have a tag information count > 0 to
    //be considered for conflation.
    return ConstElementVisitorPtr(new ElementCountVisitor());
  }
  else
  {
    return ConstElementVisitorPtr(new FeatureCountVisitor());
  }
}

void CalculateStatsOp::_generateFeatureStats(const CreatorDescription::BaseFeatureType& featureType,
                                             const float conflatableCount,
                                             const CreatorDescription::FeatureCalcType& type,
                                             ElementCriterionPtr criterion,
                                             const long poisMergedIntoPolys,
                                             const long poisMergedIntoPolysFromMap1,
                                             const long poisMergedIntoPolysFromMap2)
{
  LOG_VARD(poisMergedIntoPolys);
  const QString description = CreatorDescription::baseFeatureTypeToString(featureType);
  LOG_VARD(description);
  LOG_VARD(type);

  ConstOsmMapConsumer* mapConsumer = dynamic_cast<ConstOsmMapConsumer*>(criterion.get());
  if (mapConsumer != nullptr)
  {
    mapConsumer->setOsmMap(_constMap.get());
  }

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
        ChainCriterion(
          new StatusCriterion(Status::Conflated), criterion->clone()),
      _getElementVisitorForFeatureType(featureType)), "Conflated Feature Count: " + description);
  LOG_VARD(conflatedFeatureCount);
  if (featureType == CreatorDescription::PoiPolygonPOI)
  {
    //we need to add any pois that may have been merged into polygons by poi/poly into the
    //conflated feature count for this feature type
    conflatedFeatureCount += poisMergedIntoPolys;
    LOG_VARD(conflatedFeatureCount);
  }

  const double featuresMarkedForReview =
    _applyVisitor(
      FilteredVisitor(
        ChainCriterion(
          new NeedsReviewCriterion(_constMap),
          criterion->clone()),
      _getElementVisitorForFeatureType(featureType)), "Reviewable Feature Count: " + description);
  _addStat(QString("Conflated %1s").arg(description), conflatedFeatureCount);
  _addStat(QString("%1s Marked for Review").arg(description), featuresMarkedForReview);
  const double numFeatureReviewsToBeMade =
    _applyVisitor(
       FilteredVisitor(
         criterion->clone(),
         ConstElementVisitorPtr(new CountUniqueReviewsVisitor())),
      "Reviews To Be Made Count: " + description);
  _addStat(QString("%1 Reviews to be Made").arg(description), numFeatureReviewsToBeMade);

  double unconflatedFeatureCountMap1 =
    _applyVisitor(
      FilteredVisitor(
        ChainCriterion(
          new StatusCriterion(Status::Unknown1),
          criterion->clone()),
      _getElementVisitorForFeatureType(featureType)),
      "Unconflated Feature Count Map 1: " + description);
  LOG_VARD(unconflatedFeatureCountMap1);
  double unconflatedFeatureCountMap2 =
    _applyVisitor(
      FilteredVisitor(
        ChainCriterion(
          new StatusCriterion(Status::Unknown2),
          criterion->clone()),
      _getElementVisitorForFeatureType(featureType)),
      "Unconflated Feature Count Map 2: " + description);
  LOG_VARD(unconflatedFeatureCountMap2);
  double totalUnconflatedFeatureCount =
    _applyVisitor(
      FilteredVisitor(
        ChainCriterion(
          new NotCriterion(new StatusCriterion(Status::Conflated)),
          criterion->clone()),
      _getElementVisitorForFeatureType(featureType)),
      "Total Unconflated Feature Count: " + description);
  LOG_VARD(totalUnconflatedFeatureCount);
  if (featureType == CreatorDescription::PoiPolygonPOI)
  {
    //we need to subtract any pois that may have been merged into polygons by poi/poly from the
    //unconflated feature count for this feature type
    totalUnconflatedFeatureCount -= poisMergedIntoPolys;
    LOG_VARD(totalUnconflatedFeatureCount);
    unconflatedFeatureCountMap1 -= poisMergedIntoPolysFromMap1;
    LOG_VARD(unconflatedFeatureCountMap1);
    unconflatedFeatureCountMap2 -= poisMergedIntoPolysFromMap2;
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
          ConstElementVisitorPtr(new LengthOfWaysVisitor())),
        "Total Meters: " + description));
    _addStat(QString("Meters of Conflated %1s").arg(description),
      _applyVisitor(
        FilteredVisitor(
          ChainCriterion(
            ElementCriterionPtr(new StatusCriterion(Status::Conflated)), criterion->clone()),
          ConstElementVisitorPtr(new LengthOfWaysVisitor())),
        "Meters Conflated: " + description));
    _addStat(QString("Meters of Unmatched %1s From Map 1").arg(description),
      _applyVisitor(
        FilteredVisitor(
          ChainCriterion(
            ElementCriterionPtr(new StatusCriterion(Status::Unknown1)), criterion->clone()),
          ConstElementVisitorPtr(new LengthOfWaysVisitor())),
        "Meters Unmatched Map 1: " + description));
    _addStat(QString("Meters of Unmatched %1s From Map 2").arg(description),
      _applyVisitor(
        FilteredVisitor(
          ChainCriterion(
            ElementCriterionPtr(new StatusCriterion(Status::Unknown2)), criterion->clone()),
          ConstElementVisitorPtr(new LengthOfWaysVisitor())),
        "Meters Unmatched Map 2: " + description));
    _numGenerateFeatureStatCalls++;
  }
  else if (type == CreatorDescription::CalcTypeArea)
  {
    _addStat(QString("Total Square Meters of %1s").arg(description),
      _applyVisitor(
        FilteredVisitor(
          criterion->clone(),
          ConstElementVisitorPtr(new LengthOfWaysVisitor())),
        "Total Square Meters: " + description));
    _addStat(QString("Square Meters of Conflated %1s").arg(description),
      _applyVisitor(
        FilteredVisitor(
          ChainCriterion(
            ElementCriterionPtr(new StatusCriterion(Status::Conflated)), criterion->clone()),
          ConstElementVisitorPtr(new CalculateAreaVisitor())),
        "Area Conflated: " + description));
    _addStat(QString("Square Meters of Unmatched %1s From Map 1").arg(description),
      _applyVisitor(
        FilteredVisitor(
          ChainCriterion(
            ElementCriterionPtr(new StatusCriterion(Status::Unknown1)), criterion->clone()),
          ConstElementVisitorPtr(new CalculateAreaVisitor())),
        "Area Unmatched Map 1: " + description));
    _addStat(QString("Square Meters of Unmatched %1s From Map 2").arg(description),
      _applyVisitor(
        FilteredVisitor(
          ChainCriterion(
            ElementCriterionPtr(new StatusCriterion(Status::Unknown2)), criterion->clone()),
          ConstElementVisitorPtr(new CalculateAreaVisitor())),
        "Area Unmatched Map 2: " + description));
    _numGenerateFeatureStatCalls++;
  }

  double percentageOfTotalFeaturesConflated = 0.0;
  if (totalFeatures > 0.0)
  {
    percentageOfTotalFeaturesConflated =
      (conflatedFeatureCount / totalFeatures) * 100.0;
  }
  LOG_VARD(percentageOfTotalFeaturesConflated);
  _addStat(
    QString("Percentage of %1s Conflated").arg(description), percentageOfTotalFeaturesConflated);
  double percentageOfTotalFeaturesMarkedForReview = 0.0;
  if (totalFeatures > 0.0)
  {
    percentageOfTotalFeaturesMarkedForReview =
      (featuresMarkedForReview / totalFeatures) * 100.0;
  }
  _addStat(QString("Percentage of %1s Marked for Review").arg(description),
           percentageOfTotalFeaturesMarkedForReview);
  double percentageOfMap1FeaturesUnconflated = 0.0;
  if (totalFeatures > 0.0)
  {
    percentageOfMap1FeaturesUnconflated =
      (unconflatedFeatureCountMap1 / totalFeatures) * 100.0;
  }
  LOG_VARD(percentageOfMap1FeaturesUnconflated);
  _addStat(
    QString("Percentage of Unmatched %1s From Map 1").arg(description),
    percentageOfMap1FeaturesUnconflated);
  double percentageOfMap2FeaturesUnconflated = 0.0;
  if (totalFeatures > 0.0)
  {
    percentageOfMap2FeaturesUnconflated =
      (unconflatedFeatureCountMap2 / totalFeatures) * 100.0;
  }
  LOG_VARD(percentageOfMap2FeaturesUnconflated);
  _addStat(
    QString("Percentage of Unmatched %1s From Map 2").arg(description),
    percentageOfMap2FeaturesUnconflated);
  double percentageOfTotalFeaturesUnconflated = 0.0;
  if (totalFeatures > 0.0)
  {
    percentageOfTotalFeaturesUnconflated =
      (totalUnconflatedFeatureCount / totalFeatures) * 100.0;
  }
  LOG_VARD(percentageOfTotalFeaturesUnconflated);
  _addStat(
    QString("Percentage of Unmatched %1s").arg(description), percentageOfTotalFeaturesUnconflated);



  _numGenerateFeatureStatCalls += 6;
}

}
