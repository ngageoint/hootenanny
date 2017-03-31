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
#include <hoot/core/util/ConfPath.h>
#include <hoot/hadoop/pbf/PbfInputFormat.h>
#include <hoot/hadoop/pbf/PbfRecordReader.h>
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/util/UuidHelper.h>
#include <hoot/core/io/OsmApiDbSqlStatementFormatter.h>

// Pretty Pipes
#include <pp/mapreduce/Job.h>
#include <pp/Hdfs.h>
#include <pp/io/LineRecordWriter.h>
#include <pp/HadoopPipesUtils.h>

// Qt
#include <QFileInfo>
#include <QTextStream>

#include <geos/geom/Envelope.h>
using namespace geos::geom;

#include "WriteOsmSqlStatementsMapper.h"
#include "WriteOsmSqlStatementsReducer.h"
#include "WriteOsmSqlStatementsDriver.h"

using namespace pp;

namespace hoot
{

WriteOsmSqlStatementsDriver::WriteOsmSqlStatementsDriver() :
_outputFileCopyLocation(""),
_outputDelimiter("\t") //TODO: fix
{
  _sqlFormatter.reset(new OsmApiDbSqlStatementFormatter(_outputDelimiter));
}

bool WriteOsmSqlStatementsDriver::_outputIsDatabaseDestination(const QString output) const
{
  return output.toLower().startsWith("osmapidb://");
}

void WriteOsmSqlStatementsDriver::write(const QString input, const QString output)
{
  pp::Hdfs fs;
  LOG_DEBUG("Creating work directory...");
  QString hdfsOutput =
    "tmp/" + UuidHelper::createUuid().toString().replace("{", "").replace("}", "") +
    "-WriteOsmSqlStatementsDriver/";
  fs.mkdirs(hdfsOutput.toStdString());
  const QString sqlFile = _getSqlFileOutputLocation(hdfsOutput, output);

  _runElementSqlStatementsWriteJob(
    fs.getAbsolutePath(input.toStdString()), hdfsOutput.toStdString());

  //merge all the output files into one and copy back to the local file system; how expensive
  //is this going to be for a ton of big files?
  LOG_INFO("Merging temporary output files into one SQL file...");
  pp::HadoopPipesUtils::mergeFilesToLocalFileSystem(hdfsOutput.toStdString(), sqlFile.toStdString());

  //append the single changeset entry to the file
  _writeChangesetToSqlFile(sqlFile);

  if (_outputIsDatabaseDestination(output))
  {
    //TODO: add file name and record count to this message
    LOG_INFO("Executing element SQL.  17 separate SQL COPY statements will be executed...");
    ApiDb::execSqlFile(output, sqlFile);
  }
}

void WriteOsmSqlStatementsDriver::_writeChangesetToSqlFile(const QString sqlFileLocation)
{
  LOG_DEBUG("Writing changeset to output SQL file...");
  const QString changesetHeader = _sqlFormatter->getChangesetSqlHeaderString();
  Envelope bounds;
  bounds.init();
  //TODO: fix num changes?...how?
  const QString changesetStr = _sqlFormatter->changesetToSqlString(1, 1, 1, bounds);
  QFile outputSqlFile(sqlFileLocation);
  if (!outputSqlFile.open(QIODevice::Append))
  {
    throw HootException("Could not open SQL output file: " + sqlFileLocation);
  }
  QTextStream outStream(&outputSqlFile);
  outStream << changesetHeader;
  outStream << changesetStr;
  outStream.flush();
  outputSqlFile.close();
}

QString WriteOsmSqlStatementsDriver::_getSqlFileOutputLocation(const QString hdfsOutput,
                                                               const QString userSpecifiedOutput) const
{
  QString outputLocation;
  if (!_outputIsDatabaseDestination(userSpecifiedOutput))
  {
    outputLocation = userSpecifiedOutput;
  }
  else if (!_outputFileCopyLocation.isEmpty())
  {
    outputLocation = _outputFileCopyLocation;
  }
  else
  {
    outputLocation = hdfsOutput + "/output.sql";
  }
  return outputLocation;
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

  //job.getConfiguration().setInt("mapred.task.timeout", 6 * 3600 * 1000); //timeout to 6 hours
  // be nice and don't start the reduce tasks until most of the map tasks are done.
  //job.getConfiguration().setDouble("mapred.reduce.slowstart.completed.maps", 0.98);
  //job.getConfiguration().setInt("mapred.map.tasks", 8);
  //job.getConfiguration().setInt("mapred.reduce.tasks", 4);

  //job.getConfiguration().setLong("changesetMaxSize", ConfigOptions().getChangesetMaxSize());
  //job.getConfiguration().setLong("changesetUserId", 1); //TODO: fix
  job.getConfiguration().setLong("writeBufferSize", _writeBufferSize);

  LOG_DEBUG(job.getJobTracker());
  job.run();
}

}
