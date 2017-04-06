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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSMPBFREADER_H
#define OSMPBFREADER_H

// Qt
#include <QHash>
#include <QString>

// Standard Includes
#include <istream>
#include <stdint.h>
#include <vector>

// hoot
#include "PartialOsmMapReader.h"
#include <hoot/core/util/Configurable.h>

// tgs
#include <tgs/BigContainers/BigMap.h>

#include <boost/shared_ptr.hpp>

#include <ogr_spatialref.h>

namespace hoot
{
  namespace pb
  {
    class DenseNodes;
    class Info;
    class Node;
    class Relation;
    class Way;
  }
using namespace std;

class OsmPbfReaderData;

/**
 * A writer for http://wiki.openstreetmap.org/wiki/PBF_Format
 */
class OsmPbfReader : public PartialOsmMapReader, public Configurable
{
public:
  class BlobLocation
  {
  public:
    long headerOffset;
    long blobOffset;
    long blobSize;

    long getSize() const { return (blobOffset - headerOffset) + blobSize; }
  };

  static QString maxElementsPerMapKey() { return "pbf.reader.max.elements.per.partial.map"; }

  static string className() { return "hoot::OsmPbfReader"; }

  static unsigned int logWarnCount;

  OsmPbfReader();
  OsmPbfReader(bool useFileId);

  /**
   * Constructor that immediately attempts to open URL
   *
   * @param urlString URL of file to immediately attempt to open
   */
  OsmPbfReader(const QString urlString);

  ~OsmPbfReader();

  /**
   * Scan through the file and calculate the offsets of every blob. This is handy when
   * distributing the processing of the file.
   */
  vector<BlobLocation> loadOsmDataBlobOffsets(istream& strm);

  /**
   * Determines the reader's default element status
   */
  virtual void setDefaultStatus(Status status) { _status = status; }

  /**
   * Determines whether the reader should use the element id's from the file being read
   */
  virtual void setUseDataSourceIds(bool useDataSourceIds) { _useFileId = useDataSourceIds; }

  /**
   * Determines whether the reader should use the element statuses from the file being read
   */
  void setUseFileStatus(bool useFileStatus) { _useFileStatus = useFileStatus; }

  /**
   * If the input is a directory then the underlying files are read in turn, otherwise readFile
   * is called directly on the file.
   */
  void read(QString path, boost::shared_ptr<OsmMap> map);

  void parse(istream* strm, boost::shared_ptr<OsmMap> map);

  void parseBlob(BlobLocation& bl, istream* strm, boost::shared_ptr<OsmMap> map);

  void parseBlob(long headerOffset, istream* strm, boost::shared_ptr<OsmMap> map);

  /**
   * Reads a uint32 in network order from the stream to determine the PBF size, then reads the
   * PrimitiveBlock from the stream specified into the provided map.
   */
  void parseElements(istream* strm, const boost::shared_ptr<OsmMap>& map);

  /**
   * Allows loading of data that isn't complete such as unknown node IDs in a way.
   */
  void setPermissive(bool permissive) { _permissive = permissive; }

  virtual bool isSupported(QString urlStr);

  virtual void open(QString urlStr);

  virtual void initializePartial();
  /**
   * The read command called after open.
   */
  virtual void read(boost::shared_ptr<OsmMap> map);

  virtual bool hasMoreElements();

  virtual boost::shared_ptr<Element> readNextElement();

  virtual void finalizePartial();

  void close();

  virtual void setConfiguration(const Settings &conf);

  virtual void closeStream(
      void );

  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

  bool getSortedTypeThenId() { return _typeThenId; }

private:

  Meters _circularError;
  string _buffer;
  istream* _in;
  bool _needToCloseInput;

  /// The last position of the pointer while reading data.
  long _lastPosition;
  /// The last time we reported speed
  double _lastReadTime;
  /// When we started reading data
  double _startReadTime;
  long _fileLength;

  string _inflated;
  // Bend over backwards to keep the PBF headers out of the normal build. They're quite large.
  OsmPbfReaderData* _d;
  vector<QString> _strings;
  /// @todo Possibly, it makes sense to replace _map with _partialMap (then rename to _map in base
  /// class), which was added to PartialOsmMapReader after it was implemented on this class.
  boost::shared_ptr<OsmMap> _map;
  Tgs::BigMap<long, long> _nodeIdMap;
  Tgs::BigMap<long, long> _relationIdMap;
  Tgs::BigMap<long, long> _wayIdMap;
  bool _permissive;
  Status _status;
  bool _useFileId;
  bool _osmHeaderRead;
  bool _typeThenId;
  bool _addSourceDateTime;

  int _granularity;
  long _latOffset;
  long _lonOffset;
  long _dateGranularity;

  bool _useFileStatus;

  //for partial reading of blobs
  vector<BlobLocation> _blobs;
  int _blobIndex;

  //partial read iterators
  NodeMap::const_iterator _nodesItr;
  WayMap::const_iterator _waysItr;
  RelationMap::const_iterator _relationsItr;

  //partial read elements read counters
  long _partialNodesRead;
  long _partialWaysRead;
  long _partialRelationsRead;

  QString _urlStr;
  bool _firstPartialReadCompleted;

  void _readFile(QString path, boost::shared_ptr<OsmMap> map);

  void _init(bool useFileId);

  void _addTag(boost::shared_ptr<Element> n, QString k, QString v);

  double _convertLon(long lon);

  double _convertLat(long lat);

  ElementId _convertToElementId(long id, int memberType);

  long _createRelationId(long fromFile);

  long _createWayId(long fromFile);

  char* _getBuffer(size_t size);

  long _getNodeId(long fromFile);

  const char* _inflate(const string& compressed, size_t rawSize);

  void _loadDenseNodes();

  void _loadDenseNodes(const hoot::pb::DenseNodes& dn);

  void _loadNode(const hoot::pb::Node& n);

  void _loadNodes();

  void _loadOsmData();

  void _loadRelation(const hoot::pb::Relation& r);

  void _loadRelations();

  void _loadStrings();

  void _loadWay(const hoot::pb::Way& w);

  void _loadWays();

  void _parseBlob();

  void _parseBlobHeader();

  int _parseInt(QString s);

  void _parseOsmData();

  void _parseOsmHeader();

  Status _parseStatus(QString s);

  void _parseTimestamp(const hoot::pb::Info& info, Tags& t);

  uint32_t _readUInt32();

};

}

#endif // OSMPBFREADER_H
