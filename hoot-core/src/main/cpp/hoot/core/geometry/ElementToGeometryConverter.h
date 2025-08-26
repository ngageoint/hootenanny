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

#ifndef ELEMENT_TO_GEOMETRY_CONVERTER_H
#define ELEMENT_TO_GEOMETRY_CONVERTER_H

// GEOS
#include <geos/geom/Geometry.h>

// GDAL
#include <ogr_geometry.h>

// Hoot
#include <hoot/core/elements/ElementProvider.h>
#include <hoot/core/elements/OsmMap.h>

namespace geos
{
  namespace geom
  {
    class LineString;
    class Polygon;
  }
}

namespace hoot
{

/**
 * Converts elements to geometries
 */
class ElementToGeometryConverter
{
public:

  static int logWarnCount;

  static QString className() { return "ElementToGeometryConverter"; }

  /**
   * Constructor
   *
   * @note If the element provider passed as a parameter is an OsmMap, the spatial reference
   *      from the parameter will be set as the spatial reference for the element converter
   * @note the default spatial reference used is WGS84
   */
  ElementToGeometryConverter(const ConstElementProviderPtr& provider, const bool logWarningsForMissingElements = true);
  ~ElementToGeometryConverter() = default;

  /**
   * Converts the given element to a geos geometry object. The tags are used with the schema to
   * determine the geometry type.
   *
   * @param e the element to convert
   * @param throwError If true, an exception is thrown when encountering an invalid element. If
   * false, an empty geometry is returned when encountering an invalid element.
   * @param statsFlag If true, this geometry type is being retrieved for the purpose of map
   * statistics.
   * @return a geometry
   */
  std::shared_ptr<geos::geom::Geometry> convertToGeometry(const std::shared_ptr<const Element>& e, bool throwError = true,
                                                          const bool statsFlag = false) const;

  std::shared_ptr<geos::geom::Point> convertToGeometry(const ConstNodePtr& n) const;
  std::shared_ptr<geos::geom::Geometry> convertToGeometry(const WayPtr& w) const;
  std::shared_ptr<geos::geom::Geometry> convertToGeometry(const ConstWayPtr& w, bool throwError, const bool statsFlag = false) const;
  std::shared_ptr<geos::geom::Geometry> convertToGeometry(const ConstRelationPtr& r, bool throwError, const bool statsFlag = false) const;
  std::shared_ptr<geos::geom::Geometry> convertToGeometry(const RelationPtr& r) const;
  std::shared_ptr<geos::geom::LineString> convertToLineString(const ConstWayPtr& w) const;
  std::shared_ptr<geos::geom::Polygon> convertToPolygon(const ConstWayPtr& w) const;

  /**
   * Return the geometry type of the specific element.
   *
   * @param e the element to retrieve the geometry type of
   * @param throwError If true, an exception is thrown when encountering an invalid element. If
   * false, a an unknown geometry ID is returned when encountering an invalid element.
   * @param statsFlag If true, this geometry type is being retrieved for the purpose of map
   * statistics.
   * @return a geometry ID
   */
  static geos::geom::GeometryTypeId getGeometryType(const ConstElementPtr& e, bool throwError = false, const bool statsFlag = false);

  void setRequireAreaForPolygonConversion(bool require)
  { _requireAreaForPolygonConversion = require; }

  static const int UNKNOWN_GEOMETRY = -1;

private:

  ConstElementProviderPtr _constProvider;
  std::shared_ptr<OGRSpatialReference>  _spatialReference;

  // if true, conversions to polys require that the source element be classifiable in the schema as
  // an area
  bool _requireAreaForPolygonConversion;

  // determines whether missing elements trigger a warning
  bool _logWarningsForMissingElements;
};

}

#endif // ELEMENT_TO_GEOMETRY_CONVERTER_H
