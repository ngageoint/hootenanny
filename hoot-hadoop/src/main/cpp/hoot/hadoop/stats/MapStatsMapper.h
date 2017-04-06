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

#ifndef MAP_STATS_MAPPER_H
#define MAP_STATS_MAPPER_H

// Hoot
#include "MapStats.h"
#include <hoot/hadoop/OsmMapMapper.h>

namespace hoot
{
class OsmMap;

using namespace boost;

class MapStatsMapper : public OsmMapMapper
{
public:

  static std::string className() { return "hoot::MapStatsMapper"; }

  virtual void close();

  MapStatsMapper();

protected:

  MapStats _stats;
  long _nodeCount;
  long _wayCount;

  HadoopPipes::MapContext* _context;

  virtual void _map(boost::shared_ptr<OsmMap>& m, HadoopPipes::MapContext& context);

  void _writeStats(HadoopPipes::MapContext& context, const MapStats& stats);

};

}

#endif // MAP_STATS_MAPPER_H
