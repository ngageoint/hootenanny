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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef OSM_GBDX_JSON_WRITER
#define OSM_GBDX_JSON_WRITER

// hoot
#include <hoot/core/io/OsmGeoJsonWriterBase.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

/**
 * Writes an OsmMap to JSON files with one feature per file.
 * This is to support ingest into GBDX Vector Services.
 */
class OsmGbdxJsonWriter : public OsmGeoJsonWriterBase
{
public:

  static QString className() { return "OsmGbdxJsonWriter"; }

  OsmGbdxJsonWriter(int precision = ConfigOptions().getWriterPrecision());
  ~OsmGbdxJsonWriter() override = default;

  /**
   * @brief Create a directory to hold all of the GeoJSON files
   * @param url
   */
  void open(const QString& path) override;

  /**
   * @brief write Write the OsmMap out in GeoJSON format with one feature per file, writer must be "open"
   * @param map
   */
  void write(const ConstOsmMapPtr& map) override;
  /**
   * @brief isSupported returns true if the URL is likely supported
   * @param url Filename ending in ".gbdx"
   * @return
   */
  bool isSupported(const QString& url) const override { return url.endsWith(".gbdx", Qt::CaseInsensitive); }

  QString supportedFormats() const override { return ".gdbx"; }

protected:

  /**
   * @brief _writeNodes Iterates all nodes that aren't part of another element and writes
   *   them out individual GeoJSON files
   */
  void _writeNodes() override;
  /**
   * @brief _writeWays Iterates all ways that aren't part of another element and writes
   *   them out individual GeoJSON files
   */
  void _writeWays() override;
  /**
   * @brief _writeRelations Iterates all relations that aren't part of another element and writes
   *   them out individual GeoJSON files
   */
  void _writeRelations() override;

private:

  QDir _outputDir;
  QString _outputFileName;
  int _fileNumber;

  /**
   * @brief Close the open file and open a unique file for output
   */
  void _newOutputFile();

};

}

#endif // OSM_GBDX_JSON_WRITER
