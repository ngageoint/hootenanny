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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef WAYAVERAGER_H
#define WAYAVERAGER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

// GEOS
namespace geos
{
  namespace geom
  {
    class LineString;
  }
}

namespace hoot
{

class WayAverager
{
public:

  static QString className() { return "hoot::WayAverager"; }

  /**
   * @brief Constructor
   * @param map map owning ways to be averaged
   * @param w1 the first way to be averaged
   * @param w2 the second way to be averaged
   */
  WayAverager(const OsmMapPtr& map, const WayPtr& w1, const WayPtr& w2);

  /**
   * @brief replaceWaysWithAveragedWay averages the two input ways into a new output way. The two
   * input ways are removed from their containing map.
   * @return the averaged way
   */
  WayPtr replaceWaysWithAveragedWay();
  /**
   * @brief replaceWaysWithAveragedWay averages the two input ways into a new output way. The two
   * input ways are removed from their containing map.
   * @param map map owning ways to be averaged
   * @param w1 the first way to be averaged
   * @param w2 the second way to be averaged
   * @return the averaged way
   */
  static WayPtr replaceWaysWithAveragedWay(
    const OsmMapPtr& map, const WayPtr& w1, const WayPtr& w2);

  /**
   * @brief getMaxMovement1 returns the maximum distance that w1 moved.
   */
  Meters getMaxMovement1() const { return _maxMovement1; }
  /**
   * @brief getMaxMovement2 returns the maximum distance that w1 moved.
   */
  Meters getMaxMovement2() const { return _maxMovement2; }

  /**
   * @brief getMeanMovement1 returns the mean of the distance that the nodes in way 1 moved during
   * the last average operation.
   */
  Meters getMeanMovement1() const { return _meanMovement1; }
  /**
   * @brief getMeanMovement2 returns the mean of the distance that the nodes in way 2 moved during
   * the last average operation.
   */
  Meters getMeanMovement2() const { return _meanMovement2; }

private:

  WayPtr _w1;
  WayPtr _w2;

  OsmMapPtr _map;

  Meters _meanMovement1, _meanMovement2;
  Meters _sumMovement1, _sumMovement2;
  Meters _maxMovement1, _maxMovement2;
  int _moveCount1, _moveCount2;

  /**
   * Merges the two nodes and returns a newly created node id.
   */
  long _merge(const ConstNodePtr& node1, double weight1, const ConstNodePtr& node2, double weight2);

  /**
   * Moves the node with id n half way to the specified line string. The input node id is returned
   * for convenience.
   */
  long _moveToLine(
    long ni, double nWeight, const geos::geom::LineString* ls, double lWeight, int w1OrW2);

  /**
   * Returns the coordinate if you were to move the node with id n halfway toward the specified
   * line string.
   */
  geos::geom::Coordinate _moveToLineAsCoordinate(
    long n, double nWeight, const geos::geom::LineString* ls, double lWeight) const;
};

}

#endif // WAYAVERAGER_H
