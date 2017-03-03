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

#ifndef WAYHEADING_H
#define WAYHEADING_H

// GEOS
#include <geos/geom/Coordinate.h>

// hoot
#include <hoot/core/util/Units.h>

namespace hoot
{

class WayLocation;

  using namespace geos::geom;

/**
 * Calculates the heading of a way at any point on the way.
 */
class WayHeading
{
public:

  /**
   * Delta is the distance to travel up or down the line when calculating the heading. A larger
   * delta will be less exact for the given location, but will also average out small perturbations.
   */
  static Radians calculateHeading(const WayLocation& loc, Meters delta = 0.001);

  static Radians calculateHeading(const Coordinate& c1, const Coordinate& c2);

  static Coordinate calculateVector(const WayLocation& loc, Meters delta = 0.001);

  static Coordinate calculateVector(const Coordinate& c1, const Coordinate& c2);

  static Radians deltaMagnitude(Radians r1, Radians r2);

};

}

#endif // WAYHEADING_H
