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
 * @copyright Copyright (C) 2020-2023 Maxar (http://www.maxar.com/)
 */

#include "ElementGeometryUtils.h"

// Hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/criterion/RelationWithLinearMembersCriterion.h>
#include <hoot/core/criterion/RelationWithPolygonMembersCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>

// GEOS
#include <geos/util/TopologyException.h>

using namespace geos::geom;

namespace hoot
{

int ElementGeometryUtils::_badGeomCount = 0;

bool ElementGeometryUtils::haveGeometricRelationship(const ConstElementPtr& element1, const ConstElementPtr& element2,
                                                     const GeometricRelationship& relationship, ConstOsmMapPtr map)
{
  if (!element1 || !element2)
    throw IllegalArgumentException("One of the input elements is null.");

  std::shared_ptr<Geometry> geom1 = _getGeometry(element1, map);
  std::shared_ptr<Geometry> geom2 = _getGeometry(element2, map);
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

std::shared_ptr<Geometry> ElementGeometryUtils::_getGeometry(const ConstElementPtr& element, ConstOsmMapPtr map)
{
  if (!element)
    throw IllegalArgumentException("The input element is null.");

  std::shared_ptr<Geometry> newGeom;
  QString errorMsg = "Feature passed to ElementGeometryUtils caused topology exception on conversion to a geometry: ";
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
  if (newGeom.get() && (newGeom->isEmpty() || QString::fromStdString(newGeom->toString()).toUpper().contains("EMPTY")))
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

Meters ElementGeometryUtils::calculateLength(const ConstElementPtr& e, const ConstElementProviderPtr& constProvider)
{
  // Doing length/distance calcs only make sense if we've projected down onto a flat surface
  if (!MapProjector::isPlanar(constProvider))
    throw IllegalArgumentException("Map must be in planar coordinate system.");
  // if the element is not a point and is not an area.
  // NOTE: Originally, we used isLinear. This was a bit too strict in that it wants evidence of
  // being linear before the length is calculated. Conversely, this wants evidence that it is not
  // linear before it will assume it doesn't have a length.
  if (e->getElementType() != ElementType::Node && AreaCriterion().isSatisfied(e) == false)
  {
    // TODO: optimize - we don't really need to convert first, we can just loop through the nodes
    // and sum up the distance.
    std::shared_ptr<Geometry> geom = ElementToGeometryConverter(constProvider).convertToGeometry(e);
    if (geom && geom->isValid())
      return geom->getLength();
  }
  return 0;
}

Coordinate ElementGeometryUtils::calculateElementCentroid(const ElementId& eid, const ConstOsmMapPtr& map)
{
  if (!map)
    return Coordinate::getNull();
  if (!map->containsElement(eid))
    return Coordinate::getNull();
  double centroid_x = 0.0;
  double centroid_y = 0.0;
  double centroid_count = 0.0;
  ConstElementPtr element = map->getElement(eid);
  switch(eid.getType().getEnum())
  {
  case ElementType::Node:
  {
    ConstNodePtr node = std::dynamic_pointer_cast<const Node>(element);
    if (!_getNodeCentroidValues(node, map, centroid_x, centroid_y, centroid_count))
      return Coordinate::getNull();
    break;
  }
  case ElementType::Way:
  {
    ConstWayPtr way = std::dynamic_pointer_cast<const Way>(element);
    if (!_getWayCentroidValues(way, map, centroid_x, centroid_y, centroid_count))
      return Coordinate::getNull();
    break;
  }
  case ElementType::Relation:
  {
    ConstRelationPtr r = std::dynamic_pointer_cast<const Relation>(element);
    if (!_getRelationCentroidValues(r, map, centroid_x, centroid_y, centroid_count))
      return Coordinate::getNull();
    break;
  }
  default:
    return Coordinate::getNull();
  }
  if (centroid_count < 1.0)
    return Coordinate::getNull();
  //  Divide by the node count to find centroid
  centroid_x /= centroid_count;
  centroid_y /= centroid_count;
  return Coordinate(centroid_x, centroid_y);
}

bool ElementGeometryUtils::_getNodeCentroidValues(const ConstNodePtr& node, const ConstOsmMapPtr& /*map*/, double& centroid_x, double& centroid_y, double& centroid_count)
{
  if (!node)
    return false;
  centroid_x += node->getX();
  centroid_y += node->getY();
  centroid_count++;
  return true;
}

bool ElementGeometryUtils::_getWayCentroidValues(const ConstWayPtr& way, const ConstOsmMapPtr& map, double& centroid_x, double& centroid_y, double& centroid_count)
{
  if (!way || !map)
    return false;
  if (way->getNodeCount() < 1)
    return false;
  double x = 0.0;
  double y = 0.0;
  const std::vector<long> node_ids = way->getNodeIds();
  size_t size = node_ids.size();
  //  Don't include the last ID if it is a closed way
  if (node_ids[0] == node_ids[size - 1])
    size--;
  //  Sum all way nodes
  for (size_t index = 0; index < size; ++index)
  {
    long node_id = node_ids[index];
    if (!map->containsNode(node_id))
      return false;
    ConstNodePtr node = map->getNode(node_id);
    x += node->getX();
    y += node->getY();
  }
  centroid_x += x;
  centroid_y += y;
  centroid_count += static_cast<double>(size);
  return true;
}

bool ElementGeometryUtils::_getRelationCentroidValues(const ConstRelationPtr& relation, const ConstOsmMapPtr& map, double& centroid_x, double& centroid_y, double& centroid_count)
{
  if (!relation || !map)
    return false;
  double x = 0.0;
  double y = 0.0;
  double count = 0.0;
  //  Sum all members
  for (const auto& member : relation->getMembers())
  {
    if (!map->containsElement(member.getElementId()))
      return false;
    ConstElementPtr element = map->getElement(member.getElementId());
    switch(element->getElementType().getEnum())
    {
    case ElementType::Node:
    {
      ConstNodePtr node = std::dynamic_pointer_cast<const Node>(element);
      if (!_getNodeCentroidValues(node, map, x, y, count))
        return false;
      break;
    }
    case ElementType::Way:
    {
      ConstWayPtr way = std::dynamic_pointer_cast<const Way>(element);
      if (!_getWayCentroidValues(way, map, x, y, count))
        return false;
      break;
    }
    case ElementType::Relation:
    {
      ConstRelationPtr r = std::dynamic_pointer_cast<const Relation>(element);
      if (!_getRelationCentroidValues(r, map, x, y, count))
        return false;
      break;
    }
    default:
      return false;
    }
  }
  centroid_x += x;
  centroid_y += y;
  centroid_count += count;
  return true;
}

}
