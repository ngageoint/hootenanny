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

#ifndef WAYAVERAGER_H
#define WAYAVERAGER_H

// Hoot
#include <hoot/core/OsmMap.h>

// TGS
#include <tgs/SharedPtr.h>

namespace hoot
{

class Way;

class WayAverager
{
public:
  WayAverager(OsmMapPtr map, boost::shared_ptr<Way> w1, boost::shared_ptr<Way> w2);

  /**
   * Averages the two input ways into a new output way. The two input ways are removed from their
   * containing map.
   */
  boost::shared_ptr<Way> average();

  static boost::shared_ptr<Way> average(OsmMapPtr map, boost::shared_ptr<Way> w1, boost::shared_ptr<Way> w2);

  /**
   * Returns the maximum distance that w1 moved.
   */
  Meters getMaxMovement1() { return _maxMovement1; }

  /**
   * Returns the maximum distance that w1 moved.
   */
  Meters getMaxMovement2() { return _maxMovement2; }

  /**
   * Returns the mean of the distance that the nodes in way 1 moved during the last average
   * operation.
   */
  Meters getMeanMovement1() { return _meanMovement1; }

  /**
   * Returns the mean of the distance that the nodes in way 2 moved during the last average
   * operation.
   */
  Meters getMeanMovement2() { return _meanMovement2; }

protected:
  boost::shared_ptr<Way> _w1;
  boost::shared_ptr<Way> _w2;
  OsmMap& _map;
  Meters _meanMovement1, _meanMovement2;
  Meters _sumMovement1, _sumMovement2;
  Meters _maxMovement1, _maxMovement2;
  int _moveCount1, _moveCount2;

  /**
   * Merges the two nodes with the given node ids and returns a newly created node id.
   */
  long _merge(long ni1, double weight1, long ni2, double weight2);

  /**
   * Moves the node with id n half way to the specified line string. The input node id is returned
   * for convenience.
   */
  long _moveToLine(long ni, double nWeight, const LineString* ls, double lWeight, int w1OrW2);

  /**
   * Returns the coordinate if you were to move the node with id n halfway toward the specified
   * line string.
   */
  Coordinate _moveToLineAsCoordinate(long n, double nWeight, const LineString* ls, double lWeight);
};

}

#endif // WAYAVERAGER_H
