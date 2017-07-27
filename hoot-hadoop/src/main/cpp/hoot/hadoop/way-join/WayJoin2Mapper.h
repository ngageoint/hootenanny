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

#ifndef WAYJOIN2MAPPER_H
#define WAYJOIN2MAPPER_H

// Pretty Pipes
#include <pp/mapreduce/Mapper.h>

// Standard
#include <string>

#include "WayJoin2RecordReader.h"

namespace hoot
{

class WayJoin2Mapper : public pp::Mapper
{
public:

  enum
  {
    NodesType = 0,
    WayType
  };

  enum
  {
    PbfData = 0,
    RawData
  };

  struct KeyStruct
  {
    char elementType;
    int64_t id;
  } __attribute__((packed));

  struct RawValueStruct
  {
    char dataType;
    WayJoin1Reducer::Value rawWay;
  } __attribute__((packed));

  static std::string className() { return "hoot::WayJoin2Mapper"; }

  static std::string elementStatusKey() { return "hoot.element.status"; }

  static std::string nodeIdDeltaKey() { return "hoot.node.id.delta"; }

  static std::string wayIdDeltaKey() { return "hoot.way.id.delta"; }

  static std::string relationIdDeltaKey() { return "hoot.relation.id.delta"; }

  WayJoin2Mapper();

  // Write out any pending data.
  virtual void close() { _flushNodes(); }

  void map(HadoopPipes::MapContext& context);

  void mapOsmMap(boost::shared_ptr<OsmMap> map);

  void mapWayPoints(int64_t& k, WayJoin1Reducer::Value& v);

private:
  WayJoin2RecordReader* _reader;
  std::string _rawValueStr;
  RawValueStruct* _rawValue;

  std::string _keyStr;
  KeyStruct* _key;

  boost::shared_ptr<OsmMap> _nodeMap;

  HadoopPipes::MapContext* _context;

  void _emitNode(const boost::shared_ptr<Node>& n);

  void _flushNodes();
};

}

#endif // WAYJOIN2MAPPER_H
