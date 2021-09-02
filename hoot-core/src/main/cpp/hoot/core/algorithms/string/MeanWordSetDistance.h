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
#ifndef MEANWORDSETDISTANCE_H
#define MEANWORDSETDISTANCE_H

// hoot
#include <hoot/core/algorithms/string/StringDistanceConsumer.h>
#include <hoot/core/algorithms/string/StringTokenizer.h>

namespace hoot
{

/**
 * @brief The MeanWordSetDistance class calculates the mean distance between two sets of words.
 *
 * The mean distance is calculate by first determining the distance between each pair of words in
 * the two sets, then a heuristic is used to calculate the mean distance between the best pairings.
 * See ScoreMatrix for a more thorough description of mean.
 *
 * This is very similar to Monge-Elkan String Comparison, but makes sure a string is only used once.
 */
class MeanWordSetDistance : public StringDistance, public StringDistanceConsumer,
  public Configurable
{
public:

  /**
   * @param portion The portion parameter passed off to ScoreMatrix.
   */
  MeanWordSetDistance(StringDistancePtr d, double portion = 1.0);
  MeanWordSetDistance();
  ~MeanWordSetDistance() = default;

  static QString className() { return "MeanWordSetDistance"; }

  double compare(const QString& s1, const QString& s2) const override;

  void setConfiguration(const Settings& conf) override { _tokenizer.setConfiguration(conf); }

  void setStringDistance(const StringDistancePtr &sd) override { _d = sd; }

  QString toString() const override
  { return QString("MeanWordSet %1 %2").arg(_p).arg(_d->toString()); }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Returns a score based on the mean distance between two sets of words"; }

private:

  StringDistancePtr _d;
  StringTokenizer _tokenizer;
  double _p;
};

}

#endif // MEANWORDSETDISTANCE_H
