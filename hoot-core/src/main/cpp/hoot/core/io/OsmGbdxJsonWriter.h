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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSM_GBDX_JSON_WRITER
#define OSM_GBDX_JSON_WRITER

// hoot
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QDir>
#include <QString>

namespace hoot
{

/**
 * Writes an OsmMap to JSON files with one feature per file.
 * This is to support ingest into GBDX Vector Services.
 */
class OsmGbdxJsonWriter : public OsmJsonWriter
{
public:

  static std::string className() { return "hoot::OsmGbdxJsonWriter"; }

  OsmGbdxJsonWriter(int precision = ConfigOptions().getWriterPrecision());
  virtual ~OsmGbdxJsonWriter() = default;

  /**
   * @brief Create a directory to hold all of the GeoJSON files
   * @param url
   */
  virtual void open(const QString& path) override;

  /**
   * @brief write Write the OsmMap out in GeoJSON format with one feature per file, writer must be "open"
   * @param map
   */
  virtual void write(const ConstOsmMapPtr& map) override;

  /**
   * @brief isSupported returns true if the URL is likely supported
   * @param url Filename ending in ".gbdx"
   * @return
   */
  virtual bool isSupported(const QString& url) override { return url.toLower().endsWith(".gbdx"); }

  virtual QString supportedFormats() override { return ".gdbx"; }

protected:

  QDir _outputDir;
  QString _outputFileName;
  int _fileNumber;

  /**
   * @brief Close the open file and open a unique file for output
   */
  void _newOutputFile();

  /**
   * @brief _writeNodes Iterates all nodes that aren't part of another element and writes
   *   them out individual GeoJSON files
   */
  virtual void _writeNodes();

  /**
   * @brief _writeNode Writes a single node; metadata, tags, and geometry
   * @param node
   */
  void _writeNode(ConstNodePtr node);

  /**
   * @brief _writeWays Iterates all ways that aren't part of another element and writes
   *   them out individual GeoJSON files
   */
  virtual void _writeWays();

  /**
   * @brief _writeWay Writes a single way; metadata, tags, and geometry
   * @param way
   */
  void _writeWay(ConstWayPtr way);

  /**
   * @brief _writeRelations Iterates all relations that aren't part of another element and writes
   *   them out individual GeoJSON files
   */
  virtual void _writeRelations();
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

private:

  /**
   * @brief _buildRoles Recursive version of _buildRoles, called my previous version
   * @param relation
   * @param first Flag used for semicolon separation
   * @return Semicolon separated list of roles
   */
  std::string _buildRoles(ConstRelationPtr relation, bool& first);

};

} // hoot

#endif // OSM_GBDX_JSON_WRITER
