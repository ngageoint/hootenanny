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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RiverMaximalSublineSettingOptimizer.h"

// Hoot
#include <hoot/core/criterion/LinearWaterwayCriterion.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/util/ConfigUtils.h>

namespace hoot
{

RiverMaximalSublineSettingOptimizer::RiverMaximalSublineSettingOptimizer() :
// These values were determined empirically against a relatively small number of datasets and may
// require additional tweaking.
_minRiverLengthScalingValue(150000.0),
_maxRiverLengthScalingValue(50000000.0),
_minIterationsScalingValue(50),
_maxIterationsScalingValue(4500)
{
}

int RiverMaximalSublineSettingOptimizer::getFindBestMatchesMaxRecursions(
  const ConstOsmMapPtr& map) const
{
  LOG_STATUS("Determining optimal maximal subline configuration for river conflation...");

  int numProcessed = 0;
  int riverCount = 0;
  double totalRiverLength = 0.0;
  const int statusUpdateInterval = ConfigOptions().getTaskStatusUpdateInterval();
  LOG_VARD(MapProjector::toWkt(map->getProjection()));

  // If a bounds was specified for the conflate job, need to ensure that all the rivers whose length
  // we are summing have some porition of them within the bounds. See related note in the
  // isMatchCandidate method of River.js.
  std::shared_ptr<InBoundsCriterion> boundsCrit = ConfigUtils::getConflateBoundsCrit(map);
  LOG_VARD(boundsCrit.get());

  // Get the total length of all the rivers in the dataset.
  // TODO: add a filter option to LengthOfWaysVisitor and use it here instead of this
  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const WayPtr& way = it->second;
    if (way)
    {
      LOG_VART(LinearWaterwayCriterion().isSatisfied(way));
    }
    if (boundsCrit && way)
    {
      LOG_VART(boundsCrit->isSatisfied(way));
    }
    if (way && LinearWaterwayCriterion().isSatisfied(way) &&
        (!boundsCrit || boundsCrit->isSatisfied(way)))
    { 
      totalRiverLength += ElementGeometryUtils::calculateLength(way, map);
      riverCount++;
    }

    numProcessed++;
    if (numProcessed % statusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Processed " << StringUtils::formatLargeNumber(numProcessed) << " / " <<
        StringUtils::formatLargeNumber(ways.size()) << " ways.");
    }
  }
  boundsCrit.reset();
  LOG_VARD(riverCount);
  LOG_VARD(totalRiverLength);

  if (riverCount == 0 || totalRiverLength == 0.0)
  {
    return -1;
  }
  LOG_VARD(totalRiverLength / (double)riverCount);

  // Determine the maximum maximal subline recursions allowed when finding best matches. This is
  // scaled linearly between two river length and two recursion count values (we may end up needing
  // a non-linear function instead). The default values were determined empirically and may need
  // tweaking.

  int maxRecursions = -1;
  if (totalRiverLength < _minRiverLengthScalingValue)
  {
    // For smaller amounts of rivers, let MaximalSubline determine the limit.
    LOG_DEBUG(
      "Total river length: " << totalRiverLength << " less than minimum scaling value: " <<
      _minRiverLengthScalingValue << ". Allowing unlimited recursions.");
    maxRecursions = -1;
  }
  else if (totalRiverLength >= _maxRiverLengthScalingValue)
  {
    // Cap the reduction of recursions for large amounts of rivers.
    maxRecursions = _minIterationsScalingValue;
    LOG_DEBUG(
      "Total river length: " << totalRiverLength << " greater than or equal to maximum scaling " <<
      "value of: " << _maxRiverLengthScalingValue << ". " << "Using maximum recursion value: " <<
      maxRecursions);
  }
  else
  {
    LOG_VARD(_minRiverLengthScalingValue);
    LOG_VARD(_maxRiverLengthScalingValue);
    LOG_VARD(_minIterationsScalingValue);
    LOG_VARD(_maxIterationsScalingValue);

    // In between, we return a value that for river length is linearly scaled from min up to max,
    // and for recursions, scaled from max down to min (technically, recursions are scaled down to
    // zero, but the min case is handled separately, so doesn't matter). We may end up needing a
    // non-linear function here instead that reduces the recursions allowed more quickly as the
    // total length increases in order to optimize the performance for river length counts in the
    // middle of the scale.
    maxRecursions =
      std::round(
        ((double)_maxIterationsScalingValue / _maxRiverLengthScalingValue) *
        (_maxRiverLengthScalingValue - totalRiverLength));
    LOG_DEBUG(
      "Using maximum recursion value of: " << maxRecursions << " for total river length: " <<
      totalRiverLength);
  }
  return maxRecursions;
}

}
