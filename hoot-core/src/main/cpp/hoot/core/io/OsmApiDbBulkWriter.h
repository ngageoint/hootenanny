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
#ifndef OSMAPIDBBULKWRITER_H
#define OSMAPIDBBULKWRITER_H

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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include <string>
#include <map>
#include <list>
#include <vector>

#include <QString>
#include <QTemporaryFile>
#include <QTextStream>

#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>

#include <tgs/BigContainers/BigMap.h>

namespace hoot
{

using namespace boost;
using namespace std;
using namespace Tgs;

/**
 * OSM element writer optimized for bulk element writes.  It has two modes: offline and online.
 *
 * Offline mode is meant to be used against an offline database (one with no other writers
 * present). Offline mode does not guarantee element/changeset ID uniqueness against an online
 * database, since external writes can occur during the SQL file generation.  Offline mode will
 * run faster than online mode due an extra pass over the data required by online mode.
 *
 * Offline workflow:
 *
 *   * query for the current element/changeset ID sequences from the database
 *   * write the element/changeset SQL copy statements out to a file in a buffered fashion
 *   * execute the element/changeset SQL copy and ID sequence update statements against the database
 *
 * Online mode will guarantee element ID uniqueness against a live database  If the element SQL
 * write in online mode fails, the IDs reserved will not be freed and will go unused in the database.
 *
 * Online workflow:
 *
 *   * write the element/changeset SQL copy statements out to a file in a buffered fashion; arbitrarily
 *     start all IDs at 1
 *   * used the element count obtained during the first pass read to determine the IDs to be consumed
 *     by the write operation and lock the ID ranges out by executing setval statements against
 *     the database
 *   * execute the element/changeset SQL copy statements against the database
 */
class OsmApiDbBulkWriter : public PartialOsmMapWriter, public Configurable
{

public:

  static string className() { return "hoot::OsmApiDbBulkWriter"; }

  OsmApiDbBulkWriter();

  virtual ~OsmApiDbBulkWriter();

  virtual bool isSupported(QString url);

  virtual void open(QString url);

  void close();

  virtual void finalizePartial();

  virtual void writePartial(const ConstNodePtr& n);

  virtual void writePartial(const ConstWayPtr& w);

  virtual void writePartial(const ConstRelationPtr& r);

  virtual void setConfiguration(const hoot::Settings& conf);

  void setMode(QString mode)
  {
    if (mode != "offline" && mode != "online")
    {
      throw HootException("Invalid OSM API database bulk writer mode: " + mode);
    }
    _mode = mode;
  }
  void setFileOutputLineBufferSize(long size) { _fileOutputLineBufferSize = size; }
  void setStatusUpdateInterval(long interval) { _statusUpdateInterval = interval; }
  void setSqlFileCopyLocation(QString location) { _sqlFileCopyLocation = location; }

private:

  struct _ElementWriteStats
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
  _ElementWriteStats _writeStats;

  struct ConfigData
  {
    QString addUserEmail;
    long addUserId;
    long changesetUserId;
    long startingNodeId;
    long startingWayId;
    long startingRelationId;
    long startingChangesetId;
  };
  ConfigData _configData;

  struct _IdMappings
  {
    long nextNodeId;
    shared_ptr<BigMap<long, long> > nodeIdMap;

    long nextWayId;
    shared_ptr<BigMap<long, long> > wayIdMap;

    long nextRelationId;
    shared_ptr<BigMap<long, long> > relationIdMap;
  };
  _IdMappings _idMappings;

  struct _ChangesetData
  {
    long changesetId;
    unsigned long changesetsWritten;
    unsigned int changesInChangeset;
    Envelope changesetBounds;
  };
  _ChangesetData _changesetData;

  struct _UnresolvedRelationReference
  {
    long sourceRelationId;
    long sourceDbRelationId;
    RelationData::Entry relationMemberData;
    unsigned int relationMemberSequenceId;
  };
  struct _UnresolvedReferences
  {
    // Schema: node ID -> vector of entries w/ type: pair(way ID for waynode, 1-based sequence
    // order for waynode)
    shared_ptr<BigMap<long, vector< pair<long, unsigned long> > > > unresolvedWaynodeRefs;

    shared_ptr< map<ElementId, _UnresolvedRelationReference > > unresolvedRelationRefs;
  };
  _UnresolvedReferences _unresolvedRefs;

  OsmApiDb _database;

  map<QString, pair<shared_ptr<QTemporaryFile>, shared_ptr<QTextStream> > > _outputSections;
  list<QString> _sectionNames;

  QString _outputUrl;
  bool _dataWritten;
  QString _mode;
  long _fileOutputLineBufferSize;
  long _statusUpdateInterval;
  QString _sqlFileCopyLocation;

  void _reset();
  void _createNodeTables();
  list<QString> _createSectionNameList();
  void _createWayTables();
  void _createRelationTables();
  void _createTable(const QString& tableName, const QString& tableHeader);
  void _createTable(const QString& tableName, const QString& tableHeader,
                    const bool addByteOrderMarker);

  void _getStartingIdsFromDb();
  void _incrementChangesInChangeset();
  long _establishNewIdMapping(const ElementId& sourceId);
  void _checkUnresolvedReferences(const ConstElementPtr& element, const long elementDbId);

  unsigned int _convertDegreesToNanodegrees(const double degrees) const;
  QString _escapeCopyToData(const QString& stringToOutput) const;

  void _writeChangesetToTable();
  void _writeSequenceUpdates(const long nextChangesetId, const long nextNodeId,
                             const long nextWayId, const long nextRelationId);
  void _writeRelationToTables(const long relationDbId);
  void _writeRelationMembersToTables(const ConstRelationPtr& relation);
  void _writeRelationMember(const long sourceRelation,
    const RelationData::Entry& memberEntry, const long memberDbId,
    const unsigned int memberSequenceIndex);
  void _writeWayToTables(const long wayDbId);
  void _writeWaynodesToTables(const long wayId,
    const vector<long>& waynodeIds);
  void _writeNodeToTables(const ConstNodePtr& node, const long nodeDbId);
  void _writeTagsToTables(const Tags& tags, const long nodeDbId,
    shared_ptr<QTextStream>& currentTable, const QString& currentTableFormatString,
    shared_ptr<QTextStream>& historicalTable, const QString& historicalTableFormatString);
  void _closeSectionTempFilesAndConcat();

  shared_ptr<QTemporaryFile> _updateIdOffsets(shared_ptr<QTemporaryFile> inputSqlFile);
  void _executeElementSql(const QString sqlFile);
  void _writeMasterSqlFile(shared_ptr<QTemporaryFile> sqlTempOutputFile);
  void _lockIds();
  long _totalRecordsWritten() const;
};

}

#endif // OSMAPIDBBULKWRITER_H
