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
 * OSM element writer optimized for bulk element inserts to an OSM API database.
 *
 * If you need to write small amounts of elements to an OSM API database or modify data in an
 * existing database, you're beter off creating a new writer class or using the Rails Port.
 *
 * This writer guarantees element ID uniqueness against a live database.  If the element SQL
 * write transaction fails, the element data will not be written but the IDs reserved will not be
 * freed and will go unused in the database.
 *
 * Workflow:
 *
 *   * write the element/changeset SQL copy statements out to individual temp SQL files in a
 *     buffered fashion with a first pass over the data; arbitrarily start all ID sequences at 1
 *
 *   * combine the temp files into a master SQL file in a buffered fashion in a second pass over
 *     the data
 *
 *   * use the element count obtained during the first pass SQL files write to determine the ID
 *     ranges the elements and changesets being written consume; lock these ID ranges out by
 *     executing setval statements against the database in a separate SQL exec
 *
 *   * update the ids in the master SQL file to those that were locked out in a third pass over
 *     the data
 *
 *   * execute the element/changeset SQL copy statements against the database
 *
 * @todo Get rid of the second pass over the data by writing all SQL to a master file from the
 * start, if possible.
 */
class OsmApiDbBulkWriter : public PartialOsmMapWriter, public Configurable
{
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
    long currentNodeId;
    shared_ptr<BigMap<long, long> > nodeIdMap;

    long currentWayId;
    shared_ptr<BigMap<long, long> > wayIdMap;

    long currentRelationId;
    shared_ptr<BigMap<long, long> > relationIdMap;
  };

  struct ChangesetData
  {
    long changesetUserId;
    long currentChangesetId;
    unsigned long changesetsWritten;
    unsigned int changesInChangeset;
    Envelope changesetBounds;
  };

  struct UnresolvedRelationReference
  {
    long sourceRelationId;
    long sourceDbRelationId;
    RelationData::Entry relationMemberData;
    unsigned int relationMemberSequenceId;
  };

  struct UnresolvedReferences
  {
    // Schema: node ID -> vector of entries w/ type: pair(way ID for waynode, 1-based sequence
    // order for waynode)
    shared_ptr<BigMap<long, vector< pair<long, unsigned long> > > > unresolvedWaynodeRefs;
    shared_ptr< map<ElementId, UnresolvedRelationReference > > unresolvedRelationRefs;
  };

public:

  static string className() { return "hoot::OsmApiDbBulkWriter"; }

  static unsigned int logWarnCount;

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

  void setFileOutputLineBufferSize(long size) { _fileOutputLineBufferSize = size; }
  void setStatusUpdateInterval(long interval) { _statusUpdateInterval = interval; }
  void setSqlFileCopyLocation(QString location) { _sqlFileCopyLocation = location; }
  void setChangesetUserId(long id) { _changesetData.changesetUserId = id; }
  void setExecuteSql(bool exec) { _executeSql = exec; }
  void setMaxChangesetSize(long size) { _maxChangesetSize = size; }

private:

  // for white box testing.
  friend class ServiceOsmApiDbBulkWriterTest;

  ElementWriteStats _writeStats;
  IdMappings _idMappings;
  ChangesetData _changesetData;
  UnresolvedReferences _unresolvedRefs;

  OsmApiDb _database;

  map<QString, pair<shared_ptr<QTemporaryFile>, shared_ptr<QTextStream> > > _outputSections;
  QStringList _sectionNames;

  QString _outputUrl;
  long _fileOutputLineBufferSize;
  long _statusUpdateInterval;
  QString _sqlFileCopyLocation;
  bool _executeSql;
  long _maxChangesetSize;
  long _totalFileLinesWrittenFirstPass;

  void _reset();

  void _createNodeTables();
  QStringList _createSectionNameList();
  void _createWayTables();
  void _createRelationTables();
  void _createTable(const QString tableName, const QString tableHeader);
  void _createTable(const QString tableName, const QString tableHeader,
                    const bool addByteOrderMarker);
  void _closeSectionTempFilesAndConcat();

  void _incrementAndGetLatestIdsFromDb();
  void _incrementChangesInChangeset();
  long _establishNewIdMapping(const ElementId& sourceId);
  void _checkUnresolvedReferences(const ConstElementPtr& element, const long elementDbId);

  unsigned int _convertDegreesToNanodegrees(const double degrees) const;
  QString _escapeCopyToData(const QString stringToOutput) const;

  void _writeChangesetToTable();
  void _writeSequenceUpdates(const long changesetId, const long nodeId, const long wayId,
                             const long relationId);
  void _writeRelationToTables(const long relationDbId);
  void _writeRelationMembersToTables(const ConstRelationPtr& relation);
  void _writeRelationMember(const long sourceRelation, const RelationData::Entry& memberEntry,
                            const long memberDbId, const unsigned int memberSequenceIndex);
  void _writeWayToTables(const long wayDbId);
  void _writeWaynodesToTables(const long wayId,
    const vector<long>& waynodeIds);
  void _writeNodeToTables(const ConstNodePtr& node, const long nodeDbId);
  void _writeTagsToTables(const Tags& tags, const long nodeDbId,
    shared_ptr<QTextStream>& currentTable, const QString currentTableFormatString,
    shared_ptr<QTextStream>& historicalTable, const QString historicalTableFormatString);

  shared_ptr<QTemporaryFile> _updateIdOffsetsInNewFile(shared_ptr<QTemporaryFile> inputSqlFile);
  void _executeElementSql(const QString sqlFile);
  void _writeCombinedSqlFile(shared_ptr<QTemporaryFile> sqlTempOutputFile);
  void _lockIds();
  long _getTotalRecordsWritten() const;

  QString _formatPotentiallyLargeNumber(const long number);
  void _logStats(const bool debug = false);
};

}

#endif // OSMAPIDBBULKWRITER_H
