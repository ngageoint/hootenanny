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
 * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 */
#ifndef OSM_GEOJSON_WRITER_BASE_H
#define OSM_GEOJSON_WRITER_BASE_H

// hoot
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

/**
 * Non-complete GeoJSON class for shared code
 */
class OsmGeoJsonWriterBase : public OsmJsonWriter
{
public:

  OsmGeoJsonWriterBase(int precision = ConfigOptions().getWriterPrecision(),
                       bool taskingManagerFormat = ConfigOptions().getJsonOutputTaskingManagerAoi());
  virtual ~OsmGeoJsonWriterBase() = default;

  /**
   * @brief setConfiguration allows configuration settings to override the defaults
   * @param conf Configuration settings object
   */
  void setConfiguration(const Settings& conf) override;

protected:

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

  bool _useTaskingManagerFormat;
};

}

#endif // OSM_GEOJSON_WRITER_BASE_H
