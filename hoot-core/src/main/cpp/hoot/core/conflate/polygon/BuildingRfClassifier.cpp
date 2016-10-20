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
#include "BuildingRfClassifier.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/algorithms/aggregator/MeanAggregator.h>
#include <hoot/core/algorithms/aggregator/RmseAggregator.h>
#include <hoot/core/algorithms/aggregator/QuantileAggregator.h>
#include <hoot/core/algorithms/ExactStringDistance.h>
#include <hoot/core/algorithms/KskipBigramDistance.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/algorithms/MaxWordSetDistance.h>
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/algorithms/Soundex.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/conflate/polygon/extractors/BufferedOverlapExtractor.h>
#include <hoot/core/conflate/polygon/extractors/CentroidDistanceExtractor.h>
#include <hoot/core/conflate/polygon/extractors/CompactnessExtractor.h>
#include <hoot/core/conflate/polygon/extractors/EdgeDistanceExtractor.h>
#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
#include <hoot/core/conflate/polygon/extractors/OverlapExtractor.h>
#include <hoot/core/conflate/polygon/extractors/SmallerOverlapExtractor.h>
#include <hoot/core/conflate/polygon/extractors/AngleHistogramExtractor.h>
#include <hoot/core/schema/TranslateStringDistance.h>

// Standard
#include <sstream>

// tgs
#include <tgs/RandomForest/RandomForest.h>

namespace hoot
{

BuildingRfClassifier::BuildingRfClassifier()
{
}

void BuildingRfClassifier::_createAllExtractors() const
{
  _extractors.clear();
  vector<std::string> extractorNames = Factory::getInstance().getObjectNamesByBase(
    FeatureExtractor::className());

  for (size_t i = 0; i < extractorNames.size(); i++)
  {
    FeatureExtractor* fe = Factory::getInstance().constructObject<FeatureExtractor>(
      extractorNames[i]);
    _extractors.push_back(shared_ptr<FeatureExtractor>(fe));
  }

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

  for (double b = -0.3; b < 0.0; b += 0.1)
  {
    _extractors.push_back(shared_ptr<FeatureExtractor>(new BufferedOverlapExtractor(b)));
  }

  for (double b = 0.1; b < 0.5; b += 0.1)
  {
    _extractors.push_back(shared_ptr<FeatureExtractor>(new BufferedOverlapExtractor(b)));
  }

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

  // checking to see if averaging the top portion of word helps. It doesn't appear to make
  // any difference.
//    for (double p = 0.0; p < 1.0; p += 0.2)
//    {
//      _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//        new TranslateStringDistance(new MeanWordSetDistance(new ExactStringDistance(), p)))));
//      _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//        new MeanWordSetDistance(new ExactStringDistance(), p))));

//      _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//        new TranslateStringDistance(new MeanWordSetDistance(new LevenshteinDistance(), p)))));
//      _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
//        new MeanWordSetDistance(new LevenshteinDistance(), p))));
//    }

  _extractors.push_back(shared_ptr<FeatureExtractor>(
    new EdgeDistanceExtractor(new MeanAggregator())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(
    new EdgeDistanceExtractor(new RmseAggregator())));

  for (double q = 0; q < 1.0; q += 0.05)
  {
    _extractors.push_back(shared_ptr<FeatureExtractor>(
      new EdgeDistanceExtractor(new QuantileAggregator(q))));
  }

  for (double r = 0.0; r <= 1.0; r += 0.05)
  {
    _extractors.push_back(shared_ptr<FeatureExtractor>(new AngleHistogramExtractor(r)));
  }

  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new KskipBigramDistance())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new MaxWordSetDistance(new KskipBigramDistance()))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new MeanWordSetDistance(new KskipBigramDistance()))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new TranslateStringDistance(new KskipBigramDistance()))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new TranslateStringDistance(new MaxWordSetDistance(new KskipBigramDistance())))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new TranslateStringDistance(new MeanWordSetDistance(new KskipBigramDistance())))));
}

void BuildingRfClassifier::_createBestExtractors() const
{
  _extractors.clear();

  // this set was determined with experimentation using the Jakarta building data and weka.
  _extractors.push_back(shared_ptr<FeatureExtractor>(new AngleHistogramExtractor(0.0)));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new AngleHistogramExtractor(0.3)));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new CompactnessExtractor()));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new SmallerOverlapExtractor()));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
    new TranslateStringDistance(new MeanWordSetDistance(new LevenshteinDistance(1.45))))));
  _extractors.push_back(shared_ptr<FeatureExtractor>(
    new EdgeDistanceExtractor(new QuantileAggregator(0.4))));
}

void BuildingRfClassifier::_createExtractors() const
{
  _createBestExtractors();
}

}
