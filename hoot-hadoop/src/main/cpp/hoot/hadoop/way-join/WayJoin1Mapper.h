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

#ifndef WAYJOIN1MAPPER_H
#define WAYJOIN1MAPPER_H

// Hoot
#include <hoot/hadoop/OsmMapMapper.h>

namespace hoot
{
class OsmMap;

using namespace boost;

class WayJoin1Mapper : public OsmMapMapper
{
public:

  // The data in the value field for a way. The size of the field will discriminate it from a
  // ValueNode.
  struct ValueWay
  {
    int64_t id;
  };

  // The data in the value field for a node. The size of the field will discriminate it from a
  // ValueNode.
  struct ValueNode
  {
    double x;
    double y;
  };

  static std::string className() { return "hoot::WayJoin1Mapper"; }

  WayJoin1Mapper();

  virtual ~WayJoin1Mapper();

  virtual void close() {}

protected:

  virtual void _map(boost::shared_ptr<OsmMap>& m, HadoopPipes::MapContext& context);

};

}

#endif // WAYJOIN1MAPPER_H
