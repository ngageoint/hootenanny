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
 * @copyright Copyright (C) 2015-2025 Maxar (http://www.maxar.com/)
 */

#include "ElementToGeometryConverter.h"

// GDAL
#include <ogr_spatialref.h>

// GEOS
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>

// hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/StatsAreaCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/RelationCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/geometry/RelationToMultiPolygonConverter.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/visitors/MultiLineStringVisitor.h>

// Standard
#include <cstdint>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int ElementToGeometryConverter::logWarnCount = 0;

ElementToGeometryConverter::ElementToGeometryConverter(const ConstElementProviderPtr& provider,
                                                       const bool logWarningsForMissingElements)
  : _constProvider(provider),
    _spatialReference(provider->getProjection()),
    _requireAreaForPolygonConversion(true),
    _logWarningsForMissingElements(logWarningsForMissingElements)
{
}

std::shared_ptr<Geometry> ElementToGeometryConverter::convertToGeometry(const std::shared_ptr<const Element>& e,
                                                                        bool throwError, const bool statsFlag) const
{
  LOG_VART(e->getElementId());
  switch (e->getElementType().getEnum())
  {
  case ElementType::Node:
    return convertToGeometry(std::dynamic_pointer_cast<const Node>(e));
  case ElementType::Way:
    return convertToGeometry(std::dynamic_pointer_cast<const Way>(e), throwError, statsFlag);
  case ElementType::Relation:
    return convertToGeometry(std::dynamic_pointer_cast<const Relation>(e), throwError, statsFlag);
  default:
    LOG_VART(e->toString());
    throw HootException("Unexpected element type: " + e->getElementType().toString());
  }
}

std::shared_ptr<Point> ElementToGeometryConverter::convertToGeometry(const ConstNodePtr& n) const
{
  return std::shared_ptr<Point>(GeometryFactory::getDefaultInstance()->createPoint(n->toCoordinate()));
}

std::shared_ptr<Geometry> ElementToGeometryConverter::convertToGeometry(const WayPtr& w) const
{
  return convertToGeometry((ConstWayPtr)w);
}

std::shared_ptr<Geometry> ElementToGeometryConverter::convertToGeometry(const ConstWayPtr& e, bool throwError,
                                                                        const bool statsFlag) const
{
  GeometryTypeId gid = getGeometryType(e, throwError, statsFlag);
  LOG_VART(GeometryUtils::geometryTypeIdToString(gid));
  if (gid == GEOS_POLYGON)
    return convertToPolygon(e);
  else if (gid == GEOS_LINESTRING)
    return convertToLineString(e);
  else  // We don't recognize this geometry type
    return std::shared_ptr<Geometry>(GeometryFactory::getDefaultInstance()->createEmptyGeometry());
}

std::shared_ptr<Geometry> ElementToGeometryConverter::convertToGeometry(const ConstRelationPtr& e, bool throwError,
                                                                        const bool statsFlag) const
{
  GeometryTypeId gid = getGeometryType(e, throwError, statsFlag);
  LOG_VART(GeometryUtils::geometryTypeIdToString(gid));
  if (gid == GEOS_MULTIPOLYGON)
    return RelationToMultiPolygonConverter(_constProvider, e).createMultipolygon();
  else if (gid == GEOS_MULTILINESTRING)
  {
    MultiLineStringVisitor v;
    v.setElementProvider(_constProvider);
    e->visitRo(*_constProvider, v);
    return v.createGeometry();
  }
  else if (gid == GEOS_GEOMETRYCOLLECTION && (e->isReview() || e->isRestriction()))
  {
    //  Convert the reviewees to geometries and return the collection
    vector<const Geometry*> geometries;
    //  Cache the shared pointers to manage memory because createGeometryCollection does a deep copy
    vector<std::shared_ptr<Geometry>> cache;
    for (const auto& member : e->getMembers())
    {
      if (_constProvider->containsElement(member.getElementId()))
      {
        std::shared_ptr<Geometry> element = convertToGeometry(_constProvider->getElement(member.getElementId()));
        cache.push_back(element);
        geometries.push_back(element.get());
      }
    }
    return std::shared_ptr<Geometry>(GeometryFactory::getDefaultInstance()->createGeometryCollection(geometries));
  }
  else  // We don't recognize this geometry type.
    return std::shared_ptr<Geometry>(GeometryFactory::getDefaultInstance()->createEmptyGeometry());
}

std::shared_ptr<Geometry> ElementToGeometryConverter::convertToGeometry(const RelationPtr& r) const
{
  return convertToGeometry((ConstRelationPtr)r);
}

std::shared_ptr<LineString> ElementToGeometryConverter::convertToLineString(const ConstWayPtr& w) const
{
  LOG_TRACE("Converting " << w->getElementId() << " to line string...");

  const std::vector<long>& ids = w->getNodeIds();
  size_t size = ids.size();
  if (size == 1)
    size = 2;

  CoordinateSequence* cs = GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->create(size, 2).release();

  for (size_t i = 0; i < ids.size(); i++)
  {
    long node_id = ids[i];
    ConstNodePtr n = _constProvider->getNode(node_id);
    if (!n.get())
    {
      if (_logWarningsForMissingElements)
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Missing node: " << node_id << ". Not creating line string...");
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
      LOG_TRACE("Missing node: " << node_id << ". Not creating line string...");
      return std::shared_ptr<LineString>();
    }
    cs->setAt(n->toCoordinate(), i);
  }

  // a linestring cannot contain 1 point. Do this to keep it valid.
  if (ids.size() == 1)
  {
    ConstNodePtr n = _constProvider->getNode(ids[0]);
    if (!n.get())
    {
      if (_logWarningsForMissingElements)
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Missing node: " << ids[0] << ". Not creating line string...");
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
      LOG_TRACE("Missing node: " << ids[0] << ". Not creating line string...");
      return std::shared_ptr<LineString>();
    }
    cs->setAt(n->toCoordinate(), 1);
  }

  std::shared_ptr<LineString> result(GeometryFactory::getDefaultInstance()->createLineString(cs));
  return result;
}

std::shared_ptr<Polygon> ElementToGeometryConverter::convertToPolygon(const ConstWayPtr& w) const
{
  const std::vector<long>& ids = w->getNodeIds();
  LOG_VART(ids);
  size_t size = ids.size();
  if (size == 1)
    size = 2;

  // if the first and last nodes aren't the same.
  if (!ids.empty() && ids[0] != ids[ids.size() - 1])
    size++;

  if (size <= 3)
    return std::shared_ptr<Polygon>(GeometryFactory::getDefaultInstance()->createPolygon());

  CoordinateSequence* cs = GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->create(size, 2).release();

  size_t i;
  for (i = 0; i < ids.size(); i++)
  {
    long node_id = ids[i];
    LOG_VART(node_id);
    ConstNodePtr n = _constProvider->getNode(node_id);
    if (!n.get())
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN(
          "Node " << QString::number(node_id) << " does not exist. Skipping conversion of " <<
          w->getElementId() << " to polygon...");
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
      return std::shared_ptr<Polygon>();
    }
    cs->setAt(n->toCoordinate(), i);
  }

  // if there are fewer than two points, or the last point does not equal the first
  while (i < size)
  {
    // add the first point onto the end.
    ConstNodePtr n = _constProvider->getNode(ids[0]);
    if (!n.get())
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN(
          "Node " << QString::number(ids[i]) << " does not exist. Skipping conversion of " <<
          w->getElementId() << " to polygon...");
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
      return std::shared_ptr<Polygon>();
    }
    cs->setAt(n->toCoordinate(), i);
    i++;
  }

  // an empty set of holes
  vector<LinearRing*>* holes = new vector<LinearRing*>();
  // create the outer line; GeometryFactory takes ownership of these input parameters.
  LinearRing* outer = GeometryFactory::getDefaultInstance()->createLinearRing(cs);

  std::shared_ptr<Polygon> result(GeometryFactory::getDefaultInstance()->createPolygon(outer, holes));

  return result;
}

geos::geom::GeometryTypeId ElementToGeometryConverter::getGeometryType(const ConstElementPtr& e, bool throwError,
                                                                       const bool statsFlag)
{
  ElementType t = e->getElementType();

  switch (t.getEnum())
  {
  case ElementType::Node:
    return GEOS_POINT;
  case ElementType::Way:
  {
    ConstWayPtr w = std::dynamic_pointer_cast<const Way>(e);
    assert(w);

    LOG_VART(statsFlag);
    LOG_VART(w->isValidPolygon());
    LOG_VART(w->isClosedArea());
    LOG_VART(AreaCriterion().isSatisfied(w));
    LOG_VART(OsmSchema::getInstance().allowsFor(e, OsmGeometries::Area));

    ElementCriterionPtr areaCrit;
    if (statsFlag)
      areaCrit = std::make_shared<StatsAreaCriterion>();
    else
      areaCrit = std::make_shared<AreaCriterion>();

    // Hootenanny by default requires that an polygon element be an area in the schema in order
    // to be converted to a polygon, it is created as a linestring. There are situations, however,
    // where we want to relax this requirement (generic geometry matching).
    if (w->isValidPolygon() && w->isClosedArea())
      return GEOS_POLYGON;
    else if (w->isValidPolygon() && areaCrit->isSatisfied(w))
      return GEOS_POLYGON;
    else if (w->isClosedArea() && OsmSchema::getInstance().allowsFor(e, OsmGeometries::Area))
      return GEOS_POLYGON;
    else
      return GEOS_LINESTRING;

    break;
  }
  case ElementType::Relation:
  {
    ConstRelationPtr r = std::dynamic_pointer_cast<const Relation>(e);
    assert(r);

    LinearCriterion linearCrit;

    if (statsFlag)
    {
      if (r->isMultiPolygon() || r->getType() == MetadataTags::RelationSite() || StatsAreaCriterion().isSatisfied(r))
        return GEOS_MULTIPOLYGON;
      else if (linearCrit.isSatisfied(r))
        return GEOS_MULTILINESTRING;
    }
    else
    {
      if (r->isMultiPolygon() ||
          // Relation type=site was added to fix BadMatchPairTest crashing in Polygon.js.
          r->getType() == MetadataTags::RelationSite() ||
          AreaCriterion().isSatisfied(r))
        return GEOS_MULTIPOLYGON;
      // Restriction relations and review relations are geometry collections
      else if (r->isRestriction() || r->isReview())
        return GEOS_GEOMETRYCOLLECTION;
      // MultiPoint comes from GeoJSON
      else if (r->getType() == MetadataTags::RelationMultiPoint())
        return GEOS_MULTIPOINT;
      //  Restrictions satisfy the linear criterion so test it after restrictions
      else if (linearCrit.isSatisfied(r))
        return GEOS_MULTILINESTRING;
      // an empty geometry, pass back a collection
      else if (r->getMembers().empty() || RelationCriterion().isSatisfied(r))
        return GEOS_GEOMETRYCOLLECTION;
    }

    QString errorMsg;
    // We are going to throw an error so we save the type of relation
    QString relationType = r->getType().trimmed();
    if (relationType != "")
      errorMsg = QString("Unknown geometry type for %1 with type=%2").arg(e->getElementId().toString(), relationType);
    else
      errorMsg = QString("Unknown geometry type for %1 with missing type.").arg(e->getElementId().toString());

    if (throwError)
      throw IllegalArgumentException(errorMsg);
    else
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN(errorMsg);
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    break;
  }
  default:
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Element was not a node, way, or relation");
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
    break;
  }

  return GeometryTypeId(UNKNOWN_GEOMETRY);
}

}
