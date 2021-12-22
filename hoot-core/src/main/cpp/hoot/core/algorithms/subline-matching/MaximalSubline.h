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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MAXIMALSUBLINE_H
#define MAXIMALSUBLINE_H

// geos
#include <geos/geom/LineSegment.h>

// hoot
#include <hoot/core/algorithms/Sparse2dMatrix.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/OpenCv.h>
#include <hoot/core/util/Units.h>

namespace hoot
{
class WayLocation;
class WaySublineMatch;

using PointPairVector = std::vector<std::pair<WayLocation, WayLocation>>;

/**
 * @brief The MaximalSubline class given a set of limitations on defining whether or not two points
 * "match", calculates an approximation of the maximal line within those constraints.
 *
 * @todo It may be worth investaging a probabilistic approach to this problem. Although we'll either
 * need a clever way to do it unsupervised, or some point matching training data to establish
 * distributions. It'll also likely increase the computational complexity.
 */
class MaximalSubline
{
public:

  static QString className() { return "MaximalSubline"; }

  // hard stop at this value; This was determined empirically based on real world data...may still
  // need some tweaking. Obviously, if it gets too high conflate jobs may get really slow.
  static const int MAX_RECURSIONS_UPPER_LIMIT = 1e7;

  class MatchCriteria
  {
  public:

    MatchCriteria() = default;
    virtual ~MatchCriteria() = default;

    /**
     * @brief match returns a match score for @a index1 way segment matches @a index2.
     *
     * The indexes refer to a line segment within the way. An index refers to the line segment made
     * up by node[index] and node[index + 1].
     *
     * Generally the match is some multiple of the length of the ways that matches in meters.
     * However, the class defines what a match means.
     * @param index1 The line segment in w1.
     * @param index2 The line segment in w2.
     * @returns A value from 0 to 1 defining the match where 0 is no match and 1 is an exact match.
     */
    virtual double match(int index1, int index2) const = 0;

    /**
     * @brief maximalNearestSubline crops the line segments a and b down to their maximal nearest
     * sublines relative to each other.
     */
    virtual void maximalNearestSubline(geos::geom::LineSegment& a, geos::geom::LineSegment& b) const;

    virtual void setWays(const ConstOsmMapPtr& map, const ConstWayPtr& w1, const ConstWayPtr& w2)
    { _map = map; _w1 = w1; _w2 = w2; }

  protected:

    ConstWayPtr _w1, _w2;
    ConstOsmMapPtr _map;
  };

  /**
   * @brief The ThresholdMatchCriteria class uses the maxDistance and maxAngleDiff values to
   * determine if the two way locations are a match or not.
   *
   * 1 is returned on match and 0 is returned on mismatch.
   *
   * @note If any point on the maximal nearest subline is greater than maxDistance then the whole
   * subline is discarded. With a little fancy linear algebra we should be able to clip at the max
   * distance rather than discarding the whole segment.
   */
  class ThresholdMatchCriteria : public MatchCriteria
  {
  public:

    ThresholdMatchCriteria(Meters maxDistance, Radians maxAngleDiff);
    virtual ~ThresholdMatchCriteria() = default;

    double match(int index1, int index2) const override;

    void matchingSubline(geos::geom::LineSegment& a, geos::geom::LineSegment& b) const;

  private:

    Meters _maxDistance;
    Radians _maxAngleDiff;
  };

  /**
   * @brief MaximalSubline - Constructor that uses the specified match criteria to determine match.
   *
   * This class takes ownership of the criteria object.
   * @param criteria The matching criteria for matching these lines. This class takes ownership
   * of @a criteria.
   */
  MaximalSubline(std::shared_ptr<MatchCriteria> criteria, Meters minSplitSize);
  ~MaximalSubline() = default;

  /**
   * @brief findAllMatches given two ways, find all the matching and non-conflicting sublines
   * between w1 and w2. This method can give very different results from "findMaximalSubline". It
   * will search for the optimal set of matches to obtain the highest score within a line. For
   * simple cases this won't make any difference, but complex cases may give very different results.
   *
   * This method also searches for gentle join scenarios using the ExpectationIntersection code.
   * This is computationally complex, but for simple scenarios it should run quickly. When it is
   * done it should create "natural looking" intersection points by maximizing the similarity of
   * the match region and maximizing the similarity of the miss regions. See #2635 for a more
   * thorough description.
   *
   * This code is still experimental at best.
   * @param bestScore The score for the set of matches returned. Higher is better. The score for
   * a line is dependant on the MatchCriteria and the only thing that can safely be said about
   * it is that higher is better, but you cannot directly compare scores for different sets of
   * ways. (it is likely length dependent).
   */
  std::vector<WaySublineMatch> findAllMatches(const ConstOsmMapPtr &map, const ConstWayPtr& w1,
                                              const ConstWayPtr &w2, double &bestScore,
                                              bool snapIntersections = true);

  /**
   * @brief findMaximalSubline given two ways, finds the highest scoring subline that exists in both
   * w1 and w2.
   * @param w1 The first way
   * @param w2 The second way
   * @param wl1 The way locations that match in w1. The start is index 0 and the end is 1. The
   *  vector will be resized as needed.
   * @param wl2 The way locations that match in w1. The start is index 0 and the end is 1. The
   *  vector will be resized as needed.
   * @return 0.0 if there are no common sublines, otherwise the score of the best subline.
   */
  double findMaximalSubline(const ConstOsmMapPtr &map, const ConstWayPtr& w1, const ConstWayPtr& w2,
                            std::vector<WayLocation>& wl1, std::vector<WayLocation>& wl2) const;

  int getBestMatchesRecursionCount() const { return _findBestMatchesRecursionCount; }

  void setMaxRecursions(int maxRecursions) { _maxRecursions = maxRecursions; }

private:

  static int logWarnCount;

  std::shared_ptr<MatchCriteria> _criteria;
  Meters _spacing;
  Meters _minSplitSize;

  // places a limit on the number of recursive matching calls that can be made and throws an
  // exception once that limit has been reached; only used in select places in the code
  int _maxRecursions;
  int _findBestMatchesRecursionCount;

  struct SublineScore
  {
    int start1, stop1;
    int start2, stop2;
    double score;
  };

  /**
   * Determines the end of the way location in way a. The way location will start in the line
   * segment specified by index.
   * @param index The index of the first node in the line segment to look at.
   */
  WayLocation _calculateEndWayLocation(const ConstOsmMapPtr &map, const ConstWayPtr& a,
                                       const ConstWayPtr& b, int indexA, int indexB) const;
  /**
   * Determines the start of the way location in way a. The way location will start in the line
   * segment specified by index.
   * @param index The index of the first node in the line segment to look at.
   */
  WayLocation _calculateStartWayLocation(const ConstOsmMapPtr& map, const ConstWayPtr& a,
                                         const ConstWayPtr& b, int indexA, int indexB) const;
  void _calculateSublineScores(const ConstOsmMapPtr& map, const ConstWayPtr& w1,
                               const ConstWayPtr& w2, Sparse2dMatrix &scores) const;

  PointPairVector _discretizePointPairs(const ConstOsmMapPtr &map, const ConstWayPtr& w1, const ConstWayPtr& w2,
                                        std::vector<WaySublineMatch> &rawSublineMatches) const;

  std::vector<WaySublineMatch> _extractAllMatches(const ConstOsmMapPtr& map, const ConstWayPtr& w1,
                                                  const ConstWayPtr& w2, const Sparse2dMatrix& sublineMatrix) const;
  std::vector<WaySublineMatch> _findBestMatches(const ConstOsmMapPtr& map, const ConstWayPtr& w1,
                                                const ConstWayPtr& w2, const Sparse2dMatrix& sublineMatrix, double& bestScore);
  double _findBestMatchesRecursive(std::vector<WaySublineMatch>& candidates,
                                   std::vector<bool>& keepers, size_t offset);
  /**
   * Find the ends of all the subline matches.
   */
  std::vector<Sparse2dCellId> _findEndMatches(const Sparse2dMatrix& sublines) const;
  /**
   * Finds the starting location for the given end match location.
   */
  Sparse2dMatrix::CellId _findStartMatch(
    const Sparse2dMatrix &sublines, Sparse2dMatrix::CellId end) const;

  void _populateTotalScores(const Sparse2dMatrix& scores, Sparse2dMatrix& sublines,
    Sparse2dMatrix::CellId& bestCid, double& bestScore) const;

  std::vector<WaySublineMatch> _snapIntersections(
    const ConstOsmMapPtr &map, const ConstWayPtr& w1, const ConstWayPtr& w2,
    std::vector<WaySublineMatch> &rawSublineMatches) const;

  /**
   * If the Match Criteria considers these close enough to the ends of the line then they'll be
   * snapped to the end.
   */
  void _snapToEnd(WayLocation& wl, double thresh = -1) const { _snapToTerminal(wl, false, thresh); }
  void _snapToStart(WayLocation& wl, double thresh = -1) const { _snapToTerminal(wl, true, thresh); }
  void _snapToTerminal(WayLocation& wl, bool startOfLines = true, double threshold = -1) const;

  bool _checkForSortedSecondSubline(const std::vector<WaySublineMatch>& rawSublineMatches) const;
  bool _rawSublinesTooSmall(const std::vector<WaySublineMatch>& rawSublineMatches) const;
  cv::Mat _createConstraintMatrix(const std::vector<int>& starts, const std::vector<int>& ends,
                                  const PointPairVector& pairs, std::vector<int>& matchIndexes) const;
  void _calculateSnapStarts(const WaySublineMatch& rawSublineMatch, const int matchIndex,
                            const std::vector<double>& splits, const PointPairVector& pairs,
                            const ConstOsmMapPtr& map, const ConstWayPtr& w1, const ConstWayPtr& w2,
                            WayLocation& w1Start, WayLocation& w2Start) const;
  void _calculateSnapEnds(const int matchIndex, const std::vector<double>& splits, const PointPairVector& pairs,
                          const ConstOsmMapPtr& map, const ConstWayPtr& w1, const ConstWayPtr& w2,
                          WayLocation& w1End, WayLocation& w2End) const;
  void _calculatePointPairMatches(const double way1CircularError, const double way2CircularError,
                                  const std::vector<WaySublineMatch>& rawSublineMatches,
                                  const PointPairVector& pairs, cv::Mat& m, std::vector<int>& starts,
                                  std::vector<int>& ends) const;
};

}

#endif // MAXIMALSUBLINE_H
