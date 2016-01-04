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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "PbfReader.h"

#include <arpa/inet.h>

// Hoot Includes
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/proto/FileFormat.pb.h>
#include <hoot/core/proto/OsmFormat.pb.h>
#include <hoot/core/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/ReportMissingElementsVisitor.h>
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

HOOT_FACTORY_REGISTER(OsmMapReader, PbfReader)

class PbfReaderData
{
public:
  Blob blob;
  BlobHeader blobHeader;
  HeaderBlock headerBlock;
  PrimitiveBlock primitiveBlock;
};

PbfReader::PbfReader()
{
  _init(false);
}

PbfReader::PbfReader(bool useFileId)
{
  _init(useFileId);
}

PbfReader::PbfReader(
  const QString urlString )
{
  _init(false);

  if ( isSupported(urlString) == true )
  {
    open(urlString);
  }
}

void PbfReader::_init(bool useFileId)
{
  _d = new PbfReaderData();
  _useFileId = useFileId;
  _status = hoot::Status::Invalid;
  _circularError = 15.0;
  _badAccuracyCount = 0;
  _useFileStatus = false;
  _missingElementCount = 0;
  _permissive = true;
  _in = NULL;
  _needToCloseInput = false;

  initializePartial();

  setConfiguration(conf());

  GOOGLE_PROTOBUF_VERIFY_VERSION;
}

PbfReader::~PbfReader()
{
  delete _d;
  if ( _needToCloseInput == true )
  {
    close();
  }
}

void PbfReader::setConfiguration(const Settings &conf)
{
  ConfigOptions configOptions(conf);
  setMaxElementsPerMap(configOptions.getMaxElementsPerPartialMap());
  _addSourceDateTime = configOptions.getReaderAddSourceDatetime();
}

void PbfReader::_addTag(shared_ptr<Element> e, QString key, QString value)
{
  if (key == "hoot:status")
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
  else if (key == "accuracy" || key == "error:circular")
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
        if (_badAccuracyCount < 10)
        {
          LOG_WARN("Bad circular error value: " << value.toStdString());
          _badAccuracyCount++;
          if (_badAccuracyCount == 10)
          {
            LOG_WARN("Found 10 bad circular error values, no longer reporting bad accuracies.");
          }
        }
      }
    }
  }
  else if (key == "hoot:id")
  {
    // pass
  }
  else if (value != "")
  {
    e->setTag(key, value);
  }
}

double PbfReader::_convertLon(long lon)
{
  return .000000001 * (_lonOffset + (_granularity * lon));
}

double PbfReader::_convertLat(long lat)
{
  return .000000001 * (_latOffset + (_granularity * lat));
}

ElementId PbfReader::_convertToElementId(long id, int memberType)
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

long PbfReader::_createRelationId(long fromFile)
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

long PbfReader::_createWayId(long fromFile)
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

char* PbfReader::_getBuffer(size_t size)
{
  if (_buffer.size() < size)
  {
    _buffer.resize(size);
  }
  // is this safe? question me in case of crash.
  return (char*)_buffer.data();
}

long PbfReader::_getNodeId(long fromFile)
{
  long newId;
  if (_useFileId)
  {
    newId = fromFile;
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

const char* PbfReader::_inflate(const string& compressed, size_t rawSize)
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

void PbfReader::_loadDenseNodes(const DenseNodes& dn)
{
  int size = std::min(dn.id_size(), std::min(dn.lat_size(), dn.lon_size()));
  if (dn.id_size() != dn.lat_size() || dn.id_size() != dn.lon_size())
  {
    LOG_WARN("Dense node list sizes are not equal.");
  }

  vector< shared_ptr<hoot::Node> > nodes;
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
    shared_ptr<Node> n(new hoot::Node(_status, newId, x, y, _circularError));
    nodes.push_back(n);
    if (_map->containsNode(newId))
    {
      LOG_WARN("Map already contains node: " << newId);
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
      LOG_WARN("Dense info timestamp size doesn't match other node counts");
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
          QDateTime dt = QDateTime::fromMSecsSinceEpoch(timestamp).toTimeSpec(Qt::UTC);
          QString dts = dt.toString("yyyy-MM-ddThh:mm:ss.zzzZ");
          nodes[i]->setTag("source:datetime", dts);
        }
      }
    }
  }

}

void PbfReader::_loadDenseNodes()
{
  const PrimitiveBlock& pb = _d->primitiveBlock;

  for (int i = 0; i < pb.primitivegroup().size(); i++)
  {
    const PrimitiveGroup& pg = pb.primitivegroup().Get(i);
    _loadDenseNodes(pg.dense());
  }
}

void PbfReader::_loadNode(const hoot::pb::Node& n)
{
  // The standard style node parsing code has only been tested with internal data. I haven't
  // looked for external data sets that use this encoding.
  long newId = _getNodeId(n.id());
  double x = _convertLon(n.lon());
  double y = _convertLat(n.lat());

  shared_ptr<hoot::Node> newNode(new hoot::Node(_status, newId, x, y, _circularError));

  for (int i = 0; i < n.keys().size() && i < n.vals().size(); i++)
  {
    if (n.keys().Get(i) >= _strings.size())
    {
      LOG_WARN("Key was out of bounds: " << n.keys().Get(i) << " size: " << _strings.size());
    }
    else if (n.vals().Get(i) >= _strings.size())
    {
      LOG_WARN("Value was out of bounds: " << n.keys().Get(i) << " size: " << _strings.size());
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
    LOG_WARN("Map already contains node: " << newId);
  }
  _map->addNode(newNode);
}

void PbfReader::_loadNodes()
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

void PbfReader::_loadOsmData()
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

vector<PbfReader::BlobLocation> PbfReader::loadOsmDataBlobOffsets(istream& strm)
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

void PbfReader::_loadRelation(const hoot::pb::Relation& r)
{
  long newId = _createRelationId(r.id());

  shared_ptr<hoot::Relation> newRelation(new hoot::Relation(_status, newId, _circularError));


  if (r.roles_sid_size() != r.memids_size() || r.roles_sid_size() != r.types_size())
  {
    LOG_WARN("roles_sid size, memids size or types size don't match."
             << " roles_sid size: " << r.roles_sid_size()
             << " memids size: " << r.memids_size()
             << " types size: " << r.types_size());
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
      LOG_WARN("Relation SID was out of bounds: " << sid << " size: " << _strings.size());
    }

    ElementId eid = _convertToElementId(mid, type);

    if (eid.getType() != ElementType::Unknown)
    {
      newRelation->addElement(role, eid);
    }
  }

  if (r.keys().size() != r.vals().size())
  {
    LOG_WARN("Key and value arrays are not the same size. (" << r.keys().size() << " vs. " <<
             r.vals().size() << " way id: " << r.id() << ")");
  }

  for (int i = 0; i < r.keys().size() && i < r.vals().size(); i++)
  {
    if (r.keys().Get(i) >= _strings.size())
    {
      LOG_WARN("Key was out of bounds: " << r.keys().Get(i) << " size: " << _strings.size());
    }
    else if (r.vals().Get(i) >= _strings.size())
    {
      LOG_WARN("Value was out of bounds: " << r.vals().Get(i) << " size: " << _strings.size());
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
    LOG_WARN("Map already contains relation: " << newId);
  }
  _map->addRelation(newRelation);
}

void PbfReader::_loadRelations()
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

void PbfReader::_loadStrings()
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

void PbfReader::_loadWay(const hoot::pb::Way& w)
{
  long newId = _createWayId(w.id());

  shared_ptr<hoot::Way> newWay(new hoot::Way(_status, newId, _circularError));

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
      _missingElementCount++;
      if (_missingElementCount <= 10)
      {
        LOG_WARN("Missing node (" << ref << ") in way (" << w.id() << ").");
      }
      if (_missingElementCount == 10)
      {
        LOG_WARN("Found 10 missing elements, no longer reporting missing elements.");
      }
    }
    else
    {
      nodes.push_back(_getNodeId(ref));
    }
  }
  newWay->addNodes(nodes);

  if (w.keys().size() != w.vals().size())
  {
    LOG_WARN("Key and value arrays are not the same size. (" << w.keys().size() << " vs. " <<
             w.vals().size() << " way id: " << w.id() << ")");
  }

  for (int i = 0; i < w.keys().size() && i < w.vals().size(); i++)
  {
    if (w.keys().Get(i) >= _strings.size())
    {
      LOG_WARN("Key was out of bounds: " << w.keys().Get(i) << " size: " << _strings.size());
    }
    else if (w.vals().Get(i) >= _strings.size())
    {
      LOG_WARN("Value was out of bounds: " << w.vals().Get(i) << " size: " << _strings.size());
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
    LOG_WARN("Map already contains way: " << newId);
  }
  _map->addWay(newWay);
}

void PbfReader::_loadWays()
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

void PbfReader::parseBlob(BlobLocation& bl, istream* strm, shared_ptr<OsmMap> map)
{
  parseBlob(bl.headerOffset, strm, map);
}

void PbfReader::parseBlob(long headerOffset, istream* strm, shared_ptr<OsmMap> map)
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

void PbfReader::_parseBlob()
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

void PbfReader::_parseBlobHeader()
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

void PbfReader::parseElements(istream* strm, const shared_ptr<OsmMap>& map)
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

int PbfReader::_parseInt(QString s)
{
  bool ok;
  int result = s.toInt(&ok);

  if (ok == false)
  {
      throw HootException("Error parsing int: " + s);
  }

  return result;
}

void PbfReader::_parseOsmData()
{
  size_t size = _d->blob.raw_size();
  const char* inflated = _inflate(_d->blob.zlib_data(), size);
  _d->primitiveBlock.ParseFromArray(inflated, size);

  _loadOsmData();
}

void PbfReader::_parseOsmHeader()
{
  size_t size = _d->blob.raw_size();
  const char* inflated = _inflate(_d->blob.zlib_data(), size);
  _d->headerBlock.ParseFromArray(inflated, size);

  _osmHeaderRead = true;
}

uint32_t PbfReader::_readUInt32()
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

Status PbfReader::_parseStatus(QString s)
{
  Status result;

  result = (Status::Type)_parseInt(s);
  if (result.getEnum() < Status::Invalid || result.getEnum() > Status::Conflated)
  {
    throw HootException(QObject::tr("Invalid status value: %1").arg(s));
  }

  return result;
}

void PbfReader::parse(istream* strm, shared_ptr<OsmMap> map)
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

//TODO: this needs to be integrated with the OsmMapReader/PartialOsmMapReader interface somehow
void PbfReader::read(QString path, shared_ptr<OsmMap> map)
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
      readFile(files.at(i).filePath(), map);
    }
  }
  else
  {
    readFile(path, map);
  }

  ReportMissingElementsVisitor v(true);
  map->visitRw(v);
}

//TODO: this method can probably go away
void PbfReader::readFile(QString path, shared_ptr<OsmMap> map)
{
  fstream input(path.toUtf8().constData(), ios::in | ios::binary);

  if (input.good() == false)
  {
    throw HootException(QString("Error reading %1").arg(path));
  }

  parse(&input, map);
}

void PbfReader::read(shared_ptr<OsmMap> map)
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

//TODO: make the partial reader handle dir inputs?
bool PbfReader::isSupported(QString urlStr)
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

void PbfReader::open(QString urlStr)
{
  fstream* fp = new fstream();
  try
  {
    fp->open(urlStr.toUtf8().data(), ios::in | ios::binary);
    if (fp->is_open() == false)
    {
      throw HootException("Error opening " + urlStr + " for reading.");
    }
    _in = fp;
    _needToCloseInput = true;

    // Have to call initial partial to ensure stream functions work
    initializePartial();
  }
  catch (const HootException& e)
  {
    delete fp;
    throw e;
  }
  catch (const std::exception& e)
  {
    delete fp;
    throw e;
  }
}

void PbfReader::initializePartial()
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

bool PbfReader::hasMoreElements()
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
    if (_partialNodesRead < int(_map->getNodeMap().size()) ||
        _partialWaysRead < int(_map->getWays().size()) ||
        _partialRelationsRead < int(_map->getRelationMap().size()))
    {
      return true;
    }
  }
  return false;
}

shared_ptr<Element> PbfReader::readNextElement()
{
  if (!hasMoreElements())
  {
    throw HootException("No more elements available to read for the map.");
  }

  //if this is the first time through, or we've run out of an element buffer to read from
  if (!_firstPartialReadCompleted ||
      (_partialNodesRead == int(_map->getNodeMap().size()) &&
        _partialWaysRead == int(_map->getWays().size()) &&
        _partialRelationsRead == int(_map->getRelationMap().size())))
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

    _nodesItr = _map->getNodeMap().begin();
    _waysItr = _map->getWays().begin();
    _relationsItr = _map->getRelationMap().begin();

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
  shared_ptr<Element> element;
  if (_partialNodesRead < int(_map->getNodeMap().size()))
  {
    //LOG_DEBUG("node key: " + nodesItr.key());
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
  else if (_partialRelationsRead < int(_map->getRelationMap().size()))
  {
    element.reset(new Relation(*_relationsItr->second.get()));
    _relationsItr++;
    _partialRelationsRead++;
  }
  assert(element.get());
  _elementsRead++;
  return element;
}

void PbfReader::finalizePartial()
{
  _blobIndex = 0;

  _elementsRead = 0;
  _partialNodesRead = 0;
  _partialWaysRead = 0;
  _partialRelationsRead = 0;
}

void PbfReader::close()
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

void PbfReader::closeStream(
  void )
{
  close();
}

void PbfReader::_parseTimestamp(const hoot::pb::Info& info, Tags& t)
{
  if (_addSourceDateTime && t.getInformationCount() > 0) // Make sure we actually have attributes
  {
    long timestamp = 0;
    if (info.has_timestamp())
    {
      timestamp = info.timestamp() * _dateGranularity;

      if (timestamp != 0)
      {
        QDateTime dt = QDateTime::fromMSecsSinceEpoch(timestamp).toTimeSpec(Qt::UTC);
        QString dts = dt.toString("yyyy-MM-ddThh:mm:ss.zzzZ");

        t.set("source:datetime", dts);
      }
    }
  }
}

boost::shared_ptr<OGRSpatialReference> PbfReader::getProjection() const
{
  boost::shared_ptr<OGRSpatialReference> wgs84(new OGRSpatialReference());
  if (wgs84->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
  {
    throw HootException("Error creating EPSG:4326 projection.");
  }

  return wgs84;
}

}
