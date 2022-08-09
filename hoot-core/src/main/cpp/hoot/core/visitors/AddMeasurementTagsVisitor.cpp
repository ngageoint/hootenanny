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

#include "AddMeasurementTagsVisitor.h"

// Hoot
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/RelationMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/geometry/CoordinateExt.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/index/ElementToRelationMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/Factory.h>

// Boost
#include <boost/shared_ptr.hpp>

// Geos
#include <geos/algorithm/MinimumDiameter.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>

using namespace geos::geom;
using namespace geos::algorithm;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, AddMeasurementTagsVisitor)

AddMeasurementTagsVisitor::AddMeasurementTagsVisitor(bool area, bool length, bool width)
  : MeasurementTagsVisitor(area, length, width)
{
}

bool AddMeasurementTagsVisitor::shouldCalculate(const ElementPtr& /*pElement*/) const
{
  return getArea() || getLength() || getWidth();
}

} // namespace hoot
