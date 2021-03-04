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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef OSMPBFWRITER_H
#define OSMPBFWRITER_H

// Qt
#include <QHash>
#include <QString>

// Standard Includes
#include <istream>
#include <vector>

// TGS
#include <tgs/HashMap.h>

// hoot
#include <hoot/core/io/PartialOsmMapWriter.h>

namespace hoot
{
  namespace pb
  {
    class DenseNodes;
    class Node;
    class PrimitiveGroup;
    class Way;
  }

class OsmPbfWriterData;

/**
 * A writer for http://wiki.openstreetmap.org/wiki/PBF_Format
 */
class OsmPbfWriter : public PartialOsmMapWriter
{
public:

  static QString className() { return "hoot::OsmPbfWriter"; }

  class BlobLocation
  {
  public:
    long offset;
    long size;
  };

  static const char* const OSM_DATA;
  static const char* const OSM_HEADER;

  OsmPbfWriter();
  virtual ~OsmPbfWriter();

  /**
   * Used to finalize a call to writePartial.
   */
  void finalizePartial() override;

  /**
   * Returns the number of elements written by this class since it was instantiated.
   */
  int getElementsWritten() { return _elementsWritten; }

  /**
   * Removes the version number from the output. This is only useful for unit testing.
   */
  void includVersion(bool iv) { _includeVersion = iv; }

  void initializePartial(std::ostream* strm);

  void initializePartial() override;

  bool isSupported(const QString& url) override { return url.toLower().endsWith("osm.pbf"); }

  void open(const QString& url) override;
  void close() override;

  /**
   * Set the compression level to a value of -1 to 9. -1 is "default" or equivalent to about 7.
   * 0 is uncompressed. 1 is fastest. 9 is best compression.
   */
  void setCompressionLevel(int z) { _compressionLevel = z; }

  /**
   * Sets a value that should be added to nodes and ways before they're written out. This is
   * handy when all values need to be renumbered to avoid conflicts.
   */
  void setIdDelta(long nodeIdDelta, long wayIdDelta, long relationIdDelta);

  /**
   * Sets the granularity to 1 or higher. The granuarlity is in units of nanodegrees. The larger
   * the value the coarser the data stored, but potentially higher compression. The default of
   * 100nm introduces about 8.5mm of error on average and 15.5mm of error maximum with 2000 random
   * points.
   */
  void setGranularity(int granularity) { _granularity = granularity; }

  /**
   * The write command called after open.
   */
  void write(const ConstOsmMapPtr& map) override;

  void write(const ConstOsmMapPtr& map, const QString& path);

  void write(const ConstOsmMapPtr& map, std::ostream* strm);

  /**
   * Writes the head out to the specified output stream. This is useful when manually composing
   * a PBF out of various parts and pieces.
   */
  void writeHeader(std::ostream* strm, bool includeBounds = true, bool sorted = true);

  /**
   * Write out a map in chunks. This may be called multiple times and must be precceded with a
   * call to initializePartial and finalized with a call to finalizePartial.
   */
  void writePartial(const ConstOsmMapPtr& map) override;
  void writePartial(const ConstNodePtr& n) override;
  void writePartial(const ConstWayPtr& w) override;
  void writePartial(const ConstRelationPtr& r) override;

  /**
   * Write out the map as a PrimitiveBlock to the specified stream. The size of the primitive
   * block will first be written as a network order uint32_t
   */
  void writePb(const ConstOsmMapPtr& map, std::ostream* strm);
  void writePb(const OsmMapPtr& map, std::ostream* strm) { writePb((const ConstOsmMapPtr)map, strm); }

  /**
   * Write a single node out as a PrimitiveBlock to the specified stream. The size of the primitive
   * block will first be written as a network order uint32_t
   */
  void writePb(const ConstNodePtr& n, std::ostream* strm);
  void writePb(const NodePtr& n, std::ostream* strm) { writePb((const ConstNodePtr)n, strm); }

  /**
   * Write a single way out as a PrimitiveBlock to the specified stream. The size of the primitive
   * block will first be written as a network order uint32_t
   */
  void writePb(const ConstWayPtr& w, std::ostream* strm);
  void writePb(const WayPtr& w, std::ostream* strm) { writePb((const ConstWayPtr)w, strm); }

  /**
   * Write a single way out as a PrimitiveBlock to the specified stream. The size of the primitive
   * block will first be written as a network order uint32_t
   */
  void writePb(const ConstRelationPtr& r, std::ostream* strm);
  void writePb(const RelationPtr& r, std::ostream* strm) { writePb((const ConstRelationPtr)r, strm); }

  QString supportedFormats() override { return ".osm.pbf"; }

private:

  std::string _buffer;
  int _compressionLevel;
  std::string _deflateBuffer;
  int _deflateSize;
  bool _dirty;
  std::ostream* _out;
  bool _includeInfo;
  bool _includeVersion;
  // Bend over backwards to keep the PBF headers out of the normal build. They're quite large.
  OsmPbfWriterData* _d;
  QHash<QString, int> _strings;
  ConstOsmMapPtr _map;
  int _rawSize;
  pb::DenseNodes* _dn;
  pb::PrimitiveGroup* _pg;
  long _lastId;
  long _lastLon;
  long _lastLat;
  long _lastWayNid;
  long _minBlobTarget, _maxBlobTarget;

  int _granularity;
  long _latOffset;
  long _lonOffset;
  long _dateGranularity;
  long _tick;
  // when set to true the primitive buffer will be flushed periodically to keep the right blob
  // size.
  bool _enablePbFlushing;
  int _elementsWritten;

  long _wayIdDelta;
  long _nodeIdDelta;
  long _relationIdDelta;

  std::shared_ptr<std::fstream> _openStream;
  bool _needToCloseInput;

  void _addTag(const std::shared_ptr<Element>& n, const QString& k, const QString& v);

  long _convertLon(double lon);

  long _convertLat(double lat);

  int _convertString(const QString& s);

  void _deflate(const char* raw, size_t rawSize);

  char* _getBuffer(size_t size);

  void _initBlob();

  int _toRelationMemberType(ElementType t);

  void _writeBlob(const char* buffer, int size, const std::string& type);

  /**
   * Write out the guts of the map.
   */
  void _writeMap();

  /**
   * Writes a node using the non-dense format.
   */
  void _writeNode(const std::shared_ptr<const hoot::Node>& n);

  void _writeNodeDense(const std::shared_ptr<const hoot::Node>& n);

  void _writeOsmHeader(bool includeBounds = true, bool sorted = true);

  void _writePrimitiveBlock();

  void _writeRelation(const std::shared_ptr<const hoot::Relation>& r);

  void _writeWay(const std::shared_ptr<const hoot::Way>& w);

  void _open(const QString& url);
};

}

#endif // OSMPBFWRITER_H
