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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMGBDXXMLWRITER_H
#define OSMGBDXXMLWRITER_H

// hoot
#include <hoot/core/io/PartialOsmMapWriter.h>

// Boost
#include <boost/shared_ptr.hpp>

// Qt
#include <QDir>
#include <QFile>
#include <QHash>
#include <QString>
#include <QXmlDefaultHandler>
class QXmlStreamWriter;

// Standard
#include <deque>

// geos
#include <geos/geom/Envelope.h>

namespace hoot
{

/**
 * Writes an OsmMap to a GBDX XML format file.
 */
class OsmGbdxXmlWriter : public QXmlDefaultHandler, public PartialOsmMapWriter
{
public:

  static std::string className() { return "hoot::OsmGbdxXmlWriter"; }

  static unsigned int logWarnCount;

  OsmGbdxXmlWriter();
  virtual ~OsmGbdxXmlWriter();

  virtual bool isSupported(QString url) { return url.toLower().endsWith(".gxml"); }

  virtual QString supportedFormats() {return ".gxml";}

  virtual void open(QString url);

  void close();

  // Set the precision for writing coordinates
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

  virtual void write(ConstOsmMapPtr map) override;

  virtual void writePartial(const ConstNodePtr& node) override;
  virtual void writePartial(const ConstWayPtr& way) override;
  virtual void writePartial(const ConstRelationPtr& relation) override;
  virtual void finalizePartial();

  /**
   * Remove any invalid characters from the string s and print an error if one is found.
   */
  QString removeInvalidCharacters(const QString& s);

  bool getFormatXml() const { return _formatXml; }
  void setFormatXml(const bool format) { _formatXml = format; }

private:

  bool _formatXml;
  int _precision;
  int _encodingErrorCount;
  int _fileNumber;

  boost::shared_ptr<QIODevice> _fp;
  boost::shared_ptr<QXmlStreamWriter> _writer;
  geos::geom::Envelope _bounds;

  static QString _typeName(ElementType e);

  /**
   * @brief Close the open file and open a unique file for output
   */
  void _newOutputFile();

  QDir _outputDir;
  QString _outputFileName;

  void _writeTags(const ConstElementPtr& element);

  void _writeNodes(ConstOsmMapPtr map);

  void _writeWays(ConstOsmMapPtr map);
  void _writeWayWithPoints(const ConstWayPtr& w, ConstOsmMapPtr map);

  void _writeRelations(ConstOsmMapPtr map);
  void _writeRelationWithPoints(const ConstRelationPtr& r,  ConstOsmMapPtr map);

  /**
   * @brief _writeBounds Writes out the OSM <bounds> tag in the format:
   *  <bounds minlat="xxx" minlon="xxx" maxlat="xxx" maxlong="xxx" />
   * @param bounds the bounds to write
   */
  void _writeBounds(const geos::geom::Envelope& bounds);
};

}

#endif // OSMGBDXXMLWRITER_H
