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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "LengthOfWaysVisitor.h"

// geos
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/util/Factory.h>

// Std
#include <float.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, LengthOfWaysVisitor)

LengthOfWaysVisitor::LengthOfWaysVisitor()
  : _total(0.0),
    _smallest(0.0),
    _largest(0.0)
{
}

Meters LengthOfWaysVisitor::getLengthOfWays(const OsmMapPtr& map, ElementPtr e)
{
  LengthOfWaysVisitor v;
  v.setOsmMap(map.get());
  e->visitRo(*map, v);
  return v.getLengthOfWays();
}

void LengthOfWaysVisitor::visit(const ConstElementPtr& e)
{
  if (e->getElementType() == ElementType::Way)
  {
    const ConstWayPtr w = _map->getWay(e->getId());
    const Meters length = ElementToGeometryConverter(_map->shared_from_this()).convertToLineString(w)->getLength();

    _total += length;

    if (_smallest == 0.0 || length < _smallest)
      _smallest = length;
    if (length > _largest)
      _largest = length;

    _numAffected++;
  }
}

}
