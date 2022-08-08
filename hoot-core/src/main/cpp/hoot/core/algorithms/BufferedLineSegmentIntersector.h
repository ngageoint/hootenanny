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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef BUFFEREDLINESEGMENTINTERSECTOR_H
#define BUFFEREDLINESEGMENTINTERSECTOR_H

// geos
#include <geos/geom/LineSegment.h>

// hoot
#include <hoot/core/util/Units.h>

namespace hoot
{

class BufferedLineSegmentIntersector
{
public:

  BufferedLineSegmentIntersector() = default;

  /**
   * @brief intersect Buffer a and intersect it with b. The result is put into @a result. Return
   * true if the line intersects in more than one point.
   *
   * This was done with very little tuning. A bit of tuning will likely make it significantly
   * faster.
   */
  bool intersect(
    const geos::geom::LineSegment& a, Meters buffer, const geos::geom::LineSegment& b,
    geos::geom::LineSegment& result) const;

  /**
   * @brief isWithinLineSegment determines if the coordinate c is within the endpoints of ls. It is
   * assumed that c falls on or approximately on the line formed by ls (as in the line that goes to
   * inifite).
   */
  bool isWithinLineSegment(
    const geos::geom::LineSegment& ls, const geos::geom::Coordinate& c) const;

  /**
   * @brief circleIntersection intersects a circle with a line segment and returns the intersecting
   * points in p1 and p2.
   *
   * If there are no intersections then p1 and p2 are set to null. If there is only on intersection
   * then p2 is set to null. The algorithm is taken from [1].
   *
   * 1. http://mathworld.wolfram.com/Circle-LineIntersection.html
   */
  void circleIntersection(
    const geos::geom::Coordinate& origin, Meters radius, const geos::geom::LineSegment& l,
    geos::geom::Coordinate& p1, geos::geom::Coordinate& p2) const;
};

}

#endif // BUFFEREDLINESEGMENTINTERSECTOR_H
