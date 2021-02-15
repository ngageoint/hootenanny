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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef RIVER_MAXIMAL_SUBLINE_SETTING_OPTIMIZER_H
#define RIVER_MAXIMAL_SUBLINE_SETTING_OPTIMIZER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Determines settings to optimize river conflation's use of maximal subline matching. The goal here
 * is to prevent runaway conflate runtimes from occurring. We use river length as a measure of data
 * complexity and use that to calculate an upper limit for how many times maximal subline matching
 * can make recursive calls.
 */
class RiverMaximalSublineSettingOptimizer
{
public:

  static QString className() { return "hoot::RiverMaximalSublineSettingOptimizer"; }

  RiverMaximalSublineSettingOptimizer();
  virtual ~RiverMaximalSublineSettingOptimizer() = default;

  /**
   * Determines the runtime optimal maximum number of recursive calls allowed for finding the best
   *  maximal subline match
   *
   * @param map map containing river data
   * @return a max recursion value
   */
  int getFindBestMatchesMaxRecursions(const ConstOsmMapPtr& map) const;

  void setMinRiverLengthScalingValue(Meters val) { _minRiverLengthScalingValue = val; }
  void setMaxRiverLengthScalingValue(Meters val) { _maxRiverLengthScalingValue = val; }
  void setMinIterationsScalingValue(int val) { _minIterationsScalingValue = val; }
  void setMaxIterationsScalingValue(int val) { _maxIterationsScalingValue = val; }

private:

  // river length at which we start caring about performance
  Meters _minRiverLengthScalingValue;
  // river length after which we return a capped recursion value
  Meters _maxRiverLengthScalingValue;
  // minimum recursions we want to have
  int _minIterationsScalingValue;
  // maximimum recursions we want to have
  int _maxIterationsScalingValue;
};

}

#endif // RIVER_MAXIMAL_SUBLINE_SETTING_OPTIMIZER_H
