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

#include <string>
#include <map>
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
 * This writer requires two passes over the data.  The first pass streams elements from input and
 * writes out generated SQL statements to temporary files for each database table type.  The second
 * pass combines the data from all temp SQL files into a single SQL file with the correct
 * statement ordering, as well as updates record IDs to ensure no ID conflicts will occur if external
 * writing occurred during the first pass of the data.
 *
 * Detailed workflow:
 *
 *   * write the element/changeset SQL copy statements out to individual temp SQL files in a
 *     buffered fashion with a first pass over the data; arbitrarily start all ID sequences at 1
 *
 *   * use the element counts obtained during the first pass SQL files write to determine the ID
 *     ranges the elements and changesets being written consume; lock these ID ranges out by
 *     executing setval statements against the database
 *
 *   * combine the temp files into a master SQL file in a buffered fashion in a second pass over
 *     the data; while parsing each record update element/changeset IDs based on those locked out
 *     in the previous step
 *
 *   * execute the element/changeset SQL copy statements against the database
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
    shared_ptr<BigMap<long, vector<pair<long, unsigned long> > > > unresolvedWaynodeRefs;
    shared_ptr<map<ElementId, UnresolvedRelationReference> > unresolvedRelationRefs;
  };

public:

  static string className() { return "hoot::OsmApiDbBulkWriter"; }

  static unsigned int logWarnCount;

  OsmApiDbBulkWriter();

  virtual ~OsmApiDbBulkWriter();

  virtual bool isSupported(QString url);
  virtual void open(QString url);
  virtual void close();

  virtual void finalizePartial();
  virtual void writePartial(const ConstNodePtr& n);
  virtual void writePartial(const ConstWayPtr& w);
  virtual void writePartial(const ConstRelationPtr& r);

  virtual void setConfiguration(const Settings& conf);

  void setFileOutputLineBufferSize(long size) { _fileOutputLineBufferSize = size; }
  void setStatusUpdateInterval(long interval) { _statusUpdateInterval = interval; }
  void setOutputFileCopyLocation(QString location) { _outputFileCopyLocation = location; }
  void setChangesetUserId(long id) { _changesetData.changesetUserId = id; }
  void setExecuteSql(bool exec) { _executeSql = exec; }
  void setMaxChangesetSize(long size) { _maxChangesetSize = size; }
  void setDisableWriteAheadLogging(bool disable) { _disableWriteAheadLogging = disable; }
  void setWriteMultithreaded(bool multithreaded) { _writeMultiThreaded = multithreaded; }
  void setDisableConstraints(bool disable) { _disableConstraints = disable; }
  void setMode(QString mode) { _mode = mode; }

protected:

  ElementWriteStats _writeStats;
  ChangesetData _changesetData;
  QString _outputFileCopyLocation;
  bool _executeSql;
  map<QString, pair<shared_ptr<QTemporaryFile>, shared_ptr<QTextStream> > > _outputSections;
  QStringList _sectionNames;
  bool _disableWriteAheadLogging;
  bool _writeMultiThreaded;
  QChar _delimiter;
  QString _outputUrl;
  bool _disableConstraints;
  QString _mode;
  OsmApiDb _database;
  IdMappings _idMappings;
  UnresolvedReferences _unresolvedRefs;
  QChar _outputDelimiter;

  void _logStats(const bool debug = false);
  long _getTotalRecordsWritten() const;

  virtual void _writeDataToDb();
  void _writeChangesetToStream();

  virtual void _retainOutputFiles();

  QString _formatPotentiallyLargeNumber(const long number);
  virtual QString _escapeCopyToData(const QString stringToOutput) const;

  virtual QString _getChangesetsOutputFormatString() const;
  virtual QString _getCurrentNodesOutputFormatString() const;
  virtual QString _getHistoricalNodesOutputFormatString() const;
  virtual QString _getCurrentWaysOutputFormatString() const;
  virtual QString _getHistoricalWaysOutputFormatString() const;
  virtual QString _getCurrentWayNodesOutputFormatString() const;
  virtual QString _getHistoricalWayNodesOutputFormatString() const;
  virtual QString _getCurrentRelationsOutputFormatString() const;
  virtual QString _getHistoricalRelationsOutputFormatString() const;
  virtual QString _getCurrentRelationMembersOutputFormatString() const;
  virtual QString _getHistoricalRelationMembersOutputFormatString() const;
  virtual QString _getCurrentTagsOutputFormatString() const;
  virtual QString _getHistoricalTagsOutputFormatString() const;

private:

  // for white box testing.
  friend class ServiceOsmApiDbBulkWriterTest;

  long _fileOutputLineBufferSize;
  long _statusUpdateInterval;
  long _maxChangesetSize;
  shared_ptr<QTemporaryFile> _sqlOutputMasterFile;

  void _reset();

  void _createNodeOutputFiles();
  QStringList _createSectionNameList();
  void _createWayOutputFiles();
  void _createRelationOutputFiles();
  void _createOutputFile(const QString tableName, const QString header = "",
                         const bool addByteOrderMarker = false);

  void _incrementAndGetLatestIdsFromDb();
  void _incrementChangesInChangeset();
  long _establishNewIdMapping(const ElementId& sourceId);
  void _checkUnresolvedReferences(const ConstElementPtr& element, const long elementDbId);

  unsigned int _convertDegreesToNanodegrees(const double degrees) const;

  void _writeSequenceUpdates(const long changesetId, const long nodeId, const long wayId,
                             const long relationId, QString& outputStr);
  void _writeRelationToStream(const long relationDbId);
  void _writeRelationMembersToStream(const ConstRelationPtr& relation);
  void _writeRelationMemberToStream(const long sourceRelation, const RelationData::Entry& memberEntry,
                                    const long memberDbId, const unsigned int memberSequenceIndex);
  void _writeWayToStream(const long wayDbId);
  void _writeWayNodesToStream(const long wayId,
    const vector<long>& waynodeIds);
  void _writeNodeToStream(const ConstNodePtr& node, const long nodeDbId);
  void _writeTagsToStream(const Tags& tags, const long nodeDbId,
                          shared_ptr<QTextStream>& currentTable, shared_ptr<QTextStream>& historicalTable);

  void _updateRecordLineWithIdOffset(const QString tableName, QString& sqlRecordLine);
  void _writeCombinedSqlFile();
  void _lockIds();
};

}

#endif // OSMAPIDBBULKWRITER_H
