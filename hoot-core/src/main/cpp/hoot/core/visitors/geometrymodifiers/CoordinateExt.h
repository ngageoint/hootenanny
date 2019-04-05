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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef COORDINATEEXT_H
#define COORDINATEEXT_H

#include <geos/geom/Coordinate.h>

using namespace geos::geom;

namespace hoot
{

class CoordinateExt : public Coordinate
{
public:
  CoordinateExt(Coordinate c) : Coordinate(c) {}
  CoordinateExt(double xNew=0.0, double yNew=0.0, double zNew=DoubleNotANumber) : Coordinate( xNew, yNew, zNew ) {}

  double getLength() const
  {
    return sqrt(x * x + y * y);
  }

  void normalize()
  {
    double len = getLength();
    if( len == 0 ) return;
    x /= len;
    y /= len;
  }

  CoordinateExt operator + (const CoordinateExt  val) const
  {
    CoordinateExt sum( x + val.x, y + val.y, z + val.z );
    return sum;
  }

  CoordinateExt operator - (const CoordinateExt val) const
  {
    CoordinateExt dif( x - val.x, y - val.y, z - val.z );
    return dif;
  }
};

}

#endif // COORDINATEEXT_H
