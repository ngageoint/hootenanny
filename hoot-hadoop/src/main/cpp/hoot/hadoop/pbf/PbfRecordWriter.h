/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef PBFRECORDWRITER_H
#define PBFRECORDWRITER_H

// Boost
#include <boost/shared_ptr.hpp>

// Pretty Pipes
#include <pp/ReduceContextConsumer.h>
#include <pp/mapreduce/RecordWriter.h>

// Std
#include <ostream>

#include <hoot/core/io/OsmPbfWriter.h>

namespace hoot
{

class Node;
class Way;
class OsmMap;

using namespace boost;

/**
 * Wraps another record writer to enable writing of OSM data types.
 */
class PbfRecordWriter : public pp::RecordWriter, public pp::ReduceContextConsumer
{
public:

  static string className() { return "hoot::PbfRecordWriter"; }

  PbfRecordWriter();

  virtual ~PbfRecordWriter();

  virtual void close();

  virtual void emit(const string&, const string&);

  void emitRecord(boost::shared_ptr<OsmMap> map);

  void emitRecord(const boost::shared_ptr<const Node>& n);

  void emitRecord(const boost::shared_ptr<const Way>& w);

  OsmPbfWriter& getOsmPbfWriter() { return *_OsmPbfWriter; }

  void includeVersion(bool iv) { _OsmPbfWriter->includVersion(iv); }

  void setCompressionLevel(int z) { _OsmPbfWriter->setCompressionLevel(z); }

  virtual void setMapContext(int part, string workDir);

  virtual void setReduceContext(HadoopPipes::ReduceContext& context);

  virtual void setReduceContext(int part, string workDir);

private:
  OsmPbfWriter* _OsmPbfWriter;
  string _path;
 boost::shared_ptr<ostream> _out;
};

}

#endif // PBFRECORDWRITER_H
