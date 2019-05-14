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

#include "ConflateReducer.h"

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/OsmMapListener.h>
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/conflate/Conflator.h>
#include <hoot/core/conflate/DuplicateNameRemover.h>
#include <hoot/core/conflate/DuplicateWayRemover.h>
#include <hoot/core/conflate/ImpliedDividedMarker.h>
#include <hoot/core/conflate/NodeReplacements.h>
#include <hoot/core/conflate/NoInformationElementRemover.h>
#include <hoot/core/conflate/OutsideBoundsRemover.h>
#include <hoot/core/conflate/LargeWaySplitter.h>
#include <hoot/core/conflate/LocalTileWorker.h>
#include <hoot/core/conflate/SuperfluousWayRemover.h>
#include <hoot/core/conflate/UnlikelyIntersectionRemover.h>
#include <hoot/core/conflate/splitter/DualWaySplitter.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/ops/MergeNearbyNodes.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>
#include <hoot/hadoop/Debug.h>
#include <hoot/hadoop/HadoopIdGenerator.h>
#include <hoot/hadoop/pbf/PbfRecordWriter.h>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>
#include <pp/Hdfs.h>

#include "ConflateMapper.h"

using namespace geos::geom;
using namespace std;

namespace hoot
{

unsigned int ConflateReducer::logWarnCount = 0;

PP_FACTORY_REGISTER(pp::Reducer, ConflateReducer)

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

ConflateReducer::ConflateReducer()
{
  _initialized = false;
}

void ConflateReducer::close()
{
  if (_initialized)
  {
    // write the node replacements.
    QString path = QString("%1/part-%2m.replacement").
        arg(QString::fromStdString(_workDir)).
        arg(_partition, 5, 10, QChar('0'));

    pp::Hdfs fs;
    boost::shared_ptr<ostream> os(fs.create(path.toStdString()));
    LOG_VAR(_nr.getReplacements());
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
}

void ConflateReducer::_conflate(int key, HadoopPipes::ReduceContext& context)
{
  LOG_INFO("Conflating a map. key: " << key);
  LOG_INFO("  Envelope: " << GeometryUtils::toString(_envelopes[key]));
  boost::shared_ptr<OsmMap> map(new OsmMap());
  map->setIdGenerator(_idGen);

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

//# warning remove temporary map.
//  QString tmp = "tmp/" + QUuid::createUuid().toString().replace("{", "").replace("}", "") +
//      "-conflate-input.osm.pbf";
//  LOG_INFO("Writing input map out to temporary: " << tmp);
//  OsmPbfWriter writer;
//  pp::Hdfs fs;
// boost::shared_ptr<ostream> strm(fs.create(tmp.toStdString()));
//  writer.write(map, strm.get());
//  strm->flush();
//  strm.reset();

  _validate(map);

  // if it is an empty map there is no work to be done.
  if (map->isEmpty())
  {
    return;
  }

  // keep track of all the nodes that get replaced.
  boost::shared_ptr<ReplacedNodeListener> rnl(new ReplacedNodeListener(_nr.getReplacements()));
  map->registerListener(rnl);

  /// @todo consolidate this inside the conflator and make it easier to read.
  MapProjector::projectToPlanar(map);

  _validate(map);
  DuplicateWayRemover::removeDuplicates(map);
  _validate(map);
  SuperfluousWayRemover::removeWays(map);
  _validate(map);
  map = DualWaySplitter::splitAll(map, DualWaySplitter::Left, 12.5);
  _validate(map);
  UnlikelyIntersectionRemover::removeIntersections(map);
  _validate(map);
  map = ImpliedDividedMarker::markDivided(map);
  DuplicateNameRemover::removeDuplicates(map);

  MapProjector::projectToWgs84(map);

  // Disable no information element remover. See #4125
  // In short, the no information element remover may remove nodes that have no parent during the
  // reduce step, but have a parent during another reduce step.
  QStringList l = ConfigOptions().getMapCleanerTransforms();
  l.removeAll(QString::fromStdString(NoInformationElementRemover::className()));
  conf().set(ConfigOptions::getMapCleanerTransformsKey(), l);

  // call conflation routine
  Conflator conflator;
  //conflator.setDebugging(true);
  conflator.loadSource(map);
  conflator.conflate();

  boost::shared_ptr<OsmMap> result(new OsmMap(conflator.getBestMap()));
  MapProjector::projectToWgs84(result);

  for (HashMap<long, long>::const_iterator it = _nr.getReplacements().begin();
       it != _nr.getReplacements().end(); ++it)
  {
    if (result->containsNode(it->first))
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

  LOG_INFO("Performing debug check to make sure ways are the right size.");
  // Using a copy of the map so we can split ways as needed. Make sure they're the right size.
  const WayMap wm = result->getWays();
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    const boost::shared_ptr<Way>& w = result->getWay(it->first);
    WaySplitter::split(result, w, _maxWaySize);
  }

  // check to make sure all the ways are the right size. Using a copy of the map so we can split
  // ways as needed.
  const WayMap& wm2 = result->getWays();
  for (WayMap::const_iterator it = wm2.begin(); it != wm2.end(); ++it)
  {
    const boost::shared_ptr<const Way>& w = it->second;

    const Envelope e = w->getEnvelopeInternal(result);

    if (e.getWidth() > _maxWaySize || e.getHeight() > _maxWaySize)
    {
      throw HootException("Way is too big after conflation.");
    }
  }

  LOG_INFO("best map. Node count: " << conflator.getBestMap()->getNodes().size() <<
    " way count: " << conflator.getBestMap()->getWays().size());

  LOG_INFO("Result map. Node count: " << result->getNodes().size() << " way count: " <<
    result->getWays().size());

  _validate(result);

  _emitMap(result);
}

void ConflateReducer::_emitMap(boost::shared_ptr<OsmMap> map)
{
  Envelope* e = GeometryUtils::toEnvelope(CalculateMapBoundsVisitor::getBounds(map));
  _stats.expandEnvelope(*e);
  delete e;
  // write the map out to the working directory.
  _writer->emitRecord(map);
}

const Envelope& ConflateReducer::_getContainingEnvelope(const boost::shared_ptr<OsmMap>& map)
{
  boost::shared_ptr<Envelope> e(GeometryUtils::toEnvelope(CalculateMapBoundsVisitor::getBounds(map)));

  for (size_t i = 0; i < _envelopes.size(); i++)
  {
    if (_envelopes[i].contains(e.get()))
    {
      return _envelopes[i];
    }
  }
  throw HootException("Could not find containing envelope for: " + e->toString());
}

void ConflateReducer::_init(HadoopPipes::ReduceContext& context)
{
  HadoopPipes::RecordWriter* writer = pp::HadoopPipesUtils::getRecordWriter(&context);
  _writer = dynamic_cast<PbfRecordWriter*>(writer);
  if (_writer == NULL)
  {
    throw InternalErrorException("Error getting RecordWriter.");
  }

  boost::shared_ptr<pp::Configuration> c(pp::HadoopPipesUtils::toConfiguration(context.getJobConf()));
  _stats.read(*c);

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
  _maxWaySize = c->getDouble(ConflateMapper::maxWaySizeKey());
  _buffer = c->getDouble(ConflateMapper::bufferKey());

  // read the envelopes
  _envelopes = ConflateMapper::parseEnvelopes(context.getJobConf()->get(
    ConflateMapper::envelopesKey()));

  _initialized = true;
}

boost::shared_ptr<OsmMap> ConflateReducer::_readMap(const string& value)
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

void ConflateReducer::reduce(HadoopPipes::ReduceContext& context)
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
    LOG_INFO("Writing dregs");
    // emit all the data right out to disk.
    while (context.nextValue())
    {
      boost::shared_ptr<OsmMap> map = _readMap(context.getInputValue());
      _emitMap(map);
    }
  }
}

void ConflateReducer::_validate(const boost::shared_ptr<OsmMap>& map)
{
  LOG_INFO("Validating map.");
  Debug::printTroubled(map);
  map->validate(true);
}

}
