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
#include "extractors/PoiPolygonNameScoreExtractor.h"
#include "extractors/PoiPolygonTypeScoreExtractor.h"
#include "extractors/PoiPolygonAddressScoreExtractor.h"
#include "extractors/PoiPolygonDistanceExtractor.h"
#include "extractors/PoiPolygonAlphaShapeDistanceExtractor.h"

// hoot
#include <hoot/core/Factory.h>
//#include <hoot/core/conflate/polygon/extractors/EuclideanDistanceExtractor.h>

namespace hoot
{

PoiPolygonRfClassifier::PoiPolygonRfClassifier()
{
  _createExtractors();
}

void PoiPolygonRfClassifier::_createExtractors()
{
  _extractors.clear();

  //_extractors.push_back(shared_ptr<FeatureExtractor>(new EuclideanDistanceExtractor()));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new PoiPolygonDistanceExtractor()));
  _extractors.push_back(shared_ptr<FeatureExtractor>(
    new PoiPolygonAlphaShapeDistanceExtractor()));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new PoiPolygonNameScoreExtractor()));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new PoiPolygonTypeScoreExtractor()));
  shared_ptr<PoiPolygonAddressScoreExtractor> addressExtractor1(
    new PoiPolygonAddressScoreExtractor());
  _extractors.push_back(addressExtractor1);
  shared_ptr<PoiPolygonAddressScoreExtractor> addressExtractor2(
    new PoiPolygonAddressScoreExtractor());
  //addressExtractor2->setAddressScoreThreshold(0.8);
  //_extractors.push_back(addressExtractor2);

  /*vector<std::string> allExtractorNames = Factory::getInstance().getObjectNamesByBase(
    FeatureExtractor::className());
  vector<std::string> extractorNames;
  for (size_t i = 0; i < allExtractorNames.size(); i++)
  {
    if (allExtractorNames.at(i) != "hoot::OverlapExtractor" &&
        allExtractorNames.at(i) != "hoot::SmallerOverlapExtractor" &&
        allExtractorNames.at(i) != "hoot::SampledAngleHistogramExtractor")
    {
      extractorNames.push_back(allExtractorNames.at(i));
    }
  }
  for (size_t i = 0; i < extractorNames.size(); i++)
  {
    _extractors.push_back(
      shared_ptr<FeatureExtractor>(
        Factory::getInstance().constructObject<FeatureExtractor>(extractorNames[i])));
  }
  LOG_VAR(extractorNames);

  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
      new NameExtractor(
        new TranslateStringDistance(
          new ExactStringDistance()))));
  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
      new NameExtractor(
        new TranslateStringDistance(
          new MaxWordSetDistance(
            new ExactStringDistance())))));
  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
      new NameExtractor(
        new TranslateStringDistance(
          new MeanWordSetDistance(
            new ExactStringDistance())))));

  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
      new NameExtractor(
        new TranslateStringDistance(
          new LevenshteinDistance()))));
  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
      new NameExtractor(
        new TranslateStringDistance(
          new MaxWordSetDistance(
            new LevenshteinDistance())))));
  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
      new NameExtractor(
        new TranslateStringDistance(
          new MeanWordSetDistance(
            new LevenshteinDistance())))));
  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
      new NameExtractor(
        new TranslateStringDistance(
          new MinSumWordSetDistance(
            new LevenshteinDistance())))));

  for (double a = 1.0; a < 1.8; a += 0.05)
  {
    _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
      new MeanWordSetDistance(new LevenshteinDistance(a)))));
    _extractors.push_back(shared_ptr<FeatureExtractor>(new NameExtractor(
      new TranslateStringDistance(new MeanWordSetDistance(new LevenshteinDistance(a))))));
  }

  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
      new NameExtractor(
        new TranslateStringDistance(
          new Soundex()))));
  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
      new NameExtractor(
        new TranslateStringDistance(
          new MaxWordSetDistance(
            new Soundex())))));
  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
      new NameExtractor(
        new TranslateStringDistance(
          new MeanWordSetDistance(
            new Soundex())))));

  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
      new PoiPolygonAddressExtractor(
        new TranslateStringDistance(
          new MeanWordSetDistance(
            new LevenshteinDistance())))));

   SqliteWordWeightDictionary* dict =
      new SqliteWordWeightDictionary(
        ConfPath::search(ConfigOptions().getWeightedWordDistanceDictionary()));
  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
      new NameExtractor(
        new TranslateStringDistance(
          new WeightedWordDistance(
            new LevenshteinDistance(ConfigOptions().getLevenshteinDistanceAlpha()), dict)))));

  _extractors.push_back(shared_ptr<FeatureExtractor>(new EdgeDistanceExtractor()));
  for (double q = 0; q < 1.0; q += 0.05)
  {
    _extractors.push_back(shared_ptr<FeatureExtractor>(
      new EdgeDistanceExtractor(new QuantileAggregator(q))));
  }
  _extractors.push_back(shared_ptr<FeatureExtractor>(
    new EdgeDistanceExtractor(new RmseAggregator())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(
    new EdgeDistanceExtractor(new SigmaAggregator())));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new AngleHistogramExtractor()));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new AttributeScoreExtractor(NULL, false)));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new AttributeScoreExtractor(NULL, true)));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new WeightedShapeDistanceExtractor()));

  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
    new WeightedMetricDistanceExtractor(
      new MeanAggregator(), new SigmaAggregator())));
  _extractors.push_back(
    shared_ptr<FeatureExtractor>(
      new WeightedMetricDistanceExtractor(
        new MeanAggregator(), new RmseAggregator())));

  for (double b = -0.3; b < 0.0; b += 0.1)
  {
    _extractors.push_back(shared_ptr<FeatureExtractor>(new BufferedOverlapExtractor(b)));
  }

  for (double b = 0.1; b < 0.5; b += 0.1)
  {
    _extractors.push_back(shared_ptr<FeatureExtractor>(new BufferedOverlapExtractor(b)));
  }
  _extractors.push_back(shared_ptr<FeatureExtractor>(new BufferedOverlapExtractor()));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new BufferedOverlapExtractor(0.3)));
  _extractors.push_back(shared_ptr<FeatureExtractor>(new BufferedOverlapExtractor(0.1)));

  for (double r = 0.0; r <= 1.0; r += 0.05)
  {
    _extractors.push_back(shared_ptr<FeatureExtractor>(new AngleHistogramExtractor(r)));
  }

   _extractors.push_back(shared_ptr<FeatureExtractor>(new AttributeDistanceExtractor()));
   _extractors.push_back(shared_ptr<FeatureExtractor>(new CentroidDistanceExtractor()));
   _extractors.push_back(shared_ptr<FeatureExtractor>(new CompactnessExtractor()));
   _extractors.push_back(shared_ptr<FeatureExtractor>(new DistanceScoreExtractor()));
   _extractors.push_back(shared_ptr<FeatureExtractor>(new EuclideanDistanceExtractor()));
   _extractors.push_back(shared_ptr<FeatureExtractor>(new HausdorffDistanceExtractor()));
   _extractors.push_back(shared_ptr<FeatureExtractor>(new LengthScoreExtractor()));
   _extractors.push_back(shared_ptr<FeatureExtractor>(new SampledAngleHistogramExtractor()));*/
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
      QString factorName =
        QString::fromStdString(_extractors[i]->getName()).replace(QRegExp("[^\\w]"), "_");
      result[factorName] = v;
    }
  }

  return result;
}

}
