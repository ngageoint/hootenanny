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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef OSM_GEOJSON_WRITER_H
#define OSM_GEOJSON_WRITER_H

// hoot
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/TranslationInterface.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

/**
 * Writes an OsmMap to a GeoJson file format.
 *
 * https://geojson.org/geojson-spec.html
 */
class OsmGeoJsonWriter : public OsmJsonWriter, public TranslationInterface
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

  void open(const QString& url) override;
  /**
   * @brief write Write the OsmMap out to a file in GeoJSON format, writer must be "open"
   * @param map
   */
  void write(const ConstOsmMapPtr& map) override;
  /**
   * @brief isSupported returns true if the URL is likely supported
   * @param url Filename ending in ".geojson"
   * @return
   */
  bool isSupported(const QString& url) const override { return url.endsWith(".geojson", Qt::CaseInsensitive); }
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
   * @param element
   */
  void _writeMeta(ConstElementPtr element);
  /**
   * @brief _getBbox Create a bounding box array in GeoJSON format
   * @return
   */
  QString _getBbox() const;
  /**
   * @brief _writeNode Writes a single node; metadata, tags, and geometry
   * @param node
   */
  void _writeNode(ConstNodePtr node);
  /**
   * @brief _writeWay Writes a single way; metadata, tags, and geometry
   * @param way
   */
  void _writeWay(ConstWayPtr way);
  /**
   * @brief _writeRelationInfo Writes relation specific information, relation-type and roles
   * @param relation
   */
  void _writeRelationInfo(ConstRelationPtr relation);

  /**
   * @brief _writeFeature Calls _writeNode(), _writeWay(), or _writeRelation()
   *   based on the type of element e
   * @param element
   */
  void _writeFeature(ConstElementPtr element);
  /**
   * @brief _writeGeometry Write out the geometry in GeoJSON format based on the type
   * @param nodes Vector of node ids in OsmMap
   * @param type GeoJSON geometry type, i.e. Point, LineString, Polygon
   */
  void _writeGeometry(const std::vector<long>& nodes, std::string type);
  /**
   * @brief _writeGeometry Write out geometry for any element
   * @param element
   */
  void _writeGeometry(ConstElementPtr element);
  /**
   * @brief _writeGeometry Write out geometry for a single node
   * @param node
   */
  void _writeGeometry(ConstNodePtr node);
  /**
   * @brief _writeGeometry Write out geometry for a single way
   * @param way
   */
  void _writeGeometry(ConstWayPtr way);
  /**
   * @brief _writeGeometry Write out geometry for a single relation
   * @param relation
   */
  void _writeGeometry(ConstRelationPtr relation);
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
   * @brief _setWriterIndex Set the writer index for MultiFile objects
   * @param e Element dictating the index
   */
  void _setWriterIndex(const ConstElementPtr& e) override;

  QString _getLayerName(const std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>& feature,
                        const std::shared_ptr<geos::geom::Geometry>& geometry) const;
  QString _getThematicUnknown(const std::shared_ptr<geos::geom::Geometry>& geometry) const;
  QString _getFcodeUnknown(const std::shared_ptr<geos::geom::Geometry>& geometry) const;


  /** Tasking Manager requires that all bounding geometries are MultiPolygons and not Polygons or Linestrings,
   *  set to true when the GeoJSON output is being used for Tasking Manager bounding polygons.
   */
  bool _useTaskingManagerFormat;

  /**
   * @brief _useThematicLayers
   */
  bool _useThematicLayers;

};

}

#endif // OSM_GEOJSON_WRITER_H
