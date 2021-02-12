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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */

#ifndef MAXIMALNEARESTSUBLINE_H
#define MAXIMALNEARESTSUBLINE_H

// GEOS
#include <geos/geom/Coordinate.h>

// Standard
#include <vector>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/algorithms/linearreference/LocationOfPoint.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Units.h>

namespace hoot
{

/**
 * Computes the Maximal Nearest Subline of a given linestring relative
 * to another linestring.
 * The Maximal Nearest Subline of A relative to B is the shortest subline
 * of A which contains all the points of A which are the nearest points to
 * the points in B.
 * This effectively "trims" the ends of A which are not near to B.
 * <p>
 * An exact computation of the MNS would require computing a line Voronoi.
 * For this reason, the algorithm used in this class is heuristic-based.
 * It may compute a geometry which is shorter than the actual MNS.
 *
 * @author RoadMatcher
 * @copyright GPL
 * http://www.vividsolutions.com/products.asp?catg=spaapp&code=roadmatcher
 * The ideas were shamelessly taken from RoadMatcher, but reimplemented in C++ with Hootenanny
 * appropriate data structures.
 */
class MaximalNearestSubline
{
public:

  /**
   * Find the subline in a based on b
   */
  MaximalNearestSubline(const ConstOsmMapPtr& map, ConstWayPtr a, ConstWayPtr b,
                        Meters minSplitSize, Meters maxRelevantDistance,
                        Radians maxRelevantAngle = -1,
                        Meters headingDelta = ConfigOptions().getWayMatcherHeadingDelta());

  /**
   * Computes the interval (range) containing the Maximal Nearest Subline.
   *
   * @param map Map that contains both a and b.
   * @return an array containing the minimum and maximum locations of the
   * Maximal Nearest Subline of <code>A</code>
   */
  const std::vector<WayLocation>& getInterval();

  static WayPtr getMaximalNearestSubline(const OsmMapPtr &map, ConstWayPtr a,
    ConstWayPtr b, Meters minSplitSize, Meters maxRelevantDistance);

  /**
   * Split "a" into multiple ways with consistent topology based on "b".
   *
   * The resulting ways will not be placed in map.
   * @param mnsIndex The index of way that is the MNS is placed in mnsIndex.
   */
  std::vector<WayPtr> splitWay(OsmMapPtr map) { int a; return splitWay(map, a); }
  std::vector<WayPtr> splitWay(OsmMapPtr map, int& mnsIndex);

private:

  ConstWayPtr _a, _b;
  LocationOfPoint _aPtLocator;
  std::vector<WayLocation> _maxInterval;
  ConstNodePtr _endNode;
  ConstNodePtr _startNode;
  Meters _minSplitSize;
  ConstOsmMapPtr _map;
  Meters _maxRelevantDistance;
  Radians _maxRelevantAngle;
  static Meters _headingDelta;

  /**
   * Where to start the search for a valid MNS.
   */
  WayLocation _searchStart;

  std::vector<NodePtr> _createdNodes;

  Meters _calculateIntervalLength();

  void _expandInterval(WayLocation& loc);

  void _findNearestOnA(const geos::geom::Coordinate& bPt);

  /**
   * Returns the end node in the MNS.
   */
  ConstNodePtr _getEndNode();

  const std::vector<WayLocation>& _getInterval();

  /**
   * Returns the start node in the MNS
   */
  ConstNodePtr _getStartNode();

  bool _isInBounds(const WayLocation& wl, const std::shared_ptr<geos::geom::LineString>& ls);

  bool _isOutsideInterval(int ia);
};

}

#endif // MAXIMALNEARESTSUBLINE_H
