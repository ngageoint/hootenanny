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

#ifndef OSMMAPMAPPER_H
#define OSMMAPMAPPER_H

// Boost
#include <boost/shared_ptr.hpp>

// Hoot
#include <hoot/core/io/OsmPbfWriter.h>

// Pretty Pipes
#include <pp/mapreduce/Mapper.h>

namespace hoot
{

class OsmMap;

class OsmMapMapper : public pp::Mapper
{
public:

  OsmMapMapper() : _context(NULL) {}

  void map(HadoopPipes::MapContext& context);

  void emitRecord(HadoopPipes::MapContext& context, const string& k,
    const boost::shared_ptr<const OsmMap>& m)
  {
    stringstream ss(stringstream::out);
    _OsmPbfWriter.writePb(m, &ss);
    context.emit(k, ss.str());
  }

  void emitRecord(HadoopPipes::MapContext& context, const string& k, const boost::shared_ptr<const Way>& w)
  {
    stringstream ss(stringstream::out);
    _OsmPbfWriter.writePb(w, &ss);
    context.emit(k, ss.str());
  }

  void emitRecord(HadoopPipes::MapContext& context, const string& k,
    const boost::shared_ptr<const Node>& n)
  {
    stringstream ss(stringstream::out);
    _OsmPbfWriter.writePb(n, &ss);
    context.emit(k, ss.str());
  }

protected:
  string _path;
  long _start;
  OsmPbfWriter _OsmPbfWriter;
  HadoopPipes::MapContext* _context;

  void _loadMap(boost::shared_ptr<OsmMap>& map);

  virtual void _map(boost::shared_ptr<OsmMap>& m, HadoopPipes::MapContext& context) = 0;

};

}

#endif // OSMMAPMAPPER_H
