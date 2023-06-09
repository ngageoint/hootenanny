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
   * Will write out up to three files:
   * path + "Points.<ext>"
   * path + "Lines.<ext>"
   * path + "Polygons.<ext>"
   */
  void write(const ConstOsmMapPtr& map) override;

  QStringList getColumns(ConstOsmMapPtr map, ElementType type) const;

  void setColumns(QStringList columns) { _columns = columns; }

  void writeLines(const ConstOsmMapPtr& map, const QString& path);
  void writePoints(const ConstOsmMapPtr& map, const QString& path);
  void writePolygons(const ConstOsmMapPtr& map, const QString& path);

protected:

  QStringList _columns;
  bool _includeCircularError;
  QDir _outputDir;
  int _circularErrorIndex;

  virtual void _removeMultifile(const QString& path) const = 0;
  virtual const char* _getDriverName() const = 0;
  virtual QString _getFileExtension() const = 0;

  void _writeRelationPolygon(const ConstOsmMapPtr& map, const RelationPtr& relation, OGRLayer* poLayer,
                             const QStringList& columns, const QStringList& shpColumns) const;
  void _writeWayPolygon(const ConstOsmMapPtr& map, const WayPtr& way, OGRLayer *poLayer,
                        const QStringList& columns, const QStringList &shpColumns) const;
};

}

#endif // OGR_MULTIFILE_WRITER_H
