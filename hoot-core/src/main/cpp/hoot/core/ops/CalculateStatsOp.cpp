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
#include <hoot/core/schema/ScriptSchemaTranslatorFactory.h>
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
#include <hoot/core/schema/ScriptSchemaTranslator.h>
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
  return shared_ptr<MatchCreator>(); // empty if not found
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

  _constMap = map;

  // These arrays will move to json files
  StatData quickStats[] =
  {
    {"Nodes", "hoot::ElementCountVisitor", "hoot::NodeTypeCriterion", StatCall::None },
    {"Ways", "hoot::ElementCountVisitor", "hoot::WayTypeCriterion", StatCall::None },
    {"Relations", "hoot::ElementCountVisitor", "hoot::RelationTypeCriterion", StatCall::None },
    {"Minimum Node ID", "hoot::MinIdVisitor", "hoot::NodeTypeCriterion", StatCall::None },
    {"Maximum Node ID", "hoot::MaxIdVisitor", "hoot::NodeTypeCriterion", StatCall::None },
    {"Minimum Way ID", "hoot::MinIdVisitor", "hoot::WayTypeCriterion", StatCall::None },
    {"Maximum Way ID", "hoot::MaxIdVisitor", "hoot::WayTypeCriterion", StatCall::None },
    {"Minimum Relation ID", "hoot::MinIdVisitor", "hoot::RelationTypeCriterion", StatCall::None },
    {"Maximum Relation ID", "hoot::MaxIdVisitor", "hoot::RelationTypeCriterion", StatCall::None },
  };

  StatData slowStats[] =
  {
    {"Least Nodes in a Way", "hoot::NodesPerWayVisitor", "", StatCall::Min },
    {"Most Nodes in a Way", "hoot::NodesPerWayVisitor", "", StatCall::Max },
    {"Average Nodes Per Way", "hoot::NodesPerWayVisitor", "", StatCall::Average },
    {"Total Way Nodes", "hoot::NodesPerWayVisitor", "", StatCall::Stat },

    {"Least Members in a Relation", "hoot::MembersPerRelationVisitor", "", StatCall::Min },
    {"Most Members in a Relation", "hoot::MembersPerRelationVisitor", "", StatCall::Max },
    {"Average Members Per Relation", "hoot::MembersPerRelationVisitor", "", StatCall::Average },
    {"Total Relation Members", "hoot::MembersPerRelationVisitor", "", StatCall::Stat },

    {"Total Feature Tags", "hoot::TagCountVisitor", "", StatCall::Stat },
    {"Total Feature Information Tags", "hoot::TagCountVisitor", "", StatCall::InfoCount },
    {"Total Feature Metadata Tags","hoot::TagCountVisitor", "", StatCall::InfoDiff },

    {"Least Tags on a Feature", "hoot::TagCountVisitor", "", StatCall::Min },
    {"Most Tags on a Feature", "hoot::TagCountVisitor", "", StatCall::Max },
    {"Average Tags Per Feature", "hoot::TagCountVisitor", "", StatCall::Average },

    {"Least Information Tags on a Feature", "hoot::TagCountVisitor", "", StatCall::InfoMin },
    {"Most Information Tags on a Feature", "hoot::TagCountVisitor", "", StatCall::InfoMax },
    {"Average Information Tags Per Feature", "hoot::TagCountVisitor", "", StatCall::InfoAverage },

    {"Features with Names", "hoot::ElementCountVisitor", "hoot::HasNameCriterion", StatCall::None },
    {"Unique Names", "hoot::UniqueNamesVisitor", "", StatCall::Stat },
    {"Unique Road Names", "hoot::UniqueNamesVisitor", "hoot::HighwayCriterion", StatCall::None },

    {"Unique Building Names", "hoot::UniqueNamesVisitor", "hoot::BuildingCriterion", StatCall::None },
    {"Meters of Linear Features", "hoot::LengthOfWaysVisitor", "hoot::LinearCriterion", StatCall::None },
    {"Meters Squared of Area Features", "hoot::CalculateAreaForStatsVisitor",   "hoot::StatsAreaCriterion", StatCall::None },
    {"Meters of Roads", "hoot::LengthOfWaysVisitor", "hoot::HighwayCriterion", StatCall::None },
    {"Meters Squared of Buildings", "hoot::CalculateAreaVisitor", "hoot::BuildingCriterion", StatCall::None },

    {"Bridges", "hoot::ElementCountVisitor", "hoot::BridgeCriterion", StatCall::None },
    {"Tunnels", "hoot::ElementCountVisitor", "hoot::TunnelCriterion", StatCall::None },
    {"One-Way Streets", "hoot::ElementCountVisitor", "hoot::OneWayCriterion", StatCall::None },
    {"Road Roundabouts", "hoot::ElementCountVisitor", "hoot::RoundaboutCriterion", StatCall::None },
    {"Multi-Use Buildings", "hoot::ElementCountVisitor", "hoot::MultiUseBuildingCriterion", StatCall::None },

    {"Buildings With Height Info", "hoot::BuildingHeightVisitor", "", StatCall::Stat },
    {"Shortest Building Height", "hoot::BuildingHeightVisitor", "", StatCall::Min },
    {"Tallest Building Height", "hoot::BuildingHeightVisitor", "", StatCall::Max },
    {"Average Height Per Building", "hoot::BuildingHeightVisitor", "", StatCall::Average },

    {"Buildings With Level Info", "hoot::BuildingLevelsVisitor", "", StatCall::Stat },
    {"Least Levels in a Building", "hoot::BuildingLevelsVisitor", "", StatCall::Min },
    {"Most Levels in a Building", "hoot::BuildingLevelsVisitor", "", StatCall::Max },
    {"Average Levels Per Building", "hoot::BuildingLevelsVisitor", "", StatCall::Average },

    {"Non-Building Areas", "hoot::ElementCountVisitor", "hoot::NonBuildingAreaCriterion", StatCall::None },
    {"Features with Addresses", "hoot::ElementCountVisitor", "hoot::HasAddressCriterion", StatCall::None },
    {"Total Addresses", "hoot::AddressCountVisitor", "", StatCall::Stat },
    {"Features with Phone Numbers", "hoot::ElementCountVisitor", "hoot::HasPhoneNumberCriterion", StatCall::None },
    {"Total Phone Numbers", "hoot::PhoneNumberCountVisitor", "", StatCall::Stat },
    {"Total Features", "hoot::FeatureCountVisitor", "", StatCall::Stat },
  };

  for (StatData d : quickStats) _interpretStatData(_constMap, d);
  if (!_quick) for (StatData d : slowStats) _interpretStatData(_constMap, d);

  if (!_quick)
  {
    FeatureCountVisitor featureCountVisitor;
    _applyVisitor(&featureCountVisitor);
    const long featureCount = featureCountVisitor.getCount();
    LOG_VART(featureCount);

    // _addStat("Total Features", featureCount);

    vector<shared_ptr<MatchCreator>> matchCreators =
      MatchFactory::getInstance().getCreators();
    LOG_VARD(matchCreators.size());
    double conflatedFeatureCount =
        _applyVisitor(new StatusCriterion(Status::Conflated), new FeatureCountVisitor());

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
        matchCandidateCountsData);
    LOG_VARD(matchCreators.size());
    SumNumericTagsVisitor tagSumVis(QStringList(MetadataTags::HootPoiPolygonPoisMerged()));
    _constMap->visitRo(tagSumVis);
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
    _addStat("Total Conflatable Features", conflatableFeatureCount);
    _addStat("Percentage of Total Features Conflatable",
             ((double)conflatableFeatureCount / (double)featureCount) * 100.0);
    const double numFeaturesMarkedForReview =
      _applyVisitor(new NeedsReviewCriterion(_constMap), new FeatureCountVisitor());

    CountUniqueReviewsVisitor curv;
    _constMap->visitRo(curv);
    const double numReviewsToBeMade = curv.getStat();
    const double untaggedFeatureCount =
      _applyVisitor(new NoInformationCriterion(),new FeatureCountVisitor());
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
    QMap<CreatorDescription::BaseFeatureType, double> conflatableFeatureCounts;
    for (CreatorDescription::BaseFeatureType ft = CreatorDescription::POI;
         ft < CreatorDescription::Unknown;
         ft = CreatorDescription::BaseFeatureType(ft+1))
    {
      conflatableFeatureCounts[ft] = 0.0;
    }

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
          //see comment in _generateFeatureStats as to why ElementCountVisitor is used here
          //instead of FeatureCountVisitor
          conflatablePolyCount =
              _applyVisitor(new PoiPolygonPolyCriterion(), new ElementCountVisitor());
        }
        _addStat("Polygons Conflatable by: " + matchCreatorName, conflatablePolyCount);
        conflatableFeatureCounts[CreatorDescription::Polygon] += conflatablePolyCount;

        double conflatablePoiPolyPoiCount = 0.0;
        if (!_inputIsConflatedMapOutput)
        {
          //see comment in _generateFeatureStats as to why ElementCountVisitor is used here
          //instead of FeatureCountVisitor
          conflatablePoiPolyPoiCount =
            _applyVisitor(new PoiPolygonPoiCriterion(), new ElementCountVisitor());
        }
        _addStat("POIs Conflatable by: " + matchCreatorName, conflatablePoiPolyPoiCount);
        conflatableFeatureCounts[CreatorDescription::PoiPolygonPOI] += conflatablePoiPolyPoiCount;
      }
      _addStat("Features Conflatable by: " + matchCreatorName, conflatableFeatureCountForFeatureType);
      conflatableFeatureCounts[featureType] += conflatableFeatureCountForFeatureType;
    }

    _addStat("Total Conflated Features", conflatedFeatureCount);
    _addStat("Percentage of Total Features Conflated",
              ((double)conflatedFeatureCount / (double)featureCount) * 100.0);
    _addStat("Total Features Marked for Review", numFeaturesMarkedForReview);
    _addStat("Percentage of Total Features Marked for Review",
             ((double)numFeaturesMarkedForReview / (double)featureCount) * 100.0);
    _addStat("Total Reviews to be Made", numReviewsToBeMade);
    const double unconflatedFeatureCount =
      _applyVisitor(new NotCriterion(new StatusCriterion(Status::Conflated)),
          new FeatureCountVisitor());
    _addStat("Total Unmatched Features", unconflatedFeatureCount);
    _addStat("Percentage of Total Features Unmatched",
             ((double)unconflatedFeatureCount / (double)featureCount) * 100.0);

    for (QMap<CreatorDescription::BaseFeatureType, double>::const_iterator it =
           conflatableFeatureCounts.begin();
         it != conflatableFeatureCounts.end(); ++it)
    {
      // Unknown does not get us a usable element criterion, so skip it
      if (hoot::CreatorDescription::Unknown != it.key())
      {
        _generateFeatureStats(it.key(), it.value(),
                              CreatorDescription::getFeatureCalcType(it.key()),
                              CreatorDescription::getElementCriterion(it.key(), map),
                              poisMergedIntoPolys);
      }
    }

    LongestTagVisitor v2;
    _applyVisitor(&v2);
    _addStat("Longest Tag", v2.getStat());

    // TODO: this should be moved into _generateFeatureStats
    LOG_DEBUG("config script: " + ConfigOptions().getStatsTranslateScript());
    if (ConfigOptions().getStatsTranslateScript() != "")
    {
      shared_ptr<ScriptSchemaTranslator> st(
        ScriptSchemaTranslatorFactory::getInstance().createTranslator(
          ConfigOptions().getStatsTranslateScript()));
      st->setErrorTreatment(StrictOff);
      TranslatedTagCountVisitor tcv(st);
      _applyVisitor(&tcv);
      _addStat("Translated Populated Tag Percent", tcv.getStat());
      _addStat("Translated Populated Tags", tcv.getPopulatedCount());
      _addStat("Translated Default Tags", tcv.getDefaultCount());
      _addStat("Translated Null Tags", tcv.getNullCount());

      _addStat("Building Translated Populated Tag Percent",
        _applyVisitor(new BuildingCriterion(map), new TranslatedTagCountVisitor(st)));
      _addStat("Road Translated Populated Tag Percent",
        _applyVisitor(new HighwayCriterion(map), new TranslatedTagCountVisitor(st)));
      _addStat("POI Translated Populated Tag Percent",
        _applyVisitor(new PoiCriterion(), new TranslatedTagCountVisitor(st)));
      _addStat("Waterway Translated Populated Tag Percent",
        _applyVisitor(new LinearWaterwayCriterion(), new TranslatedTagCountVisitor(st)));
      _addStat("Polygon Conflatable POI Translated Populated Tag Percent",
        _applyVisitor(new PoiPolygonPoiCriterion(), new TranslatedTagCountVisitor(st)));
      _addStat("Polygon Translated Populated Tag Percent",
        _applyVisitor(new PoiPolygonPolyCriterion(), new TranslatedTagCountVisitor(st)));
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

void CalculateStatsOp::_addStat(const QString& name, double value)
{
  _stats.append(SingleStat(name,value));
}

void CalculateStatsOp::_addStat(const char* name, double value)
{
  _stats.append(SingleStat(QString(name),value));
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
        pCrit = shared_ptr<ElementCriterion>(
            static_cast<ElementCriterion *>(
              Factory::getInstance().constructObject<ElementCriterion>(d.criterion)));

        // make sure the map is set before we use it
        ConstOsmMapConsumer* pMapConsumer = dynamic_cast<ConstOsmMapConsumer*>(pCrit.get());
        if(pMapConsumer) pMapConsumer->setOsmMap(constMap.get());
      }
      catch (...)
      {
        throw HootException(QString("Unable to construct criterion '%1' in stats data entry '%2'").arg(d.criterion, d.name) );
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
        pCriterionVisitor = shared_ptr<ConstElementVisitor>(
              static_cast<ConstElementVisitor *>(
                Factory::getInstance().constructObject<ElementVisitor>(d.visitor)));
      }
      catch (...)
      {
        throw HootException(QString("Unable to construct visitor '%1' in stats data entry '%2'").arg(d.visitor, d.name) );
      }

      FilteredVisitor filteredVisitor = FilteredVisitor(pCrit, ConstElementVisitorPtr(pCriterionVisitor));
      val = _applyVisitor(filteredVisitor);
    }
    else
    {
      if (d.statCall == None)
      {
        throw HootException(QString("Stats data entry '%1' is invalid").arg(d.name) );
      }
      else
      {
        shared_ptr<ConstElementVisitor> pVisitor;

        if (_appliedVisitorCache.contains(d.visitor))
        {
          pVisitor = _appliedVisitorCache[d.visitor];
        }
        else
        {
          try
          {
            pVisitor = shared_ptr<ConstElementVisitor>(
                  static_cast<ConstElementVisitor *>(
                    Factory::getInstance().constructObject<ElementVisitor>(d.visitor)));
          }
          catch (...)
          {
            throw HootException(QString("Unable to construct visitor '%1' in stats data entry '%2'").arg(d.visitor, d.name) );
          }

          // without criterion, apply the visitor directly and interpret as NumericStatistic
          _applyVisitor(pVisitor.get());
          _appliedVisitorCache[d.visitor] = pVisitor;
        }

        SingleStatistic* ss = dynamic_cast<SingleStatistic*>(pVisitor.get());
        NumericStatistic* ns = NULL;

        if (d.statCall != Stat)
        {
          ns = dynamic_cast<NumericStatistic*>(pVisitor.get());
        }

        switch (d.statCall)
        {
          case None: val = 0; break;
          case Min: val = ns->getMin(); break;
          case Max: val = ns->getMax(); break;
          case Average: val = ns->getAverage(); break;
          case Stat: val = ss->getStat(); break;
          case InfoCount: val = ns->getInformationCount(); break;
          case InfoMin: val = ns->getInformationMin(); break;
          case InfoMax: val = ns->getInformationMax(); break;
          case InfoAverage: val = ns->getInformationAverage(); break;
          case InfoDiff: val = ns->getInformationCountDiff(); break;
        }
      }
    }

    //LOG_VARD(val);
    //_addStat("-------> " + d.name, val);
    _addStat(d.name, val);
  }
}

bool CalculateStatsOp::_matchDescriptorCompare(const CreatorDescription& m1,
                                               const CreatorDescription& m2)
{
  return m1.className > m2.className;
}

double CalculateStatsOp::_applyVisitor(ElementCriterion* pCrit, ConstElementVisitor* pVis)
{
  return _applyVisitor(FilteredVisitor(pCrit, pVis));
}

double CalculateStatsOp::_applyVisitor(const FilteredVisitor& v)
{
  boost::any emptyVisitorData;
  return _applyVisitor(v, emptyVisitorData);
}

double CalculateStatsOp::_applyVisitor(const FilteredVisitor& v, boost::any& visitorData)
{
  // this is a hack to let C++ pass v as a temporary. Bad Jason.
  FilteredVisitor* fv = const_cast<FilteredVisitor*>(&v);
  shared_ptr<FilteredVisitor> critFv;
  if (_criterion)
  {
    critFv.reset(new FilteredVisitor(*_criterion, *fv));
    fv = critFv.get();
  }
  SingleStatistic* ss = dynamic_cast<SingleStatistic*>(&v.getChildVisitor());
  assert(ss != 0);

  _constMap->visitRo(*fv);

  DataProducer* dataProducer = dynamic_cast<DataProducer*>(&v.getChildVisitor());
  if (dataProducer != 0)
  {
    visitorData = dataProducer->getData();
  }

  return ss->getStat();
}

void CalculateStatsOp::_applyVisitor(ConstElementVisitor *v)
{
  shared_ptr<FilteredVisitor> critFv;
  if (_criterion)
  {
    critFv.reset(new FilteredVisitor(*_criterion, *v));
    v = critFv.get();
  }
  _constMap->visitRo(*v);
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

void CalculateStatsOp::_generateFeatureStats(const CreatorDescription::BaseFeatureType& featureType,
                                             const float conflatableCount,
                                             const CreatorDescription::FeatureCalcType& type,
                                             ElementCriterionPtr criterion,
                                             const long poisMergedIntoPolys)
{
  LOG_VARD(poisMergedIntoPolys);
  const QString description = CreatorDescription::baseFeatureTypeToString(featureType);
  LOG_VARD(description);

  double totalFeatures = 0.0;
  totalFeatures = _applyVisitor(FilteredVisitor(criterion->clone(), _getElementVisitorForFeatureType(featureType)));
  LOG_VARD(totalFeatures);
  _addStat(QString("%1s").arg(description), totalFeatures);
  _addStat(QString("Conflatable %1s").arg(description), conflatableCount);

  double conflatedFeatureCount =
    _applyVisitor(
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
      FilteredVisitor(
        ChainCriterion(
          new NeedsReviewCriterion(_constMap),
          criterion->clone()),
      _getElementVisitorForFeatureType(featureType)));
  _addStat(QString("Conflated %1s").arg(description), conflatedFeatureCount);
  _addStat(QString("%1s Marked for Review").arg(description), featuresMarkedForReview);
  const double numFeatureReviewsToBeMade =
    _applyVisitor(
       FilteredVisitor(
         criterion->clone(),
         ConstElementVisitorPtr(new CountUniqueReviewsVisitor())));
  _addStat(QString("%1 Reviews to be Made").arg(description), numFeatureReviewsToBeMade);

  double unconflatedFeatureCount =
    _applyVisitor(
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
  _addStat(QString("Unmatched %1s").arg(description), unconflatedFeatureCount);

  if (type == CreatorDescription::CalcTypeLength)
  {
    _addStat(QString("Meters of %1 Processed by Conflation").arg(description),
      _applyVisitor(
        FilteredVisitor(ChainCriterion(ElementCriterionPtr(new StatusCriterion(Status::Conflated)),
        criterion->clone()), ConstElementVisitorPtr(new LengthOfWaysVisitor()))));
  }
  else if (type == CreatorDescription::CalcTypeArea)
  {
    _addStat(QString("Meters Squared of %1s Processed by Conflation").arg(description),
      _applyVisitor(FilteredVisitor(ChainCriterion(ElementCriterionPtr(new StatusCriterion(Status::Conflated)),
        criterion->clone()), ConstElementVisitorPtr(new CalculateAreaVisitor()))));
  }

  double percentageOfTotalFeaturesConflated = 0.0;
  if (totalFeatures > 0.0)
  {
    percentageOfTotalFeaturesConflated =
      ((double)conflatedFeatureCount / (double)totalFeatures) * 100.0;
  }
  LOG_VARD(percentageOfTotalFeaturesConflated);
  _addStat(QString("Percentage of %1s Conflated").arg(description), percentageOfTotalFeaturesConflated);
  double percentageOfTotalFeaturesMarkedForReview = 0.0;
  if (totalFeatures > 0.0)
  {
    percentageOfTotalFeaturesMarkedForReview =
      ((double)featuresMarkedForReview / (double)totalFeatures) * 100.0;
  }
  _addStat(QString("Percentage of %1s Marked for Review").arg(description),
           percentageOfTotalFeaturesMarkedForReview);
  double percentageOfTotalFeaturesUnconflated = 0.0;
  if (totalFeatures > 0.0)
  {
    percentageOfTotalFeaturesUnconflated =
      ((double)unconflatedFeatureCount / (double)totalFeatures) * 100.0;
  }
  LOG_VARD(percentageOfTotalFeaturesUnconflated);
  _addStat(QString("Percentage of Unmatched %1s").arg(description), percentageOfTotalFeaturesUnconflated);
}

}
