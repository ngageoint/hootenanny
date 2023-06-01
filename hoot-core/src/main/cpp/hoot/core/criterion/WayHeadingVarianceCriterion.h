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
 * @copyright Copyright (C) 2021-2023 Maxar (http://www.maxar.com/)
 */
#ifndef WAY_HEADING_VARIANCE_CRITERION_H
#define WAY_HEADING_VARIANCE_CRITERION_H

// hoot
#include <hoot/core/algorithms/extractors/SampledAngleHistogramExtractor.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/NumericComparisonType.h>
#include <hoot/core/util/Units.h>

namespace hoot
{

class WayHeadingVarianceCriterion : public ElementCriterion, public ConstOsmMapConsumerBase
{
public:

  static QString className() { return "WayHeadingVarianceCriterion"; }

  WayHeadingVarianceCriterion();
  WayHeadingVarianceCriterion(const Degrees comparisonVariance, const NumericComparisonType& numericComparisonType,
                              ConstOsmMapPtr map);
  ~WayHeadingVarianceCriterion() override = default;

  /**
   * @see ElementCriterion
   */
  bool isSatisfied(const ConstElementPtr& e) const override;

  ElementCriterionPtr clone() override
  {
    return std::make_shared<WayHeadingVarianceCriterion>(_comparisonVariance, _numericComparisonType, _map);
  }

  /**
   * Determines the largest single heading variance for a way
   *
   * @param way the way to examine
   * @return a variance in degrees
   */
  Degrees getLargestHeadingVariance(const ConstWayPtr& way) const;

  QString getDescription() const override
  { return "Identifies ways that meet a heading variance threshold"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }

  void setNumHistogramBins(const int numBins);
  void setSampleDistance(const Meters distance);
  void setHeadingDelta(const Degrees delta);

private:

  SampledAngleHistogramExtractor _sampledAngleHist;

  Degrees _comparisonVariance;
  NumericComparisonType _numericComparisonType;

};

}

#endif // WAY_HEADING_VARIANCE_CRITERION_H
