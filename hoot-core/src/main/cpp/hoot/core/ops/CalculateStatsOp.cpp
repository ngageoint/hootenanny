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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CalculateStatsOp.h"

#include <hoot/core/util/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/NeedsReviewCriterion.h>
#include <hoot/core/criterion/NoInformationCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/StatsAreaCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/criterion/LinearWaterwayCriterion.h>
#include <hoot/core/io/ScriptTranslatorFactory.h>
#include <hoot/core/visitors/CalculateAreaVisitor.h>
#include <hoot/core/visitors/CalculateAreaForStatsVisitor.h>
#include <hoot/core/visitors/CountUniqueReviewsVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
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
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/visitors/MatchCandidateCountVisitor.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/conflate/stats/DataProducer.h>
#include <hoot/core/io/ScriptTranslator.h>
#include <hoot/core/visitors/SumNumericTagsVisitor.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>
#include <hoot/core/visitors/AddressCountVisitor.h>
#include <hoot/core/visitors/PhoneNumberCountVisitor.h>
#include <hoot/core/criterion/HasAddressCriterion.h>
#include <hoot/core/criterion/HasPhoneNumberCriterion.h>
#include <hoot/core/criterion/HasNameCriterion.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/criterion/MultiUseBuildingCriterion.h>
#include <hoot/core/criterion/NonBuildingAreaCriterion.h>
#include <hoot/core/criterion/RoundaboutCriterion.h>
#include <hoot/core/criterion/BridgeCriterion.h>
#include <hoot/core/criterion/TunnelCriterion.h>
#include <hoot/core/visitors/BuildingHeightVisitor.h>
#include <hoot/core/visitors/BuildingLevelsVisitor.h>
#include <hoot/core/visitors/NodesPerWayVisitor.h>
#include <hoot/core/visitors/MembersPerRelationVisitor.h>

#include <math.h>

using namespace boost;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, CalculateStatsOp)

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
  LOG_VART(_inputIsConflatedMapOutput);
}

boost::shared_ptr<MatchCreator> CalculateStatsOp::getMatchCreator(
    const vector<boost::shared_ptr<MatchCreator>>& matchCreators,
    const QString& matchCreatorName,
    CreatorDescription::BaseFeatureType& featureType)
{
  for (vector<boost::shared_ptr<MatchCreator>>::const_iterator matchIt = matchCreators.begin();
       matchIt != matchCreators.end(); ++matchIt)
  {
    vector<CreatorDescription> desc = (*matchIt)->getAllCreators();
    for (vector<CreatorDescription>::const_iterator descIt = desc.begin();
         descIt != desc.end(); ++descIt)
    {
      QString testName = QString::fromStdString(descIt->className);
      LOG_VART(testName);
      if (0 == matchCreatorName.compare(testName))
      {
        featureType = descIt->baseFeatureType;
        LOG_VART(featureType);
        return (*matchIt);
      }
    }
  }
  return boost::shared_ptr<MatchCreator>(); // empty if not found
}

void CalculateStatsOp::apply(const OsmMapPtr& map)
{
  QString logMsg = "Calculating map statistics";
  if (!_mapName.isEmpty())
  {
    logMsg += " for " + _mapName;
  }
  logMsg += "...";
  LOG_INFO(logMsg);

  MapProjector::projectToPlanar(map);

  boost::shared_ptr<const OsmMap> constMap = map;

  _stats.append(SingleStat("Node Count",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Node),
      ConstElementVisitorPtr(new ElementCountVisitor())))));
  _stats.append(SingleStat("Way Count",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Way),
      ConstElementVisitorPtr(new ElementCountVisitor())))));
  _stats.append(SingleStat("Relation Count",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Relation),
      ConstElementVisitorPtr(new ElementCountVisitor())))));
  _stats.append(SingleStat("Minimum Node ID",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Node),
      ConstElementVisitorPtr(new MinIdVisitor())))));
  _stats.append(SingleStat("Maximum Node ID",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Node),
      ConstElementVisitorPtr(new MaxIdVisitor())))));
  _stats.append(SingleStat("Minimum Way ID",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Way),
      ConstElementVisitorPtr(new MinIdVisitor())))));
  _stats.append(SingleStat("Maximum Way ID",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Way),
      ConstElementVisitorPtr(new MaxIdVisitor())))));
  _stats.append(SingleStat("Minimum Relation ID",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Relation),
      ConstElementVisitorPtr(new MinIdVisitor())))));
  _stats.append(SingleStat("Maximum Relation ID",
    _applyVisitor(constMap, FilteredVisitor(ElementTypeCriterion(ElementType::Relation),
      ConstElementVisitorPtr(new MaxIdVisitor())))));

  if (!_quick)
  {
    NodesPerWayVisitor nodesPerWayVis;
    _applyVisitor(constMap, &nodesPerWayVis);
    _stats.append(SingleStat("Least Nodes in a Way", nodesPerWayVis.getMin()));
    _stats.append(SingleStat("Most Nodes in a Way", nodesPerWayVis.getMax()));
    _stats.append(SingleStat("Average Nodes Per Way", nodesPerWayVis.getAverage()));
    _stats.append(SingleStat("Total Way Nodes", nodesPerWayVis.getStat()));

    MembersPerRelationVisitor membersPerRelationVis;
    _applyVisitor(constMap, &membersPerRelationVis);
    _stats.append(SingleStat("Least Members in a Relation", membersPerRelationVis.getMin()));
    _stats.append(SingleStat("Most Members in a Relation", membersPerRelationVis.getMax()));
    _stats.append(SingleStat("Average Members Per Relation", membersPerRelationVis.getAverage()));
    _stats.append(SingleStat("Total Relation Members", membersPerRelationVis.getStat()));

    TagCountVisitor tagCountVisitor;
    _applyVisitor(constMap, &tagCountVisitor);
    const long numTotalTags = (long)tagCountVisitor.getStat();
    _stats.append(SingleStat("Total Feature Tags", numTotalTags));
    const long numInformationTags = tagCountVisitor.getInformationCount();
    _stats.append(SingleStat("Total Feature Information Tags", numInformationTags));
    _stats.append(SingleStat("Total Feature Metadata Tags", numTotalTags - numInformationTags));
    _stats.append(SingleStat("Least Tags on a Feature", tagCountVisitor.getMin()));
    _stats.append(SingleStat("Most Tags on a Feature", tagCountVisitor.getMax()));
    _stats.append(SingleStat("Average Tags Per Feature", tagCountVisitor.getAverage()));
    _stats.append(
      SingleStat("Least Information Tags on a Feature", tagCountVisitor.getInformationMin()));
    _stats.append(
      SingleStat("Most Information Tags on a Feature", tagCountVisitor.getInformationMax()));
    _stats.append(
      SingleStat("Average Information Tags Per Feature", tagCountVisitor.getInformationAverage()));

    _stats.append(SingleStat("Features with Names",
      _applyVisitor(
        constMap, FilteredVisitor(HasNameCriterion(),
        ConstElementVisitorPtr(new ElementCountVisitor())))));
    UniqueNamesVisitor v;
    _applyVisitor(constMap, &v);
    _stats.append(SingleStat("Unique Names", v.getStat()));

    _stats.append(
      SingleStat("Meters of Linear Features",
      _applyVisitor(
        constMap,
        FilteredVisitor(LinearCriterion(), ConstElementVisitorPtr(new LengthOfWaysVisitor())))));
    _stats.append(
      SingleStat("Meters Squared of Area Features",
      _applyVisitor(
        constMap,
        FilteredVisitor(StatsAreaCriterion(),
          ConstElementVisitorPtr(new CalculateAreaForStatsVisitor())))));
    _stats.append(
      SingleStat("Meters of Roads",
      _applyVisitor(
        constMap,
        FilteredVisitor(HighwayCriterion(), ConstElementVisitorPtr(new LengthOfWaysVisitor())))));
    _stats.append(
      SingleStat("Road Unique Name Count",
      _applyVisitor(
      constMap,
      FilteredVisitor(HighwayCriterion(), ConstElementVisitorPtr(new UniqueNamesVisitor())))));
    _stats.append(
      SingleStat("Meters Squared of Buildings",
      _applyVisitor(
      constMap,
      FilteredVisitor(BuildingCriterion(map), ConstElementVisitorPtr(new CalculateAreaVisitor())))));
    _stats.append(
      SingleStat("Building Unique Name Count",
      _applyVisitor(
      constMap,
      FilteredVisitor(BuildingCriterion(map), ConstElementVisitorPtr(new UniqueNamesVisitor())))));

    _stats.append(SingleStat("Bridges",
      _applyVisitor(
        constMap, FilteredVisitor(BridgeCriterion(),
        ConstElementVisitorPtr(new ElementCountVisitor())))));
    _stats.append(SingleStat("Tunnels",
      _applyVisitor(
        constMap, FilteredVisitor(TunnelCriterion(),
        ConstElementVisitorPtr(new ElementCountVisitor())))));
    _stats.append(SingleStat("One-Way Streets",
      _applyVisitor(
        constMap, FilteredVisitor(OneWayCriterion(),
        ConstElementVisitorPtr(new ElementCountVisitor())))));
    _stats.append(SingleStat("Road Roundabouts",
      _applyVisitor(
        constMap, FilteredVisitor(RoundaboutCriterion(),
        ConstElementVisitorPtr(new ElementCountVisitor())))));

    _stats.append(SingleStat("Multi-Use Buildings",
      _applyVisitor(
        constMap, FilteredVisitor(MultiUseBuildingCriterion(),
        ConstElementVisitorPtr(new ElementCountVisitor())))));
    BuildingHeightVisitor buildingHeightVis;
    _applyVisitor(constMap, &buildingHeightVis);
    _stats.append(SingleStat("Buildings With Height Info", buildingHeightVis.numWithStat()));
    _stats.append(SingleStat("Shortest Building Height", buildingHeightVis.getMin()));
    _stats.append(SingleStat("Tallest Building Height", buildingHeightVis.getMax()));
    _stats.append(SingleStat("Average Height Per Building", buildingHeightVis.getAverage()));
    BuildingLevelsVisitor buildingLevelVis;
    _applyVisitor(constMap, &buildingLevelVis);
    _stats.append(SingleStat("Buildings With Level Info", buildingLevelVis.numWithStat()));
    _stats.append(SingleStat("Least Levels in a Building", buildingLevelVis.getMin()));
    _stats.append(SingleStat("Most Levels in a Building", buildingHeightVis.getMax()));
    _stats.append(
      SingleStat("Average Levels Per Building", buildingHeightVis.getAverage()));

    _stats.append(SingleStat("Non-Building Areas",
      _applyVisitor(
        constMap, FilteredVisitor(NonBuildingAreaCriterion(),
        ConstElementVisitorPtr(new ElementCountVisitor())))));

    _stats.append(SingleStat("Features with Addresses",
      _applyVisitor(
        constMap, FilteredVisitor(HasAddressCriterion(),
        ConstElementVisitorPtr(new ElementCountVisitor())))));
    AddressCountVisitor addressCountVis;
    _applyVisitor(constMap, &addressCountVis);
    _stats.append(SingleStat("Total Addresses", addressCountVis.getStat()));

    _stats.append(SingleStat("Features with Phone Numbers",
      _applyVisitor(
        constMap, FilteredVisitor(HasPhoneNumberCriterion(),
        ConstElementVisitorPtr(new ElementCountVisitor())))));
    PhoneNumberCountVisitor phoneCountVis;
    _applyVisitor(constMap, &phoneCountVis);
    _stats.append(SingleStat("Total Phone Numbers", phoneCountVis.getStat()));

    FeatureCountVisitor featureCountVisitor;
    _applyVisitor(constMap, &featureCountVisitor);
    const long featureCount = featureCountVisitor.getCount();
    LOG_VART(featureCount);
    _stats.append(SingleStat("Total Feature Count", featureCount));
    vector< boost::shared_ptr<MatchCreator> > matchCreators =
      MatchFactory::getInstance().getCreators();
    LOG_VARD(matchCreators.size());
    double conflatedFeatureCount =
      _applyVisitor(
        constMap,
        FilteredVisitor(
          StatusCriterion(Status::Conflated), ConstElementVisitorPtr(new FeatureCountVisitor())));

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
    any matchCandidateCountsData;
    double conflatableFeatureCount =
      _applyVisitor(
        constMap,
        FilteredVisitor(
          ChainCriterion(
            ElementCriterionPtr(
              new NotCriterion(ElementCriterionPtr(new StatusCriterion(Status::Conflated)))),
            ElementCriterionPtr(
              new NotCriterion(ElementCriterionPtr(new NeedsReviewCriterion(constMap))))),
          ConstElementVisitorPtr(new MatchCandidateCountVisitor(matchCreators))),
        matchCandidateCountsData);
    LOG_VARD(matchCreators.size());
    SumNumericTagsVisitor tagSumVis(QStringList(MetadataTags::HootPoiPolygonPoisMerged()));
    constMap->visitRo(tagSumVis);
    long poisMergedIntoPolys = tagSumVis.getStat();
    //we need to add any pois that may have been merged into polygons by poi/poly into the total
    //conflated feature count
    conflatedFeatureCount += poisMergedIntoPolys;

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
      _applyVisitor(
        constMap, FilteredVisitor(new NoInformationCriterion(),new FeatureCountVisitor()));
    _stats.append(SingleStat("Untagged Feature Count", untaggedFeatureCount));
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
    _stats.append(SingleStat("Total Unconflatable Features", unconflatableFeatureCount));
    _stats.append(
      SingleStat(
        "Percentage of Total Features Unconflatable",
        ((double)unconflatableFeatureCount / (double)featureCount) * 100.0));

    _stats.append(SingleStat("Match Creators", matchCreators.size()));
    QMap<CreatorDescription::BaseFeatureType, double> conflatableFeatureCounts;
    for (CreatorDescription::BaseFeatureType ft = CreatorDescription::POI;
         ft < CreatorDescription::Unknown;
         ft = CreatorDescription::BaseFeatureType(ft+1))
    {
      conflatableFeatureCounts[ft] = 0.0;
    }

    const QMap<QString, long> matchCandidateCountsByMatchCreator =
      any_cast<QMap<QString, long> >(matchCandidateCountsData);
    LOG_VARD(matchCandidateCountsByMatchCreator.size());
    LOG_VARD(matchCandidateCountsByMatchCreator);
    for (QMap<QString, long >::const_iterator iterator = matchCandidateCountsByMatchCreator.begin();
         iterator != matchCandidateCountsByMatchCreator.end(); ++iterator)
    {
      const QString matchCreatorName = iterator.key();
      LOG_VARD(matchCreatorName);
      CreatorDescription::BaseFeatureType featureType = CreatorDescription::Unknown;
      /*boost::shared_ptr<MatchCreator> matchCreator =*/
        getMatchCreator(matchCreators, iterator.key(), featureType);

      double conflatableFeatureCountForFeatureType = 0.0;
      if (!_inputIsConflatedMapOutput)
      {
        conflatableFeatureCountForFeatureType = matchCandidateCountsByMatchCreator[matchCreatorName];
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
          conflatablePolyCount =
            _applyVisitor(
              constMap,
              //see comment in _generateFeatureStats as to why ElementCountVisitor is used here
              //instead of FeatureCountVisitor
              FilteredVisitor(
                PoiPolygonPolyCriterion(), ConstElementVisitorPtr(new ElementCountVisitor())));
        }
        _stats.append(
          SingleStat(
            "Polygons Conflatable by: " + matchCreatorName, conflatablePolyCount));
        conflatableFeatureCounts[CreatorDescription::Polygon] += conflatablePolyCount;

        double conflatablePoiPolyPoiCount = 0.0;
        if (!_inputIsConflatedMapOutput)
        {
          conflatablePoiPolyPoiCount =
            _applyVisitor(
              constMap,
              //see comment in _generateFeatureStats as to why ElementCountVisitor is used here
              //instead of FeatureCountVisitor
              FilteredVisitor(
                PoiPolygonPoiCriterion(), ConstElementVisitorPtr(new ElementCountVisitor())));
        }
        _stats.append(
          SingleStat(
            "POIs Conflatable by: " + matchCreatorName, conflatablePoiPolyPoiCount));
        conflatableFeatureCounts[CreatorDescription::PoiPolygonPOI] += conflatablePoiPolyPoiCount;
      }
      _stats.append(
        SingleStat(
          "Features Conflatable by: " + matchCreatorName, conflatableFeatureCountForFeatureType));
      conflatableFeatureCounts[featureType] += conflatableFeatureCountForFeatureType;
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
      SingleStat("Total Reviews to be Made", numReviewsToBeMade));
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

    for (QMap<CreatorDescription::BaseFeatureType, double>::const_iterator it =
           conflatableFeatureCounts.begin();
         it != conflatableFeatureCounts.end(); ++it)
    {
      // Unknown does not get us a usable element criterion, so skip it
      if (hoot::CreatorDescription::Unknown != it.key())
      {
        _generateFeatureStats(constMap, it.key(), it.value(),
                              CreatorDescription::getFeatureCalcType(it.key()),
                              CreatorDescription::getElementCriterion(it.key(), map),
                              poisMergedIntoPolys);
      }
    }

    LongestTagVisitor v2;
    _applyVisitor(constMap, &v2);
    _stats.append(SingleStat("Longest Tag", v2.getStat()));

    // TODO: this should be moved into _generateFeatureStats
    LOG_DEBUG("config script: " + ConfigOptions().getStatsTranslateScript());
    if (ConfigOptions().getStatsTranslateScript() != "")
    {
      boost::shared_ptr<ScriptTranslator> st(
        ScriptTranslatorFactory::getInstance().createTranslator(
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
          ConstElementVisitorPtr(new TranslatedTagCountVisitor(st))))));
      _stats.append(SingleStat("Road Translated Populated Tag Percent",
        _applyVisitor(constMap, FilteredVisitor(HighwayCriterion(),
          ConstElementVisitorPtr(new TranslatedTagCountVisitor(st))))));
      _stats.append(SingleStat("POI Translated Populated Tag Percent",
        _applyVisitor(constMap, FilteredVisitor(PoiCriterion(),
          ConstElementVisitorPtr(new TranslatedTagCountVisitor(st))))));
      _stats.append(SingleStat("Waterway Translated Populated Tag Percent",
        _applyVisitor(constMap, FilteredVisitor(LinearWaterwayCriterion(),
          ConstElementVisitorPtr(new TranslatedTagCountVisitor(st))))));
      _stats.append(SingleStat("Polygon Conflatable POI Translated Populated Tag Percent",
        _applyVisitor(constMap, FilteredVisitor(PoiPolygonPoiCriterion(),
          ConstElementVisitorPtr(new TranslatedTagCountVisitor(st))))));
      _stats.append(SingleStat("Polygon Translated Populated Tag Percent",
        _applyVisitor(constMap, FilteredVisitor(PoiPolygonPolyCriterion(),
          ConstElementVisitorPtr(new TranslatedTagCountVisitor(st))))));
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
  LOG_DEBUG(logMsg);
}

bool CalculateStatsOp::_matchDescriptorCompare(const CreatorDescription& m1,
                                               const CreatorDescription& m2)
{
  return m1.className > m2.className;
}

double CalculateStatsOp::_applyVisitor(boost::shared_ptr<const OsmMap> &map,
                                       const FilteredVisitor& v)
{
  any emptyVisitorData;
  return _applyVisitor(map, v, emptyVisitorData);
}

double CalculateStatsOp::_applyVisitor(boost::shared_ptr<const OsmMap> &map,
                                       const FilteredVisitor& v, any& visitorData)
{
  // this is a hack to let C++ pass v as a temporary. Bad Jason.
  FilteredVisitor* fv = const_cast<FilteredVisitor*>(&v);
  boost::shared_ptr<FilteredVisitor> critFv;
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

void CalculateStatsOp::_applyVisitor(boost::shared_ptr<const OsmMap>& map, ConstElementVisitor *v)
{
  boost::shared_ptr<FilteredVisitor> critFv;
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

void CalculateStatsOp::_generateFeatureStats(boost::shared_ptr<const OsmMap>& map,
                                             const CreatorDescription::BaseFeatureType& featureType,
                                             const float conflatableCount,
                                             const CreatorDescription::FeatureCalcType& type,
                                             ElementCriterionPtr criterion,
                                             const long poisMergedIntoPolys)
{
  LOG_VARD(poisMergedIntoPolys);
  const QString description = CreatorDescription::baseFeatureTypeToString(featureType);
  LOG_VARD(description);

  double totalFeatures = 0.0;
  totalFeatures =
    _applyVisitor(
      map, FilteredVisitor(criterion->clone(), _getElementVisitorForFeatureType(featureType)));
  LOG_VARD(totalFeatures);
  _stats.append(SingleStat(QString("%1 Count").arg(description), totalFeatures));
  _stats.append(SingleStat(QString("Conflatable %1s").arg(description), conflatableCount));

  double conflatedFeatureCount =
    _applyVisitor(
      map,
      FilteredVisitor(
        ChainCriterion(
          new StatusCriterion(Status::Conflated), criterion->clone()),
      _getElementVisitorForFeatureType(featureType)));
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
      map,
      FilteredVisitor(
        ChainCriterion(
          new NeedsReviewCriterion(map),
          criterion->clone()),
      _getElementVisitorForFeatureType(featureType)));
  _stats.append(
    SingleStat(QString("Conflated %1s").arg(description), conflatedFeatureCount));
  _stats.append(
    SingleStat(QString("%1s Marked for Review").arg(description), featuresMarkedForReview));
  const double numFeatureReviewsToBeMade =
    _applyVisitor(
      map,
       FilteredVisitor(
         criterion->clone(),
         ConstElementVisitorPtr(new CountUniqueReviewsVisitor())));
  _stats.append(
    SingleStat(QString("%1 Reviews to be Made").arg(description), numFeatureReviewsToBeMade));

  double unconflatedFeatureCount =
    _applyVisitor(
      map,
      FilteredVisitor(
        ChainCriterion(
          new NotCriterion(new StatusCriterion(Status::Conflated)),
          criterion->clone()),
      _getElementVisitorForFeatureType(featureType)));
  LOG_VARD(unconflatedFeatureCount);
  if (featureType == CreatorDescription::PoiPolygonPOI)
  {
    //we need to subtract any pois that may have been merged into polygons by poi/poly from the
    //unconflated feature count for this feature type
    unconflatedFeatureCount -= poisMergedIntoPolys;
    LOG_VARD(unconflatedFeatureCount);
  }
  _stats.append(SingleStat(QString("Unmatched %1s").arg(description), unconflatedFeatureCount));

  if (type == CreatorDescription::CalcTypeLength)
  {
    _stats.append(
      SingleStat(
        QString("Meters of %1 Processed by Conflation").arg(description),
      _applyVisitor(
        map,
        FilteredVisitor(ChainCriterion(ElementCriterionPtr(new StatusCriterion(Status::Conflated)),
        criterion->clone()), ConstElementVisitorPtr(new LengthOfWaysVisitor())))));
  }
  else if (type == CreatorDescription::CalcTypeArea)
  {
    _stats.append(SingleStat(QString("Meters Squared of %1s Processed by Conflation").arg(description),
      _applyVisitor(map, FilteredVisitor(ChainCriterion(ElementCriterionPtr(new StatusCriterion(Status::Conflated)),
        criterion->clone()), ConstElementVisitorPtr(new CalculateAreaVisitor())))));
  }

  double percentageOfTotalFeaturesConflated = 0.0;
  if (totalFeatures > 0.0)
  {
    percentageOfTotalFeaturesConflated =
      ((double)conflatedFeatureCount / (double)totalFeatures) * 100.0;
  }
  LOG_VARD(percentageOfTotalFeaturesConflated);
  _stats.append(
    SingleStat(
      QString("Percentage of %1s Conflated").arg(description), percentageOfTotalFeaturesConflated));
  double percentageOfTotalFeaturesMarkedForReview = 0.0;
  if (totalFeatures > 0.0)
  {
    percentageOfTotalFeaturesMarkedForReview =
      ((double)featuresMarkedForReview / (double)totalFeatures) * 100.0;
  }
  _stats.append(
    SingleStat(
      QString(
        "Percentage of %1s Marked for Review").arg(description),
        percentageOfTotalFeaturesMarkedForReview));
  double percentageOfTotalFeaturesUnconflated = 0.0;
  if (totalFeatures > 0.0)
  {
    percentageOfTotalFeaturesUnconflated =
      ((double)unconflatedFeatureCount / (double)totalFeatures) * 100.0;
  }
  LOG_VARD(percentageOfTotalFeaturesUnconflated);
  _stats.append(
    SingleStat(
      QString("Percentage of Unmatched %1s").arg(description), percentageOfTotalFeaturesUnconflated));
}

}
