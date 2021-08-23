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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "UnionPolygonsOp.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/GeometryToElementConverter.h>

#include <hoot/core/io/IoUtils.h>
#include <hoot/core/visitors/WorstCircularErrorVisitor.h>

// geos
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>

using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, UnionPolygonsOp)

UnionPolygonsOp::UnionPolygonsOp() :
_combiner(std::make_shared<UnionPolygonsVisitor>())
{
}

void UnionPolygonsOp::apply(std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;

  map->visitRo(*_combiner);
  std::shared_ptr<Geometry> g = _combiner->getUnion();
  LOG_VART(g.get());

  OsmMapPtr result = std::make_shared<OsmMap>();
  GeometryToElementConverter(result).convertGeometryToElement(
    g.get(), Status::Unknown1, WorstCircularErrorVisitor::getWorstCircularError(map));

  map = std::make_shared<OsmMap>(result);
  LOG_VART(map.get());
}

}
