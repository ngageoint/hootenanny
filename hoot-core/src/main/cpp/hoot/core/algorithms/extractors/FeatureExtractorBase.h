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
#ifndef FEATUREEXTRACTORBASE_H
#define FEATUREEXTRACTORBASE_H

#include <hoot/core/algorithms/extractors/FeatureExtractor.h>
#include <hoot/core/info/ApiEntityInfo.h>

namespace hoot
{

/**
 * Base class for feature extractors, which compare two feature and provide a metric score
 */
class FeatureExtractorBase : public FeatureExtractor, public ApiEntityInfo
{
public:

  static std::string className() { return "hoot::FeatureExtractorBase"; }

  FeatureExtractorBase() {}
  virtual ~FeatureExtractorBase() {}

  static double nullValue() { return -999999999; }

  /**
   * Returns the factor type for this feature/factor (Nominal or Numeric).
   */
  Tgs::DataFrame::FactorType getFactorType() const override
  { return Tgs::DataFrame::Numerical; }

  /**
   * Returns the null treatment for this feature/factor (NullAsValue or NullAsMissingValue).
   */
  Tgs::DataFrame::NullTreatment getNullTreatment() const override
  {
    return Tgs::DataFrame::NullAsMissingValue;
  }
};

}

#endif // FEATUREEXTRACTORBASE_H
