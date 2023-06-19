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

#ifndef FLAT_GEOBUF_WRITER_H
#define FLAT_GEOBUF_WRITER_H

// hoot
#include <hoot/core/io/OgrMultifileWriter.h>

namespace hoot
{

/**
 * FlatGeobufWriter writes an OsmMap to FlatGeobuf file format, and is capable of custom handling of
 * columns. It is not capable of using translations. For that, use OgrWriter.
 */
class FlatGeobufWriter : public OgrMultifileWriter
{
public:

  static QString className() { return "FlatGeobufWriter"; }

  FlatGeobufWriter() = default;
  ~FlatGeobufWriter() override = default;

  QString supportedFormats() const override { return ".fgb"; }
  bool isSupported(const QString& url) const override { return url.endsWith(".fgb", Qt::CaseInsensitive); }

protected:

  void _removeMultifile(const QString& path) const override;
  const char* _getDriverName() const override { return "FlatGeobuf"; };
  QString _getFileExtension() const override { return ".fgb"; }
  OgrOptions _getOptions() const override;
  bool _convertPolygons() const override { return true; }

};

}

#endif // FLAT_GEOBUF_WRITER_H
