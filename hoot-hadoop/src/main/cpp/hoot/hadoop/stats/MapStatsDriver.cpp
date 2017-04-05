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

#include "MapStatsDriver.h"

// Hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>
#include <hoot/hadoop/pbf/PbfInputFormat.h>
#include <hoot/hadoop/pbf/PbfRecordReader.h>
#include <hoot/core/util/UuidHelper.h>

// Pretty Pipes
#include <pp/Hdfs.h>
#include <pp/mapreduce/Job.h>

#include "MapStatsMapper.h"

namespace hoot
{
using namespace pp;

MapStatsDriver::MapStatsDriver()
{
}

const MapStats& MapStatsDriver::calculateStats(const QString& input)
{
  // create a job
  Job job;

  job.setVerbose(Log::getInstance().getLevel() <= Log::Debug);
  // set the name
  job.setName(("MapStatsDriver " + input).toStdString());

  QString tmp = "tmp/" + UuidHelper::createUuid().toString().replace("{", "").replace("}", "") +
      "-MapStatsDriver.stats";

  // set the input/output
  job.setInput(input.toStdString());
  job.setOutput(tmp.toStdString());

  // setup the mapper and reducer classes.
  job.setMapperClass(MapStatsMapper::className());
  job.setNumReduceTask(0);
  job.setInputFormatClass(PbfInputFormat::className());
  job.setRecordReaderClass(PbfRecordReader::className());

  // Adds all libraries in this directory to the job.
  job.addLibraryDirs(ConfigOptions().getHootHadoopLibpath());

  job.addFile(ConfPath::search("hoot.json").toStdString());

  // This library will be used to provide mapper/reducer classes and anything else referenced
  // by the factory.
  job.addPlugin(getenv("HOOT_HOME") + string("/lib/libHootHadoop.so.1"));

  _addDefaultJobSettings(job);

  // run the job.
  job.run();

  _stats.readDir(tmp);
  LOG_INFO("Stats: " << _stats.toString());

  Hdfs fs;
  fs.deletePath(tmp.toStdString(), true);

  return _stats;
}

}
