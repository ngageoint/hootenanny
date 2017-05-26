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

#include "MapStatsMapper.h"

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/Hdfs.h>

using namespace std;

namespace hoot
{

PP_FACTORY_REGISTER(pp::Mapper, MapStatsMapper)

MapStatsMapper::MapStatsMapper()
{
  _nodeCount = 0;
  _wayCount = 0;
}

void MapStatsMapper::close()
{
  _writeStats(*_context, _stats);
}

void MapStatsMapper::_map(OsmMapPtr& m, HadoopPipes::MapContext& context)
{
  _context = &context;

  const NodeMap& nm = m->getNodes();
  for (NodeMap::const_iterator it = nm.begin(); it != nm.end(); ++it)
  {
    const ConstNodePtr& n = it->second;

    _stats.expandNodeRange(n);
    _nodeCount++;
  }

  const WayMap& wm = m->getWays();
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    const ConstWayPtr& w = it->second;
    _stats.expandWayRange(w->getId());
    _wayCount++;
  }
}

void MapStatsMapper::_writeStats(HadoopPipes::MapContext& context, const MapStats& stats)
{
  LOG_INFO("node count: " << _nodeCount);
  LOG_INFO("way count: " << _wayCount);
  if (stats.isValid())
  {
    pp::Hdfs fs;
    int partition = context.getJobConf()->getInt("mapred.task.partition");
    string workDir = context.getJobConf()->get("mapred.work.output.dir");

    LOG_INFO("Stats: " << stats.toString());

    QString path = QString("%1/part-%2.stats").
        arg(QString::fromStdString(workDir)).
        arg(partition, 5, 10, QChar('0'));

    LOG_INFO("Writing to: " << path);
    boost::shared_ptr<ostream> osStats(fs.create(path.toStdString()));

    stats.write(*osStats);
  }
  else
  {
    LOG_INFO("Stats are not valid.");
  }
}

}

