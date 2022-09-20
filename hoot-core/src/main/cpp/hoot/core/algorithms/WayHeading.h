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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef WAYHEADING_H
#define WAYHEADING_H

// hoot
#include <hoot/core/util/Units.h>

namespace hoot
{

class WayLocation;

/**
 * @brief The WayHeading class calculates the heading of a way at any point on the way.
 */
class WayHeading
{
public:

  /**
   * @brief calculateHeading calculates the heading of a way at any point on the way.
   * @param delta Delta is the distance to travel up or down the line when calculating the heading.
   * A larger delta will be less exact for the given location, but will also average out small
   * perturbations.
   */
  static Radians calculateHeading(const WayLocation& loc, Meters delta = 0.001);
  static Radians calculateHeading(const geos::geom::Coordinate& c1, const geos::geom::Coordinate& c2);

  static geos::geom::Coordinate calculateVector(const WayLocation& loc, Meters delta = 0.001);
  static geos::geom::Coordinate calculateVector(const geos::geom::Coordinate& c1, const geos::geom::Coordinate& c2);

  static Radians deltaMagnitude(Radians r1, Radians r2);
};

}

#endif // WAYHEADING_H
