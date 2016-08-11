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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonRfClassifier.h"

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
#include <hoot/core/conflate/extractors/WeightedMetricDistanceExtractor.h>
#include <hoot/core/conflate/extractors/WeightedShapeDistanceExtractor.h>
#include <hoot/core/conflate/polygon/extractors/BufferedOverlapExtractor.h>
#include <hoot/core/conflate/polygon/extractors/EdgeDistanceExtractor.h>
#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
#include <hoot/core/conflate/polygon/extractors/AngleHistogramExtractor.h>
#include <hoot/core/ops/CopySubsetOp.h>
#include <hoot/core/schema/TranslateStringDistance.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/algorithms/string/SqliteWordWeightDictionary.h>
#include <hoot/core/algorithms/string/WeightedWordDistance.h>

#include <hoot/core/conflate/polygon/extractors/CentroidDistanceExtractor.h>
#include <hoot/core/conflate/extractors/DistanceScoreExtractor.h>
#include <hoot/core/conflate/extractors/LengthScoreExtractor.h>
#include <hoot/core/conflate/polygon/extractors/CompactnessExtractor.h>
#include <hoot/core/conflate/polygon/extractors/OverlapExtractor.h>
#include <hoot/core/conflate/polygon/extractors/SmallerOverlapExtractor.h>

// Standard
#include <fstream>

// Qt
#include <QFile>

namespace hoot
{

PoiPolygonRfClassifier::PoiPolygonRfClassifier()
{
  _createAllExtractors();
}

void PoiPolygonRfClassifier::_createAllExtractors()
{
  _extractors.clear();
  vector<std::string> extractorNames = Factory::getInstance().getObjectNamesByBase(
    FeatureExtractor::className());
  LOG_VAR(extractorNames);

  for (size_t i = 0; i < extractorNames.size(); i++)
  {
    FeatureExtractor* fe = Factory::getInstance().constructObject<FeatureExtractor>(
      extractorNames[i]);
    _extractors.push_back(shared_ptr<FeatureExtractor>(fe));
  }

  // These are all the factors that seem reasonable. Many of the other factors will overtrain due
  // to distance values and such.
  _extractors.push_back(shared_ptr<FeatureExtractor>(new AttributeScoreExtractor(NULL, false)));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new AttributeScoreExtractor(NULL, true)));

  // at some point names will make sense, but for now there isn't enough name data.
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new ExactStringDistance())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new MaxWordSetDistance(new ExactStringDistance()))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new MeanWordSetDistance(new ExactStringDistance()))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new TranslateStringDistance(new ExactStringDistance()))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new TranslateStringDistance(new MaxWordSetDistance(new ExactStringDistance())))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new TranslateStringDistance(new MeanWordSetDistance(new ExactStringDistance())))));

  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new LevenshteinDistance())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new MaxWordSetDistance(new LevenshteinDistance()))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new MeanWordSetDistance(new LevenshteinDistance()))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new TranslateStringDistance(new LevenshteinDistance()))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new TranslateStringDistance(new MaxWordSetDistance(new LevenshteinDistance())))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new TranslateStringDistance(new MeanWordSetDistance(new LevenshteinDistance())))));

  for (double a = 1.0; a < 1.8; a += 0.05)
  {
    _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
      new MeanWordSetDistance(new LevenshteinDistance(a)))));
    _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
      new TranslateStringDistance(new MeanWordSetDistance(new LevenshteinDistance(a))))));
  }

  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(new Soundex())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new MaxWordSetDistance(new Soundex()))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new MeanWordSetDistance(new Soundex()))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new TranslateStringDistance(new Soundex()))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new TranslateStringDistance(new MaxWordSetDistance(new Soundex())))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new TranslateStringDistance(new MeanWordSetDistance(new Soundex())))));

   SqliteWordWeightDictionary* dict =
      new SqliteWordWeightDictionary(
        ConfPath::search(ConfigOptions().getWeightedWordDistanceDictionary()));
  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
      new NameExtractor(
        new TranslateStringDistance(
          new WeightedWordDistance(
            new LevenshteinDistance(ConfigOptions().getLevenshteinDistanceAlpha()), dict)))));

  _extractors.push_back(shared_ptr<FeatureExtractor>(
    new EdgeDistanceExtractor(new RmseAggregator())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(
    new EdgeDistanceExtractor(new SigmaAggregator())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new AngleHistogramExtractor()));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new AttributeScoreExtractor(NULL, false)));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new AttributeScoreExtractor(NULL, true)));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new WeightedShapeDistanceExtractor()));

  _extractors.push_back(shared_ptr<FeatureExtractor>(new WeightedMetricDistanceExtractor(
    new MeanAggregator(), new SigmaAggregator())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new WeightedMetricDistanceExtractor(
    new MeanAggregator(), new RmseAggregator())));

  for (double b = -0.3; b < 0.0; b += 0.1)
  {
    _extractors.push_back(shared_ptr<FeatureExtractor>(new BufferedOverlapExtractor(b)));
  }

  for (double b = 0.1; b < 0.5; b += 0.1)
  {
    _extractors.push_back(shared_ptr<FeatureExtractor>(new BufferedOverlapExtractor(b)));
  }

  for (double q = 0; q < 1.0; q += 0.05)
  {
    _extractors.push_back(shared_ptr<FeatureExtractor>(
      new EdgeDistanceExtractor(new QuantileAggregator(q))));
  }

  for (double r = 0.0; r <= 1.0; r += 0.05)
  {
    _extractors.push_back(shared_ptr<FeatureExtractor>(new AngleHistogramExtractor(r)));
  }
}

map<QString, double> PoiPolygonRfClassifier::getFeatures(const ConstOsmMapPtr& m,
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
      QString factorName = QString::fromStdString(_extractors[i]->getName()).
          replace(QRegExp("[^\\w]"), "_");
      result[factorName] = v;
    }
  }

  return result;
}

}
