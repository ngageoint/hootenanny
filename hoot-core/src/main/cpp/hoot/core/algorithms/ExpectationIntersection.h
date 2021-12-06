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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef EXPECTATIONINTERSECTION_H
#define EXPECTATIONINTERSECTION_H

// Tbs
#include <tbs/stats/TDistribution.h>

namespace hoot
{

/**
 * @brief The ExpectationIntersection class provides the ability to segement an ordered set of
 * points with constraints.
 */
class ExpectationIntersection
{
public:

  ExpectationIntersection(const int maxCacheSize = 100000);

  /**
   * @brief snapMatches Given a set of matches along a line, determine a good split point using an
   * EM inspired algorithm. [1]
   * 1. http://en.wikipedia.org/wiki/Expectation%E2%80%93maximization_algorithm
   * @param matches An ordered matrix of the points describing matches. Each row is a point record
   * and the columns are features of that point. Must be of type CV_64F.
   * @param ranges The valid ranges for each of the sublines. The ranges must overlap at least
   * a little. Each row is a range, the columns are the start and stop values. Must be of type
   * CV_32S.
   * @return Returns a vector of the split points. Each split point will be between the overlapping
   * ranges.
   */
  std::vector<double> snapMatches(const cv::Mat& matches, const cv::Mat& ranges) const;

private:

  int _maxIterations;
  int _maxCacheSize;

  void _calculateTDistribution(const cv::Mat &matches, int start, int end,
                               tbs::TDistribution& t);

  double _findSplit(const tbs::TDistribution& t1, const tbs::TDistribution& t2, const cv::Mat& matches,
                    int start, int stop) const;

  /**
   * Verify that the caller provided meaningful ranges. Throws an exception on failure.
   */
  void _validateRanges(int rowCount, const cv::Mat& ranges) const;
};

}

#endif // EXPECTATIONINTERSECTION_H
