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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "WeightedWordDistance.h"

// hoot
#include <hoot/core/algorithms/string/LevenshteinDistance.h>
#include <hoot/core/algorithms/string/SqliteWordWeightDictionary.h>
#include <hoot/core/algorithms/string/TextFileWordWeightDictionary.h>
#include <hoot/core/schema/ScoreMatrix.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

// Standard
#include <iomanip>
#include <math.h>

// Tgs
#include <tbs/stats/SampleStats.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(StringDistance, WeightedWordDistance)

WeightedWordDistance::WeightedWordDistance(std::shared_ptr<StringDistance> d, std::shared_ptr<WordWeightDictionary> dictionary)
  : _d(d),
    _dictionary(dictionary)
{
  setConfiguration(conf());
}

WeightedWordDistance::WeightedWordDistance()
  : _d(std::make_shared<LevenshteinDistance>(1.5))
{
  QString dictPath;
  try
  {
    dictPath = ConfPath::search(ConfigOptions().getWeightedWordDistanceDictionary());
  }
  catch (const FileNotFoundException&)
  {
    LOG_WARN("Unable to locate words.sqlite. This should be downloaded during the make "
      "process. It can be manually downloaded from "
      "https://hoot-support.s3.amazonaws.com/words1.sqlite.bz2 "
      "or similar. You can also override the default name with the " +
      ConfigOptions::getWeightedWordDistanceDictionaryKey() + " config option.");
    dictPath = ConfPath::search(ConfigOptions().getWeightedWordDistanceAbridgedDictionary());
    LOG_WARN("Using abridged dictionary. This may result in reduced conflation accuracy. " + dictPath);
  }
  _dictionary = std::make_shared<SqliteWordWeightDictionary>(dictPath);

  setConfiguration(conf());
}

vector<double> WeightedWordDistance::_calculateWeights(QStringList l) const
{
  vector<double> result;
  result.resize(l.size());

  for (int i = 0; i < l.size(); i++)
  {
    double w = pow(_dictionary->getWeight(l[i]), _p);
    LOG_VART(w);
    // if there is no evidence of the word then just set the value to the heighest weight.
    if (w == 0)
      result[i] = 1.0 / pow(_dictionary->getMinWeight(), _p);
    else
      result[i] = 1.0 / w;
  }

  return result;
}

double WeightedWordDistance::compare(const QString& s1, const QString& s2) const
{
  if (!_d || ! _dictionary)
    throw HootException("You must specify both a distance function and dictionary.");

  LOG_VART(s1);
  LOG_VART(s2);

  QStringList sl1 = _tokenizer.tokenize(s1);
  QStringList sl2 = _tokenizer.tokenize(s2);
  LOG_VART(sl1);
  LOG_VART(sl2);

  LOG_TRACE(std::setprecision(8));
  // calculate the relative weight of each word term.
  vector<double> w1 = _calculateWeights(sl1);
  vector<double> w2 = _calculateWeights(sl2);
  LOG_VART(w1.size());
  LOG_VART(w2.size());

  ScoreMatrix<double> scores(sl1.size() + 1, sl2.size() + 1);
  ScoreMatrix<double> weightedScores(sl1.size() + 1, sl2.size() + 1);

  for (int i = 0; i < sl1.size(); i++)
  {
    scores.set(i + 1, 0, 1);
    weightedScores.set(i + 1, 0, w1[i]);
    for (int j = 0; j < sl2.size(); j++)
    {
      double distance = 1 - _d->compare(sl1[i], sl2[j]);
      LOG_TRACE(sl1[i] << " vs. " << sl2[j] << ": " << distance);
      // if we assume they represent the same word this is the weight of that new combined word.
      double w = w1[i] + w2[j];
      scores.set(i + 1, j + 1, distance);
      weightedScores.set(i + 1, j + 1, distance * w);
    }
  }

  for (int j = 0; j < sl2.size(); j++)
  {
    scores.set(0, j + 1, 1);
    weightedScores.set(0, j + 1, w2[j]);
  }

  LOG_VART(scores.toTableString());
  LOG_VART(weightedScores.toTableString());
  LOG_VART(scores.minSumMatrix().toTableString());
  LOG_VART(scores.minSumMatrix().multiplyCells(weightedScores).toTableString());

  double denominator = tbs::SampleStats(w1).calculateSum() + tbs::SampleStats(w2).calculateSum();
  LOG_VART(denominator);

  double score = scores.minSumMatrix().multiplyCells(weightedScores).sumCells();

  LOG_VART(1 - score / denominator);
  return 1 - score / denominator;
}

void WeightedWordDistance::setConfiguration(const Settings& conf)
{
  _p = ConfigOptions(conf).getWeightedWordDistanceProbability();
  _tokenizer.setConfiguration(conf);
}

}
