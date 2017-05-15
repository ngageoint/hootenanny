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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef GEOMETRYUTILS_H
#define GEOMETRYUTILS_H

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

class GeometryUtils
{
public:

  static std::string className() { return "hoot::GeometryUtils"; }

  static unsigned int logWarnCount;

  static double MEAN_EARTH_RADIUS;

  /**
   * Calculate the destination given the starting coordinate in lat/lng and the bearing in degrees.
   *
   * Taken from http://www.movable-type.co.uk/scripts/latlong.html
   */
  static geos::geom::Coordinate calculateDestination(const geos::geom::Coordinate& start, Degrees bearing, Meters distance);

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

  static QString toString(const geos::geom::Envelope& e);

  static geos::geom::Geometry* validateGeometry(const geos::geom::Geometry *g);

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
   * Converts a bounds in the format used in the hoot options config (minx,miny,maxx,maxy) to an
   * envelope
   *
   * @param boundsStr bounds string in the format used in the hoot options config to an envelope
   * @return
   */
  static geos::geom::Envelope envelopeFromConfigString(const QString boundsStr);

  /**
   * Creates a bounds string in the format used in the hoot options config (minx,miny,maxx,maxy)
   * from an envelope
   *
   * @param boundsStr bounds string in the format used in the hoot options config to an envelope
   * @return
   */
  static QString envelopeToConfigString(const geos::geom::Envelope& bounds);

};

}

#endif // GEOMETRYUTILS_H
