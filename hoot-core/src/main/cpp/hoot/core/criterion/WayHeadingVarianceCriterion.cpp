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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "WayHeadingVarianceCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/algorithms/extractors/Histogram.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, WayHeadingVarianceCriterion)

WayHeadingVarianceCriterion::WayHeadingVarianceCriterion() :
_comparisonVariance(-1.0),
_numericComparisonType(NumericComparisonType::EqualTo)
{
}

WayHeadingVarianceCriterion::WayHeadingVarianceCriterion(ConstOsmMapPtr map) :
_map(map)
{
}

WayHeadingVarianceCriterion::WayHeadingVarianceCriterion(
  const Degrees comparisonVariance, const NumericComparisonType& numericComparisonType,
  ConstOsmMapPtr map) :
_comparisonVariance(comparisonVariance),
_numericComparisonType(numericComparisonType),
_map(map)
{
}

void WayHeadingVarianceCriterion::setNumHistogramBins(const int numBins)
{
  _sampledAngleHist.setBins(numBins);
}

void WayHeadingVarianceCriterion::setSampleDistance(const Meters distance)
{
  _sampledAngleHist.setSampleDistance(distance);
}
void WayHeadingVarianceCriterion::setHeadingDelta(const Degrees delta)
{
  _sampledAngleHist.setHeadingDelta(delta);
}

Degrees WayHeadingVarianceCriterion::getLargestHeadingVariance(const ConstWayPtr& way) const
{
  std::shared_ptr<Histogram> hist = _sampledAngleHist.getNormalizedHistogram(*_map, way);
  const std::vector<double> bins = hist->getBins();
  Degrees lowestAngle = -1.0;
  Degrees largestDiff = -1.0;
  for (size_t i = 0; i < bins.size(); ++i)
  {
    if (bins[i] > 0.0)
    {
      const Degrees heading = toDegrees(hist->getBinCenter(i));
      if (lowestAngle == -1.0)
      {
        lowestAngle = heading;
      }
      else
      {
        largestDiff = heading - lowestAngle;
      }
    }
  }
  LOG_TRACE("Largest diff for " << way->getElementId() << ": " << largestDiff);
  return largestDiff;
}

bool WayHeadingVarianceCriterion::isSatisfied(const ConstElementPtr& e) const
{
  // A comparison variance of -1 effectively bypasses this crit.
  if (_comparisonVariance == -1.0)
  {
    return true;
  }
  if (!_map)
  {
    throw IllegalArgumentException("WayHeadingVarianceCriterion requires a map.");
  }

  if (e && e->getElementType() == ElementType::Way)
  {
    LOG_VART(e->getElementId());
    return
      _numericComparisonType.satisfiesComparison(
        getLargestHeadingVariance(std::dynamic_pointer_cast<const Way>(e)), _comparisonVariance);
  }
  return false;
}

}
