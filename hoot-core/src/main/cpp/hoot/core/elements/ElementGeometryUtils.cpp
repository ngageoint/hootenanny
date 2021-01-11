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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ElementGeometryUtils.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/criterion/RelationWithLinearMembersCriterion.h>
#include <hoot/core/criterion/RelationWithPointMembersCriterion.h>
#include <hoot/core/criterion/RelationWithPolygonMembersCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/criterion/AreaCriterion.h>

// GEOS
#include <geos/util/TopologyException.h>

namespace hoot
{

int ElementGeometryUtils::_badGeomCount = 0;

bool ElementGeometryUtils::haveGeometricRelationship(
  const ConstElementPtr& element1, const ConstElementPtr& element2,
  const GeometricRelationship& relationship, ConstOsmMapPtr map)
{
  if (!element1 || !element2)
  {
    throw IllegalArgumentException("One of the input elements is null.");
  }

  std::shared_ptr<geos::geom::Geometry> geom1 = _getGeometry(element1, map);
  std::shared_ptr<geos::geom::Geometry> geom2 = _getGeometry(element2, map);
  bool haveRelationship = false;
  if (geom1 && geom2)
  {
    switch (relationship.getEnum())
    {
      case GeometricRelationship::Contains:
        haveRelationship = geom1->contains(geom2.get());
        break;
      case GeometricRelationship::Covers:
        haveRelationship = geom1->covers(geom2.get());
        break;
      case GeometricRelationship::Crosses:
        haveRelationship = geom1->crosses(geom2.get());
        break;
      case GeometricRelationship::DisjointWith:
        haveRelationship = geom1->disjoint(geom2.get());
        break;
      case GeometricRelationship::Equals:
        haveRelationship = geom1->equals(geom2.get());
        break;
      case GeometricRelationship::Intersects:
        haveRelationship = geom1->intersects(geom2.get());
        break;
      case GeometricRelationship::IsWithin:
        haveRelationship = geom1->within(geom2.get());
        break;
      case GeometricRelationship::Overlaps:
        haveRelationship = geom1->overlaps(geom2.get());
        break;
      case GeometricRelationship::Touches:
        haveRelationship = geom1->touches(geom2.get());
        break;
      default:
        throw IllegalArgumentException("Unsupported geometry relationship type.");
    }
  }
  else
  {
    LOG_TRACE(
      "Unable to calculate geometric relationship: " << relationship.toString() << " for: " <<
      element1->getElementId() << " and: " << element2->getElementId() << ".");
  }
  return haveRelationship;
}

bool ElementGeometryUtils::haveGeometricRelationship(
  const ConstElementPtr& element, const std::shared_ptr<geos::geom::Geometry>& bounds,
  const GeometricRelationship& relationship, ConstOsmMapPtr map)
{
  if (!element)
  {
    throw IllegalArgumentException("The input element is null.");
  }
  if (!bounds)
  {
    throw IllegalArgumentException("The input bounds is null.");
  }
  if (!map)
  {
    throw IllegalArgumentException("The input map is null.");
  }
  LOG_VART(bounds->toString());
  LOG_VART(relationship.toString())

  std::shared_ptr<geos::geom::Geometry> elementGeom = _getGeometry(element, map);
  LOG_VART(elementGeom.get());
  bool haveRelationship = false;
  if (elementGeom)
  {
    LOG_VART(elementGeom->toString());
    switch (relationship.getEnum())
    {
      case GeometricRelationship::Contains:
        haveRelationship = bounds->contains(elementGeom.get());
        break;
      case GeometricRelationship::Covers:
        haveRelationship = bounds->covers(elementGeom.get());
        break;
      case GeometricRelationship::Crosses:
        haveRelationship = elementGeom->crosses(bounds.get());
        break;
      case GeometricRelationship::DisjointWith:
        haveRelationship = bounds->disjoint(elementGeom.get());
        break;
      case GeometricRelationship::Equals:
        haveRelationship = bounds->equals(elementGeom.get());
        break;
      case GeometricRelationship::Intersects:
        haveRelationship = elementGeom->intersects(bounds.get());
        break;
      case GeometricRelationship::IsWithin:
        haveRelationship = elementGeom->within(bounds.get());
        break;
      case GeometricRelationship::Overlaps:
        haveRelationship = elementGeom->overlaps(bounds.get());
        break;
      case GeometricRelationship::Touches:
        haveRelationship = elementGeom->touches(bounds.get());
        break;
      default:
        throw IllegalArgumentException("Unsupported geometry relationship type.");
    }
  }
  else
  {
    LOG_TRACE(
      "Unable to calculate geometric relationship: " << relationship.toString() << " for: " <<
      element->getElementId() << " and: " << QString::fromStdString(bounds->toString()).right(50) <<
      ".");
  }
  LOG_VART(haveRelationship);
  return haveRelationship;
}

bool ElementGeometryUtils::relationHasMemberWithGeometricRelationship(
  const ConstRelationPtr& relation, const std::shared_ptr<geos::geom::Geometry>& bounds,
  const GeometricRelationship& relationship, ConstOsmMapPtr map)
{
  const std::vector<RelationData::Entry>& relationMembers = relation->getMembers();
  for (size_t i = 0; i < relationMembers.size(); i++)
  {
    ConstElementPtr member = map->getElement(relationMembers[i].getElementId());
    if (member)
    {
      LOG_VART(member->getElementId());
      if (haveGeometricRelationship(member, bounds, relationship, map))
      {
        return true;
      }
    }
  }
  return false;
}

std::shared_ptr<geos::geom::Geometry> ElementGeometryUtils::_getGeometry(
  const ConstElementPtr& element, ConstOsmMapPtr map)
{
  if (!element)
  {
    throw IllegalArgumentException("The input element is null.");
  }

  std::shared_ptr<geos::geom::Geometry> newGeom;
  QString errorMsg =
    "Feature passed to OsmUtils caused topology exception on conversion to a geometry: ";
  try
  {
    newGeom = ElementToGeometryConverter(map).convertToGeometry(element);
  }
  catch (const geos::util::TopologyException& e)
  {
    // try to clean it
    newGeom.reset(GeometryUtils::validateGeometry(newGeom.get()));
    if (_badGeomCount <= Log::getWarnMessageLimit())
    {
      LOG_TRACE(errorMsg << element->toString() << "\n" << e.what());
      _badGeomCount++;
    }
  }
  catch (const HootException& e)
  {
    if (_badGeomCount <= Log::getWarnMessageLimit())
    {
      LOG_TRACE(errorMsg << element->toString() << "\n" << e.what());
      _badGeomCount++;
    }
  }
  if (newGeom.get() &&
      QString::fromStdString(newGeom->toString()).toUpper().contains("EMPTY"))
  {
    if (_badGeomCount <= Log::getWarnMessageLimit())
    {
      LOG_TRACE("Invalid element passed: " << newGeom->toString());
      _badGeomCount++;
    }
    newGeom.reset();
  }
  return newGeom;
}

GeometryTypeCriterion::GeometryType ElementGeometryUtils::geometryTypeForElement(
  const ConstElementPtr& element, ConstOsmMapPtr map)
{
  const ElementType type = element->getElementType();
  if (type == ElementType::Node)
  {
    return GeometryTypeCriterion::GeometryType::Point;
  }
  else if (type == ElementType::Way)
  {
    if (PolygonCriterion(map).isSatisfied(element))
    {
      return GeometryTypeCriterion::GeometryType::Polygon;
    }
    else
    {
      return GeometryTypeCriterion::GeometryType::Line;
    }
  }
  else if (type == ElementType::Relation)
  {
    if (!map)
    {
      throw IllegalArgumentException(
        "A map must be set when determining the geometry type of relations.");
    }
    // using the strict definition only here
    if (RelationWithLinearMembersCriterion(map).isSatisfied(element))
    {
      return GeometryTypeCriterion::GeometryType::Point;
    }
    else if (RelationWithPolygonMembersCriterion(map).isSatisfied(element))
    {
      return GeometryTypeCriterion::GeometryType::Polygon;
    }
    else if (RelationWithLinearMembersCriterion(map).isSatisfied(element))
    {
      return GeometryTypeCriterion::GeometryType::Line;
    }
    else
    {
      return GeometryTypeCriterion::GeometryType::Unknown;
    }
  }
  else
  {
    throw IllegalArgumentException("Invalid element type.");
  }
}

Meters ElementGeometryUtils::calculateLength(const ConstElementPtr& e,
                                             const ConstElementProviderPtr& constProvider)
{
  // Doing length/distance calcs only make sense if we've projected down onto a flat surface
  if (!MapProjector::isPlanar(constProvider))
  {
    throw IllegalArgumentException("Map must be in planar coordinate system.");
  }

  // if the element is not a point and is not an area.
  // NOTE: Originally, we used isLinear. This was a bit too strict in that it wants evidence of
  // being linear before the length is calculated. Conversely, this wants evidence that it is not
  // linear before it will assume it doesn't have a length.
  if (e->getElementType() != ElementType::Node && AreaCriterion().isSatisfied(e) == false)
  {
    // TODO: optimize - we don't really need to convert first, we can just loop through the nodes
    // and sum up the distance.
    return ElementToGeometryConverter(constProvider).convertToGeometry(e)->getLength();
  }
  else
  {
    return 0;
  }
}

}
