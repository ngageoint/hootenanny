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

#include "UpdateMeasurementTagsVisitor.h"

// Hoot
//#include <hoot/core/elements/MapProjector.h>
//#include <hoot/core/elements/RelationMap.h>
//#include <hoot/core/elements/Way.h>
//#include <hoot/core/geometry/CoordinateExt.h>
//#include <hoot/core/geometry/ElementToGeometryConverter.h>
//#include <hoot/core/index/ElementToRelationMap.h>
//#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/Factory.h>

// Boost
//#include <boost/shared_ptr.hpp>

// Geos
//#include <geos/algorithm/MinimumDiameter.h>
//#include <geos/geom/GeometryFactory.h>
//#include <geos/geom/LineString.h>

//using namespace geos::geom;
//using namespace geos::algorithm;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, UpdateMeasurementTagsVisitor)

void UpdateMeasurementTagsVisitor::visit(const ElementPtr& e)
{
  //  Nodes aren't 2D objects with area, length, or width
  if (e->getElementType() == ElementType::Node)
    return;
  //  Update the area, length, width tags based on their prior existance
  const Tags& tags = e->getTags();
  bool area = tags.contains(MetadataTags::FeatureArea());
  bool length = tags.contains(MetadataTags::Length());
  bool width = tags.contains(MetadataTags::Width());
  //  Setting these flags for each element controls the updates
  setArea(area);
  setLength(length);
  setWidth(width);
  //  Update the calculate variable
  _shouldCalculate = area || length || width;
  //  Apply the measurement visitor to the element
  MeasurementTagsVisitor::visit(e);
}

} // namespace hoot
