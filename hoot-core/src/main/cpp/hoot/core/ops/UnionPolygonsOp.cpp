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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "UnionPolygonsOp.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/GeometryConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/visitors/UnionPolygonsVisitor.h>

// geos
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>

using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, UnionPolygonsOp)

UnionPolygonsOp::UnionPolygonsOp()
{
}

void UnionPolygonsOp::apply(boost::shared_ptr<OsmMap>& map)
{
  UnionPolygonsVisitor v;
  map->visitRo(v);
  boost::shared_ptr<Geometry> g = v.getUnion();
  LOG_VART(g.get());

  OsmMapPtr result(new OsmMap());
  GeometryConverter(result).convertGeometryToElement(g.get(), Status::Unknown1, ElementData::CIRCULAR_ERROR_EMPTY);

  map.reset(new OsmMap(result));
  LOG_VART(map.get());
}

}
