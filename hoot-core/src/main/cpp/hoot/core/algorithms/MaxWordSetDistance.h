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
#ifndef MAXWORDSETDISTANCE_H
#define MAXWORDSETDISTANCE_H

#include "StringDistanceConsumer.h"

// hoot
#include <hoot/core/algorithms/string/StringTokenizer.h>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{

/**
 * Returns the max (best) pairwise word comparison within two sets of words.
 */
class MaxWordSetDistance : public StringDistance, public StringDistanceConsumer, public Configurable
{
public:
  static std::string className() { return "hoot::MaxWordSetDistance"; }

  MaxWordSetDistance(StringDistance* d);
  MaxWordSetDistance() {}

  virtual double compare(const QString& s1, const QString& s2) const;

  virtual void setConfiguration(const Settings& conf);

  virtual void setStringDistance(const StringDistancePtr& sd) { _d = sd; }

  virtual QString toString() const { return "MaxWordSet " + _d->toString(); }

private:
  StringDistancePtr _d;
  StringTokenizer _tokenizer;
};

}

#endif // WORDSETDISTANCE_H
