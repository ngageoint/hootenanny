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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/info/ApiEntityInfo.h>

// Qt
#include <qnumeric.h>

// Standard
#include <string>

// tgs
#include <tgs/RandomForest/DataFrame.h>

namespace hoot
{

class OsmMap;

/**
 * Extracts a single feature (AKA factor) for a given element pair.
 */
class FeatureExtractor : public ApiEntityInfo
{
public:

  FeatureExtractor() = default;
  virtual ~FeatureExtractor() = default;

  static std::string className() { return "hoot::FeatureExtractor"; }

  static double nullValue() { return -999999999; }

  /**
   * Extracts a feature from a given pair of elements. The feature may be something like the
   * distance between colors, the overlap of two polygons, etc.
   */
  virtual double extract(const OsmMap& map, const std::shared_ptr<const Element>& target,
    const std::shared_ptr<const Element>& candidate) const = 0;

  /**
   * Returns the factor type for this feature/factor (Nominal or Numeric).
   */
  virtual Tgs::DataFrame::FactorType getFactorType() const = 0;

  /**
   * Returns the null treatment for this feature/factor (NullAsValue or NullAsMissingValue).
   */
  virtual Tgs::DataFrame::NullTreatment getNullTreatment() const = 0;

  static bool isNull(double v) { return v == nullValue() || ::qIsNaN(v); }

  virtual QString toString() const override { return ""; }
};

typedef std::shared_ptr<FeatureExtractor> FeatureExtractorPtr;

}

#endif // FEATUREEXTRACTOR_H
