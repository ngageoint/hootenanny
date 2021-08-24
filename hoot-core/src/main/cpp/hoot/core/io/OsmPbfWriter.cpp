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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "OsmPbfWriter.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/PbfConstants.h>
#include <hoot/core/proto/FileFormat.pb.h>
#include <hoot/core/proto/OsmFormat.pb.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>
//  Version must be included last
#include <hoot/core/info/Version.h>
#include <hoot/core/info/VersionDefines.h>

// Qt
#include <qendian.h>

// Standard
#include <fstream>

// ZLib
#include <zlib.h>

//  htonl
#include <arpa/inet.h>

using namespace geos::geom;
using namespace hoot::pb;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmPbfWriter)

const char* const OsmPbfWriter::OSM_DATA = "OSMData";
const char* const OsmPbfWriter::OSM_HEADER = "OSMHeader";

class OsmPbfWriterData
{
public:

  Blob blob;
  BlobHeader blobHeader;
  HeaderBlock headerBlock;
  HeaderBBox headerBBox;
  PrimitiveBlock primitiveBlock;
};

OsmPbfWriter::OsmPbfWriter() :
_d(std::make_shared<OsmPbfWriterData>())
{
  _dn = nullptr;
  _lonOffset = 0.0;
  _latOffset = 0.0;
  _granularity = 100;
  // Throw a warning if the blob is bigger than this.
  _maxBlobTarget = 32 * 1024 * 1024;
  // If the blob is larger than this then serialize it.
  _minBlobTarget = 15 * 1024 * 1024;
  _enablePbFlushing = true;
  _elementsWritten = 0;
  _nodeIdDelta = 0;
  _relationIdDelta = 0;
  _wayIdDelta = 0;
  _compressionLevel = -1;
  _includeVersion = true;
  _needToCloseInput = false;

  GOOGLE_PROTOBUF_VERIFY_VERSION;
}

OsmPbfWriter::~OsmPbfWriter()
{
  close();
}

long OsmPbfWriter::_convertLon(double lon) const
{
  return (1000000000 * lon - _lonOffset) / _granularity;
}

long OsmPbfWriter::_convertLat(double lat) const
{
  return (1000000000 * lat - _latOffset) / _granularity;
}

int OsmPbfWriter::_convertString(const QString& s)
{
  int id;
  QHash<QString, int>::const_iterator sit = _strings.find(s);
  if (sit != _strings.end())
  {
    id = sit.value();
  }
  else
  {
    _strings.insert(s, _strings.size() + 1);
    id = _strings.size();
    _d->primitiveBlock.mutable_stringtable()->add_s(s.toUtf8());
  }

  return id;
}

char* OsmPbfWriter::_getBuffer(size_t size)
{
  if (_buffer.size() < size)
  {
    _buffer.resize(size);
  }
  // is this safe? question me in case of crash.
  return (char*)_buffer.data();
}

void OsmPbfWriter::_deflate(const char* raw, size_t rawSize)
{
  // make sure we have enough room for deflation plus a little head room.
  if (_deflateBuffer.size() < rawSize + 1024)
  {
    _deflateBuffer.resize(rawSize + 1024);
  }

  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  deflateInit(&strm, _compressionLevel);
  strm.next_in = (Bytef*)raw;
  strm.avail_in = rawSize;
  strm.avail_out = rawSize + 1024;
  strm.next_out = (Bytef*)_deflateBuffer.data();

  int result = deflate(&strm, Z_FINISH);
  if (result != Z_STREAM_END)
  {
    throw HootException(QString("Error deflating zlib stream. %1").arg(result));
  }

  result = deflateEnd(&strm);
  if (result != Z_OK)
  {
    throw HootException(QString("Error freeing deflate stream. %1").arg(result));
  }

  _deflateSize = strm.total_out;
}

void OsmPbfWriter::finalizePartial()
{
  LOG_DEBUG("Finalizing partial...");
  // finalize the current blob.
  _writePrimitiveBlock();
}

void OsmPbfWriter::_initBlob()
{
  LOG_TRACE("Initializing the blob...");

  _d->blob.Clear();
  _d->blobHeader.Clear();
  _d->primitiveBlock.Clear();
  // if the granularity isn't the default.
  if (_granularity != 100)
  {
    _d->primitiveBlock.set_granularity(_granularity);
  }
  _d->primitiveBlock.mutable_stringtable()->add_s("");
  _lastId = 0;
  _lastLon = 0;
  _lastLat = 0;
  _lastWayNid = 0;
  _pg = nullptr;
  _dn = nullptr;
  _tick = 0;
  _strings.clear();
}

void OsmPbfWriter::initializePartial(ostream* strm)
{
  LOG_DEBUG("Initializing partial...");
  _out = strm;
  _writeOsmHeader(false, false);
  _initBlob();
}

void OsmPbfWriter::initializePartial()
{
  LOG_DEBUG("Initializing partial...");
  _writeOsmHeader();
  _initBlob();
}

void OsmPbfWriter::_open(const QString& url)
{
  LOG_TRACE("Opening url: " << url);
  _openStream = std::make_shared<std::fstream>(url.toUtf8().constData(), ios::out | ios::binary);
  if (_openStream->good() == false)
  {
    throw HootException(QString("Error opening for writing: %1").arg(url));
  }
  _out = _openStream.get();
  _needToCloseInput = true;
}

void OsmPbfWriter::open(const QString& url)
{
  _open(url);
  //  OsmPbfWriter::open() shouldn't call OsmPbfWriter::initializePartial()
  //  or it will cause duplicate headers
}

void OsmPbfWriter::close()
{
  if (_needToCloseInput)
  {
    LOG_DEBUG("Closing PBF writer...");
    if (_openStream.get())
    {
      _openStream->close();
    }
    _needToCloseInput = false;
  }
}

void OsmPbfWriter::setIdDelta(long nodeIdDelta, long wayIdDelta, long relationIdDelta)
{
  _nodeIdDelta = nodeIdDelta;
  _wayIdDelta = wayIdDelta;
  _relationIdDelta = relationIdDelta;
}

int OsmPbfWriter::_toRelationMemberType(ElementType t) const
{
  // This is actually a 1 to 1 translation, but I don't want to rely on the element type
  // enumerations staying static.
  switch (t.getEnum())
  {
  case ElementType::Node:
    return Relation_MemberType_NODE;
  case ElementType::Relation:
    return Relation_MemberType_RELATION;
  case ElementType::Way:
    return Relation_MemberType_WAY;
  default:
    throw IllegalArgumentException("Unexpected element type.");
  }
}

void OsmPbfWriter::write(const ConstOsmMapPtr& map)
{
  write(map, _openStream.get());
  _openStream.reset();
}

void OsmPbfWriter::write(const ConstOsmMapPtr& map, ostream* strm)
{
  LOG_DEBUG("Writing stream...");

  _out = strm;
  _map = map;

  _rawSize = 0;

  // write out the "OSMHeader fileblock"
  _writeOsmHeader();

  // intialialize the blob. This doesn't actually write anything to the output stream.
  _initBlob();

  _writeMap();

  // finalize the current blob.
  _writePrimitiveBlock();
}

void OsmPbfWriter::write(const ConstOsmMapPtr& map, const QString& path)
{
  fstream output(path.toUtf8().constData(), ios::out | ios::binary);

  if (output.good() == false)
  {
    throw HootException(QString("Error opening for writing: %1").arg(path));
  }

  write(map, &output);
}

void OsmPbfWriter::writeHeader(ostream* strm, bool includeBounds, bool sorted)
{
  LOG_DEBUG("Writing header...");

  _out = strm;

  _rawSize = 0;

  // write out the "OSMHeader fileblock"
  _writeOsmHeader(includeBounds, sorted);
}

void OsmPbfWriter::writePb(const ConstOsmMapPtr& m, ostream* strm)
{
  _initBlob();

  _map = m;
  bool oldSetting = _enablePbFlushing;
  _enablePbFlushing = false;
  _writeMap();
  uint32_t size = qToBigEndian((uint32_t)_d->primitiveBlock.ByteSizeLong());
  strm->write((const char*)&size, sizeof(uint32_t));
  _d->primitiveBlock.SerializePartialToOstream(strm);
  _map.reset();
  _enablePbFlushing = oldSetting;
}

void OsmPbfWriter::writePb(const ConstNodePtr& n, ostream* strm)
{
  _initBlob();

  _writeNode(n);
  uint32_t size = qToBigEndian((uint32_t)_d->primitiveBlock.ByteSizeLong());
  strm->write((const char*)&size, sizeof(uint32_t));
  _d->primitiveBlock.SerializePartialToOstream(strm);
}

void OsmPbfWriter::writePb(const ConstWayPtr& w, ostream* strm)
{
  _initBlob();

  _writeWay(w);
  uint32_t size = qToBigEndian((uint32_t)_d->primitiveBlock.ByteSizeLong());
  strm->write((const char*)&size, sizeof(uint32_t));
  _d->primitiveBlock.SerializePartialToOstream(strm);
}

void OsmPbfWriter::writePb(const ConstRelationPtr& r, ostream* strm)
{
  _initBlob();

  _writeRelation(r);
  uint32_t size = qToBigEndian((uint32_t)_d->primitiveBlock.ByteSizeLong());
  strm->write((const char*)&size, sizeof(uint32_t));
  _d->primitiveBlock.SerializePartialToOstream(strm);
}

void OsmPbfWriter::_writeBlob(const char* buffer, int size, const string& type)
{
  LOG_DEBUG("Writing blob...");

  // compress the buffer
  _deflate(buffer, size);

  // create the blob first so we can put the size in the header.
  _d->blob.Clear();
  _d->blob.set_raw_size(size);
  _d->blob.set_zlib_data(_deflateBuffer.data(), _deflateSize);

  // create and serialize the blob header
  _d->blobHeader.Clear();
  _d->blobHeader.set_type(type);
  _d->blobHeader.set_datasize((uint32_t)_d->blob.ByteSizeLong());

  uint32_t blobHeaderSize = htonl((uint32_t)_d->blobHeader.ByteSizeLong());
  _out->write((char*)&blobHeaderSize, 4);

  // serialize the blob header
  _d->blobHeader.SerializePartialToOstream(_out);

  // serialize the blob
  _d->blob.SerializePartialToOstream(_out);

  _initBlob();
}

void OsmPbfWriter::_writeMap()
{
  // Add the nodes and ways to the blob one at a time. When the blob gets sufficiently large it is
  // written to the output stream and a new blob is started.
  const NodeMap& nodes = _map->getNodes();
  vector<long> nids;
  nids.reserve(nodes.size());
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    nids.push_back((it->second)->getId());
  }
  sort(nids.begin(), nids.end());
  for (size_t i = 0; i < nids.size(); i++)
  {
    const ConstNodePtr& n = _map->getNode(nids[i]);
    _writeNodeDense(n);

    if (_enablePbFlushing && _tick % 100000 == 0 &&
        (uint32_t)_d->primitiveBlock.ByteSizeLong() > _minBlobTarget)
    {
      _writePrimitiveBlock();
    }
    _tick++;
  }

  const WayMap& ways = _map->getWays();
  vector<long> wids;
  wids.reserve(ways.size());

  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const std::shared_ptr<const hoot::Way>& w = it->second;
    wids.push_back(w->getId());
  }
  sort(wids.begin(), wids.end());

  for (size_t i = 0; i < wids.size(); i++)
  {
    const std::shared_ptr<const hoot::Way>& w = _map->getWay(wids[i]);
    _writeWay(w);

    if (_enablePbFlushing && _tick % 10000 == 0 && (uint32_t)_d->primitiveBlock.ByteSizeLong() > _minBlobTarget)
    {
      _writePrimitiveBlock();
    }
    _tick++;
  }

  // write out relations
  const RelationMap& relations = _map->getRelations();
  vector<long> rids;
  rids.reserve(relations.size());

  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    const ConstRelationPtr& r = it->second;
    rids.push_back(r->getId());
  }
  sort(rids.begin(), rids.end());

  for (size_t i = 0; i < rids.size(); i++)
  {
    const ConstRelationPtr& r = _map->getRelation(rids[i]);
    _writeRelation(r);

    if (_enablePbFlushing && _tick % 10000 == 0 && (uint32_t)_d->primitiveBlock.ByteSizeLong() > _minBlobTarget)
    {
      _writePrimitiveBlock();
    }
    _tick++;
  }
}

void OsmPbfWriter::_writeNode(const std::shared_ptr<const hoot::Node>& n)
{
  //  Ignore NULL elements
  if (!n) return;

  _elementsWritten++;
  if (_pg == nullptr)
  {
    _pg = _d->primitiveBlock.add_primitivegroup();
  }

  ::google::protobuf::RepeatedPtrField< ::hoot::pb::Node >* nodes =
    _pg->mutable_nodes();

  _dirty = true;

  pb::Node* newNode = nodes->Add();

  newNode->set_id(n->getId() + _nodeIdDelta);
  newNode->set_lon(_convertLon(n->getX()));
  newNode->set_lat(_convertLat(n->getY()));

  const Tags& tags = n->getTags();
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
    const QString& key = it.key();
    const QString& value = it.value();
    int kid = _convertString(key);
    int vid = _convertString(value);
    if (!value.isEmpty())
    {
      newNode->add_keys(kid);
      newNode->add_vals(vid);
    }
  }

  // if there are tags on the node then record the CE. CE isn't used as part of a way
  // at this point. Instead the way records the CE for the entire way. No need to waste disk.
  if (n->getTags().getNonDebugCount() > 0)
  {
    int kid = _convertString(MetadataTags::ErrorCircular());
    int vid = _convertString(QString::number(n->getCircularError()));
    newNode->add_keys(kid);
    newNode->add_vals(vid);

    if (n->getStatus() != Status::Invalid)
    {
      kid = _convertString(MetadataTags::HootStatus());
      vid = _convertString(QString::number(n->getStatus().getEnum()));
      newNode->add_keys(kid);
      newNode->add_vals(vid);
    }
  }
}

void OsmPbfWriter::_writeNodeDense(const std::shared_ptr<const hoot::Node>& n)
{
  //  Ignore NULL elements
  if (!n) return;

  LOG_TRACE("Writing node: " << n->getElementId() << "...");

  _elementsWritten++;
  if (_dn == nullptr)
  {
    _dn = _d->primitiveBlock.add_primitivegroup()->mutable_dense();
  }

  _dirty = true;

  // From http://wiki.openstreetmap.org/wiki/PBF_Format#Nodes
  // I store the group 'columnwise', as an array of ID's, array of latitudes, and array of
  // longitudes. Each column is delta-encoded. This reduces header overheads and allows
  // delta-coding to work very effectively.
  long lon = _convertLon(n->getX());
  long lat = _convertLat(n->getY());
  _dn->add_id(n->getId() + _nodeIdDelta - _lastId);
  _dn->add_lon(lon - _lastLon);
  _dn->add_lat(lat - _lastLat);
  _lastId = n->getId() + _nodeIdDelta;
  _lastLon = lon;
  _lastLat = lat;

  // From http://wiki.openstreetmap.org/wiki/PBF_Format#Nodes
  // Keys and values for all nodes are encoded as a single array of stringid's. Each node's tags
  // are encoded in alternating <keyid> <valid>. We use a single stringid of 0 to delimit when the
  // tags of a node ends and the tags of the next node begin. The storage pattern is:
  // ((<keyid> <valid>)* '0' )*
  const Tags& tags = n->getTags();
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
    const QString& key = it.key();
    const QString& value = it.value().trimmed();
    int kid = _convertString(key);
    int vid = _convertString(value);
    if (!value.isEmpty())
    {
      _dn->add_keys_vals(kid);
      _dn->add_keys_vals(vid);
    }
  }

  // if there are tags on the node then record the CE. CE isn't used as part of a way
  // at this point. Instead the way records the CE for the entire way. No need to waste disk.
  if (n->getTags().getNonDebugCount() > 0)
  {
    int kid = _convertString(MetadataTags::ErrorCircular());
    int vid = _convertString(QString::number(n->getCircularError()));
    _dn->add_keys_vals(kid);
    _dn->add_keys_vals(vid);

    if (n->getStatus() != Status::Invalid)
    {
      kid = _convertString(MetadataTags::HootStatus());
      vid = _convertString(QString::number(n->getStatus().getEnum()));
      _dn->add_keys_vals(kid);
      _dn->add_keys_vals(vid);
    }
  }

  _dn->add_keys_vals(0);
}

void OsmPbfWriter::_writeOsmHeader(bool includeBounds, bool sorted)
{
  LOG_TRACE("Writing the OSM header...");
  LOG_VART(includeBounds);
  LOG_VART(sorted);

  // create the header block
  _d->headerBlock.Clear();

  LOG_VART(includeBounds);
  LOG_VART(_map.get());
  if (includeBounds && _map.get())
  {
    const OGREnvelope& env = CalculateMapBoundsVisitor::getBounds(_map);
    _d->headerBlock.mutable_bbox()->set_bottom(env.MinY);
    _d->headerBlock.mutable_bbox()->set_left(env.MinX);
    _d->headerBlock.mutable_bbox()->set_right(env.MaxX);
    _d->headerBlock.mutable_bbox()->set_top(env.MaxY);
  }
  else
  {
    _d->headerBlock.clear_bbox();
  }

  _d->headerBlock.mutable_required_features()->Add()->assign(PBF_OSM_SCHEMA_V06);
  _d->headerBlock.mutable_required_features()->Add()->assign(PBF_DENSE_NODES);

  LOG_VART(sorted);
  if (sorted)
  {
    _d->headerBlock.mutable_optional_features()->Add()->assign(PBF_SORT_TYPE_THEN_ID);
  }
  LOG_VART(_includeVersion);
  if (_includeVersion)
  {
    _d->headerBlock.mutable_writingprogram()->assign(HOOT_FULL_VERSION);
  }
  else
  {
    _d->headerBlock.mutable_writingprogram()->assign(HOOT_NAME);
  }

  int size = (uint32_t)_d->headerBlock.ByteSizeLong();
  LOG_VART(size);
  _d->headerBlock.SerializePartialToArray(_getBuffer(size), size);
  _writeBlob(_buffer.data(), size, PBF_OSM_HEADER);
}

void OsmPbfWriter::writePartial(const ConstOsmMapPtr& map)
{
  _map = map;
  _writeMap();
  _map.reset();
}

void OsmPbfWriter::writePartial(const ConstNodePtr& n)
{
  _writeNodeDense(n);

  if (_enablePbFlushing && _tick % 100000 == 0 && (uint32_t)_d->primitiveBlock.ByteSizeLong() > _minBlobTarget)
  {
    _writePrimitiveBlock();
  }
  _tick++;
}

void OsmPbfWriter::writePartial(const ConstWayPtr& w)
{
  _writeWay(w);

  if (_enablePbFlushing && _tick % 10000 == 0 && (uint32_t)_d->primitiveBlock.ByteSizeLong() > _minBlobTarget)
  {
    _writePrimitiveBlock();
  }
  _tick++;
}

void OsmPbfWriter::writePartial(const ConstRelationPtr& r)
{
  _writeRelation(r);

  if (_enablePbFlushing && _tick % 10000 == 0 && (uint32_t)_d->primitiveBlock.ByteSizeLong() > _minBlobTarget)
  {
    _writePrimitiveBlock();
  }
  _tick++;
}

void OsmPbfWriter::_writePrimitiveBlock()
{
  if (_dirty)
  {
    LOG_DEBUG("Writing primitive block...");
    int size = (uint32_t)_d->primitiveBlock.ByteSizeLong();
    _d->primitiveBlock.SerializePartialToArray(_getBuffer(size), size);
    _writeBlob(_buffer.data(), size, PBF_OSM_DATA);
  }
}

void OsmPbfWriter::_writeRelation(const std::shared_ptr<const hoot::Relation>& r)
{
  //  Ignore NULL elements
  if (!r) return;

  LOG_TRACE("Writing relation: " << r->getElementId() << "...");

  _elementsWritten++;

  if (_pg == nullptr)
  {
    _pg = _d->primitiveBlock.add_primitivegroup();
  }

  pb::Relation* pbr = _pg->add_relations();

  pbr->set_id(r->getId() + _relationIdDelta);

  // From http://wiki.openstreetmap.org/wiki/PBF_Format#Ways_and_Relations
  // For ways and relations, which contain the ID's of other nodes, I exploit the tendency of
  // consecutive nodes in a way or relation to have nearby node ID's by using delta compression,
  // resulting in small integers. (I.E., instead of encoding x_1, x_2, x_3, I encode
  // x_1, x_2-x_1, x_3-x_2, ...).
  long lastId = 0;
  const vector<RelationData::Entry>& entries = r->getMembers();
  for (size_t i = 0; i < entries.size(); i++)
  {
    long id = entries[i].getElementId().getId() + _nodeIdDelta;
    pbr->add_memids(id - lastId);
    lastId = id + _nodeIdDelta;
    pbr->add_types((hoot::pb::Relation_MemberType)
                   _toRelationMemberType(entries[i].getElementId().getType()));
    pbr->add_roles_sid(_convertString(entries[i].getRole()));
  }

  // From http://wiki.openstreetmap.org/wiki/PBF_Format#Ways_and_Relations
  // Tags are encoded as two parallel arrays, one array of string-id's of the keys, and the other
  // of string-id's of the values.
  const Tags& tags = r->getTags();
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
    const QString& key = it.key();
    const QString& value = it.value().trimmed();
    //LOG_VART(key);
    //LOG_VART(value);
    if (!value.isEmpty())
    {
      pbr->add_keys(_convertString(key));
      pbr->add_vals(_convertString(value));
    }
  }
  int kid = _convertString(MetadataTags::ErrorCircular());
  int vid = _convertString(QString::number(r->getCircularError()));
  pbr->add_keys(kid);
  pbr->add_vals(vid);

  if (r->getStatus() != Status::Invalid)
  {
    kid = _convertString(MetadataTags::HootStatus());
    vid = _convertString(QString::number(r->getStatus().getEnum()));
    pbr->add_keys(kid);
    pbr->add_vals(vid);
  }

  //don't see anywhere in the pbf spec to store a relation type, so adding it to the tags
  kid = _convertString(MetadataTags::RelationType());
  vid = _convertString(r->getType());
  pbr->add_keys(kid);
  pbr->add_vals(vid);

  _dirty = true;
}

void OsmPbfWriter::_writeWay(const std::shared_ptr<const hoot::Way>& w)
{
  //  Ignore NULL elements
  if (!w) return;

  LOG_TRACE("Writing way: " << w->getElementId() << "...");

  _elementsWritten++;

  if (_pg == nullptr)
  {
    _pg = _d->primitiveBlock.add_primitivegroup();
  }

  pb::Way* pbw = _pg->add_ways();

  pbw->set_id(w->getId() + _wayIdDelta);

  // if the cached envelope is valid
  const Envelope& env = w->getApproximateEnvelope(_map);
  if (env.isNull() == false)
  {
    // write the way's approximate bounding box. This is a custom Hoot extension to the format.
    pbw->mutable_bbox()->set_left(_convertLon(env.getMinX()));
    pbw->mutable_bbox()->set_right(_convertLon(env.getMaxX()));
    pbw->mutable_bbox()->set_top(_convertLat(env.getMaxY()));
    pbw->mutable_bbox()->set_bottom(_convertLat(env.getMinY()));
  }

  // From http://wiki.openstreetmap.org/wiki/PBF_Format#Ways_and_Relations
  // For ways and relations, which contain the ID's of other nodes, I exploit the tendency of
  // consecutive nodes in a way or relation to have nearby node ID's by using delta compression,
  // resulting in small integers. (I.E., instead of encoding x_1, x_2, x_3, I encode
  // x_1, x_2-x_1, x_3-x_2, ...).
  long lastId = 0;
  const std::vector<long>& ids = w->getNodeIds();
  for (size_t i = 0; i < ids.size(); i++)
  {
    pbw->add_refs(ids[i] + _nodeIdDelta - lastId);
    lastId = ids[i] + _nodeIdDelta;
  }

  // From http://wiki.openstreetmap.org/wiki/PBF_Format#Ways_and_Relations
  // Tags are encoded as two parallel arrays, one array of string-id's of the keys, and the other
  // of string-id's of the values.
  const Tags& tags = w->getTags();
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
    const QString& key = it.key();
    const QString& value = it.value().trimmed();
    if (!value.isEmpty())
    {
      pbw->add_keys(_convertString(key));
      pbw->add_vals(_convertString(value));
    }
  }
  int kid = _convertString(MetadataTags::ErrorCircular());
  int vid = _convertString(QString::number(w->getCircularError()));
  pbw->add_keys(kid);
  pbw->add_vals(vid);

  if (w->getStatus() != Status::Invalid)
  {
    kid = _convertString(MetadataTags::HootStatus());
    vid = _convertString(QString::number(w->getStatus().getEnum()));
    pbw->add_keys(kid);
    pbw->add_vals(vid);
  }

  _dirty = true;

}

}
