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

// Hoot
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/elements/RelationMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/ElementToRelationMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/geometry/CoordinateExt.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/MapProjector.h>

// Boost
#include <boost/shared_ptr.hpp>

// Geos
#include <geos/algorithm/MinimumDiameter.h>
#include <geos/geom/LineString.h>
#include <geos/geom/GeometryFactory.h>

using namespace geos::geom;
using namespace geos::algorithm;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, AddMeasurementTagsVisitor)

void AddMeasurementTagsVisitor::visit(const ElementPtr& pElement)
{
  if (!_addArea && !_addWidth && !_addLength) return;

  _numProcessed++;

  if (!_map->getProjection()->IsProjected())
  {
    MapProjector::projectToPlanar(_map->shared_from_this());
  }

  if (pElement->getElementType() == ElementType::Relation)
  {
    const RelationPtr& pRelation = std::dynamic_pointer_cast<Relation>(pElement);
    processRelation(pRelation);
    _numAffected++;
  }

  if (pElement->getElementType() == ElementType::Way)
  {
    const WayPtr& pWay = std::dynamic_pointer_cast<Way>(pElement);
    processWay(pWay);
    _numAffected++;
  }
}

void AddMeasurementTagsVisitor::processRelation(const RelationPtr pRelation)
{
  // for length/width combine all way member polygons
  std::shared_ptr<Geometry> pCombined = std::shared_ptr<Polygon>(GeometryFactory::getDefaultInstance()->createPolygon());
  ElementToGeometryConverter ElementToGeometryConverter(_map->shared_from_this());

  // for area add all outer role polys and subtract all inner role polys
  double totalArea = 0;

  foreach (RelationData::Entry entry, pRelation->getMembers())
  {
    ElementPtr pMember =_map->getElement(entry.getElementId());
    if (pMember->getElementType() == ElementType::Way)
    {
      const WayPtr& pWay = std::dynamic_pointer_cast<Way>(pMember);
      std::shared_ptr<Polygon> pPoly = ElementToGeometryConverter.convertToPolygon(pWay);

      // build a combined polygon for extents
      if (_addLength || _addWidth)
      {
        pCombined = std::shared_ptr<Geometry>(pCombined->Union(pPoly.get()));
      }

      // gather all valid areas and their roles to calculate total area
      if (_addArea && pWay->isClosedArea())
      {
        double area = pPoly->getArea();

        if (entry.getRole() == MetadataTags::RelationOuter())
        {
          totalArea += area;
        }

        if (entry.getRole() == MetadataTags::RelationInner())
        {
          totalArea -= area;
        }
      }
    }
  }

  // calculate length and width of combined polygon
  double polyLength = 0;
  double polyWidth = 0;

  calculateExtents(pCombined.get(), polyLength, polyWidth);

  // write to tags
  Tags& tags = pRelation->getTags();
  if (_addLength) tags["length"] = QString::number(polyLength);
  if (_addWidth) tags["width"] = QString::number(polyWidth);
  if (_addArea) tags["area"] = QString::number(totalArea);
}

void AddMeasurementTagsVisitor::processWay(const WayPtr pWay)
{
  Tags& tags = pWay->getTags();

  ElementToGeometryConverter ElementToGeometryConverter(_map->shared_from_this());
  std::shared_ptr<Polygon> pPoly = ElementToGeometryConverter.convertToPolygon(pWay);

  if (_addLength || _addWidth)
  {
    // calculate polygon length and width
    double polyLength = 0;
    double polyWidth = 0;

    if (pPoly->getNumPoints() == 0)
    {
      std::shared_ptr<LineString> pLine = ElementToGeometryConverter.convertToLineString(pWay);
      polyLength = pLine->getLength();
    }
    else
    {
      calculateExtents(pPoly.get(), polyLength, polyWidth);
    }

    if (_addLength) tags["length"] = QString::number(polyLength);
    if (_addWidth) tags["width"] = QString::number(polyWidth);
  }

  if (_addArea && pWay->isClosedArea())
  {
    // calculate poly area only if we need it
    double polyArea = pPoly->getArea();
    tags["area"] = QString::number(polyArea);
  }
}

void AddMeasurementTagsVisitor::calculateExtents(Geometry* pGeometry, double& length, double &width)
{
  // calculate polygon length and width
  length = 0;
  width = 0;

  // calculate minimum rectangle/aligned bounding box
  Geometry* pMinRect = MinimumDiameter::getMinimumRectangle(pGeometry);
  CoordinateSequence* pMinRectCoords = pMinRect->getCoordinates();

  if (pMinRectCoords->getSize() > 2)
  {
    double len1 = (CoordinateExt(pMinRectCoords->getAt(0)) - CoordinateExt(pMinRectCoords->getAt(1))).length();
    double len2 = (CoordinateExt(pMinRectCoords->getAt(1)) - CoordinateExt(pMinRectCoords->getAt(2))).length();
    length = std::max(len1,len2);
    width = std::min(len1,len2);
  }
}

} // namespace hoot
