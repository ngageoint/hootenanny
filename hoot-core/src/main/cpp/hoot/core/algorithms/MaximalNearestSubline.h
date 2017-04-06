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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef MAXIMALNEARESTSUBLINE_H
#define MAXIMALNEARESTSUBLINE_H

/**
 * @author RoadMatcher
 * @copyright GPL
 * http://www.vividsolutions.com/products.asp?catg=spaapp&code=roadmatcher
 * The ideas were shamelessly taken from RoadMatcher, but reimplemented in C++ with Hootenanny
 * appropriate data structures.
 */

// GEOS
#include <geos/geom/Coordinate.h>

// Standard
#include <vector>

// TGS
#include <tgs/SharedPtr.h>

// Hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/OsmMap.h>

#include "linearreference/WayLocation.h"
#include "linearreference/LocationOfPoint.h"

namespace hoot
{
  // Standard
  using namespace std;

  class Way;

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

  static boost::shared_ptr<Way> getMaximalNearestSubline(const OsmMapPtr &map, ConstWayPtr a,
    ConstWayPtr b, Meters minSplitSize, Meters maxRelevantDistance);

  /**
   * Split "a" into multiple ways with consistent topology based on "b".
   *
   * The resulting ways will not be placed in map.
   * @param mnsIndex The index of way that is the MNS is placed in mnsIndex.
   */
  vector< boost::shared_ptr<Way> > splitWay(boost::shared_ptr<OsmMap> map) { int a; return splitWay(map, a); }
  vector< boost::shared_ptr<Way> > splitWay(boost::shared_ptr<OsmMap> map, int& mnsIndex);

private:

  ConstWayPtr _a, _b;
  LocationOfPoint _aPtLocator;
  std::vector<WayLocation> _maxInterval;
  boost::shared_ptr<const Node> _endNode;
  boost::shared_ptr<const Node> _startNode;
  Meters _minSplitSize;
  ConstOsmMapPtr _map;
  Meters _maxRelevantDistance;
  Radians _maxRelevantAngle;
  static Meters _headingDelta;

  /**
   * Where to start the search for a valid MNS.
   */
  WayLocation _searchStart;

  std::vector< boost::shared_ptr<Node> > _createdNodes;

  Meters _calculateIntervalLength();

  void _expandInterval(WayLocation& loc);

  void _findNearestOnA(const geos::geom::Coordinate& bPt);

  /**
   * Returns the end node in the MNS.
   */
  boost::shared_ptr<const Node> _getEndNode();

  const vector<WayLocation>& _getInterval();

  /**
   * Returns the start node in the MNS
   */
  boost::shared_ptr<const Node> _getStartNode();

  bool _isInBounds(const WayLocation& wl, const boost::shared_ptr<LineString>& ls);

  bool _isOutsideInterval(int ia);
};

}

#endif // MAXIMALNEARESTSUBLINE_H
