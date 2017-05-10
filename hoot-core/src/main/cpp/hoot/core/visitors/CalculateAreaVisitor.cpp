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
#include "CalculateAreaVisitor.h"

// geos
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/ElementConverter.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, CalculateAreaVisitor)

Meters CalculateAreaVisitor::getArea(const OsmMapPtr& map, ElementPtr e)
{
  CalculateAreaVisitor v;
  v.setOsmMap(map.get());
  e->visitRo(*map, v);
  return v.getArea();
}

void CalculateAreaVisitor::visit(const ConstElementPtr& e)
{
  boost::shared_ptr<Geometry> g = ElementConverter(_map->shared_from_this()).convertToGeometry(e);
  _total += g->getArea();
}

}
