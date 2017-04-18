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

#ifndef WAYJOINREDUCER_H
#define WAYJOINREDUCER_H

// Hoot
#include <hoot/hadoop/stats/MapStats.h>

// Pretty Pipes
#include <pp/io/RecordWriter.h>
#include <pp/mapreduce/Reducer.h>

// Standard
#include <string>
#include <vector>

namespace hoot
{
using namespace std;

class WayJoin1Reducer : public pp::Reducer
{
public:

  struct Value
  {
    int64_t nodeId;
    double x;
    double y;
  } __attribute__((packed));

  static string className() { return "hoot::WayJoin1Reducer"; }

  static unsigned int logWarnCount;

  WayJoin1Reducer();

  void close();

  void reduce(HadoopPipes::ReduceContext& context);

private:
  pp::RecordWriter* _writer;
  vector<int64_t> _wayIds;
  int _missingNodes;

  MapStats _stats;

  string _workDir;
  int _partition;
};

}

#endif // WAYJOINREDUCER_H
