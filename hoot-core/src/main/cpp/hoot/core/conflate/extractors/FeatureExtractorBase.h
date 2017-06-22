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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef FEATUREEXTRACTORBASE_H
#define FEATUREEXTRACTORBASE_H

#include <hoot/core/conflate/extractors/FeatureExtractor.h>

namespace hoot
{

/**
 * Similar to Shape Distance as described in [1].
 * 1. Savary & Zeitouni, 2005
 */
class FeatureExtractorBase : public FeatureExtractor
{
public:
  static std::string className() { return "hoot::FeatureExtractorBase"; }

  FeatureExtractorBase() {}

  static double nullValue() { return -999999999; }

  /**
   * Returns the factor type for this feature/factor (Nominal or Numeric).
   */
  virtual Tgs::DataFrame::FactorType getFactorType() const { return Tgs::DataFrame::Numerical; }

  /**
   * Returns the null treatment for this feature/factor (NullAsValue or NullAsMissingValue).
   */
  virtual Tgs::DataFrame::NullTreatment getNullTreatment() const
  {
    return Tgs::DataFrame::NullAsMissingValue;
  }
};

}

#endif // FEATUREEXTRACTORBASE_H
