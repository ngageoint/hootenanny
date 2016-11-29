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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include <ogr_spatialref.h>
#include <boost/shared_ptr.hpp>

#include "ElementConverter.h"

// GEOS
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/NotImplementedException.h>
#include <hoot/core/visitors/MultiLineStringVisitor.h>

// Qt
#include <QString>
#include <QStringList>

// Standard
#include <stdint.h>

#include "MultiPolygonCreator.h"

namespace hoot
{

ElementConverter::ElementConverter(const ConstElementProviderPtr& provider) :
  _constProvider(provider),
  _spatialReference(provider->getProjection())
{

}

Meters ElementConverter::calculateLength(const ConstElementPtr &e) const
{
  // Doing length/distance calcs only make sense if we've projected down onto a flat surface
  assert(MapProjector::isPlanar(_constProvider));

  // if the element is not a point and is not an area.
  // NOTE: Originally I was using isLinear. This was a bit too strict in that it wants evidence of
  // being linear before the length is calculated. Conversely, this wants evidence that is is not
  // linear before it will assume it doesn't have a length.
  if (e->getElementType() != ElementType::Node &&
      OsmSchema::getInstance().isArea(e) == false)
  {
    /// @optimize
    // we don't really need to convert first, we can just loop through the nodes and sum up the
    // distance.
    return convertToGeometry(e)->getLength();
  }
  else
  {
    return 0;
  }
}

shared_ptr<Geometry> ElementConverter::convertToGeometry(const shared_ptr<const Element>& e,
                                                         bool throwError,
                                                         const bool statsFlag) const
{
  switch(e->getElementType().getEnum())
  {
  case ElementType::Node:
    return convertToGeometry(dynamic_pointer_cast<const Node>(e));
  case ElementType::Way:
    return convertToGeometry(dynamic_pointer_cast<const Way>(e), throwError, statsFlag);
  case ElementType::Relation:
    return convertToGeometry(dynamic_pointer_cast<const Relation>(e), throwError, statsFlag);
  default:
    LOG_WARN(e->toString());
    throw HootException("Unexpected element type: " + e->getElementType().toString());
  }
}


shared_ptr<Point> ElementConverter::convertToGeometry(const shared_ptr<const Node>& n) const
{
  return shared_ptr<Point>(GeometryFactory::getDefaultInstance()->createPoint(n->toCoordinate()));
}

shared_ptr<Geometry> ElementConverter::convertToGeometry(const WayPtr& w) const
{
  return convertToGeometry((ConstWayPtr)w);
}

shared_ptr<Geometry> ElementConverter::convertToGeometry(const shared_ptr<const Way>& e, bool throwError, const bool statsFlag) const
{
  GeometryTypeId gid = getGeometryType(e, throwError, statsFlag);
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
    shared_ptr<Geometry> g(GeometryFactory::getDefaultInstance()->createEmptyGeometry());
    return g;
  }
}

shared_ptr<Geometry> ElementConverter::convertToGeometry(const shared_ptr<const Relation>& e, bool throwError, const bool statsFlag) const
{
  GeometryTypeId gid = getGeometryType(e, throwError, statsFlag);

  if (gid == GEOS_MULTIPOLYGON)
  {
    return MultiPolygonCreator(_constProvider, e).createMultipolygon();
  }
  else if (gid == GEOS_MULTILINESTRING)
  {
    MultiLineStringVisitor v;
    v.setElementProvider(_constProvider);
    e->visitRo(*_constProvider, v);
    shared_ptr<Geometry> result(v.createMultiLineString());
    return result;
  }
  else
  {
    // we don't recognize this geometry type.
    shared_ptr<Geometry> g(GeometryFactory::getDefaultInstance()->createEmptyGeometry());
    return g;
  }
}

shared_ptr<Geometry> ElementConverter::convertToGeometry(const shared_ptr<Relation>& r) const
{
  return convertToGeometry((ConstRelationPtr)r);
}

shared_ptr<LineString> ElementConverter::convertToLineString(const ConstWayPtr& w) const
{
  const std::vector<long>& ids = w->getNodeIds();
  int size = ids.size();
  if (size == 1)
  {
    size = 2;
  }
  CoordinateSequence* cs = GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->
                           create(size, 2);

  for (size_t i = 0; i < ids.size(); i++)
  {
    shared_ptr<const Node> n = _constProvider->getNode(ids[i]);
    cs->setAt(n->toCoordinate(), i);
  }

  // a linestring cannot contain 1 point. Do this to keep it valid.
  if (ids.size() == 1)
  {
    shared_ptr<const Node> n = _constProvider->getNode(ids[0]);
    cs->setAt(n->toCoordinate(), 1);
  }

  shared_ptr<LineString> result(GeometryFactory::getDefaultInstance()->createLineString(cs));

  return result;
}

shared_ptr<Polygon> ElementConverter::convertToPolygon(const ConstWayPtr& w) const
{
  const std::vector<long>& ids = w->getNodeIds();
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
    return shared_ptr<Polygon>(GeometryFactory::getDefaultInstance()->createPolygon());
  }

  CoordinateSequence* cs = GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->
                           create(size, 2);

  size_t i;
  for (i = 0; i < ids.size(); i++)
  {
    shared_ptr<const Node> n = _constProvider->getNode(ids[i]);
    cs->setAt(n->toCoordinate(), i);
  }

  // if there are fewer than two points, or the last point does not equal the first
  while (i < size)
  {
    // add the first point onto the end.
    shared_ptr<const Node> n = _constProvider->getNode(ids[0]);
    cs->setAt(n->toCoordinate(), i);
    i++;
  }

  // an empty set of holes
  vector<Geometry*>* holes = new vector<Geometry*>();
  // create the outer line
  LinearRing* outer = GeometryFactory::getDefaultInstance()->createLinearRing(cs);

  shared_ptr<Polygon> result(GeometryFactory::getDefaultInstance()->createPolygon(outer, holes));

  return result;
}

geos::geom::GeometryTypeId ElementConverter::getGeometryType(const ConstElementPtr& e,
  bool throwError, const bool statsFlag)
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
      ConstWayPtr w = dynamic_pointer_cast<const Way>(e);
      assert(w);

      if(statsFlag)
      {
        if (w->isValidPolygon() && OsmSchema::getInstance().isAreaForStats(w->getTags(), ElementType::Way))
          return GEOS_POLYGON;
        else
          return GEOS_LINESTRING;
      } else {
        if (w->isValidPolygon() && OsmSchema::getInstance().isArea(w->getTags(), ElementType::Way))
          return GEOS_POLYGON;
        else
          return GEOS_LINESTRING;
      }

      break;
    }

  case ElementType::Relation:
    {
      ConstRelationPtr r = dynamic_pointer_cast<const Relation>(e);
      assert(r);

      if(statsFlag)
      {
        if (r->isMultiPolygon() || OsmSchema::getInstance().isAreaForStats(r->getTags(), ElementType::Relation))
          return GEOS_MULTIPOLYGON;
        else if (OsmSchema::getInstance().isLinear(*r))
          return GEOS_MULTILINESTRING;
      } else {
        if (r->isMultiPolygon() || OsmSchema::getInstance().isArea(r->getTags(), ElementType::Relation))
          return GEOS_MULTIPOLYGON;
        else if (OsmSchema::getInstance().isLinear(*r)) {
          return GEOS_MULTILINESTRING;
        }
        else if (r->getMembers().size() == 0 ||
                 OsmSchema::getInstance().isCollection(*r)) {
          // an empty geometry, pass back a collection
          return GEOS_GEOMETRYCOLLECTION;
        }
        // Need to find a better way of doing this.
        // If we have a review, send back a collection. This gets converted into an empty geometry.
        else if (r->isReview()) {
          return GEOS_GEOMETRYCOLLECTION;
        }
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
