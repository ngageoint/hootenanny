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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMJSONWRITER_H
#define OSMJSONWRITER_H

// hoot
#include <hoot/core/io/OsmMapWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/AddExportTagsVisitor.h>

// Qt
#include <QFile>
#include <QHash>
#include <QString>
#include <QXmlDefaultHandler>
class QXmlStreamWriter;

// Standard
#include <deque>


namespace hoot
{

/**
 * Writes an OsmMap to a .json file format.
 * http://overpass-api.de/output_formats.html#json
 *
 * This class uses very simple writing of one element at a time out to the string with print
 * methods. There is not proper JSON writer. This both simplifies dependencies, code and should
 * make things a bit faster.
 *
 * This is being used for unit testing only as of 3/24/2014 and has not been tested for
 * interoperability with any other tools.
 */
class OsmJsonWriter : public QXmlDefaultHandler, public OsmMapWriter, public Configurable
{
public:

  static std::string className() { return "hoot::OsmJsonWriter"; }

  OsmJsonWriter(int precision = ConfigOptions().getWriterPrecision());
  virtual ~OsmJsonWriter() = default;

  virtual bool isSupported(const QString& url) override { return url.toLower().endsWith(".json"); }

  /**
   * Mark up a string so it can be used in JSON. This will add double quotes around the string too.
   */
  static QString markupString(const QString& str);

  virtual void open(const QString& url) override;

  virtual void close() { if (_fp.isOpen()) { _fp.close(); } }

  void setIncludeHootInfo(bool includeInfo) { _includeDebug = includeInfo; }

  void setPrecision(int p) { _precision = p; }

  /**
   * Provided for backwards compatibility. Better to just use OsmMapWriterFactory::write()
   */
  void write(const ConstOsmMapPtr& map, const QString& path);

  virtual void write(const ConstOsmMapPtr& map);

  /**
   * Very handy for testing.
   */
  QString toString(const ConstOsmMapPtr& map);

  /**
   * Allow the writer to write empty tags to JSON
   */
  void SetWriteEmptyTags(bool writeEmpty) { _writeEmptyTags = writeEmpty; }

  void setIncludeCompatibilityTags(bool includeCompatibility)
  { _includeCompatibilityTags = includeCompatibility; }
  void setIncludeCircularError(bool includeCircularError)
  { _addExportTagsVisitor.setIncludeCircularError( includeCircularError); }

  virtual QString supportedFormats() override { return ".json"; }

  virtual void setConfiguration(const Settings& conf) override;

protected:

  ConstOsmMapPtr _map;
  bool _includeDebug;
  // This setting is here to stay in sync with how OsmXmlWriter writes attribute metadata.
  bool _includeCompatibilityTags;
  int _precision;
  QFile _fp;
  QIODevice* _out;
  bool _pretty;
  bool _firstElement;
  bool _writeEmptyTags;
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
  bool _hasTags(const ConstElementPtr& e);
  void _writeTag(const QString& key, const QString& value, bool& firstTag);
  void _writeTags(const ConstElementPtr& e);
  void _writeMetadata(const Element& element);

private:

  AddExportTagsVisitor _addExportTagsVisitor;
};

}

#endif // OSMJSONWRITER_H
