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

// Hoot
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/hadoop/pbf/PbfInputFormat.h>
#include <hoot/hadoop/pbf/PbfRecordReader.h>

// Pretty Pipes
#include <pp/mapreduce/Job.h>
#include <pp/Hdfs.h>
#include <pp/io/LineRecordWriter.h>
#include <pp/io/LineRecordReader.h>

// Qt
#include <QFileInfo>

#include "WriteOsmSqlStatementsMapper.h"
#include "WriteOsmSqlStatementsReducer.h"
#include "WriteOsmSqlStatementsDriver.h"

namespace hoot
{

WriteOsmSqlStatementsDriver::WriteOsmSqlStatementsDriver()
{
}

void WriteOsmSqlStatementsDriver::write(const QString input, const QString output)
{
  //we'll ignore the output file name for now and dump all the output in separate files in the
  //output dir
  pp::Hdfs fs;
  QFileInfo outputInfo(output);
  QString hdfsOutput = QString::fromStdString(fs.getAbsolutePath(output.toStdString()));
  hdfsOutput.replace(outputInfo.fileName(), "");
  const QString elementSqlOutputDir = hdfsOutput + "/WriteOsmSqlStatements";
  fs.mkdirs(elementSqlOutputDir.toStdString());

  _runElementSqlStatementsWriteJob(
    fs.getAbsolutePath(input.toStdString()), elementSqlOutputDir.toStdString());

  //merge all the output files into one and copy back to the local file system
  //const QString cmd = "hadoop fs -getmerge " + changesetSqlOutputDir + " " + output;
  LOG_INFO("Merging output files...");
  const QString cmd = "hadoop fs -getmerge " + elementSqlOutputDir + " " + output;
  LOG_VARD(cmd);
  if (system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Failed merging SQL statements output into a single SQL file: " + output);
  }
}

void WriteOsmSqlStatementsDriver::_runElementSqlStatementsWriteJob(const string& input,
                                                                   const string& output)
{
  LOG_INFO("Running element SQL statements write job...");

  pp::Job job;
  job.setVerbose(Log::getInstance().getLevel() <= Log::Debug);
  job.setName("WriteElementSqlStatements");

  job.setInput(input);
  job.setOutput(output);

  job.setMapperClass(WriteOsmSqlStatementsMapper::className());
  job.setReducerClass(WriteOsmSqlStatementsReducer::className());
  job.setInputFormatClass(PbfInputFormat::className());
  job.setRecordReaderClass(PbfRecordReader::className());
  job.setRecordWriterClass(pp::LineRecordWriter::className());

  // Adds all libraries in this directory to the job.
  job.addLibraryDirs(ConfigOptions().getHootHadoopLibpath());
  job.addFile(ConfPath::search("hoot.json").toStdString());
  // This library will be used to provide mapper/reducer classes and anything else referenced
  // by the factory.
  job.addPlugin(getenv("HOOT_HOME") + string("/lib/libHootHadoop.so.1"));
  _addDefaultJobSettings(job);

  // Setting timeout to 6 hours
  //job.getConfiguration().setInt("mapred.task.timeout", 6 * 3600 * 1000);
  // be nice and don't start the reduce tasks until most of the map tasks are done.
  //job.getConfiguration().setDouble("mapred.reduce.slowstart.completed.maps", 0.98);
  //job.getConfiguration().setInt("mapred.map.tasks", 8);
  //job.getConfiguration().setInt("mapred.reduce.tasks", 4);
  job.getConfiguration().setLong("changesetMaxSize", ConfigOptions().getChangesetMaxSize());
  //TODO: fix
  job.getConfiguration().setLong("changesetUserId", 1);

  //job.setDefaultJobTracker("localhost:9001");
  //job.setJobTracker("localhost:9001");
  LOG_DEBUG(job.getJobTracker());
  job.run();
}

}
