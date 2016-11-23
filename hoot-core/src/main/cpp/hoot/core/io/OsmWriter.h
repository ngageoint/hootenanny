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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMWRITER_H
#define OSMWRITER_H

// hoot
#include <hoot/core/elements/ElementData.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/io/OsmMapWriter.h>

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


namespace hoot
{
using namespace boost;

/**
 * Writes an OsmMap to a .osm (XML) file format.
 */
class OsmWriter : public QXmlDefaultHandler, public OsmMapWriter
{
public:
  static std::string className() { return "hoot::OsmWriter"; }

  OsmWriter();

  virtual bool isSupported(QString url) { return url.toLower().endsWith(".osm"); }

  virtual void open(QString url);

  /**
   * These tags can be included to allow Osmosis to read the files. There is no useful
   * information in the tags.
   */
  void setIncludeCompatibilityTags(bool includeCompatibility);

  void setIncludeIds(bool includeIds) { _includeIds = includeIds; }

  void setIncludeHootInfo(bool includeInfo) { _includeDebug = includeInfo; }

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
   */
  static QString toString(const ConstOsmMapPtr& map);

  /**
   * Provided for backwards compatibility. Better to just use OsmMapWriterFactory::write()
   */
  void write(boost::shared_ptr<const OsmMap> map, const QString& path);

  virtual void write(boost::shared_ptr<const OsmMap> map);

  /**
   * Remove any invalid characters from the string s and print an error if one is found.
   */
  QString removeInvalidCharacters(const QString& s);

protected:
  bool _formatXml;
  bool _includeIds;
  bool _includeDebug;
  bool _includePointInWays;
  bool _includeCompatibilityTags;
  bool _textStatus;
  QString _osmSchema;
  QString _timestamp;
  int _precision;
  auto_ptr<QIODevice> _fp;
  int _encodingErrorCount;

  static QString _typeName(ElementType e);

  void _writeMetadata(QXmlStreamWriter& writer, const Element* e);
  void _writeNodes(shared_ptr<const OsmMap> map, QXmlStreamWriter& writer);
  void _writeWays(shared_ptr<const OsmMap> map, QXmlStreamWriter& writer);
  void _writeRelations(shared_ptr<const OsmMap> map, QXmlStreamWriter& writer);
};

} // hoot

#endif // OSMWRITER_H
