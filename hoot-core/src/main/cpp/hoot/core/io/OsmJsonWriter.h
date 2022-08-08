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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef OSMJSONWRITER_H
#define OSMJSONWRITER_H

// hoot
#include <hoot/core/io/OsmMapWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/AddExportTagsVisitor.h>

// Qt
#include <QXmlDefaultHandler>

namespace hoot
{

/**
 * Writes an OsmMap to a .json file format: http://overpass-api.de/output_formats.html#json
 *
 * This class uses very simple writing of one element at a time out to the string with print
 * methods. There is not proper JSON writer. This both simplifies dependencies, code and should
 * make things a bit faster.
 */
class OsmJsonWriter : public QXmlDefaultHandler, public OsmMapWriter, public Configurable
{
public:

  static QString className() { return "OsmJsonWriter"; }

  OsmJsonWriter(int precision = ConfigOptions().getWriterPrecision());
  virtual ~OsmJsonWriter() = default;

  void setConfiguration(const Settings& conf) override;

  bool isSupported(const QString& url) const override { return url.endsWith(".json", Qt::CaseInsensitive); }
  void open(const QString& url) override;
  virtual void close() { if (_fp.isOpen()) { _fp.close(); } }
  void write(const ConstOsmMapPtr& map) override;
  QString supportedFormats() const override { return ".json"; }

  /**
   * Provided for backwards compatibility. Better to just use OsmMapWriterFactory::write()
   */
  void write(const ConstOsmMapPtr& map, const QString& path);

  /**
   * Mark up a string so it can be used in JSON. This will add double quotes around the string too.
   */
  static QString markupString(const QString& str);

  /**
   * Very handy for testing.
   */
  QString toString(const ConstOsmMapPtr& map);

  void setPrecision(int p) { _precision = p; }
  void setIncludeCompatibilityTags(bool includeCompatibility)
  { _includeCompatibilityTags = includeCompatibility; }
  void setIncludeCircularError(bool includeCircularError)
  { _addExportTagsVisitor.setIncludeCircularError( includeCircularError); }

protected:

  ConstOsmMapPtr _map;

  int _precision;
  QFile _fp;
  QIODevice* _out;

  bool _firstElement;
  bool _writeHootFormat;

  long _numWritten;
  long _statusUpdateInterval;

  static QString _typeName(ElementType e);

  virtual void _writeNodes();
  virtual void _writeWays();
  virtual void _writeRelations();
  void _write(const QString& str) { _write(str, _pretty); }
  void _write(const QString& str, bool newLine);
  void _writeLn(const QString& str) { _write(str, true); }
  void _writeKvp(const QString& key, const QString& value);
  void _writeKvp(const QString& key, long value);
  void _writeKvp(const QString& key, double value);
  bool _hasTags(const ConstElementPtr& e) const;
  void _writeTags(const ConstElementPtr& e);

private:

  bool _includeDebug;
  // This setting is here to stay in sync with how OsmXmlWriter writes attribute metadata.
  bool _includeCompatibilityTags;
  bool _pretty;
  bool _writeEmptyTags;

  AddExportTagsVisitor _addExportTagsVisitor;

  void _writeTag(const QString& key, const QString& value, bool& firstTag);
  void _writeMetadata(const Element& element);
};

}

#endif // OSMJSONWRITER_H
