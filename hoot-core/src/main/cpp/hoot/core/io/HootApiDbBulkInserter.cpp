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

#include "HootApiDbBulkInserter.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/io/HootApiDbSqlStatementFormatter.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QDateTime>
#include <QFileInfo>
#include <QStringBuilder>
#include <QDir>
#include <QUuid>
#include <QLatin1String>
#include <QTextStream>

// Tgs
#include <tgs/System/SystemInfo.h>

// Std
#include <set>

namespace hoot
{

unsigned int HootApiDbBulkInserter::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapWriter, HootApiDbBulkInserter)

HootApiDbBulkInserter::HootApiDbBulkInserter() :
_outputDelimiter("\t"),
_fileDataPassCtr(0),
_includeDebugTags(ConfigOptions().getWriterIncludeDebugTags())
{
  _reset();
  setConfiguration(conf());
}

HootApiDbBulkInserter::~HootApiDbBulkInserter()
{
  close();
}

bool HootApiDbBulkInserter::isSupported(QString urlStr)
{
  LOG_VARD(urlStr);
  QUrl url(urlStr);
  return _database.isSupported(url) && _fastBulkInsertActivated;
}

void HootApiDbBulkInserter::open(QString url)
{
  _outputUrl = url;

  // Make sure we're not already open and the URL is valid
  if (!isSupported(url))
  {
    throw HootException(QString("Could not open URL ") + url);
  }

  _verifyStartingIds();
  if (_database.getDB().isOpen())
  {
    throw HootException(
      QString("Database already open.  Close the existing database connection before opening ") +
      QString("a new one.  URL: ") + url);
  }

  //TODO: a lot of duplicated code here from HootApiDbWriter

  if (_userEmail.isEmpty())
  {
    throw HootException("Please set the user's email address via the '" +
                        ConfigOptions::getApiDbEmailKey() + "' configuration setting.");
  }

  _database.open(_outputUrl);
  _database.transaction();

  // create the user before we have a transaction so we can make sure the user gets added.
  if (_createUserIfNotFound)
  {
    _database.setUserId(_database.getOrCreateUser(_userEmail, _userEmail));
  }
  else
  {
    _database.setUserId(_database.getUserId(_userEmail, true));
  }
  _changesetData.changesetUserId = _database.getUserId(_userEmail, true);

  QStringList pList = QUrl(url).path().split("/");
  QString mapName = pList[2];
  std::set<long> mapIds = _database.selectMapIds(mapName);
  assert(mapIds.size() == 1);
  if (mapIds.size() > 0)
  {
    if (_overwriteMap) // delete map and overwrite it
    {
      for (std::set<long>::const_iterator it = mapIds.begin(); it != mapIds.end(); ++it)
      {
        LOG_DEBUG("Removing map with ID: " << *it << "...");
        _database.deleteMap(*it);
        LOG_DEBUG("Finished removing map with ID: " << *it);
      }

      _database.setMapId(_database.insertMap(mapName, true));
    }
    else if (mapIds.size() > 1)
    {
      LOG_ERROR("There are multiple maps with this name. Consider using "
                "'hootapi.db.writer.overwrite.map'. Map IDs: " << mapIds);
    }
    else
    {
      std::set<long>::const_iterator idItr = mapIds.begin();
      _database.setMapId(*idItr);
      LOG_DEBUG("Updating map with ID: " << _database.getMapId() << "...");
    }
  }
  else if (mapIds.size() == 0)
  {
    LOG_DEBUG("Map " << mapName << " was not found, must insert.");
    _database.setMapId(_database.insertMap(mapName, true));
  }
  _database.commit();
  LOG_VART(_database.getMapId());

  _sectionNames = _createSectionNameList();
  _sqlFormatter.reset(new HootApiDbSqlStatementFormatter(_outputDelimiter, _database.getMapId()));

  _verifyDependencies();
}

void HootApiDbBulkInserter::_verifyStartingIds()
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

  _idMappings.currentNodeId = _idMappings.startingNodeId;
  _idMappings.currentWayId = _idMappings.startingWayId;
  _idMappings.currentRelationId = _idMappings.startingRelationId;
}

void HootApiDbBulkInserter::_verifyDependencies()
{
  if (system(QString("psql --version > /dev/null").toStdString().c_str()) != 0)
  {
    throw HootException("Unable to access the psql application.  Is Postgres installed?");
  }
}

void HootApiDbBulkInserter::close()
{
  LOG_DEBUG("Closing writer...");

  _closeOutputFiles();
  _database.close();

  _reset();
  _sectionNames = _createSectionNameList();
  setConfiguration(conf());
}

void HootApiDbBulkInserter::_closeOutputFiles()
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

void HootApiDbBulkInserter::_logStats(const bool debug)
{
  QStringList messages;
  messages.append(QString("\tNodes: ") + StringUtils::formatLargeNumber(_writeStats.nodesWritten));
  messages.append(
    QString("\tNode tags: ") + StringUtils::formatLargeNumber(_writeStats.nodeTagsWritten));
  messages.append(QString("\tWays: ") + StringUtils::formatLargeNumber(_writeStats.waysWritten));
  messages.append(
    QString("\tWay nodes: ") + StringUtils::formatLargeNumber(_writeStats.wayNodesWritten));
  messages.append(
    QString("\tWay tags: ") + StringUtils::formatLargeNumber(_writeStats.wayTagsWritten));
  messages.append(
    QString("\tRelations: ") + StringUtils::formatLargeNumber(_writeStats.relationsWritten));
  messages.append(
    QString("\tRelation members: ") +
    StringUtils::formatLargeNumber(_writeStats.relationMembersWritten));
  messages.append(
    QString("\tRelation tags: ") + StringUtils::formatLargeNumber(_writeStats.relationTagsWritten));
  messages.append(
    QString("\tUnresolved relation members: ") +
    StringUtils::formatLargeNumber(_writeStats.relationMembersUnresolved));
  messages.append(
    QString("\tTotal features: ") + StringUtils::formatLargeNumber(_getTotalFeaturesWritten()));
  messages.append(
    QString("\tChangesets: ") + StringUtils::formatLargeNumber(_changesetData.changesetsWritten));
  messages.append(
    QString("\tChangeset change size (each): ") +
    StringUtils::formatLargeNumber(_maxChangesetSize));
  messages.append(
    QString("\tExecutable SQL records: ") +
    StringUtils::formatLargeNumber(_getTotalRecordsWritten()));

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

unsigned int HootApiDbBulkInserter::_numberOfFileDataPasses() const
{
  return 3;
}

void HootApiDbBulkInserter::_flush()
{
  for (QStringList::const_iterator it = _sectionNames.begin(); it != _sectionNames.end(); ++it)
  {
    if (_outputSections.find(*it) == _outputSections.end())
    {
      LOG_TRACE("No data for table " + *it);
      continue;
    }

    LOG_VART(*it);
    LOG_TRACE("Flushing section " << *it << " to file " << _outputSections[*it]->fileName());
    _outputSections[*it]->write(QString("\\.\n\n").toUtf8());
    // Flush any residual content from file
    if (!_outputSections[*it]->flush())
    {
      throw HootException("Could not flush file for table " + *it);
    }
  }
}

void HootApiDbBulkInserter::_clearIdCollections()
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

void HootApiDbBulkInserter::finalizePartial()
{
  LOG_INFO(
    StringUtils::formatLargeNumber(_getTotalFeaturesWritten()) <<
    " input records parsed (data pass #" << _fileDataPassCtr << " of " <<
    _numberOfFileDataPasses() << ").  Time elapsed: " <<
    StringUtils::secondsToDhms(_timer->elapsed()));

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

  //combine all the element/changeset files that were written during partial streaming into
  //one file and update the ids in the SQL file according to the id sequences previously reserved
  //out
  _writeCombinedSqlFile();

  LOG_DEBUG("File write stats:");
  _logStats(true);

  _writeDataToDb();
  if (_outputFilesCopyLocation.isEmpty())
  {
    _sqlOutputCombinedFile->remove();
  }
  LOG_DEBUG("Final database write stats:");
  _logStats(true);
}

void HootApiDbBulkInserter::_writeDataToDbPsql()
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
  LOG_VART(_outputUrl);
  LOG_VART(HootApiDb::removeLayerName(_outputUrl));
  ApiDb::execSqlFile(HootApiDb::removeLayerName(_outputUrl), _sqlOutputCombinedFile->fileName());
  _database.close();

  LOG_INFO(
    "SQL execution complete.  Time elapsed: " << StringUtils::secondsToDhms(_timer->elapsed()));
}

void HootApiDbBulkInserter::_writeDataToDb()
{
  //indexes are already being handled in HootApiDb

  //TODO: figure out what's going on with constraints
  //_database.disableConstraints();

  _writeDataToDbPsql();

  //_database.enableConstraints();
}

QString HootApiDbBulkInserter::_getCombinedSqlFileName() const
{
  QString dest;
  if (!_outputFilesCopyLocation.isEmpty())
  {
    dest = _outputFilesCopyLocation;
  }
  else
  {
    dest = _tempDir + "/HootApiDbBulkInserter-" + QUuid::createUuid().toString() + ".sql";
  }
  return dest;
}

void HootApiDbBulkInserter::_writeCombinedSqlFile()
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

  //We're not reserving the ID ranges in the database, so we'll write the appropriate setval
  //statements to the sql output here for applying at a later time.  we want
  //setval to reflect the last id in the sequence
  QString reserveElementIdsSql;
  //TODO: may be able to collapse this logic; see notes in _establishIdMapping
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

        do
        {
          QString line;
          line.reserve(600);
          line.append(QString::fromUtf8(tempInputFile.readLine().constData()));
          LOG_VART(line.left(25));
          LOG_VART(line.length());

          if (!line.trimmed().isEmpty() &&
              line.trimmed() != QLatin1String("\\.") && !line.startsWith("COPY"))
          {
            recordCtr++;
          }

          _sqlOutputCombinedFile->write(QString(line).toUtf8());

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
    StringUtils::secondsToDhms(_timer->elapsed()));
  LOG_DEBUG("Parsed " << StringUtils::formatLargeNumber(recordCtr) << " total SQL file lines.");
  QFileInfo outputInfo(_sqlOutputCombinedFile->fileName());
  LOG_VART(Tgs::SystemInfo::humanReadable(outputInfo.size()));
}

unsigned long HootApiDbBulkInserter::_getTotalFeaturesWritten() const
{
  return _writeStats.nodesWritten + _writeStats.waysWritten + _writeStats.relationsWritten;
}

unsigned long HootApiDbBulkInserter::_getTotalRecordsWritten() const
{
  return
    _writeStats.nodesWritten +
    _writeStats.relationMembersWritten + _writeStats.relationsWritten +
    _writeStats.wayNodesWritten + _writeStats.waysWritten  + _changesetData.changesetsWritten;
}

void HootApiDbBulkInserter::writePartial(const ConstNodePtr& node)
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

  //TODO: See #1451.  This changeset bounds calculation actually won't work when ways or relations
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

void HootApiDbBulkInserter::writePartial(const ConstWayPtr& way)
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

  _writeWay(wayDbId, way->getTags());
  _writeWayNodes(wayDbId, way->getNodeIds());
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

void HootApiDbBulkInserter::writePartial(const ConstRelationPtr& relation)
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

  if (_includeDebugTags)
  {
    Tags tags = relation->getTags();
    //keep the hoot:id tag in sync with what could be a newly assigned id
    tags.set(MetadataTags::HootId(), QString::number(relationDbId));
  }

  _writeRelation(relationDbId, relation->getTags());
  _writeRelationMembers(relation, relationDbId);
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

void HootApiDbBulkInserter::setConfiguration(const Settings& conf)
{
  const ConfigOptions confOptions(conf);

  setOutputFilesCopyLocation(confOptions.getOsmapidbBulkInserterOutputFilesCopyLocation().trimmed());
  setFileOutputElementBufferSize(confOptions.getMaxElementsPerPartialMap());
  setStatusUpdateInterval(confOptions.getOsmapidbBulkInserterFileOutputStatusUpdateInterval());
  setMaxChangesetSize(confOptions.getChangesetMaxSize());
  setStartingNodeId(confOptions.getOsmapidbBulkInserterStartingNodeId());
  setStartingWayId(confOptions.getOsmapidbBulkInserterStartingWayId());
  setStartingRelationId(confOptions.getOsmapidbBulkInserterStartingRelationId());
  setStxxlMapMinSize(confOptions.getOsmapidbBulkInserterStxxlMapMinSize());
  setValidateData(confOptions.getOsmapidbBulkInserterValidateData());
  setTempDir(confOptions.getOsmapidbBulkInserterTempFileDir());
  setUserEmail(confOptions.getApiDbEmail());
  setCreateUser(confOptions.getHootapiDbWriterCreateUser());
  setOverwriteMap(confOptions.getHootapiDbWriterOverwriteMap());
  setFastBulkInsertActivated(confOptions.getHootapiDbWriterFastBulkInsert());
}

QStringList HootApiDbBulkInserter::_createSectionNameList()
{
  QStringList sections;
  sections.push_back(HootApiDb::getChangesetsTableName(_database.getMapId()));
  sections.push_back(HootApiDb::getCurrentNodesTableName(_database.getMapId()));
  sections.push_back(HootApiDb::getCurrentWaysTableName(_database.getMapId()));
  sections.push_back(HootApiDb::getCurrentWayNodesTableName(_database.getMapId()));
  sections.push_back(HootApiDb::getCurrentRelationsTableName(_database.getMapId()));
  sections.push_back(HootApiDb::getCurrentRelationMembersTableName(_database.getMapId()));
  return sections;
}

void HootApiDbBulkInserter::_createNodeOutputFiles()
{
  const QStringList nodeSqlHeaders = HootApiDbSqlStatementFormatter::getNodeSqlHeaderStrings(_database.getMapId());

  _createOutputFile(HootApiDb::getCurrentNodesTableName(_database.getMapId()), nodeSqlHeaders[0]);
}

void HootApiDbBulkInserter::_reset()
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

unsigned long HootApiDbBulkInserter::_establishIdMapping(const ElementId& sourceId)
{
  //TODO: can probably reduce the current element id increment logic to just that of when
  //_validateData = false

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
        dbIdentifier = abs(sourceId.getId());
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
        dbIdentifier = abs(sourceId.getId());
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
        dbIdentifier = abs(sourceId.getId());
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

void HootApiDbBulkInserter::_writeNode(const ConstNodePtr& node, const unsigned long nodeDbId)
{
  LOG_TRACE("Writing node to stream...");
  const QStringList nodeSqlStrs =
    _sqlFormatter->nodeToSqlStrings(
      node, nodeDbId, _changesetData.currentChangesetId, _validateData);
  LOG_VART(nodeSqlStrs[0]);
  _outputSections[HootApiDb::getCurrentNodesTableName(_database.getMapId())]->write(nodeSqlStrs[0].toUtf8());
}

void HootApiDbBulkInserter::_createWayOutputFiles()
{
  const QStringList waySqlHeaders = HootApiDbSqlStatementFormatter::getWaySqlHeaderStrings(_database.getMapId());
  const QStringList wayNodeSqlHeaders = HootApiDbSqlStatementFormatter::getWayNodeSqlHeaderStrings(_database.getMapId());

  _createOutputFile(HootApiDb::getCurrentWaysTableName(_database.getMapId()), waySqlHeaders[0]);
  _createOutputFile(HootApiDb::getCurrentWayNodesTableName(_database.getMapId()), wayNodeSqlHeaders[0]);
}

void HootApiDbBulkInserter::_writeWay(const unsigned long wayDbId, const Tags& tags)
{
  LOG_TRACE("Writing way to stream...");

  const QStringList waySqlStrs =
    _sqlFormatter->wayToSqlStrings(wayDbId, _changesetData.currentChangesetId, tags);
  _outputSections[HootApiDb::getCurrentWaysTableName(_database.getMapId())]->write(waySqlStrs[0].toUtf8());
}

void HootApiDbBulkInserter::_writeWayNodes(const unsigned long dbWayId,
                                          const std::vector<long>& wayNodeIds)
{
  LOG_TRACE("Writing way nodes to stream...");

  unsigned int wayNodeIndex = 1;
  for (std::vector<long>::const_iterator it = wayNodeIds.begin(); it != wayNodeIds.end(); ++it)
  {
    unsigned long wayNodeIdVal;
    if (!_validateData)
    {
      wayNodeIdVal = abs(*it);
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
    _outputSections[HootApiDb::getCurrentWayNodesTableName(_database.getMapId())]->write(wayNodeSqlStrs[0].toUtf8());

    ++wayNodeIndex;
  }
}

void HootApiDbBulkInserter::_createRelationOutputFiles()
{
  const QStringList relationSqlHeaders =
    HootApiDbSqlStatementFormatter::getRelationSqlHeaderStrings(_database.getMapId());
  const QStringList relationMemberSqlHeaders =
    HootApiDbSqlStatementFormatter::getRelationMemberSqlHeaderStrings(_database.getMapId());

  _createOutputFile(HootApiDb::getCurrentRelationsTableName(_database.getMapId()), relationSqlHeaders[0]);
  _createOutputFile(HootApiDb::getCurrentRelationMembersTableName(_database.getMapId()), relationMemberSqlHeaders[0]);
}

void HootApiDbBulkInserter::_writeRelation(const unsigned long relationDbId, const Tags& tags)
{
  LOG_TRACE("Writing relation to stream...");

  const QStringList relationSqlStrs =
    _sqlFormatter->relationToSqlStrings(relationDbId, _changesetData.currentChangesetId, tags);
  _outputSections[HootApiDb::getCurrentRelationsTableName(_database.getMapId())]->write(relationSqlStrs[0].toUtf8());
}

void HootApiDbBulkInserter::_writeRelationMembers(const ConstRelationPtr& relation,
                                                 const unsigned long dbRelationId)
{
  LOG_TRACE("Writing relation members to stream...");

  unsigned int memberSequenceIndex = 1;
  const long relationId = relation->getId();
  const std::vector<RelationData::Entry> relationMembers = relation->getMembers();
  boost::shared_ptr<Tgs::BigMap<long, unsigned long> > knownElementMap;

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
      unsigned long memberIdVal = abs(memberElementId.getId());
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

void HootApiDbBulkInserter::_writeRelationMember(const unsigned long sourceRelationDbId,
                                                const RelationData::Entry& member,
                                                const unsigned long memberDbId,
                                                const unsigned int memberSequenceIndex)
{
  const QStringList relationMemberSqlStrs =
    _sqlFormatter->relationMemberToSqlStrings(
      sourceRelationDbId, memberDbId, member, memberSequenceIndex);
  _outputSections[HootApiDb::getCurrentRelationMembersTableName(_database.getMapId())]->write(
    relationMemberSqlStrs[0].toUtf8());
  _writeStats.relationMembersWritten++;
}

void HootApiDbBulkInserter::_createOutputFile(const QString tableName, const QString header)
{
  QString msg = "Creating output file " + tableName;
  if (!header.trimmed().isEmpty())
  {
    msg += " and writing table header";
  }
  msg += "...";
  LOG_DEBUG(msg);

  _outputSections[tableName].reset(
    new QTemporaryFile(_tempDir + "/HootApiDbBulkInserter-" + tableName + "-temp-XXXXXX.sql"));
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

void HootApiDbBulkInserter::_incrementChangesInChangeset()
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

void HootApiDbBulkInserter::_checkUnresolvedReferences(const ConstElementPtr& element,
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

void HootApiDbBulkInserter::_writeChangeset()
{
  LOG_VART(_changesetData.changesetUserId);
  LOG_VART(_changesetData.currentChangesetId);

  if (_changesetData.changesetUserId == -1)
  {
    throw HootException(
      "Invalid changeset user ID: " + QString::number(_changesetData.changesetUserId));
  }

  if (!_outputSections[HootApiDb::getChangesetsTableName(_database.getMapId())])
  {
    _createOutputFile(
      HootApiDb::getChangesetsTableName(_database.getMapId()), _sqlFormatter->getChangesetSqlHeaderString(_database.getMapId()));
  }

  _outputSections[HootApiDb::getChangesetsTableName(_database.getMapId())]->write(
    _sqlFormatter->changesetToSqlString(
      _changesetData.currentChangesetId,
      _changesetData.changesetUserId,
      _changesetData.changesInChangeset,
      _changesetData.changesetBounds).toUtf8());
}

void HootApiDbBulkInserter::_writeSequenceUpdates(long changesetId, const unsigned long nodeId,
                                                 const unsigned long wayId,
                                                 const unsigned long relationId, QString& outputStr)
{
  LOG_DEBUG("Writing sequence updates stream...");

  if (changesetId <= 0) //TODO: probably can get rid of this
  {
    changesetId = 1;
  }

  QTextStream sequenceUpdatesStream(&outputStr);
  const QString sequenceUpdateFormat("SELECT pg_catalog.setval('%1', %2);\n");

  //At least one changeset and some nodes should always be written by a write operation; ways
  //and relations are optional.

  assert(changesetId > 0);
  sequenceUpdatesStream <<
    sequenceUpdateFormat.arg(HootApiDb::getChangesetsSequenceName(_database.getMapId()), QString::number(changesetId)).toUtf8();

  assert(nodeId > 0);
  sequenceUpdatesStream <<
    sequenceUpdateFormat.arg(HootApiDb::getCurrentNodesSequenceName(_database.getMapId()), QString::number(nodeId)).toUtf8();

  if (wayId > 0)
  {
    sequenceUpdatesStream <<
      sequenceUpdateFormat.arg(HootApiDb::getCurrentWaysSequenceName(_database.getMapId()), QString::number(wayId)).toUtf8();
  }

  if (relationId > 0)
  {
    sequenceUpdatesStream <<
      sequenceUpdateFormat.arg(
        HootApiDb::getCurrentRelationsSequenceName(_database.getMapId()), QString::number(relationId)).toUtf8();
  }

  sequenceUpdatesStream << QString("\n").toUtf8();
}

}
