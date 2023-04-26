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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */
#ifndef OSMXMLWRITER_H
#define OSMXMLWRITER_H

// hoot
#include <hoot/core/io/PartialOsmMapWriter.h>

// Qt
#include <QXmlDefaultHandler>
class QXmlStreamWriter;

namespace hoot
{

/**
 * Writes an OsmMap to a .osm (XML) file format.
 */
class OsmXmlWriter : public QXmlDefaultHandler, public PartialOsmMapWriter
{
public:

  static QString className() { return "OsmXmlWriter"; }

  OsmXmlWriter();
  ~OsmXmlWriter() override;

  bool isSupported(const QString& url) const override { return url.endsWith(".osm", Qt::CaseInsensitive); }
  void open(const QString& url) override;
  void openunbuff(const QString& url);
  void close() override;
  QString supportedFormats() const override { return ".osm"; }
  /**
   * Get underlying position from file handle
   */
  uint64_t getPos() const;
  void write(const ConstOsmMapPtr& map) override;
  void writePartial(const ConstNodePtr& node) override;
  void writePartial(const ConstWayPtr& way) override;
  void writePartial(const ConstRelationPtr& relation) override;
  void finalizePartial() override;

  /**
   * Provided for backwards compatibility. Better to just use OsmMapWriterFactory::write()
   */
  void write(const ConstOsmMapPtr& map, const QString& path);

  /**
   * Write the map out to a string and return it. This is handy for debugging, but has obvious
   * memory limitations with real data.
   *
   * @param map the map to write out as a string
   * @param formatXml if true, formats the xml with indentations and new lines
   * @return an OSM XML string
   */
  static QString toString(const ConstOsmMapPtr& map, const bool formatXml = true);

  /**
   * Remove illegal XML characters from the string s and print an error if one is found.  These
   * chars cannot exist in an XML document in any spot and could only have been read in from a
   * non-XML source.
   */
  QString removeInvalidCharacters(const QString& s);

  bool getFormatXml() const { return _formatXml; }

  void setIncludeCompatibilityTags(bool includeCompatibility)
  { _includeCompatibilityTags = includeCompatibility; }
  /**
   * Includes the x/y location of the node with the nd element. This is not standard for OSM
   * and can cause inconsistencies if it isn't updated properly when the file is rewritten, however
   * it can make the recreation of way geometries dramatically more efficient.
   */
  void setIncludePointsInWays(bool includePoints) { _includePointInWays = includePoints; }
  void setPrecision(int p) { _precision = p; }
  void setFormatXml(const bool format) { _formatXml = format; }
  void setIgnoreProgress(const bool ignore) { _ignoreProgress = ignore; }

private:

  static int logWarnCount;

  bool _formatXml;

  bool _includePointInWays;
  bool _includeCompatibilityTags;

  bool _sortTags;
  QString _osmSchema;

  int _precision;
  std::shared_ptr<QIODevice> _fp;
  int _encodingErrorCount;

  std::shared_ptr<QXmlStreamWriter> _writer;

  geos::geom::Envelope _bounds;

  int _numWritten;
  int _statusUpdateInterval;
  /** Some usages of the OsmXmlWriter don't need progress status written out so it can be ignored */
  bool _ignoreProgress;

  static QString _typeName(ElementType e);

  void _initWriter();

  void _writeTags(const ConstElementPtr& element);
  void _writeMetadata(const Element* e) const;
  void _writeNodes(ConstOsmMapPtr map);
  void _writePartialIncludePoints(const ConstWayPtr& w, ConstOsmMapPtr map);
  void _writeWays(ConstOsmMapPtr map);
  void _writeRelations(ConstOsmMapPtr map);

  /**
   * @brief _writeBounds Writes out the OSM <bounds> tag in the format:
   *  <bounds minlat="xxx" minlon="xxx" maxlat="xxx" maxlong="xxx" />
   * @param bounds the bounds to write
   */
  void _writeBounds(const geos::geom::Envelope& bounds) const;

  /**
   * Sets debug settings to add extra metadata to output map for debugging purposes
   * enabled with `debug.maps.write` setting
   */
  void _overrideDebugSettings();
};

}

#endif // OSMXMLWRITER_H
