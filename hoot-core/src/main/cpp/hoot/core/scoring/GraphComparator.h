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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef GRAPHCOMPARATOR_H
#define GRAPHCOMPARATOR_H

//  Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/scoring/BaseComparator.h>

//  Standard
#include <mutex>
#include <queue>
#include <thread>

//  Tgs
#include <tgs/Statistics/Random.h>

namespace hoot
{

class DirectedGraph;
class ShortestPath;
class Way;

class GraphComparator : public BaseComparator
{
public:

  GraphComparator(OsmMapPtr map1, OsmMapPtr map2);
  ~GraphComparator() = default;

  double compareMaps() override;

  /**
   * Returns the 90% confidence interval.
   */
  double getConfidenceInterval() const { return _ci; }
  double getMeanScore() const { return _mean; }
  double getMedianScore() const { return _median; }
  double getStandardDeviation() const { return _s; }

  void setDebugImages(bool di) { _debugImages = di; }
  void setIterations(int i) { _iterations = i; }
  void setMaxThreads(int t) { _maxThreads = t; }

  void drawCostDistance(
    OsmMapPtr map, std::vector<geos::geom::Coordinate>& c, QString output, double& maxGraphCost);

private:

  /** Number of times to iterate of the map calculating scores */
  int _iterations;
  /** Median score of all iterations */
  double _median;
  /** Mean score of all iterations */
  double _mean;
  /** Confidence interval of scores over all iterations */
  double _ci;
  /** Sampled standard deviation over all iterations */
  double _s;
  /** Flag to output debug images of graph comparisons */
  bool _debugImages;
  /** Structure of information to build an iteration of the comparator */
  struct WorkInfo
  {
    int index;
    geos::geom::Coordinate coord;
    OsmMapPtr referenceMap;
  };
  /** Queue of WorkInfo objects preallocated for the threads to work on */
  std::queue<WorkInfo> _workQueue;
  std::mutex _workQueueMutex;
  /** Maximum number of threads to process the graph comparator iterations */
  int _maxThreads;
  /** Pool of threads to process iterations of _maxThreads count */
  std::vector<std::thread> _threadPool;
  /** Vector of error values - returned from _calculateError() in each iteration */
  std::vector<double> _results;
  std::mutex _resultsMutex;
  std::mutex _logMutex;
  /**
   * @brief _graphCompareThreadFunc - Thread function that processes a graph comparison operation
   */
  void _graphCompareThreadFunc();

  cv::Mat _calculateCostDistance(
    OsmMapPtr map, geos::geom::Coordinate c, double& maxGraphCost, const Tgs::RandomPtr& random);

  void _calculateRasterCost(cv::Mat& mat, const Tgs::RandomPtr& random) const;

  void _exportGraphImage(
    OsmMapPtr map, const ShortestPath& sp, const QString& path,
    const geos::geom::Coordinate& coord) const;

  void _init();

  cv::Mat _paintGraph(
    const ConstOsmMapPtr& map, const DirectedGraph& graph, const ShortestPath& sp,
    double& maxGraphCost) const;

  void _paintWay(
    cv::Mat& mat, const ConstOsmMapPtr& map, const ConstWayPtr& way, double friction,
    double startCost, double endCost) const;
};

}

#endif // GRAPHCOMPARATOR_H
