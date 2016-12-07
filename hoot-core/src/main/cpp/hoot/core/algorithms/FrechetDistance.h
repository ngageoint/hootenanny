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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef FRECHETDISTANCE_H
#define FRECHETDISTANCE_H

//  Boost
#include <boost/multi_array.hpp>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/Units.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/elements/Way.h>

// TGS
#include <tgs/SharedPtr.h>

namespace hoot
{

typedef boost::multi_array<Meters, 2> frechet_matrix;
typedef frechet_matrix::index frechet_index;
typedef std::pair<int, int> vertex_match;
typedef std::vector<vertex_match> frechet_subline;

typedef shared_ptr<LineString> LineStringPtr;

/** Forward declaration for friend class so that the test class can access protected members
 */
class FrechetDistanceTest;

/** Class for calculating Frechet Distance between two ways and calculating maximal subline matches.
 *  Algorithm developed from "A new merging process for data integration base on the descrete Frechet distance"
 *  (http://thomas.devogele.free.fr/articles/avant_2003_selection/DevogeleSDH2002.pdf)
 */
class FrechetDistance
{
public:
  /**
   * @brief FrechetDistance class constructor
   * @param map - pointer to the OSM map containing the two ways to be compared
   * @param way1 - pointer to the first way to be compared
   * @param way2 - pointer to the second way to be compared
   */
  FrechetDistance(const ConstOsmMapPtr& map, const ConstWayPtr& way1, const ConstWayPtr& way2, Radians maxAngle = M_PI);

  /**
   * @brief distance - calculate the Frechet distance between the two ways, _w1 and _w2
   * @return Frechet distance in meters
   */
  Meters distance();

  /**
   * @brief maxSubline - find the maximum length, in nodes, subline for the two ways
   * @param maxDistance - max distance between the two ways to consider as a match
   * @return set of matching node indices that represent the subline
   */
  frechet_subline maxSubline(Meters maxDistance = 15.0);

protected:
  /**
   * @brief calculateMatrix
   * @return distinct Frechet matrix of distances calculated
   */
  frechet_matrix calculateMatrix();
  /**
   * @brief advanceAndCheck
   * @param rows - constant number of rows in the Frechet matrix
   * @param cols - constant number of columns in the Frechet matrix
   * @param r - current row number, could be modified in the advance operation
   * @param c - current column number, could be modified in the advance operation
   * @param max_frechet - maximum distance
   * @param advance_row - boolean flag to advance the row
   * @param advance_col - boolean flag to advance the column
   */
  void advanceAndCheck(const int rows, const int cols, int& r, int& c, Meters& max_frechet, bool advance_row, bool advance_col);
  /**
   * @brief advanceAndCheckRow - helper function that calls advanceAndCheck that advances
   *  a row
   * See advanceAndCheck for parameter information
   */
  void advanceAndCheckRow(const int rows, const int cols, int& r, int& c, Meters& max_frechet);
  /**
   * @brief advanceAndCheckColumn - helper function that calls advanceAndCheck that advances
   *  a column
   * See advanceAndCheck for parameter information
   */
  void advanceAndCheckColumn(const int rows, const int cols, int& r, int& c, Meters& max_frechet);
  /**
   * @brief advanceAndCheckBoth - helper function that calls advanceAndCheck that advances
   *  both a row and a column
   * See advanceAndCheck for parameter information
   */
  void advanceAndCheckBoth(const int rows, const int cols, int& r, int& c, Meters& max_frechet);
  /**
   * @brief getHeading - get the heading, in radians, of the way specified at the node index specified
   *  used to ensure that segments of two ways are both heading the same general direction
   * @param way - pointer to way to get the heading of
   * @param index - node index into way of where to get the heading at
   * @return heading of way at the point index in radians
   */
  Radians getHeading(WayPtr way, int index);
  /**
   * @brief getHeadingWay1 and getHeadingWay2 - helper functions to make using getHeading easier to use
   * @param index - node index into way of where to get the heading at
   * @return heading of way at the point index in radians
   */
  Radians getHeadingWay1(int index);
  Radians getHeadingWay2(int index);

  Radians getHeadingAvg(WayPtr way, int index);

protected:
  //  Allow test class to access protected members for white box testing
  friend class FrechetDistanceTest;
  //  Copies of the map and both ways
  OsmMapPtr _map;
  WayPtr _w1;
  WayPtr _w2;
  //  Ways converted to LineString objects
  LineStringPtr _ls1;
  LineStringPtr _ls2;
  //  Distinct Frechet matrix
  frechet_matrix _matrix;
  //  Locations along _w2 for each node in _w1
  vector<WayLocation> _locations_w1;
  //  Locations along _w1 for each node in _w2
  vector<WayLocation> _locations_w2;
  //  Distances between each node in _w1 and the corresponding WayLocation on _w2
  vector<Meters> _distances_w1;
  //  Distances between each node in _w2 and the corresponding WayLocation on _w1
  vector<Meters> _distances_w2;
  //  Max difference in angles to calculate Frechet distance for
  Radians _angleDiff;
  //  Max angle to compare _angleDiff against
  Radians _maxAngle;
};

}
#endif // FRECHETDISTANCE_H
