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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */

#ifndef OGR_MULTIFILE_WRITER_H
#define OGR_MULTIFILE_WRITER_H

// hoot
#include <hoot/core/io/OgrOptions.h>
#include <hoot/core/io/OsmMapWriter.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * OgrMultifileWriter writes an OsmMap to multiple files, one per layer, and is capable of custom handling of
 * columns. It is not capable of using translations. For that, use OgrWriter.
 */
class OgrMultifileWriter : public OsmMapWriter, public Configurable
{
public:

  OgrMultifileWriter();
  ~OgrMultifileWriter() override = default;

  /**
   * Set the configuration for this object.
   */
  void setConfiguration(const Settings& conf) override;

  void open(const QString& url) override;
  /**
   * Will write out up to four files:
   * path + "Points.<ext>"
   * path + "Lines.<ext>"
   * path + "Polygons.<ext>"
   * path + "MultiPolygons.<ext>" (for FlatGeobuf format only)
   */
  void write(const ConstOsmMapPtr& map) override;
  void write(const ConstOsmMapPtr& map, const QString& path);

  QStringList getColumns(ConstOsmMapPtr map, ElementType type) const;

  void setColumns(QStringList columns) { _baseColumns = columns; }

  void writeLines(const ConstOsmMapPtr& map, const QString& path);
  void writePoints(const ConstOsmMapPtr& map, const QString& path);
  void writePolygons(const ConstOsmMapPtr& map, const QString& path);

protected:

  QStringList _baseColumns;
  QStringList _usedColumns;
  QStringList _ogrColumns;
  bool _includeCircularError;
  QDir _outputDir;
  int _circularErrorIndex;
  GDALDriver* _driver;
  GDALDataset* _dataset;
  OGRLayer* _layer;

  virtual void _removeMultifile(const QString& path) const = 0;
  virtual const char* _getDriverName() const = 0;
  virtual QString _getFileExtension() const = 0;
  virtual OgrOptions _getOptions() const = 0;
  virtual bool _convertPolygons() const = 0;

  void _writeRelationPolygon(const ConstOsmMapPtr& map, const RelationPtr& relation) const;
  void _writeWayPolygon(const ConstOsmMapPtr& map, const WayPtr& way) const;

  void _setupDataset(const ConstOsmMapPtr& map, const QString& path, OGRwkbGeometryType geometry_type, ElementType element_type);
  OGRFeature* _createFeature(const Tags& tags, double circular_error) const;
  void _cleanupDataset();
  std::shared_ptr<geos::geom::Geometry> _polyToMultipoly(const std::shared_ptr<geos::geom::Geometry>& geometry) const;
};

}

#endif // OGR_MULTIFILE_WRITER_H
