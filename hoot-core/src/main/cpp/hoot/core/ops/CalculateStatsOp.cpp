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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CalculateStatsOp.h"

#include <hoot/core/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/filters/AreaFilter.h>
#include <hoot/core/filters/BuildingCriterion.h>
#include <hoot/core/filters/ChainCriterion.h>
#include <hoot/core/filters/ElementTypeCriterion.h>
#include <hoot/core/filters/HighwayFilter.h>
#include <hoot/core/filters/LinearFilter.h>
#include <hoot/core/filters/NeedsReviewCriterion.h>
#include <hoot/core/filters/NoInformationCriterion.h>
#include <hoot/core/filters/NotCriterion.h>
#include <hoot/core/filters/PoiCriterion.h>
#include <hoot/core/filters/StatsAreaFilter.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/filters/TagCriterion.h>
#include <hoot/core/filters/WaterwayCriterion.h>
#include <hoot/core/io/ScriptTranslatorFactory.h>
#include <hoot/core/visitors/CalculateAreaVisitor.h>
#include <hoot/core/visitors/CalculateAreaForStatsVisitor.h>
#include <hoot/core/visitors/CountUniqueReviewsVisitor.h>
#include <hoot/core/visitors/CountVisitor.h>
#include <hoot/core/visitors/FeatureCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/LengthOfWaysVisitor.h>
#include <hoot/core/visitors/LongestTagVisitor.h>
#include <hoot/core/visitors/MaxIdVisitor.h>
#include <hoot/core/visitors/MinIdVisitor.h>
#include <hoot/core/visitors/TranslatedTagCountVisitor.h>
#include <hoot/core/visitors/UniqueNamesVisitor.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/visitors/TagCountVisitor.h>
#include <hoot/core/conflate/MatchFactory.h>
#include <hoot/core/visitors/MatchCandidateCountVisitor.h>
#include <hoot/core/Factory.h>
#include <hoot/core/util/DataProducer.h>

#include <math.h>
using namespace std;

namespace hoot
{

CalculateStatsOp::CalculateStatsOp(QString mapName, bool inputIsConflatedMapOutput) :
  _mapName(mapName),
  _quick(false),
  _inputIsConflatedMapOutput(inputIsConflatedMapOutput)
{
}

CalculateStatsOp::CalculateStatsOp(ElementCriterionPtr criterion, QString mapName,
                                   bool inputIsConflatedMapOutput) :
  _criterion(criterion),
  _mapName(mapName),
  _quick(false),
  _inputIsConflatedMapOutput(inputIsConflatedMapOutput)
{
  LOG_VARD(_inputIsConflatedMapOutput);
}

shared_ptr<MatchCreator> CalculateStatsOp::getMatchCreator(
    const vector< shared_ptr<MatchCreator> > &matchCreators,
    const QString &matchCreatorName,
    MatchCreator::BaseFeatureType &featureType)
{
  for (vector< shared_ptr<MatchCreator> >::const_iterator matchIt = matchCreators.begin();
       matchIt != matchCreators.end(); ++matchIt)
  {
    vector<MatchCreator::Description> desc = (*matchIt)->getAllCreators();
    for (vector<MatchCreator::Description>::const_iterator descIt = desc.begin();
         descIt != desc.end(); ++descIt)
    {
      QString testName = QString::fromStdString(descIt->className);
      if (0 == matchCreatorName.compare(testName))
      {
        featureType = descIt->baseFeatureType;
        return (*matchIt);
      }
    }
  }
  return shared_ptr<MatchCreator>(); // empty if not found
}

void CalculateStatsOp::apply(const shared_ptr<OsmMap>& map)
{
  QString logMsg = "Calculating map statistics";
  if (!_mapName.isEmpty())
  {
    logMsg += " for " + _mapName;
  }
  logMsg += "...";
  LOG_INFO(logMsg);
  Filter::FilterType keep = Filter::KeepMatches;

  MapProjector::projectToPlanar(map);

  shared_ptr<const OsmMap> constMap = map;

  _stats.append(SingleStat("Node Count",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Node),
      new CountVisitor()))));
  _stats.append(SingleStat("Way Count",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Way),
      new CountVisitor()))));
  _stats.append(SingleStat("Relation Count",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Relation),
      new CountVisitor()))));
  _stats.append(SingleStat("Minimum Node ID",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Node),
      new MinIdVisitor()))));
  _stats.append(SingleStat("Maximum Node ID",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Node),
      new MaxIdVisitor()))));
  _stats.append(SingleStat("Minimum Way ID",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Way),
      new MinIdVisitor()))));
  _stats.append(SingleStat("Maximum Way ID",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Way),
      new MaxIdVisitor()))));
  _stats.append(SingleStat("Minimum Relation ID",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Relation),
      new MinIdVisitor()))));
  _stats.append(SingleStat("Maximum Relation ID",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Relation),
      new MaxIdVisitor()))));

  if (!_quick)
  {
    TagCountVisitor tagCountVisitor;
    _applyVisitor(constMap, &tagCountVisitor);
    const long numTotalTags = (long)tagCountVisitor.getStat();
    _stats.append(SingleStat("Total Feature Tags", numTotalTags));
    const long numInformationTags = tagCountVisitor.getInformationCount();
    _stats.append(SingleStat("Total Feature Information Tags", numInformationTags));
    _stats.append(SingleStat("Total Feature Metadata Tags", numTotalTags - numInformationTags));

    UniqueNamesVisitor v;
    _applyVisitor(constMap, &v);
    _stats.append(SingleStat("Unique Names", v.getStat()));
    _stats.append(SingleStat("Meters of Linear Features",
      _applyVisitor(constMap, FilteredVisitor(LinearFilter(keep), new LengthOfWaysVisitor()))));
    _stats.append(SingleStat("Meters Squared of Area Features",
      _applyVisitor(constMap, FilteredVisitor(StatsAreaFilter(keep), new CalculateAreaForStatsVisitor()))));
    _stats.append(SingleStat("Meters of Highway",
      _applyVisitor(constMap, FilteredVisitor(HighwayFilter(keep), new LengthOfWaysVisitor()))));
    _stats.append(SingleStat("Highway Unique Name Count",
      _applyVisitor(constMap, FilteredVisitor(HighwayFilter(keep), new UniqueNamesVisitor()))));
    _stats.append(SingleStat("Meters Squared of Buildings",
      _applyVisitor(constMap, FilteredVisitor(BuildingCriterion(map), new CalculateAreaVisitor()))));
    _stats.append(SingleStat("Building Unique Name Count",
      _applyVisitor(constMap, FilteredVisitor(BuildingCriterion(map), new UniqueNamesVisitor()))));

    FeatureCountVisitor featureCountVisitor;
    _applyVisitor(constMap, &featureCountVisitor);
    const long featureCount = featureCountVisitor.getCount();
    _stats.append(SingleStat("Total Feature Count", featureCount));
    vector< shared_ptr<MatchCreator> > matchCreators = MatchFactory::getInstance().getCreators();
    const double conflatedFeatureCount =
      _applyVisitor(
        constMap,
        FilteredVisitor(StatusCriterion(Status::Conflated), new FeatureCountVisitor()));
    //We're tailoring the stats to whether the map being examined is the input to a conflation job
    //or the output from a conflation job.  When the stats option is called from the conflate
    //command, this is accomplished by allowing the conflate command to notify this op which kind
    //of map is being examined.  When the stats command is called by itself, there is no mechanism
    //for doing that, so we're assuming if there are any conflated features in the map, that the map
    //should be considered an the output of a conflation job.  This logic, of course, breaks down if
    //the same data is conflated more than once.  In that case, it may be wise to let the stats
    //command pass in a variable stating whether the map is input/output, like the conflate command
    //does.
    if (conflatedFeatureCount > 0)
    {
      _inputIsConflatedMapOutput = true;
    }
    double conflatableFeatureCount = -1.0;
    any matchCandidateCountsData;
    conflatableFeatureCount =
      _applyVisitor(
        constMap,
        FilteredVisitor(
          ChainCriterion(
            new NotCriterion(new StatusCriterion(Status::Conflated)),
            new NotCriterion(new NeedsReviewCriterion(constMap))),
          new MatchCandidateCountVisitor(matchCreators)),
        matchCandidateCountsData);
    if (_inputIsConflatedMapOutput)
    {
      //The conflatable stat has no meaning on a conflated output map, since everything in the
      //output is either conflated, passed through, marked for review, or left unconflated.
      conflatableFeatureCount = 0.0;
    }

    _stats.append(SingleStat("Total Conflatable Features", conflatableFeatureCount));
    _stats.append(
      SingleStat(
        "Percentage of Total Features Conflatable",
        ((double)conflatableFeatureCount / (double)featureCount) * 100.0));
    const double numFeaturesMarkedForReview =
      _applyVisitor(
        constMap,
        FilteredVisitor(new NeedsReviewCriterion(constMap), new FeatureCountVisitor()));

    CountUniqueReviewsVisitor curv;
    constMap->visitRo(curv);
    const double numReviewsToBeMade = curv.getStat();
    const double untaggedFeatureCount =
      _applyVisitor(constMap,FilteredVisitor(new NoInformationCriterion(),new FeatureCountVisitor()));
    _stats.append(SingleStat("Untagged Feature Count", untaggedFeatureCount));
    long unconflatableFeatureCount = -1.0;
    if (!_inputIsConflatedMapOutput)
    {
      unconflatableFeatureCount = fmax((featureCount - untaggedFeatureCount - conflatableFeatureCount), (long)0);
    }
    else
    {
      //this stat has no meaning on a conflated output map (see total feature conflatable stat
      //comment)
      unconflatableFeatureCount = 0.0;
    }
    _stats.append(SingleStat("Total Unconflatable Features", unconflatableFeatureCount));
    _stats.append(
      SingleStat(
        "Percentage of Total Features Unconflatable",
        ((double)unconflatableFeatureCount / (double)featureCount) * 100.0));

    _stats.append(SingleStat("Number of Match Creators", matchCreators.size()));
    QMap<MatchCreator::BaseFeatureType, double> featureCounts;
    for(MatchCreator::BaseFeatureType ft = MatchCreator::POI; ft < MatchCreator::Unknown; ft = MatchCreator::BaseFeatureType(ft+1))
    {
      featureCounts[ft] = 0.0;
    }

    QMap<QString, long> matchCandidateCountsByMatchCreator =
      any_cast<QMap<QString, long> >(matchCandidateCountsData);
    LOG_VARD(matchCandidateCountsByMatchCreator.size());
    LOG_VARD(matchCandidateCountsByMatchCreator);
    for (QMap<QString, long >::const_iterator iterator = matchCandidateCountsByMatchCreator.begin();
         iterator != matchCandidateCountsByMatchCreator.end(); ++iterator)
    {
      const QString matchCreatorName = iterator.key();
      LOG_VARD(matchCreatorName);
      MatchCreator::BaseFeatureType featureType = MatchCreator::Unknown;
      shared_ptr<MatchCreator> matchCreator = getMatchCreator(matchCreators, iterator.key(), featureType);

      double conflatableFeatureCountForFeatureType = 0.0;
      if (!_inputIsConflatedMapOutput)
      {
        conflatableFeatureCountForFeatureType = matchCandidateCountsByMatchCreator[matchCreatorName];
        LOG_VARD(conflatableFeatureCountForFeatureType);
      }

      _stats.append(
            SingleStat(
              "Features Conflatable by: " + matchCreatorName,
              conflatableFeatureCountForFeatureType));

      featureCounts[featureType] += conflatableFeatureCountForFeatureType;
    }

    _stats.append(SingleStat("Total Conflated Features", conflatedFeatureCount));
    _stats.append(
      SingleStat(
        "Percentage of Total Features Conflated",
        ((double)conflatedFeatureCount / (double)featureCount) * 100.0));
    _stats.append(
      SingleStat("Total Features Marked for Review", numFeaturesMarkedForReview));
    _stats.append(
      SingleStat(
        "Percentage of Total Features Marked for Review",
        ((double)numFeaturesMarkedForReview / (double)featureCount) * 100.0));
    _stats.append(
      SingleStat("Total Number of Reviews to be Made", numReviewsToBeMade));
    const double unconflatedFeatureCount =
      _applyVisitor(
        constMap,
        FilteredVisitor(
            new NotCriterion(new StatusCriterion(Status::Conflated)),
          new FeatureCountVisitor()));
    _stats.append(SingleStat("Total Unmatched Features", unconflatedFeatureCount));
    _stats.append(
      SingleStat(
        "Percentage of Total Features Unmatched",
        ((double)unconflatedFeatureCount / (double)featureCount) * 100.0));

    for (QMap<MatchCreator::BaseFeatureType, double>::iterator it = featureCounts.begin();
         it != featureCounts.end(); ++it)
    {
      _generateFeatureStats(constMap,
                            MatchCreator::BaseFeatureTypeToString(it.key()),
                            it.value(),
                            MatchCreator::getFeatureCalcType(it.key()),
                            MatchCreator::getElementCriterion(it.key(), map));
    }

    LongestTagVisitor v2;
    _applyVisitor(constMap, &v2);
    _stats.append(SingleStat("Longest Tag", v2.getStat()));

    LOG_DEBUG("config script: " + ConfigOptions().getStatsTranslateScript());
    if (ConfigOptions().getStatsTranslateScript() != "")
    {
      shared_ptr<ScriptTranslator> st(ScriptTranslatorFactory::getInstance().createTranslator(
        ConfigOptions().getStatsTranslateScript()));
      st->setErrorTreatment(StrictOff);
      TranslatedTagCountVisitor tcv(st);
      _applyVisitor(constMap, &tcv);
      _stats.append(SingleStat("Translated Populated Tag Percent", tcv.getStat()));
      _stats.append(SingleStat("Translated Populated Tag Count", tcv.getPopulatedCount()));
      _stats.append(SingleStat("Translated Default Tag Count", tcv.getDefaultCount()));
      _stats.append(SingleStat("Translated Null Tag Count", tcv.getNullCount()));

      _stats.append(SingleStat("Building Translated Populated Tag Percent",
        _applyVisitor(constMap, FilteredVisitor(BuildingCriterion(map),
          new TranslatedTagCountVisitor(st)))));
      _stats.append(SingleStat("Highway Translated Populated Tag Percent",
        _applyVisitor(constMap, FilteredVisitor(HighwayFilter(keep),
          new TranslatedTagCountVisitor(st)))));
      _stats.append(SingleStat("POI Translated Populated Tag Percent",
        _applyVisitor(constMap, FilteredVisitor(PoiCriterion(),
          new TranslatedTagCountVisitor(st)))));
      _stats.append(SingleStat("Waterway Translated Populated Tag Percent",
        _applyVisitor(constMap, FilteredVisitor(new WaterwayCriterion(),
          new TranslatedTagCountVisitor(st)))));
    }
    else
    {
      LOG_DEBUG("Skipping stats translation");
    }
  }

  logMsg = "Map statistics calculated";
  if (!_mapName.isEmpty())
  {
    logMsg += " for " + _mapName;
  }
  logMsg += ".";
  LOG_INFO(logMsg);
}

bool CalculateStatsOp::_matchDescriptorCompare(const MatchCreator::Description& m1,
                                               const MatchCreator::Description& m2)
{
  return m1.className > m2.className;
}

double CalculateStatsOp::_applyVisitor(shared_ptr<const OsmMap> &map, const FilteredVisitor& v)
{
  any emptyVisitorData;
  return _applyVisitor(map, v, emptyVisitorData);
}

double CalculateStatsOp::_applyVisitor(shared_ptr<const OsmMap> &map, const FilteredVisitor& v,
                                       any& visitorData)
{
  // this is a hack to let C++ pass v as a temporary. Bad Jason.
  FilteredVisitor* fv = const_cast<FilteredVisitor*>(&v);
  auto_ptr<FilteredVisitor> critFv;
  if (_criterion)
  {
    critFv.reset(new FilteredVisitor(*_criterion, *fv));
    fv = critFv.get();
  }
  SingleStatistic* ss = dynamic_cast<SingleStatistic*>(&v.getChildVisitor());
  assert(ss != 0);

  map->visitRo(*fv);

  DataProducer* dataProducer = dynamic_cast<DataProducer*>(&v.getChildVisitor());
  if (dataProducer != 0)
  {
    visitorData = dataProducer->getData();
  }

  return ss->getStat();
}

void CalculateStatsOp::_applyVisitor(shared_ptr<const OsmMap>& map, ElementVisitor *v)
{
  auto_ptr<FilteredVisitor> critFv;
  if (_criterion)
  {
    critFv.reset(new FilteredVisitor(*_criterion, *v));
    v = critFv.get();
  }
  map->visitRo(*v);
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

void CalculateStatsOp::printStats()
{
  LOG_INFO("Number of statistics calculated: " + QString::number(_stats.size()));
  for (int i = 0; i < _stats.size(); i++)
  {
    LOG_INFO(_stats[i].toString());
  }
}

void CalculateStatsOp::_generateFeatureStats(shared_ptr<const OsmMap> &map, QString description,
                                             float conflatableCount,
                                             MatchCreator::FeatureCalcType type,
                                             ElementCriterion* criterion)
{
  shared_ptr<ElementCriterion> e_criterion(criterion);
  const double totalFeatures =
    _applyVisitor(map, FilteredVisitor(e_criterion->clone(), new FeatureCountVisitor()));
  _stats.append(SingleStat(QString("%1 Count").arg(description), totalFeatures));
  _stats.append(SingleStat(QString("Conflatable %1s").arg(description), conflatableCount));
  const double conflatedFeatureCount =
    _applyVisitor(map, FilteredVisitor(
      ChainCriterion(new StatusCriterion(Status::Conflated), e_criterion->clone()), new FeatureCountVisitor()));
  const double featuresMarkedForReview =
    _applyVisitor(map, FilteredVisitor(
      ChainCriterion(new NeedsReviewCriterion(map), e_criterion->clone()), new FeatureCountVisitor()));
  _stats.append(
    SingleStat(QString("Conflated %1s").arg(description), conflatedFeatureCount));
  _stats.append(SingleStat(QString("%1s Marked for Review").arg(description), featuresMarkedForReview));
  const double numFeatureReviewsToBeMade =
    _applyVisitor(
      map,
       FilteredVisitor(
        e_criterion->clone(),
        new CountUniqueReviewsVisitor()));
  _stats.append(
    SingleStat(QString("Number of %1 Reviews to be Made").arg(description), numFeatureReviewsToBeMade));
  const double unconflatedFeatureCount =
    _applyVisitor(
      map,
      FilteredVisitor(
        ChainCriterion(
          new NotCriterion(new StatusCriterion(Status::Conflated)),
          e_criterion->clone()),
        new FeatureCountVisitor()));
  _stats.append(SingleStat(QString("Unmatched %1s").arg(description), unconflatedFeatureCount));
  if (type == MatchCreator::CalcTypeLength)
  {
    _stats.append(SingleStat(QString("Meters of %1 Processed by Conflation").arg(description),
      _applyVisitor(map, FilteredVisitor(ChainCriterion(new StatusCriterion(Status::Conflated),
        e_criterion->clone()), new LengthOfWaysVisitor()))));
  }
  else if (type == MatchCreator::CalcTypeArea)
  {
    _stats.append(SingleStat(QString("Meters Squared of %1s Processed by Conflation").arg(description),
      _applyVisitor(map, FilteredVisitor(ChainCriterion(new StatusCriterion(Status::Conflated),
        e_criterion->clone()), new CalculateAreaVisitor()))));
  }
  double percentageOfTotalFeaturesConflated = 0.0;
  if (totalFeatures > 0.0)
  {
    percentageOfTotalFeaturesConflated =
      ((double)conflatedFeatureCount / (double)totalFeatures) * 100.0;
  }
  _stats.append(
    SingleStat(QString("Percentage of %1s Conflated").arg(description), percentageOfTotalFeaturesConflated));
  double percentageOfTotalFeaturesMarkedForReview = 0.0;
  if (totalFeatures > 0.0)
  {
    percentageOfTotalFeaturesMarkedForReview =
      ((double)featuresMarkedForReview / (double)totalFeatures) * 100.0;
  }
  _stats.append(
    SingleStat(QString("Percentage of %1s Marked for Review").arg(description), percentageOfTotalFeaturesMarkedForReview));
  double percentageOfTotalFeaturesUnconflated = 0.0;
  if (totalFeatures > 0.0)
  {
    percentageOfTotalFeaturesUnconflated = ((double)unconflatedFeatureCount / (double)totalFeatures) * 100.0;
  }
  _stats.append(
    SingleStat(QString("Percentage of Unmatched %1s").arg(description), percentageOfTotalFeaturesUnconflated));
}

}
