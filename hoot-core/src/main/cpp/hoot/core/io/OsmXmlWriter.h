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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMXMLWRITER_H
#define OSMXMLWRITER_H

// hoot
#include <hoot/core/io/PartialOsmMapWriter.h>

// Boost
#include <boost/shared_ptr.hpp>

// Qt
#include <QFile>
#include <QHash>
#include <QXmlDefaultHandler>
class QString;
class QXmlStreamWriter;

// Standard
#include <deque>

// geos
#include <geos/geom/Envelope.h>

namespace hoot
{

/**
 * Writes an OsmMap to a .osm (XML) file format.
 */
class OsmXmlWriter : public QXmlDefaultHandler, public PartialOsmMapWriter
{
public:

  static std::string className() { return "hoot::OsmXmlWriter"; }

  static unsigned int logWarnCount;

  OsmXmlWriter();
  virtual ~OsmXmlWriter();

  virtual bool isSupported(QString url) { return url.toLower().endsWith(".osm"); }

  virtual void open(QString url);

  void close();

  /**
   * These tags can be included to allow Osmosis to read the files. There is no useful
   * information in the tags.
   */
  void setIncludeCompatibilityTags(bool includeCompatibility);

  void setIncludeIds(bool includeIds) { _includeIds = includeIds; }

  void setIncludeHootInfo(bool includeInfo) { _includeDebug = includeInfo; }

  void setIncludePid(bool includePid) { _includePid = includePid; }

  /**
   * Includes the x/y location of the node with the nd element. This is not standard for OSM
   * and can cause inconsistencies if it isn't updated properly when the file is rewritten, however
   * it can make the recreation of way geometries dramatically more efficient.
   */
  void setIncludePointsInWays(bool includePoints) { _includePointInWays = includePoints; }

  void setPrecision(int p) { _precision = p; }

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
   * Provided for backwards compatibility. Better to just use OsmMapWriterFactory::write()
   */
  void write(ConstOsmMapPtr map, const QString& path);

  virtual void write(ConstOsmMapPtr map);

  virtual void writePartial(const ConstNodePtr& node);
  virtual void writePartial(const ConstWayPtr& way);
  virtual void writePartial(const ConstRelationPtr& relation);
  virtual void finalizePartial();

  /**
   * Remove any invalid characters from the string s and print an error if one is found.
   */
  QString removeInvalidCharacters(const QString& s);

  bool getFormatXml() const { return _formatXml; }
  void setFormatXml(const bool format) { _formatXml = format; }

  virtual QString supportedFormats() { return ".osm"; }

private:

  bool _formatXml;
  bool _includeIds;
  bool _includeDebug;
  bool _includePointInWays;
  bool _includeCompatibilityTags;
  bool _includePid;
  bool _textStatus;
  QString _osmSchema;
  QString _timestamp;
  int _precision;
  boost::shared_ptr<QIODevice> _fp;
  int _encodingErrorCount;
  boost::shared_ptr<QXmlStreamWriter> _writer;
  geos::geom::Envelope _bounds;
  bool _includeCircularErrorTags;

  static QString _typeName(ElementType e);

  void _initWriter();

  void _writeTags(const ConstElementPtr& element);
  void _writeMetadata(const Element* e);
  void _writeNodes(ConstOsmMapPtr map);
  void _writePartialIncludePoints(const ConstWayPtr& w, ConstOsmMapPtr map);
  void _writeWays(ConstOsmMapPtr map);
  void _writeRelations(ConstOsmMapPtr map);

  /**
   * @brief _writeBounds Writes out the OSM <bounds> tag in the format:
   *  <bounds minlat="xxx" minlon="xxx" maxlat="xxx" maxlong="xxx" />
   * @param bounds the bounds to write
   */
  void _writeBounds(const geos::geom::Envelope& bounds);
};

}

#endif // OSMXMLWRITER_H
