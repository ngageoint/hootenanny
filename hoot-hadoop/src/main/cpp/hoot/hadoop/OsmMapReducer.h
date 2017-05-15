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

#ifndef OSMMAPREDUCER_H
#define OSMMAPREDUCER_H

#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <hadoop/Pipes.hh>
#pragma GCC diagnostic warning "-Wunused-parameter"

// Hoot
#include <hoot/core/io/OsmPbfReader.h>

// Pretty Pipes
#include <pp/util/Iterator.h>

namespace hoot
{

class OsmMap;
class PbfRecordWriter;

/**
 * This class is useful for reducers that take OsmMap elements as input and produce a OsmMap as
 * output. The PbfRecordWriter must be used with classes that sublcass OsmMapReducer.
 */
class OsmMapReducer : public HadoopPipes::Reducer
{
public:
  OsmMapReducer();

  virtual void reduce(HadoopPipes::ReduceContext& context);

  virtual void reduce(const std::string& key, pp::Iterator<OsmMapPtr >& values,
    HadoopPipes::ReduceContext& context) = 0;

  PbfRecordWriter* getPbfRecordWriter();

protected:
  HadoopPipes::ReduceContext* _context;

private:
  PbfRecordWriter* _OsmPbfWriter;
  PbfRecordWriter* _writer;
  OsmPbfReader _reader;
  OsmMapPtr _map;
};

}

#endif // OSMMAPREDUCER_H
