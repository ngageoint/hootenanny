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
#ifndef OSMXMLREADER_H
#define OSMXMLREADER_H

// Hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/elements/Tags.h>
#include "PartialOsmMapReader.h"

// Qt
#include <QHash>
#include <QXmlDefaultHandler>
#include <QXmlStreamReader>
class QString;

// Boost
#include <boost/shared_ptr.hpp>

// Standard
#include <deque>

namespace hoot
{

class OsmMap;
class ElementData;
class Element;

/**
 * Reads in a .osm file into an OsmMap data structure.
 *
 * @todo This could use a nice overhaul for readability and possibly performance.
 */
class OsmXmlReader : public QXmlDefaultHandler, public PartialOsmMapReader
{
public:

  static std::string className() { return "hoot::OsmXmlReader"; }

  static unsigned int logWarnCount;

  OsmXmlReader();
  virtual ~OsmXmlReader();

  virtual void close();

  virtual QString errorString() const { return _errorString; }

  virtual bool endElement(const QString &namespaceURI, const QString &localName,
                  const QString &qName);

  virtual bool fatalError(const QXmlParseException &exception);

  virtual void finalizePartial();

  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

  virtual bool hasMoreElements();

  virtual bool isSupported(QString url);

  virtual void open(QString url);

  virtual void read(OsmMapPtr map);

  void readFromString(QString xml, OsmMapPtr map);

  void read(const QString& path, OsmMapPtr map);

  virtual ElementPtr readNextElement();

  virtual void setDefaultStatus(Status s) { _status = s; }

  virtual bool startElement(const QString &namespaceURI, const QString &localName,
                            const QString &qName, const QXmlAttributes &attributes);

  void setUseDataSourceIds(bool useDataSourceIds) { _useDataSourceId = useDataSourceIds; }
  void setUseStatusFromFile(bool useFileStatus) { _useFileStatus = useFileStatus; }
  void setKeepStatusFromFile(bool keepFileStatus) { _keepFileStatus = keepFileStatus; }
  void setDefaultAccuracy(Meters circularError) { _circularError = circularError; } 
  void setAddSourceDateTime(bool add) { _addSourceDateTime = add; }

private:

  bool _osmFound;
  double _x, _y;
  long _id;

  std::deque<long> _nodeIds;
  /// Maps from old node ids to new node ids.
  QHash<long, long> _nodeIdMap;
  QHash<long, long> _relationIdMap;
  QHash<long, long> _wayIdMap;

  hoot::Tags _tags;
  QString _errorString;
  OsmMapPtr _map;
  boost::shared_ptr<Element> _element;

  Status _status;
  Meters _circularError;

  int _missingNodeCount;
  int _missingWayCount;
  int _badAccuracyCount;
  QString _path;

  bool _keepFileStatus;
  bool _useFileStatus;
  bool _useDataSourceId;
  bool _addSourceDateTime;      ///< Should reader add source:datetime attribute to values read in?

  long _wayId;
  long _relationId;

  mutable boost::shared_ptr<OGRSpatialReference> _wgs84;

  bool _inputCompressed;

  bool _preserveAllTags;

  QFile _inputFile;  //used for partial reading
  QXmlStreamReader _streamReader; //used for partial reading

  // store all key/value strings in this QHash, this promotes implicit sharing of string data. The
  // QHash goes away when the reading is done, but the memory sharing remains.
  QHash<QString, QString> _strings;

  void _createNode(const QXmlAttributes &attributes);
  void _createWay(const QXmlAttributes &attributes);
  void _createRelation(const QXmlAttributes &attributes);

  bool _foundOsmElementXmlStartElement() const;
  bool _foundOsmElementXmlEndElement() const;
  bool _foundOsmHeaderXmlStartElement();

  void _parseTimeStamp(const QXmlAttributes &attributes);

  /**
   * Given the file ID return a relation ID. If the ID has already been mapped it will simply
   * return the mapped ID. If the ID has not been mapped a new ID will be created, mapped and
   * returned.
   */
  long _getRelationId(long fileId);

  double _parseDouble(QString s);
  int _parseInt(QString s);
  long _parseLong(QString s);

  const QString& _saveMemory(const QString& s);

  QXmlAttributes _streamAttributesToAttributes(const QXmlStreamAttributes& streamAttributes);

  void _uncompressInput();
};

}

#endif // OSMXMLREADER_H
