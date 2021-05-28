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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef GEOMETRY_TO_ELEMENT_CONVERTER_H
#define GEOMETRY_TO_ELEMENT_CONVERTER_H

// GDAL
#include <ogr_geometry.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>

// Qt
#include <QString>

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
    class Point;
  }
}

namespace hoot
{

/**
 * Converts geometries between OSM and GEOS formats
 */
class GeometryToElementConverter
{
public:

  static QString className() { return "hoot::GeometryToElementConverter"; }

  GeometryToElementConverter(const OsmMapPtr& map);
  virtual ~GeometryToElementConverter() = default;

  class NodeFactory
  {
  public:

    NodeFactory() = default;
    virtual ~NodeFactory() = default;
    virtual NodePtr createNode(
      const OsmMapPtr& map, const geos::geom::Coordinate& c, Status s, double circularError) = 0;
  };

  /**
   * Creates a generic relation that will contain all the geometries in the geometry collection.
   * Each of the geometries will be added to the relation with an empty role. If there is only one
   * geometry in the collection then convertGeometryToElement will be called for that geometry.
   * If there are no geometries in the collection then a null point is returned.
   *
   * @param gc geometry collection containing geometries to add to returned element
   * @param s status of the returned element
   * @param circularError circular error of the returned element
   * @return an OSM element
   */
  std::shared_ptr<Element> convertGeometryCollection(
    const geos::geom::GeometryCollection* gc, Status s, double circularError);

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
   * @return an OSM element
   */
  std::shared_ptr<Element> convertGeometryToElement(
    const geos::geom::Geometry* g, Status s, double circularError);

  /**
   * Converts a GEOS line string to an OSM way
   *
   * @param ls line string to convert to OSM
   * @param map map to assign the returned element to
   * @param s status of the returned element
   * @param circularError circular error of the returned element
   * @return an OSM way
   */
  WayPtr convertLineStringToWay(
    const geos::geom::LineString* ls, const OsmMapPtr& map, Status s, double circularError) const;

  /**
   * If the MultiLineString contains multiple lines a multilinestring relation is returned. If the
   * multilinestring contains just one LineString a single Way is returned.
   *
   * @param mls multi-line string to convert to OSM
   * @param map map to assign the returned element to
   * @param s status of the returned element
   * @param circularError circular error of the returned element
   * @return an OSM element
   */
  std::shared_ptr<Element> convertMultiLineStringToElement(
    const geos::geom::MultiLineString* mls, const OsmMapPtr& map, Status s,
    double circularError) const;

  /**
   * Converts a GEOS multi-polygon to an OSM relation
   *
   * @param mp multi-polygon to convert to OSM
   * @param map map to assign the returned element to
   * @param s status of the returned element
   * @param circularError circular error of the returned element
   * @return an OSM relation
   */
  RelationPtr convertMultiPolygonToRelation(
    const geos::geom::MultiPolygon* mp, const OsmMapPtr& map, Status s, double circularError);

  /**
   * Converts the provided polygon into an element. If the polygon contains holes then a multi
   * polygon relation will be created. If the polygon doesn't contai
   *
   * @param polygon polygon to convert to OSM
   * @param map map to assign the returned element to
   * @param s status of the returned element
   * @param circularError circular error of the returned element
   * @return an OSM element
   */
  std::shared_ptr<Element> convertPolygonToElement(
    const geos::geom::Polygon* polygon, const OsmMapPtr& map, Status s, double circularError);

  /**
   * Converts a GEOS polygon to an OSM relation
   *
   * @param polygon polygon to convert to OSM
   * @param map map to assign the returned element to
   * @param s status of the returned element
   * @param circularError circular error of the returned element
   * @return an OSM relation
   */
  RelationPtr convertPolygonToRelation(
    const geos::geom::Polygon* polygon, const OsmMapPtr& map, Status s, double circularError) const;

  /**
   * Converts a GEOS polygon to an OSM relation
   *
   * @param polygon polygon to convert to OSM
   * @param map map to assign the returned element to
   * @param r TODO
   * @param s status of the returned element
   * @param circularError circular error of the returned element
   */
  void convertPolygonToRelation(
    const geos::geom::Polygon* polygon, const OsmMapPtr& map, const RelationPtr& r, Status s,
    double circularError) const;

  /**
   * Converts a GEOS point to an OSM node
   *
   * @param point point to convert to OSM
   * @param map map to assign the returned element to
   * @param s status of the returned element
   * @param circularError circular error of the returned element
   * @return an OSM node
   */
  NodePtr convertPointToNode(
    const geos::geom::Point* point, const OsmMapPtr& map, Status s, double circularError) const;

  void setNodeFactory(const std::shared_ptr<NodeFactory>& nf) { _nf = nf; }

protected:

  static int logWarnCount;

  NodePtr _createNode(const OsmMapPtr& map, const geos::geom::Coordinate& c, Status s,
    double circularError) const;

  std::shared_ptr<NodeFactory> _nf;
  ConstOsmMapPtr _constMap;
  OsmMapPtr _map;
};

}

#endif // GEOMETRY_TO_ELEMENT_CONVERTER_H
