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

#ifndef CONFLATEREDUCER_H
#define CONFLATEREDUCER_H

// Hoot
#include <hoot/hadoop/stats/MapStats.h>
#include <hoot/core/conflate/NodeReplacements.h>

// Pretty Pipes
#include <pp/mapreduce/Reducer.h>

namespace hoot
{

class PbfRecordWriter;
class HadoopIdGenerator;
class Way;
class OsmMap;

class ConflateReducer : public pp::Reducer
{
public:
  static string className() { return "hoot::ConflateReducer"; }

  static unsigned int logWarnCount;

  ConflateReducer();

  virtual void close();

  virtual void reduce(HadoopPipes::ReduceContext& context);

private:

  vector<Envelope> _envelopes;

  bool _initialized;

  /// Maximum way size in degrees.
  double _maxWaySize;
  double _buffer;
  NodeReplacements _nr;

  MapStats _stats;
 boost::shared_ptr<HadoopIdGenerator> _idGen;
  int _partition;
  std::string _workDir;
  PbfRecordWriter* _writer;

  void _conflate(int key, HadoopPipes::ReduceContext& context);

  void _emitMap(OsmMapPtr map);

  const Envelope& _getContainingEnvelope(const OsmMapPtr& map);

  void _init(HadoopPipes::ReduceContext& context);

 OsmMapPtr _readMap(const string& value);

  void _removeReplacedNodes(OsmMapPtr& map);

  void _splitWay(WayPtr w);

  void _validate(const OsmMapPtr& map);

  void _writeNodes(HadoopPipes::ReduceContext& context);
  void _writeWay(HadoopPipes::ReduceContext& context);
};

}

#endif // CONFLATEREDUCER_H
