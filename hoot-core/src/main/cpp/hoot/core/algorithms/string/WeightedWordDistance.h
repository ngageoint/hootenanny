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
#ifndef WEIGHTEDWORDDISTANCE_H
#define WEIGHTEDWORDDISTANCE_H

// hoot
#include <hoot/core/algorithms/StringDistance.h>
#include <hoot/core/algorithms/StringDistanceConsumer.h>
#include <hoot/core/algorithms/string/StringTokenizer.h>
#include <hoot/core/algorithms/string/WordWeightDictionary.h>

// Standart
#include <vector>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{
using namespace std;

/**
 * Returns the max (best) pairwise word comparison within two sets of words.
 */
class WeightedWordDistance : public StringDistance, public StringDistanceConsumer,
  public Configurable
{
public:
  static std::string className() { return "hoot::WeightedWordDistance"; }

  WeightedWordDistance(StringDistance* d, WordWeightDictionary* dictionary);
  WeightedWordDistance();

  virtual double compare(const QString& s1, const QString& s2) const;

  virtual void setConfiguration(const Settings& conf);

  virtual void setStringDistance(const StringDistancePtr& sd) { _d = sd; }

  virtual QString toString() const { return "WeightedWordDistance " + _d->toString(); }

private:
  StringDistancePtr _d;
  WordWeightDictionaryPtr _dictionary;
  StringTokenizer _tokenizer;
  double _p;

  vector<double> _calculateWeights(QStringList l) const;
};

}

#endif // WEIGHTEDWORDDISTANCE_H
