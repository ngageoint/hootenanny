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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmApiDbBulkInserter.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/io/OsmApiDbSqlStatementFormatter.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QFileInfo>
#include <QStringBuilder>
#include <QDir>
#include <QUuid>
#include <QLatin1String>
#include <QTextStream>

// Tgs
#include <tgs/System/SystemInfo.h>

namespace hoot
{

int OsmApiDbBulkInserter::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmApiDbBulkInserter)

OsmApiDbBulkInserter::OsmApiDbBulkInserter() :
_outputDelimiter("\t"),
_fileDataPassCtr(0),
_includeDebugTags(ConfigOptions().getWriterIncludeDebugTags())
{
  _reset();
  _sectionNames = _createSectionNameList();
  setConfiguration(conf());
  _sqlFormatter.reset(new OsmApiDbSqlStatementFormatter(_outputDelimiter));
}

OsmApiDbBulkInserter::~OsmApiDbBulkInserter()
{
  close();
}

bool OsmApiDbBulkInserter::isSupported(const QString& urlStr)
{
  LOG_VART(urlStr);
  QUrl url(urlStr);
  //if we ever want any other writers that the convert command invokes to output sql, then
  //this will have to be made more specific
  return urlStr.toLower().endsWith(".sql") || _database.isSupported(url);
}

void OsmApiDbBulkInserter::open(const QString& url)
{
  _outputUrl = url;

  // Make sure we're not already open and the URL is valid
  if (!isSupported(url))
  {
    throw HootException(QString("Could not open URL ") + url);
  }

  _verifyStartingIds();
  _verifyOutputCopySettings();
  //early test to make sure we can write to file output locations, so we don't get a nasty
  //surprise after taking a long amount of time to write a huge output file
  if (!_destinationIsDatabase() || !_outputFilesCopyLocation.isEmpty())
  {
    _verifyFileOutputs();
  }
  if (_destinationIsDatabase() && _database.getDB().isOpen())
  {
    throw HootException(
      QString("Database already open.  Close the existing database connection before opening ") +
      QString("a new one.  URL: ") + url);
  }
  if (_destinationIsDatabase())
  {
    _database.open(_outputUrl);
    _verifyChangesetUserId();
  }
  _verifyDependencies();
}

void OsmApiDbBulkInserter::_verifyChangesetUserId()
{
  LOG_VARD(_changesetData.changesetUserId);
  const QString errorMsg =
    "Invalid changeset user ID: " + QString::number(_changesetData.changesetUserId);
  if (_changesetData.changesetUserId == -1 || !_database.userExists(_changesetData.changesetUserId))
  {
    throw HootException(errorMsg);
  }
}

void OsmApiDbBulkInserter::_verifyFileOutputs()
{
  QString finalOutput = _outputUrl;
  if (_destinationIsDatabase() && !_outputFilesCopyLocation.isEmpty())
  {
    finalOutput = _outputFilesCopyLocation;
  }
  QFile outputFile(finalOutput);
  if (outputFile.exists())
  {
    outputFile.remove();
  }
  if (!outputFile.open(QIODevice::WriteOnly))
  {
    throw HootException("Could not open file for output: " + finalOutput);
  }
  outputFile.close();
  outputFile.remove();
}

void OsmApiDbBulkInserter::_verifyStartingIds()
{
  if (_idMappings.startingNodeId < 1 || _idMappings.startingWayId < 1 ||
      _idMappings.startingRelationId < 1)
  {
    throw HootException(
      "Invalid element starting ID specified.  IDs must be greater than or equal to 1.");
  }
  else if (!_validateData &&
           (_idMappings.startingNodeId > 1 || _idMappings.startingWayId > 1 ||
            _idMappings.startingRelationId > 1))
  {
    throw HootException("Cannot specify element starting IDs when data validation is turned off.");
  }
  //this one may not be necessary, but haven't had time to think it through yet, so being safe for
  //now
  else if (_reserveRecordIdsBeforeWritingData && !_validateData)
  {
    throw HootException("Cannot reserve record IDs when data validation is turned off.");
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

void OsmApiDbBulkInserter::_verifyDependencies()
{
  if (system(QString("psql --version > /dev/null").toStdString().c_str()) != 0)
  {
    throw HootException("Unable to access the psql application.  Is Postgres installed?");
  }
}

void OsmApiDbBulkInserter::_verifyOutputCopySettings()
{
  if (_destinationIsDatabase() && !_outputFilesCopyLocation.isEmpty())
  {
    QFileInfo outputCopyLocationInfo(_outputFilesCopyLocation);
    if (!outputCopyLocationInfo.completeSuffix().toLower().endsWith("sql"))
    {
      throw HootException(
        QString("Output file copy location should be set to a SQL file (.sql) when using the ") +
        QString("'psql' writer application.  Location specified: ") + _outputFilesCopyLocation);
    }
  }
}

void OsmApiDbBulkInserter::close()
{
  LOG_TRACE("Closing writer...");

  _closeOutputFiles();
  if (_destinationIsDatabase())
  {
    _database.close();
  }

  _reset();
  _sectionNames = _createSectionNameList();
  setConfiguration(conf());
}

void OsmApiDbBulkInserter::_closeOutputFiles()
{
  for (QStringList::const_iterator sectionNamesItr = _sectionNames.begin();
       sectionNamesItr != _sectionNames.end(); ++sectionNamesItr)
  {
    if (_outputSections.find(*sectionNamesItr) == _outputSections.end())
    {
      LOG_TRACE("No data for table " + *sectionNamesItr);
      continue;
    }

    if (_outputSections[*sectionNamesItr])
    {
      _outputSections[*sectionNamesItr]->close();
    }
  }

  if (_sqlOutputCombinedFile)
  {
    _sqlOutputCombinedFile->close();
  }
}

void OsmApiDbBulkInserter::_logStats(const bool debug)
{
  QStringList messages;
  messages.append(
    QString("\tTotal features: ") + StringUtils::formatLargeNumber(_getTotalFeaturesWritten()));
  if (debug)
  {
    messages.append(QString("\tNodes: ") + StringUtils::formatLargeNumber(_writeStats.nodesWritten));
    messages.append(
      QString("\tNode tags: ") + StringUtils::formatLargeNumber(_writeStats.nodeTagsWritten));
    messages.append(
      QString("\tMaximum node ID: ") + StringUtils::formatLargeNumber(_idMappings.currentNodeId));
    messages.append(QString("\tWays: ") + StringUtils::formatLargeNumber(_writeStats.waysWritten));
    messages.append(
      QString("\tWay nodes: ") + StringUtils::formatLargeNumber(_writeStats.wayNodesWritten));
    messages.append(
      QString("\tWay tags: ") + StringUtils::formatLargeNumber(_writeStats.wayTagsWritten));
    messages.append(
      QString("\tMaximum way ID: ") + StringUtils::formatLargeNumber(_idMappings.currentWayId));
    messages.append(
      QString("\tRelations: ") + StringUtils::formatLargeNumber(_writeStats.relationsWritten));
    messages.append(
      QString("\tRelation members: ") +
      StringUtils::formatLargeNumber(_writeStats.relationMembersWritten));
    messages.append(
      QString("\tRelation tags: ") + StringUtils::formatLargeNumber(_writeStats.relationTagsWritten));
    messages.append(
      QString("\tMaximum relation ID: ") +
      StringUtils::formatLargeNumber(_idMappings.currentRelationId));
    messages.append(
      QString("\tUnresolved relation members: ") +
      StringUtils::formatLargeNumber(_writeStats.relationMembersUnresolved));
    messages.append(
      QString("\tChangesets: ") + StringUtils::formatLargeNumber(_changesetData.changesetsWritten));
    messages.append(
      QString("\tChangeset change size (each): ") +
      StringUtils::formatLargeNumber(_maxChangesetSize));
    messages.append(
      QString("\tExecutable SQL records: ") +
       StringUtils::formatLargeNumber(_getTotalRecordsWritten()));
  }

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

unsigned int OsmApiDbBulkInserter::_numberOfFileDataPasses() const
{
  //using psql always requires a minimum of two passes due to having to combine all the temp
  //sql files into one
  unsigned int numPasses = 2;
  if (_destinationIsDatabase())
  {
    //writing to the database is another pass over the data
    numPasses++;
  }
  return numPasses;
}

void OsmApiDbBulkInserter::_flush()
{
  for (QStringList::const_iterator it = _sectionNames.begin(); it != _sectionNames.end(); ++it)
  {
    if (_outputSections.find(*it) == _outputSections.end())
    {
      LOG_TRACE("No data for table " + *it);
      continue;
    }

    LOG_TRACE("Flushing section " << *it << " to file " << _outputSections[*it]->fileName());
    _outputSections[*it]->write(QString("\\.\n\n").toUtf8());
    // Flush any residual content from file
    if (!_outputSections[*it]->flush())
    {
      throw HootException("Could not flush file for table " + *it);
    }
  }
}

void OsmApiDbBulkInserter::_clearIdCollections()
{
  LOG_DEBUG("Clearing out ID mappings...");
  if (_idMappings.nodeIdMap)
  {
    _idMappings.nodeIdMap->clear();
  }
  _idMappings.nodeIdMap.reset();
  if (_idMappings.wayIdMap)
  {
    _idMappings.wayIdMap->clear();
  }
  _idMappings.wayIdMap.reset();
  if (_idMappings.relationIdMap)
  {
    _idMappings.relationIdMap->clear();
  }
  _idMappings.relationIdMap.reset();
  if (_unresolvedRefs.unresolvedRelationRefs)
  {
    _unresolvedRefs.unresolvedRelationRefs->clear();
  }
  _unresolvedRefs.unresolvedRelationRefs.reset();
}

void OsmApiDbBulkInserter::finalizePartial()
{
  LOG_INFO(
    StringUtils::formatLargeNumber(_getTotalFeaturesWritten()) <<
    " input records parsed (data pass #" << _fileDataPassCtr << " of " <<
    _numberOfFileDataPasses() << ").  Time elapsed: " <<
    StringUtils::millisecondsToDhms(_timer->elapsed()));

  //go ahead and clear out some of the data structures we don't need anymore
  _clearIdCollections();

  if (_writeStats.nodesWritten == 0)
  {
    LOG_DEBUG("No input data was written to files.");
    return;
  }

  // Do we have an unfinished changeset that needs flushing?
  if (_changesetData.changesInChangeset > 0)
  {
    _writeChangeset();
  }
  //If there was only one changeset written total, this won't have yet been incremented, so do it
  //now.
  if (_changesetData.changesetsWritten == 0)
  {
    _changesetData.changesetsWritten++;
  }
  _flush();
  _closeOutputFiles();

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

  //combine all the element/changeset files that were written during partial streaming into
  //one file and update the ids in the SQL file according to the id sequences previously reserved
  //out
  _writeCombinedSqlFile();

  if (_destinationIsDatabase())
  {
    LOG_INFO("File write stats:");
    _logStats(false);

    _writeDataToDb();
    if (_outputFilesCopyLocation.isEmpty())
    {
      _sqlOutputCombinedFile->remove();
    }
  }
  else
  {
    LOG_DEBUG("Skipping SQL execution against database due to configuration...");
    LOG_INFO("File write stats:");
    _logStats(false);
  }
}

bool OsmApiDbBulkInserter::_destinationIsDatabase() const
{
  return _outputUrl.toLower().startsWith(MetadataTags::OsmApiDbScheme() + "://");
}

void OsmApiDbBulkInserter::_writeDataToDbPsql()
{
  _timer->restart();
  _fileDataPassCtr++;
  //I believe a COPY header is created whether there are any records to copy for the table or not,
  //which is why the number of copy statements to be executed is hardcoded here.  Might be cleaner
  //to not write the header if there are no records to copy for the table...
  LOG_INFO(
    "Executing SQL for " << StringUtils::formatLargeNumber(_getTotalRecordsWritten()) <<
    " records (data pass #" << _fileDataPassCtr << " of " << _numberOfFileDataPasses() << ")...");

  //exec element sql against the db; Using psql here b/c it is doing buffered reads against the
  //sql file, so no need doing the extra work to handle buffering the sql read manually and
  //applying it to a QSqlQuery.
  ApiDb::execSqlFile(_outputUrl, _sqlOutputCombinedFile->fileName());

  LOG_INFO(
    "SQL execution complete.  Time elapsed: " << StringUtils::millisecondsToDhms(_timer->elapsed()));
}

void OsmApiDbBulkInserter::_writeDataToDb()
{
  if (_disableDatabaseConstraintsDuringWrite)
  {
    _database.disableConstraints();
  }
  if (_disableDatabaseIndexesDuringWrite)
  {
    _database.dropIndexes();
  }

  _writeDataToDbPsql();

  if (_disableDatabaseConstraintsDuringWrite)
  {
    _database.enableConstraints();
  }
  if (_disableDatabaseIndexesDuringWrite)
  {
    _database.createIndexes();
  }
}

QString OsmApiDbBulkInserter::_getCombinedSqlFileName() const
{
  QString dest;
  if (!_destinationIsDatabase())
  {
    dest = _outputUrl;
  }
  else if (!_outputFilesCopyLocation.isEmpty())
  {
    dest = _outputFilesCopyLocation;
  }
  else
  {
    dest = _tempDir + "/ApiDbBulkInserter-" + QUuid::createUuid().toString() + ".sql";
  }
  return dest;
}

void OsmApiDbBulkInserter::_writeCombinedSqlFile()
{
  _timer->restart();
  _fileDataPassCtr++;

  const QString dest = _getCombinedSqlFileName();
  LOG_VART(dest);
  QFile outputFile(dest);
  if (outputFile.exists())
  {
    outputFile.remove();
  }
  _sqlOutputCombinedFile.reset(new QFile(dest));
  if (!_sqlOutputCombinedFile->open(QIODevice::WriteOnly))
  {
    throw HootException("Could not open file for SQL output: " + dest);
  }

  LOG_INFO(
    "Writing combined SQL output file to " << _sqlOutputCombinedFile->fileName() <<
    ".  (data pass #" << _fileDataPassCtr << " of " << _numberOfFileDataPasses() << ")...");
  LOG_VART(_sectionNames.size());
  LOG_VART(_outputSections.size());

  _sqlOutputCombinedFile->write(QString("BEGIN TRANSACTION;\n\n").toUtf8());

  if (!_reserveRecordIdsBeforeWritingData && _writeIdSequenceUpdates)
  {
    //We're not reserving the ID ranges in the database, so we'll write the appropriate setval
    //statements to the sql output here for applying at a later time.  we want
    //setval to reflect the last id in the sequence
    QString reserveElementIdsSql;
    // may be able to collapse this logic; see notes in _establishIdMapping
    if (_validateData)
    {
      //with data validation on, we increment for each element read and all our counters are
      //incremented one past the element/changeset count and we need to decrement them by one
      _writeSequenceUpdates(_changesetData.currentChangesetId - 1,
                            _idMappings.currentNodeId - 1,
                            _idMappings.currentWayId - 1,
                            _idMappings.currentRelationId - 1,
                            reserveElementIdsSql);
    }
    else
    {
      //with data validation off, changesets are incremented one past, but the element current
      //id's were always assigned the highest parsed value
      _writeSequenceUpdates(_changesetData.currentChangesetId - 1,
                            _idMappings.currentNodeId,
                            _idMappings.currentWayId,
                            _idMappings.currentRelationId,
                            reserveElementIdsSql);
    }
    LOG_VART(reserveElementIdsSql);
    _sqlOutputCombinedFile->write(reserveElementIdsSql.toUtf8());
  }

  long recordCtr = 0;
  for (QStringList::const_iterator it = _sectionNames.begin(); it != _sectionNames.end(); ++it)
  {
    LOG_DEBUG("Parsing data for temp file " << *it);
    if (_outputSections.find(*it) == _outputSections.end())
    {
      LOG_DEBUG("No data for table " + *it);
      continue;
    }

    // Append contents of section subfiles to output file

    //This file was originally opened as write only and has already been closed by this point,
    //so create a new readonly file for reading it back in.
    QFile tempInputFile(_outputSections[*it]->fileName());
    try
    {
      LOG_DEBUG("Opening file: " << _outputSections[*it]->fileName());
      if (tempInputFile.open(QIODevice::ReadOnly))
      {
        LOG_DEBUG("Parsing file for table: " << *it << "...");

        const bool updateIdOffsets = _destinationIsDatabase() && _reserveRecordIdsBeforeWritingData;
        do
        {
          //these element lines won't contain tags
          QString line;
          line.reserve(75);
          line.append(QString::fromUtf8(tempInputFile.readLine().constData()));
          LOG_VART(line.left(25));
          LOG_VART(line.length());

          if (!line.trimmed().isEmpty() &&
              line.trimmed() != QLatin1String("\\.") && !line.startsWith("COPY"))
          {
            if (updateIdOffsets)
            {
              _updateRecordLineWithIdOffset(*it, line);
            }
            recordCtr++;
          }

          _sqlOutputCombinedFile->write(line.toUtf8());

          if (recordCtr > 0 && (recordCtr % (_statusUpdateInterval * 100) == 0))
          {
            //changesets is throwing off the progress totals here...not sure why...don't
            //care that much right now, since the changeset count is far outnumbered by the
            //size of the rest of the data
            PROGRESS_INFO(
              "Parsed " << StringUtils::formatLargeNumber(recordCtr) << "/" <<
              StringUtils::formatLargeNumber(
                _getTotalRecordsWritten() - _changesetData.changesetsWritten) <<
              " SQL file lines.");
          }
        }
        while (!tempInputFile.atEnd());

        tempInputFile.close();
        LOG_DEBUG("Closing file for " << *it << "...");
        _outputSections[*it]->close();
        //shouldn't need to do this since its a temp file
        _outputSections[*it]->remove();
        _outputSections[*it].reset();
      }
      else
      {
        throw HootException("Unable to open input file: " + tempInputFile.fileName());
      }
    }
    catch (...)
    {
      tempInputFile.close();
      _closeOutputFiles();
      throw;
    }

    LOG_DEBUG("Wrote contents of section " << *it);
  }
  LOG_DEBUG("Finished parsing temp files...");
  _sqlOutputCombinedFile->write("COMMIT;");
  _sqlOutputCombinedFile->flush();
  _sqlOutputCombinedFile->close();

  LOG_INFO(
    "SQL file write complete.  (data pass #" << _fileDataPassCtr << " of " <<
    _numberOfFileDataPasses() << ").  Time elapsed: " <<
    StringUtils::millisecondsToDhms(_timer->elapsed()));
  LOG_DEBUG("Parsed " << StringUtils::formatLargeNumber(recordCtr) << " total SQL file lines.");
  QFileInfo outputInfo(_sqlOutputCombinedFile->fileName());
  LOG_VART(Tgs::SystemInfo::humanReadable(outputInfo.size()));
}

void OsmApiDbBulkInserter::_updateRecordLineWithIdOffset(const QString& tableName,
                                                         QString& recordLine)
{
  LOG_TRACE("Updating ID offset for line: " << recordLine.left(25));
  LOG_VART(tableName);

  QStringList lineParts;
  lineParts.reserve(15);
  lineParts.append(recordLine.split(_outputDelimiter));

  //update any element/changeset ID reference by adding the current ID offset to it

  bool lineUpdated = true;
  if (tableName == ApiDb::getChangesetsTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toULong() + _changesetData.currentChangesetId);
  }
  else if (tableName == ApiDb::getCurrentNodesTableName() ||
           tableName == ApiDb::getNodesTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toULong() + _idMappings.currentNodeId);
    lineParts[3] = QString::number(lineParts[3].toULong() + _changesetData.currentChangesetId);
  }
  else if (tableName == ApiDb::getCurrentWaysTableName() ||
           tableName == ApiDb::getWaysTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toULong() + _idMappings.currentWayId);
    lineParts[1] = QString::number(lineParts[1].toULong() + _changesetData.currentChangesetId);
  }
  else if (tableName == ApiDb::getCurrentWayNodesTableName() ||
           tableName == ApiDb::getWayNodesTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toULong() + _idMappings.currentWayId);
    lineParts[1] = QString::number(lineParts[1].toULong() + _idMappings.currentNodeId);
  }
  else if (tableName == ApiDb::getCurrentRelationsTableName() ||
           tableName == ApiDb::getRelationsTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toULong() + _idMappings.currentRelationId);
    lineParts[1] = QString::number(lineParts[1].toULong() + _changesetData.currentChangesetId);
  }
  else if (tableName == ApiDb::getCurrentRelationMembersTableName() ||
           tableName == ApiDb::getRelationMembersTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toULong() + _idMappings.currentRelationId);
    const unsigned long memberId = lineParts[2].toULong();
    if (lineParts[1].toLower() == QLatin1String("node"))
    {
      lineParts[2] = QString::number(memberId + _idMappings.currentNodeId);
    }
    else if (lineParts[1].toLower() == QLatin1String("way"))
    {
      lineParts[2] = QString::number(memberId + _idMappings.currentWayId);
    }
  }
  else if (tableName == ApiDb::getCurrentNodeTagsTableName() ||
           tableName == ApiDb::getNodeTagsTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toULong() + _idMappings.currentNodeId);
  }
  else if (tableName == ApiDb::getCurrentWayTagsTableName() ||
           tableName == ApiDb::getWayTagsTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toULong() + _idMappings.currentWayId);
  }
  else if (tableName == ApiDb::getCurrentRelationTagsTableName() ||
           tableName == ApiDb::getRelationTagsTableName())
  {
    lineParts[0] = QString::number(lineParts[0].toULong() + _idMappings.currentRelationId);
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

void OsmApiDbBulkInserter::_reserveIdsInDb()
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
  _writeSequenceUpdates(_changesetData.currentChangesetId + _changesetData.changesetsWritten,
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

unsigned long OsmApiDbBulkInserter::_getTotalFeaturesWritten() const
{
  return _writeStats.nodesWritten + _writeStats.waysWritten + _writeStats.relationsWritten;
}

unsigned long OsmApiDbBulkInserter::_getTotalRecordsWritten() const
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

void OsmApiDbBulkInserter::_incrementAndGetLatestIdsFromDb()
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

void OsmApiDbBulkInserter::writePartial(const ConstNodePtr& node)
{
  if (_writeStats.nodesWritten == 0)
  {
    _timer.reset(new QElapsedTimer());
    _timer->start();
    _fileDataPassCtr++;
    LOG_INFO(
      "Streaming elements from input to file outputs.  (data pass #" <<
      _fileDataPassCtr << " of " << _numberOfFileDataPasses() << ")...");
    _createNodeOutputFiles();
    _idMappings.nodeIdMap.reset(new Tgs::BigMap<long, unsigned long>(_stxxlMapMinSize));
  }

  LOG_VART(node);

  //See #1451.  This changeset bounds calculation actually won't work when ways or relations
  //are written in separate changesets than the nodes they reference.  Since we're streaming the
  //elements, there's no way to get back to the bounds information.  This bug has always been here,
  //but just recently noticed.
  //_changesetData.changesetBounds.expandToInclude(node->getX(), node->getY());
  //LOG_VART(_changesetData.changesetBounds.toString());

  // Do we already know about this node?
  if (_validateData && _idMappings.nodeIdMap->contains(node->getId()))
  {
    throw NotImplementedException("Writer class does not support update operations.");
  }
  LOG_VART(node->getElementId());
  const unsigned long nodeDbId = _establishIdMapping(node->getElementId());
  LOG_VART(ElementId(ElementType::Node, nodeDbId));

  if (_includeDebugTags)
  {
    Tags tags = node->getTags();
    //keep the hoot:id tag in sync with what could be a newly assigned id
    tags.set(MetadataTags::HootId(), QString::number(nodeDbId));
  }

  _writeNode(node, nodeDbId);
  _writeTags(node->getTags(), ElementType::Node, nodeDbId,
    _outputSections[ApiDb::getCurrentNodeTagsTableName()],
    _outputSections[ApiDb::getNodeTagsTableName()]);
  _writeStats.nodesWritten++;
  _writeStats.nodeTagsWritten += node->getTags().size();
  _incrementChangesInChangeset();
  if (_validateData)
  {
    _checkUnresolvedReferences(node, nodeDbId);
  }

  if (_writeStats.nodesWritten % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO(
      "Parsed " << StringUtils::formatLargeNumber(_writeStats.nodesWritten) <<
      " nodes from input.");
  }

  LOG_VART(node->getVersion());
}

void OsmApiDbBulkInserter::writePartial(const ConstWayPtr& way)
{
  LOG_VART(way);

  if (_writeStats.waysWritten == 0)
  {
    _createWayOutputFiles();
    _idMappings.wayIdMap.reset(new Tgs::BigMap<long, unsigned long>(_stxxlMapMinSize));
  }

  // Do we already know about this way?
  if (_validateData && _idMappings.wayIdMap->contains(way->getId()))
  {
    throw NotImplementedException("Writer class does not support update operations");
  }
  LOG_VART(way->getElementId());
  const unsigned long wayDbId = _establishIdMapping(way->getElementId());
  LOG_VART(ElementId(ElementType::Way, wayDbId));

  if (_includeDebugTags)
  {
    Tags tags = way->getTags();
    //keep the hoot:id tag in sync with what could be a newly assigned id
    tags.set(MetadataTags::HootId(), QString::number(wayDbId));
  }

  _writeWay(wayDbId);
  _writeWayNodes(wayDbId, way->getNodeIds());
  _writeTags(way->getTags(), ElementType::Way, wayDbId,
    _outputSections[ApiDb::getCurrentWayTagsTableName()],
    _outputSections[ApiDb::getWayTagsTableName()]);
  _writeStats.waysWritten++;
  _writeStats.wayTagsWritten += way->getTags().size();
  _writeStats.wayNodesWritten += way->getNodeIds().size();
  _incrementChangesInChangeset();
  if (_validateData)
  {
    _checkUnresolvedReferences(way, wayDbId);
  }

  if (_writeStats.waysWritten % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO(
      "Parsed " << StringUtils::formatLargeNumber(_writeStats.waysWritten) <<
      " ways from input.");
  }
}

void OsmApiDbBulkInserter::writePartial(const ConstRelationPtr& relation)
{
  LOG_VART(relation);

  if (_writeStats.relationsWritten == 0)
  {
    _createRelationOutputFiles();
    _idMappings.relationIdMap.reset(new Tgs::BigMap<long, unsigned long>(_stxxlMapMinSize));
  }

  // Do we already know about this node?
  if (_validateData && _idMappings.relationIdMap->contains(relation->getId()))
  {
    throw NotImplementedException("Writer class does not support update operations");
  }
  LOG_VART(relation->getElementId());
  const unsigned long relationDbId = _establishIdMapping(relation->getElementId());
  LOG_VART(ElementId(ElementType::Relation, relationDbId));

  Tags tags = relation->getTags();
  //  keep the hoot:id tag in sync with what could be a newly assigned id
  if (_includeDebugTags)
    tags.set(MetadataTags::HootId(), QString::number(relationDbId));
  //  Add the "type" of the relation to the tags
  if (relation->getType() != "")
    tags["type"] = relation->getType();

  _writeRelation(relationDbId);
  _writeRelationMembers(relation, relationDbId);
  _writeTags(tags, ElementType::Relation, relationDbId,
    _outputSections[ApiDb::getCurrentRelationTagsTableName()],
    _outputSections[ApiDb::getRelationTagsTableName()]);
  _writeStats.relationsWritten++;
  _writeStats.relationTagsWritten += relation->getTags().size();
  _writeStats.relationMembersWritten += relation->getMembers().size();
  _incrementChangesInChangeset();
  if (_validateData)
  {
    _checkUnresolvedReferences(relation, relationDbId);
  }

  if (_writeStats.relationsWritten % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO(
      "Parsed " << StringUtils::formatLargeNumber(_writeStats.relationsWritten) <<
      " relations from input.");
  }
}

void OsmApiDbBulkInserter::setConfiguration(const Settings& conf)
{
  const ConfigOptions confOptions(conf);

  setOutputFilesCopyLocation(confOptions.getApidbBulkInserterOutputFilesCopyLocation().trimmed());
  setChangesetUserId(confOptions.getChangesetUserId());
  setFileOutputElementBufferSize(confOptions.getMaxElementsPerPartialMap());
  setStatusUpdateInterval(confOptions.getTaskStatusUpdateInterval());
  setMaxChangesetSize(confOptions.getChangesetMaxSize());
  setReserveRecordIdsBeforeWritingData(
    confOptions.getOsmapidbBulkInserterReserveRecordIdsBeforeWritingData());
  setStartingNodeId(confOptions.getApidbBulkInserterStartingNodeId());
  setStartingWayId(confOptions.getApidbBulkInserterStartingWayId());
  setStartingRelationId(confOptions.getApidbBulkInserterStartingRelationId());
  //apidb.bulk.inserter.run.validation.in.memory
  if (confOptions.getApidbBulkInserterRunValidationInMemory())
  {
    setStxxlMapMinSize(LONG_MAX);
    setValidateData(true);
  }
  else
  {
    setStxxlMapMinSize(confOptions.getApidbBulkInserterStxxlMapMinSize());
    setValidateData(confOptions.getApidbBulkInserterValidateData());
  }
  setDisableDatabaseConstraintsDuringWrite(
    confOptions.getOsmapidbBulkInserterDisableDatabaseConstraintsDuringWrite());
  setTempDir(confOptions.getApidbBulkInserterTempFileDir());
  setDisableDatabaseIndexesDuringWrite(
    confOptions.getOsmapidbBulkInserterDisableDatabaseIndexesDuringWrite());
  setWriteIdSequenceUpdates(confOptions.getOsmapidbBulkInserterWriteSqlFileIdSequenceUpdates());
}

QStringList OsmApiDbBulkInserter::_createSectionNameList()
{
  QStringList sections;
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

void OsmApiDbBulkInserter::_createNodeOutputFiles()
{
  const QStringList nodeSqlHeaders = OsmApiDbSqlStatementFormatter::getNodeSqlHeaderStrings();
  const QStringList nodeTagSqlHeaders = OsmApiDbSqlStatementFormatter::getNodeTagSqlHeaderStrings();

  _createOutputFile(ApiDb::getCurrentNodesTableName(), nodeSqlHeaders[0]);
  _createOutputFile(ApiDb::getCurrentNodeTagsTableName(), nodeTagSqlHeaders[0]);

  _createOutputFile(ApiDb::getNodesTableName(), nodeSqlHeaders[1]);
  _createOutputFile(ApiDb::getNodeTagsTableName(), nodeTagSqlHeaders[1]);
}

void OsmApiDbBulkInserter::_reset()
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

  _unresolvedRefs.unresolvedRelationRefs.reset();

  _outputSections.clear();
  _sectionNames.clear();
}

unsigned long OsmApiDbBulkInserter::_establishIdMapping(const ElementId& sourceId)
{  
  unsigned long dbIdentifier;

  switch (sourceId.getType().getEnum())
  {
    case ElementType::Node:
      if (_validateData)
      {
        dbIdentifier = _idMappings.currentNodeId;
        _idMappings.nodeIdMap->insert(sourceId.getId(), dbIdentifier);
        _idMappings.currentNodeId++;
      }
      else
      {
        //be sure to use the std version of abs here, b/c the global one doesn't handle 8 byte
        //long as expected
        dbIdentifier = std::abs(sourceId.getId());
        if (dbIdentifier > _idMappings.currentNodeId)
        {
          _idMappings.currentNodeId = dbIdentifier;
        }
      }
      break;

    case ElementType::Way:
      if (_validateData)
      {
        dbIdentifier = _idMappings.currentWayId;
        _idMappings.wayIdMap->insert(sourceId.getId(), dbIdentifier);
        _idMappings.currentWayId++;
      }
      else
      {
        dbIdentifier = std::abs(sourceId.getId());
        if (dbIdentifier > _idMappings.currentWayId)
        {
          _idMappings.currentWayId = dbIdentifier;
        }
      }
    break;

    case ElementType::Relation:
      if (_validateData)
      {
        dbIdentifier = _idMappings.currentRelationId;
        _idMappings.relationIdMap->insert(sourceId.getId(), dbIdentifier);
        _idMappings.currentRelationId++;
      }
      else
      {
        dbIdentifier = std::abs(sourceId.getId());
        if (dbIdentifier > _idMappings.currentRelationId)
        {
          _idMappings.currentRelationId = dbIdentifier;
        }
      }
    break;

    default:
      throw UnsupportedException("Unsupported element type.");
  }

  return dbIdentifier;
}

void OsmApiDbBulkInserter::_writeNode(const ConstNodePtr& node, const unsigned long nodeDbId)
{
  LOG_TRACE("Writing node to stream...");
  const QStringList nodeSqlStrs =
    _sqlFormatter->nodeToSqlStrings(
      node, nodeDbId, _changesetData.currentChangesetId, _validateData);
  _outputSections[ApiDb::getCurrentNodesTableName()]->write(nodeSqlStrs[0].toUtf8());
  _outputSections[ApiDb::getNodesTableName()]->write(nodeSqlStrs[1].toUtf8());
}

void OsmApiDbBulkInserter::_writeTags(const Tags& tags,
                                      const ElementType::Type& elementType,
                                      const unsigned long dbId,
                                      const std::shared_ptr<QFile>& currentTableFile,
                                      const std::shared_ptr<QFile>& historicalTableFile)
{
  LOG_TRACE("Writing tags to stream...");

  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    QString key = it.key();
    QString value = it.value().trimmed();
    if (!value.isEmpty())
    {
      const QStringList tagSqlStrs = _sqlFormatter->tagToSqlStrings(dbId, elementType, key, value);
      currentTableFile->write(tagSqlStrs[0].toUtf8());
      historicalTableFile->write(tagSqlStrs[1].toUtf8());
    }
  }
}

void OsmApiDbBulkInserter::_createWayOutputFiles()
{
  const QStringList waySqlHeaders = OsmApiDbSqlStatementFormatter::getWaySqlHeaderStrings();
  const QStringList wayTagSqlHeaders = OsmApiDbSqlStatementFormatter::getWayTagSqlHeaderStrings();
  const QStringList wayNodeSqlHeaders = OsmApiDbSqlStatementFormatter::getWayNodeSqlHeaderStrings();

  _createOutputFile(ApiDb::getCurrentWaysTableName(), waySqlHeaders[0]);
  _createOutputFile(ApiDb::getCurrentWayTagsTableName(), wayTagSqlHeaders[0]);
  _createOutputFile(ApiDb::getCurrentWayNodesTableName(), wayNodeSqlHeaders[0]);

  _createOutputFile(ApiDb::getWaysTableName(), waySqlHeaders[1]);
  _createOutputFile(ApiDb::getWayTagsTableName(), wayTagSqlHeaders[1]);
  _createOutputFile(ApiDb::getWayNodesTableName(), wayNodeSqlHeaders[1]);
}

void OsmApiDbBulkInserter::_writeWay(const unsigned long wayDbId)
{
  LOG_TRACE("Writing way to stream...");

  const QStringList waySqlStrs =
    _sqlFormatter->wayToSqlStrings(wayDbId, _changesetData.currentChangesetId);
  _outputSections[ApiDb::getCurrentWaysTableName()]->write(waySqlStrs[0].toUtf8());
  _outputSections[ApiDb::getWaysTableName()]->write(waySqlStrs[1].toUtf8());
}

void OsmApiDbBulkInserter::_writeWayNodes(const unsigned long dbWayId,
                                          const std::vector<long>& wayNodeIds)
{
  LOG_TRACE("Writing way nodes to stream...");

  unsigned int wayNodeIndex = 1;
  for (std::vector<long>::const_iterator it = wayNodeIds.begin(); it != wayNodeIds.end(); ++it)
  {
    unsigned long wayNodeIdVal;
    if (!_validateData)
    {
      wayNodeIdVal = std::abs(*it);
    }
    else if (_idMappings.nodeIdMap->contains(*it))
    {
      wayNodeIdVal = _idMappings.nodeIdMap->at(*it);
    }
    else
    {
      throw UnsupportedException(
        "Unresolved way nodes are not supported.  " +
        QString("Way %1 has reference to unknown node ID %2").arg(dbWayId, *it));
    }

    const QStringList wayNodeSqlStrs =
      _sqlFormatter->wayNodeToSqlStrings(dbWayId, wayNodeIdVal, wayNodeIndex);
    _outputSections[ApiDb::getCurrentWayNodesTableName()]->write(wayNodeSqlStrs[0].toUtf8());
    _outputSections[ApiDb::getWayNodesTableName()]->write(wayNodeSqlStrs[1].toUtf8());

    ++wayNodeIndex;
  }
}

void OsmApiDbBulkInserter::_createRelationOutputFiles()
{
  const QStringList relationSqlHeaders =
    OsmApiDbSqlStatementFormatter::getRelationSqlHeaderStrings();
  const QStringList relationTagSqlHeaders =
    OsmApiDbSqlStatementFormatter::getRelationTagSqlHeaderStrings();
  const QStringList relationMemberSqlHeaders =
    OsmApiDbSqlStatementFormatter::getRelationMemberSqlHeaderStrings();

  _createOutputFile(ApiDb::getCurrentRelationsTableName(), relationSqlHeaders[0]);
  _createOutputFile(ApiDb::getCurrentRelationTagsTableName(), relationTagSqlHeaders[0]);
  _createOutputFile(ApiDb::getCurrentRelationMembersTableName(), relationMemberSqlHeaders[0]);

  _createOutputFile(ApiDb::getRelationsTableName(), relationSqlHeaders[1]);
  _createOutputFile(ApiDb::getRelationTagsTableName(), relationTagSqlHeaders[1]);
  _createOutputFile(ApiDb::getRelationMembersTableName(),relationMemberSqlHeaders[1]);
}

void OsmApiDbBulkInserter::_writeRelation(const unsigned long relationDbId)
{
  LOG_TRACE("Writing relation to stream...");

  const QStringList relationSqlStrs =
    _sqlFormatter->relationToSqlStrings(relationDbId, _changesetData.currentChangesetId);
  _outputSections[ApiDb::getCurrentRelationsTableName()]->write(relationSqlStrs[0].toUtf8());
  _outputSections[ApiDb::getRelationsTableName()]->write(relationSqlStrs[1].toUtf8());
}

void OsmApiDbBulkInserter::_writeRelationMembers(const ConstRelationPtr& relation,
                                                 const unsigned long dbRelationId)
{
  LOG_TRACE("Writing relation members to stream...");

  unsigned int memberSequenceIndex = 1;
  const long relationId = relation->getId();
  const std::vector<RelationData::Entry> relationMembers = relation->getMembers();
  std::shared_ptr<Tgs::BigMap<long, unsigned long>> knownElementMap;

  for (std::vector<RelationData::Entry>::const_iterator it = relationMembers.begin();
       it != relationMembers.end(); ++it)
  {
    const ElementId memberElementId = it->getElementId();

    if (_validateData)
    {
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
    }

    if (!_validateData)
    {
      unsigned long memberIdVal = std::abs(memberElementId.getId());
      _writeRelationMember(dbRelationId, *it, memberIdVal, memberSequenceIndex);
    }
    else if (knownElementMap && knownElementMap->contains(memberElementId.getId()))
    {
      _writeRelationMember(
        dbRelationId, *it, knownElementMap->at(memberElementId.getId()), memberSequenceIndex);
    }
    else
    {
      if (!_unresolvedRefs.unresolvedRelationRefs)
      {
        _unresolvedRefs.unresolvedRelationRefs.reset(
          new std::map<ElementId, UnresolvedRelationReference>());
      }
      const UnresolvedRelationReference relationRef =
        { relationId, dbRelationId, *it, memberSequenceIndex };
      _unresolvedRefs.unresolvedRelationRefs->insert(
        std::pair<ElementId, UnresolvedRelationReference>(memberElementId, relationRef));
    }

    ++memberSequenceIndex;
  }
}

void OsmApiDbBulkInserter::_writeRelationMember(const unsigned long sourceRelationDbId,
                                                const RelationData::Entry& member,
                                                const unsigned long memberDbId,
                                                const unsigned int memberSequenceIndex)
{
  const QStringList relationMemberSqlStrs =
    _sqlFormatter->relationMemberToSqlStrings(
      sourceRelationDbId, memberDbId, member, memberSequenceIndex);
  _outputSections[ApiDb::getCurrentRelationMembersTableName()]->write(
    relationMemberSqlStrs[0].toUtf8());
  _outputSections[ApiDb::getRelationMembersTableName()]->write(relationMemberSqlStrs[1].toUtf8());
  _writeStats.relationMembersWritten++;
}

void OsmApiDbBulkInserter::_createOutputFile(const QString& tableName, const QString& header)
{
  QString msg = "Creating output file " + tableName;
  if (!header.trimmed().isEmpty())
  {
    msg += " and writing table header";
  }
  msg += "...";
  LOG_DEBUG(msg);

  _outputSections[tableName].reset(
    new QTemporaryFile(_tempDir + "/ApiDbBulkInserter-" + tableName + "-temp-XXXXXX.sql"));
  if (!_outputSections[tableName]->open())
  {
    throw HootException(
      "Could not open file at: " + _outputSections[tableName]->fileName() +
      " for contents of table: " + tableName);
  }
  //for debugging only
  //_outputSections[tableName]->setAutoRemove(false);

  if (!header.trimmed().isEmpty())
  {
    _outputSections[tableName]->write(header.toUtf8());
  }
}

void OsmApiDbBulkInserter::_incrementChangesInChangeset()
{
  _changesetData.changesInChangeset++;
  if (_changesetData.changesInChangeset == _maxChangesetSize)
  {
    LOG_VART(_changesetData.changesInChangeset);
    _writeChangeset();
    _changesetData.currentChangesetId++;
    LOG_VART(_changesetData.currentChangesetId);
    _changesetData.changesInChangeset = 0;
    _changesetData.changesetBounds.init();
    _changesetData.changesetsWritten++;
    LOG_VART(_changesetData.changesetsWritten);
  }
}

void OsmApiDbBulkInserter::_checkUnresolvedReferences(const ConstElementPtr& element,
                                                      const unsigned long elementDbId)
{
  // Regardless of type, may be referenced in relation
  if (_unresolvedRefs.unresolvedRelationRefs)
  {
    std::map<ElementId, UnresolvedRelationReference >::iterator relationRef =
      _unresolvedRefs.unresolvedRelationRefs->find(element->getElementId());

    if (relationRef != _unresolvedRefs.unresolvedRelationRefs->end())
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Found unresolved relation member ref!:");
        LOG_WARN(QString( "Relation ID ") % QString::number(relationRef->second.sourceRelationId) %
          QString(" (DB ID=") % QString::number(relationRef->second.sourceDbRelationId) %
           QString(") has ref to ") % relationRef->second.relationMemberData.toString());
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;

      _writeRelationMember(
        relationRef->second.sourceDbRelationId, relationRef->second.relationMemberData,
        elementDbId, relationRef->second.relationMemberSequenceId);

      // Remove entry from unresolved list
      _unresolvedRefs.unresolvedRelationRefs->erase(relationRef);
    }
  }
}

void OsmApiDbBulkInserter::_writeChangeset()
{
  LOG_VART(_changesetData.changesetUserId);
  LOG_VART(_changesetData.currentChangesetId);

  if (!_outputSections[ApiDb::getChangesetsTableName()])
  {
    _createOutputFile(
      ApiDb::getChangesetsTableName(), _sqlFormatter->getChangesetSqlHeaderString());
  }

  _outputSections[ApiDb::getChangesetsTableName()]->write(
    _sqlFormatter->changesetToSqlString(
      _changesetData.currentChangesetId,
      _changesetData.changesetUserId,
      _changesetData.changesInChangeset,
      _changesetData.changesetBounds).toUtf8());
}

void OsmApiDbBulkInserter::_writeSequenceUpdates(long changesetId, const unsigned long nodeId,
                                                 const unsigned long wayId,
                                                 const unsigned long relationId, QString& outputStr)
{
  LOG_DEBUG("Writing sequence updates stream...");

  if (changesetId <= 0) //probably can get rid of this
  {
    changesetId = 1;
  }

  QTextStream sequenceUpdatesStream(&outputStr);
  const QString sequenceUpdateFormat("SELECT pg_catalog.setval('%1', %2);\n");

  //At least one changeset and some nodes should always be written by a write operation; ways
  //and relations are optional.

  assert(changesetId > 0);
  sequenceUpdatesStream <<
    sequenceUpdateFormat.arg(ApiDb::getChangesetsSequenceName(), QString::number(changesetId)).toUtf8();

  assert(nodeId > 0);
  sequenceUpdatesStream <<
    sequenceUpdateFormat.arg(ApiDb::getCurrentNodesSequenceName(), QString::number(nodeId)).toUtf8();

  if (wayId > 0)
  {
    sequenceUpdatesStream <<
      sequenceUpdateFormat.arg(ApiDb::getCurrentWaysSequenceName(), QString::number(wayId)).toUtf8();
  }

  if (relationId > 0)
  {
    sequenceUpdatesStream <<
      sequenceUpdateFormat.arg(
        ApiDb::getCurrentRelationsSequenceName(), QString::number(relationId)).toUtf8();
  }

  sequenceUpdatesStream << QString("\n").toUtf8();
}

}
