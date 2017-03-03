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
#include "ExpectationIntersection.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

// Tbs
#include <tbs/stats/TDistribution.h>

namespace hoot
{

using namespace tbs;

ExpectationIntersection::ExpectationIntersection()
{
  _maxIterations = 100;
}

double ExpectationIntersection::_findSplit(const TDistribution& t1, const TDistribution& t2,
  const Mat &matches, int start, int stop)
{
  vector<double> p1(matches.rows);
  double sum2 = 0.0;
  for (int i = start; i <= stop; i++)
  {
    double l1 = t1.getLikelihood(matches.row(i));
    double l2 = t2.getLikelihood(matches.row(i));
    p1[i] = l1 / (l1 + l2);
    sum2 += 1.0 - p1[i];
  }

  // maximize the score by maximizing the probabilities associated with both t1 and t2.
  int best = 0;
  double bestScore = 0;
  double cumulative1 = 0.0;
  double cumulative2 = 0.0;
  for (int i = start; i < stop; i++)
  {
    cumulative1 += p1[i];
    cumulative2 += 1 - p1[i];
    double score1 = cumulative1;
    double score2 = sum2 - cumulative2;

    double score = score1 + score2;

    if (score > bestScore)
    {
      bestScore = score;
      best = i;
    }
  }

  double result = best;
  int split = (int)best;

  // figure out what fraction we need to use to determine a point where it is equal probability.
  if (p1[split] > 0.5 && p1[split + 1] < 0.5)
  {
    double m = p1[split + 1] - p1[split];
    double b = p1[split];
    double x = (0.5 - b) / m;
    assert (x >= 0.0 && x <= 1.0);
    result = result + x;
  }

  result = max((double)start, min((double)stop, result));

  return result;
}

vector<double> ExpectationIntersection::snapMatches(const Mat& matches, const Mat& ranges)
{
  _validateRanges(matches.rows, ranges);

  set< vector<int> > visitedSplits;
  vector<int> splitsInt(ranges.rows - 1);
  vector<double> splits(ranges.rows - 1);

  if (splits.size() == 0)
  {
    return splits;
  }

  // initialize the splits to something reasonable.
  for (int i = 0; i < ranges.rows - 1; i++)
  {
    splits[i] = (double)(ranges.at<int>(i, 1) + ranges.at<int>(i + 1, 0)) / 2.0;
  }
  LOG_TRACE(splits);

  int iteration = 0;
  bool done = false;

  while (!done && iteration++ < _maxIterations)
  {
    ////
    // Expectation stage
    ////
    // calculate the pdfs.
    vector<TDistribution> pdfs(ranges.rows);
    int nextStart = 0;
    for (size_t i = 0; i < splits.size(); i++)
    {
      pdfs[i].initialize(matches.rowRange(nextStart, splits[i]));
      nextStart = splits[i];
    }
    pdfs[pdfs.size() - 1].initialize(matches.rowRange(nextStart, matches.rows - 1));

    ////
    // Maximization stage.
    ////

    // Move the split towards the point where the probability of it being in either split is
    // approximately equal.
    for (size_t i = 0; i < splits.size(); i++)
    {
      // the valid range for the split is the upper bound of the lower range and the lower bound
      // of the upper range. E.g. if you have ranges 0-17 and 1-49, then the valid range is 1-17.
      double split = _findSplit(pdfs[i], pdfs[i + 1], matches, ranges.at<int>(i + 1, 0),
        ranges.at<int>(i, 1));
      splits[i] = split;
      splitsInt[i] = (int)split;
    }

    // check to see if this split is a repeat.
    if (visitedSplits.find(splitsInt) != visitedSplits.end())
    {
      done = true;
    }
    visitedSplits.insert(splitsInt);
  }

  // calculate a more exact split point
  //return _calculateMoreExactSplit(t1, t2, matches, split);
  return splits;
}

void ExpectationIntersection::_validateRanges(int rowCount, const Mat& ranges)
{
  assert(ranges.at<int>(0, 0) == 0);
  if (ranges.at<int>(ranges.rows - 1, 1) != rowCount - 1)
  {
    throw InternalErrorException();
  }

  // verify that the ranges at least touch, but do not have a complete overlap.
  for (int i = 1; i < ranges.rows; i++)
  {
    assert(ranges.at<int>(i - 1, 1) >= ranges.at<int>(i, 0));
    assert(ranges.at<int>(i - 1, 1) < ranges.at<int>(i, 1));
  }
}


}
