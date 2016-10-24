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

#ifndef GEOMETRYCONVERTER_H
#define GEOMETRYCONVERTER_H

// GEOS
#include <geos/geom/Envelope.h>

namespace geos
{
  namespace geom
  {
    class Geometry;
    class GeometryCollection;
    class LinearRing;
    class LineString;
    class MultiLineString;
    class MultiPolygon;
    class Polygon;
  }
}

// GDAL
#include <ogr_geometry.h>

// Hoot
#include <hoot/core/OsmMap.h>

// Qt
#include <QString>

namespace hoot
{
using namespace geos::geom;

/**
 * GeometryConverter is undergoing a transition. We've moving from using the element's pointers to
 * the OsmMap to an internal pointer. Ultimately this will fix some circular errors (see #4120).
 * For new code please use the constructor that takes a map.
 * @todo old code should be transitioned eventually.
 */
class GeometryConverter
{
public:

  /**
   * see class description
   */
  GeometryConverter() { _logCount = 0; }

  /**
   * see class description
   */
  GeometryConverter(const OsmMapPtr& map) : _constMap(map), _map(map) { _logCount = 0; assert(map.get()); }

  /**
   * see class description
   */
  GeometryConverter(const ConstOsmMapPtr& map) : _constMap(map) { assert(map.get()); }

  class NodeFactory
  {
  public:
    virtual shared_ptr<Node> createNode(const shared_ptr<OsmMap>& map, const Coordinate& c,
      Status s, double circularError) = 0;
  };

  /**
   * Creates a generic relation that will contain all the geometries in the geometry collection.
   * Each of the geometries will be added to the relation with an empty role. If there is only one
   * geometry in the collection then convertGeometryToElement will be called for that geometry.
   * If there are no geometries in the collection then a null point is returned.
   */
  shared_ptr<Element> convertGeometryCollection(const GeometryCollection* gc,
    Status s, double circularError);

  /**
   * Calls the appropriate convert* method based on the geometry passed in and adds the resulting
   * elements to the provided OsmMap. The root object will be returned. For instance, converting
   * a linestring will simply return a pointer to the Way. Converting a multi linestring will
   * return a relation that points to a series of ways. All created elements will be accessible
   * from the returned element.
   *
   * If the geometry is empty then a null pointer will be returned and nothing will be added to
   * the map.
   *
   * @param g Geometry to convert. Not all geometries are supported. See the source for details.
   * @param status The status to assign to the newly created elements.
   * @param circularError The circular error to assign to the newly created elements.
   */
  shared_ptr<Element> convertGeometryToElement(const Geometry* g, Status s, double circularError);

  shared_ptr<Way> convertLineStringToWay(const LineString* ls, const shared_ptr<OsmMap>& map,
    Status s, double circularError);

  /**
   * If the MultiLineString contains multiple lines a multilinestring relation is returned. If the
   * multilinestring contains just one LineString a single Way is returned.
   */
  shared_ptr<Element> convertMultiLineStringToElement(const MultiLineString* mls,
    const shared_ptr<OsmMap>& map, Status s, double circularError);

  shared_ptr<Relation> convertMultiPolygonToRelation(const MultiPolygon* mp,
    const shared_ptr<OsmMap>& map, Status s, double circularError);

  /**
   * Converts the provided polygon into an element. If the polygon contains holes then a multi
   * polygon relation will be created. If the polygon doesn't contain holes then a closed way will
   * be created and the area=yes tag will be set.
   */
  shared_ptr<Element> convertPolygonToElement(const Polygon* polygon,
    const shared_ptr<OsmMap>& map, Status s, double circularError);

  shared_ptr<Relation> convertPolygonToRelation(const Polygon* polygon,
    const shared_ptr<OsmMap>& map, Status s, double circularError);

  void convertPolygonToRelation(const Polygon* polygon,
    const shared_ptr<OsmMap>& map, const shared_ptr<Relation>& r, Status s, double circularError);

  void convertPolygonToWays(const Polygon* polygon, const shared_ptr<OsmMap>& map,
    Status s, double circularError);

  void setNodeFactory(shared_ptr<NodeFactory> nf) { _nf = nf; }

protected:
  shared_ptr<Node> _createNode(const shared_ptr<OsmMap>& map, const Coordinate& c, Status s,
    double circularError);

  shared_ptr<NodeFactory> _nf;
  ConstOsmMapPtr _constMap;
  OsmMapPtr _map;
  int _logCount;
};

}

#endif // GEOMETRYCONVERTER_H
