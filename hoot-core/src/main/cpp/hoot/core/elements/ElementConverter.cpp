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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ElementConverter.h"

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
#include <hoot/core/conflate/polygon/MultiPolygonCreator.h>
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/StatsAreaCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/CollectionRelationCriterion.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/NotImplementedException.h>
#include <hoot/core/visitors/MultiLineStringVisitor.h>
#include <hoot/core/util/GeometryUtils.h>

// Qt
#include <QString>
#include <QStringList>

// Standard
#include <cstdint>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int ElementConverter::logWarnCount = 0;

ElementConverter::ElementConverter(const ConstElementProviderPtr& provider) :
_constProvider(provider),
_spatialReference(provider->getProjection()),
_requireAreaForPolygonConversion(true)
{
}

Meters ElementConverter::calculateLength(const ConstElementPtr &e) const
{
  // Doing length/distance calcs only make sense if we've projected down onto a flat surface
  if (!MapProjector::isPlanar(_constProvider))
  {
    throw IllegalArgumentException("Map must be in planar coordinate system.");
  }

  // if the element is not a point and is not an area.
  // NOTE: Originally, I was using isLinear. This was a bit too strict in that it wants evidence of
  // being linear before the length is calculated. Conversely, this wants evidence that is is not
  // linear before it will assume it doesn't have a length.
  if (e->getElementType() != ElementType::Node && AreaCriterion().isSatisfied(e) == false)
  {
    // TODO: optimize - we don't really need to convert first, we can just loop through the nodes
    // and sum up the distance.
    return convertToGeometry(e)->getLength();
  }
  else
  {
    return 0;
  }
}

std::shared_ptr<Geometry> ElementConverter::convertToGeometry(
  const std::shared_ptr<const Element>& e, bool throwError, const bool statsFlag) const
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


std::shared_ptr<Point> ElementConverter::convertToGeometry(const ConstNodePtr& n) const
{
  return
    std::shared_ptr<Point>(GeometryFactory::getDefaultInstance()->createPoint(n->toCoordinate()));
}

std::shared_ptr<Geometry> ElementConverter::convertToGeometry(const WayPtr& w) const
{
  return convertToGeometry((ConstWayPtr)w);
}

std::shared_ptr<Geometry> ElementConverter::convertToGeometry(const ConstWayPtr& e,
                                                              bool throwError,
                                                              const bool statsFlag) const
{
  GeometryTypeId gid = getGeometryType(e, throwError, statsFlag, _requireAreaForPolygonConversion);
  LOG_VART(GeometryUtils::geometryTypeIdToString(gid));
  if (gid == GEOS_POLYGON)
  {
    return convertToPolygon(e);
  }
  else if (gid == GEOS_LINESTRING)
  {
    return convertToLineString(e);
  }
  else
  {
    // we don't recognize this geometry type.
    LOG_TRACE("Returning empty geometry...");
    std::shared_ptr<Geometry> g(GeometryFactory::getDefaultInstance()->createEmptyGeometry());
    return g;
  }
}

std::shared_ptr<Geometry> ElementConverter::convertToGeometry(const ConstRelationPtr& e,
                                                              bool throwError,
                                                              const bool statsFlag) const
{
  GeometryTypeId gid = getGeometryType(e, throwError, statsFlag, _requireAreaForPolygonConversion);
  LOG_VART(GeometryUtils::geometryTypeIdToString(gid));
  if (gid == GEOS_MULTIPOLYGON)
  {
    return MultiPolygonCreator(_constProvider, e).createMultipolygon();
  }
  else if (gid == GEOS_MULTILINESTRING)
  {
    MultiLineStringVisitor v;
    v.setElementProvider(_constProvider);
    e->visitRo(*_constProvider, v);
    std::shared_ptr<Geometry> result(v.createMultiLineString());
    return result;
  }
  else
  {
    // we don't recognize this geometry type.
    std::shared_ptr<Geometry> g(GeometryFactory::getDefaultInstance()->createEmptyGeometry());
    return g;
  }
}

std::shared_ptr<Geometry> ElementConverter::convertToGeometry(const RelationPtr& r) const
{
  return convertToGeometry((ConstRelationPtr)r);
}

std::shared_ptr<LineString> ElementConverter::convertToLineString(const ConstWayPtr& w) const
{
  const std::vector<long>& ids = w->getNodeIds();
  int size = ids.size();
  if (size == 1)
  {
    size = 2;
  }
  CoordinateSequence* cs =
    GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->create(size, 2);

  for (size_t i = 0; i < ids.size(); i++)
  {
    ConstNodePtr n = _constProvider->getNode(ids[i]);
    if (!n.get())
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Missing node: " << ids[i] << ". Not creating line string...");
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
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
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Missing node: " << ids[0] << ". Not creating line string...");
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
      return std::shared_ptr<LineString>();
    }
    cs->setAt(n->toCoordinate(), 1);
  }

  std::shared_ptr<LineString> result(GeometryFactory::getDefaultInstance()->createLineString(cs));
  return result;
}

std::shared_ptr<Polygon> ElementConverter::convertToPolygon(const ConstWayPtr& w) const
{
  const std::vector<long>& ids = w->getNodeIds();
  LOG_VART(ids);
  size_t size = ids.size();
  if (size == 1)
  {
    size = 2;
  }

  // if the first and last nodes aren't the same.
  if (ids.size() > 0 && ids[0] != ids[ids.size() - 1])
  {
    size++;
  }

  if (size <= 3)
  {
    return std::shared_ptr<Polygon>(GeometryFactory::getDefaultInstance()->createPolygon());
  }

  CoordinateSequence* cs =
    GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->create(size, 2);

  size_t i;
  for (i = 0; i < ids.size(); i++)
  {
    LOG_VART(ids[i]);
    ConstNodePtr n = _constProvider->getNode(ids[i]);
    //LOG_VART(n.get());
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
  }

  // if there are fewer than two points, or the last point does not equal the first
  while (i < size)
  {
    // add the first point onto the end.
    ConstNodePtr n = _constProvider->getNode(ids[0]);
    cs->setAt(n->toCoordinate(), i);
    i++;
  }

  // an empty set of holes
  vector<Geometry*>* holes = new vector<Geometry*>();
  // create the outer line
  LinearRing* outer = GeometryFactory::getDefaultInstance()->createLinearRing(cs);

  std::shared_ptr<Polygon> result(
    GeometryFactory::getDefaultInstance()->createPolygon(outer, holes));

  return result;
}

geos::geom::GeometryTypeId ElementConverter::getGeometryType(
  const ConstElementPtr& e, bool throwError, const bool statsFlag,
  const bool requireAreaForPolygonConversion)
{
  // This is used to pass the relation type back to the exception handler
  QString relationType = "";

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
      {
        areaCrit.reset(new StatsAreaCriterion());
      }
      else
      {
        areaCrit.reset(new AreaCriterion());
      }

      // Hootenanny by default requires that an polygon element be an area in the schema in order
      // to be converted to a polygon, it is created as a linestring. There are situations, however,
      // where we want to relax this requirement (generic geometry matching).
      if (!requireAreaForPolygonConversion && w->isValidPolygon() && w->isClosedArea())
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
        if (r->isMultiPolygon() || StatsAreaCriterion().isSatisfied(r))
          return GEOS_MULTIPOLYGON;
        else if (linearCrit.isSatisfied(r))
          return GEOS_MULTILINESTRING;
      }
      else
      {
        if (r->isMultiPolygon() || AreaCriterion().isSatisfied(r))
          return GEOS_MULTIPOLYGON;
        // Restriction relations are empty geometry
        else if (r->isRestriction())
          return GEOS_GEOMETRYCOLLECTION;
        // Need to find a better way of doing this.
        // If we have a review, send back a collection. This gets converted into an empty geometry.
        else if (r->isReview())
          return GEOS_GEOMETRYCOLLECTION;
        // MultiPoint comes from GeoJSON
        else if (r->getType() == MetadataTags::RelationMultiPoint())
          return GEOS_MULTIPOINT;
        //  Restrictions satisfy the linear criterion so test it after restrictions
        else if (linearCrit.isSatisfied(r))
          return GEOS_MULTILINESTRING;
        // an empty geometry, pass back a collection
        else if (r->getMembers().size() == 0 || CollectionRelationCriterion().isSatisfied(r))
          return GEOS_GEOMETRYCOLLECTION;
      }

      // We are going to throw an error so we save the type of relation
      relationType = r->getType();
      break;

    }

  default:
    LOG_ERROR("Element was not a node, way, or relation");
    break;
  }

  if (throwError)
  {
    if (relationType != "")
    {
      throw IllegalArgumentException("Unknown geometry type: Relation = " + relationType);
    }
    else
    {
      throw IllegalArgumentException("Unknown geometry type.");
    }
  }
  else
  {
    return GeometryTypeId(UNKNOWN_GEOMETRY);
  }
}

}
