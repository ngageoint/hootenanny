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

#include "WayJoin1Reducer.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/hadoop/Debug.h>

// Pretty Pipes
#include <pp/DataOutputStream.h>
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>
#include <pp/Hdfs.h>

// Standard
#include <limits>

#include "WayJoin1Mapper.h"

using namespace std;

namespace hoot
{

unsigned int WayJoin1Reducer::logWarnCount = 0;

PP_FACTORY_REGISTER(pp::Reducer, WayJoin1Reducer)

WayJoin1Reducer::WayJoin1Reducer()
{
  _writer = NULL;
  _missingNodes = 0;
}

void WayJoin1Reducer::close()
{
  pp::Hdfs fs;

  QString path = QString("%1/part-%2.stats").
      arg(QString::fromStdString(_workDir)).
      arg(_partition, 5, 10, QChar('0'));

  boost::shared_ptr<ostream> os(fs.create(path.toStdString()));

  if (_missingNodes > 0)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Found " << _missingNodes << " missing nodes.");
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }
  _stats.write(*os);
}

void WayJoin1Reducer::reduce(HadoopPipes::ReduceContext& context)
{
  if (_writer == NULL)
  {
    HadoopPipes::RecordWriter* writer = pp::HadoopPipesUtils::getRecordWriter(&context);
    _writer = dynamic_cast<pp::RecordWriter*>(writer);
    if (_writer == NULL)
    {
      throw HootException("Error getting RecordWriter.");
    }
    _partition = context.getJobConf()->getInt("mapred.task.partition");
    _workDir = context.getJobConf()->get("mapred.work.output.dir");
  }

  const string& keyStr = context.getInputKey();
  int64_t* key = (int64_t*)keyStr.data();

  bool foundNode = false;
  Value point;
  _wayIds.resize(0);
  point.nodeId = *key;

  _stats.expandNodeRange(*key);

  while (context.nextValue())
  {
    const string& value = context.getInputValue();

    if (value.size() == sizeof(WayJoin1Mapper::ValueNode))
    {
      WayJoin1Mapper::ValueNode* node = (WayJoin1Mapper::ValueNode*)value.data();
      point.x = node->x;
      point.y = node->y;
      _stats.expandEnvelope(point.x, point.y);
      LOG_TRACE("Got node: " << " x: " << point.x << " y: " << point.y);
      foundNode = true;
    }
    else if (value.size() == sizeof(WayJoin1Mapper::ValueWay))
    {
      WayJoin1Mapper::ValueWay* way = (WayJoin1Mapper::ValueWay*)value.data();
      _wayIds.push_back(way->id);

      _stats.expandWayRange(way->id);
      LOG_TRACE("Got way: " << way->id);
    }
    else
    {
      throw HootException("Got unrecognized record.");
    }
  }

# ifdef DEBUG
    if (Debug::isTroubledNode(point.nodeId))
    {
      LOG_WARN("Found troubled node: " << point.nodeId);
      LOG_WARN("  foundNode: " << foundNode);
      LOG_WARN("  wayIds: " << _wayIds);
    }
    for (size_t i = 0; i < _wayIds.size(); i++)
    {
      if (Debug::isTroubledWay(_wayIds[i]))
      {
        LOG_WARN("Found troubled way: " << _wayIds[i]);
        LOG_WARN("  node: " << point.nodeId)
      }
    }
# endif

  // if we didn't find a node to go with our ways. This usually happens with bad input files, but
  // is not uncommon, especially around the border of the input.
  if (foundNode == false)
  {
    // if we found some ways, but no node.
    if (_wayIds.size() > 0)
    {
      // record the number of missing nodes for reporting.
      _missingNodes++;
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Found ways, but no nodes. node id: " << *key << "  wayIds: " << _wayIds);
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
  }
  // we found a node to go with our ways.
  else
  {
    for (size_t i = 0; i < _wayIds.size(); i++)
    {
      LOG_TRACE("emitting: " << _wayIds[i] << " nodeId: " << point.nodeId << " x: " << point.x <<
                " y: " << point.y);
      _writer->emitRaw<int64_t, Value>(_wayIds[i], point);
    }
  }
}

}
