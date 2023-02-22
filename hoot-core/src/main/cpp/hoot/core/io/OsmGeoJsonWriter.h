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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021, 2022, 2023 Maxar (http://www.maxar.com/)
 */
#ifndef OSM_GEOJSON_WRITER_H
#define OSM_GEOJSON_WRITER_H

// hoot
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/TranslationInterface.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

/**
 * Writes an OsmMap to a GeoJson file format.
 *
 * https://geojson.org/geojson-spec.html
 */
class OsmGeoJsonWriter : public OsmJsonWriter, public TranslationInterface, public Boundable
{
public:
  static QString className() { return "OsmGeoJsonWriter"; }

  OsmGeoJsonWriter(int precision = ConfigOptions().getWriterPrecision());
  ~OsmGeoJsonWriter() override = default;
  /**
   * @brief setConfiguration allows configuration settings to override the defaults
   * @param conf Configuration settings object
   */
  void setConfiguration(const Settings& conf) override;
  /**
   * @brief open Open the writer and get everything ready to write()
   * @param url URL to open
   */
  void open(const QString& url) override;
  /**
   * @brief write Write the OsmMap out to a file in GeoJSON format, writer must be "open"
   * @param map Map to write out to file(s)
   */
  void write(const ConstOsmMapPtr& map) override;
  /**
   * @brief isSupported checks if this URL is supported
   * @param url Filename ending in ".geojson"
   * @return true if the URL is likely supported
   */
  bool isSupported(const QString& url) const override { return url.endsWith(".geojson", Qt::CaseInsensitive); }
  /**
   * @brief supportedFormats List of supported file formats
   * @return Only .geojson is supported at this time
   */
  QString supportedFormats() const override { return ".geojson"; }

protected:
  /**
   * @brief _writeNodes Iterates all nodes that aren't part of another element and writes
   *   them out to the GeoJSON file
   */
  void _writeNodes() override;
  /**
   * @brief _writeWays Iterates all ways that aren't part of another element and writes
   *   them out to the GeoJSON file
   */
  void _writeWays() override;
  /**
   * @brief _writeRelations Iterates all relations that aren't part of another element and writes
   *   them out to the GeoJSON file
   */
  void _writeRelations() override;
  /**
   * @brief _writeMeta Write node/way/relation metadata, i.e. timestamp, version, and visible
   * @param element Element to write out
   */
  void _writeMeta(ConstElementPtr element);
  /**
   * @brief _getBbox Create a bounding box array in GeoJSON format
   * @return Bounding box string
   */
  QString _getBbox() const;
  /**
   * @brief _writeNode Writes a single element; metadata, tags, and geometry
   * @param element Element to write out
   * @param geometry Element geometry to write out
   */
  void _writeElement(ConstElementPtr element, ConstElementPtr translated_element, const std::shared_ptr<geos::geom::Geometry>& geometry);
  /**
   * @brief _writeRelationInfo Writes relation specific information, relation-type and roles
   * @param relation Relation to write out
   */
  void _writeRelationInfo(ConstRelationPtr relation);
  /**
   * @brief _writeFeature Calls _writeNode(), _writeWay(), or _writeRelation()
   *   based on the type of element e
   * @param element Element to write out
   */
  void _writeFeature(ConstElementPtr element);
  /**
   * @brief _writeGeometry Write out the geometry in GeoJSON format based on the type
   * @param nodes Coordinate sequence
   * @param type GeoJSON geometry type, i.e. Point, LineString, Polygon
   */
//  void _writeGeometry(const std::shared_ptr<geos::geom::CoordinateSequence>& nodes, std::string type);
  /**
   * @brief _writeGeometry Write out geometry for any element
   * @param element Element to write out
   */
  void _writeGeometry(ConstElementPtr element, const std::shared_ptr<geos::geom::Geometry>& geometry);
  /**
   * @brief _writeGeometry Write out geometry for a single node
   * @param node Node to write out
   */
  void _writeGeometry(ConstNodePtr node, const std::shared_ptr<geos::geom::Geometry>& geometry);
  /**
   * @brief _writeGeometry Write out geometry for a single way
   * @param way Way to write out
   */
  void _writeGeometry(ConstWayPtr way, const std::shared_ptr<geos::geom::Geometry>& geometry);
  /**
   * @brief _writeGeometry Write out geometry for a single relation
   * @param relation Relation to write out
   */
  void _writeGeometry(ConstRelationPtr relation, const std::shared_ptr<geos::geom::Geometry>& geometry);

  void _writeComplexGeometry(const std::shared_ptr<geos::geom::Geometry>& geometry, const QString& type, const QString& sub_type);

  void _writePointGeometry(const geos::geom::Geometry* geometry);
  void _writeLineStringGeometry(const geos::geom::Geometry* geometry);
  void _writePolygonGeometry(const geos::geom::Geometry* geometry);
  void _writeMultiLineStringGeometry(const geos::geom::Geometry* geometry);
  void _writeMultiPolygonGeometry(const geos::geom::Geometry* geometry);
  void _writeGeometryCollectionGeometry(const geos::geom::Geometry* geometry);
  void _writeCoordinateSequence(const std::shared_ptr<geos::geom::CoordinateSequence>& coordinates);
  void _writeCoordinate(const geos::geom::Coordinate& coordinate);
  void _writePolygonCoordinateSequence(const std::shared_ptr<geos::geom::CoordinateSequence>& coordinates);
  /**
   * @brief _buildRoles Iterates all members of relations (recurses super-relations) collecting roles
   * @param relation
   * @return Semicolon separated list of roles
   */
  std::string _buildRoles(ConstRelationPtr relation);
  /**
   * @brief _buildRoles Recursive version of _buildRoles, called my previous version
   * @param relation
   * @param first Flag used for semicolon separation
   * @return Semicolon separated list of roles
   */
  std::string _buildRoles(ConstRelationPtr relation, bool& first);
  /**
   * @brief _translateElement Translate the provided element, pushes at least one element onto the _translatedElementQueue map
   * @param e Element to be translated
   * @return Element converted to GEOS geometry
   */
  std::shared_ptr<geos::geom::Geometry> _translateElement(const ConstElementPtr& e);
  /**
   * @brief _getLayerName Get the layer name based on the translation or geometry type
   * @param feature Translated feature information (i.e. translated tags and layer name)
   * @param geometry Used in case of no translation information
   * @return Layer name appropriate for this element
   */
  QString _getLayerName(const ScriptToOgrSchemaTranslator::TranslatedFeature& feature,
                        const std::shared_ptr<geos::geom::Geometry>& geometry) const;
  QString _getLayerName(const std::shared_ptr<geos::geom::Geometry>& geometry) const;
  /**
   * @brief _getThematicUnknown Returns unknown layername based on geometry for thematic files
   * @param geometry Geometry to check type
   * @return Layername for unknown geometries (i.e. Unknown, UnknownPnt, UnknownCrv, UnknownSrf)
   */
  QString _getThematicUnknown(const std::shared_ptr<geos::geom::Geometry>& geometry) const;
  /**
   * @brief _getFcodeUnknown Returns unknown layername based on geometry for FCODE files
   * @param geometry Geometry to check type
   * @return Layername for unknown geometries (i.e. UNKNOWN, UNKNOWN_P, UNKNOWN_C, UNKNOWN_S)
   */
  QString _getFcodeUnknown(const std::shared_ptr<geos::geom::Geometry>& geometry) const;
  /**
   * @brief _cropGeometryToBounds Crops geometry to the _bounds if needed
   * @param geometry Geometry to crop
   * @return whole geometry if inside the bounds (or no crop is desired), cropped geometry if on the border, nullptr if outside of bounds
   */
  std::shared_ptr<geos::geom::Geometry> _cropGeometryToBounds(const std::shared_ptr<geos::geom::Geometry>& geometry) const;

  /** Tasking Manager requires that all bounding geometries are MultiPolygons and not Polygons or Linestrings,
   *  set to true when the GeoJSON output is being used for Tasking Manager bounding polygons.
   */
  bool _useTaskingManagerFormat;
  /** Flag set to true when the output is into different layers (thematic or FCODE) */
  bool _useThematicLayers;
  /** Map of translated features with the layer name as the key and the value is a translated element */
  std::map<QString, ConstElementPtr> _translatedElementMap;
  /** Crop any features that cross the `bounds` of the operation */
  bool _cropFeaturesCrossingBounds;
};

}

#endif // OSM_GEOJSON_WRITER_H
