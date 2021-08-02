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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __POLYGON_COMPARE_H__
#define __POLYGON_COMPARE_H__

// GEOS
#include <geos/geom/Geometry.h>

// Standard
#include <memory>
#include <vector>

//  Forward declaration
namespace Tgs
{
class HilbertCurve;
}

namespace hoot
{

class PolygonCompare
{
public:

  PolygonCompare(const geos::geom::Envelope& e);

  bool operator()(const std::shared_ptr<geos::geom::Geometry>& p1,
                  const std::shared_ptr<geos::geom::Geometry>& p2) const;

private:

  PolygonCompare() = default;
  PolygonCompare& operator=(PolygonCompare& other);

  geos::geom::Envelope _e;
  std::shared_ptr<Tgs::HilbertCurve> _curve;
  double _size;
};

}

#endif  //  __POLYGON_COMPARE_H__
