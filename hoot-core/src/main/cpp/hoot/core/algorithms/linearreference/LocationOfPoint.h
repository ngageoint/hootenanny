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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef LOCATIONOFPOINT_H
#define LOCATIONOFPOINT_H

// GEOS
#include <geos/geom/LineSegment.h>

// Hoot
#include <hoot/core/algorithms/linearreference/WayLocation.h>

namespace hoot
{

/**
 * @author RoadMatcher
 * @copyright GPL
 * http://www.vividsolutions.com/products.asp?catg=spaapp&code=roadmatcher
 * The ideas were shamelessly taken from RoadMatcher, but reimplemented in C++ with Hootenanny
 * appropriate data structures.
 */
class LocationOfPoint
{
public:

  LocationOfPoint(const ConstOsmMapPtr& map, ConstWayPtr way);

  static WayLocation locate(const ConstOsmMapPtr& map, ConstWayPtr way,
    const geos::geom::Coordinate& inputPt);

  /**
   * @brief locate finds the nearest location along a {@link Way} to a given point.
   * @param inputPt the coordinate to locate
   * @return the location of the nearest point
   */
  WayLocation locate(const geos::geom::Coordinate& inputPt) const;
  /**
   * @brief locate finds the point on way that is d distance along the way.
   * @param d the distance along way.
   * @return the location of the point d distance along the way.
   */
  geos::geom::Coordinate locate(double d);

  /**
   * @brief locateAfter finds the nearest location along a {@link Way} to a given point after the
   * specified minimum {@link LineStringLocation}. If possible the location returned will be
   * strictly greater than the <code>minLocation</code>. If this is not possible, the value returned
   * will equal <code>minLocation</code>.  (An example where this is not possible is when
   * minLocation = [end of line]).
   * @param inputPt the coordinate to locate
   * @param minLocation the minimum location for the point location
   * @return the location of the nearest point
   */
  WayLocation locateAfter(const geos::geom::Coordinate& inputPt,
                          const WayLocation& minLocation) const;

  /**
   * @brief isGreater tests whether a location given by a <index, segmentFraction> pair is located
   * after a {@link WayLocation}.
   * @param i the segment index
   * @param segFrac the fraction along the segment
   * @param loc a location
   * @return <code>true</code> if the first location is greater than the second
   */
  bool isGreater(int i, double segFrac, const WayLocation& loc) const;

  static double segmentFraction(const geos::geom::LineSegment& seg,
                                const geos::geom::Coordinate& inputPt);

private:

  ConstOsmMapPtr _map;
  ConstWayPtr _way;
  mutable double _length;
};

}

#endif // LOCATIONOFPOINT_H
