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
#ifndef MAP_COMPARE_UTILS_H
#define MAP_COMPARE_UTILS_H

// hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Utilities for comparing maps
 */
class MapCompareUtils
{
public:

  /**
   * Gets a scaled attribute similarity comparison score with confidence for two maps
   *
   * @param map1 the first map to compare
   * @param map2 the second map to compare
   * @param mean the attribute comparison score
   * @param confidence the confidence in the attribute comparison score
   * @param numIterations the number of scoring iterations to run
   */
  static void getAttributeComparisonFinalScores(
    const OsmMapPtr& map1, const OsmMapPtr& map2, int& mean, int& confidence,
    const int numIterations);

  /**
   * Gets a scaled attribute similarity comparison score for two maps
   *
   * @param map1 the first map to compare
   * @param map2 the second map to compare
   * @param numIterations the number of scoring iterations to run
   * @return the attribute comparison score (1 to 1000)
   */
  static int getAttributeComparisonFinalScore(
    const OsmMapPtr& map1, const OsmMapPtr& map2, const int numIterations);

  /**
   * Gets an unscaled graph similarity comparison score with confidence for two maps
   *
   * @param map1 the first map to compare
   * @param map2 the second map to compare
   * @param mean the graph comparison score
   * @param confidence the confidence in the graph comparison score
   */
  static void getGraphComparisonRawScores(
    const OsmMapPtr& map1, const OsmMapPtr& map2, double& mean, double& confidence);

  /**
   * Gets a scaled graph similarity comparison score for two maps
   *
   * @param map1 the first map to compare
   * @param map2 the second map to compare
   * @return the graph comparison score (1 to 1000)
   */
  static int getGraphComparisonFinalScore(const OsmMapPtr& map1, const OsmMapPtr& map2);

  /**
   * Gets an unscaled raster similarity comparison score with confidence for two maps
   *
   * @param map1 the first map to compare
   * @param map2 the second map to compare
   * @param mean the raster comparison score
   */
  static void getRasterComparisonRawScores(
    const OsmMapPtr& map1, const OsmMapPtr& map2, double& mean);

  /**
   * Gets a scaled raster similarity comparison score for two maps
   *
   * @param map1 the first map to compare
   * @param map2 the second map to compare
   * @return the raster comparison score (1 to 1000)
   */
  static int getRasterComparisonFinalScore(const OsmMapPtr& map1, const OsmMapPtr& map2);

  /**
   * Converts a raw similarity score to a final scaled score
   *
   * @param score the raw score
   * @return a final scaled score (1 to 1000)
   */
  static int convertRawScoreToFinalScore(const double score);
};

}

#endif // MAP_COMPARE_UTILS_H
