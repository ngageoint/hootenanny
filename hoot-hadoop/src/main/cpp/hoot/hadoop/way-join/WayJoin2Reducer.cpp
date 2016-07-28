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

#include "WayJoin2Reducer.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/io/PbfReader.h>
#include <hoot/core/ops/RemoveNodeOp.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/hadoop/HadoopIdGenerator.h>
#include <hoot/hadoop/Debug.h>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>
#include <pp/Hdfs.h>

#include "WayJoin2Mapper.h"

namespace hoot
{

PP_FACTORY_REGISTER(pp::Reducer, WayJoin2Reducer)

WayJoin2Reducer::WayJoin2Reducer()
{
  _writer = NULL;
  _map.reset(new OsmMap());
  _strict = false;
  _wayCount = 0;
}

void WayJoin2Reducer::close()
{
  if (_stats.isValid())
  {
    pp::Hdfs fs;

    QString path = QString("%1/part-%2.stats").
        arg(QString::fromStdString(_workDir)).
        arg(_partition, 5, 10, QChar('0'));

    shared_ptr<ostream> os(fs.create(path.toStdString()));

    _stats.expandNodeRange(_idGen->getMaxNodeId());
    _stats.expandWayRange(_idGen->getMaxWayId());

    // update the stats to use the new delta.
    MapStats outputStats;
    outputStats.expandNodeRange(_stats.getMinNodeId() + _nodeIdDelta);
    outputStats.expandNodeRange(_stats.getMaxNodeId() + _nodeIdDelta);
    outputStats.expandWayRange(_stats.getMaxWayId() + _wayIdDelta);
    outputStats.expandWayRange(_stats.getMinWayId() + _wayIdDelta);
    outputStats.expandRelationRange(_stats.getMaxWayId() + _relationIdDelta);
    outputStats.expandRelationRange(_stats.getMinWayId() + _relationIdDelta);
    outputStats.write(*os);
    LOG_INFO("Output stats: " << outputStats.toString());
  }
  LOG_INFO("Way count: " << _wayCount);
}

void WayJoin2Reducer::reduce(HadoopPipes::ReduceContext& context)
{
  if (_writer == NULL)
  {
    HadoopPipes::RecordWriter* writer = pp::HadoopPipesUtils::getRecordWriter(&context);
    _writer = dynamic_cast<PbfRecordWriter*>(writer);
    if (_writer == NULL)
    {
      throw InternalErrorException("Error getting RecordWriter.");
    }

    // read out the maximum way size from the context.
    shared_ptr<pp::Configuration> c(pp::HadoopPipesUtils::toConfiguration(context.getJobConf()));
    _stats.read(*c);

    _nodeIdDelta = c->getLong(WayJoin2Mapper::nodeIdDeltaKey());
    _wayIdDelta = c->getLong(WayJoin2Mapper::wayIdDeltaKey());
    _relationIdDelta = c->getLong(WayJoin2Mapper::relationIdDeltaKey());
    LOG_INFO("wayIdDelta: " << _wayIdDelta
             << " nodeIdDelta: " << _nodeIdDelta
             << " relationIdDelta: " << _relationIdDelta);
    _newStatus = (Status::Type)c->getInt(WayJoin2Mapper::elementStatusKey());
    // add to the way/node ids to avoid conflicts w/ another map.
    _writer->getPbfWriter().setIdDelta(_nodeIdDelta, _wayIdDelta, _relationIdDelta);

    _partition = context.getJobConf()->getInt("mapred.task.partition");
    _workDir = context.getJobConf()->get("mapred.work.output.dir");
    int taskCount = context.getJobConf()->getInt("mapred.reduce.tasks");
    // create a new id generator and use it with our map.
    _idGen.reset(new HadoopIdGenerator(_partition, taskCount, _stats.getMaxWayId(),
      _stats.getMaxNodeId()));
    _map->setIdGenerator(_idGen);
    _maxWaySize = c->getDouble("hoot.max.way.size");
  }

  const string& keyStr = context.getInputKey();
  WayJoin2Mapper::KeyStruct* key = (WayJoin2Mapper::KeyStruct*)keyStr.data();

  // if it is a group of nodes
  if (key->elementType == WayJoin2Mapper::NodesType)
  {
    // write the nodes.
    _writeNodes(context);
  }
  // if it is a way
  else
  {
    // Determine the bounding box and write the way.
    _writeWay(context);
  }
}

void WayJoin2Reducer::_writeNodes(HadoopPipes::ReduceContext& context)
{
  while (context.nextValue())
  {
    LOG_INFO("In _writeNodes");
    // write the nodes.
    stringstream ss(context.getInputValue(), stringstream::in);
    char type;
    ss.read(&type, 1);
    assert(type == WayJoin2Mapper::PbfData);
    _map->clear();
    PbfReader reader(true);
    reader.setDefaultStatus(_newStatus);
    reader.parseElements(&ss, _map);

    Debug::printTroubled(_map);

    LOG_INFO("map data size: " << context.getInputValue().size());
    LOG_INFO("Writing nodes: " << _map->getNodeMap().size());
    _writer->emitRecord(_map);
    LOG_INFO("Wrote nodes.");
  }
}

void WayJoin2Reducer::_writeWay(HadoopPipes::ReduceContext& context)
{
  shared_ptr<Way> w;
  Envelope env;

  _map->clear();

  set<long> rawNodes;
  while (context.nextValue())
  {
    const string& value = context.getInputValue();
    char type = value.at(0);

    if (type == WayJoin2Mapper::PbfData)
    {
      stringstream ss(value, stringstream::in);
      ss.read(&type, 1);
      PbfReader reader(true);
      reader.parseElements(&ss, _map);
      const WayMap& wm = _map->getWays();
      WayMap::const_iterator it = wm.begin();
      w = it->second;
      w->setStatus(_newStatus);
      _wayCount++;
      ++it;
      if (it != wm.end())
      {
        throw InternalErrorException("Internal Error: Only expecting one way.");
      }
    }
    // parse out the nodes that make up this way
    else if (type == WayJoin2Mapper::RawData)
    {
      // build up the points on the way with temporary nodes.
      WayJoin2Mapper::RawValueStruct* v = (WayJoin2Mapper::RawValueStruct*)value.data();
      long nid = v->rawWay.nodeId;
      rawNodes.insert(nid);
      // create an invalid placeholder node. We'll remove it later.
      shared_ptr<Node> n(new Node(Status::Invalid, nid, v->rawWay.x, v->rawWay.y, 0.0));
      _map->addNode(n);
      env.expandToInclude(v->rawWay.x, v->rawWay.y);
      //LOG_INFO("Got node: " << n->toString());
    }
    else
    {
      throw InternalErrorException("Internal Error: Expecting PbfData or RawData type.");
    }
  }

  if (Debug::isTroubledWay(w->getId()))
  {
    LOG_WARN("Found troubled way: " << w->toString());
  }

  if (Debug::printTroubled(_map))
  {
    LOG_WARN("Troubled nodes/ways are in _writeWay");
  }

  /// @todo make it prettier, break into functions
  set<long> nodeIds;
  const vector<long>& nids = w->getNodeIds();
  nodeIds.insert(nids.begin(), nids.end());
  // if the way has all the intended nodes.
  if (_map->getNodeMap().size() == nodeIds.size())
  {
    QList<long> tempNodes;
    for (NodeMap::const_iterator it = _map->getNodeMap().begin();
      it != _map->getNodeMap().end(); ++it)
    {
      tempNodes.append(it->first);
    }

    // if the envelope is larger than the max way threshold
    if (_maxWaySize > 0 && (env.getWidth() > _maxWaySize || env.getHeight() > _maxWaySize))
    {
      WaySplitter::split(_map, w, _maxWaySize);
    }

    const WayMap& wm = _map->getWays();
    for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
    {
      const shared_ptr<Way>& way = it->second;

      if (way->getId() != w->getId() && Debug::isTroubledWay(way->getId()))
      {
        LOG_WARN("Found troubled way: " << way->toString());
      }

      const Envelope& e = way->getEnvelopeInternal(_map);
//  #warning debug envelope is on.
//      QString eStr = QString("%1,%2,%3,%4").
//          arg(e.getMinX(), 0, 'g', 9).
//          arg(e.getMinY(), 0, 'g', 9).
//          arg(e.getMaxX(), 0, 'g', 9).
//          arg(e.getMaxY(), 0, 'g', 9);
//      way->setTag("bbox", eStr);
//      LOG_VAR(eStr);
      if (e.getWidth() > _maxWaySize || e.getHeight() > _maxWaySize)
      {
        LOG_ERROR("way too big. env: " << GeometryUtils::toString(e).toStdString() <<
                  " way: " << way->toString().toStdString());
        throw InternalErrorException("Way too big after splitting.");
      }
    }

    for (int i = 0; i < tempNodes.size(); i++)
    {
      RemoveNodeOp::removeNodeNoCheck(_map, tempNodes[i]);
    }

    if (w == NULL)
    {
      throw InternalErrorException("Internal Error: Strange, no way in group.");
    }
    if (env.isNull() == true && w->getNodeIds().size() > 0)
    {
      throw InternalErrorException("Internal Error: Strange, no way point values.");
    }

    _writer->emitRecord(_map);
  }
  else
  {
    set<long> missing = nodeIds;
    for (set<long>::const_iterator it = rawNodes.begin(); it != rawNodes.end(); ++it)
    {
      missing.erase(*it);
    }
    LOG_WARN("Dropping invalid way due to missing nodes. " << w->toString());
    LOG_WARN("  Missing nodes: " << missing);
    if (_strict)
    {
      throw HootException("Shouldn't be here with good data.");
    }
  }
}

}
