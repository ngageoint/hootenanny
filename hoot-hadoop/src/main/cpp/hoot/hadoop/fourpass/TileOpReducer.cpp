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

#include "TileOpReducer.h"

// Hoot
#include <hoot/core/conflate/Conflator.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMapListener.h>
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/io/ObjectInputStream.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/ops/Boundable.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>
#include <hoot/hadoop/Debug.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/hadoop/HadoopIdGenerator.h>
#include <hoot/hadoop/pbf/PbfRecordWriter.h>
#include <hoot/core/OsmMap.h>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>
#include <pp/Hdfs.h>

// Tgs
#include <tgs/System/SystemInfo.h>

#include "TileOpDriver.h"
#include "TileOpMapper.h"

using namespace geos::geom;
using namespace std;

namespace hoot
{

unsigned int TileOpReducer::logWarnCount = 0;

PP_FACTORY_REGISTER(pp::Reducer, TileOpReducer)

class ReplacedNodeListener : public OsmMapListener
{
public:
  ReplacedNodeListener(HashMap<long, long>& m) : _map(m) {}

  virtual boost::shared_ptr<OsmMapListener> clone() const
  {
    return boost::shared_ptr<OsmMapListener>(new ReplacedNodeListener(_map));
  }

  void replaceNodePre(long oldId, long newId) { _map[oldId] = newId; }

protected:
  HashMap<long, long>& _map;
};

TileOpReducer::TileOpReducer()
{
  _initialized = false;
}

void TileOpReducer::close()
{
  if (_initialized)
  {
    LOG_DEBUG(Tgs::SystemInfo::getMemoryUsageString());
    // write the node replacements.
    QString path = QString("%1/part-%2m.replacement").
        arg(QString::fromStdString(_workDir)).
        arg(_partition, 5, 10, QChar('0'));

    pp::Hdfs fs;
    boost::shared_ptr<ostream> os(fs.create(path.toStdString()));
    _nr.write(*os);
    os.reset();

    // write out the max/min node values.
    _stats.expandNodeRange(_idGen->getMaxNodeId());
    _stats.expandWayRange(_idGen->getMaxWayId());

    path = QString("%1/part-%2m.stats").
        arg(QString::fromStdString(_workDir)).
        arg(_partition, 5, 10, QChar('0'));

    boost::shared_ptr<ostream> osStats(fs.create(path.toStdString()));

    _stats.write(*osStats);
  }
  LOG_DEBUG(Tgs::SystemInfo::getMemoryUsageString());
}

void TileOpReducer::_conflate(int key, HadoopPipes::ReduceContext& context)
{
  LOG_INFO("Conflating a map. key: " << key);
  LOG_INFO("  Envelope: " << GeometryUtils::toString(_envelopes[key]));

  LOG_DEBUG(Tgs::SystemInfo::getMemoryUsageString());

  boost::shared_ptr<OsmMap> map(new OsmMap());
  map->setIdGenerator(_idGen);

//#warning remove temporary map.
//  QString tmp = "tmp/" + QUuid::createUuid().toString().replace("{", "").replace("}", "") +
//      "-conflate-input.pbf";
//  LOG_INFO("Writing input map out to temporary: " << tmp);
//  OsmPbfWriter writer;
//  pp::Hdfs fs;
// boost::shared_ptr<ostream> strm(fs.create(tmp.toStdString()));
//  writer.write(map, strm.get());
//  strm->flush();
//  strm.reset();

  while (context.nextValue())
  {
    // read the map from the given string.
    stringstream ss(context.getInputValue(), stringstream::in);

    OsmPbfReader reader(true);
    reader.setUseFileStatus(true);
    reader.parse(&ss, map);
  }
  LOG_INFO("Got map. Node count: " << map->getNodes().size() << " way count: " <<
    map->getWays().size());
  Envelope* e = GeometryUtils::toEnvelope(CalculateMapBoundsVisitor::getBounds(map));
  LOG_INFO("Map envelope: " << e->toString());
  delete e;

  _validate(map);

  // if it is an empty map there is no work to be done.
  if (map->isEmpty())
  {
    return;
  }

  // keep track of all the nodes that get replaced.
  boost::shared_ptr<ReplacedNodeListener> rnl(new ReplacedNodeListener(_nr.getReplacements()));
  map->registerListener(rnl);

  Boundable* b = dynamic_cast<Boundable*>(_op.get());
  if (b)
  {
    const Envelope& e = _getContainingEnvelope(map);
    b->setBounds(e);
  }

  _op->apply(map);

  for (HashMap<long, long>::const_iterator it = _nr.getReplacements().begin();
       it != _nr.getReplacements().end(); ++it)
  {
    if (map->containsNode(it->first))
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Strange, a replaced node is still in the map.  nid: " << it->first);
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
  }

  LOG_INFO("Result before way splitting. Node count: " << map->getNodes().size() <<
           " way count: " << map->getWays().size());

  MapProjector::projectToWgs84(map);

  // Using a copy of the map so we can split ways as needed. Make sure they're the right size.
  const WayMap wm = map->getWays();
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    const boost::shared_ptr<Way>& w = map->getWay(it->first);
    WaySplitter::split(map, w, _maxWaySize);
  }

# ifdef DEBUG
    LOG_INFO("Performing debug check to make sure ways are the right size.");
    // check to make sure all the ways are the right size. Using a copy of the map so we can split
    // ways as needed.
    const WayMap& wm2 = map->getWays();
    for (WayMap::const_iterator it = wm2.begin(); it != wm2.end(); ++it)
    {
      const boost::shared_ptr<const Way>& w = it->second;

      const Envelope e = w->getEnvelopeInternal(map);

      if (e.getWidth() > _maxWaySize || e.getHeight() > _maxWaySize)
      {
        throw HootException("Way is too big after conflation.");
      }
    }
# endif

  LOG_INFO("Result map. Node count: " << map->getNodes().size() << " way count: " <<
    map->getWays().size());

  _emitMap(map);

  LOG_DEBUG(Tgs::SystemInfo::getMemoryUsageString());
}

void TileOpReducer::_emitMap(boost::shared_ptr<OsmMap> map)
{
  Envelope* e = GeometryUtils::toEnvelope(CalculateMapBoundsVisitor::getBounds(map));
  _stats.expandEnvelope(*e);
  delete e;
  // write the map out to the working directory.
  _writer->emitRecord(map);
  LOG_DEBUG(Tgs::SystemInfo::getMemoryUsageString());
}

const Envelope& TileOpReducer::_getContainingEnvelope(const boost::shared_ptr<OsmMap>& map)
{
  boost::shared_ptr<Envelope> e(GeometryUtils::toEnvelope(CalculateMapBoundsVisitor::getBounds(map)));

  for (size_t i = 0; i < _envelopes.size(); i++)
  {
    Envelope env = _envelopes[i];
    env.expandBy(_buffer);
    if (env.contains(e.get()))
    {
      return _envelopes[i];
    }
  }
  throw HootException("Could not find containing envelope for: " + e->toString());
}

void TileOpReducer::_init(HadoopPipes::ReduceContext& context)
{
  HadoopPipes::RecordWriter* writer = pp::HadoopPipesUtils::getRecordWriter(&context);
  _writer = dynamic_cast<PbfRecordWriter*>(writer);
  if (_writer == NULL)
  {
    throw InternalErrorException("Error getting RecordWriter.");
  }

  boost::shared_ptr<pp::Configuration> c(pp::HadoopPipesUtils::toConfiguration(context.getJobConf()));
  _stats.read(*c);

  LOG_DEBUG("Serialized settings: " << c->get(TileOpDriver::settingsConfKey().toStdString(), "{}"));
  conf().loadFromString(
        QString::fromStdString(c->get(TileOpDriver::settingsConfKey().toStdString(), "{}")));
  conf().loadEnvironment();

  // set GDAL_DATA to the current working directory. This avoids some projection issues when
  // GDAL isn't installed on the slaves.
  char* cwd = getcwd(NULL, 0);
  setenv("GDAL_DATA", cwd, 1);
  free(cwd);
  LOG_INFO("Set GDAL_DATA to: " << getenv("GDAL_DATA"));

  _partition = c->getInt("mapred.task.partition");
  _workDir = c->get("mapred.work.output.dir");
  int taskCount = c->getInt("mapred.reduce.tasks");
  // create a new id generator and use it with our map.
  _idGen.reset(new HadoopIdGenerator(_partition, taskCount, _stats.getMaxWayId(),
    _stats.getMaxNodeId()));
  _maxWaySize = c->getDouble(TileOpMapper::maxWaySizeKey());
  _buffer = c->getDouble(TileOpMapper::bufferKey());

  stringstream ss(c->getBytes(opKey()));
  _op.reset(ObjectInputStream(ss).readObject<OsmMapOperation>());

  // read the envelopes
  _envelopes = TileOpMapper::parseEnvelopes(context.getJobConf()->get(
    TileOpMapper::envelopesKey()));

  _initialized = true;
}

boost::shared_ptr<OsmMap> TileOpReducer::_readMap(const string& value)
{
  // read the map from the given string.
  boost::shared_ptr<OsmMap> result(new OsmMap());
  stringstream ss(value, stringstream::in);

  OsmPbfReader reader(true);
  reader.setUseFileStatus(true);
  reader.parse(&ss, result);
//  LOG_INFO("Read map. value size: " << value.size() << " node count: " <<
//    result->getNodes().size() << " way count: " << result->getWays().size());

  return result;
}

void TileOpReducer::reduce(HadoopPipes::ReduceContext& context)
{
  if (!_initialized)
  {
    _init(context);
  }
  const string& keyStr = context.getInputKey();
  int64_t* key = (int64_t*)keyStr.data();

  // if this is data that needs to be conflated
  if (*key >= 0)
  {
    _conflate(*key, context);
  }
  // if this is not data that needs to be conflated (dregs)
  else
  {
    // emit all the data right out to disk.
    while (context.nextValue())
    {
      boost::shared_ptr<OsmMap> map = _readMap(context.getInputValue());
      LOG_INFO("Passing dregs. Node Count: " << map->getNodes().size() << " Way Count: " <<
               map->getWays().size());
      _emitMap(map);
    }
  }
}

void TileOpReducer::_validate(const boost::shared_ptr<OsmMap>& map)
{
  LOG_INFO("Validating map.");
  Debug::printTroubled(map);
  map->validate(true);

}

}
