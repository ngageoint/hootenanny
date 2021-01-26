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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef GEOMETRYUTILS_H
#define GEOMETRYUTILS_H

// GEOS
#include <geos/geom/Envelope.h>
#include <geos/geom/Polygon.h>

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
  }
}

// GDAL
#include <ogr_geometry.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>

// Qt
#include <QString>

namespace hoot
{

class GeometryUtils
{
public:

  static QString className() { return "hoot::GeometryUtils"; }

  static int logWarnCount;

  static double MEAN_EARTH_RADIUS;

  /**
   * Calculate the destination given the starting coordinate in lat/lng and the bearing in degrees.
   *
   * Taken from http://www.movable-type.co.uk/scripts/latlong.html
   */
  static geos::geom::Coordinate calculateDestination(
    const geos::geom::Coordinate& start, Degrees bearing, Meters distance);

  /**
   * Returns the distance between two coordinates using the haversine formula and a mean earth
   * radius of MEAN_EARTH_RADIUS.
   */
  static double haversine(const geos::geom::Coordinate& c1, const geos::geom::Coordinate& c2);
  static bool highPrecisionHaversine();

  static geos::geom::Envelope* toEnvelope(const OGREnvelope& e);

  /**
   * Converts the envelope into a hex representation. This preserves all double information but
   * is likely platform dependent.
   */
  static geos::geom::Envelope* toEnvelopeFromHex(const QString& s);

  static QString toHexString(const geos::geom::Envelope& e);

  static OGREnvelope* toOGREnvelope(const geos::geom::Envelope& e);

  /** Creates a bounds string in the format (minx,maxx,miny,maxy)
   *  from an envelope using the `writer.precision` value
   */
  static QString toString(const geos::geom::Envelope& e);

  /** Creates a bounds string in the format (minx,maxx,miny,maxy)
   *  from an envelope using the `writer.precision` value
   */
  static QString toConfigString(const geos::geom::Envelope& e);

  /**
   * Determines if a string represents an envelope
   *
   * @param str string to examine
   * @return true if the input represents an envelope; false otherwise
   */
  static bool isEnvelopeString(const QString& str);

  /**
   * Creates a bounds string in the format used in the hoot options config (minx,miny,maxx,maxy)
   * from an envelope
   *
   * @param boundsStr bounds string in the format used in the hoot options config to an envelope
   * @return an envelope string
   * @todo This should be replaced by toConfigString.
   */
  static QString envelopeToString(const geos::geom::Envelope& bounds);

  /**
   * Converts a bounds in the format used in the hoot options config (minx,miny,maxx,maxy) to an
   * envelope
   *
   * @param boundsStr bounds string in the format used in the hoot options config to an envelope
   * @return an envelope
   * @throws if the string is non-empty and an invalid envelope string
   */
  static geos::geom::Envelope envelopeFromString(const QString& boundsStr);

  /**
   * Determines if a string represents a polygon
   *
   * @param str string to examine
   * @return true if the input represents an polygon; false otherwise
   */
  static bool isPolygonString(const QString& str);

  /**
   * Converts an envelope to a polygon
   *
   * @param env the envelope to convert
   * @return a polygon
   */
  static std::shared_ptr<geos::geom::Polygon> envelopeToPolygon(const geos::geom::Envelope& env);

  /**
   * Parses a polygon from a string
   *
   * @param str the string to parse
   * @return a valid polygon or null if the input string is empty
   * @throws if the string is non-empty and an invalid polygon string
   */
  static std::shared_ptr<geos::geom::Polygon> polygonFromString(const QString& str);

  /**
   * Converts a polygon to a string representation
   *
   * @param poly the polygon to convert
   * @return a polygon string
   */
  static QString polygonToString(const std::shared_ptr<geos::geom::Polygon>& poly);

  /**
   * Converts either a envelope or polygon bounds string to a polygon
   *
   * Currently we're only support polygon here, but the bounds string format could be extended to
   * support other geometries if needed.
   *
   * @param str the string to parse
   * @return a valid polygon if the input string is a valid envelope or polygon string; a null
   * object otherwise
   */
  static std::shared_ptr<geos::geom::Polygon> boundsFromString(const QString& str);

  /**
   * Converts a polygon string to an envelope string
   *
   * @param str the string to convert
   * @return an envelope string
   */
  static QString polygonStringToEnvelopeString(const QString& str);

  static geos::geom::Geometry* validateGeometry(const geos::geom::Geometry* g);

  static geos::geom::Geometry* validateGeometryCollection(const geos::geom::GeometryCollection* gc);

  static geos::geom::Geometry* validateLineString(const geos::geom::LineString* ls);

  static geos::geom::LinearRing* validateLinearRing(const geos::geom::LinearRing* lr);

  /**
   * Performs the following cleanup operations:
   * - Removes polygons with an area of zero
   * - Removes linear rings less than 3 points
   */
  static geos::geom::Geometry* validatePolygon(const geos::geom::Polygon* p);

  /**
   * Creates a rectangular map representing a single bounding box
   *
   * @param bounds bounding box
   * @return a bounding box map
   */
  static OsmMapPtr createMapFromBounds(const geos::geom::Envelope& bounds);

  /**
   * Creates a map representing a single polygon bounds
   *
   * @param bounds bounds
   * @return a bounds map
   */
  static OsmMapPtr createMapFromBounds(const std::shared_ptr<geos::geom::Polygon>& bounds);

  /**
   * Creates a rectangular map representing multiple bounding boxes
   *
   * @param boundsCollection a collection of bounding boxes
   * @return a bounding box map
   */
  static OsmMapPtr createMapFromBoundsCollection(
    const QList<geos::geom::Envelope>& boundsCollection);

  /**
   * Creates a rectangular map representing multiple bounding boxes with IDs
   *
   * @param boundsCollection a collection of bounding boxes keyed by ID
   * @return a bounding box map
   */
  static OsmMapPtr createMapFromBoundsCollection(
    const QMap<int, geos::geom::Envelope>& boundsCollection);

  /**
   * Creates a bounding rectangle within the specified map
   *
   * @param map the map to insert the bounding rectangle in
   * @param bounds bounding box
   * @return ElementId of the way created
   */
  static ElementId createBoundsInMap(const OsmMapPtr& map, const geos::geom::Envelope& bounds);

  /**
   * Returns a string representation for a GEOS geometry type
   *
   * @param geometryTypeId the geometry type to convert
   * @return a string
   */
  static QString geometryTypeIdToString(const geos::geom::GeometryTypeId& geometryTypeId);

  /**
   * Returns a string representation for a GEOS geometry
   *
   * @param geometry the geometry to convert
   * @return a string
   */
  static QString geometryTypeIdToString(const std::shared_ptr<geos::geom::Geometry>& geometry);

  /**
   * Reads a file containing one or more rectangular AOIs
   *
   * @param input path to the bounds file
   * @return a collection of bounds rectangles
   * @throws IllegalArgumentException if the features in the input are not ways
   */
  static QList<geos::geom::Envelope> readBoundsFile(const QString& input);

  /**
   * Reads a file containing one or more rectangular AOIs where individual features have an
   * identifier
   *
   * @param input path to the bounds file; input must contain way features with an "id" field
   * @return a collection of bounds rectangles
   * @throws IllegalArgumentException if the features in the input are not ways containing an "id"
   * tag
   */
  static QMap<int, geos::geom::Envelope> readBoundsFileWithIds(const QString& input);

};

}

#endif // GEOMETRYUTILS_H
