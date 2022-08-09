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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "HootApiDbBulkInserter.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/io/HootApiDbSqlStatementFormatter.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QStringBuilder>
#include <QTextStream>

// Tgs
#include <tgs/System/SystemInfo.h>

namespace hoot
{

int HootApiDbBulkInserter::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapWriter, HootApiDbBulkInserter)

HootApiDbBulkInserter::HootApiDbBulkInserter() : OsmApiDbBulkInserter()
{
  _reset();
  setConfiguration(conf());

  _changesetTags["bot"] = "yes";
  _changesetTags["created_by"] = HOOT_PACKAGE_NAME;
}

HootApiDbBulkInserter::~HootApiDbBulkInserter()
{
  close();
}

bool HootApiDbBulkInserter::isSupported(const QString& urlStr) const
{
  LOG_VART(urlStr);
  QUrl url(urlStr);
  return _database.isSupported(url) && _copyBulkInsertActivated;
}

void HootApiDbBulkInserter::open(const QString& url)
{
  _outputUrl = url;

  // Make sure we're not already open and the URL is valid
  if (!isSupported(url))
  {
    throw HootException(QString("Could not open URL ") + url);
  }

  _verifyDependencies();
  _verifyStartingIds();
  if (_database.getDB().isOpen())
  {
    throw HootException(
      QString("Database already open.  Close the existing database connection before opening ") +
      QString("a new one.  URL: ") + _outputUrl);
  }
  _database.open(_outputUrl);
  _database.setCreateIndexesOnClose(false);
  _database.setFlushOnClose(false);

  //not starting a transaction here, b/c the exec'd SQL file will declare one instead
  _getOrCreateMap();
  LOG_VART(_database.getMapId());

  _sectionNames = _createSectionNameList();
  _sqlFormatter =
    std::make_shared<HootApiDbSqlStatementFormatter>(_outputDelimiter, _database.getMapId());
}

void HootApiDbBulkInserter::_getOrCreateMap()
{
  // There's a lot of overlap between this and similar code in HootApiDbWriter that needs
  // merging.

  if (_userEmail.isEmpty())
  {
    throw HootException("Please set the user's email address via the '" +
                        ConfigOptions::getApiDbEmailKey() + "' configuration setting.");
  }

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
  LOG_DEBUG("Creating changeset: " << _changesetData.currentChangesetId);

  // URL must have name in it
  const QStringList pList = QUrl(_outputUrl).path().split("/");
  const QString mapName = pList[2];
  LOG_VARD(mapName);

  const long mapId = _database.getMapIdByNameForCurrentUser(mapName);
  LOG_VART(mapId);

  if (mapId != -1)
  {
    if (_overwriteMap) // delete map and overwrite it
    {
      _database.verifyCurrentUserMapUse(mapId, true);
      _database.deleteMap(mapId);
      _database.setMapId(_database.insertMap(mapName));
    }
    else
    {
      _database.verifyCurrentUserMapUse(mapId, true);
      _database.setMapId(mapId);
      LOG_DEBUG("Updating map with ID: " << _database.getMapId() << "...");
    }
  }
  else// if (mapIds.size() == 0)
  {
    LOG_DEBUG("Map " << mapName << " was not found, must insert.");
    _database.setMapId(_database.insertMap(mapName));
  }
}

unsigned int HootApiDbBulkInserter::_numberOfFileDataPasses() const
{
  return 3;
}

void HootApiDbBulkInserter::finalizePartial()
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
  LOG_VART(HootApiDb::removeTableName(_outputUrl));
  ApiDb::execSqlFile(HootApiDb::removeTableName(_outputUrl), _sqlOutputCombinedFile->fileName());

  LOG_INFO(
    "SQL execution complete.  Time elapsed: " << StringUtils::millisecondsToDhms(_timer->elapsed()));
}

void HootApiDbBulkInserter::_writeDataToDb()
{
  _writeDataToDbPsql();

  //hoot api db starts with no indexes at all, since a brand new databas is created for each layer,
  //so let's create it now
  // This causes SQL exceptions with certain datasets, so disabling index creation for now. - #2216
  //_database.createPendingMapIndexes();
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
  _sqlOutputCombinedFile = std::make_shared<QFile>(dest);
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

  //skipping the writing of sequence updates here, since that doesn't seem to play nicely when
  //this class and HootApiDbWriter are mixed in the same workflow.  That works, b/c HootApiDbReader
  //handles sequences internally when the data is later read back out (?).

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
          //these element lines will contain tags
          QString line = QString::fromUtf8(tempInputFile.readLine().constData());
          LOG_VART(line.left(25));
          LOG_VART(line.length());

          if (!line.trimmed().isEmpty() &&
              line.trimmed() != QLatin1String("\\.") && !line.startsWith("COPY"))
          {
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
  LOG_VART(Tgs::SystemInfo::humanReadableStorageSize(outputInfo.size()));
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
    _timer = std::make_shared<QElapsedTimer>();
    _timer->start();
    _fileDataPassCtr++;
    LOG_INFO(
      "Streaming elements from input to file outputs.  (data pass #" <<
      _fileDataPassCtr << " of " << _numberOfFileDataPasses() << ")...");
    _createNodeOutputFiles();
    _idMappings.nodeIdMap = std::make_shared<Tgs::BigMap<long, unsigned long>>(_stxxlMapMinSize);
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

  //increment the changeset counter before writing the element in order to stay in sync with
  //HootApiDb
  _incrementChangesInChangeset();
  _writeNode(node, nodeDbId);
  _writeStats.nodesWritten++;
  _writeStats.nodeTagsWritten += node->getTags().size();
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
    _idMappings.wayIdMap = std::make_shared<Tgs::BigMap<long, unsigned long>>(_stxxlMapMinSize);
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

  //increment the changeset counter before writing the element in order to stay in sync with
  //HootApiDb
  _incrementChangesInChangeset();
  _writeWay(wayDbId, way->getTags(), way->getVersion());
  _writeWayNodes(wayDbId, way->getNodeIds(), way->getVersion());
  _writeStats.waysWritten++;
  _writeStats.wayTagsWritten += way->getTags().size();
  _writeStats.wayNodesWritten += way->getNodeIds().size();
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
    _idMappings.relationIdMap =
      std::make_shared<Tgs::BigMap<long, unsigned long>>(_stxxlMapMinSize);
  }

  //  Do we already know about this node?
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
  if (relation->getType() != "")
    tags["type"] = relation->getType();

  //  increment the changeset counter before writing the element in order to stay in sync with
  //  HootApiDb
  _incrementChangesInChangeset();
  _writeRelation(relationDbId, tags, relation->getVersion());
  _writeRelationMembers(relation, relationDbId, relation->getVersion());
  _writeStats.relationsWritten++;
  _writeStats.relationTagsWritten += relation->getTags().size();
  _writeStats.relationMembersWritten += relation->getMembers().size();
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
  OsmApiDbBulkInserter::setConfiguration(conf);

  const ConfigOptions confOptions(conf);
  setUserEmail(confOptions.getApiDbEmail());
  setCreateUser(confOptions.getHootapiDbWriterCreateUser());
  setOverwriteMap(confOptions.getHootapiDbWriterOverwriteMap());
  setCopyBulkInsertActivated(confOptions.getHootapiDbWriterCopyBulkInsert());
}

QStringList HootApiDbBulkInserter::_createSectionNameList()
{
  QStringList sections;
  sections.push_back(HootApiDb::getCurrentNodesTableName(_database.getMapId()));
  sections.push_back(HootApiDb::getCurrentWaysTableName(_database.getMapId()));
  sections.push_back(HootApiDb::getCurrentWayNodesTableName(_database.getMapId()));
  sections.push_back(HootApiDb::getCurrentRelationsTableName(_database.getMapId()));
  sections.push_back(HootApiDb::getCurrentRelationMembersTableName(_database.getMapId()));
  return sections;
}

void HootApiDbBulkInserter::_createNodeOutputFiles()
{
  _createOutputFile(
    HootApiDb::getCurrentNodesTableName(_database.getMapId()),
    HootApiDbSqlStatementFormatter::getNodeSqlHeaderString(_database.getMapId()));
}

void HootApiDbBulkInserter::_writeNode(const ConstNodePtr& node, const unsigned long nodeDbId)
{
  LOG_TRACE("Writing node to stream...");
  _outputSections[HootApiDb::getCurrentNodesTableName(_database.getMapId())]->write(
    _sqlFormatter->nodeToSqlString(
      node, nodeDbId, _changesetData.currentChangesetId, node->getVersion(), _validateData).toUtf8());
}

void HootApiDbBulkInserter::_createWayOutputFiles()
{
  _createOutputFile(
    HootApiDb::getCurrentWaysTableName(_database.getMapId()),
    HootApiDbSqlStatementFormatter::getWaySqlHeaderString(_database.getMapId()));
  _createOutputFile(
    HootApiDb::getCurrentWayNodesTableName(_database.getMapId()),
    HootApiDbSqlStatementFormatter::getWayNodeSqlHeaderString(_database.getMapId()));
}

void HootApiDbBulkInserter::_writeWay(const unsigned long wayDbId, const Tags& tags, const unsigned long version)
{
  LOG_TRACE("Writing way to stream...");
  _outputSections[HootApiDb::getCurrentWaysTableName(_database.getMapId())]->write(
    _sqlFormatter->wayToSqlString(wayDbId, _changesetData.currentChangesetId, tags, version).toUtf8());
}

void HootApiDbBulkInserter::_writeWayNodes(const unsigned long dbWayId,
                                           const std::vector<long>& wayNodeIds,
                                           const unsigned long /*version*/)
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

    _outputSections[HootApiDb::getCurrentWayNodesTableName(_database.getMapId())]->write(
      _sqlFormatter->wayNodeToSqlString(dbWayId, wayNodeIdVal, wayNodeIndex).toUtf8());

    ++wayNodeIndex;
  }
}

void HootApiDbBulkInserter::_createRelationOutputFiles()
{
  _createOutputFile(
    HootApiDb::getCurrentRelationsTableName(_database.getMapId()),
    HootApiDbSqlStatementFormatter::getRelationSqlHeaderString(_database.getMapId()));
  _createOutputFile(
    HootApiDb::getCurrentRelationMembersTableName(_database.getMapId()),
    HootApiDbSqlStatementFormatter::getRelationMemberSqlHeaderString(_database.getMapId()));
}

void HootApiDbBulkInserter::_writeRelation(const unsigned long relationDbId, const Tags& tags,
                                           const unsigned long version)
{
  LOG_TRACE("Writing relation to stream...");
  _outputSections[HootApiDb::getCurrentRelationsTableName(_database.getMapId())]->write(
    _sqlFormatter->relationToSqlString(relationDbId, _changesetData.currentChangesetId, tags, version).toUtf8());
}

void HootApiDbBulkInserter::_writeRelationMember(const unsigned long sourceRelationDbId,
                                                 const RelationData::Entry& member,
                                                 const unsigned long memberDbId,
                                                 const unsigned int memberSequenceIndex,
                                                 const unsigned long version)
{
  _outputSections[HootApiDb::getCurrentRelationMembersTableName(_database.getMapId())]->write(
    _sqlFormatter->relationMemberToSqlString(
      sourceRelationDbId, memberDbId, member, memberSequenceIndex, version).toUtf8());
  _writeStats.relationMembersWritten++;
}

void HootApiDbBulkInserter::_incrementChangesInChangeset()
{
  //To stay in sync with how HootApiDb assigns changeset ID's, we'll get the initial changeset ID
  //if this is the first record being writen.  Changeset ID's will then be retrieved with each call
  //to _writeChangeset.
  //This seems to be writing double the amount of changesets needed with every other changeset
  //being empty. - #2217
  if (_changesetData.changesInChangeset ==/*>*/ 0)
  {
    _writeChangeset();
  }
  _changesetData.changesInChangeset++;
  if (_changesetData.changesInChangeset == _maxChangesetSize)
  {
    LOG_VART(_changesetData.changesInChangeset);
    _writeChangeset();
    LOG_VART(_changesetData.currentChangesetId);
    _changesetData.changesInChangeset = 0;
    _changesetData.changesetBounds.init();
    _changesetData.changesetsWritten++;
    LOG_VART(_changesetData.changesetsWritten);
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

  //Rather than write the changeset as a COPY statement out to file, just going to write it directly
  //to the db.  Rhis helps this class and HootApiDbWriter play nicely when the two are mixed together
  //in a data workflow.  A downside to this is that the changeset insert isn't part of the SQL
  //transaction used with the copy statements.
  _changesetData.currentChangesetId =
    _database.insertChangeset(_changesetData.changesetBounds, _changesetTags, _maxChangesetSize);
  LOG_DEBUG(
    "Inserted changeset with ID: " << _changesetData.currentChangesetId << " and " <<
    _changesetData.changesInChangeset << " changes.");
}

}
