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

#include "ConflateMapper.h"

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>
#include <pp/Hdfs.h>
#include <pp/conf/Configuration.h>

// Hoot
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/hadoop/Debug.h>
#include <hoot/core/OsmMap.h>

namespace hoot
{

PP_FACTORY_REGISTER(pp::Mapper, ConflateMapper)

ConflateMapper::ConflateMapper()
{
  _initialized = false;
}

void ConflateMapper::_addNode(const shared_ptr<Node>& n)
{
  long key = -1;

  // if the node falls in one of the envelopes
  for (size_t i = 0; i < _envelopes.size(); i++)
  {
    // set a positive key
    if (_envelopes[i].contains(n->getX(), n->getY()))
    {
      if (key != -1)
      {
        throw HootException("This should never happen. Envelopes must not overlap.");
      }
      key = i;
    }
  }
  // if the node is not in one of the envelopes.
  if (key == -1)
  {
    // calculate a negative key based on node ID
    key = -1 - (abs(n->getId()) % _reduceTaskCount);
  }

  if (key < -_reduceTaskCount || key >= (int)_envelopes.size())
  {
    LOG_INFO("envelope size: " << _envelopes.size());
    throw HootException(QString("Key is out of range. nid: %1 key: %2").arg(n->getId()).arg(key));
  }

  if (Debug::isTroubledNode(n->getId()))
  {
    LOG_WARN("Writing a troubled node: " << n->toString());
    LOG_WARN("  key: " << key)
  }

  // add the node to the appropriate map
  _writers[key]->writePartial(n);
}

void ConflateMapper::_addWay(ConstOsmMapPtrR map, const shared_ptr<Way> &w)
{
  long key = -1;

  const Envelope& e = w->getApproximateEnvelope(map);

  // if the way falls completely inside one of the envelopes
  for (size_t i = 0; i < _envelopes.size(); i++)
  {
    // set a positive key
    if (_envelopes[i].contains(e))
    {
      if (key != -1)
      {
        throw HootException("This should never happen. Envelopes must not overlap.");
      }
      key = i;
    }
  }
  // if the way is not in one of the envelopes.
  if (key == -1)
  {
    // calculate a negative key based on node ID
    key = -1 - (abs(w->getId()) % _reduceTaskCount);
  }

  if (Debug::isTroubledWay(w->getId()))
  {
    LOG_WARN("Writing a troubled way: " << w->toString());
    LOG_WARN("  key: " << key)
  }

  // add the way to the appropriate map
  _writers[key]->writePartial(w);
}

void ConflateMapper::_flush()
{
  LOG_INFO("Flushing.");
  string key;
  key.resize(sizeof(int64_t));
  int64_t* k = (int64_t*)key.data();
  // emit all maps
  for (QHash< int, shared_ptr<OsmPbfWriter> >::iterator it = _writers.begin();
    it != _writers.end(); ++it)
  {
    *k = it.key();
    it.value()->finalizePartial();
    if (it.value()->getElementsWritten() > 0)
    {
      _context->emit(key, _buffers[*k]->str());
    }
    // free up the RAM
    it.value().reset();
    _buffers[*k].reset();
  }
}

void ConflateMapper::_init(HadoopPipes::MapContext& context)
{
  LOG_INFO("Initializing.");
  // read the envelopes
  _envelopes = parseEnvelopes(context.getJobConf()->get(envelopesKey()));

  shared_ptr<pp::Configuration> c(pp::HadoopPipesUtils::toConfiguration(context.getJobConf()));
  _tileBufferSize = c->getDouble(bufferKey());

  // read the node replacements
  /// @todo depending on the replacements size it may make sense to add this to the job cache.
  _replacmentsPath = context.getJobConf()->get(replacementsKey());

  // create all the necessary OsmMaps for holding our results.
  for (size_t i = 0; i < _envelopes.size(); i++)
  {
    _writers[i] = shared_ptr<OsmPbfWriter>(new OsmPbfWriter());
    _buffers[i] = shared_ptr<stringstream>(new stringstream(stringstream::out));
    _writers[i]->intializePartial(_buffers[i].get());
    LOG_INFO("key: " << i << " envelope: " << _envelopes[i].toString());
  }
  _reduceTaskCount = context.getJobConf()->getInt("mapred.reduce.tasks");
  for (int i = 0; i < _reduceTaskCount; i++)
  {
    _writers[-1 - i] = shared_ptr<OsmPbfWriter>(new OsmPbfWriter());
    _buffers[-1 - i] = shared_ptr<stringstream>(new stringstream(stringstream::out));
    _writers[-1 - i]->intializePartial(_buffers[-1 - i].get());
    LOG_INFO("key: " << -1 - i << " dregs");
  }

  _initialized = true;
}

void ConflateMapper::_map(shared_ptr<OsmMap>& m, HadoopPipes::MapContext& context)
{
  _init(context);

  LOG_DEBUG("Troubled data before replace nodes:");
  Debug::printTroubled(m);

  _replaceNodes(m);

  LOG_DEBUG("Troubled data after replace nodes:");
  Debug::printTroubled(m);

  // go through all ways
  const WayMap& wm = m->getWays();
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    const shared_ptr<Way>& w = it->second;

    // add way to appropriate map
    _addWay(m, w);
  }

  // go through all nodes
  const NodeMap& nm = m->getNodeMap();
  for (NodeMap::const_iterator it = nm.begin(); it != nm.end(); ++it)
  {
    const shared_ptr<Node>& n = it->second;

    // add node to appropriate map
    _addNode(n);
  }

  _flush();
}

vector<Envelope> ConflateMapper::parseEnvelopes(const string& envStr)
{
  vector<Envelope> result;

  QStringList envList = QString::fromStdString(envStr).split(";");
  for (int i = 0; i < envList.size(); i++)
  {
    if (envList.at(i) == "null")
    {
      continue;
    }
    QStringList aEnvStr = envList.at(i).split(",");
    if (aEnvStr.size() != 4)
    {
      throw HootException("Invalid Envelope: Expected 4 bounds.");
    }
    double b[4];
    for (int j = 0; j < aEnvStr.size(); j++)
    {
      bool ok;
      b[j] = aEnvStr.at(j).toDouble(&ok);
      if (ok == false)
      {
        throw HootException("Invalid Envelope: Unable to parse double.");
      }
    }
    Envelope env(b[0], b[1], b[2], b[3]);
    result.push_back(env);
  }

  return result;
}

void ConflateMapper::_replaceNodes(shared_ptr<OsmMap>& m)
{
  pp::Hdfs fs;

  if (!fs.exists(_replacmentsPath))
  {
    throw HootException("replace nodes path does not exist: (" + _replacmentsPath + ")");
  }

  if (fs.getFileStatus(_replacmentsPath).isDir())
  {
    vector<pp::FileStatus> status = fs.listStatus(_replacmentsPath, true);
    for (size_t i = 0; i < status.size(); i++)
    {
      if (QString::fromStdString(status[i].getPath()).endsWith(".replacement") &&
        status[i].isFile())
      {
        auto_ptr<istream> is(fs.open(status[i].getPath()));
        _replaceNodes(m, *is);
      }
    }
  }
  else
  {
    auto_ptr<istream> is(fs.open(_replacmentsPath));
    _replaceNodes(m, *is);
  }
}

void ConflateMapper::_replaceNodes(shared_ptr<OsmMap>& m, istream& is)
{
  int64_t ids[2];
  while (!is.eof())
  {
    is.read((char*)&ids, sizeof(ids));
    if (is.gcount() != 0 && is.gcount() != sizeof(ids))
    {
      throw HootException("Didn't read the expected number of bytes.");
    }

    if (is.gcount() == sizeof(ids))
    {
      if (m->containsNode(ids[0]))
      {
        throw HootException("Did not expect to find a replaced node in the map.");
      }
      m->replaceNode(ids[0], ids[1]);
    }
  }
}

}
