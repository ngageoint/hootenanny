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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef SHAPEFILEWRITER_H
#define SHAPEFILEWRITER_H

// hoot
#include <hoot/core/io/OsmMapWriter.h>

// Qt
#include <QDir>
#include <QHash>
#include <QString>
#include <QStringList>
#include <QXmlDefaultHandler>
class QString;

// Standard
#include <vector>

namespace hoot
{

class OsmMap;
class Way;
class Relation;

/**
 * Reads in a .osm file into an OsmMap data structure. During this process all IDs are mapped from
 * the .osm node/way ID to a new ID.
 */
class ShapefileWriter : public OsmMapWriter
{
public:

  static std::string className() { return "hoot::ShapefileWriter"; }

  ShapefileWriter() { _includeInfo = true; _includeIds = false; _circularErrorIndex = -1; }

  virtual bool isSupported(QString url) { return url.toLower().endsWith(".shp"); }

  virtual void open(QString url);

  QStringList getColumns(ConstOsmMapPtr map, ElementType type) const;

  void setColumns(QStringList columns) { _columns = columns; }

  void setIncludeIds(bool includeIds) { _includeIds = includeIds; }

  void setIncludeHootInfo(bool includeInfo) { _includeInfo = includeInfo; }

  /**
   * Will write out up to three files:
   * path + "Polygon.shp"
   * path + "Line.shp"
   */
  void write(boost::shared_ptr<const OsmMap> map);

  /**
   * @deprecated Use open and write instead.
   */
  void write(boost::shared_ptr<const OsmMap> map, QString path);

  void writeLines(boost::shared_ptr<const OsmMap> map, const QString& path);

  void writePoints(boost::shared_ptr<const OsmMap> map, const QString& path);

  void writePolygons(boost::shared_ptr<const OsmMap> map, const QString& path);

protected:
  QStringList _columns;
  bool _includeIds;
  bool _includeInfo;
  QDir _outputDir;
  int _circularErrorIndex;

  void _removeShapefile(QString path);

  void _writeRelationPolygon(const ConstOsmMapPtr& map, const boost::shared_ptr<Relation> &relation,
    OGRLayer *poLayer, const QStringList &columns, const QStringList &shpColumns);

  void _writeWayPolygon(const ConstOsmMapPtr& map, const boost::shared_ptr<Way>& way, OGRLayer *poLayer,
    const QStringList& columns, const QStringList &shpColumns);
};

} // hoot


#endif // SHAPEFILEWRITER_H
