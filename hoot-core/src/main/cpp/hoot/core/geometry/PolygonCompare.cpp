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

#include "PolygonCompare.h"

// GEOS
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Point.h>
#include <geos/util/IllegalArgumentException.h>

// TGS
#include <tgs/RStarTree/HilbertCurve.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/GeometryToElementConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/StringUtils.h>

using namespace geos::geom;
using namespace Tgs;

namespace hoot
{

PolygonCompare::PolygonCompare(const Envelope& e) :
_e(e),
_curve(std::make_shared<HilbertCurve>(2, 8)),
_size((1 << 8) - 1)
{
}

bool PolygonCompare::operator()(const std::shared_ptr<geos::geom::Geometry>& p1,
                                const std::shared_ptr<geos::geom::Geometry>& p2) const
{
  const Envelope* e1 = p1->getEnvelopeInternal();
  const Envelope* e2 = p2->getEnvelopeInternal();
  double x1 = (e1->getMinX() + e1->getMaxX()) / 2.0;
  double x2 = (e2->getMinX() + e2->getMaxX()) / 2.0;
  double y1 = (e1->getMinY() + e1->getMaxY()) / 2.0;
  double y2 = (e2->getMinY() + e2->getMaxY()) / 2.0;

  int i1[2], i2[2];
  i1[0] = (int)((x1 - _e.getMinX()) / (_e.getWidth()) * _size);
  i1[1] = (int)((y1 - _e.getMinY()) / (_e.getHeight()) * _size);
  i2[0] = (int)((x2 - _e.getMinX()) / (_e.getWidth()) * _size);
  i2[1] = (int)((y2 - _e.getMinY()) / (_e.getHeight()) * _size);

  bool result;
  long h1 = _curve->encode(i1);
  long h2 = _curve->encode(i2);
  if (h1 == h2)
    result = p1 < p2;
  else
    result = h1 < h2;

  return result;
}

}
