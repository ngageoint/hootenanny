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

#include "WayJoinDriver.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/hadoop/pbf/PbfInputFormat.h>
#include <hoot/hadoop/pbf/PbfRecordReader.h>
#include <hoot/hadoop/pbf/PbfRecordWriter.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/UuidHelper.h>

// Pretty Pipes
#include <pp/Hdfs.h>
#include <pp/mapreduce/Job.h>
#include <pp/io/CppSeqFileRecordWriter.h>

#include "WayJoin1Mapper.h"
#include "WayJoin1Reducer.h"
#include "WayJoin2Mapper.h"
#include "WayJoin2Reducer.h"
#include "WayJoin2RecordReader.h"
#include "WayJoin2InputFormat.h"

namespace hoot
{
using namespace pp;

WayJoinDriver::WayJoinDriver(Meters maxWaySize)
{
  _maxWaySize = maxWaySize;
  _nodeIdDelta = 0;
  _relationIdDelta = 0;
  _wayIdDelta = 0;
  _newStatus = Status::Unknown1;
}

void WayJoinDriver::calculateWayBounds(QString in, QString out)
{
  Hdfs fs;
  QString tmp = "tmp/" + UuidHelper::createUuid().toString().replace("{", "").replace("}", "") +
      "-JoinWaysToPoints.csq";
  try
  {
    joinWaysToNodes(in, tmp);

    joinPointsToWays(in + "," + tmp, out);

    fs.deletePath(tmp.toStdString(), true);
  }
  catch (HootException& e)
  {
    if (fs.exists(tmp.toStdString()))
    {
      fs.deletePath(tmp.toStdString(), true);
    }
    throw e;
  }
}

void WayJoinDriver::joinWaysToNodes(QString input, QString out)
{
  // create a job
  Job job;

  job.setVerbose(Log::getInstance().getLevel() <= Log::Debug);
  // set the name
  job.setName("WayJoin1 " + input.toStdString());

  // be nice and don't start the reduce tasks until most of the map tasks are done.
  job.getConfiguration().setDouble("mapred.reduce.slowstart.completed.maps", 0.98);

  // set the input/output
  job.setInput(input.toStdString());
  job.setOutput(out.toStdString());

  // setup the mapper and reducer classes.
  job.setMapperClass(WayJoin1Mapper::className());
  job.setReducerClass(WayJoin1Reducer::className());
  job.setInputFormatClass(PbfInputFormat::className());
  job.setRecordReaderClass(PbfRecordReader::className());
  job.setRecordWriterClass(CppSeqFileRecordWriter::className());

  // Adds all libraries in this directory to the job.
  job.addLibraryDirs(ConfigOptions().getHootHadoopLibpath());

  job.addFile(ConfPath::search("hoot.json").toStdString());

  // This library will be used to provide mapper/reducer classes and anything else referenced
  // by the factory.
  job.addPlugin(getenv("HOOT_HOME") + string("/lib/libHootHadoop.so.1"));

  _addDefaultJobSettings(job);

  // run the job.
  job.run();

  LOG_INFO("Finished job.");
}

void WayJoinDriver::joinPointsToWays(QString input, QString out)
{
  // create a job
  Job job;

  job.setVerbose(Log::getInstance().getLevel() <= Log::Debug);
  // set the name
  job.setName("WayJoin2 " + input.toStdString());

  // set the input/output
  LOG_INFO("input: " << input);
  job.setInput(input.toStdString());
  job.setOutput(out.toStdString());

  // Pass the min/max values as parameters to the job.
  MapStats stats;
  stats.readDir(input);
  stats.write(job.getConfiguration());
  LOG_INFO("MapStats: " << stats.toString().toStdString());

  // Pass the maximum way size as a parameter to the job.
  job.getConfiguration().setDouble("hoot.max.way.size", _maxWaySize);
  // be nice and don't start the reduce tasks until most of the map tasks are done.
  job.getConfiguration().setDouble("mapred.reduce.slowstart.completed.maps", 0.98);

  job.getConfiguration().setInt(WayJoin2Mapper::elementStatusKey(), _newStatus.getEnum());
  job.getConfiguration().setLong(WayJoin2Mapper::nodeIdDeltaKey(), _nodeIdDelta);
  job.getConfiguration().setLong(WayJoin2Mapper::relationIdDeltaKey(), _relationIdDelta);
  job.getConfiguration().setLong(WayJoin2Mapper::wayIdDeltaKey(), _wayIdDelta);

  // setup the mapper and reducer classes.
  job.setMapperClass(WayJoin2Mapper::className());
  job.setReducerClass(WayJoin2Reducer::className());
  job.setInputFormatClass(WayJoin2InputFormat::className());
  job.setRecordReaderClass(WayJoin2RecordReader::className());
  job.setRecordWriterClass(PbfRecordWriter::className());

  // Adds all libraries in this directory to the job.
  job.addLibraryDirs(ConfigOptions().getHootHadoopLibpath());

  job.addFile(ConfPath::search("hoot.json").toStdString());

  // This library will be used to provide mapper/reducer classes and anything else referenced
  // by the factory.
  job.addPlugin(getenv("HOOT_HOME") + string("/lib/libHootHadoop.so.1"));

  _addDefaultJobSettings(job);

  // run the job.
  job.run();
}

void WayJoinDriver::setRenumbering(long nodeIdDelta, long wayIdDelta, long relationIdDelta,
                                   Status newStatus)
{
  _nodeIdDelta = nodeIdDelta;
  _relationIdDelta = relationIdDelta;
  _wayIdDelta = wayIdDelta;
  _newStatus = newStatus;
}

}
