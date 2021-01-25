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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef SHAPEFILEWRITER_H
#define SHAPEFILEWRITER_H

// hoot
#include <hoot/core/io/OsmMapWriter.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QDir>
#include <QString>
#include <QStringList>

// Standard
#include <vector>

namespace hoot
{

class OsmMap;
class Way;
class Relation;

/**
 * Writes an OsmMap to Shapefile file format.
 */
class ShapefileWriter : public OsmMapWriter, public Configurable
{
public:

  static QString className() { return "hoot::ShapefileWriter"; }

  ShapefileWriter();
  virtual ~ShapefileWriter() = default;

  virtual bool isSupported(const QString& url) override { return url.toLower().endsWith(".shp"); }

  virtual void open(const QString& url) override;

  QStringList getColumns(ConstOsmMapPtr map, ElementType type) const;

  void setColumns(QStringList columns) { _columns = columns; }

  /**
   * Will write out up to three files:
   * path + "Points.shp"
   * path + "Lines.shp"
   * path + "Polygons.shp"
   */
  virtual void write(const ConstOsmMapPtr& map) override;

  /**
   * @deprecated Use open and write instead.
   */
  void write(const ConstOsmMapPtr& map, const QString& path);

  void writeLines(const ConstOsmMapPtr& map, const QString& path);

  void writePoints(const ConstOsmMapPtr& map, const QString& path);

  void writePolygons(const ConstOsmMapPtr& map, const QString& path);

  virtual QString supportedFormats() override { return ".shp"; }

  /**
   * Set the configuration for this object.
   */
  virtual void setConfiguration(const Settings& conf) override;

protected:

  QStringList _columns;
  bool _includeCircularError;
  QDir _outputDir;
  int _circularErrorIndex;

  void _removeShapefile(const QString& path);

  void _writeRelationPolygon(const ConstOsmMapPtr& map, const RelationPtr& relation,
    OGRLayer* poLayer, const QStringList& columns, const QStringList& shpColumns);

  void _writeWayPolygon(const ConstOsmMapPtr& map, const WayPtr& way, OGRLayer *poLayer,
    const QStringList& columns, const QStringList &shpColumns);
};

} // hoot


#endif // SHAPEFILEWRITER_H
