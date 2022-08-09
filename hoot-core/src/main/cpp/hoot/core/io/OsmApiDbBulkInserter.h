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
#ifndef OSMAPIDBBULKINSERTER_H
#define OSMAPIDBBULKINSERTER_H

// Hoot
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>

// Qt
#include <QTemporaryFile>
#include <QElapsedTimer>

// Tgs
#include <tgs/BigContainers/BigMap.h>

namespace hoot
{

class OsmApiDbSqlStatementFormatter;

/**
 * OSM element writer optimized for bulk element additions to an OSM API v0.6 database.
 *
 * If you need to write small amounts of elements to an OSM API database or modify data in an
 * existing database, you should create a new writer class similar to how HootApiDbWriter works or
 * use the Rails Port instead.
 *
 * This writer:
 *
 * * has two intended workflows for use: online and offline.  The appropriate workflow can be set
 *   up by configuring the writer with the appropriate options.  See the user documentation for
 *   details on the two workflows and examples of how to use them.
 *
 * * allows for directly writing to a target database or for generating output SQL file that can be
 *   manually written to a database
 *
 * * has the ability to guarantee element ID uniqueness against a live database if the record ID
 *   reservation option is used.
 *
 * * is transaction safe
 *
 * * requires two passes over the input data *before* writing it to the database
 *
 * * uses the psql command to execute SQL statements; There is another version available that gives
 * the option of using pg_bulkload instead, which is several times faster than psql.  It is
 * currently in the 1446 development branch.
 */
class OsmApiDbBulkInserter : public PartialOsmMapWriter, public Configurable
{
  //some ids are stored as unsigned long and some as long due to the fact, the incoming data could
  //have positive or negative ids and when writing the output we end up with only positive ids

  struct ElementWriteStats
  {
    unsigned long nodesWritten;
    unsigned long nodeTagsWritten;
    unsigned long waysWritten;
    unsigned long wayNodesWritten;
    unsigned long wayTagsWritten;
    unsigned long relationsWritten;
    unsigned long relationMembersWritten;
    unsigned long relationMembersUnresolved;
    unsigned long relationTagsWritten;
  };

  struct IdMappings
  {
    unsigned long startingNodeId;
    unsigned long currentNodeId;
    std::shared_ptr<Tgs::BigMap<long, unsigned long>> nodeIdMap;

    unsigned long startingWayId;
    unsigned long currentWayId;
    std::shared_ptr<Tgs::BigMap<long, unsigned long>> wayIdMap;

    unsigned long startingRelationId;
    unsigned long currentRelationId;
    std::shared_ptr<Tgs::BigMap<long, unsigned long>> relationIdMap;
  };

  struct ChangesetData
  {
    long changesetUserId; //keep this as signed b/c the default is -1 to force users to update it
    unsigned long currentChangesetId;
    unsigned long changesetsWritten;
    unsigned int changesInChangeset;
    geos::geom::Envelope changesetBounds;
  };

  struct UnresolvedRelationReference
  {
    long sourceRelationId;
    unsigned long sourceDbRelationId;
    RelationData::Entry relationMemberData;
    unsigned int relationMemberSequenceId;
  };

  struct UnresolvedReferences
  {
    //keeps track of unresolved relations, which aren't a deal breaker when writing to the db
    std::shared_ptr<std::map<ElementId, UnresolvedRelationReference>> unresolvedRelationRefs;
  };

public:

  static QString className() { return "OsmApiDbBulkInserter"; }

  OsmApiDbBulkInserter();
  virtual ~OsmApiDbBulkInserter();

  bool isSupported(const QString& url) const override;
  void open(const QString& url) override;
  void close() override;
  QString supportedFormats() const override { return MetadataTags::OsmApiDbScheme() + "://"; }

  void finalizePartial() override;
  void writePartial(const ConstNodePtr& node) override;
  void writePartial(const ConstWayPtr& way) override;
  void writePartial(const ConstRelationPtr& relation) override;

  void setConfiguration(const Settings& conf) override;

  void setFileOutputElementBufferSize(long size) { _fileOutputElementBufferSize = size; }
  void setStatusUpdateInterval(long interval) { _statusUpdateInterval = interval; }
  void setOutputFilesCopyLocation(const QString& location) { _outputFilesCopyLocation = location; }
  void setChangesetUserId(long id) { _changesetData.changesetUserId = id; }
  void setMaxChangesetSize(long size) { _maxChangesetSize = size; }
  void setReserveRecordIdsBeforeWritingData(bool reserve)
  { _reserveRecordIdsBeforeWritingData = reserve; }
  void setStartingNodeId(long id)
  {
    if (id < 1)
    {
      throw HootException("Invalid starting ID: " + QString::number(id));
    }
    _idMappings.startingNodeId = id;
  }
  void setStartingWayId(long id)
  {
    if (id < 1)
    {
      throw HootException("Invalid starting ID: " + QString::number(id));
    }
    _idMappings.startingWayId = id;
  }
  void setStartingRelationId(long id)
  {
    if (id < 1)
    {
      throw HootException("Invalid starting ID: " + QString::number(id));
    }
    _idMappings.startingRelationId = id;
  }
  void setStxxlMapMinSize(long size) { _stxxlMapMinSize = size; }
  void setValidateData(bool validate) { _validateData = validate; }
  void setDisableDatabaseConstraintsDuringWrite(bool disable)
  { _disableDatabaseConstraintsDuringWrite = disable; }
  void setTempDir(const QString& location) { _tempDir = location; }
  void setDisableDatabaseIndexesDuringWrite(bool disable)
  { _disableDatabaseIndexesDuringWrite = disable; }
  void setWriteIdSequenceUpdates(bool write)
  { _writeIdSequenceUpdates = write; }

protected:

  static int logWarnCount;

  ElementWriteStats _writeStats;
  ChangesetData _changesetData;
  IdMappings _idMappings;

  long _statusUpdateInterval;
  QString _outputFilesCopyLocation;
  QString _outputUrl;
  QString _outputDelimiter;
  std::shared_ptr<QFile> _sqlOutputCombinedFile;
  unsigned int _fileDataPassCtr;
  long _stxxlMapMinSize;
  bool _validateData;
  bool _includeDebugTags;
  std::map<QString, std::shared_ptr<QTemporaryFile>> _outputSections;
  QStringList _sectionNames;
  std::shared_ptr<QElapsedTimer> _timer;
  long _maxChangesetSize;

  void _reset();
  void _clearIdCollections();
  void _verifyStartingIds();
  void _closeOutputFiles();
  void _flush();
  void _verifyDependencies() const;

  void _createOutputFile(const QString& tableName, const QString& header = "");
  QString _getCombinedSqlFileName() const;

  //creates the output files containing the data
  virtual void _createNodeOutputFiles();
  virtual QStringList _createSectionNameList();
  virtual void _createWayOutputFiles();
  virtual void _createRelationOutputFiles();

  virtual void _writeChangeset();
  virtual void _writeRelation(const unsigned long relationDbId,
                              const unsigned long version);
  void _writeRelationMembers(const ConstRelationPtr& relation,
                             const unsigned long dbRelationId,
                             const unsigned long version);
  virtual void _writeRelationMember(const unsigned long sourceRelationDbId,
                                    const RelationData::Entry& member,
                                    const unsigned long memberDbId,
                                    const unsigned int memberSequenceIndex,
                                    const unsigned long version);
  virtual void _writeWay(const unsigned long wayDbId,
                         const unsigned long version);
  virtual void _writeWayNodes(const unsigned long wayId,
                              const std::vector<long>& wayNodeIds,
                              const unsigned long version);
  virtual void _writeNode(const ConstNodePtr& node,
                          const unsigned long nodeDbId);
  virtual void _writeTags(const Tags& tags, const ElementType::Type& elementType,
                          const unsigned long dbId,
                          const std::shared_ptr<QFile>& currentTableFile,
                          const std::shared_ptr<QFile>& historicalTableFile,
                          const unsigned long version);

  virtual void _incrementChangesInChangeset();

  virtual void _writeDataToDb();
  virtual void _writeDataToDbPsql();

  virtual void _writeCombinedSqlFile();

  /*
   * Since we're converting the input element IDs to our own sequence, we need to keep a mapping
   * between the two for reference.
   */
  unsigned long _establishIdMapping(const ElementId& sourceId);
  void _checkUnresolvedReferences(const ConstElementPtr& element, const unsigned long elementDbId);

  virtual unsigned int _numberOfFileDataPasses() const;

  void _logStats(const bool debug = false) const;

  virtual unsigned long _getTotalRecordsWritten() const;
  virtual unsigned long _getTotalFeaturesWritten() const;

  virtual bool _destinationIsDatabase() const;

private:

  UnresolvedReferences _unresolvedRefs;

  long _fileOutputElementBufferSize;
  bool _reserveRecordIdsBeforeWritingData;
  bool _disableDatabaseConstraintsDuringWrite;
  QString _tempDir;
  bool _disableDatabaseIndexesDuringWrite;
  bool _writeIdSequenceUpdates;

  OsmApiDb _database;
  std::shared_ptr<OsmApiDbSqlStatementFormatter> _sqlFormatter;

  void _verifyOutputCopySettings() const;
  void _verifyFileOutputs() const;
  void _verifyChangesetUserId();

  void _incrementAndGetLatestIdsFromDb();
  void _updateRecordLineWithIdOffset(const QString& tableName, QString& recordLine) const;
  void _reserveIdsInDb();

  void _writeSequenceUpdates(long changesetId, const unsigned long nodeId,
                             const unsigned long wayId, const unsigned long relationId,
                             QString& outputStr) const;
};

}

#endif // OSMAPIDBBULKINSERTER_H
