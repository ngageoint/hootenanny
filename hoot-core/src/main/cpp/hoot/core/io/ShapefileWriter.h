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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef SHAPEFILEWRITER_H
#define SHAPEFILEWRITER_H

// hoot
#include <hoot/core/io/OsmMapWriter.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * ShapefileWriter writes an OsmMap to Shapefile file format, and is capable of custom handling of
 * columns. It is not capable of using translations. For that, use OgrWriter.
 */
class ShapefileWriter : public OsmMapWriter, public Configurable
{
public:

  static QString className() { return "hoot::ShapefileWriter"; }

  ShapefileWriter();
  ~ShapefileWriter() = default;

  /**
   * Set the configuration for this object.
   */
  void setConfiguration(const Settings& conf) override;

  bool isSupported(const QString& url) override { return url.toLower().endsWith(".shp"); }
  void open(const QString& url) override;
  /**
   * Will write out up to three files:
   * path + "Points.shp"
   * path + "Lines.shp"
   * path + "Polygons.shp"
   */
  void write(const ConstOsmMapPtr& map) override;
  QString supportedFormats() override { return ".shp"; }

  /**
   * @deprecated Use open and write instead.
   */
  void write(const ConstOsmMapPtr& map, const QString& path);
  void writeLines(const ConstOsmMapPtr& map, const QString& path);
  void writePoints(const ConstOsmMapPtr& map, const QString& path);
  void writePolygons(const ConstOsmMapPtr& map, const QString& path);

  QStringList getColumns(ConstOsmMapPtr map, ElementType type) const;

  void setColumns(QStringList columns) { _columns = columns; }

private:

  QStringList _columns;
  bool _includeCircularError;
  QDir _outputDir;
  int _circularErrorIndex;

  void _removeShapefile(const QString& path) const;

  void _writeRelationPolygon(const ConstOsmMapPtr& map, const RelationPtr& relation,
    OGRLayer* poLayer, const QStringList& columns, const QStringList& shpColumns) const;
  void _writeWayPolygon(const ConstOsmMapPtr& map, const WayPtr& way, OGRLayer *poLayer,
    const QStringList& columns, const QStringList &shpColumns) const;
};

}


#endif // SHAPEFILEWRITER_H
