/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmApiDbBulkWriter.h"

#include <QDateTime>
#include <QFileInfo>
#include <QStringBuilder>
#include <QDir>

#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/DbUtils.h>

#include <tgs/System/SystemInfo.h>

namespace hoot
{

using namespace Tgs;

unsigned int OsmApiDbBulkWriter::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmApiDbBulkWriter)

OsmApiDbBulkWriter::OsmApiDbBulkWriter() :
_outputDelimiter("\t"),
_tempFileDataPassCtr(0)
{
  _reset();
  _sectionNames = _createSectionNameList();
  setConfiguration(conf());
}

OsmApiDbBulkWriter::~OsmApiDbBulkWriter()
{
  close();
}

bool OsmApiDbBulkWriter::isSupported(QString urlStr)
{
  LOG_VARD(urlStr);
  QUrl url(urlStr);
  //if we ever want any other writers that the convert command invokes to output sql or csv, then
  //this will have to be made more specific
  return urlStr.endsWith(".sql") || urlStr.endsWith(".csv") || _database.isSupported(url);
}

void OsmApiDbBulkWriter::open(QString url)
{
  _outputUrl = url;

  // Make sure we're not already open and the URL is valid
  if (!isSupported(url))
  {
    throw HootException(QString("Could not open URL ") + url);
  }

  _verifyStartingIds();
  _verifyApp();
  _verifyOutputCopySettings();
  if (_usingDatabase() && _database.getDB().isOpen())
  {
    throw HootException(
      QString("Database already open.  Close the existing database connection before opening ") +
      QString("a new one.  URL: ") + url);
  }
  if (_usingDatabase())
  {
    _database.open(_outputUrl);
  }
  _verifyDependencies();
}

void OsmApiDbBulkWriter::_verifyStartingIds()
{
  if (_idMappings.startingNodeId < 1 || _idMappings.startingWayId < 1 ||
      _idMappings.startingRelationId < 1)
  {
    throw HootException(
      "Invalid element starting ID specified.  IDs must be greater than or equal to 1.");
  }

  //this setting overrides any specified starting ids
  if (_reserveRecordIdsBeforeWritingData)
  {
    if (_idMappings.startingNodeId > 1 || _idMappings.startingWayId > 1 ||
        _idMappings.startingRelationId > 1)
    {
      LOG_WARN(
        "Custom starting element IDs ignored due to reserve record IDs before writing data enabled.");
    }
    _idMappings.startingNodeId = 1;
    _idMappings.startingWayId = 1;
    _idMappings.startingRelationId = 1;
  }
  else
  {
    _idMappings.currentNodeId = _idMappings.startingNodeId;
    _idMappings.currentWayId = _idMappings.startingWayId;
    _idMappings.currentRelationId = _idMappings.startingRelationId;
  }
}

void OsmApiDbBulkWriter::_verifyDependencies()
{
  if (system(QString(_writerApp + " --version > /dev/null").toStdString().c_str()) != 0)
  {
    throw HootException(
      "Unable to access the " + _writerApp + " application.  Is " + _writerApp + " installed?");
  }

  if (_usingDatabase() && _writerApp == "pg_bulkload" && !_database.hasExtension(_writerApp))
  {
    throw HootException(
      QString("The " + _writerApp + " application was selected for database writing. ") +
      QString("To use this writer application, the database being written to must have the ") +
      QString(_writerApp + " extension installed."));
  }
}

void OsmApiDbBulkWriter::_verifyApp()
{
  if (_writerApp != "psql" && _writerApp != "pg_bulkload")
  {
    throw HootException("Invalid OSM API database writer application: " + _writerApp);
  }
  else if (_writerApp == "psql" &&
           (_disableWriteAheadLogging || _writeMultiThreaded || _disableConstraints))
  {
    throw HootException(
      QString("OSM API database writer app is 'psql'.  The following options may only be ") +
      QString("enabled with the 'pg_bulkload' writer application: disabling write ahead ") +
      QString("logging, multi-threaded writes, disabling table constraint checks."));
  }
  else if (_writerApp == "psql" && _outputUrl.endsWith(".csv"))
  {
    throw HootException(
      QString("When using the 'psql' writer application, the only valid output formats are ") +
      QString("SQL file (.sql) or OSM API database (osmapidb://)."));
  }
  else if (_writerApp == "pg_bulkload" && _outputUrl.endsWith(".sql"))
  {
    throw HootException(
      QString("When using the 'pg_bulkload' writer application, the only valid output ") +
      QString("formats are CSV file (.sql) or OSM API database (osmapidb://)."));
  }

  if (_writerApp == "psql")
  {
    _outputDelimiter = "\t";
  }
  else
  {
  _outputDelimiter = ",";
  }
}

void OsmApiDbBulkWriter::_verifyOutputCopySettings()
{
  if (_destinationIsDatabase() && !_outputFilesCopyLocation.isEmpty())
  {
    QFileInfo outputCopyLocationInfo(_outputFilesCopyLocation);
    if (_writerApp == "pg_bulkload" && !outputCopyLocationInfo.completeSuffix().isEmpty())
    {
      throw HootException(
        QString("Output file copy location should be set to a directory when using the ") +
        QString("'pg_bulkload' writer application.  Location specified: ") +
                _outputFilesCopyLocation);
    }
    else if (_writerApp == "psql" &&
             !outputCopyLocationInfo.completeSuffix().toLower().endsWith("sql"))
    {
      throw HootException(
        QString("Output file copy location should be set to a SQL file (.sql) when using the ") +
        QString("'psql' writer application.  Location specified: ") + _outputFilesCopyLocation);
    }
  }
}

void OsmApiDbBulkWriter::close()
{
  _closeOutputFiles();
  if (_usingDatabase())
  {
    _database.close();
  }

  _reset();
  _sectionNames = _createSectionNameList();
  setConfiguration(conf());
}

void OsmApiDbBulkWriter::_closeOutputFiles()
{
  for (QStringList::const_iterator sectionNamesItr = _sectionNames.begin();
       sectionNamesItr != _sectionNames.end(); sectionNamesItr++)
  {
    if (_outputSections[*sectionNamesItr].first)
    {
      _outputSections[*sectionNamesItr].first->close();
    }
  }

  if (_sqlOutputMasterFile)
  {
    _sqlOutputMasterFile->close();
  }
}

QString OsmApiDbBulkWriter::_formatPotentiallyLargeNumber(const long number)
{
  //I want to see comma separators...probably a better way to handle this...will go with this for
  //now.
  const QLocale& cLocale = QLocale::c();
  QString ss = cLocale.toString((qulonglong)number);
  ss.replace(cLocale.groupSeparator(), ',');
  return ss;
}

void OsmApiDbBulkWriter::_logStats(const bool debug)
{
  QStringList messages;
  messages.append(QString("\tNodes: ") + _formatPotentiallyLargeNumber(_writeStats.nodesWritten));
  messages.append(
    QString("\tNode tags: ") + _formatPotentiallyLargeNumber(_writeStats.nodeTagsWritten));
  messages.append(QString("\tWays: ") + _formatPotentiallyLargeNumber(_writeStats.waysWritten));
  messages.append(
    QString("\tWay nodes: ") + _formatPotentiallyLargeNumber(_writeStats.wayNodesWritten));
  messages.append(
    QString("\tWay tags: ") + _formatPotentiallyLargeNumber(_writeStats.wayTagsWritten));
  messages.append(
    QString("\tRelations: ") + _formatPotentiallyLargeNumber(_writeStats.relationsWritten));
  messages.append(
    QString("\tRelation members: ") +
    _formatPotentiallyLargeNumber(_writeStats.relationMembersWritten));
  messages.append(
    QString("\tRelation tags: ") + _formatPotentiallyLargeNumber(_writeStats.relationTagsWritten));
  messages.append(
    QString("\tUnresolved relations: ") +
    _formatPotentiallyLargeNumber(_writeStats.relationMembersUnresolved));
  messages.append(
    QString("\tChangesets: ") + _formatPotentiallyLargeNumber(_changesetData.changesetsWritten));
  messages.append(
    QString("\tChangeset change size (each): ") + _formatPotentiallyLargeNumber(_maxChangesetSize));
  messages.append(
    QString("\tExecutable SQL records: ") +
    _formatPotentiallyLargeNumber(_getTotalRecordsWritten()));

  for (int i = 0; i < messages.size(); i++)
  {
    if (debug)
    {
      LOG_DEBUG(messages.at(i));
    }
    else
    {
      LOG_INFO(messages.at(i));
    }
  }
}

unsigned int OsmApiDbBulkWriter::_numberOfTempFileDataPasses() const
{
  unsigned int numPasses = 1;
  if (_writerApp == "psql")
  {
    numPasses++;
  }
  else if (_writerApp != "psql" && _destinationIsDatabase() && _reserveRecordIdsBeforeWritingData)
  {
    numPasses++;
  }
  return numPasses;
}

void OsmApiDbBulkWriter::_updateRecordLinesWithIdOffsetInCsvFiles()
{
  _timer->restart();
  _tempFileDataPassCtr++;
  LOG_INFO(
    "Updating record IDs for CSV files .  (data pass #" << _tempFileDataPassCtr << " of " <<
    _numberOfTempFileDataPasses() << "...");
  LOG_VART(_sectionNames.size());
  LOG_VART(_outputSections.size());

  long progressLineCtr = 0;
  for (QStringList::const_iterator it = _sectionNames.begin(); it != _sectionNames.end(); ++it)
  {
    if (_outputSections.find(*it) == _outputSections.end())
    {
      LOG_DEBUG("No data for table " + *it);
      continue;
    }

    // Write updated IDs to a new copy of each file

    QFile tempInputFile(_outputSections[*it].first->fileName());
    shared_ptr<QTemporaryFile> newCsvFile(new QTemporaryFile());
    try
    {
      if (!_destinationIsDatabase() || !_outputFilesCopyLocation.isEmpty())
      {
        newCsvFile->setAutoRemove(false);
      }
      if (!newCsvFile->open())
      {
        throw HootException(
          "Could not open new CSV file for updated ID output: " + newCsvFile->fileName());
      }
      QTextStream outStream(newCsvFile.get());

      LOG_DEBUG("Opening temp file: " << _outputSections[*it].first->fileName());
      if (tempInputFile.open(QIODevice::ReadOnly))
      {
        LOG_DEBUG("Parsing temp file for table: " << *it << "...");
        QTextStream inStream(&tempInputFile);
        QString line;
        long lineCtr = 0;
        do
        {
          line = inStream.readLine();
          LOG_VART(line.left(100));

          if (!line.isEmpty())
          {
            if (_destinationIsDatabase() && _reserveRecordIdsBeforeWritingData)
            {
              _updateRecordLineWithIdOffset(*it, line);
            }
            progressLineCtr++;
            outStream << line << "\n";
            lineCtr++;
          }

          if (lineCtr == _fileOutputLineBufferSize)
          {
            LOG_TRACE("Flushing records to combined file " << newCsvFile->fileName() << "...");
            outStream.flush();
            lineCtr = 0;
          }

          if (progressLineCtr > 0 && (progressLineCtr % _statusUpdateInterval == 0))
          {
            PROGRESS_INFO(
              "Parsed " <<
              _formatPotentiallyLargeNumber(progressLineCtr) << "/" <<
              _formatPotentiallyLargeNumber(
                _getTotalRecordsWritten() - _changesetData.changesetsWritten) <<
              " CSV file lines.");
          }
        }
        while (!line.isNull());
        outStream.flush();

        tempInputFile.close();
        LOG_DEBUG("Closing and removing old temp file and adding updated one for " << *it << "...");
        _outputSections[*it].first->close();
        _outputSections[*it].first->remove();
        if (!newCsvFile->flush())
        {
          throw HootException("Could not flush tempfile for table " + *it);
        }
        newCsvFile->close();
        _outputSections[*it] =
          pair<shared_ptr<QTemporaryFile>, shared_ptr<QTextStream> >(
            newCsvFile, shared_ptr<QTextStream>());

        QFileInfo outputInfo(_outputSections[*it].first->fileName());
        LOG_VART(SystemInfo::humanReadable(outputInfo.size()));
      }
      else
      {
        throw HootException("Unable to open temp input file: " + tempInputFile.fileName());
      }
    }
    catch (const Exception& e)
    {
      tempInputFile.close();
      newCsvFile->close();
      throw e;
    }

    LOG_DEBUG("Wrote contents of section " << *it);
  }

  LOG_INFO(
    "CSV ID update write complete.  (data pass #" << _tempFileDataPassCtr << " of " <<
    _numberOfTempFileDataPasses() << "...");
  LOG_DEBUG(
    "Parsed " << _formatPotentiallyLargeNumber(progressLineCtr) << " total CSV file lines.");
}

void OsmApiDbBulkWriter::_flushTempStreams()
{
  for (QStringList::const_iterator it = _sectionNames.begin(); it != _sectionNames.end(); ++it)
  {
    if (_outputSections.find(*it) == _outputSections.end())
    {
      LOG_DEBUG("No data for table " + *it);
      continue;
    }

    LOG_DEBUG("Flushing section " << *it << " to file " << _outputSections[*it].first->fileName());
    if ((*it != "byte_order_mark") && _writerApp == "psql")
    {
      LOG_TRACE("Writing closing byte order mark to stream...");
      *(_outputSections[*it].second) << QString("\\.\n\n\n");
    }
    // Flush any residual content from text stream/file
    _outputSections[*it].second->flush();
    if (!_outputSections[*it].first->flush())
    {
      throw HootException("Could not flush tempfile for table " + *it);
    }
  }
}

void OsmApiDbBulkWriter::finalizePartial()
{
  LOG_INFO(
    "Input records parsed (data pass #" << _tempFileDataPassCtr << " of " <<
    _numberOfTempFileDataPasses() << ".  Time elapsed: " << _secondsToDhms(_timer->elapsed()));

  //go ahead and clear out some of the data structures we don't need anymore
  _idMappings.nodeIdMap.reset();
  _idMappings.wayIdMap.reset();
  _idMappings.relationIdMap.reset();
  _unresolvedRefs.unresolvedWaynodeRefs.reset();
  _unresolvedRefs.unresolvedRelationRefs.reset();

  if (_writeStats.nodesWritten == 0)
  {
    LOG_DEBUG("No input data was written to temporary files.");
    return;
  }

  // Do we have an unfinished changeset that needs flushing?
  if (_changesetData.changesInChangeset > 0)
  {
    _writeChangesetToStream();
  }
  //If there was only one changeset written total, this won't have yet been incremented, so do it
  //now.
  if (_changesetData.changesetsWritten == 0)
  {
    _changesetData.changesetsWritten++;
  }
  _flushTempStreams();

  if (_destinationIsDatabase() && _reserveRecordIdsBeforeWritingData)
  {
    //Get the latest id sequences in case other writes have occurred while we were parsing the input
    //data, and reserve those ids out so we can exec the sql w/o risk of future ID conflicts
    //*after* we update the ids in the sql file in the next step.
    _reserveIdsInDb();
  }
  else
  {
    //If the output is a sql file, the setval statements will be written to the file later.  If
    //the output is csv files, then ID sequences must be managed manually outside of this writer.
    LOG_DEBUG("Skipping record ID reservation in database due to configuration or output type...");
  }

  if (_writerApp == "psql")
  {
    _sqlOutputMasterFile.reset(new QTemporaryFile());
    if (!_destinationIsDatabase() || !_outputFilesCopyLocation.isEmpty())
    {
      _sqlOutputMasterFile->setAutoRemove(false);
    }

    // Start initial section that holds nothing but UTF-8 byte-order mark (BOM)
    _createOutputFile("byte_order_mark", "\n", true);

    //combine all the element/changeset temp files that were written during partial streaming into
    //one file and update the ids in the SQL file according to the id sequences previously reserved
    //out
    _writeCombinedSqlFile();
  }
  else if (_destinationIsDatabase() && _reserveRecordIdsBeforeWritingData)
  {
    //update the ids in the CSV file according to the id sequences previously reserved
    _updateRecordLinesWithIdOffsetInCsvFiles();
  }

  LOG_INFO("Temporary file write stats:");
  _logStats();

  if (_outputUrl.startsWith("osmapidb://"))
  {
    _writeDataToDb();
    LOG_INFO("Final database write stats:");
  }
  else
  {
    LOG_DEBUG("Skipping SQL execution against database due to configuration...");
    LOG_INFO("Final temporary file write stats:");
  }
  _logStats();

  //retain the output file(s) if that option was selected; this needs to happen after the database
  //write
  if (_destinationIsDatabase() && !_outputFilesCopyLocation.isEmpty())
  {
    _retainOutputFiles();
  }
}

bool OsmApiDbBulkWriter::_usingDatabase() const
{
  return _destinationIsDatabase() ||
         ((_outputUrl.endsWith(".sql") || _outputUrl.endsWith(".csv")) &&
            _reserveRecordIdsBeforeWritingData);
}

bool OsmApiDbBulkWriter::_destinationIsDatabase() const
{
  return _outputUrl.startsWith("osmapidb://");
}

void OsmApiDbBulkWriter::_retainOutputFilesPsql()
{
  QFile copyFile(_outputFilesCopyLocation);
  if (copyFile.exists())
  {
    copyFile.remove();
  }
  QFileInfo sqlOutputFileInfo(_sqlOutputMasterFile->fileName());
  LOG_INFO(
    "Moving " << SystemInfo::humanReadable(sqlOutputFileInfo.size()) << " SQL output file " <<
    " at " << _sqlOutputMasterFile->fileName() << " to " << _outputFilesCopyLocation << "...");
  if (!_sqlOutputMasterFile->rename(_sqlOutputMasterFile->fileName(), _outputFilesCopyLocation))
  {
    LOG_WARN("Unable to move SQL output file to " << _outputFilesCopyLocation << ".");
  }
}

void OsmApiDbBulkWriter::_retainOutputFilesPgBulk()
{
  LOG_INFO("Copying temporary CSV output files to " << _outputFilesCopyLocation);

  if (!QDir().mkpath(_outputFilesCopyLocation))
  {
    LOG_ERROR("Unable to create temp copy file output directory.");
    return;
  }

  QDir outputDir(_outputFilesCopyLocation);
  for (QStringList::const_iterator sectionNamesItr = _sectionNames.begin();
       sectionNamesItr != _sectionNames.end(); sectionNamesItr++)
  {
    if (_outputSections[*sectionNamesItr].first)
    {
      const QString outputPath =
        outputDir.path() + "/" + outputDir.dirName() + "-" + *sectionNamesItr + ".csv";
      const QString fileToCopyPath = (_outputSections[*sectionNamesItr].first)->fileName();
      QFile copyDestFile(outputPath);
      if (copyDestFile.exists())
      {
        copyDestFile.remove();
      }
      QFileInfo fileToCopyInfo(fileToCopyPath);
      LOG_DEBUG(
        "Copying " << SystemInfo::humanReadable(fileToCopyInfo.size()) << " CSV output file " <<
        "at " << fileToCopyPath << " to " << outputPath << "...");
      if (!(_outputSections[*sectionNamesItr].first)->copy(outputPath))
      {
        LOG_WARN("Unable to copy CSV output file to " << outputPath << ".");
      }
      //TODO: temp
//      if (!copyDestFile.setPermissions(QFile::ReadOther))
//      {
//        LOG_WARN("Unable to set permissions on " << outputPath);
//      }
      if (!(_outputSections[*sectionNamesItr].first)->remove())
      {
        LOG_WARN(
          "Unable to remove temp file " << (_outputSections[*sectionNamesItr].first)->fileName());
      }
    }
  }
}

void OsmApiDbBulkWriter::_retainOutputFiles()
{
  if (_writerApp == "psql")
  {
    _retainOutputFilesPsql();
  }
  else
  {
    _retainOutputFilesPgBulk();
  }
}

void OsmApiDbBulkWriter::_writeDataToDbPsql()
{
  _timer->restart();
  //I believe a COPY header is created whether there are any records to copy for the table or not,
  //which is why the number of copy statements to be executed is hardcoded here.  Might be cleaner
  //to not write the header if there are no records to copy for the table...
  LOG_INFO(
    "Executing element SQL for " << _formatPotentiallyLargeNumber(_getTotalRecordsWritten()) <<
    " records.  17 separate SQL COPY statements will be executed...");

  //exec element sql against the db; Using psql here b/c it is doing buffered reads against the
  //sql file, so no need doing the extra work to handle buffering the sql read manually and
  //applying it to a QSqlQuery.
  const QMap<QString, QString> dbUrlParts = ApiDb::getDbUrlParts(_outputUrl);
  QString cmd = "export PGPASSWORD=" + dbUrlParts["password"] + "; psql";
  if (!(Log::getInstance().getLevel() <= Log::Info))
  {
    cmd += " --quiet";
  }
  cmd += " " + ApiDb::getPsqlString(_outputUrl) + " -f " + _sqlOutputMasterFile->fileName();
  if (!(Log::getInstance().getLevel() <= Log::Info))
  {
    cmd += " > /dev/null";
  }
  LOG_DEBUG(cmd);
  if (system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Failed executing bulk element SQL write against the OSM API database.");
  }
  LOG_INFO("SQL execution complete.  Time elapsed: " << _secondsToDhms(_timer->elapsed()));
}

void OsmApiDbBulkWriter::_writeDataToDbPgBulk()
{
  _timer->restart();
  bool someDataNotLoaded = false;
  LOG_INFO(
    "Writing CSV data for " << _formatPotentiallyLargeNumber(_getTotalRecordsWritten()) <<
    " records.  " << _outputSections.size() - 1 << " CSV files will be written to the database...");

  //Do we want to remove these every time?
  if (!_pgBulkLogPath.isEmpty())
  {
    QFile pgBulkOutputLog(_pgBulkLogPath);
    if (pgBulkOutputLog.exists())
    {
      pgBulkOutputLog.remove();
    }
  }
  if (!_pgBulkBadRecordsLogPath.isEmpty())
  {
    QFile pgBulkBadRecordsOutputLog(_pgBulkBadRecordsLogPath);
    if (pgBulkBadRecordsOutputLog.exists())
    {
      pgBulkBadRecordsOutputLog.remove();
    }
  }

  for (QStringList::const_iterator sectionNamesItr = _sectionNames.begin();
       sectionNamesItr != _sectionNames.end(); sectionNamesItr++)
  {
    if (*sectionNamesItr != "byte_order_mark" && _outputSections[*sectionNamesItr].first)
    {
      LOG_DEBUG("Closing temp file for " << *sectionNamesItr << "...");
      _outputSections[*sectionNamesItr].second.reset();
      if (!_outputSections[*sectionNamesItr].first->exists())
      {
        throw HootException(
          "Temp file " + _outputSections[*sectionNamesItr].first->fileName() + " does not exist.");
      }

      const QMap<QString, QString> dbUrlParts = ApiDb::getDbUrlParts(_outputUrl);

      //TODO:
      // - this needs to work for the hoot user
      // - need to remove vagrant home path from executable

      if (!(_outputSections[*sectionNamesItr]).first->setPermissions(QFile::ReadOther))
      {
        LOG_WARN(
          "Unable to set permissions on " << (_outputSections[*sectionNamesItr]).first->fileName());
      }
      QString cmd =
        "sudo -u postgres /home/vagrant/pg_bulkload/bin/pg_bulkload -d " +
        dbUrlParts["database"] + " -O " + *sectionNamesItr + " -i " +
        (_outputSections[*sectionNamesItr]).first->fileName();

//      QString cmd = "export PGPASSWORD=" + dbUrlParts["password"] + ";";
//      cmd += " export PGDATABASE=" + dbUrlParts["database"] + ";";
//      cmd += " export PGHOST=" + dbUrlParts["host"] + ";";
//      cmd += " export PGPORT=" + dbUrlParts["port"] + ";";
//      cmd += " export PGUSER=" + dbUrlParts["user"] + ";";
//      cmd +=
//        " pg_bulkload -d " + dbUrlParts["database"] + " -h " + dbUrlParts["host"] + " -p " +
//        dbUrlParts["port"] + " -U " + dbUrlParts["user"] + " -W " + dbUrlParts["password"] +
//        " -O " + *sectionNamesItr + " -i " +
//         (_outputSections[*sectionNamesItr]).first->fileName();

      if (!_pgBulkLogPath.isEmpty())
      {
        cmd += " -l " + _pgBulkLogPath;
      }
      if (!_pgBulkBadRecordsLogPath.isEmpty())
      {
        cmd += " -P " + _pgBulkBadRecordsLogPath;
      }
      if (!_disableConstraints)
      {
        cmd += " -o \"CHECK_CONSTRAINTS=YES\"";
      }
      if (!_disableWriteAheadLogging)
      {
        cmd += " -o \"WRITER=BUFFERED\"";
      }
      if (_writeMultiThreaded)
      {
        cmd += " -o \"MULTI_PROCESS=YES\"";
      }
      if (Log::getInstance().getLevel() <= Log::Trace)
      {
        cmd += " -o \"VERBOSE=YES\"";
      }
      switch (Log::getInstance().getLevel())
      {
        case Log::Trace:
          cmd += " -E DEBUG";
          break;
        case Log::Debug:
          cmd += " -E DEBUG";
          break;
        case Log::Info:
          //info is the default
          break;
        case Log::Warn:
          cmd += " -E WARNING";
          break;
        case Log::Error:
          cmd += " -E ERROR";
          break;
        default:
          throw HootException("Unsupported log level.");
      }
      if (!(Log::getInstance().getLevel() <= Log::Info))
      {
        cmd += " > /dev/null";
      }
      LOG_DEBUG(cmd);

      LOG_INFO("Writing CSV data for " << *sectionNamesItr << "...");
      const int status = system(cmd.toStdString().c_str());
      if (status != 0)
      {
        if (status == 3)
        {
          LOG_WARN("Some data could not be loaded.");
          someDataNotLoaded = true;
        }
        else
        {
          throw HootException(
            "Failed executing record write for table " + *sectionNamesItr +
            " against the OSM API database: " + _outputUrl + ".  Error code: " +
            QString::number(status));
        }
      }
      LOG_DEBUG("Wrote CSV data for " << *sectionNamesItr << ".");

      if (_destinationIsDatabase() && _outputFilesCopyLocation.isEmpty())
      {
        LOG_DEBUG("Removing temp file for " << *sectionNamesItr << "...");
        _outputSections[*sectionNamesItr].first->remove();
        _outputSections[*sectionNamesItr].first.reset();
      }
    }
  }

  LOG_INFO("SQL execution complete.  Time elapsed: " << _secondsToDhms(_timer->elapsed()));
  if (someDataNotLoaded)
  {
    LOG_WARN("Some data was not loaded.");
  }
}

void OsmApiDbBulkWriter::_writeDataToDb()
{
  if (_writerApp == "psql")
  {
    _writeDataToDbPsql();
  }
  else
  {
    _writeDataToDbPgBulk();
  }
}

void OsmApiDbBulkWriter::_writeCombinedSqlFile()
{
  _timer->restart();
  _tempFileDataPassCtr++;
  LOG_INFO(
    "Writing combined SQL output file to " << _sqlOutputMasterFile->fileName() <<
    ".  (data pass #" << _tempFileDataPassCtr << " of " << _numberOfTempFileDataPasses() << "...");
  LOG_VART(_sectionNames.size());
  LOG_VART(_outputSections.size());

  if (!_sqlOutputMasterFile->open())
  {
    throw HootException(
      "Could not open temp file for SQL output: " + _sqlOutputMasterFile->fileName());
  }

  QTextStream outStream(_sqlOutputMasterFile.get());
  outStream << "BEGIN TRANSACTION;\n\n";
  outStream.flush();

  if (!_reserveRecordIdsBeforeWritingData)
  {
    //We're not reserving the ID ranges in the database, so we'll write the appropriate setval
    //statements to the sql output here for applying at a later time.
    QString reserveElementIdsSql;
    _writeSequenceUpdatesToStream(_changesetData.currentChangesetId - 1,
                                 _idMappings.currentNodeId - 1,
                                 _idMappings.currentWayId - 1,
                                 _idMappings.currentRelationId - 1,
                                 reserveElementIdsSql);
    LOG_VART(reserveElementIdsSql);
    outStream << reserveElementIdsSql;
    outStream.flush();
  }

  long progressLineCtr = 0;
  for (QStringList::const_iterator it = _sectionNames.begin(); it != _sectionNames.end(); ++it)
  {
    if (_outputSections.find(*it) == _outputSections.end())
    {
      LOG_DEBUG("No data for table " + *it);
      continue;
    }

    // Append contents of section subfiles to output file
    QFile tempInputFile(_outputSections[*it].first->fileName());
    try
    {
      LOG_DEBUG("Opening temp file: " << _outputSections[*it].first->fileName());
      if (tempInputFile.open(QIODevice::ReadOnly))
      {
        LOG_DEBUG("Parsing temp file for table: " << *it << "...");
        QTextStream inStream(&tempInputFile);
        QString line;
        long lineCtr = 0;
        do
        {
          line = inStream.readLine();
          LOG_VART(line.left(100));

          if (!line.contains("COPY") && !line.isEmpty() && line != "\\.")
          {
            if (_destinationIsDatabase() && _reserveRecordIdsBeforeWritingData)
            {
              _updateRecordLineWithIdOffset(*it, line);
            }
            progressLineCtr++;
          }
          outStream << line << "\n";
          lineCtr++;

          if (lineCtr == _fileOutputLineBufferSize)
          {
            LOG_TRACE(
              "Flushing records to combined file " << _sqlOutputMasterFile->fileName() << "...");
            outStream.flush();
            lineCtr = 0;
          }

          if (progressLineCtr > 0 && (progressLineCtr % _statusUpdateInterval == 0))
          {
            //TODO: changesets is throwing off the progress totals here...not sure why...don't
            //care that much right now, since the changeset count is far outnumbered by the
            //size of the rest of the data
            PROGRESS_INFO(
              "Parsed " <<
              _formatPotentiallyLargeNumber(progressLineCtr) << "/" <<
              _formatPotentiallyLargeNumber(
                _getTotalRecordsWritten() - _changesetData.changesetsWritten) <<
              " SQL file lines.");
          }
        }
        while (!line.isNull());
        outStream.flush();

        tempInputFile.close();
        LOG_DEBUG("Closing and removing temp file for " << *it << "...");
        _outputSections[*it].second.reset();
        _outputSections[*it].first->close();
        _outputSections[*it].first->remove();
        _outputSections[*it].first.reset();
      }
      else
      {
        throw HootException("Unable to open temp input file: " + tempInputFile.fileName());
      }
    }
    catch (const Exception& e)
    {
      tempInputFile.close();
      throw e;
    }

    LOG_DEBUG("Wrote contents of section " << *it);
  }
  outStream << "COMMIT;";
  outStream.flush();
  _sqlOutputMasterFile->flush();
  _sqlOutputMasterFile->close();

  LOG_INFO(
    "SQL file write complete.  (data pass #" << _tempFileDataPassCtr << " of " <<
    _numberOfTempFileDataPasses() << "...");
  LOG_DEBUG(
    "Parsed " << _formatPotentiallyLargeNumber(progressLineCtr) << " total SQL file lines.");
  QFileInfo outputInfo(_sqlOutputMasterFile->fileName());
  LOG_VART(SystemInfo::humanReadable(outputInfo.size()));
}

void OsmApiDbBulkWriter::_updateRecordLineWithIdOffset(const QString tableName, QString& recordLine)
{
  LOG_TRACE("Updating ID offset for line...");
  LOG_VART(tableName);

  QStringList lineParts = recordLine.split(_outputDelimiter);

  //update any element/changeset ID reference by adding the current ID offset to it

  bool lineUpdated = true;
  if (tableName == ApiDb::getChangesetsTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toLong() + _changesetData.currentChangesetId);
  }
  else if (tableName == ApiDb::getCurrentNodesTableName() ||
           tableName == ApiDb::getNodesTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toLong() + _idMappings.currentNodeId);
    lineParts[3] = QString::number(lineParts[3].toLong() + _changesetData.currentChangesetId);
  }
  else if (tableName == ApiDb::getCurrentWaysTableName() || tableName == ApiDb::getWaysTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toLong() + _idMappings.currentWayId);
    lineParts[1] = QString::number(lineParts[1].toLong() + _changesetData.currentChangesetId);
  }
  else if (tableName == ApiDb::getCurrentWayNodesTableName() ||
           tableName == ApiDb::getWayNodesTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toLong() + _idMappings.currentWayId);
    lineParts[1] = QString::number(lineParts[1].toLong() + _idMappings.currentNodeId);
  }
  else if (tableName == ApiDb::getCurrentRelationsTableName() ||
           tableName == ApiDb::getRelationsTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toLong() + _idMappings.currentRelationId);
    lineParts[1] = QString::number(lineParts[1].toLong() + _changesetData.currentChangesetId);
  }
  else if (tableName == ApiDb::getCurrentRelationMembersTableName() ||
           tableName == ApiDb::getRelationMembersTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toLong() + _idMappings.currentRelationId);
    const long memberId = lineParts[2].toLong();
    if (lineParts[1].toLower() == "node")
    {
      lineParts[2] = QString::number(memberId + _idMappings.currentNodeId);
    }
    else if (lineParts[1].toLower() == "way")
    {
      lineParts[2] = QString::number(memberId + _idMappings.currentWayId);
    }
  }
  else if (tableName == ApiDb::getCurrentNodeTagsTableName() ||
           tableName == ApiDb::getNodeTagsTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toLong() + _idMappings.currentNodeId);
  }
  else if (tableName == ApiDb::getCurrentWayTagsTableName() ||
           tableName == ApiDb::getWayTagsTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toLong() + _idMappings.currentWayId);
  }
  else if (tableName == ApiDb::getCurrentRelationTagsTableName() ||
           tableName == ApiDb::getRelationTagsTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toLong() + _idMappings.currentRelationId);
  }
  else
  {
    lineUpdated = false;
  }

  if (!lineUpdated)
  {
    throw HootException("Bad line passed to record ID offsetter: " + recordLine.left(100));
  }

  recordLine = lineParts.join(_outputDelimiter);
  LOG_TRACE("ID offset updated for line: " << recordLine.left(100));
}

void OsmApiDbBulkWriter::_reserveIdsInDb()
{
  //this assumes the input data has already been written out to file once and _writeStats has valid
  //values for the number of elements written
  if (_writeStats.nodesWritten == 0)
  {
    throw HootException(
      QString("OSM API database bulk writer cannot reserve element ID range if no elements ") +
      QString("have been parsed from the input."));
  }

  //get the next available id from the db for all sequence types
  _incrementAndGetLatestIdsFromDb();
  //generate setval statements to reserve each of the id ranges out from use by others
  QString reserveElementIdsSql;
  _writeSequenceUpdatesToStream(_changesetData.currentChangesetId + _changesetData.changesetsWritten,
                                _idMappings.currentNodeId + _writeStats.nodesWritten,
                                _idMappings.currentWayId + _writeStats.waysWritten,
                                _idMappings.currentRelationId + _writeStats.relationsWritten,
                                 reserveElementIdsSql);
  LOG_VART(reserveElementIdsSql);

  LOG_INFO("Writing sequence ID updates to database to reserve record IDs...");
  _database.transaction();
  DbUtils::execNoPrepare(_database.getDB(), reserveElementIdsSql);
  _database.commit();
  LOG_DEBUG("Sequence updates written to database.");
}

long OsmApiDbBulkWriter::_getTotalRecordsWritten() const
{
  //the multiplications by 2 account for the fact that two records are written for each type (other
  //than changesets), one for the current tables and one for the historical tables
  return
    (_writeStats.nodesWritten * 2) + (_writeStats.nodeTagsWritten * 2) +
    (_writeStats.relationMembersWritten * 2) + (_writeStats.relationsWritten * 2) +
    (_writeStats.relationTagsWritten * 2) + (_writeStats.wayNodesWritten * 2) +
    (_writeStats.waysWritten * 2) + (_writeStats.wayTagsWritten * 2) +
    _changesetData.changesetsWritten;
}

void OsmApiDbBulkWriter::_incrementAndGetLatestIdsFromDb()
{
  LOG_DEBUG("Incrementing current ID sequences in database and updating local record IDs...");
  _idMappings.currentNodeId = _database.getNextId(ElementType::Node);
  _idMappings.currentWayId = _database.getNextId(ElementType::Way);
  _idMappings.currentRelationId = _database.getNextId(ElementType::Relation);
  _changesetData.currentChangesetId = _database.getNextId(ApiDb::getChangesetsTableName());
  LOG_VART(_changesetData.currentChangesetId);
  LOG_VART(_idMappings.currentNodeId);
  LOG_VART(_idMappings.currentWayId);
  LOG_VART(_idMappings.currentRelationId);
}

void OsmApiDbBulkWriter::writePartial(const ConstNodePtr& node)
{
  _timer.reset(new QElapsedTimer());
  _timer->start();

  if (_writeStats.nodesWritten == 0)
  {
    _tempFileDataPassCtr++;
    LOG_INFO(
      "Streaming elements from input to temporary file outputs.  (data pass #" <<
      _tempFileDataPassCtr << " of " << _numberOfTempFileDataPasses() << "...");
  }

  LOG_VART(node);

  //Since we're only creating elements, the changeset bounds is simply the combined bounds
  //of all the nodes involved in the changeset.

  //TODO: See #1451.  This changeset bounds calculation actually won't work when ways or relations
  //are written in separate changesets than the nodes they reference.  Since we're streaming the
  //elements, there's no way to get back to the bounds information.  This bug has always been here,
  //but just recently noticed.

  _changesetData.changesetBounds.expandToInclude(node->getX(), node->getY());
  LOG_VART(_changesetData.changesetBounds.toString());

  if (_writeStats.nodesWritten == 0)
  {
    _createNodeOutputFiles();
    _idMappings.nodeIdMap.reset(new BigMap<long, long>());
  }

  long nodeDbId;
  // Do we already know about this node?
  if (_idMappings.nodeIdMap->contains(node->getId()))
  {
    throw NotImplementedException("Writer class does not support update operations.");
  }
  // Have to establish new mapping
  nodeDbId = _establishNewIdMapping(node->getElementId());
  LOG_VART(nodeDbId);

  _writeNodeToStream(node, nodeDbId);
  _writeTagsToStream(node->getTags(), ElementType::Node, nodeDbId,
    _outputSections[ApiDb::getCurrentNodeTagsTableName()].second,
    _outputSections[ApiDb::getNodeTagsTableName()].second);
  _writeStats.nodesWritten++;
  _writeStats.nodeTagsWritten += node->getTags().size();

  _incrementChangesInChangeset();

  _checkUnresolvedReferences(node, nodeDbId);

  if ((_writeStats.nodesWritten / 2) % _fileOutputLineBufferSize == 0)
  {
    LOG_TRACE(
      "Flushing " << _formatPotentiallyLargeNumber(_fileOutputLineBufferSize) <<
      " nodes to temp files...");
    _outputSections[ApiDb::getCurrentNodesTableName()].second->flush();
    _outputSections[ApiDb::getNodesTableName()].second->flush();
  }
  if ((node->getTags().size() > 0) &&
      ((_writeStats.nodeTagsWritten / 2) % _fileOutputLineBufferSize == 0))
  {
    LOG_TRACE(
      "Flushing " << _formatPotentiallyLargeNumber(_fileOutputLineBufferSize) <<
      " node tags to temp files...");
    _outputSections[ApiDb::getCurrentNodeTagsTableName()].second->flush();
    _outputSections[ApiDb::getNodeTagsTableName()].second->flush();
  }

  if (_writeStats.nodesWritten % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO(
      "Parsed " << _formatPotentiallyLargeNumber(_writeStats.nodesWritten) << " nodes from input.");
  }
}

QString OsmApiDbBulkWriter::_secondsToDhms(const qint64 durationInMilliseconds) const
{
  //TODO: move to utility class
  QString res;
  int duration = (int)(durationInMilliseconds / 1000);
  const int seconds = (int)(duration % 60);
  duration /= 60;
  const int minutes = (int)(duration % 60);
  duration /= 60;
  const int hours = (int)(duration % 24);
  const int days = (int)(duration / 24);
  if ((hours == 0) && (days == 0))
  {
    return res.sprintf("%02d:%02d", minutes, seconds);
  }
  if (days == 0)
  {
    return res.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
  }
  return res.sprintf("%dd%02d:%02d:%02d", days, hours, minutes, seconds);
}

void OsmApiDbBulkWriter::writePartial(const ConstWayPtr& way)
{
  LOG_VART(way);

  if (_writeStats.waysWritten == 0)
  {
    _createWayOutputFiles();
    _idMappings.wayIdMap.reset(new BigMap<long, long>());
  }

  long wayDbId;
  // Do we already know about this way?
  if (_idMappings.wayIdMap->contains(way->getId()))
  {
    throw NotImplementedException("Writer class does not support update operations");
  }
  // Have to establish new mapping
  wayDbId = _establishNewIdMapping(way->getElementId());
  LOG_VART(wayDbId);

  _writeWayToStream(wayDbId);
  _writeWayNodesToStream(_idMappings.wayIdMap->at(way->getId()), way->getNodeIds());
  _writeTagsToStream(way->getTags(), ElementType::Way, wayDbId,
    _outputSections[ApiDb::getCurrentWayTagsTableName()].second,
    _outputSections[ApiDb::getWayTagsTableName()].second);
  _writeStats.waysWritten++;
  _writeStats.wayTagsWritten += way->getTags().size();
  _writeStats.wayNodesWritten += way->getNodeIds().size();

  _incrementChangesInChangeset();

  _checkUnresolvedReferences(way, wayDbId);

  if ((_writeStats.waysWritten / 2) % _fileOutputLineBufferSize == 0)
  {
    LOG_TRACE(
      "Flushing " << _formatPotentiallyLargeNumber(_fileOutputLineBufferSize) <<
      " ways to temp files...");
    _outputSections[ApiDb::getCurrentWaysTableName()].second->flush();
    _outputSections[ApiDb::getWaysTableName()].second->flush();
  }
  if ((way->getTags().size() > 0) &&
      ((_writeStats.wayTagsWritten / 2) % _fileOutputLineBufferSize == 0))
  {
    LOG_TRACE(
      "Flushing " << _formatPotentiallyLargeNumber(_fileOutputLineBufferSize) <<
      " way tags to temp files...");
    _outputSections[ApiDb::getCurrentWayTagsTableName()].second->flush();
    _outputSections[ApiDb::getWayTagsTableName()].second->flush();
  }
  if ((_writeStats.wayNodesWritten / 2) % _fileOutputLineBufferSize == 0)
  {
    LOG_TRACE(
      "Flushing " << _formatPotentiallyLargeNumber(_fileOutputLineBufferSize) <<
      " way nodes to temp files...");
    _outputSections[ApiDb::getCurrentWayNodesTableName()].second->flush();
    _outputSections[ApiDb::getWayNodesTableName()].second->flush();
  }

  if (_writeStats.waysWritten % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO(
      "Parsed " << _formatPotentiallyLargeNumber(_writeStats.waysWritten) << " ways from input.");
  }
}

void OsmApiDbBulkWriter::writePartial(const ConstRelationPtr& relation)
{
  LOG_VART(relation);

  if (_writeStats.relationsWritten == 0)
  {
    _createRelationOutputFiles();
    _idMappings.relationIdMap.reset(new BigMap<long, long>());
  }

  long relationDbId;
  // Do we already know about this node?
  if (_idMappings.relationIdMap->contains(relation->getId()))
  {
    throw hoot::NotImplementedException("Writer class does not support update operations");
  }
  // Have to establish new mapping
  relationDbId = _establishNewIdMapping(relation->getElementId());
  LOG_VART(relationDbId);

  _writeRelationToStream(relationDbId);
  _writeRelationMembersToStream(relation);
  _writeTagsToStream(relation->getTags(), ElementType::Relation, relationDbId,
    _outputSections[ApiDb::getCurrentRelationTagsTableName()].second,
    _outputSections[ApiDb::getRelationTagsTableName()].second);
  _writeStats.relationsWritten++;
  _writeStats.relationTagsWritten += relation->getTags().size();
  _writeStats.relationMembersWritten += relation->getMembers().size();

  _incrementChangesInChangeset();

  _checkUnresolvedReferences(relation, relationDbId);

  if ((_writeStats.relationsWritten / 2) % _fileOutputLineBufferSize == 0)
  {
    LOG_TRACE(
      "Flushing " << _formatPotentiallyLargeNumber(_fileOutputLineBufferSize) <<
      " relations to temp files...");
    _outputSections[ApiDb::getCurrentRelationsTableName()].second->flush();
    _outputSections[ApiDb::getRelationsTableName()].second->flush();
  }
  if ((relation->getTags().size() > 0) &&
      ((_writeStats.relationTagsWritten / 2) % _fileOutputLineBufferSize == 0))
  {
    LOG_TRACE(
      "Flushing " << _formatPotentiallyLargeNumber(_fileOutputLineBufferSize) <<
      " relation tags to temp files...");
    _outputSections[ApiDb::getCurrentRelationTagsTableName()].second->flush();
    _outputSections[ApiDb::getRelationTagsTableName()].second->flush();
  }
  if ((_writeStats.relationMembersWritten / 2) % _fileOutputLineBufferSize == 0)
  {
    LOG_TRACE(
      "Flushing " << _formatPotentiallyLargeNumber(_fileOutputLineBufferSize) <<
      " relation members to temp files...");
    _outputSections[ApiDb::getCurrentRelationMembersTableName()].second->flush();
    _outputSections[ApiDb::getRelationMembersTableName()].second->flush();
  }

  if (_writeStats.relationsWritten % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO(
      "Parsed " << _formatPotentiallyLargeNumber(_writeStats.relationsWritten) <<
      " relations from input.");
  }
}

void OsmApiDbBulkWriter::setConfiguration(const Settings& conf)
{
  const ConfigOptions confOptions(conf);

  setDisableWriteAheadLogging(confOptions.getOsmapidbBulkWriterDisableWriteAheadLogging());
  setWriteMultithreaded(confOptions.getOsmapidbBulkWriterMultithreaded());
  setDisableConstraints(confOptions.getOsmapidbBulkWriterDisableConstraints());
  setOutputFilesCopyLocation(confOptions.getOsmapidbBulkWriterOutputFilesCopyLocation().trimmed());
  _changesetData.changesetUserId = confOptions.getChangesetUserId();
  setFileOutputLineBufferSize(confOptions.getOsmapidbBulkWriterFileOutputBufferMaxLineSize());
  setStatusUpdateInterval(confOptions.getOsmapidbBulkWriterFileOutputStatusUpdateInterval());
  setMaxChangesetSize(confOptions.getChangesetMaxSize());
  setPgBulkloadLogPath(confOptions.getOsmapidbBulkWriterPgbulkloadLogPath().trimmed());
  setPgBulkloadBadRecordsLogPath(
    confOptions.getOsmapidbBulkWriterPgbulkloadBadRecordsLogPath().trimmed());
  setWriterApp(confOptions.getOsmapidbBulkWriterApp().toLower().trimmed());
  setReserveRecordIdsBeforeWritingData(
    confOptions.getOsmapidbBulkWriterReserveRecordIdsBeforeWritingData());
  setStartingNodeId(confOptions.getOsmapidbBulkWriterStartingNodeId());
  setStartingWayId(confOptions.getOsmapidbBulkWriterStartingWayId());
  setStartingRelationId(confOptions.getOsmapidbBulkWriterStartingRelationId());

  LOG_VART(_changesetData.changesetUserId);
  LOG_VART(_fileOutputLineBufferSize);
  LOG_VART(_statusUpdateInterval);
  LOG_VART(_maxChangesetSize);
  LOG_VART(_outputFilesCopyLocation);
  LOG_VART(_disableWriteAheadLogging);
  LOG_VART(_writeMultiThreaded);
  LOG_VART(_outputUrl);
  LOG_VART(_disableConstraints);
  LOG_VART(_outputDelimiter);
  LOG_VART(_pgBulkLogPath);
  LOG_VART(_pgBulkBadRecordsLogPath);
  LOG_VART(_writerApp);
  LOG_VART(_reserveRecordIdsBeforeWritingData);
  LOG_VART(_idMappings.startingNodeId);
  LOG_VART(_changesetData.changesetUserId);
  LOG_VART(_idMappings.startingWayId);
}

QStringList OsmApiDbBulkWriter::_createSectionNameList()
{
  QStringList sections;
  sections.push_back(QString("byte_order_mark"));
  sections.push_back(ApiDb::getChangesetsTableName());
  sections.push_back(ApiDb::getCurrentNodesTableName());
  sections.push_back(ApiDb::getCurrentNodeTagsTableName());
  sections.push_back(ApiDb::getNodesTableName());
  sections.push_back(ApiDb::getNodeTagsTableName());
  sections.push_back(ApiDb::getCurrentWaysTableName());
  sections.push_back(ApiDb::getCurrentWayNodesTableName());
  sections.push_back(ApiDb::getCurrentWayTagsTableName());
  sections.push_back(ApiDb::getWaysTableName());
  sections.push_back(ApiDb::getWayNodesTableName());
  sections.push_back(ApiDb::getWayTagsTableName());
  sections.push_back(ApiDb::getCurrentRelationsTableName());
  sections.push_back(ApiDb::getCurrentRelationMembersTableName());
  sections.push_back(ApiDb::getCurrentRelationTagsTableName());
  sections.push_back(ApiDb::getRelationsTableName());
  sections.push_back(ApiDb::getRelationMembersTableName());
  sections.push_back(ApiDb::getRelationTagsTableName());
  return sections;
}

void OsmApiDbBulkWriter::_createNodeOutputFiles()
{
  if (_writerApp == "psql")
  {
    _createOutputFile(
      ApiDb::getCurrentNodesTableName(),
      "COPY " + ApiDb::getCurrentNodesTableName() +
      " (id, latitude, longitude, changeset_id, visible, \"timestamp\", tile, version) " +
      "FROM stdin;\n");
    _createOutputFile(
      ApiDb::getCurrentNodeTagsTableName(),
      "COPY " + ApiDb::getCurrentNodeTagsTableName() + " (node_id, k, v) FROM stdin;\n");

    _createOutputFile(
      ApiDb::getNodesTableName(),
      "COPY " + ApiDb::getNodesTableName() +
      " (node_id, latitude, longitude, changeset_id, visible, \"timestamp\", tile, version, " +
      "redaction_id) FROM stdin;\n");
    _createOutputFile(
      ApiDb::getNodeTagsTableName(),
      //yes, this one is different than the others...see explanation in header file
      "COPY " + ApiDb::getNodeTagsTableName() + " (node_id, version, k, v) FROM stdin;\n");
  }
  else
  {
    _createOutputFile(ApiDb::getCurrentNodesTableName());
    _createOutputFile(ApiDb::getCurrentNodeTagsTableName());

    _createOutputFile(ApiDb::getNodesTableName());
    _createOutputFile(ApiDb::getNodeTagsTableName());
  }
}

void OsmApiDbBulkWriter::_reset()
{
  LOG_TRACE("Resetting variables...");

  _writeStats.nodesWritten = 0;
  _writeStats.nodeTagsWritten = 0;
  _writeStats.waysWritten = 0;
  _writeStats.wayNodesWritten = 0;
  _writeStats.wayTagsWritten = 0;
  _writeStats.relationsWritten = 0;
  _writeStats.relationMembersWritten = 0;
  _writeStats.relationMembersUnresolved = 0;
  _writeStats.relationTagsWritten = 0;

  _changesetData.changesetUserId = -1;
  _changesetData.currentChangesetId = 1;
  _changesetData.changesInChangeset = 0;
  _changesetData.changesetsWritten = 0;

  _idMappings.startingNodeId = 1;
  _idMappings.currentNodeId = 1;
  _idMappings.nodeIdMap.reset();

  _idMappings.startingWayId = 1;
  _idMappings.currentWayId = 1;
  _idMappings.wayIdMap.reset();

  _idMappings.startingRelationId = 1;
  _idMappings.currentRelationId = 1;
  _idMappings.relationIdMap.reset();

  _unresolvedRefs.unresolvedWaynodeRefs.reset();
  _unresolvedRefs.unresolvedRelationRefs.reset();

  _outputSections.clear();
  _sectionNames.erase(_sectionNames.begin(), _sectionNames.end());
}

long OsmApiDbBulkWriter::_establishNewIdMapping(const ElementId& sourceId)
{
  long dbIdentifier;

  switch (sourceId.getType().getEnum())
  {
  case ElementType::Node:
    dbIdentifier = _idMappings.currentNodeId;
    _idMappings.nodeIdMap->insert(sourceId.getId(), dbIdentifier);
    _idMappings.currentNodeId++;
    break;

  case ElementType::Way:
    dbIdentifier = _idMappings.currentWayId;
    _idMappings.wayIdMap->insert(sourceId.getId(), dbIdentifier);
    _idMappings.currentWayId++;
    break;

  case ElementType::Relation:
    dbIdentifier = _idMappings.currentRelationId;
    _idMappings.relationIdMap->insert(sourceId.getId(), dbIdentifier);
    _idMappings.currentRelationId++;
    break;

  default:
    throw NotImplementedException("Unsupported element type.");
  }

  return dbIdentifier;
}

void OsmApiDbBulkWriter::_writeNodeToStream(const ConstNodePtr& node, const long nodeDbId)
{
  const double nodeY = node->getY();
  const double nodeX = node->getX();
  //TODO: should be able to use OsmApiDb::toOsmApiDbCoord here instead
  //const long nodeYNanodegrees = OsmApiDb::toOsmApiDbCoord(nodeY);
  //const long nodeXNanodegrees = OsmApiDb::toOsmApiDbCoord(nodeX);
  const int nodeYNanodegrees = _convertDegreesToNanodegrees(nodeY);
  const int nodeXNanodegrees = _convertDegreesToNanodegrees(nodeX);
  if ((nodeYNanodegrees < -900000000) || (nodeYNanodegrees > 900000000))
  {
    throw HootException(
      QString("Invalid latitude conversion, Y = %1 to %2").arg(
        QString::number(nodeY), QString::number(nodeYNanodegrees)));
  }
  if ((nodeXNanodegrees < -1800000000) || (nodeXNanodegrees > 1800000000))
  {
    throw HootException(
      QString("Invalid longitude conversion, X = %1 to %2").arg(
        QString::number(nodeX), QString::number(nodeXNanodegrees)));
  }
  const int changesetId = _changesetData.currentChangesetId;
  const QString datestring =
    QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz");
  const QString tileNumberString(QString::number(ApiDb::tileForPoint(nodeY, nodeX)));

  QString currentFormatString = CURRENT_NODES_OUTPUT_FORMAT_STRING;
  QString outputLine =
    currentFormatString.replace("\t", _outputDelimiter).arg(
      QString::number(nodeDbId),
      QString::number(nodeYNanodegrees),
      QString::number(nodeXNanodegrees),
      QString::number(changesetId),
      datestring,
      tileNumberString);
  *(_outputSections[ApiDb::getCurrentNodesTableName()].second) << outputLine;

  QString historicalFormatString = HISTORICAL_NODES_OUTPUT_FORMAT_STRING;
  historicalFormatString.replace("\t", _outputDelimiter);
  if (_writerApp == "pg_bulkload")
  {
    historicalFormatString.replace("\\N", "");
  }
  outputLine =
    historicalFormatString.arg(
      QString::number(nodeDbId),
      QString::number(nodeYNanodegrees),
      QString::number(nodeXNanodegrees),
      QString::number(changesetId),
      datestring,
      tileNumberString);
  *(_outputSections[ApiDb::getNodesTableName()].second) << outputLine;
}

unsigned int OsmApiDbBulkWriter::_convertDegreesToNanodegrees(const double degrees) const
{
  return round(degrees * ApiDb::COORDINATE_SCALE);
}

void OsmApiDbBulkWriter::_writeTagsToStream(const Tags& tags, const ElementType::Type& elementType,
                                            const long dbId,
                                            shared_ptr<QTextStream>& currentTable,
                                            shared_ptr<QTextStream>& historicalTable)
{
  const QString dbIdString(QString::number(dbId));
  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    QString key = _escapeCopyToData(it.key());
    //pg_bulkload doesn't seem to be tolerating the empty data
    if (key.trimmed().isEmpty())
    {
      key = "<empty>";
    }
    LOG_VART(key);
    QString value = _escapeCopyToData(it.value());
    LOG_VART(value);
    if (value.trimmed().isEmpty())
    {
      value = "<empty>";
    }

    QString currentFormatString = CURRENT_TAGS_OUTPUT_FORMAT_STRING;
    *currentTable <<
      currentFormatString.replace("\t", _outputDelimiter)
        .arg(dbIdString, key, value);
    QString historicalFormatString = HISTORICAL_TAGS_OUTPUT_FORMAT_STRING;
    if (elementType == ElementType::Node)
    {
      //see explanation for this silliness in the header file
      historicalFormatString = HISTORICAL_NODE_TAGS_OUTPUT_FORMAT_STRING;
    }
    *historicalTable <<
      historicalFormatString.replace("\t", _outputDelimiter)
        .arg(dbIdString, key, value);
  }
}

void OsmApiDbBulkWriter::_createWayOutputFiles()
{
  if (_writerApp == "psql")
  {
    _createOutputFile(
      ApiDb::getCurrentWaysTableName(),
      "COPY " + ApiDb::getCurrentWaysTableName() +
      " (id, changeset_id, \"timestamp\", visible, version) FROM stdin;\n");
    _createOutputFile(
      ApiDb::getCurrentWayTagsTableName(),
      "COPY " + ApiDb::getCurrentWayTagsTableName() + " (way_id, k, v) FROM stdin;\n");
    _createOutputFile(
      ApiDb::getCurrentWayNodesTableName(),
      "COPY " + ApiDb::getCurrentWayNodesTableName() +
      " (way_id, node_id, sequence_id) FROM stdin;\n");

    _createOutputFile(
      ApiDb::getWaysTableName(),
      "COPY " + ApiDb::getWaysTableName() +
      " (way_id, changeset_id, \"timestamp\", version, visible, redaction_id) FROM stdin;\n");
    _createOutputFile(
      ApiDb::getWayTagsTableName(),
      "COPY " + ApiDb::getWayTagsTableName() +
      " (way_id, k, v, version) FROM stdin;\n");
    _createOutputFile(
      ApiDb::getWayNodesTableName(),
      "COPY " + ApiDb::getWayNodesTableName() +
      " (way_id, node_id, version, sequence_id) FROM stdin;\n");
  }
  else
  {
    //we're writing csv data (not sql), so no table headers needed

    _createOutputFile(ApiDb::getCurrentWaysTableName());
    _createOutputFile(ApiDb::getCurrentWayTagsTableName());
    _createOutputFile(ApiDb::getCurrentWayNodesTableName());

    _createOutputFile(ApiDb::getWaysTableName());
    _createOutputFile(ApiDb::getWayTagsTableName());
    _createOutputFile(ApiDb::getWayNodesTableName());
  }
}

void OsmApiDbBulkWriter::_writeWayToStream(const long wayDbId)
{
  const int changesetId = _changesetData.currentChangesetId;
  const QString datestring =
    QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz");

  QString currentFormatString = CURRENT_WAYS_OUTPUT_FORMAT_STRING;
  QString outputLine =
    currentFormatString.replace("\t", _outputDelimiter)
      .arg(wayDbId)
      .arg(changesetId)
      .arg(datestring);
  *(_outputSections[ApiDb::getCurrentWaysTableName()].second) << outputLine;

  QString historicalFormatString = HISTORICAL_WAYS_OUTPUT_FORMAT_STRING;
  historicalFormatString.replace("\t", _outputDelimiter);
  if (_writerApp == "pg_bulkload")
  {
    historicalFormatString.replace("\\N", "");
  }
  outputLine =
    historicalFormatString
      .arg(wayDbId)
      .arg(changesetId)
      .arg(datestring);
  *(_outputSections[ApiDb::getWaysTableName()].second) << outputLine;
}

void OsmApiDbBulkWriter::_writeWayNodesToStream(const long dbWayId, const vector<long>& waynodeIds)
{
  unsigned int nodeIndex = 1;
  const QString dbWayIdString( QString::number(dbWayId));
  for (vector<long>::const_iterator it = waynodeIds.begin(); it != waynodeIds.end(); ++it)
  {
    if (_idMappings.nodeIdMap->contains(*it))
    {
      const QString dbNodeIdString = QString::number(_idMappings.nodeIdMap->at(*it));
      const QString nodeIndexString(QString::number(nodeIndex));
      QString currentFormatString = CURRENT_WAY_NODES_OUTPUT_FORMAT_STRING;
      *_outputSections[ApiDb::getCurrentWayNodesTableName()].second <<
        currentFormatString.replace("\t", _outputDelimiter)
          .arg(dbWayIdString, dbNodeIdString, nodeIndexString);
      QString historicalFormatString = HISTORICAL_WAY_NODES_OUTPUT_FORMAT_STRING;
      *_outputSections[ApiDb::getWayNodesTableName()].second <<
        historicalFormatString.replace("\t", _outputDelimiter).arg(
          dbWayIdString, dbNodeIdString, nodeIndexString);
    }
    else
    {
      throw NotImplementedException(
        "Unresolved waynodes are not supported.  " +
        QString("Way %1 has reference to unknown node ID %2").arg(dbWayId, *it));
    }
    ++nodeIndex;
  }
}

void OsmApiDbBulkWriter::_createRelationOutputFiles()
{
  if (_writerApp == "psql")
  {
    _createOutputFile(
      ApiDb::getCurrentRelationsTableName(),
      "COPY " + ApiDb::getCurrentRelationsTableName() +
      " (id, changeset_id, \"timestamp\", visible, version) FROM stdin;\n");
    _createOutputFile(
      ApiDb::getCurrentRelationTagsTableName(),
      "COPY " + ApiDb::getCurrentRelationTagsTableName() + " (relation_id, k, v) FROM stdin;\n");
    _createOutputFile(
      ApiDb::getCurrentRelationMembersTableName(),
      "COPY " + ApiDb::getCurrentRelationMembersTableName() +
      " (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;\n");

    _createOutputFile(
      ApiDb::getRelationsTableName(),
      "COPY " + ApiDb::getRelationsTableName() +
      " (relation_id, changeset_id, \"timestamp\", version, visible, redaction_id) FROM stdin;\n");
    _createOutputFile(
      ApiDb::getRelationTagsTableName(),
      "COPY " + ApiDb::getRelationTagsTableName() +
      " (relation_id, k, v, version) FROM stdin;\n");
    _createOutputFile(
      ApiDb::getRelationMembersTableName(),
      "COPY " + ApiDb::getRelationMembersTableName() +
      " (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;\n");
  }
  else
  {
    //we're writing csv data (not sql), so no table headers needed

    _createOutputFile(ApiDb::getCurrentRelationsTableName());
    _createOutputFile(ApiDb::getCurrentRelationTagsTableName());
    _createOutputFile(ApiDb::getCurrentRelationMembersTableName());

    _createOutputFile(ApiDb::getRelationsTableName());
    _createOutputFile(ApiDb::getRelationTagsTableName());
    _createOutputFile(ApiDb::getRelationMembersTableName());
  }
}

void OsmApiDbBulkWriter::_writeRelationToStream(const long relationDbId)
{
  const int changesetId = _changesetData.currentChangesetId;
  const QString datestring =
  QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz");

  QString currentFormatString = CURRENT_RELATIONS_OUTPUT_FORMAT_STRING;
  QString outputLine =
    currentFormatString.replace("\t", _outputDelimiter)
      .arg(relationDbId)
      .arg(changesetId)
      .arg(datestring);
  *(_outputSections[ApiDb::getCurrentRelationsTableName()].second) << outputLine;

  QString historicalFormatString = HISTORICAL_RELATIONS_OUTPUT_FORMAT_STRING;
  historicalFormatString.replace("\t", _outputDelimiter);
  if (_writerApp == "pg_bulkload")
  {
    historicalFormatString.replace("\\N", "");
  }
  outputLine =
    historicalFormatString
      .arg(relationDbId)
      .arg(changesetId)
      .arg(datestring);
  *(_outputSections[ApiDb::getRelationsTableName()].second) << outputLine;
}

void OsmApiDbBulkWriter::_writeRelationMembersToStream(const ConstRelationPtr& relation)
{
  unsigned int memberSequenceIndex = 1;
  const long relationId = relation->getId();
  const long dbRelationId = _idMappings.relationIdMap->at(relationId);
  const vector<RelationData::Entry> relationMembers = relation->getMembers();
  shared_ptr<BigMap<long, long> > knownElementMap;

  for (vector<RelationData::Entry>::const_iterator it = relationMembers.begin();
       it != relationMembers.end(); ++it)
  {
    const ElementId memberElementId = it->getElementId();

    switch (memberElementId.getType().getEnum())
    {
    case ElementType::Node:
      knownElementMap = _idMappings.nodeIdMap;
      break;

    case ElementType::Way:
      knownElementMap = _idMappings.wayIdMap;
      break;

    case ElementType::Relation:
      knownElementMap = _idMappings.relationIdMap;
      break;

    default:
      throw HootException("Unsupported element member type");
    }

    if (knownElementMap && knownElementMap->contains(memberElementId.getId()))
    {
      _writeRelationMemberToStream(
        dbRelationId, *it, knownElementMap->at(memberElementId.getId()), memberSequenceIndex);
    }
    else
    {
      if (!_unresolvedRefs.unresolvedRelationRefs)
      {
        _unresolvedRefs.unresolvedRelationRefs.reset(
          new map<ElementId, UnresolvedRelationReference>());
      }

      const UnresolvedRelationReference relationRef =
        { relationId, dbRelationId, *it, memberSequenceIndex };

      _unresolvedRefs.unresolvedRelationRefs->insert(
        pair<ElementId, UnresolvedRelationReference>(memberElementId, relationRef));
    }

    ++memberSequenceIndex;
  }
}

void OsmApiDbBulkWriter::_writeRelationMemberToStream(const long sourceRelationDbId,
                                                      const RelationData::Entry& memberEntry,
                                                      const long memberDbId,
                                                      const unsigned int memberSequenceIndex)
{
  QString memberType;
  const ElementId memberElementId = memberEntry.getElementId();

  switch (memberElementId.getType().getEnum())
  {
  case ElementType::Node:
    memberType = "Node";
    break;
  case ElementType::Way:
    memberType = "Way";
    break;
  case ElementType::Relation:
    memberType = "Relation";
    break;
  default:
    throw HootException("Unsupported element member type: " + memberType);
  }

  const QString dbRelationIdString(QString::number(sourceRelationDbId));
  const QString memberRefIdString(QString::number(memberDbId));
  const QString memberSequenceString(QString::number(memberSequenceIndex));
  QString memberRole = _escapeCopyToData(memberEntry.getRole());
  //pg_bulkload doesn't seem to be handling empty data
  if (memberRole.trimmed().isEmpty())
  {
    memberRole = "<no role>";
  }

  QString currentFormatString = CURRENT_RELATION_MEMBERS_OUTPUT_FORMAT_STRING;
  *_outputSections[ApiDb::getCurrentRelationMembersTableName()].second <<
    currentFormatString.replace("\t", _outputDelimiter).arg(
      dbRelationIdString, memberType, memberRefIdString, memberRole, memberSequenceString);
  QString historicalFormatString = HISTORICAL_RELATION_MEMBERS_OUTPUT_FORMAT_STRING;
  *_outputSections[ApiDb::getRelationMembersTableName()].second <<
    historicalFormatString.replace("\t", _outputDelimiter).arg(
      dbRelationIdString, memberType, memberRefIdString, memberRole, memberSequenceString);

  _writeStats.relationMembersWritten++;
}

void OsmApiDbBulkWriter::_createOutputFile(const QString tableName, const QString header,
                                           const bool addByteOrderMark)
{
  QString msg = "Creating output file";
  if (!header.trimmed().isEmpty())
  {
    msg += " and writing table header";
  }
  msg += "...";
  LOG_TRACE(msg);

  shared_ptr<QTemporaryFile> tempfile(new QTemporaryFile());
  if (!_destinationIsDatabase() && _writerApp == "pg_bulkload" &&
      !_outputFilesCopyLocation.isEmpty())
  {
    tempfile->setAutoRemove(false);
  }
  if (!tempfile->open())
  {
    throw HootException(
      "Could not open temp file at: " + tempfile->fileName() + " for contents of table " +
      tableName);
  }
  _outputSections[tableName] =
    pair<shared_ptr<QTemporaryFile>, shared_ptr<QTextStream> >(
      tempfile, shared_ptr<QTextStream>(new QTextStream(tempfile.get())));

  // Database is encoded in UTF-8, so force encoding as otherwise file is in local
  //    Western encoding which goes poorly for a lot of countries
  _outputSections[tableName].second->setCodec("UTF-8");

  // First table written out should have byte order mark to help identifify content as UTF-8
  if (addByteOrderMark)
  {
    _outputSections[tableName].second->setGenerateByteOrderMark(true);
  }

  if (!header.trimmed().isEmpty())
  {
    *(_outputSections[tableName].second) << header;
  }
}

void OsmApiDbBulkWriter::_incrementChangesInChangeset()
{
  _changesetData.changesInChangeset++;
  if (_changesetData.changesInChangeset == _maxChangesetSize)
  {
    LOG_VART(_changesetData.changesInChangeset);
    _writeChangesetToStream();
    _changesetData.currentChangesetId++;
    LOG_VART(_changesetData.currentChangesetId);
    _changesetData.changesInChangeset = 0;
    _changesetData.changesetBounds.init();
    _changesetData.changesetsWritten++;
    LOG_VART(_changesetData.changesetsWritten);
  }
}

void OsmApiDbBulkWriter::_checkUnresolvedReferences(const ConstElementPtr& element,
                                                    const long elementDbId)
{
  // Regardless of type, may be referenced in relation
  if (_unresolvedRefs.unresolvedRelationRefs)
  {
    map<ElementId, UnresolvedRelationReference >::iterator relationRef =
      _unresolvedRefs.unresolvedRelationRefs->find(element->getElementId());

    if (relationRef != _unresolvedRefs.unresolvedRelationRefs->end())
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Found unresolved relation member ref!:");
        LOG_WARN(QString( "Relation ID ") % QString::number(relationRef->second.sourceRelationId) %
          QString(" (DB ID=") % QString::number(relationRef->second.sourceDbRelationId) %
           QString(") has ref to ") % relationRef->second.relationMemberData.toString());
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;

      _writeRelationMemberToStream(
        relationRef->second.sourceDbRelationId, relationRef->second.relationMemberData,
        elementDbId, relationRef->second.relationMemberSequenceId);

      // Remove entry from unresolved list
      _unresolvedRefs.unresolvedRelationRefs->erase(relationRef);
    }
  }

  // If newly written element is a node, check noderefs as well
  if (element->getElementType().getEnum() == ElementType::Node)
  {
    if (_unresolvedRefs.unresolvedWaynodeRefs &&
        _unresolvedRefs.unresolvedWaynodeRefs->contains(element->getId()))
    {
      throw NotImplementedException(
        "Found unresolved waynode ref!  For node: " + QString::number(element->getId()) +
        " Need to insert waynode ref that is now resolved");
    }
  }
}

QString OsmApiDbBulkWriter::_escapeCopyToData(const QString stringToOutput) const
{
  QString escapedString(stringToOutput);
  // Escape any special characters as required by
  //    http://www.postgresql.org/docs/9.2/static/sql-copy.html
  escapedString.replace(QChar(92), QString("\\\\"));  // Escape single backslashes first
  escapedString.replace(QChar(8), QString("\\b"));
  escapedString.replace(QChar(9), QString("\\t"));
  escapedString.replace(QChar(10), QString("\\n"));
  escapedString.replace(QChar(11), QString("\\v"));
  escapedString.replace(QChar(12), QString("\\f"));
  escapedString.replace(QChar(13), QString("\\r"));
  //pg_bulkload data comes from csv, so need a different delimiter than a comma
  if (_writerApp == "pg_bulkload")
  {
    escapedString.replace(QChar(44), QString(";"));
  }
  return escapedString;
}

void OsmApiDbBulkWriter::_writeChangesetToStream()
{
  LOG_VART(_changesetData.changesetUserId);
  LOG_VART(_changesetData.currentChangesetId);

  if (_changesetData.changesetUserId == -1)
  {
    throw HootException(
      "Invalid changeset user ID: " + QString::number(_changesetData.changesetUserId));
  }

  if (!_outputSections[ApiDb::getChangesetsTableName()].second)
  {
    if (_writerApp == "psql")
    {
      _createOutputFile(
        ApiDb::getChangesetsTableName(),
        "COPY " + ApiDb::getChangesetsTableName() +
        " (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes) " +
        "FROM stdin;\n");
    }
    else
    {
      _createOutputFile(ApiDb::getChangesetsTableName(), "");
    }
  }

  const QString datestring =
    QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz");

  QString formatString = CHANGESETS_OUTPUT_FORMAT_STRING;
  *_outputSections[ApiDb::getChangesetsTableName()].second <<
    formatString.replace("\t", _outputDelimiter).arg(
      QString::number(_changesetData.currentChangesetId),
      QString::number(_changesetData.changesetUserId),
      datestring,
      QString::number(
        (qlonglong)OsmApiDb::toOsmApiDbCoord(_changesetData.changesetBounds.getMinY())),
      QString::number(
        (qlonglong)OsmApiDb::toOsmApiDbCoord(_changesetData.changesetBounds.getMaxY())),
      QString::number(
        (qlonglong)OsmApiDb::toOsmApiDbCoord(_changesetData.changesetBounds.getMinX())),
      QString::number(
        (qlonglong)OsmApiDb::toOsmApiDbCoord(_changesetData.changesetBounds.getMaxX())),
      datestring,
      QString::number(_changesetData.changesInChangeset));
}

void OsmApiDbBulkWriter::_writeSequenceUpdatesToStream(const long changesetId, const long nodeId,
                                                       const long wayId, const long relationId,
                                                       QString& outputStr)
{
  LOG_DEBUG("Writing sequence updates stream...");

  shared_ptr<QTextStream> sequenceUpdatesStream(new QTextStream(&outputStr));
  const QString sequenceUpdateFormat("SELECT pg_catalog.setval('%1', %2);\n");

  //At least one changeset and some nodes should always be written by a write operation; ways
  //and relations are optional.

  assert(changesetId > 0);
  *sequenceUpdatesStream <<
    sequenceUpdateFormat.arg(ApiDb::getChangesetsSequenceName(), QString::number(changesetId));

  assert(nodeId > 0);
  *sequenceUpdatesStream <<
    sequenceUpdateFormat.arg(ApiDb::getCurrentNodesSequenceName(), QString::number(nodeId));

  if (wayId > 0)
  {
    *sequenceUpdatesStream <<
      sequenceUpdateFormat.arg(ApiDb::getCurrentWaysSequenceName(), QString::number(wayId));
  }

  if (relationId > 0)
  {
    *sequenceUpdatesStream <<
      sequenceUpdateFormat.arg(
        ApiDb::getCurrentRelationsSequenceName(), QString::number(relationId)) << "\n\n";
  }
}

}
