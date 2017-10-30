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
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/io/OsmApiDbSqlStatementFormatter.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/UuidHelper.h>
#include <hoot/hadoop/pbf/PbfInputFormat.h>
#include <hoot/hadoop/pbf/PbfRecordReader.h>

// Pretty Pipes
#include <pp/mapreduce/Job.h>
#include <pp/Hdfs.h>
#include <pp/HadoopPipesUtils.h>
using namespace pp;

// Qt
#include <QFileInfo>
#include <QTextStream>
#include <QDir>

// geos
#include <geos/geom/Envelope.h>
using namespace geos::geom;

#include "WriteOsmSqlStatementsMapper.h"
#include "WriteOsmSqlStatementsReducer.h"
#include "WriteOsmSqlStatementsDriver.h"
#include "SqlStatementLineRecordWriter.h"

using namespace std;

namespace hoot
{

WriteOsmSqlStatementsDriver::WriteOsmSqlStatementsDriver() :
_outputFileCopyLocation(""),
_outputDelimiter("\t")
{
  _sqlFormatter.reset(new OsmApiDbSqlStatementFormatter(_outputDelimiter));
  setConfiguration(conf());
}

WriteOsmSqlStatementsDriver::~WriteOsmSqlStatementsDriver()
{
  close();
}

bool WriteOsmSqlStatementsDriver::_destinationIsDatabase(const QString output) const
{
  return output.toLower().startsWith("osmapidb://");
}

bool WriteOsmSqlStatementsDriver::isSupported(QString urlStr)
{
  LOG_VARD(urlStr);
  QUrl url(urlStr);
  //if we ever want any other writers that the convert command invokes to output sql, then
  //this will have to be made more specific
  return urlStr.toLower().endsWith(".sql") || _database.isSupported(url);
}

void WriteOsmSqlStatementsDriver::open(QString url)
{
  LOG_DEBUG("Opening writer...");

  _output = url;

  // Make sure we're not already open and the URL is valid
  if (!isSupported(url))
  {
    throw HootException(QString("Could not open URL ") + url);
  }

  if (_destinationIsDatabase(_output))
  {
    _database.open(_output);
    LOG_DEBUG("Postgres database version: " << DbUtils::getPostgresDbVersion(_database.getDB()));
  }
}

void WriteOsmSqlStatementsDriver::close()
{
  LOG_DEBUG("Closing writer...");

  if (_destinationIsDatabase(_output) && _database.getDB().isOpen())
  {
    //now re-enable the constraints to make sure the db is valid now that we're done with it
    _database.enableConstraints();
    _database.close();
  }
}

void WriteOsmSqlStatementsDriver::write(const QString inputMapFile)
{
  if (_output.isEmpty())
  {
    throw HootException("Writer has not been opened.");
  }

  Hdfs fs;
  LOG_DEBUG("Creating work directory...");
  const QString hdfsOutput =
    "tmp/" + UuidHelper::createUuid().toString().replace("{", "").replace("}", "") +
    "-WriteOsmSqlStatementsDriver/";
  fs.mkdirs(hdfsOutput.toStdString());
  const QString sqlFile = _getSqlFileOutputLocation(hdfsOutput, _output);

  if (_destinationIsDatabase(_output))
  {
    //We have to turn off constraints before writing the sql file to the db, since the table
    //copy commands are out of dependency order and will violate ref integrity.
    _database.disableConstraints();
  }

  _runElementSqlStatementsWriteJob(
    fs.getAbsolutePath(inputMapFile.toStdString()), hdfsOutput.toStdString());

  if (!_destinationIsDatabase(_output) || !_outputFileCopyLocation.isEmpty())
  {
    //merge all the output files into one and copy it back from hdfs to the local file system
    LOG_INFO("Merging temporary output files into one SQL file...");
    HadoopPipesUtils::mergeFilesToLocalFileSystem(hdfsOutput.toStdString(), sqlFile.toStdString());

    //append the single changeset entry to the file
    _writeChangesetToSqlFile(sqlFile);
  }

  if (_destinationIsDatabase(_output))
  { 
    if (!_execSqlWithMapreduce)
    {
      //we didn't write to the db in the reducer, so exec the whole sql file now with psql
      LOG_INFO("Executing element SQL...");
      ApiDb::execSqlFile(_output, sqlFile);
    }
    else
    {
      //we already wrote the data to the db in the reducer, so all we need to write here is the
      //changeset; the num changes here will not be accurate
      LOG_INFO("Writing changeset record to database...");
      DbUtils::execNoPrepare(
        _database.getDB(),
        QString("INSERT INTO %1 (id, user_id, created_at, closed_at, num_changes) VALUES "
                "(%2, %3, %4, %4, %5);")
          .arg(ApiDb::getChangesetsTableName())
          .arg(1)
          .arg(_changesetUserId)
          .arg(OsmApiDb::TIMESTAMP_FUNCTION)
          .arg(1)
          .toUtf8());
    }
  }

  _writeSequenceUpdateStatements(hdfsOutput + "/elementCounts", sqlFile);
}

void WriteOsmSqlStatementsDriver::_writeSequenceUpdateStatements(const QString elementCountsDir,
                                                                 const QString sqlFileLocation)
{
  //This should at most be 3 element count entries per reduce task, so the number entries to parse
  //shouldn't be prohibitively large.

  LOG_INFO("Merging element count files into one file...");
  const QString mergedElementCountFileTempPath =
    QDir::tempPath() + "/WriteOsmSqlStatementsDriver-" + QUuid::createUuid().toString();
  HadoopPipesUtils::mergeFilesToLocalFileSystem(
    elementCountsDir.toStdString(), mergedElementCountFileTempPath.toStdString());

  LOG_INFO("Determining total element counts from element count files...");
  QMap<ElementType::Type, long> elementCounts;
  elementCounts[ElementType::Node] = 0;
  elementCounts[ElementType::Way] = 0;
  elementCounts[ElementType::Relation] = 0;
  const QStringList elementCountsStrList =
    FileUtils::readFully(mergedElementCountFileTempPath).split("\n");
  for (int i = 0; i < elementCountsStrList.size(); i++)
  {
    const QString line = elementCountsStrList.at(i);
    if (!line.trimmed().isEmpty())
    {
      LOG_VART(line);
      const QStringList elementCountParts = line.split(";");
      assert(elementCountParts.size() == 2);
      ElementType elementType;
      if (elementCountParts[0] == "nodes")
      {
        elementType = ElementType::Node;
      }
      else if (elementCountParts[0] == "ways")
      {
        elementType = ElementType::Way;
      }
      else if (elementCountParts[0] == "relations")
      {
        elementType = ElementType::Relation;
      }
      else
      {
        throw HootException("Unsupported element type: " + elementCountParts[0]);
      }
      elementCounts[elementType.getEnum()] =
        elementCounts[elementType.getEnum()] + elementCountParts[1].toLong();
    }
  }
  if (elementCounts[ElementType::Way] == 0)
  {
    elementCounts[ElementType::Way] = 1;
  }
  if (elementCounts[ElementType::Relation] == 0)
  {
    elementCounts[ElementType::Relation] = 1;
  }
  LOG_VART(elementCounts[ElementType::Node]);
  LOG_VART(elementCounts[ElementType::Way]);
  LOG_VART(elementCounts[ElementType::Relation]);

  QStringList setValStatements;
  setValStatements.append(
    "SELECT pg_catalog.setval('current_nodes_id_seq', " +
    QString::number(elementCounts[ElementType::Node]) + ");\n");
  setValStatements.append(
    "SELECT pg_catalog.setval('current_ways_id_seq', " +
    QString::number(elementCounts[ElementType::Way]) + ");\n");
  setValStatements.append(
    "SELECT pg_catalog.setval('current_relations_id_seq', " +
    QString::number(elementCounts[ElementType::Relation]) + ");");
  if (!_destinationIsDatabase(_output) || !_outputFileCopyLocation.isEmpty())
  {
    LOG_INFO("Writing setval SQL statements to SQL file...");
    QFile outputSqlFile(sqlFileLocation);
    if (!outputSqlFile.open(QIODevice::Append))
    {
      throw HootException("Could not open SQL output file: " + sqlFileLocation);
    }
    QTextStream outStream(&outputSqlFile);
    for (int i = 0; i < setValStatements.size(); i++)
    {
      outStream << setValStatements.at(i);
    }
    outStream.flush();
    outputSqlFile.close();
  }
  else
  {
    LOG_INFO("Writing setval updates to database...");
    QString sql = "";
    for (int i = 0; i < setValStatements.size(); i++)
    {
      sql += setValStatements.at(i);
    }
    DbUtils::execNoPrepare(_database.getDB(), sql);
  }
}

void WriteOsmSqlStatementsDriver::_writeChangesetToSqlFile(const QString sqlFileLocation)
{
  LOG_DEBUG("Writing changeset to output SQL file...");
  const QString changesetHeader = _sqlFormatter->getChangesetSqlHeaderString();
  Envelope bounds;
  bounds.init();
  //the num changes here will not be accurate
  const QString changesetStr = _sqlFormatter->changesetToSqlString(1, _changesetUserId, 1, bounds);
  QFile outputSqlFile(sqlFileLocation);
  if (!outputSqlFile.open(QIODevice::Append))
  {
    throw HootException("Could not open SQL output file: " + sqlFileLocation);
  }
  QTextStream outStream(&outputSqlFile);
  outStream << changesetHeader;
  outStream << changesetStr;
  outStream << "\\.\n\n";
  outStream.flush();
  outputSqlFile.close();
}

QString WriteOsmSqlStatementsDriver::_getSqlFileOutputLocation(const QString hdfsOutput,
                                                               const QString userSpecifiedOutput) const
{
  if (!_destinationIsDatabase(userSpecifiedOutput))
  {
    return userSpecifiedOutput;
  }
  else if (!_outputFileCopyLocation.isEmpty())
  {
    return _outputFileCopyLocation;
  }
  return hdfsOutput + "/output.sql";
}

void WriteOsmSqlStatementsDriver::_runElementSqlStatementsWriteJob(const string& input,
                                                                   const string& output)
{
  LOG_INFO("Running element SQL statements write job...");

  Job job;
  job.setVerbose(Log::getInstance().getLevel() <= Log::Debug);
  job.setName("WriteElementSqlStatements");

  job.setInput(input);
  job.setOutput(output);

  job.setMapperClass(WriteOsmSqlStatementsMapper::className());
  job.setReducerClass(WriteOsmSqlStatementsReducer::className());
  job.setInputFormatClass(PbfInputFormat::className());
  job.setRecordReaderClass(PbfRecordReader::className());
  job.setRecordWriterClass(SqlStatementLineRecordWriter::className());

  // Adds all libraries in this directory to the job.
  job.addLibraryDirs(ConfigOptions().getHootHadoopLibpath());
  job.addFile(ConfPath::search("hoot.json").toStdString());
  // This library will be used to provide mapper/reducer classes and anything else referenced
  // by the factory.
  job.addPlugin(getenv("HOOT_HOME") + string("/lib/libHootHadoop.so.1"));
  _addDefaultJobSettings(job);

  job.getConfiguration().setLong("changesetUserId", _changesetUserId);
  job.getConfiguration().setLong("writeBufferSize", _fileOutputElementBufferSize);
  LOG_VARD(_execSqlWithMapreduce);
  if (_execSqlWithMapreduce && _destinationIsDatabase(_output))
  {
    job.getConfiguration().set("dbConnUrl", _output.toStdString());
  }
  const bool retainSqlFile = !_destinationIsDatabase(_output) || !_outputFileCopyLocation.isEmpty();
  job.getConfiguration().set("retainSqlFile", QString::number(retainSqlFile).toStdString());
  if (_numReduceTasks != -1)
  {
    job.getConfiguration().setInt("mapred.reduce.tasks", _numReduceTasks);
  }

  LOG_DEBUG(job.getJobTracker());
  job.run();
}

void WriteOsmSqlStatementsDriver::setConfiguration(const Settings& conf)
{
  const ConfigOptions confOptions(conf);

  setOutputFilesCopyLocation(confOptions.getApidbBulkInserterOutputFilesCopyLocation().trimmed());
  setFileOutputElementBufferSize(confOptions.getMaxElementsPerPartialMap());
  setChangesetUserId(confOptions.getChangesetUserId());
  setExecSqlWithMapreduce(confOptions.getBigConvertExecSqlWithMapreduce());
  setNumReduceTasks(confOptions.getBigConvertNumReduceTasks());

  LOG_VART(_outputFileCopyLocation);
  LOG_VART(_fileOutputElementBufferSize);
  LOG_VART(_changesetUserId);
  LOG_VART(_execSqlWithMapreduce);
  LOG_VART(_numReduceTasks);
}

}
