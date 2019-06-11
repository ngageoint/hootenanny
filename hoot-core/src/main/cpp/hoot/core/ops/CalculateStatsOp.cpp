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
#include <hoot/core/util/MapProjector.h>
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
  _inputIsConflatedMapOutput(inputIsConflatedMapOutput)
{
  _readGenericStatsData();
}

CalculateStatsOp::CalculateStatsOp(ElementCriterionPtr criterion, QString mapName,
                                   bool inputIsConflatedMapOutput) :
  _pConf(&conf()),
  _criterion(criterion),
  _mapName(mapName),
  _quick(false),
  _inputIsConflatedMapOutput(inputIsConflatedMapOutput)
{
  LOG_VART(_inputIsConflatedMapOutput);
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
  QList<StatData>* pCurr = NULL;
  QHash<QString,StatCall> enumLookup({ {"stat", Stat},
                                       {"min", Min},
                                       {"max", Max},
                                       {"average", Average},
                                       {"infocount", InfoCount},
                                       {"infomin", InfoMin},
                                       {"infomax", InfoMax},
                                       {"infoaverage", InfoAverage},
                                       {"infodiff", InfoDiff}});

  foreach (bpt::ptree::value_type listType, propPtree)
  {
    if (listType.first == "quick") pCurr = &_quickStatData;
    else if (listType.first == "slow") pCurr = &_slowStatData;
    else
    {
      throw HootException("Invalid stats data list name '" + listType.first + "' in " + statsFileName.toStdString() + ". Allowed: 'quick' or 'slow'.");
    }

    foreach (bpt::ptree::value_type entry, listType.second)
    {
      StatData newStatData;

      foreach (bpt::ptree::value_type data, entry.second)
      {
        QString key = QString::fromStdString(data.first).toLower();
        QString val = QString::fromStdString(data.second.data());

        if (key== "name") newStatData.name = val;
        else if (key== "visitor") newStatData.visitor = val;
        else if (key== "criterion") newStatData.criterion = val;
        else if (key== "statcall")
        {
          QString enumVal = val.toLower();
          if (enumLookup.contains(enumVal))
          {
            newStatData.statCall = enumLookup[enumVal];
          }
          else
          {
            throw HootException("Invalid stats data field '" + val.toStdString() + "' in " + statsFileName.toStdString());
          }
        }
        else
        {
          throw HootException("Invalid stats data field '" + val.toStdString() + "' in " + statsFileName.toStdString());
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

  for (StatData d : _quickStatData) _interpretStatData(_constMap, d);
  if (!_quick) for (StatData d : _slowStatData) _interpretStatData(_constMap, d);

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
      SchemaTranslatedTagCountVisitor tcv(st);
      _applyVisitor(&tcv);
      _addStat("Translated Populated Tag Percent", tcv.getStat());
      _addStat("Translated Populated Tags", tcv.getPopulatedCount());
      _addStat("Translated Default Tags", tcv.getDefaultCount());
      _addStat("Translated Null Tags", tcv.getNullCount());

      _addStat("Building Translated Populated Tag Percent",
        _applyVisitor(new BuildingCriterion(map), new SchemaTranslatedTagCountVisitor(st)));
      _addStat("Road Translated Populated Tag Percent",
        _applyVisitor(new HighwayCriterion(map), new SchemaTranslatedTagCountVisitor(st)));
      _addStat("POI Translated Populated Tag Percent",
        _applyVisitor(new PoiCriterion(), new SchemaTranslatedTagCountVisitor(st)));
      _addStat("Waterway Translated Populated Tag Percent",
        _applyVisitor(new LinearWaterwayCriterion(), new SchemaTranslatedTagCountVisitor(st)));
      _addStat("Polygon Conflatable POI Translated Populated Tag Percent",
        _applyVisitor(new PoiPolygonPoiCriterion(), new SchemaTranslatedTagCountVisitor(st)));
      _addStat("Polygon Translated Populated Tag Percent",
        _applyVisitor(new PoiPolygonPolyCriterion(), new SchemaTranslatedTagCountVisitor(st)));
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
      val = _applyVisitor(filteredVisitor, d.statCall);
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

double CalculateStatsOp::_applyVisitor(ElementCriterion* pCrit, ConstElementVisitor* pVis, StatCall call)
{
  return _applyVisitor(FilteredVisitor(pCrit, pVis), call);
}

double CalculateStatsOp::_applyVisitor(const FilteredVisitor& v, StatCall call)
{
  boost::any emptyVisitorData;
  return _applyVisitor(v, emptyVisitorData, call);
}

double CalculateStatsOp::_applyVisitor(const FilteredVisitor& v, boost::any& visitorData, StatCall call)
{
  // this is a hack to let C++ pass v as a temporary. Bad Jason.
  FilteredVisitor* fv = const_cast<FilteredVisitor*>(&v);
  shared_ptr<FilteredVisitor> critFv;
  if (_criterion)
  {
    critFv.reset(new FilteredVisitor(*_criterion, *fv));
    fv = critFv.get();
  }

  ElementVisitor& childVisitor = v.getChildVisitor();

  _constMap->visitRo(*fv);

  DataProducer* dataProducer = dynamic_cast<DataProducer*>(&childVisitor);
  if (dataProducer != 0)
  {
    visitorData = dataProducer->getData();
  }

  return GetRequestedStatValue(&childVisitor, call);
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
