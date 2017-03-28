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
#include <QUuid>

#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/io/OsmApiDbSqlStatementFormatter.h>

#include <tgs/System/SystemInfo.h>

namespace hoot
{

using namespace Tgs;

unsigned int OsmApiDbBulkWriter::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmApiDbBulkWriter)

OsmApiDbBulkWriter::OsmApiDbBulkWriter() :
_outputDelimiter("\t"),
_fileDataPassCtr(0)
{
  _reset();
  _sectionNames = _createSectionNameList();
  setConfiguration(conf());
  _sqlFormatter.reset(new OsmApiDbSqlStatementFormatter(_outputDelimiter));
}

OsmApiDbBulkWriter::~OsmApiDbBulkWriter()
{
  close();
}

bool OsmApiDbBulkWriter::isSupported(QString urlStr)
{
  LOG_VARD(urlStr);
  QUrl url(urlStr);
  //if we ever want any other writers that the convert command invokes to output sql, then
  //this will have to be made more specific
  return urlStr.toLower().endsWith(".sql") || _database.isSupported(url);
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
  }
  _verifyDependencies();
}

void OsmApiDbBulkWriter::_verifyFileOutputs()
{
  QString finalOutput = _outputUrl;
  if (_destinationIsDatabase() && !_outputFilesCopyLocation.isEmpty())
  {
    finalOutput = _outputFilesCopyLocation;
  }
  //just a test to make sure we can write to specified file output locations
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
  if (system(QString("psql --version > /dev/null").toStdString().c_str()) != 0)
  {
    throw HootException("Unable to access the psql application.  Is psql installed?");
  }
}

void OsmApiDbBulkWriter::_verifyOutputCopySettings()
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

void OsmApiDbBulkWriter::close()
{
  _closeOutputFiles();
  if (_destinationIsDatabase())
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

  if (_sqlOutputCombinedFile)
  {
    _sqlOutputCombinedFile->close();
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
    QString("\tUnresolved relation members: ") +
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

unsigned int OsmApiDbBulkWriter::_numberOfFileDataPasses() const
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

void OsmApiDbBulkWriter::_flushStreams(const bool writeClosingMark)
{
  for (QStringList::const_iterator it = _sectionNames.begin(); it != _sectionNames.end(); ++it)
  {
    if (_outputSections.find(*it) == _outputSections.end())
    {
      LOG_DEBUG("No data for table " + *it);
      continue;
    }

    LOG_TRACE("Flushing section " << *it << " to file " << _outputSections[*it].first->fileName());
    if (writeClosingMark && *it != "byte_order_mark")
    {
      LOG_TRACE("Writing closing byte order mark to stream...");
      *(_outputSections[*it].second) << QString("\\.\n\n\n");
    }
    // Flush any residual content from text stream/file
    _outputSections[*it].second->flush();
    if (!_outputSections[*it].first->flush())
    {
      throw HootException("Could not flush file for table " + *it);
    }
  }
}

void OsmApiDbBulkWriter::finalizePartial()
{
  LOG_INFO(
    "Input records parsed (data pass #" << _fileDataPassCtr << " of " <<
    _numberOfFileDataPasses() << ").  Time elapsed: " << _secondsToDhms(_timer->elapsed()));

  //go ahead and clear out some of the data structures we don't need anymore
  _idMappings.nodeIdMap.reset();
  _idMappings.wayIdMap.reset();
  _idMappings.relationIdMap.reset();
  _unresolvedRefs.unresolvedWaynodeRefs.reset();
  _unresolvedRefs.unresolvedRelationRefs.reset();

  if (_writeStats.nodesWritten == 0)
  {
    LOG_DEBUG("No input data was written to files.");
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
  _flushStreams(true);

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

  // Start initial section that holds nothing but UTF-8 byte-order mark (BOM)
  _createOutputFile("byte_order_mark", "\n", true);

  //combine all the element/changeset files that were written during partial streaming into
  //one file and update the ids in the SQL file according to the id sequences previously reserved
  //out
  _writeCombinedSqlFile();

  LOG_INFO("File write stats:");
  _logStats();

  if (_destinationIsDatabase())
  {
    _writeDataToDb();
    LOG_INFO("Final database write stats:");
  }
  else
  {
    LOG_DEBUG("Skipping SQL execution against database due to configuration...");
    LOG_INFO("Final file write stats:");
  }
  _logStats();
}

bool OsmApiDbBulkWriter::_destinationIsDatabase() const
{
  return _outputUrl.toLower().startsWith("osmapidb://");
}

void OsmApiDbBulkWriter::_writeDataToDbPsql()
{
  _timer->restart();
  _fileDataPassCtr++;
  //I believe a COPY header is created whether there are any records to copy for the table or not,
  //which is why the number of copy statements to be executed is hardcoded here.  Might be cleaner
  //to not write the header if there are no records to copy for the table...
  LOG_INFO(
    "Executing element SQL for " << _formatPotentiallyLargeNumber(_getTotalRecordsWritten()) <<
    " records (data pass #" << _fileDataPassCtr << " of " << _numberOfFileDataPasses() <<
    ").  17 separate SQL COPY statements will be executed...");

  //exec element sql against the db; Using psql here b/c it is doing buffered reads against the
  //sql file, so no need doing the extra work to handle buffering the sql read manually and
  //applying it to a QSqlQuery.
  const QMap<QString, QString> dbUrlParts = ApiDb::getDbUrlParts(_outputUrl);
  QString cmd = "export PGPASSWORD=" + dbUrlParts["password"] + "; psql";
  if (!(Log::getInstance().getLevel() <= Log::Info))
  {
    cmd += " --quiet";
  }
  cmd += " " + ApiDb::getPsqlString(_outputUrl) + " -f " + _sqlOutputCombinedFile->fileName();
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

void OsmApiDbBulkWriter::_writeDataToDb()
{
  _writeDataToDbPsql();
}

QString OsmApiDbBulkWriter::_getCombinedSqlFileName() const
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
    dest = QDir::tempPath() + "/OsmApiDbBulkWriter-" + QUuid::createUuid().toString() + ".sql";
  }
  return dest;
}

void OsmApiDbBulkWriter::_writeCombinedSqlFile()
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
  if (!_sqlOutputCombinedFile->open(QIODevice::Append))
  {
    throw HootException("Could not open file for SQL output: " + dest);
  }

  LOG_INFO(
    "Writing combined SQL output file to " << _sqlOutputCombinedFile->fileName() <<
    ".  (data pass #" << _fileDataPassCtr << " of " << _numberOfFileDataPasses() << ")...");
  LOG_VART(_sectionNames.size());
  LOG_VART(_outputSections.size());

  QTextStream outStream(_sqlOutputCombinedFile.get());
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
      LOG_DEBUG("Opening file: " << _outputSections[*it].first->fileName());
      if (tempInputFile.open(QIODevice::ReadOnly))
      {
        LOG_DEBUG("Parsing file for table: " << *it << "...");
        QTextStream inStream(&tempInputFile);
        QString line;
        long lineCtr = 0;
        const bool updateIdOffsets = _destinationIsDatabase() && _reserveRecordIdsBeforeWritingData;
        do
        {
          line = inStream.readLine();
          LOG_VART(line.left(100));

          if (!line.contains("COPY") && !line.isEmpty() && line != "\\.")
          {
            if (updateIdOffsets)
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
              "Flushing records to combined file " << _sqlOutputCombinedFile->fileName() << "...");
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
        LOG_DEBUG("Closing and removing file for " << *it << "...");
        _outputSections[*it].second.reset();
        _outputSections[*it].first->close();
        _outputSections[*it].first->remove();
        _outputSections[*it].first.reset();
      }
      else
      {
        throw HootException("Unable to open input file: " + tempInputFile.fileName());
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
  _sqlOutputCombinedFile->flush();
  _sqlOutputCombinedFile->close();

  LOG_INFO(
    "SQL file write complete.  (data pass #" << _fileDataPassCtr << " of " <<
    _numberOfFileDataPasses() << ").  Time elapsed: " << _secondsToDhms(_timer->elapsed()));
  LOG_DEBUG(
    "Parsed " << _formatPotentiallyLargeNumber(progressLineCtr) << " total SQL file lines.");
  QFileInfo outputInfo(_sqlOutputCombinedFile->fileName());
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
  if (_writeStats.nodesWritten == 0)
  {
    _timer.reset(new QElapsedTimer());
    _timer->start();
    _fileDataPassCtr++;
    LOG_INFO(
      "Streaming elements from input to file outputs.  (data pass #" <<
      _fileDataPassCtr << " of " << _numberOfFileDataPasses() << ")...");
    _createNodeOutputFiles();
    _idMappings.nodeIdMap.reset(new BigMap<long, long>());
  }

  LOG_VART(node);

  //TODO: See #1451.  This changeset bounds calculation actually won't work when ways or relations
  //are written in separate changesets than the nodes they reference.  Since we're streaming the
  //elements, there's no way to get back to the bounds information.  This bug has always been here,
  //but just recently noticed.
  //_changesetData.changesetBounds.expandToInclude(node->getX(), node->getY());
  //LOG_VART(_changesetData.changesetBounds.toString());

  // Do we already know about this node?
  if (_idMappings.nodeIdMap->contains(node->getId()))
  {
    throw NotImplementedException("Writer class does not support update operations.");
  }
  // Have to establish new mapping
  const long nodeDbId = _establishNewIdMapping(node->getElementId());
  LOG_VART(nodeDbId);

  _writeNodeToStream(node, nodeDbId);
  _writeTagsToStream(node->getTags(), ElementType::Node, nodeDbId,
    _outputSections[ApiDb::getCurrentNodeTagsTableName()].second,
    _outputSections[ApiDb::getNodeTagsTableName()].second);
  _writeStats.nodesWritten++;
  _writeStats.nodeTagsWritten += node->getTags().size();
  _incrementChangesInChangeset();
  _checkUnresolvedReferences(node, nodeDbId);

  if (((_writeStats.nodesWritten + _writeStats.nodeTagsWritten) / 2) %
      _fileOutputLineBufferSize == 0)
  {
    LOG_TRACE(
      "Flushing " << _formatPotentiallyLargeNumber(_fileOutputLineBufferSize) <<
      " lines of node data to files...");
    _flushStreams();
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

  // Do we already know about this way?
  if (_idMappings.wayIdMap->contains(way->getId()))
  {
    throw NotImplementedException("Writer class does not support update operations");
  }
  // Have to establish new mapping
  const long wayDbId = _establishNewIdMapping(way->getElementId());
  LOG_VART(wayDbId);

  _writeWayToStream(wayDbId);
  _writeWayNodesToStream(wayDbId, way->getNodeIds());
  _writeTagsToStream(way->getTags(), ElementType::Way, wayDbId,
    _outputSections[ApiDb::getCurrentWayTagsTableName()].second,
    _outputSections[ApiDb::getWayTagsTableName()].second);
  _writeStats.waysWritten++;
  _writeStats.wayTagsWritten += way->getTags().size();
  _writeStats.wayNodesWritten += way->getNodeIds().size();
  _incrementChangesInChangeset();
  _checkUnresolvedReferences(way, wayDbId);

  if (((_writeStats.waysWritten + _writeStats.wayTagsWritten + _writeStats.wayNodesWritten) / 2) %
      _fileOutputLineBufferSize == 0)
  {
    LOG_TRACE(
      "Flushing " << _formatPotentiallyLargeNumber(_fileOutputLineBufferSize) <<
      " lines of way data to files...");
    _flushStreams();
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

  // Do we already know about this node?
  if (_idMappings.relationIdMap->contains(relation->getId()))
  {
    throw NotImplementedException("Writer class does not support update operations");
  }
  // Have to establish new mapping
  const long relationDbId = _establishNewIdMapping(relation->getElementId());
  LOG_VART(relationDbId);

  _writeRelationToStream(relationDbId);
  _writeRelationMembersToStream(relation, relationDbId);
  _writeTagsToStream(relation->getTags(), ElementType::Relation, relationDbId,
    _outputSections[ApiDb::getCurrentRelationTagsTableName()].second,
    _outputSections[ApiDb::getRelationTagsTableName()].second);
  _writeStats.relationsWritten++;
  _writeStats.relationTagsWritten += relation->getTags().size();
  _writeStats.relationMembersWritten += relation->getMembers().size();
  _incrementChangesInChangeset();
  _checkUnresolvedReferences(relation, relationDbId);

  if (((_writeStats.relationsWritten + _writeStats.relationTagsWritten +
        _writeStats.relationMembersWritten) / 2) %
      _fileOutputLineBufferSize == 0)
  {
    LOG_TRACE(
      "Flushing " << _formatPotentiallyLargeNumber(_fileOutputLineBufferSize) <<
      " lines of relation data to files...");
    _flushStreams();
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

  setOutputFilesCopyLocation(confOptions.getOsmapidbBulkWriterOutputFilesCopyLocation().trimmed());
  _changesetData.changesetUserId = confOptions.getChangesetUserId();
  setFileOutputLineBufferSize(confOptions.getOsmapidbBulkWriterFileOutputBufferMaxLineSize());
  setStatusUpdateInterval(confOptions.getOsmapidbBulkWriterFileOutputStatusUpdateInterval());
  setMaxChangesetSize(confOptions.getChangesetMaxSize());
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
  LOG_VART(_outputUrl);
  LOG_VART(_outputDelimiter);
  LOG_VART(_reserveRecordIdsBeforeWritingData);
  LOG_VART(_idMappings.startingNodeId);
  LOG_VART(_idMappings.startingWayId);
  LOG_VART(_idMappings.startingRelationId);
}

QStringList OsmApiDbBulkWriter::_createSectionNameList()
{
  QStringList sections;
  sections.push_back("byte_order_mark");
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
  const QStringList nodeSqlHeaders = _sqlFormatter->getNodeSqlHeaderStrings();
  const QStringList nodeTagSqlHeaders = _sqlFormatter->getNodeTagSqlHeaderStrings();

  _createOutputFile(ApiDb::getCurrentNodesTableName(), nodeSqlHeaders[0]);
  _createOutputFile(ApiDb::getCurrentNodeTagsTableName(), nodeTagSqlHeaders[0]);

  _createOutputFile(ApiDb::getNodesTableName(), nodeSqlHeaders[1]);
  _createOutputFile(ApiDb::getNodeTagsTableName(), nodeTagSqlHeaders[1]);
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
    throw UnsupportedException("Unsupported element type.");
  }

  return dbIdentifier;
}

void OsmApiDbBulkWriter::_writeNodeToStream(const ConstNodePtr& node, const long nodeDbId)
{
  const QStringList nodeSqlStrs =
    _sqlFormatter->nodeToSqlStrings(node, nodeDbId, _changesetData.currentChangesetId);
  *(_outputSections[ApiDb::getCurrentNodesTableName()].second) << nodeSqlStrs[0];
  *(_outputSections[ApiDb::getNodesTableName()].second) << nodeSqlStrs[1];
}

void OsmApiDbBulkWriter::_writeTagsToStream(const Tags& tags, const ElementType::Type& elementType,
                                            const long dbId, shared_ptr<QTextStream>& currentTable,
                                            shared_ptr<QTextStream>& historicalTable)
{
  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    const QStringList tagSqlStrs =
      _sqlFormatter->tagToSqlStrings(ElementId(elementType, dbId), it.key(), it.value());
    *currentTable << tagSqlStrs[0];
    *historicalTable << tagSqlStrs[1];
  }
}

void OsmApiDbBulkWriter::_createWayOutputFiles()
{
  const QStringList waySqlHeaders = _sqlFormatter->getWaySqlHeaderStrings();
  const QStringList wayTagSqlHeaders = _sqlFormatter->getWayTagSqlHeaderStrings();
  const QStringList wayNodeSqlHeaders = _sqlFormatter->getWayNodeSqlHeaderStrings();

  _createOutputFile(ApiDb::getCurrentWaysTableName(), waySqlHeaders[0]);
  _createOutputFile(ApiDb::getCurrentWayTagsTableName(), wayTagSqlHeaders[0]);
  _createOutputFile(ApiDb::getCurrentWayNodesTableName(), wayNodeSqlHeaders[0]);

  _createOutputFile(ApiDb::getWaysTableName(), waySqlHeaders[1]);
  _createOutputFile(ApiDb::getWayTagsTableName(), wayTagSqlHeaders[1]);
  _createOutputFile(ApiDb::getWayNodesTableName(), wayNodeSqlHeaders[1]);
}

void OsmApiDbBulkWriter::_writeWayToStream(const long wayDbId)
{
  const QStringList waySqlStrs =
    _sqlFormatter->wayToSqlStrings(wayDbId, _changesetData.currentChangesetId);
  *(_outputSections[ApiDb::getCurrentWaysTableName()].second) << waySqlStrs[0];
  *(_outputSections[ApiDb::getWaysTableName()].second) << waySqlStrs[1];
}

void OsmApiDbBulkWriter::_writeWayNodesToStream(const long dbWayId, const vector<long>& waynodeIds)
{
  unsigned int wayNodeIndex = 1;
  const QString dbWayIdString( QString::number(dbWayId));
  for (vector<long>::const_iterator it = waynodeIds.begin(); it != waynodeIds.end(); ++it)
  {
    if (_idMappings.nodeIdMap->contains(*it))
    {
      const QStringList wayNodeSqlStrs =
        _sqlFormatter->wayNodeToSqlStrings(dbWayId, _idMappings.nodeIdMap->at(*it), wayNodeIndex);
      *_outputSections[ApiDb::getCurrentWayNodesTableName()].second << wayNodeSqlStrs[0];
      *_outputSections[ApiDb::getWayNodesTableName()].second << wayNodeSqlStrs[1];
    }
    else
    {
      throw UnsupportedException(
        "Unresolved waynodes are not supported.  " +
        QString("Way %1 has reference to unknown node ID %2").arg(dbWayId, *it));
    }
    ++wayNodeIndex;
  }
}

void OsmApiDbBulkWriter::_createRelationOutputFiles()
{
  const QStringList relationSqlHeaders = _sqlFormatter->getRelationSqlHeaderStrings();
  const QStringList relationTagSqlHeaders = _sqlFormatter->getRelationTagSqlHeaderStrings();
  const QStringList relationMemberSqlHeaders = _sqlFormatter->getRelationMemberSqlHeaderStrings();

  _createOutputFile(ApiDb::getCurrentRelationsTableName(), relationSqlHeaders[0]);
  _createOutputFile(ApiDb::getCurrentRelationTagsTableName(), relationTagSqlHeaders[0]);
  _createOutputFile(ApiDb::getCurrentRelationMembersTableName(), relationMemberSqlHeaders[0]);

  _createOutputFile(ApiDb::getRelationsTableName(), relationSqlHeaders[1]);
  _createOutputFile(ApiDb::getRelationTagsTableName(), relationTagSqlHeaders[1]);
  _createOutputFile(ApiDb::getRelationMembersTableName(),relationMemberSqlHeaders[1]);
}

void OsmApiDbBulkWriter::_writeRelationToStream(const long relationDbId)
{
  const QStringList relationSqlStrs =
    _sqlFormatter->relationToSqlStrings(relationDbId, _changesetData.currentChangesetId);
  *(_outputSections[ApiDb::getCurrentRelationsTableName()].second) << relationSqlStrs[0];
  *(_outputSections[ApiDb::getRelationsTableName()].second) << relationSqlStrs[1];
}

void OsmApiDbBulkWriter::_writeRelationMembersToStream(const ConstRelationPtr& relation,
                                                       const long dbRelationId)
{
  unsigned int memberSequenceIndex = 1;
  const long relationId = relation->getId();
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

void OsmApiDbBulkWriter::_writeRelationMemberToStream(const long relationDbId,
                                                      const RelationData::Entry& member,
                                                      const long memberDbId,
                                                      const unsigned int memberSequenceIndex)
{
  const QStringList relationMemberSqlStrs =
    _sqlFormatter->relationMemberToSqlStrings(relationDbId, memberDbId, member, memberSequenceIndex);
  *_outputSections[ApiDb::getCurrentRelationMembersTableName()].second << relationMemberSqlStrs[0];
  *_outputSections[ApiDb::getRelationMembersTableName()].second << relationMemberSqlStrs[1];
  _writeStats.relationMembersWritten++;
}

QString OsmApiDbBulkWriter::_getTableOutputFileName(const QString tableName) const
{
  return QDir::tempPath() + "/" + tableName + "-temp-" + QUuid::createUuid().toString() + ".sql";
}

void OsmApiDbBulkWriter::_createOutputFile(const QString tableName, const QString header,
                                           const bool addByteOrderMark)
{
  QString msg = "Creating output file " + tableName;
  if (!header.trimmed().isEmpty())
  {
    msg += " and writing table header";
  }
  msg += "...";
  LOG_TRACE(msg);

  const QString dest = _getTableOutputFileName(tableName);
  LOG_VART(dest);
  QFile outputFile(dest);
  if (outputFile.exists())
  {
    outputFile.remove();
  }
  shared_ptr<QFile> file(new QFile(dest));
  if (!file->open(QIODevice::Append))
  {
    throw HootException(
      "Could not open file at: " + file->fileName() + " for contents of table: " + tableName);
  }
  _outputSections[tableName] =
    pair<shared_ptr<QFile>, shared_ptr<QTextStream> >(
      file, shared_ptr<QTextStream>(new QTextStream(file.get())));

  // Database is encoded in UTF-8, so force encoding as otherwise file is in local
  //    Western encoding which goes poorly for a lot of countries
  _outputSections[tableName].second->setCodec("UTF-8");

  // First table written out should have byte order mark to help identify content as UTF-8
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
      throw UnsupportedException(
        "Found unresolved way node ref for node: " + QString::number(element->getId()));
    }
  }
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
    _createOutputFile(
      ApiDb::getChangesetsTableName(), _sqlFormatter->getChangesetSqlHeaderString());
  }

  *_outputSections[ApiDb::getChangesetsTableName()].second <<
    _sqlFormatter->changesetToSqlString(
      _changesetData.currentChangesetId,
      _changesetData.changesetUserId,
      _changesetData.changesInChangeset,
      _changesetData.changesetBounds);
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
