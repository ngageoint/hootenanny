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
 * @copyright Copyright (C) 2015, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef DISTANCE_H
#define DISTANCE_H

// GEOS


// Hoot
#include <hoot/core/elements/Node.h>

namespace hoot
{

class Distance
{
public:

  double static euclidean(const Node& n1, const Node& n2);

  double static euclidean(const geos::geom::Coordinate& c1, const geos::geom::Coordinate& c2);
};

inline double Distance::euclidean(const Node& n1, const Node& n2)
{
  return sqrt((n1.getX() - n2.getX()) * (n1.getX() - n2.getX()) +
              (n1.getY() - n2.getY()) * (n1.getY() - n2.getY()));
}

inline double Distance::euclidean(const geos::geom::Coordinate& c1,
                                  const geos::geom::Coordinate& c2)
{
  return sqrt((c1.x - c2.x) * (c1.x - c2.x) +
              (c1.y - c2.y) * (c1.y - c2.y));
}

}

#endif // DISTANCE_H
