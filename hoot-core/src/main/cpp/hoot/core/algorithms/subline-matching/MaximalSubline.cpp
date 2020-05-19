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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MaximalSubline.h"

// geos
#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>

// hoot
#include <hoot/core/algorithms/BufferedLineSegmentIntersector.h>
#include <hoot/core/algorithms/ExpectationIntersection.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/algorithms/linearreference/LocationOfPoint.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/algorithms/linearreference/WaySublineMatch.h>

using namespace cv;
using namespace geos::geom;
using namespace std;

namespace hoot
{

int MaximalSubline::logWarnCount = 0;

MaximalSubline::ThresholdMatchCriteria::ThresholdMatchCriteria(Meters maxDistance,
  Radians maxAngleDiff)
{
  _maxDistance = maxDistance;
  _maxAngleDiff = maxAngleDiff;
}

double MaximalSubline::ThresholdMatchCriteria::match(int index1, int index2) const
{
  Coordinate c11 = _map->getNode(_w1->getNodeId(index1))->toCoordinate();
  Coordinate c12 = _map->getNode(_w1->getNodeId(index1 + 1))->toCoordinate();
  Coordinate c21 = _map->getNode(_w2->getNodeId(index2))->toCoordinate();
  Coordinate c22 = _map->getNode(_w2->getNodeId(index2 + 1))->toCoordinate();

  LineSegment ls1(c11, c12);
  LineSegment ls2(c21, c22);

  Radians heading1, heading2;

  // if the line segment is zero length
  if (ls1.p0 == ls1.p1)
  {
    // calculate heading based on location in way. See #4765 for the drama associated with this fix.
    // This should find way headings appropriately even if there are one or more duplicate nodes in
    // the way. This is done by calculating the way location before and after this node location.
    // There may be some edge conditions when this will fail, but that seems very unlikely.
    heading1 = WayHeading::calculateHeading(WayLocation(_map, _w1, index1, 0));
  }
  else
  {
    // little faster than above and better defined.
    heading1 = ls1.angle();
  }

  // if the line segment is zero length
  if (ls2.p0 == ls2.p1)
  {
    // calculate heading based on location in way
    heading2 = WayHeading::calculateHeading(WayLocation(_map, _w2, index2, 0));
  }
  else
  {
    // little faster than above and better defined.
    heading2 = ls2.angle();
  }

  Radians angleDiff = WayHeading::deltaMagnitude(heading1, heading2);

  if (angleDiff > _maxAngleDiff)
  {
    return 0.0;
  }

  double maxD;

  // Treat this as a variation of Frechet's Distance. This means that we're looking for the longest
  // subline that is within _maxDistance of the other linestring.

  // find the subline that is within _maxDistance. Ultimately we'll need to integrate a proper
  // implementation of Frechet's distance.
  matchingSubline(ls1, ls2);

  // previously I found the maximalNearest subline, but that causes issues when there is a large
  // offset between the two datasets.
  //maximalNearestSubline(ls1, ls2);

  maxD = max(ls1.p0.distance(ls2.p0), ls1.p1.distance(ls2.p1));
  double mns = min(ls1.getLength(), ls2.getLength());
  const double epsilon = 1e-6;
  if (maxD > _maxDistance + epsilon)
  {
    return 0.0;
  }

  return 1.0 * mns;
}

void MaximalSubline::ThresholdMatchCriteria::matchingSubline(LineSegment &a, LineSegment &b) const
{
  BufferedLineSegmentIntersector bi;

  LineSegment sublineA, sublineB;
  bi.intersect(a, _maxDistance, b, sublineB);
  bi.intersect(b, _maxDistance, a, sublineA);

  a = sublineA;
  b = sublineB;
}

void MaximalSubline::MatchCriteria::maximalNearestSubline(LineSegment& a,
  LineSegment& b) const
{
  Coordinate a1, a2;
  Coordinate b1, b2;

  a.closestPoint(b.p0, a1);
  a.closestPoint(b.p1, a2);

  b.closestPoint(a.p0, b1);
  b.closestPoint(a.p1, b2);

  a.p0 = a1;
  a.p1 = a2;

  b.p0 = b1;
  b.p1 = b2;
}

MaximalSubline::MaximalSubline(MatchCriteria* criteria, Meters minSplitSize) :
_criteria(criteria),
_spacing(ConfigOptions().getMaximalSublineSpacing()),
_minSplitSize(minSplitSize)
{
}

WayLocation MaximalSubline::_calculateEndWayLocation(const ConstOsmMapPtr &map,
  const ConstWayPtr& a, const ConstWayPtr& b, int indexA, int indexB)
{
  Coordinate ca1 = map->getNode(a->getNodeId(indexA))->toCoordinate();
  Coordinate ca2 = map->getNode(a->getNodeId(indexA + 1))->toCoordinate();
  Coordinate cb2 = map->getNode(b->getNodeId(indexB + 1))->toCoordinate();

  LineSegment lsA(ca1, ca2);
  Coordinate start;
  lsA.closestPoint(cb2, start);

  return LocationOfPoint(map, a).locateAfter(start, WayLocation(map, a, indexA, 0));
}

WayLocation MaximalSubline::_calculateStartWayLocation(const ConstOsmMapPtr& map,
  const ConstWayPtr& a, const ConstWayPtr& b, int indexA, int indexB)
{
  Coordinate ca1 = map->getNode(a->getNodeId(indexA))->toCoordinate();
  Coordinate ca2 = map->getNode(a->getNodeId(indexA + 1))->toCoordinate();
  Coordinate cb1 = map->getNode(b->getNodeId(indexB))->toCoordinate();

  LineSegment lsA(ca1, ca2);
  Coordinate start;
  lsA.closestPoint(cb1, start);

  return LocationOfPoint(map, a).locate(start);
}

void MaximalSubline::_calculateSublineScores(const ConstOsmMapPtr& map, const ConstWayPtr& way1,
  const ConstWayPtr& way2, Sparse2dMatrix& scores)
{
  LOG_TRACE("Calculating subline scores...");

  _criteria->setWays(map, way1, way2);

  for (int n1 = 0; n1 < (int)way1->getNodeCount() - 1; n1++)
  {
    for (int n2 = 0; n2 < (int)way2->getNodeCount() - 1; n2++)
    {
      double s = _criteria->match(n1, n2);

      if (s > 0.0)
      {
        scores.set(n1, n2, s);
      }
    }
  }
}

vector<pair<WayLocation, WayLocation>> MaximalSubline::_discretizePointPairs(
  const ConstOsmMapPtr &map, const ConstWayPtr& w1, const ConstWayPtr& w2,
  vector<WaySublineMatch>& rawSublineMatches)
{
  LOG_TRACE("Discretizing point pairs...");

  // TODO: this requires a more robust mechanism for generating point pairs - r2701 (need to port
  // issue to github)

  LOG_TRACE("Calculating offset...");

  // figure out where to start on the lines
  double diff =
    rawSublineMatches[0].getSubline1().getStart().calculateDistanceOnWay() -
      rawSublineMatches[0].getSubline2().getStart().calculateDistanceOnWay();

  LOG_TRACE("Converting to line strings...");

  Meters w1Offset = max(0.0, diff);
  Meters w2Offset = max(0.0, -diff);
  Meters w1Length = ElementConverter(map).convertToLineString(w1)->getLength();
  Meters w2Length = ElementConverter(map).convertToLineString(w2)->getLength();

  const int count = min((w1Length - w1Offset) / _spacing, (w2Length - w2Offset) / _spacing) + 1;
  LOG_VART(count);

  vector<pair<WayLocation, WayLocation>> result(count);

  LOG_TRACE("Calculating results...");

  WayLocation lastMatch(map, w2, 0, 0);
  for (int i = 0; i < count; i++)
  {
    const double loc = (double)i * _spacing;

    WayLocation wl1(map, w1, loc + w1Offset);
    WayLocation wl2(LocationOfPoint(map, w2).locateAfter(wl1.getCoordinate(), lastMatch));
    result[i] = pair<WayLocation, WayLocation>(wl1, wl2);
    lastMatch = wl2;
  }

  return result;
}

vector<WaySublineMatch> MaximalSubline::_extractAllMatches(const ConstOsmMapPtr &map,
  const ConstWayPtr& w1, const ConstWayPtr& w2, Sparse2dMatrix& sublineMatrix)
{
  LOG_TRACE("Extracting all matches...");

  vector<Sparse2dCellId> endMatches = _findEndMatches(sublineMatrix);
  vector<Sparse2dCellId> startMatches(endMatches.size());

  for (size_t i = 0; i < endMatches.size(); i++)
  {
    startMatches[i] = _findStartMatch(sublineMatrix, endMatches[i]);
  }

  vector<WaySublineMatch> result;
  result.reserve(endMatches.size());

  // calculate the way locations for each subline on each way.
  for (size_t i = 0; i < endMatches.size(); i++)
  {
    WayLocation start1 =
      _calculateStartWayLocation(map, w1, w2, startMatches[i].row(), startMatches[i].col());
    WayLocation start2 =
      _calculateStartWayLocation(map, w2, w1, startMatches[i].col(), startMatches[i].row());
    _snapToStart(start1);
    _snapToStart(start2);

    WayLocation end1 =
      _calculateEndWayLocation(map, w1, w2, endMatches[i].row(), endMatches[i].col());
    WayLocation end2 =
      _calculateEndWayLocation(map, w2, w1, endMatches[i].col(), endMatches[i].row());
    _snapToEnd(end1);
    _snapToEnd(end2);

    WaySubline ws1(start1, end1);
    WaySubline ws2(start2, end2);

    if (ws1.isValid() && ws1.isZeroLength() == false &&
        ws2.isValid() && ws2.isZeroLength() == false)
    {
      result.push_back(WaySublineMatch(ws1, ws2));
    }
  }

  return result;
}

vector<WaySublineMatch> MaximalSubline::findAllMatches(const ConstOsmMapPtr &map,
  const ConstWayPtr& w1, const ConstWayPtr& w2, double& bestScore, bool snapIntersections)
{
  bestScore = 0.0;

  // create a sparse matrix of line segment scores
  Sparse2dMatrix scores;
  _calculateSublineScores(map, w1, w2, scores);

  // create an empty sparse matrix of subline scores where the upper left is the start and the lower
  // right is the end score (total) for a subline. See #2550 for a small PPT describing the routine.
  Sparse2dMatrix sublineMatrix;

  double score;

  // incrementally update the subline scores and find the highest subline score, this is our maximal
  // subline.
  Sparse2dMatrix::CellId endMatch;
  _populateTotalScores(scores, sublineMatrix, endMatch, score);
  LOG_VART(scores.size());
  LOG_VART(scores.toString());

  vector<WaySublineMatch> rawSublines;

  // if there were no matches.
  if (endMatch.row() < 0 || endMatch.col() < 0)
  {
    return rawSublines;
  }

  rawSublines = _findBestMatches(map, w1, w2, sublineMatrix, bestScore);

  vector<WaySublineMatch> result;
  // Until the point pair matching is more sophisticated, we can only snap when it is a simple
  // single match.
  if (rawSublines.size() == 1 && snapIntersections)
  {
    LOG_TRACE("Snapping intersections...");
    result = _snapIntersections(map, w1, w2, rawSublines);
  }
  else
  {
    LOG_TRACE("Not snapping intersections.");
    result = rawSublines;
  }

  return result;
}

vector<WaySublineMatch> MaximalSubline::_findBestMatches(const ConstOsmMapPtr &map,
  const ConstWayPtr& w1, const ConstWayPtr& w2, Sparse2dMatrix& sublineMatrix, double& bestScore)
{
  LOG_TRACE("Finding best matches...");

  vector<WaySublineMatch> result;

  vector<WaySublineMatch> sublines = _extractAllMatches(map, w1, w2, sublineMatrix);
  LOG_VART(sublines.size());

  // use a exhaustive approach to find the best sublines. The number of matching sublines is
  // typically quite small.

  vector<bool> keepers(sublines.size(), false);
  bestScore = _findBestMatchesRecursive(sublines, keepers, 0);

  for (size_t i = 0; i < keepers.size(); i++)
  {
    LOG_VART(sublines[i].toString());
    if (keepers[i])
    {
      result.push_back(sublines[i]);
    }
  }

  LOG_TRACE("best sublines count: " << result.size());

  return result;
}

double MaximalSubline::_findBestMatchesRecursive(
  vector<WaySublineMatch>& candidates, vector<bool>& keepers, size_t offset)
{
  if (offset == candidates.size())
  {
    return 0.0;
  }

  bool conflict = false;
  for (size_t i = 0; i < offset; i++)
  {
    if (candidates[offset].overlaps(candidates[i]))
    {
      LOG_TRACE("Found conflict.");
      conflict = true;
    }
  }

  double score1 = 0.0;
  vector<bool> test1;

  if (!conflict)
  {
    // see what happens if we keep this candidate in
    keepers[offset] = true;
    score1 = candidates[offset].getSubline1().getLength();
    score1 += _findBestMatchesRecursive(candidates, keepers, offset + 1);
    test1 = keepers;
  }

  // see what happens if we throw this candidate out
  keepers[offset] = false;
  double score2 = 0.0;
  score2 += _findBestMatchesRecursive(candidates, keepers, offset + 1);

  double result;
  if (score1 > score2)
  {
    keepers = test1;
    result = score1;
  }
  else
  {
    result = score2;
  }

  return result;
}

vector<Sparse2dCellId> MaximalSubline::_findEndMatches(Sparse2dMatrix& sublines)
{
  vector<Sparse2dCellId> result;

  // go through all the scores
  for (Sparse2dMatrix::const_iterator it = sublines.begin(); it != sublines.end(); ++it)
  {
    Sparse2dMatrix::CellId cid = it->first;

    // if this is the end of a subline match.
    if (sublines.get(cid.row(), cid.col() + 1) == 0.0 &&
        sublines.get(cid.row() + 1, cid.col()) == 0.0 &&
        sublines.get(cid.row() + 1, cid.col() + 1) == 0.0)
    {
      result.push_back(cid);
    }
  }

  return result;
}


double MaximalSubline::findMaximalSubline(const ConstOsmMapPtr& map, const ConstWayPtr& w1,
  const ConstWayPtr& w2, vector<WayLocation>& wl1, vector<WayLocation>& wl2)
{
  // create a sparse matrix of line segment scores
  Sparse2dMatrix scores;
  _calculateSublineScores(map, w1, w2, scores);

  // create an empty sparse matrix of subline scores where the upper left is the start and the lower
  // right is the end score (total) for a subline. See #2550 for a small PPT describing the routine.
  Sparse2dMatrix sublines;

  double score;

  // incrementally update the subline scores and find the highest subline score, this is our maximal
  // subline.
  Sparse2dMatrix::CellId endMatch;
  _populateTotalScores(scores, sublines, endMatch, score);

  // if there were no matches.
  if (endMatch.row() < 0 || endMatch.col() < 0)
  {
    return 0.0;
  }

  // traverse the subline matrix backwards to find the starting line segement
  Sparse2dMatrix::CellId startMatch;
  startMatch = _findStartMatch(sublines, endMatch);

  // calculate the way locations for the maximal subline on each way.
  wl1.resize(2);
  wl2.resize(2);
  wl1[0] = _calculateStartWayLocation(map, w1, w2, startMatch.row(), startMatch.col());
  wl2[0] = _calculateStartWayLocation(map, w2, w1, startMatch.col(), startMatch.row());
  wl1[1] = _calculateEndWayLocation(map, w1, w2, endMatch.row(), endMatch.col());
  wl2[1] = _calculateEndWayLocation(map, w2, w1, endMatch.col(), endMatch.row());

  // snap to the terminals if they're just a bit off
  _snapToStart(wl1[0]);
  _snapToStart(wl2[0]);
  _snapToEnd(wl1[1]);
  _snapToEnd(wl2[1]);

  return score;
}

Sparse2dMatrix::CellId MaximalSubline::_findStartMatch(Sparse2dMatrix& sublines,
  Sparse2dMatrix::CellId end)
{
  Sparse2dMatrix::CellId result = end;

  bool done = false;
  while (!done)
  {
    Sparse2dMatrix::CellId best = result;
    double bestScore = 0.0;
    double s;

    s = sublines.get(result.row() - 1, result.col() - 1);
    if (s > bestScore)
    {
      bestScore = s;
      best = Sparse2dMatrix::CellId(result.row() - 1, result.col() - 1);
    }

    s = sublines.get(result.row(), result.col() - 1);
    if (s > bestScore)
    {
      bestScore = s;
      best = Sparse2dMatrix::CellId(result.row(), result.col() - 1);
    }

    s = sublines.get(result.row() - 1, result.col());
    if (s > bestScore)
    {
      bestScore = s;
      best = Sparse2dMatrix::CellId(result.row() - 1, result.col());
    }

    if (best == result)
    {
      done = true;
    }
    else
    {
      result = best;
    }
  }

  return result;
}

void MaximalSubline::_populateTotalScores(const Sparse2dMatrix& scores, Sparse2dMatrix& sublines,
  Sparse2dMatrix::CellId& bestCid, double& bestScore)
{
  LOG_TRACE("Populating total scores...");

  bool change;
  double highestScore = 0.0;
  bestCid.row() = -1;
  bestCid.col() = -1;

  // todo this could be made more efficient by using Dijkstras or similar
  do
  {
    change = false;
    // go through all the scores
    for (Sparse2dMatrix::const_iterator it = scores.begin(); it != scores.end(); ++it)
    {
      Sparse2dMatrix::CellId cid = it->first;
      double matchScore = it->second;
      // update the subline cell by determining the best score based on the neighbors
      double highestNeighbor = 0.0;
      highestNeighbor = max(highestNeighbor, sublines.get(cid.row(), cid.col() - 1));
      highestNeighbor = max(highestNeighbor, sublines.get(cid.row() - 1, cid.col() - 1));
      highestNeighbor = max(highestNeighbor, sublines.get(cid.row() - 1, cid.col()));

      double currentSum = sublines.get(cid.row(), cid.col());

      double newSum = highestNeighbor + matchScore;
      if (newSum > currentSum)
      {
        sublines.set(cid.row(), cid.col(), newSum);
        change = true;

        if (newSum > highestScore)
        {
          highestScore = newSum;
          bestCid = cid;
        }
      }
    }
  } while (change);

  bestScore = highestScore;
}

bool lessThan(const WaySublineMatch& ws1, const WaySublineMatch& ws2)
{
  return ws1.getSubline1().getStart() < ws2.getSubline1().getStart();
}

bool MaximalSubline::_checkForSortedSecondSubline(const vector<WaySublineMatch>& rawSublineMatches) const
{
  for (size_t i = 2; i < rawSublineMatches.size(); i++)
  {
    if (rawSublineMatches[i].getSubline2().getStart() >
        rawSublineMatches[i - 1].getSubline2().getStart())
    {
      return false;
    }
  }
  return true;
}

bool MaximalSubline::_rawSublinesTooSmall(const vector<WaySublineMatch>& rawSublineMatches) const
{
  for (size_t i = 0; i < rawSublineMatches.size(); i++)
  {

    if (rawSublineMatches[i].getSubline1().getLength() < _spacing * 2 ||
        rawSublineMatches[i].getSubline2().getLength() < _spacing * 2)
    {
      return true;
    }
  }
  return false;
}

cv::Mat MaximalSubline::_createConstraintMatrix(const vector<int>& starts, const vector<int>& ends,
                                                const vector<pair<WayLocation, WayLocation>>& pairs,
                                                vector<int>& matchIndexes)
{
  LOG_TRACE("Creating constraint matrix...");

  // create the matrix of constraints.
  vector<int> finalStarts;
  vector<int> finalEnds;

  if (starts[0] != 0)
  {
    finalStarts.push_back(0);
    finalEnds.push_back(starts[0] + (ends[0] - starts[0]) / 3);
  }

  for (size_t i = 0; i < starts.size(); i++)
  {
    if (starts[i] == numeric_limits<int>::max())
    {
      // Due to poor subline pair matching we cannot properly snap these intersections. Warn the
      // user and move on. It is likely they aren't a good match anyways.
      throw HootException("A solid set of point pair matches could not be found.");
    }
    matchIndexes[i] = finalStarts.size();
    finalStarts.push_back(starts[i]);
    finalEnds.push_back(ends[i]);

    if (i != starts.size() - 1)
    {
      finalStarts.push_back(ends[i] - (ends[i] - starts[i]) / 3);
      finalEnds.push_back(starts[i + 1] + (ends[i + 1] - starts[i + 1]) / 3);
    }
  }

  int last = ends.size() - 1;
  if ((size_t)ends[last] != pairs.size() - 1)
  {
    finalStarts.push_back(ends[last] - (ends[last] - starts[last]) / 3);
    finalEnds.push_back(pairs.size() - 1);
  }

  LOG_TRACE("finalStarts: " << finalStarts);
  LOG_TRACE("finalEnds: " << finalEnds);

  Mat ranges(finalStarts.size(), 2, CV_32S);
  for (size_t i = 0; i < finalStarts.size(); i++)
  {
    ranges.at<int>(i, 0) = finalStarts[i];
    ranges.at<int>(i, 1) = finalEnds[i];
  }

  return ranges;
}

void MaximalSubline::_calculateSnapStarts(const WaySublineMatch& rawSublineMatch,
                                          const int matchIndex, const vector<double>& splits,
                                          const vector<pair<WayLocation, WayLocation>>& pairs,
                                          const ConstOsmMapPtr& map, const ConstWayPtr& w1,
                                          const ConstWayPtr& w2, WayLocation& w1Start,
                                          WayLocation& w2Start)
{
  // if this is the first subline, then it starts at the beginning of the subline.
  if (matchIndex == 0)
  {
    w1Start = WayLocation(rawSublineMatch.getSubline1().getStart());
    w2Start = WayLocation(rawSublineMatch.getSubline2().getStart());
  }
  else
  {
    int wi = (int)splits[matchIndex - 1];
    LOG_TRACE("start split: " << wi);
    double r = splits[matchIndex - 1] - wi;
    Meters offset1 =
      pairs[wi].first.calculateDistanceOnWay() * r +
      pairs[wi + 1].first.calculateDistanceOnWay() * (1 - r);
    Meters offset2 =
      pairs[wi].second.calculateDistanceOnWay() * r +
      pairs[wi + 1].second.calculateDistanceOnWay() * (1 - r);

    LOG_TRACE("offset1: " << offset1 << " r: " << r);
    LOG_TRACE("offset2: " << offset2 << " r: " << r);
    w1Start = WayLocation(map, w1, offset1);
    w2Start = WayLocation(map, w2, offset2);
  }

  LOG_VART(w1Start);
  LOG_VART(w2Start);

  // if we are passed the point where we have a node pairing, then snap it back to a legit pair.
  if (w1Start < pairs.front().first.move(-_spacing))
  {
    w1Start = pairs.front().first;
  }
  if (w2Start < pairs.front().second.move(-_spacing))
  {
    w2Start = pairs.front().second;
  }

  LOG_VART(w1Start);
  LOG_VART(w2Start);
}

void MaximalSubline::_calculateSnapEnds(const int matchIndex, const vector<double>& splits,
                                        const vector<pair<WayLocation, WayLocation>>& pairs,
                                        const ConstOsmMapPtr& map, const ConstWayPtr& w1,
                                        const ConstWayPtr& w2, WayLocation& w1End,
                                        WayLocation& w2End)
{
  // convert the end split location into a WayLocation
  if (matchIndex < (int)splits.size())
  {
    int wi = (int)splits[matchIndex];
    double r = splits[matchIndex] - wi;
    Meters offset1 =
      pairs[wi].first.calculateDistanceOnWay() * r +
      pairs[wi + 1].first.calculateDistanceOnWay() * (1 - r);
    Meters offset2 =
      pairs[wi].second.calculateDistanceOnWay() * r +
      pairs[wi + 1].second.calculateDistanceOnWay() * (1 - r);

    w1End = WayLocation(map, w1, offset1);
    w2End = WayLocation(map, w2, offset2);

    LOG_TRACE("offset1: " << offset1 << " r: " << r);
    LOG_TRACE("offset2: " << offset2 << " r: " << r);
  }

  // if we are passed the point where we have a node pairing, then snap it back to a legit pair.
  LOG_VART(w1End);
  LOG_VART(pairs.back().first.move(_spacing));
  if (w1End > pairs.back().first.move(_spacing))
  {
    w1End = pairs.back().first;
  }
  if (w2End > pairs.back().second.move(_spacing))
  {
    w2End = pairs.back().second;
  }
  LOG_TRACE("w1End: " << w1End.toString());
  LOG_TRACE("w2End: " << w2End.toString());
}

void MaximalSubline::_calculatePointPairMatches(const double way1CircularError,
                                                const double way2CircularError,
                                                const vector<WaySublineMatch>& rawSublineMatches,
                                                const vector<pair<WayLocation, WayLocation>>& pairs,
                                                cv::Mat& m, vector<int>& starts, vector<int>& ends)
{
    LOG_TRACE("Calculating point pair matches...");

    // extract features on the point pairs and populate a matrix.
    Meters acc = way1CircularError + way2CircularError;

    size_t currentSubline = 0;

    for (size_t i = 0; i < pairs.size(); i++)
    {
      WayLocation wl1 = pairs[i].first;
      WayLocation wl2 = pairs[i].second;
      // If the rawSublineMatches is smaller than _spacing, then it may not directly overlap with
      // one of the point pairs. To avoid this, we create a subline that surrounds the point pair
      // and will guarantee that each rawSublineMatches will touch at least one point pair.
      WaySubline ws1 = WaySubline(wl1.move(-_spacing / 2.0), wl1.move(_spacing / 2.0));
      WaySubline ws2 = WaySubline(wl2.move(-_spacing / 2.0), wl2.move(_spacing / 2.0));

      if (currentSubline < rawSublineMatches.size())
      {
        // figure out the first and last match for this subline.
        if (rawSublineMatches[currentSubline].getSubline1().touches(ws1) ||
            rawSublineMatches[currentSubline].getSubline2().touches(ws2))
        {
          starts[currentSubline] = min<int>(starts[currentSubline], i);
          ends[currentSubline] = max<int>(ends[currentSubline], i);
        }
        else
        {
          // if this is past the current subline, advance to the right subline.
          while (currentSubline < rawSublineMatches.size() &&
                 rawSublineMatches[currentSubline].getSubline1().getEnd() < ws1.getStart() &&
                 rawSublineMatches[currentSubline].getSubline2().getEnd() < ws2.getStart())
          {
            currentSubline++;
          }
        }
      }

      Meters distance = wl1.getCoordinate().distance(wl2.getCoordinate());
      Radians angle1 = WayHeading::calculateHeading(wl1);
      Radians angle2 = WayHeading::calculateHeading(wl2);
      Radians angleDiff = WayHeading::deltaMagnitude(angle1, angle2);

      m.at<double>(i, 0) = distance / acc;
      m.at<double>(i, 1) = angleDiff;
    }

    LOG_TRACE("starts: " << starts);
    LOG_TRACE("ends: " << ends);
}

vector<WaySublineMatch> MaximalSubline::_snapIntersections(const ConstOsmMapPtr& map,
  const ConstWayPtr& w1, const ConstWayPtr& w2, vector<WaySublineMatch>& rawSublineMatches)
{
  // this only works if the rawSublineMatches are in order. We order by subline1
  sort(rawSublineMatches.begin(), rawSublineMatches.end(), lessThan);

  // make sure that ordering by subline1 results in sorted subline2s. If this isn't the case then
  // there isn't much we can do.
  if (!_checkForSortedSecondSubline(rawSublineMatches))
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Way matches sublines out of order. This is unusual and may give a sub-optimal "
        "result.");
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
    return rawSublineMatches;
  }

  // if any of the raw sublines are crazy small, then don't try to snap the intersections.
  if (_rawSublinesTooSmall(rawSublineMatches))
  {
    return rawSublineMatches;
  }

  ////
  /// calculate a series of point pair matches along the lines.
  ////

  // discretize the first line into a series of points.
  vector<pair<WayLocation, WayLocation>> pairs;
  pairs = _discretizePointPairs(map, w1, w2, rawSublineMatches);
  LOG_VART(pairs.size());
  assert(pairs.size() > 0);
  LOG_VART(pairs);

  vector<int> starts(rawSublineMatches.size(), numeric_limits<int>::max());
  vector<int> ends(rawSublineMatches.size(), 0);
  cv::Mat m(pairs.size(), 2, CV_64F);
  _calculatePointPairMatches(
    w1->getCircularError(), w2->getCircularError(), rawSublineMatches, pairs, m, starts, ends);

  // this maps finalStarts indexes to the rawSublineMatches indexes. E.g.
  // rawSublineMatches[i] maps to finalStarts[matchIndexes[i]]
  vector<int> matchIndexes(starts.size());
  cv::Mat ranges;
  try
  {
    ranges = _createConstraintMatrix(starts, ends, pairs, matchIndexes);
  }
  catch (const HootException& e)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN(e.getWhat());
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
    return rawSublineMatches;
  }

  // run ExpectationIntersection to determine new intersection points.
  ExpectationIntersection ei;
  vector<double> splits = ei.snapMatches(m, ranges);

  LOG_VART(splits);

  vector<WaySublineMatch> result = rawSublineMatches;

  LOG_TRACE("Performing point pair matching...");

  for (size_t i = 0; i < matchIndexes.size(); i++)
  {
    LOG_VART(rawSublineMatches[i]);

    const int mi = matchIndexes[i];

    WayLocation w1Start;
    WayLocation w2Start;
    _calculateSnapStarts(rawSublineMatches[i], mi, splits, pairs, map, w1, w2, w1Start, w2Start);

    WayLocation w1End(rawSublineMatches[i].getSubline1().getEnd());
    WayLocation w2End(rawSublineMatches[i].getSubline2().getEnd());
    _calculateSnapEnds(mi, splits, pairs, map, w1, w2, w1End, w2End);

    WaySubline ws1Expanded = rawSublineMatches[i].getSubline1().expand(
          max(_minSplitSize, _spacing));
    WaySubline ws2Expanded = rawSublineMatches[i].getSubline2().expand(
          max(_minSplitSize, _spacing));
    if (ws1Expanded.contains(w1Start) == false ||
        ws2Expanded.contains(w2Start) == false ||
        ws1Expanded.contains(w1End) == false ||
        ws2Expanded.contains(w2End) == false ||
        w1Start > w1End ||
        w2Start > w2End)
    {
      LOG_TRACE("Point pair matching failed, skipping intersection snapping.");
      return rawSublineMatches;
    }

    // snap to the start if we're within spacing distance.
    _snapToStart(w1Start, max(_minSplitSize, _spacing));
    _snapToStart(w2Start, max(_minSplitSize, _spacing));

    // snap to the end if we're within spacing distance.
    _snapToEnd(w1End, max(_minSplitSize, _spacing));
    _snapToEnd(w2End, max(_minSplitSize, _spacing));

    LOG_VART(w1End);
    LOG_VART(w2End);

    WaySubline ws1(w1Start, w1End);
    WaySubline ws2(w2Start, w2End);
    result[i] = WaySublineMatch(ws1, ws2);
  }

  // Put the updated intersection points into the result.
  return result;
}

void MaximalSubline::_snapToTerminal(WayLocation& wl, bool startOfLines, double thresh)
{
  Meters d1 = wl.calculateDistanceOnWay();

  // if we're not at the start of the line
  if (!startOfLines)
  {
    // calculate the distance from the end to this way location.
    d1 = ElementConverter(wl.getMap()).convertToLineString(wl.getWay())->getLength() - d1;
  }

  if (thresh == -1)
  {
    thresh = _minSplitSize;
  }

  // if we should snap the end points
  if (d1 <= thresh)
  {
    // if we're at the start of the line
    if (startOfLines)
    {
      // snap to the beginning
      wl = WayLocation(wl.getMap(), wl.getWay(), 0, 0.0);
    }
    // if we're at the end of the line
    else
    {
      // snap to the end
      wl = WayLocation(wl.getMap(), wl.getWay(), wl.getWay()->getNodeCount(), 0.0);
    }
  }
}

}



