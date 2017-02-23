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

#ifndef ELEMENTCONVERTER_H
#define ELEMENTCONVERTER_H

#include <ogr_spatialref.h>

#include <boost/shared_ptr.hpp>

// GEOS
#include <geos/geom/Envelope.h>
#include <geos/geom/Geometry.h>

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
    class Point;
    class Polygon;
  }
}

// GDAL
#include <ogr_geometry.h>

// Hoot
#include <hoot/core/elements/ElementProvider.h>
#include <hoot/core/OsmMap.h>

// Qt
#include <QString>

namespace hoot
{
using namespace geos::geom;

/**
 * Converts elements to geometries
 */
class ElementConverter
{
public:

  /**
   * see class description
   *
   * @note If the element provider passed as a parameter is an OsmMap, the spatial reference
   *      from the parameter will be set as the spatial reference for the element converter
   * @note the default spatial reference used is WGS84
   */
  ElementConverter(const ConstElementProviderPtr& provider);

  /**
   * Calculate the length of the given way in meters. The projection must be planar.
   */
  Meters calculateLength(const ConstElementPtr& e) const;

  /**
   * Converts the given element to a geos geometry object. The tags are used with OsmSchema to
   * determine the geometry type.
   */
  shared_ptr<geos::geom::Geometry> convertToGeometry(const shared_ptr<const Element>& e,
                                                     bool throwError=true,
                                                     const bool statsFlag=false) const;
  shared_ptr<geos::geom::Point> convertToGeometry(const ConstNodePtr& n) const;
  shared_ptr<geos::geom::Geometry> convertToGeometry(const WayPtr& w) const;
  shared_ptr<geos::geom::Geometry> convertToGeometry(const shared_ptr<const Way>& w, bool throwError,
                                                     const bool statsFlag=false) const;
  shared_ptr<geos::geom::Geometry> convertToGeometry(const shared_ptr<const Relation>& r,
                                                     bool throwError,
                                                     const bool statsFlag=false) const;
  shared_ptr<geos::geom::Geometry> convertToGeometry(const shared_ptr<Relation>& r) const;
  shared_ptr<geos::geom::LineString> convertToLineString(const ConstWayPtr& w) const;
  shared_ptr<geos::geom::Polygon> convertToPolygon(const ConstWayPtr& w) const;

  /**
   * Return the geometry type of the specific element.
   * @param e
   * @param throwError If true an exception is thrown with an invalid geometry. If false a -1 is
   *  returned on error.
   */
  static geos::geom::GeometryTypeId getGeometryType(const ConstElementPtr& e,
    bool throwError=true, const bool statsFlag=false);

  static const int UNKNOWN_GEOMETRY = -1;

protected:
  ConstElementProviderPtr                 _constProvider;
  boost::shared_ptr<OGRSpatialReference>  _spatialReference;
};

}

#endif // ELEMENTCONVERTER_H
