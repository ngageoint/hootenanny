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
#include "PoiRfClassifier.h"

// hoot
#include <hoot/core/algorithms/aggregator/MeanAggregator.h>
#include <hoot/core/algorithms/aggregator/RmseAggregator.h>
#include <hoot/core/algorithms/aggregator/SigmaAggregator.h>
#include <hoot/core/algorithms/aggregator/QuantileAggregator.h>
#include <hoot/core/algorithms/splitter/MultiLineStringSplitter.h>
#include <hoot/core/algorithms/string/ExactStringDistance.h>
#include <hoot/core/algorithms/string/LevenshteinDistance.h>
#include <hoot/core/algorithms/string/MaxWordSetDistance.h>
#include <hoot/core/algorithms/string/MeanWordSetDistance.h>
#include <hoot/core/algorithms/string/Soundex.h>
#include <hoot/core/algorithms/extractors/AngleHistogramExtractor.h>
#include <hoot/core/algorithms/extractors/AttributeScoreExtractor.h>
#include <hoot/core/algorithms/extractors/CentroidDistanceExtractor.h>
#include <hoot/core/algorithms/extractors/CompactnessExtractor.h>
#include <hoot/core/algorithms/extractors/DistanceScoreExtractor.h>
#include <hoot/core/algorithms/extractors/EdgeDistanceExtractor.h>
#include <hoot/core/algorithms/extractors/LengthScoreExtractor.h>
#include <hoot/core/algorithms/extractors/NameExtractor.h>
#include <hoot/core/algorithms/extractors/OverlapExtractor.h>
#include <hoot/core/algorithms/extractors/SmallerOverlapExtractor.h>
#include <hoot/core/algorithms/extractors/WeightedMetricDistanceExtractor.h>
#include <hoot/core/algorithms/extractors/WeightedShapeDistanceExtractor.h>
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/language/ToEnglishTranslateStringDistance.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

// Standard
#include <fstream>

// Qt
#include <QFile>

using namespace std;
using namespace Tgs;

namespace hoot
{

int PoiRfClassifier::logWarnCount = 0;

PoiRfClassifier::PoiRfClassifier()
{
  _createAllExtractors();
  //_createTestExtractors();

  QString path = ConfPath::search(ConfigOptions().getConflateMatchPoiModel());
  LOG_INFO("Loading POI model from: " << path);

  QFile file(path.toLatin1().data());
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
  catch (const Exception&)
  {
    file.close();
    throw;
  }

  vector<string> factorLabels = _rf->getFactorLabels();
  LOG_VART(factorLabels);

  QStringList extractorNames;
  for (size_t i = 0; i < _extractors.size(); i++)
  {
    extractorNames.append(_extractors[i]->getName().replace(QRegExp("[^\\w]"), "_"));
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
  LOG_VART(extractorNames);
  LOG_VART(missingExtractors);
  LOG_VART(_rfFactorLabels);

  if (!missingExtractors.empty())
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN(
        "An extractor used by the model is not being calculated. We will still try, but this will "
        "undoubtably result in poor quality matches. Missing extractors: " << missingExtractors);
      LOG_TRACE("Available extractors: " << extractorNames);
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }
}

MatchClassification PoiRfClassifier::classify(const ConstOsmMapPtr& map,
  ElementId eid1, ElementId eid2)
{
  MatchClassification p;

  std::map<QString, double> features = getFeatures(map, eid1, eid2);

  if (features.empty())
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

  return p;
}

void PoiRfClassifier::_createAllExtractors()
{
  _extractors.clear();
  vector<QString> extractorNames =
    Factory::getInstance().getObjectNamesByBase(FeatureExtractor::className());
  LOG_VART(extractorNames);

//  for (size_t i = 0; i < extractorNames.size(); i++)
//  {
//    FeatureExtractor* fe = Factory::getInstance().constructObject<FeatureExtractor>(
//      extractorNames[i]);
//    _extractors.push_back(FeatureExtractorPtr(fe));
//  }

  // These are all the factors that seem reasonable. Many of the other factors will overtrain due
  // to distance values and such.
  _extractors.push_back(FeatureExtractorPtr(new AttributeScoreExtractor(false)));
  _extractors.push_back(FeatureExtractorPtr(new AttributeScoreExtractor(true)));

  // at some point names will make sense, but for now there isn't enough name data.
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new ExactStringDistance()))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new MaxWordSetDistance(
                        StringDistancePtr(new ExactStringDistance()))))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new MeanWordSetDistance(
                        StringDistancePtr(new ExactStringDistance()))))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new ToEnglishTranslateStringDistance(
                        StringDistancePtr(new ExactStringDistance()))))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new ToEnglishTranslateStringDistance(
                        StringDistancePtr(new MaxWordSetDistance(
                        StringDistancePtr(new ExactStringDistance()))))))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new ToEnglishTranslateStringDistance(
                        StringDistancePtr(new MeanWordSetDistance(
                        StringDistancePtr(new ExactStringDistance()))))))));

  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new LevenshteinDistance()))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new MaxWordSetDistance(
                        StringDistancePtr(new LevenshteinDistance()))))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new MeanWordSetDistance(
                        StringDistancePtr(new LevenshteinDistance()))))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new ToEnglishTranslateStringDistance(
                        StringDistancePtr(new LevenshteinDistance()))))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new ToEnglishTranslateStringDistance(
                        StringDistancePtr(new MaxWordSetDistance(
                        StringDistancePtr(new LevenshteinDistance()))))))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new ToEnglishTranslateStringDistance(
                        StringDistancePtr(new MeanWordSetDistance(
                        StringDistancePtr(new LevenshteinDistance()))))))));

  for (double a = 1.0; a < 1.8; a += 0.05)
  {
    _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                          StringDistancePtr(new MeanWordSetDistance(
                          StringDistancePtr(new LevenshteinDistance(a)))))));
    _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                          StringDistancePtr(new ToEnglishTranslateStringDistance(
                          StringDistancePtr(new MeanWordSetDistance(
                          StringDistancePtr(new LevenshteinDistance(a)))))))));
  }

  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new Soundex()))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new MaxWordSetDistance(
                        StringDistancePtr(new Soundex()))))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new MeanWordSetDistance(
                        StringDistancePtr(new Soundex()))))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new ToEnglishTranslateStringDistance(
                        StringDistancePtr(new Soundex()))))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new ToEnglishTranslateStringDistance(
                        StringDistancePtr(new MaxWordSetDistance(
                        StringDistancePtr(new Soundex()))))))));
  _extractors.push_back(FeatureExtractorPtr(new NameExtractor(
                        StringDistancePtr(new ToEnglishTranslateStringDistance(
                        StringDistancePtr(new MeanWordSetDistance(
                        StringDistancePtr(new Soundex()))))))));
}

void PoiRfClassifier::_createTestExtractors()
{
  _extractors.clear();

  _extractors.push_back(FeatureExtractorPtr(new EdgeDistanceExtractor(
                        ValueAggregatorPtr(new RmseAggregator()))));
  _extractors.push_back(FeatureExtractorPtr(new EdgeDistanceExtractor(
                        ValueAggregatorPtr(new SigmaAggregator()))));
  _extractors.push_back(FeatureExtractorPtr(new AngleHistogramExtractor()));
//  _extractors.push_back(FeatureExtractorPtr(new AttributeScoreExtractor(false)));
//  _extractors.push_back(FeatureExtractorPtr(new AttributeScoreExtractor(true)));
//  _extractors.push_back(FeatureExtractorPtr(new WeightedShapeDistanceExtractor()));

//  _extractors.push_back(FeatureExtractorPtr(new WeightedMetricDistanceExtractor(
//    new MeanAggregator(), new SigmaAggregator())));
  _extractors.push_back(FeatureExtractorPtr(new WeightedMetricDistanceExtractor(
                        ValueAggregatorPtr(new MeanAggregator()),
                        ValueAggregatorPtr(new RmseAggregator()))));

}

map<QString, double> PoiRfClassifier::getFeatures(const ConstOsmMapPtr& m,
  ElementId eid1, ElementId eid2) const
{
  map<QString, double> result;

  ConstElementPtr e1 = m->getElement(eid1);
  ConstElementPtr e2 = m->getElement(eid2);

  for (size_t i = 0; i < _extractors.size(); i++)
  {
    double v = _extractors[i]->extract(*m, e1, e2);
    // if it isn't null then include it.
    if (!FeatureExtractor::isNull(v))
    {
      QString factorName = _extractors[i]->getName().replace(QRegExp("[^\\w]"), "_");
      result[factorName] = v;
    }
  }

  return result;
}

}
