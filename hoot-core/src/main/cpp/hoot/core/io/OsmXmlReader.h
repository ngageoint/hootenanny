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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMXMLREADER_H
#define OSMXMLREADER_H

// Hoot
#include <hoot/core/elements/Tags.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/Units.h>

// Qt
#include <QHash>
#include <QString>
#include <QXmlDefaultHandler>
#include <QXmlStreamReader>

// Standard
#include <deque>

namespace hoot
{

class OsmMap;
class Element;

/**
 * Reads in a .osm file into an OsmMap data structure.
 */
class OsmXmlReader : public QXmlDefaultHandler, public PartialOsmMapReader, public Boundable
{
public:

  static std::string className() { return "hoot::OsmXmlReader"; }

  static int logWarnCount;

  OsmXmlReader();
  virtual ~OsmXmlReader();

  virtual void close();

  virtual QString errorString() const { return _errorString; }

  virtual bool endElement(const QString& namespaceURI, const QString& localName,
                          const QString& qName);

  virtual bool fatalError(const QXmlParseException &exception);

  virtual void initializePartial() {}

  virtual void finalizePartial();

  virtual std::shared_ptr<OGRSpatialReference> getProjection() const;

  virtual bool hasMoreElements();

  virtual bool isSupported(const QString& url) override;

  virtual void read(const OsmMapPtr& map) override;

  void readFromString(const QString& xml, const OsmMapPtr& map);

  /**
   * Converts OSM XML to a map
   *
   * @param xml the XML string to convert
   * @param useDataSourceId use IDs from the source data
   * @param useDataSourceStatus use statues from the source data
   * @param keepStatusTag retain the status tag in the output
   * @param addChildRefsWhenMissing add referenced children when they are missing from the input
   * @return a map
   */
  static OsmMapPtr fromXml(const QString& xml, const bool useDataSourceId = false,
                           const bool useDataSourceStatus = false,
                           const bool keepStatusTag = false,
                           const bool addChildRefsWhenMissing = false);

  void read(const QString& path, const OsmMapPtr& map);

  virtual ElementPtr readNextElement() override;

  virtual void setDefaultStatus(Status s) override { _status = s; }
  virtual void setUseFileStatus(bool useFileStatus) { _useFileStatus = useFileStatus; }

  virtual bool startElement(const QString& namespaceURI, const QString& localName,
                            const QString& qName, const QXmlAttributes& attributes);

  virtual void setUseDataSourceIds(bool useDataSourceIds) { _useDataSourceId = useDataSourceIds; }
  void setKeepStatusTag(bool keepStatusTag) { _keepStatusTag = keepStatusTag; }
  void setDefaultAccuracy(Meters circularError) { _defaultCircularError = circularError; }
  void setAddSourceDateTime(bool add) { _addSourceDateTime = add; }
  void setPreserveAllTags(bool preserve) { _preserveAllTags = preserve; }
  void setStatusUpdateInterval(long interval) { _statusUpdateInterval = interval; }
  /**
   * This will adds child refs to elements when they aren't present in the source data.  This is
   * only useful when dealing with disconnected chunks of map data, as in external sorting, and
   * should only be activated in that circumstance.  Some verification should be done after
   * reading data with the parameter enabled to ensure all child data is actually present (reading
   * the data a second time will log warnings if any data is missing).
   */
  void setAddChildRefsWhenMissing(bool addChildRefsWhenMissing)
  { _addChildRefsWhenMissing = addChildRefsWhenMissing; }

  virtual QString supportedFormats() override { return ".osm;.osm.bz2;.osm.gz"; }

  virtual void setBounds(const geos::geom::Envelope& bounds) { _bounds = bounds; }

  virtual void setConfiguration(const Settings& conf) override;

  // Its possible we may want to move this method and the ones for all other classes using it up
  // to OsmMapReader;
  void setKeepImmediatelyConnectedWaysOutsideBounds(bool keep)
  { _keepImmediatelyConnectedWaysOutsideBounds = keep; }

protected:

  bool _osmFound;

  /// Maps from old node ids to new node ids.
  QHash<long, long> _nodeIdMap;
  QHash<long, long> _relationIdMap;
  QHash<long, long> _wayIdMap;

  QString _errorString;
  OsmMapPtr _map;
  std::shared_ptr<Element> _element;

  Status _status;
  Meters _defaultCircularError;

  int _missingNodeCount;
  int _missingWayCount;
  int _badAccuracyCount;

  bool _keepStatusTag;
  bool _useFileStatus;
  bool _useDataSourceId;
  bool _addSourceDateTime;      ///< Should reader add source:datetime attribute to values read in?

  long _wayId;
  long _relationId;

  mutable std::shared_ptr<OGRSpatialReference> _wgs84;

  bool _inputCompressed;

  bool _preserveAllTags;

  QFile _inputFile;  //used for partial reading
  QXmlStreamReader _streamReader; //used for partial reading

  // store all key/value strings in this QHash, this promotes implicit sharing of string data. The
  // QHash goes away when the reading is done, but the memory sharing remains.
  QHash<QString, QString> _strings;

  //adds child refs to elements when they aren't present in the source data
  bool _addChildRefsWhenMissing;

  long _numRead;
  long _statusUpdateInterval;

  geos::geom::Envelope _bounds;
  // only valid is _bounds is not null
  bool _keepImmediatelyConnectedWaysOutsideBounds;

  void _createNode(const QXmlAttributes& attributes);
  void _createWay(const QXmlAttributes& attributes);
  void _createRelation(const QXmlAttributes& attributes);

  bool _foundOsmElementXmlStartElement() const;
  bool _foundOsmElementXmlEndElement() const;
  bool _foundOsmHeaderXmlStartElement();

  void _parseTimeStamp(const QXmlAttributes& attributes);

  /**
   * Given the file ID return a relation ID. If the ID has already been mapped it will simply
   * return the mapped ID. If the ID has not been mapped a new ID will be created, mapped and
   * returned.
   */
  long _getRelationId(long fileId);

  double _parseDouble(const QString& s);
  long _parseLong(const QString& s);

  const QString& _saveMemory(const QString& s);

  QXmlAttributes _streamAttributesToAttributes(const QXmlStreamAttributes& streamAttributes);

  void _uncompressInput();

  void _cropToBounds();
};

}

#endif // OSMXMLREADER_H
