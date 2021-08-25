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
#ifndef MAXWORDSETDISTANCE_H
#define MAXWORDSETDISTANCE_H

// hoot
#include <hoot/core/algorithms/string/StringDistanceConsumer.h>
#include <hoot/core/algorithms/string/StringTokenizer.h>

namespace hoot
{

/**
 * @brief The MaxWordSetDistance class returns the max (best) pairwise word comparison within two
 * sets of words.
 */
class MaxWordSetDistance : public StringDistance, public StringDistanceConsumer, public Configurable
{
public:

  static QString className() { return "MaxWordSetDistance"; }

  MaxWordSetDistance(StringDistancePtr d);
  MaxWordSetDistance() = default;
  ~MaxWordSetDistance() = default;

  double compare(const QString& s1, const QString& s2) const override;

  void setConfiguration(const Settings& conf) override;

  void setStringDistance(const StringDistancePtr& sd) override { _d = sd; }

  QString toString() const override { return "MaxWordSet " + _d->toString(); }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Returns a score based on the max (best) pairwise word comparison within two sets of words"; }

private:

  StringDistancePtr _d;
  StringTokenizer _tokenizer;
};

}

#endif // WORDSETDISTANCE_H
