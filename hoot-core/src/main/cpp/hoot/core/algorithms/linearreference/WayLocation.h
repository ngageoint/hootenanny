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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef __WAY_LOCATION_H__
#define __WAY_LOCATION_H__

// GEOS
#include <geos/geom/Coordinate.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/Units.h>
#include <hoot/core/elements/Way.h>

// TGS
#include <tgs/SharedPtr.h>

namespace hoot
{
  class Node;
  class Way;

  using namespace geos::geom;

/**
 * The WayLocation has a segment index and segmentFraction. The segment index is
 * [0, node count - 1]. The segment fraction is [0, 1). If a segment fraction >= 1 is passed in then
 * the index is automatically incremented and the segment fraction is set to 0.
 *
 * @author RoadMatcher
 * http://www.vividsolutions.com/products.asp?catg=spaapp&code=roadmatcher
 * The ideas were shamelessly taken from RoadMatcher, but reimplemented in C++ with Hootenanny
 * appropriate data structures.
 */
class WayLocation
{
public:

  WayLocation();

  /**
   * Constructs a way location that is relative to the start of the way. If the location is
   * outside the bounds of the way it will be cropped to the beginning or end of the way. An error
   * will not be reported.
   */
  WayLocation(ConstOsmMapPtr map, ConstWayPtr way, double distance);

  WayLocation(ConstOsmMapPtr map, ConstWayPtr way, int segmentIndex, double segmentFraction);

  virtual ~WayLocation() {}

  Meters calculateDistanceFromEnd() const;

  /**
   * Calculates the meters of distance this location is along the way.
   */
  Meters calculateDistanceOnWay() const;

  static int compareLocationValues(int segmentIndex0, double segmentFraction0,
                                   int segmentIndex1, double segmentFraction1);

  /**
   *  Compares this object with the specified object for order.
   *
   *@param  other The WayLocation with which this WayLocation is being compared
   *@return a negative integer, zero, or a positive integer as this WayLocation
   *      is less than, equal to, or greater than the specified WayLocation.
   */
  int compareTo(const WayLocation& other) const;

  /**
   * Construct a way location that represents the end of the specified way.
   */
  static WayLocation createAtEndOfWay(const ConstOsmMapPtr &map, const ConstWayPtr way);

  /**
   * Computes the location of a point a given length along a line segment.
   * If the length exceeds the length of the line segment the last
   * point of the segment is returned.
   * If the length is negative the first point
   * of the segment is returned.
   *
   * @param p0 the first point of the line segment
   * @param p1 the last point of the line segment
   * @param length the length to the desired point
   * @return the Coordinate of the desired point
   */
  static const Coordinate pointAlongSegmentByFraction(const Coordinate& p0, const Coordinate& p1,
                                                      double frac);

  const ConstOsmMapPtr& getMap() const { return _map; }

  const shared_ptr<const Way>& getWay() const { return _way; }

  int getSegmentIndex() const { return _segmentIndex; }

  double getSegmentFraction() const { return _segmentFraction; }

  bool isNode() const { return _segmentFraction <= 0.0 || _segmentFraction >= 1.0; }
  bool isFirst() const { return _segmentIndex == 0 && _segmentFraction == 0.0; }
  bool isLast() const { return _segmentIndex == (int)_way->getNodeCount() - 1; }

  bool isValid() const { return _segmentIndex != -1; }

  const Coordinate getCoordinate() const;

  /**
   * Move the location on the way. Negative values will move closer to the beginning of the way.
   * Higher values will move towards the end of the way. This will create a new WayLocation and
   * return it. The result will not go past the beginning or end of the line.
   */
  WayLocation move(Meters distance) const;

  QString toString() const;

protected:
  ConstOsmMapPtr _map;
  ConstWayPtr _way;
  int _segmentIndex;
  double _segmentFraction;

};

inline bool operator==(const WayLocation& a, const WayLocation& b)
{
  return a.compareTo(b) == 0;
}

inline bool operator!=(const WayLocation& a, const WayLocation& b)
{
  return a.compareTo(b) != 0;
}

inline bool operator<(const WayLocation& a, const WayLocation& b)
{
  return a.compareTo(b) < 0;
}

inline bool operator>(const WayLocation& a, const WayLocation& b)
{
  return a.compareTo(b) > 0;
}

inline bool operator<=(const WayLocation& a, const WayLocation& b)
{
  return a.compareTo(b) <= 0;
}

inline bool operator>=(const WayLocation& a, const WayLocation& b)
{
  return a.compareTo(b) >= 0;
}

typedef shared_ptr<WayLocation> WayLocationPtr;
typedef shared_ptr<const WayLocation> ConstWayLocationPtr;

}

#endif
