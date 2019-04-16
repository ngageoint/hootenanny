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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "AddMeasurementTagsVisitor.h"

#include <geos/algorithm/MinimumDiameter.h>
#include <hoot/core/elements/ElementConverter.h>
#include <geos/geom/Polygon.h>
#include <boost/shared_ptr.hpp>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/CoordinateExt.h>

using namespace boost;
using namespace geos::geom;
using namespace geos::algorithm;

namespace hoot
{

void AddMeasurementTagsVisitor::visit(const ElementPtr& pElement)
{
  if( !_addArea && !_addWidth && !_addArea ) return;
  // only process area ways
  if( pElement->getElementType() != ElementType::Way ) return;

  Tags& tags = pElement->getTags();
  const WayPtr& pWay = boost::dynamic_pointer_cast<Way>(pElement);
  ElementConverter elementConverter(_map->shared_from_this());
  shared_ptr<Polygon> pPoly = elementConverter.convertToPolygon(pWay);

  if( _addLength || _addWidth )
  {
    // calculate polygon length and width
    double polyLength = 0;
    double polyWidth = 0;

    // calculate minimum rectangle/aligned bounding box
    Geometry* pMinRect = MinimumDiameter::getMinimumRectangle(pPoly.get());
    CoordinateSequence* pMinRectCoords = pMinRect->getCoordinates();

    if( pMinRectCoords->getSize() > 2 )
    {
      double len1 = (CoordinateExt(pMinRectCoords->getAt(0)) - CoordinateExt(pMinRectCoords->getAt(1))).length();
      double len2 = (CoordinateExt(pMinRectCoords->getAt(1)) - CoordinateExt(pMinRectCoords->getAt(2))).length();
      polyLength = std::max(len1,len2);
      polyWidth = std::min(len1,len2);
    }

    if( _addLength ) tags["length"] = polyLength;
    if( _addWidth ) tags["width"] = polyWidth;
  }

  if( _addArea && pWay->isClosedArea() )
  {
    // calculate poly area only if we need it
    double polyArea = pPoly->getArea();
    tags["area"] = polyArea;
  }
}

} // namespace hoot
