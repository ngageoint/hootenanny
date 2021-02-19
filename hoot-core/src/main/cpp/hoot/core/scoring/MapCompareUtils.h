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

class MapCompareUtils
{
public:

  /**
   * TODO
   *
   * @param map1
   * @param map2
   * @param mean
   * @param confidence
   * @param numIterations
   */
  static void getAttributeComparisonFinalScores(
    const OsmMapPtr& map1, const OsmMapPtr& map2, int& mean, int& confidence,
    const int numIterations);

  /**
   * TODO
   *
   * @param map1
   * @param map2
   * @param outMap
   * @param numIterations
   * @return
   */
  static int getAttributeComparisonFinalScore(
    const OsmMapPtr& map1, const OsmMapPtr& map2, const int numIterations);

  /**
   * TODO
   *
   * @param map1
   * @param map2
   * @param mean
   * @param confidence
   */
  static void getGraphComparisonRawScores(
    const OsmMapPtr& map1, const OsmMapPtr& map2, double& mean, double& confidence);

  /**
   * TODO
   *
   * @param map1
   * @param map2
   * @return
   */
  static int getGraphComparisonFinalScore(const OsmMapPtr& map1, const OsmMapPtr& map2);

  /**
   * TODO
   *
   * @param map1
   * @param map2
   * @param mean
   */
  static void getRasterComparisonRawScores(
    const OsmMapPtr& map1, const OsmMapPtr& map2, double& mean);

  /**
   * TODO
   *
   * @param map1
   * @param map2
   * @return
   */
  static int getRasterComparisonFinalScore(const OsmMapPtr& map1, const OsmMapPtr& map2);

  /**
   * TODO
   *
   * @param score
   * @return
   */
  static int convertRawScoreToFinalScore(const double score);
};

}

#endif // MAP_COMPARE_UTILS_H
