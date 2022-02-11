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
#include <hoot/core/io/OsmGeoJsonWriterBase.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

/**
 * Writes an OsmMap to a GeoJson file format.
 *
 * https://geojson.org/geojson-spec.html
 */
class OsmGeoJsonWriter : public OsmGeoJsonWriterBase
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

  QString toString(const ConstOsmMapPtr& map);

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

};

}

#endif // OSM_GEOJSON_WRITER_H
