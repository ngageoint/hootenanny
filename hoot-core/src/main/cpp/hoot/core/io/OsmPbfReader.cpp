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

#include "OsmPbfReader.h"

#include <arpa/inet.h>

// Hoot Includes
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/proto/FileFormat.pb.h>
#include <hoot/core/proto/OsmFormat.pb.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/ReportMissingElementsVisitor.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/elements/Element.h>
#include "ElementInputStream.h"
#include <hoot/core/OsmMap.h>

using namespace hoot::pb;

// Standard Includes
#include <fstream>

// Qt
#include <QDateTime>
#include <QDir>
#include <QFileInfo>

// TGS
#include <tgs/System/Time.h>

#include <boost/shared_ptr.hpp>

#include <ogr_spatialref.h>

// ZLib Includes
#include <zlib.h>

#include "PbfConstants.h"

namespace hoot
{

unsigned int OsmPbfReader::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapReader, OsmPbfReader)

class OsmPbfReaderData
{
public:
  Blob blob;
  BlobHeader blobHeader;
  HeaderBlock headerBlock;
  PrimitiveBlock primitiveBlock;
};

OsmPbfReader::OsmPbfReader()
{
  _init(false);
}

OsmPbfReader::OsmPbfReader(bool useFileId)
{
  _init(useFileId);
}

OsmPbfReader::OsmPbfReader(
  const QString urlString )
{
  _init(false);

  if ( isSupported(urlString) == true )
  {
    open(urlString);
  }
}

void OsmPbfReader::_init(bool useFileId)
{
  _d = new OsmPbfReaderData();
  _useFileId = useFileId;
  _status = hoot::Status::Invalid;
  _circularError = 15.0;
  _useFileStatus = false;
  _permissive = true;
  _in = NULL;
  _needToCloseInput = false;
  _typeThenId = false;

  initializePartial();

  setConfiguration(conf());

  GOOGLE_PROTOBUF_VERIFY_VERSION;
}

OsmPbfReader::~OsmPbfReader()
{
  delete _d;
  if ( _needToCloseInput == true )
  {
    close();
  }
}

void OsmPbfReader::setConfiguration(const Settings &conf)
{
  ConfigOptions configOptions(conf);
  setMaxElementsPerMap(configOptions.getMaxElementsPerPartialMap());
  _addSourceDateTime = configOptions.getReaderAddSourceDatetime();
}

void OsmPbfReader::_addTag(boost::shared_ptr<Element> e, QString key, QString value)
{
  if (key == MetadataTags::HootStatus())
  {
    if (_useFileStatus)
    {
      e->setStatus(_parseStatus(value));
    }
    else
    {
      e->setStatus(_status);
    }
  }
  else if (key == MetadataTags::Accuracy() || key == MetadataTags::ErrorCircular())
  {
    bool ok;
    Meters circularError = value.toDouble(&ok);
    if (circularError > 0 && ok)
    {
      e->setCircularError(circularError);
    }
    else
    {
      bool isBad = false;
      Tags t1;
      t1.set(key, value);
      try
      {
        circularError = t1.getLength(key).value();
        if (circularError > 0)
        {
          e->setCircularError(circularError);
        }
        else
        {
          isBad = true;
        }
      }
      catch (const HootException& e)
      {
        isBad = true;
      }

      if (isBad)
      {
        e->setCircularError(_circularError);

        if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
        {
          LOG_WARN("Bad circular error value: " << value.toStdString());
        }
        else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
    }
  }
  else if (key == MetadataTags::HootId())
  {
    // pass
  }
  else if (value != "")
  {
    e->setTag(key, value);
  }
}

double OsmPbfReader::_convertLon(long lon)
{
  return .000000001 * (_lonOffset + (_granularity * lon));
}

double OsmPbfReader::_convertLat(long lat)
{
  return .000000001 * (_latOffset + (_granularity * lat));
}

ElementId OsmPbfReader::_convertToElementId(long id, int memberType)
{
  ElementType t;
  switch (memberType)
  {
  case Relation_MemberType_NODE:
    t = ElementType::Node;
    break;
  case Relation_MemberType_WAY:
    t = ElementType::Way;
    break;
  case Relation_MemberType_RELATION:
    t = ElementType::Relation;
    break;
  default:
    t = ElementType::Unknown;
  }

  return ElementId(t, id);
}

long OsmPbfReader::_createRelationId(long fromFile)
{
  long id = fromFile;
  long newId;
  if (_useFileId)
  {
    newId = id;
  }
  else
  {
    if (_relationIdMap.contains(fromFile))
    {
      newId = _relationIdMap[fromFile];
    }
    else
    {
      newId = _map->createNextRelationId();
      _relationIdMap.insert(fromFile, newId);
    }
  }

  return newId;
}

long OsmPbfReader::_createWayId(long fromFile)
{
  long id = fromFile;
  long newId;
  if (_useFileId)
  {
    newId = id;
  }
  else
  {
    if (_wayIdMap.contains(fromFile))
    {
      newId = _wayIdMap[fromFile];
    }
    else
    {
      newId = _map->createNextWayId();
      _wayIdMap.insert(fromFile, newId);
    }
  }

  return newId;
}

char* OsmPbfReader::_getBuffer(size_t size)
{
  if (_buffer.size() < size)
  {
    _buffer.resize(size);
  }
  // is this safe? question me in case of crash.
  return (char*)_buffer.data();
}

long OsmPbfReader::_getNodeId(long fromFile)
{
  long newId;
  if (_useFileId)
  {
    if (_permissive == false && _typeThenId == false)
    {
      if (_nodeIdMap.contains(fromFile))
      {
        newId = _nodeIdMap[fromFile];
      }
      else
      {
        newId = fromFile;
        _nodeIdMap.insert(fromFile, newId);
      }
    }
    else
    {
      newId = fromFile;
    }
  }
  else
  {
    if (_nodeIdMap.contains(fromFile))
    {
      newId = _nodeIdMap[fromFile];
    }
    else
    {
      newId = _map->createNextNodeId();
      _nodeIdMap.insert(fromFile, newId);
    }
  }

  return newId;
}

const char* OsmPbfReader::_inflate(const string& compressed, size_t rawSize)
{
  if (_inflated.size() < rawSize)
  {
    _inflated.resize(rawSize);
  }

  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  inflateInit(&strm);
  strm.next_in = (Bytef*)compressed.data();
  strm.avail_in = compressed.size();
  strm.avail_out = rawSize;
  strm.next_out = (Bytef*)_inflated.data();
  int result = inflate(&strm, Z_NO_FLUSH);
  inflateEnd(&strm);

  if (result < Z_OK)
  {
    throw HootException(QString("Error inflating zlib stream. %1").arg(result));
  }

  return _inflated.data();
}

void OsmPbfReader::_loadDenseNodes(const DenseNodes& dn)
{
  int size = std::min(dn.id_size(), std::min(dn.lat_size(), dn.lon_size()));
  if (dn.id_size() != dn.lat_size() || dn.id_size() != dn.lon_size())
  {
    if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN("Dense node list sizes are not equal.");
    }
    else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }

  vector< boost::shared_ptr<hoot::Node> > nodes;
  nodes.reserve(size);

  // the file uses delta encoding
  long lon = 0;
  long lat = 0;
  long id = 0;
  for (int i = 0; i < size; i++)
  {
    id += dn.id().Get(i);
    lon += dn.lon().Get(i);
    lat += dn.lat().Get(i);
    long newId = _getNodeId(id);
    double x = _convertLon(lon);
    double y = _convertLat(lat);
    boost::shared_ptr<Node> n(new hoot::Node(_status, newId, x, y, _circularError));
    nodes.push_back(n);
    if (_map->containsNode(newId))
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Map already contains node: " << newId);
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    _map->addNode(n);
  }

  int index = 0;
  int kv = 0;
  QString k, v;
  for (int i = 0; i < dn.keys_vals_size(); i++)
  {
    int sid = dn.keys_vals().Get(i);
    if (sid == 0)
    {
      index++;
      kv = 0;
    }
    else
    {
      QString str = _strings[sid];
      if (kv == 0)
      {
        k = str;
        kv = 1;
      }
      else
      {
        v = str;
        _addTag(nodes[index], k, v);
        kv = 0;
      }
    }
  }

  if (dn.has_denseinfo())
  {
    const DenseInfo& di = dn.denseinfo();
    int timestampSize = di.timestamp_size();
    if (timestampSize != size)
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Dense info timestamp size doesn't match other node counts");
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
      timestampSize = std::min(size, timestampSize);
    }

    if (_addSourceDateTime)
    {
      long timestamp = 0;
      for (int i = 0; i < timestampSize; i++)
      {
        timestamp += di.timestamp().Get(i) * _dateGranularity;

        if (timestamp != 0 && nodes[i]->getTags().getInformationCount() > 0)
        {
          // QT 4.6 does not have fromMSecsSinceEpoch
          //QDateTime dt = QDateTime::fromMSecsSinceEpoch(timestamp).toTimeSpec(Qt::UTC);
    // same time, but friendly to earlier Qt version
          QDateTime dt = QDateTime::fromTime_t(0).addMSecs(timestamp).toUTC();
          QString dts = dt.toString("yyyy-MM-ddThh:mm:ss.zzzZ");
          nodes[i]->setTag("source:datetime", dts);
        }
      }
    }
  }

}

void OsmPbfReader::_loadDenseNodes()
{
  const PrimitiveBlock& pb = _d->primitiveBlock;

  for (int i = 0; i < pb.primitivegroup().size(); i++)
  {
    const PrimitiveGroup& pg = pb.primitivegroup().Get(i);
    _loadDenseNodes(pg.dense());
  }
}

void OsmPbfReader::_loadNode(const hoot::pb::Node& n)
{
  // The standard style node parsing code has only been tested with internal data. I haven't
  // looked for external data sets that use this encoding.
  long newId = _getNodeId(n.id());
  double x = _convertLon(n.lon());
  double y = _convertLat(n.lat());

  boost::shared_ptr<hoot::Node> newNode(new hoot::Node(_status, newId, x, y, _circularError));

  for (int i = 0; i < n.keys().size() && i < n.vals().size(); i++)
  {
    if (n.keys().Get(i) >= _strings.size())
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Key was out of bounds: " << n.keys().Get(i) << " size: " << _strings.size());
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    else if (n.vals().Get(i) >= _strings.size())
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Value was out of bounds: " << n.keys().Get(i) << " size: " << _strings.size());
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    else
    {
      QString key = _strings[n.keys().Get(i)];
      QString value = _strings[n.vals().Get(i)];

      _addTag(newNode, key, value);
    }
  }

  _parseTimestamp(n.info(), newNode->getTags());

  if (_map->containsNode(newId))
  {
    if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN("Map already contains node: " << newId);
    }
    else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }
  _map->addNode(newNode);
}

void OsmPbfReader::_loadNodes()
{
  const PrimitiveBlock& pb = _d->primitiveBlock;

  for (int i = 0; i < pb.primitivegroup().size(); i++)
  {
    const PrimitiveGroup& pg = pb.primitivegroup().Get(i);
    for (int j = 0; j < pg.nodes().size(); j++)
    {
      _loadNode(pg.nodes().Get(j));
    }
  }
}

void OsmPbfReader::_loadOsmData()
{
  _loadStrings();

  _granularity = _d->primitiveBlock.granularity();
  _lonOffset = _d->primitiveBlock.lon_offset();
  _latOffset = _d->primitiveBlock.lat_offset();
  _dateGranularity = _d->primitiveBlock.date_granularity();

  _loadNodes();
  _loadDenseNodes();
  _loadWays();
  _loadRelations();
  // we don't handle change sets
  // _loadChangeSets();
}

vector<OsmPbfReader::BlobLocation> OsmPbfReader::loadOsmDataBlobOffsets(istream& strm)
{
  vector<BlobLocation> result;

  _in = &strm;

  _in->seekg (0, ios::end);
  long length = _in->tellg();
  _in->seekg (0, ios::beg);

  double start = Tgs::Time::getTime();
  double last = start;
  double t;
  long lastPos = 0;

  // while not done
  while (_in->eof() == false && _in->tellg() < length)
  {
    BlobLocation bl;
    bl.headerOffset = _in->tellg();
    _parseBlobHeader();
    // if the type is a OSMData
    if (_d->blobHeader.type() == PBF_OSM_DATA)
    {
      bl.blobOffset = _in->tellg();
      bl.blobSize = _d->blobHeader.datasize();
      result.push_back(bl);
    }
    _in->seekg(_d->blobHeader.datasize(), ios_base::cur);
    t = Tgs::Time::getTime();
    if (Log::getInstance().getLevel() <= Log::Info && t - start > 5 && t - last >= 2)
    {
      long pos = _in->tellg();
      printf("%.1f / %.1f - %.2f MB/s                  \r",
        pos / 1.0e6, length / 1.0e6,
        ((_in->tellg() - lastPos) / (t - last)) / 1.0e6);
      cout.flush();
      last = t;
      lastPos = _in->tellg();
    }
  }

  t = Tgs::Time::getTime();
  if (Log::getInstance().getLevel() <= Log::Info && t - start > 5)
  {
    // print the final summary
    printf("%.1f / %.1f - %.2f MB/s                  \n",
      length / 1.0e6, length / 1.0e6,
      ((length) / (t - start)) / 1.0e6);
    cout.flush();
  }

  return result;
}

void OsmPbfReader::_loadRelation(const hoot::pb::Relation& r)
{
  long newId = _createRelationId(r.id());

  boost::shared_ptr<hoot::Relation> newRelation(new hoot::Relation(_status, newId, _circularError));


  if (r.roles_sid_size() != r.memids_size() || r.roles_sid_size() != r.types_size())
  {
    if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN("roles_sid size, memids size or types size don't match."
               << " roles_sid size: " << r.roles_sid_size()
               << " memids size: " << r.memids_size()
               << " types size: " << r.types_size());
    }
    else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }
  int membersSize = std::min(r.roles_sid_size(), std::min(r.memids_size(), r.types_size()));

  long mid = 0;

  for (int i = 0; i < membersSize; i++)
  {
    int sid = r.roles_sid().Get(i);
    mid += r.memids().Get(i);
    int type = r.types().Get(i);
    QString role;
    if ((size_t)sid < _strings.size() && sid >= 0)
    {
      role = _strings[sid];
    }
    else
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Relation SID was out of bounds: " << sid << " size: " << _strings.size());
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }

    ElementId eid = _convertToElementId(mid, type);

    if (eid.getType() != ElementType::Unknown)
    {
      newRelation->addElement(role, eid);
    }
  }

  if (r.keys().size() != r.vals().size())
  {
    if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN("Key and value arrays are not the same size. (" << r.keys().size() << " vs. " <<
               r.vals().size() << " way id: " << r.id() << ")");
    }
    else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }

  for (int i = 0; i < r.keys().size() && i < r.vals().size(); i++)
  {
    if (r.keys().Get(i) >= _strings.size())
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Key was out of bounds: " << r.keys().Get(i) << " size: " << _strings.size());
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    else if (r.vals().Get(i) >= _strings.size())
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Value was out of bounds: " << r.vals().Get(i) << " size: " << _strings.size());
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    else
    {
      QString key = _strings[r.keys().Get(i)];
      QString value = _strings[r.vals().Get(i)];

      _addTag(newRelation, key, value);
    }
  }

  _parseTimestamp(r.info(), newRelation->getTags());

  if (_map->containsRelation(newId))
  {
    if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN("Map already contains relation: " << newId);
    }
    else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }
  _map->addRelation(newRelation);
}

void OsmPbfReader::_loadRelations()
{
  const PrimitiveBlock& pb = _d->primitiveBlock;

  for (int i = 0; i < pb.primitivegroup().size(); i++)
  {
    const PrimitiveGroup& pg = pb.primitivegroup().Get(i);
    for (int j = 0; j < pg.relations().size(); j++)
    {
      _loadRelation(pg.relations().Get(j));
    }
  }
}

void OsmPbfReader::_loadStrings()
{
  _strings.clear();

  const StringTable& st = _d->primitiveBlock.stringtable();
  _strings.resize(st.s_size() + 1);
  for (int i = 0; i < st.s_size(); i++)
  {
    QString str = QString::fromUtf8(st.s().Get(i).data());
    _strings[i] = str;
  }
}

void OsmPbfReader::_loadWay(const hoot::pb::Way& w)
{
  long newId = _createWayId(w.id());

  boost::shared_ptr<hoot::Way> newWay(new hoot::Way(_status, newId, _circularError));

  // if the cached envelope is valid
  if (w.has_bbox())
  {
    // read the way's approximate bounding box. This is a custom Hoot extension to the format.
    double minX, maxX, minY, maxY;
    minX = _convertLon(w.bbox().left());
    maxX = _convertLon(w.bbox().right());
    minY = _convertLat(w.bbox().bottom());
    maxY = _convertLat(w.bbox().top());
    newWay->setCachedEnvelope(Envelope(minX, maxX, minY, maxY));
  }

  long nid = 0;
  vector<long> nodes;
  for (int i = 0; i < w.refs_size(); i++)
  {
    nid += w.refs().Get(i);
    long ref = nid;
    if (_permissive == false && _nodeIdMap.contains(ref) == false)
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Missing node (" << ref << ") in way (" << w.id() << ").");
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    else
    {
      nodes.push_back(_getNodeId(ref));
    }
  }
  newWay->addNodes(nodes);

  if (w.keys().size() != w.vals().size())
  {
    if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN("Key and value arrays are not the same size. (" << w.keys().size() << " vs. " <<
               w.vals().size() << " way id: " << w.id() << ")");
    }
    else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }

  for (int i = 0; i < w.keys().size() && i < w.vals().size(); i++)
  {
    if (w.keys().Get(i) >= _strings.size())
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Key was out of bounds: " << w.keys().Get(i) << " size: " << _strings.size());
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    else if (w.vals().Get(i) >= _strings.size())
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Value was out of bounds: " << w.vals().Get(i) << " size: " << _strings.size());
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    else
    {
      QString key = _strings[w.keys().Get(i)];
      QString value = _strings[w.vals().Get(i)];

      _addTag(newWay, key, value);
    }
  }

  _parseTimestamp(w.info(), newWay->getTags());

  if (_map->containsWay(newId))
  {
    if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN("Map already contains way: " << newId);
    }
    else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }
  _map->addWay(newWay);
}

void OsmPbfReader::_loadWays()
{
  const PrimitiveBlock& pb = _d->primitiveBlock;

  for (int i = 0; i < pb.primitivegroup().size(); i++)
  {
    const PrimitiveGroup& pg = pb.primitivegroup().Get(i);
    for (int j = 0; j < pg.ways().size(); j++)
    {
      _loadWay(pg.ways().Get(j));
    }
  }
}

void OsmPbfReader::parseBlob(BlobLocation& bl, istream* strm, boost::shared_ptr<OsmMap> map)
{
  parseBlob(bl.headerOffset, strm, map);
}

void OsmPbfReader::parseBlob(long headerOffset, istream* strm, boost::shared_ptr<OsmMap> map)
{
  _in = strm;
  _map = map;

  if (strm->good() == false)
  {
    throw HootException("The stream passed in is not \"good\".");
  }

  strm->seekg(headerOffset, ios_base::beg);
  _parseBlobHeader();
  // Did we hit OSM header?
  if (_d->blobHeader.type() == PBF_OSM_DATA)
  {
    // read the blob and parse data
    _parseBlob();
    _parseOsmData();
  }
}

void OsmPbfReader::_parseBlob()
{
  int size = _d->blobHeader.datasize();
  _in->read(_getBuffer(size), size);
  if (_in->gcount() != size)
  {
    throw HootException(QString("Did not read the expected number of bytes from blob. "
                                "(%1 instead of %2)").
                        arg(_in->gcount()).arg(size));
  }
  _d->blob.ParseFromArray(_buffer.data(), size);
}

void OsmPbfReader::_parseBlobHeader()
{
  uint32_t size = _readUInt32();
  if (_in->eof())
  {
    return;
  }

  _in->read(_getBuffer(size), size);
  if (_in->gcount() != size)
  {
    throw HootException(QString("Did not read the expected number of bytes from blob header. "
                                "(%1 instead of %2)").
                        arg(_in->gcount()).arg(size));
  }
  _d->blobHeader.ParseFromArray(_buffer.data(), size);
}

void OsmPbfReader::parseElements(istream* strm, const boost::shared_ptr<OsmMap>& map)
{
  _map = map;
  _in = strm;
  uint32_t size = _readUInt32();
  strm->read(_getBuffer(size), size);
  if (strm->gcount() != size)
  {
    throw HootException(QString("Did not read the expected number of bytes from stream. "
                                "(%1 instead of %2)").
                        arg(strm->gcount()).arg(size));
  }
  _d->primitiveBlock.ParseFromArray(_buffer.data(), size);

  _loadOsmData();
}

int OsmPbfReader::_parseInt(QString s)
{
  bool ok;
  int result = s.toInt(&ok);

  if (ok == false)
  {
      throw HootException("Error parsing int: " + s);
  }

  return result;
}

void OsmPbfReader::_parseOsmData()
{
  size_t size = _d->blob.raw_size();
  const char* inflated = _inflate(_d->blob.zlib_data(), size);
  _d->primitiveBlock.ParseFromArray(inflated, size);

  _loadOsmData();
}

void OsmPbfReader::_parseOsmHeader()
{
  size_t size = _d->blob.raw_size();
  const char* inflated = _inflate(_d->blob.zlib_data(), size);
  _d->headerBlock.ParseFromArray(inflated, size);

  int optionalFeatureSize = _d->headerBlock.optional_features_size();
  for (int i = 0; i < optionalFeatureSize; i++)
  {
    std::string typeThenId = _d->headerBlock.optional_features(i);
    if (typeThenId == PBF_SORT_TYPE_THEN_ID)
    {
      _typeThenId = true;
    }
  }

  _osmHeaderRead = true;
}

uint32_t OsmPbfReader::_readUInt32()
{
  uint32_t buf = 0xFFFFFFFF;
  _in->read((char*)&buf, 4);
  if (_in->gcount() == 0 && _in->eof())
  {
    return 0xFFFFFFFF;
  }
  if (_in->gcount() != 4)
  {
    throw HootException(QString("Did not read the expected number of bytes. %1 instead of 4").
                        arg(_in->gcount()));
  }
  return ntohl(buf);
}

Status OsmPbfReader::_parseStatus(QString s)
{
  Status result;

  result = (Status::Type)_parseInt(s);
  if (result.getEnum() < Status::Invalid || result.getEnum() > Status::Conflated)
  {
    throw HootException(QObject::tr("Invalid status value: %1").arg(s));
  }

  return result;
}

void OsmPbfReader::parse(istream* strm, boost::shared_ptr<OsmMap> map)
{
  _in = strm;
  _map = map;

  // read blob header
  _parseBlobHeader();
  // read blob
  _parseBlob();
  // read OSMHeader
  _parseOsmHeader();

  // read blob header
  _parseBlobHeader();

  // while not done
  while (_in->eof() == false)
  {
    // if the type is a OSMData
    if (_d->blobHeader.type() == PBF_OSM_DATA)
    {
      // read the blob and parse data
      _parseBlob();
      _parseOsmData();
    }
    // if we don't recognize the type
    else
    {
      // skip ahead to the next blob
      strm->seekg(_d->blobHeader.datasize(), ios_base::cur);
    }

    // read blob header
    _parseBlobHeader();
  }
}

/// @todo this needs to be integrated with the OsmMapReader/PartialOsmMapReader interface somehow
void OsmPbfReader::read(QString path, boost::shared_ptr<OsmMap> map)
{
  if (_status == Status::Invalid)
  {
    _useFileStatus = true;
  }

  QFileInfo fi(path);
  if (fi.isDir())
  {
    QDir d(path);
    QStringList filter;
    filter << "*.pbf";
    QFileInfoList files = d.entryInfoList(filter, QDir::Files, QDir::Name);
    for (int i = 0; i < files.size(); i++)
    {
      _readFile(files.at(i).filePath(), map);
    }
  }
  else
  {
    _readFile(path, map);
  }

  ReportMissingElementsVisitor v(true);
  map->visitRw(v);
}

void OsmPbfReader::_readFile(QString path, boost::shared_ptr<OsmMap> map)
{
  fstream input(path.toUtf8().constData(), ios::in | ios::binary);

  if (input.good() == false)
  {
    throw HootException(QString("Error reading %1").arg(path));
  }

  parse(&input, map);
}

void OsmPbfReader::read(boost::shared_ptr<OsmMap> map)
{
  assert(map.get());
  if (_status == Status::Invalid)
  {
    _useFileStatus = true;
  }

  parse(_in, map);

  ReportMissingElementsVisitor v(true);
  map->visitRw(v);
}

/// @todo make the partial reader handle dir inputs?
bool OsmPbfReader::isSupported(QString urlStr)
{
  QFileInfo fileInfo(urlStr);
  if (fileInfo.isDir())
  {
    throw HootException("Can't handle dirs with partial read yet.");
  }

  QFile input(urlStr);
  return
    //there is actually some test data that ends in .pbf instead of .osm.pbf, so allowing that
    //extension too for now
    input.exists() && (urlStr.toLower().endsWith(".osm.pbf") || urlStr.toLower().endsWith(".pbf"));
}

void OsmPbfReader::open(QString urlStr)
{
  fstream* fp = new fstream();
  fp->open(urlStr.toUtf8().data(), ios::in | ios::binary);
  if (fp->is_open() == false)
  {
    delete fp;
    throw HootException("Error opening " + urlStr + " for reading.");
  }
  _in = fp;
  _needToCloseInput = true;

  // Have to call initial partial to ensure stream functions work
  initializePartial();
}

void OsmPbfReader::initializePartial()
{
  _permissive = true;

  _map.reset(new OsmMap());
  _blobIndex = 0;

  _elementsRead = 0;
  _partialNodesRead = 0;
  _partialWaysRead = 0;
  _partialRelationsRead = 0;

  // If nothing's been opened yet, this needs to be a no-op to be safe
  if ( _in != NULL )
  {
    _blobs = loadOsmDataBlobOffsets(*_in);
    _in->seekg (0, ios::end);    _fileLength = _in->tellg();
    _in->seekg (0, ios::beg);
  }
}

bool OsmPbfReader::hasMoreElements()
{
  // If we've closed/finalized, definitely no
  if ( _in == NULL )
  {
    return false;
  }
  //see if we've read all the blobs
  else if (_blobIndex < (int)_blobs.size())
  {
    return true;
  }
  //see if we've iterated through all of the elements
  else
  {
    if (_partialNodesRead < int(_map->getNodes().size()) ||
        _partialWaysRead < int(_map->getWays().size()) ||
        _partialRelationsRead < int(_map->getRelations().size()))
    {
      return true;
    }
  }
  return false;
}

boost::shared_ptr<Element> OsmPbfReader::readNextElement()
{
  if (!hasMoreElements())
  {
    throw HootException("No more elements available to read for the map.");
  }

  //if this is the first time through, or we've run out of an element buffer to read from
  if (!_firstPartialReadCompleted ||
      (_partialNodesRead == int(_map->getNodes().size()) &&
        _partialWaysRead == int(_map->getWays().size()) &&
        _partialRelationsRead == int(_map->getRelations().size())))
  {
    if (!_firstPartialReadCompleted)
    {
      _startReadTime = Tgs::Time::getTime();
      _lastReadTime = _startReadTime;
      _lastPosition = 0;
    }
    //clear out the map and read a new blob

    _map->clear();
    _partialNodesRead = 0;
    _partialWaysRead = 0;
    _partialRelationsRead = 0;

    parseBlob(_blobs[_blobIndex++], _in, _map);

    _nodesItr = _map->getNodes().begin();
    _waysItr = _map->getWays().begin();
    _relationsItr = _map->getRelations().begin();

    _firstPartialReadCompleted = true;
  }

  if ((_elementsRead & 0x100) == 0 && Log::getInstance().getLevel() <= Log::Debug)
  {
    double t = Tgs::Time::getTime();
    double diff = t - _lastReadTime;
    if (t - _startReadTime > 5 && diff > 2)
    {
      long pos = _in->tellg();
      printf("Reading .osm.pbf %.1f / %.1f - %.2f MB/s                  \r",
        pos / 1.0e6, _fileLength / 1.0e6,
        ((pos - _lastPosition) / (t - _lastReadTime)) / 1.0e6);
      cout.flush();
      _lastReadTime = t;
      _lastPosition = pos;
    }
  }

  //read nodes, then ways, then relations
  //there's possibly a way to read the element in one code block instead of three...just wasn't
  //able to get it to work yet
  boost::shared_ptr<Element> element;
  if (_partialNodesRead < int(_map->getNodes().size()))
  {
    /// @optimize
    // we have to copy here so that the element isn't part of two maps. This should be fixed if we
    // need the reader to go faster.

    element.reset(new Node(*_nodesItr->second.get()));
    _nodesItr++;
    _partialNodesRead++;
  }
  else if (_partialWaysRead < int(_map->getWays().size()))
  {
    element.reset(new Way(*_waysItr->second.get()));
    _waysItr++;
    _partialWaysRead++;
  }
  else if (_partialRelationsRead < int(_map->getRelations().size()))
  {
    element.reset(new Relation(*_relationsItr->second.get()));
    _relationsItr++;
    _partialRelationsRead++;
  }
  assert(element.get());
  _elementsRead++;
  return element;
}

void OsmPbfReader::finalizePartial()
{
  _blobIndex = 0;

  _elementsRead = 0;
  _partialNodesRead = 0;
  _partialWaysRead = 0;
  _partialRelationsRead = 0;
}

void OsmPbfReader::close()
{
  finalizePartial();

  if ( _needToCloseInput == true )
  {
    // Deleting fstream objects invokes the istream destructor, who in turn calls istream::close as part of its contract
    delete _in;
    _needToCloseInput = false;
  }

  // Either path, drop our pointer to the stream
  _in = NULL;
}

void OsmPbfReader::closeStream(
  void )
{
  close();
}

void OsmPbfReader::_parseTimestamp(const hoot::pb::Info& info, Tags& t)
{
  if (_addSourceDateTime && t.getInformationCount() > 0) // Make sure we actually have attributes
  {
    long timestamp = 0;
    if (info.has_timestamp())
    {
      timestamp = info.timestamp() * _dateGranularity;

      if (timestamp != 0)
      {
        //QDateTime dt = QDateTime::fromMSecsSinceEpoch(timestamp).toTimeSpec(Qt::UTC);
        QDateTime dt = QDateTime::fromTime_t(0).addMSecs(timestamp).toUTC();
        QString dts = dt.toString("yyyy-MM-ddThh:mm:ss.zzzZ");

        t.set("source:datetime", dts);
      }
    }
  }
}

boost::shared_ptr<OGRSpatialReference> OsmPbfReader::getProjection() const
{
  boost::shared_ptr<OGRSpatialReference> wgs84(new OGRSpatialReference());
  if (wgs84->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
  {
    throw HootException("Error creating EPSG:4326 projection.");
  }

  return wgs84;
}

}
