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
#include "HighwayRfClassifier.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/algorithms/aggregator/MeanAggregator.h>
#include <hoot/core/algorithms/aggregator/RmseAggregator.h>
#include <hoot/core/algorithms/aggregator/SigmaAggregator.h>
#include <hoot/core/algorithms/aggregator/QuantileAggregator.h>
#include <hoot/core/algorithms/ExactStringDistance.h>
#include <hoot/core/algorithms/MaxWordSetDistance.h>
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/algorithms/MultiLineStringSplitter.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/algorithms/Soundex.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/conflate/extractors/AttributeScoreExtractor.h>
#include <hoot/core/conflate/extractors/DistanceScoreExtractor.h>
#include <hoot/core/conflate/extractors/LengthScoreExtractor.h>
#include <hoot/core/conflate/extractors/WeightedMetricDistanceExtractor.h>
#include <hoot/core/conflate/extractors/WeightedShapeDistanceExtractor.h>
#include <hoot/core/conflate/polygon/extractors/BufferedOverlapExtractor.h>
#include <hoot/core/conflate/polygon/extractors/CentroidDistanceExtractor.h>
#include <hoot/core/conflate/polygon/extractors/CompactnessExtractor.h>
#include <hoot/core/conflate/polygon/extractors/EdgeDistanceExtractor.h>
#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
#include <hoot/core/conflate/polygon/extractors/OverlapExtractor.h>
#include <hoot/core/conflate/polygon/extractors/SmallerOverlapExtractor.h>
#include <hoot/core/conflate/polygon/extractors/AngleHistogramExtractor.h>
#include <hoot/core/ops/CopySubsetOp.h>
#include <hoot/core/schema/TranslateStringDistance.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>

// Standard
#include <fstream>

// Qt
#include <QFile>

namespace hoot
{

HOOT_FACTORY_REGISTER(HighwayClassifier, HighwayRfClassifier)

HighwayRfClassifier::HighwayRfClassifier()
{
  // we do a lazy initialization
}

MatchClassification HighwayRfClassifier::classify(const ConstOsmMapPtr& map,
  ElementId eid1, ElementId eid2, const WaySublineMatchString& match)
{
  _init();

  MatchClassification p;

  if (match.isValid() == false)
  {
    p.setMiss();
  }
  else
  {
    std::map<QString, double> features = getFeatures(map, eid1, eid2, match);

    if (features.size() == 0)
    {
      p.setMiss();
    }
    else
    {
      vector<double> row;
      row.reserve(_rfFactorLabels.size());
      for (size_t i = 0; i < (size_t)_rfFactorLabels.size(); i++)
      {
        std::map<QString, double>::const_iterator it = features.find(_rfFactorLabels[i]);
        if (it != features.end())
        {
          row.push_back(it->second);
        }
        else
        {
          // for now we're using -1 instead of null.
          row.push_back(-1);
        }
      }
      std::map<string, double> scores;
      _rf->classifyVector(row, scores);

      p.setMissP(scores["miss"]);
      p.setMatchP(scores["match"]);
      p.setReviewP(scores["review"]);
    }
  }

  return p;
}

void HighwayRfClassifier::_createAllExtractors() const
{
  _extractors.clear();
//  vector<std::string> extractorNames = Factory::getInstance().getObjectNamesByBase(
//    FeatureExtractor::className());

//  for (size_t i = 0; i < extractorNames.size(); i++)
//  {
//    FeatureExtractor* fe = Factory::getInstance().constructObject<FeatureExtractor>(
//      extractorNames[i]);
//    _extractors.push_back(shared_ptr<FeatureExtractor>(fe));
//  }

  // These are all the factors that seem reasonable. Many of the other factors will overtrain due
  // to distance values and such.
  _extractors.push_back(shared_ptr<FeatureExtractor>(new AngleHistogramExtractor()));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new AttributeScoreExtractor(NULL, false)));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new AttributeScoreExtractor(NULL, true)));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new WeightedShapeDistanceExtractor()));

  _extractors.push_back(shared_ptr<FeatureExtractor>(new WeightedMetricDistanceExtractor(
    new MeanAggregator(), new SigmaAggregator())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new WeightedMetricDistanceExtractor(
    new MeanAggregator(), new RmseAggregator())));

  _extractors.push_back(shared_ptr<FeatureExtractor>(
    new EdgeDistanceExtractor(new RmseAggregator())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(
    new EdgeDistanceExtractor(new SigmaAggregator())));

  // at some point names will make sense, but for now there isn't enough name data.
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new ExactStringDistance())));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new MaxWordSetDistance(new ExactStringDistance()))));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new MeanWordSetDistance(new ExactStringDistance()))));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new TranslateStringDistance(new ExactStringDistance()))));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new TranslateStringDistance(new MaxWordSetDistance(new ExactStringDistance())))));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new TranslateStringDistance(new MeanWordSetDistance(new ExactStringDistance())))));

//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new LevenshteinDistance())));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new MaxWordSetDistance(new LevenshteinDistance()))));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new MeanWordSetDistance(new LevenshteinDistance()))));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new TranslateStringDistance(new LevenshteinDistance()))));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new TranslateStringDistance(new MaxWordSetDistance(new LevenshteinDistance())))));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new TranslateStringDistance(new MeanWordSetDistance(new LevenshteinDistance())))));

//  for (double a = 1.0; a < 1.8; a += 0.05)
//  {
//    _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//      new MeanWordSetDistance(new LevenshteinDistance(a)))));
//    _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//      new TranslateStringDistance(new MeanWordSetDistance(new LevenshteinDistance(a))))));
//  }

//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(new Soundex())));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new MaxWordSetDistance(new Soundex()))));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new MeanWordSetDistance(new Soundex()))));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new TranslateStringDistance(new Soundex()))));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new TranslateStringDistance(new MaxWordSetDistance(new Soundex())))));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//    new TranslateStringDistance(new MeanWordSetDistance(new Soundex())))));

}

void HighwayRfClassifier::_createTestExtractors() const
{
  _extractors.clear();

  _extractors.push_back(shared_ptr<FeatureExtractor>(
    new EdgeDistanceExtractor(new RmseAggregator())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(
    new EdgeDistanceExtractor(new SigmaAggregator())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new AngleHistogramExtractor()));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new AttributeScoreExtractor(false)));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new AttributeScoreExtractor(true)));
//  _extractors.push_back(shared_ptr<FeatureExtractor>(new WeightedShapeDistanceExtractor()));

//  _extractors.push_back(shared_ptr<FeatureExtractor>(new WeightedMetricDistanceExtractor(
//    new MeanAggregator(), new SigmaAggregator())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new WeightedMetricDistanceExtractor(
    new MeanAggregator(), new RmseAggregator(), ConfigOptions().getSearchRadiusHighway())));

}

map<QString, double> HighwayRfClassifier::getFeatures(const shared_ptr<const OsmMap>& m,
  ElementId eid1, ElementId eid2, const WaySublineMatchString& match) const
{
  _init();

  map<QString, double> result;

  // convert match into elements in a new map.
  set<ElementId> eids;
  eids.insert(eid1);
  eids.insert(eid2);
  OsmMapPtr copiedMap(new OsmMap(m->getProjection()));
  CopySubsetOp(m, eids).apply(copiedMap);
  WaySublineMatchString copiedMatch(match, copiedMap);

  // split the shared line based on the matching subline
  ElementPtr match1, scraps1;
  ElementPtr match2, scraps2;
  WaySublineCollection string1 = copiedMatch.getSublineString1();
  WaySublineCollection string2 = copiedMatch.getSublineString2();

  MultiLineStringSplitter().split(copiedMap, string1, copiedMatch.getReverseVector1(), match1,
    scraps1);
  MultiLineStringSplitter().split(copiedMap, string2, copiedMatch.getReverseVector2(), match2,
    scraps2);

  if (!match1 || !match2)
  {
    LOG_WARN("Internal Error: Found a situation where the match after copy is invalid. Marking as "
             "needs review");
    LOG_VAR(eid1);
    LOG_VAR(eid2);
    LOG_VAR(match.toString());
    throw NeedsReviewException("Internal Error: Expected a matching subline, but got an empty "
                               "match. Please report this to hootenanny.help@digitalglobe.com.");
  }
  else
  {
    for (size_t i = 0; i < _extractors.size(); i++)
    {
      double v = _extractors[i]->extract(*copiedMap, match1, match2);
      // if it isn't null then include it.
      if (!FeatureExtractor::isNull(v))
      {
        QString factorName = QString::fromStdString(_extractors[i]->getName()).
            replace(QRegExp("[^\\w]"), "_");
        result[factorName] = v;
      }
    }
  }

  return result;
}

void HighwayRfClassifier::_init() const
{
  if (!_rf)
  {
    //_createAllExtractors();
    _createTestExtractors();

    QString path = ConfPath::search(ConfigOptions().getConflateMatchHighwayModel());
    LOG_INFO("Loading highway model from: " << path);

    QFile file(path.toAscii().data());
    if (!file.open(QIODevice::ReadOnly))
    {
      throw HootException("Error opening file: " + path);
    }
    _rf.reset(new RandomForest());
    try
    {
      _rf->importModel(file);
      file.close();
    }
    catch (const Exception& e)
    {
      file.close();
      throw e;
    }

    vector<string> factorLabels = _rf->getFactorLabels();
    LOG_VAR(factorLabels);

    QStringList extractorNames;
    for (size_t i = 0; i < _extractors.size(); i++)
    {
      extractorNames.append(QString::fromStdString(_extractors[i]->getName()).
                            replace(QRegExp("[^\\w]"), "_"));
    }

    QStringList missingExtractors;
    for (size_t i = 0; i < factorLabels.size(); i++)
    {
      QString fn = QString::fromStdString(factorLabels[i]);
      if (extractorNames.contains(fn) == false)
      {
        missingExtractors.append(fn);
      }
      _rfFactorLabels.append(fn);
    }
    LOG_VAR(extractorNames);
    LOG_VAR(missingExtractors);
    LOG_VAR(_rfFactorLabels);

    if (missingExtractors.size() > 0)
    {
      LOG_WARN("An extractor used by the model is not being calculated. We will still try, but");
      LOG_WARN("this will undoubtably result in poor quality matches. Missing extractors:");
      LOG_WARN(missingExtractors);
      LOG_WARN("Available extractors: " << extractorNames);
    }
  }
}

}
