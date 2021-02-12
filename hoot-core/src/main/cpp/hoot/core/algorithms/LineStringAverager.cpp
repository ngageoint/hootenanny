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

#include "LineStringAverager.h"

// GEOS
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>
#include <geos/operation/distance/DistanceOp.h>
using namespace geos::operation::distance;

// Qt
#include <QDebug>

using namespace geos::geom;

namespace hoot
{

LineStringAverager::LineStringAverager(const std::shared_ptr<LineString>& l1, const std::shared_ptr<LineString>& l2)
{
  _l1 = l1;
  _l2 = l2;
}

std::shared_ptr<LineString> LineStringAverager::average()
{
  const LineString* l1 = _l1.get();
  const LineString* l2 = _l2.get();
  const CoordinateSequence& cs1 = *l1->getCoordinatesRO();
  const CoordinateSequence& cs2 = *l2->getCoordinatesRO();

  CoordinateSequence& result = *GeometryFactory::getDefaultInstance()->
    getCoordinateSequenceFactory()->create(cs1.size() + cs2.size() - 2, 2);

  size_t i1 = 0;
  size_t i2 = 0;
  size_t ri = 0;

  result.setAt(_merge(cs1[i1++], cs2[i2++]), ri++);

  // while there is more than one point available in each line
  while (i1 < cs1.size() - 1 || i2 < cs2.size() - 1)
  {
    // if we're all out of cs1 points
    if (i1 == cs1.size() - 1)
    {
      result.setAt(_moveToLine(cs2[i2++], l1), ri++);
    }
    // if we're all out of cs2 points
    else if (i2 == cs2.size() - 1)
    {
      result.setAt(_moveToLine(cs1[i1++], l2), ri++);
    }
    else
    {
      const Coordinate& last = result[ri];

      const Coordinate& nc1 = _moveToLine(cs1[i1], l2);
      const Coordinate& nc2 = _moveToLine(cs2[i2], l1);

      if (nc1.distance(last) < nc2.distance(last))
      {
        result.setAt(nc1, ri++);
        i1++;
      }
      else
      {
        result.setAt(nc2, ri++);
        i2++;
      }
    }
  }

  // merge the last two nodes and move to the average location
  result.setAt(_merge(cs1[i1], cs2[i2]), ri++);

  return std::shared_ptr<LineString>(GeometryFactory::getDefaultInstance()->createLineString(&result));
}

std::shared_ptr<LineString> LineStringAverager::average(const std::shared_ptr<LineString>& l1,
                                                        const std::shared_ptr<LineString>& l2)
{
  LineStringAverager lsa(l1, l2);
  return lsa.average();
}

Coordinate LineStringAverager::_merge(const Coordinate& c1, const Coordinate& c2)
{
  return Coordinate((c1.x + c2.x) / 2.0, (c1.y + c2.y) / 2.0);
}

Coordinate LineStringAverager::_moveToLine(const Coordinate& c1, const LineString* ls)
{
  Point* point(GeometryFactory::getDefaultInstance()->createPoint(c1));

  // find the two closest points
  CoordinateSequence* cs = DistanceOp::closestPoints(point, const_cast<LineString*>(ls));

  Coordinate result = _merge(cs->getAt(0), cs->getAt(1));

  delete cs;
  delete point;

  return result;
}

}
