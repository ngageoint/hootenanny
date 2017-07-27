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

#ifndef WAYJOIN2REDUCER_H
#define WAYJOIN2REDUCER_H

// Hoot
#include <hoot/hadoop/stats/MapStats.h>
#include <hoot/core/elements/Status.h>

// Pretty Pipes
#include <pp/mapreduce/Reducer.h>

// Standard
#include <string>

#include "WayJoin2RecordReader.h"

namespace hoot
{

class HadoopIdGenerator;
class PbfRecordWriter;

class WayJoin2Reducer : public pp::Reducer
{
public:

  static std::string className() { return "hoot::WayJoin2Reducer"; }

  static unsigned int logWarnCount;

  WayJoin2Reducer();

  virtual void close();

  virtual void reduce(HadoopPipes::ReduceContext& context);

private:

  PbfRecordWriter* _writer;
  boost::shared_ptr<OsmMap> _map;
  MapStats _stats;
  double _maxWaySize;
  boost::shared_ptr<HadoopIdGenerator> _idGen;
  int _partition;
  std::string _workDir;
  bool _strict;

  long _wayIdDelta;
  long _nodeIdDelta;
  long _relationIdDelta;
  Status _newStatus;
  long _wayCount;

  void _writeNodes(HadoopPipes::ReduceContext& context);
  void _writeWay(HadoopPipes::ReduceContext& context);
};

}

#endif // WAYJOIN2REDUCER_H
