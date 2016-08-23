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
#ifndef NAMEEXTRACTOR_H
#define NAMEEXTRACTOR_H

// hoot
#include <hoot/core/algorithms/StringDistanceConsumer.h>
#include <hoot/core/conflate/extractors/FeatureExtractor.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 *
 */
class NameExtractor : public FeatureExtractor, public StringDistanceConsumer
{
public:
  NameExtractor();

  NameExtractor(StringDistance* d);

  static string className() { return "hoot::NameExtractor"; }

  virtual string getClassName() const { return NameExtractor::className(); }

  virtual string getName() const;

  virtual DataFrame::FactorType getFactorType() const { return DataFrame::Numerical; }

  virtual DataFrame::NullTreatment getNullTreatment() const
  {
    return DataFrame::NullAsMissingValue;
  }

  virtual double extract(const OsmMap& /*map*/, const shared_ptr<const Element>& target,
    const shared_ptr<const Element>& candidate) const { return extract(target, candidate); }

  virtual double extract(const ConstElementPtr& target,
    const ConstElementPtr& candidate) const;

  virtual void setStringDistance(const StringDistancePtr &sd) { _d = sd; }

private:
  StringDistancePtr _d;
};

}

#endif // OVERLAPEXTRACTOR_H
