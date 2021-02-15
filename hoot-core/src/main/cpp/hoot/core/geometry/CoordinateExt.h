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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef COORDINATEEXT_H
#define COORDINATEEXT_H

#include <geos/geom/Coordinate.h>

using namespace geos::geom;

namespace hoot
{

/*
 * Helper for 2D vector math with Coordinates
 */
class CoordinateExt : public Coordinate
{
public:

  CoordinateExt(Coordinate c) : Coordinate(c) {}
  CoordinateExt(double xNew = 0.0, double yNew = 0.0, double zNew = DoubleNotANumber)
    : Coordinate( xNew, yNew, zNew ) {}

  double length() const
  {
    return sqrt(x * x + y * y);
  }

  double lengthSquared() const
  {
    return x * x + y * y;
  }

  void normalize()
  {
    double len = length();
    if (len == 0) return;
    x /= len;
    y /= len;
  }

  double dotProduct(CoordinateExt val)
  {
    return x * val.x + y * val.y;
  }

  double crossProduct(CoordinateExt val)
  {
      return x * val.y - y * val.x;
  }

  CoordinateExt operator + (const CoordinateExt  val) const
  {
    CoordinateExt sum( x + val.x, y + val.y );
    return sum;
  }

  CoordinateExt operator - (const CoordinateExt val) const
  {
    CoordinateExt dif( x - val.x, y - val.y );
    return dif;
  }

  CoordinateExt operator * (const double val) const
  {
    CoordinateExt mul( x * val, y * val );
    return mul;
  }

  CoordinateExt operator * (const CoordinateExt val) const
  {
    CoordinateExt mul( x * val.x, y * val.y );
    return mul;
  }

  // https://www.codeproject.com/Tips/862988/Find-the-Intersection-Point-of-Two-Line-Segments
  static std::shared_ptr<CoordinateExt> lineSegementsIntersect(
    const CoordinateExt& p1, const CoordinateExt& p2, const CoordinateExt& q1,
    const CoordinateExt& q2)
  {
    std::shared_ptr<CoordinateExt> intersection;

    CoordinateExt r = p2 - p1;
    CoordinateExt s = q2 - q1;
    double rxs = r.crossProduct(s);
    double qpxr = (q1 - p1).crossProduct(r);

    if (isZero(rxs) && isZero(qpxr)) return intersection;
    if (isZero(rxs) && !isZero(qpxr)) return intersection;

    double t = (q1 - p1).crossProduct(s)/rxs;
    double u = (q1 - p1).crossProduct(r)/rxs;

    if (!isZero(rxs) && (0 <= t && t <= 1) && (0 <= u && u <= 1))
    {
      intersection = std::shared_ptr<CoordinateExt>(std::make_shared<CoordinateExt>(p1 + r*t));
      return intersection;
    }

    return intersection;
  }

private:

  static bool isZero(double d)
  {
    const double Epsilon = 1e-10;
    return fabs(d) < Epsilon;
  }
};

}

#endif // COORDINATEEXT_H
