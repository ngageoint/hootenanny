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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "UnionPolygonsVisitor.h"

// geos
#include <geos/geom/GeometryFactory.h>

// hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/util/Factory.h>

using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, UnionPolygonsVisitor)

UnionPolygonsVisitor::UnionPolygonsVisitor()
  : _result(GeometryFactory::getDefaultInstance()->createEmptyGeometry())
{
}

void UnionPolygonsVisitor::visit(const std::shared_ptr<const Element>& e)
{
  if (e->getElementType() == ElementType::Node)
    return;

  if (AreaCriterion().isSatisfied(e))
  {
    std::shared_ptr<Geometry> g = ElementToGeometryConverter(_map->shared_from_this()).convertToGeometry(e);
    if (g && !g->isEmpty())
    {
      _result = g->Union(_result.get());
      _numAffected++;
    }
  }
}

}
