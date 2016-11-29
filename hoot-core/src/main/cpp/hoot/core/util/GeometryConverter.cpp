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

#include "GeometryConverter.h"

// GEOS
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>

// hoot
#include <hoot/core/schema/OsmSchema.h>
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

shared_ptr<Element> GeometryConverter::convertGeometryCollection(const GeometryCollection* gc,
  Status s, double circularError)
{
  if (gc->getNumGeometries() > 1)
  {
    LOG_INFO("Creating relation. convertGeometryCollection");
    shared_ptr<Relation> r(new Relation(s, _map->createNextRelationId(), circularError));
    int count = gc->getNumGeometries();

    for (int i = 0; i < count; i++)
    {
      const Geometry* g = gc->getGeometryN(i);
      r->addElement("", convertGeometryToElement(g, s, circularError));
    }

    return r;
  }
  else if (gc->getNumGeometries() == 1)
  {
    return convertGeometryToElement(gc->getGeometryN(0), s, circularError);
  }
  else
  {
    return shared_ptr<Element>();
  }
}

shared_ptr<Element> GeometryConverter::convertGeometryToElement(const Geometry* g, Status s,
  double circularError)
{
  switch (g->getGeometryTypeId())
  {
  case GEOS_LINESTRING:
  case GEOS_LINEARRING:
    return convertLineStringToWay(dynamic_cast<const LineString*>(g), _map, s, circularError);
  case GEOS_POLYGON:
    return convertPolygonToElement(dynamic_cast<const Polygon*>(g), _map, s, circularError);
  case GEOS_MULTILINESTRING:
    return convertMultiLineStringToElement(dynamic_cast<const MultiLineString*>(g), _map, s,
      circularError);
  case GEOS_MULTIPOLYGON:
    return convertMultiPolygonToRelation(dynamic_cast<const MultiPolygon*>(g), _map, s,
      circularError);
  case GEOS_GEOMETRYCOLLECTION:
    return convertGeometryCollection(dynamic_cast<const GeometryCollection*>(g), s,
      circularError);
  default:\
    _logCount++;
    int logLimit = ConfigOptions().getOgrLogLimit();
    if (_logCount <= logLimit)
    {
      LOG_WARN("Unsupported geometry type. Element will be removed from the map. " + g->toString());
    }
    else
    {
      LOG_WARN("GeometryConverter::convertGeometryToElement reached maximum number of log. No longer logging.");
    }
    return shared_ptr<Element>();
  }
}

shared_ptr<Way> GeometryConverter::convertLineStringToWay(const LineString* ls,
  const shared_ptr<OsmMap>& map, Status s, double circularError)
{
  shared_ptr<Way> way;
  if (ls->getNumPoints() > 0)
  {
    Coordinate c = ls->getCoordinateN(0);
    way.reset(new Way(s, map->createNextWayId(), circularError));

    for (size_t i = 0; i < ls->getNumPoints(); i++)
    {
      c = ls->getCoordinateN(i);
      way->addNode(_createNode(map, c, s, circularError)->getId());
    }
    map->addWay(way);
  }

  return way;
}

shared_ptr<Element> GeometryConverter::convertMultiLineStringToElement(const MultiLineString* mls,
  const shared_ptr<OsmMap>& map, Status s, double circularError)
{
  if (mls->getNumGeometries() > 1)
  {
    shared_ptr<Relation> r(new Relation(s, map->createNextRelationId(), circularError,
      Relation::MULTILINESTRING));
    for (size_t i = 0; i < mls->getNumGeometries(); i++)
    {
      shared_ptr<Way> w = convertLineStringToWay(
        dynamic_cast<const LineString*>(mls->getGeometryN(i)), map, s, circularError);
      r->addElement("", w);
    }
    map->addRelation(r);
    return r;
  }
  else
  {
    return convertLineStringToWay(dynamic_cast<const LineString*>(mls->getGeometryN(0)),
      map, s, circularError);
  }
}

shared_ptr<Relation> GeometryConverter::convertMultiPolygonToRelation(const MultiPolygon* mp,
  const shared_ptr<OsmMap>& map, Status s, double circularError)
{
  shared_ptr<Relation> r(new Relation(s, map->createNextRelationId(), circularError,
    Relation::MULTIPOLYGON));
  for (size_t i = 0; i < mp->getNumGeometries(); i++)
  {
    convertPolygonToRelation(dynamic_cast<const Polygon*>(mp->getGeometryN(i)), map, r, s,
      circularError);
  }
  map->addRelation(r);
  return r;
}

shared_ptr<Element> GeometryConverter::convertPolygonToElement(const Polygon* polygon,
  const shared_ptr<OsmMap>& map, Status s, double circularError)
{
  // if the geometry is empty.
  if (polygon->isEmpty())
  {
    return shared_ptr<Element>();
  }
  else if (polygon->getNumInteriorRing() == 0)
  {
    shared_ptr<Way> result = convertLineStringToWay(polygon->getExteriorRing(), map, s, circularError);
    result->getTags()["area"] = "yes";
    return result;
  }
  else
  {
    return convertPolygonToRelation(polygon, map, s, circularError);
  }
}

shared_ptr<Relation> GeometryConverter::convertPolygonToRelation(const Polygon* polygon,
  const shared_ptr<OsmMap>& map, Status s, double circularError)
{
  shared_ptr<Relation> r(new Relation(s, map->createNextRelationId(), circularError,
    Relation::MULTIPOLYGON));
  convertPolygonToRelation(polygon, map, r, s, circularError);
  map->addRelation(r);

  return r;
}

void GeometryConverter::convertPolygonToRelation(const Polygon* polygon,
  const shared_ptr<OsmMap>& map, const shared_ptr<Relation>& r, Status s, double circularError)
{
  shared_ptr<Way> outer = convertLineStringToWay(polygon->getExteriorRing(), map, s, circularError);
  if (outer != NULL)
  {
    r->addElement(Relation::OUTER, outer);
    for (size_t i = 0; i < polygon->getNumInteriorRing(); i++)
    {
      shared_ptr<Way> inner = convertLineStringToWay(polygon->getInteriorRingN(i), map, s, circularError);
      r->addElement(Relation::INNER, inner);
    }
  }
}

shared_ptr<Node> GeometryConverter::_createNode(const shared_ptr<OsmMap>& map, const Coordinate& c,
  Status s, double circularError)
{
  if (_nf == 0)
  {
    shared_ptr<Node> n = shared_ptr<Node>(new Node(s, map->createNextNodeId(), c, circularError));
    map->addNode(n);
    return n;
  }
  else
  {
    return _nf->createNode(map, c, s, circularError);
  }
}

}
