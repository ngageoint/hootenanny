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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef GRAPHCOMPARATOR_H
#define GRAPHCOMPARATOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/scoring/BaseComparator.h>

namespace hoot
{

class DirectedGraph;
class ShortestPath;
class Way;

class GraphComparator : public BaseComparator
{
public:
  GraphComparator(OsmMapPtr map1, OsmMapPtr map2);

  virtual ~GraphComparator() {}

  double compareMaps();

  /**
   * Returns the 90% confidence interval.
   */
  double getConfidenceInterval() { return _ci; }

  double getMeanScore() { return _mean; }

  double getMedianScore() { return _median; }

  double getStandardDeviation() { return _s; }

  void setDebugImages(bool di) { _debugImages = di; }

  void setIterations(int i) { _iterations = i; }

  void drawCostDistance(OsmMapPtr map, std::vector<geos::geom::Coordinate>& c, QString output);

private:

  int _iterations;
  geos::geom::Coordinate _r;
  double _median;
  double _mean;
  // confidence interval
  double _ci;
  // sampled standard deviation
  double _s;
  double _maxGraphCost;
  bool _debugImages;

  cv::Mat _calculateCostDistance(OsmMapPtr map, geos::geom::Coordinate c);

  void _calculateRasterCost(cv::Mat& mat);

  void _exportGraphImage(OsmMapPtr map, DirectedGraph& graph, ShortestPath& sp,
                         QString path);

  void _init();

  cv::Mat _paintGraph(OsmMapPtr map, DirectedGraph& graph, ShortestPath& sp);

  void _paintWay(cv::Mat& mat, ConstOsmMapPtr map, WayPtr way, double friction,
    double startCost, double endCost);
};

}

#endif // GRAPHCOMPARATOR_H
