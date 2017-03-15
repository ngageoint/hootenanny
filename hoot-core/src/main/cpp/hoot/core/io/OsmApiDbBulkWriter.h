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
#include <QElapsedTimer>

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
 * OSM element writer optimized for bulk element additions to an OSM API v0.6 database.
 *
 * If you need to write small amounts of elements to an OSM API database or modify data in an
 * existing database, you're better off creating a new writer class or using the Rails Port.
 *
 * This writer has two modes: online and offline.
 *
 * Online mode:
 *
 * In this mode, the writer guarantees element ID uniqueness against a live database and uses psql
 * to write the data.  If the element SQL write transaction fails, the element data will not be
 * written but the IDs reserved will not be freed and will go unused in the database.
 *
 * In this mode, the writer requires two passes over the data before writing the data to the
 * database.  The first pass streams elements from input and writes out SQL COPY statements to
 * temporary files for each database table type.  The second pass combines the data from all temp
 * SQL files into a single SQL file with the correct table ordering, as well as updates record IDs
 * to ensure no ID conflicts will occur if external writing occurred during the first pass of the
 * data.
 *
 * Detailed online mode workflow:
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
 *
 * Offline mode:
 *
 * In this mode, the writer does not guarantee element ID uniqueness against a live database
 * (verify this is correct), so it must be used against an offline database (no other writers).
 * The writer also does not write the data within a transaction covering all the data (just a
 * transaction for each table type).  This mode also provides options that will result in better
 * data loading performance, including disabling constraint checking, disabling the write ahead log,
 * and multi-threaded writes.  See the user documentation for further details on when and how to
 * use these options.
 */
static const QString CHANGESETS_OUTPUT_FORMAT_STRING = "%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8\t%9\n";
static const QString CURRENT_NODES_OUTPUT_FORMAT_STRING = "%1\t%2\t%3\t%4\tt\t%5\t%6\t1\n";
static const QString HISTORICAL_NODES_OUTPUT_FORMAT_STRING = "%1\t%2\t%3\t%4\tt\t%5\t%6\t1\t\\N\n";
static const QString CURRENT_WAYS_OUTPUT_FORMAT_STRING = "%1\t%2\t%3\tt\t1\n";
static const QString HISTORICAL_WAYS_OUTPUT_FORMAT_STRING = "%1\t%2\t%3\t1\tt\t\\N\n";
static const QString CURRENT_WAY_NODES_OUTPUT_FORMAT_STRING = "%1\t%2\t%3\n";
static const QString HISTORICAL_WAY_NODES_OUTPUT_FORMAT_STRING = "%1\t%2\t1\t%3\n";
static const QString CURRENT_RELATIONS_OUTPUT_FORMAT_STRING = "%1\t%2\t%3\tt\t1\n";
static const QString HISTORICAL_RELATIONS_OUTPUT_FORMAT_STRING = "%1\t%2\t%3\t1\tt\t\\N\n";
static const QString CURRENT_RELATION_MEMBERS_OUTPUT_FORMAT_STRING = "%1\t%2\t%3\t%4\t%5\n";
static const QString HISTORICAL_RELATION_MEMBERS_OUTPUT_FORMAT_STRING = "%1\t%2\t%3\t%4\t1\t%5\n";
static const QString CURRENT_TAGS_OUTPUT_FORMAT_STRING = "%1\t%2\t%3\n";
static const QString HISTORICAL_TAGS_OUTPUT_FORMAT_STRING = "%1\t%2\t%3\t1\n";
//for whatever strange reason, the historical node tags table column order in the API datbase
//is different than the other historical tags tables; this makes a difference when using the
//offline loader, since it is sensitive to ordering
static const QString HISTORICAL_NODE_TAGS_OUTPUT_FORMAT_STRING = "%1\t1\t%2\t%3\n";

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
  void close();

  virtual void finalizePartial();
  virtual void writePartial(const ConstNodePtr& node);
  virtual void writePartial(const ConstWayPtr& way);
  virtual void writePartial(const ConstRelationPtr& relation);

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
  void setOfflineLogPath(QString path) { _pgBulkLogPath = path; }
  void setOfflineBadRecordsLogPath(QString path) { _pgBulkBadRecordsLogPath = path; }

private:

  // for white box testing.
  friend class ServiceOsmApiDbBulkWriterTest;

  ElementWriteStats _writeStats;
  ChangesetData _changesetData;
  IdMappings _idMappings;
  UnresolvedReferences _unresolvedRefs;

  long _fileOutputLineBufferSize;
  long _statusUpdateInterval;
  long _maxChangesetSize;
  QString _outputFileCopyLocation;
  bool _executeSql;
  bool _disableWriteAheadLogging;
  bool _writeMultiThreaded;
  QString _outputUrl;
  bool _disableConstraints;
  QString _mode;
  QString _outputDelimiter;
  shared_ptr<QTemporaryFile> _sqlOutputMasterFile;
  QString _pgBulkLogPath;
  QString _pgBulkBadRecordsLogPath;

  map<QString, pair<shared_ptr<QTemporaryFile>, shared_ptr<QTextStream> > > _outputSections;
  QStringList _sectionNames;

  OsmApiDb _database;

  shared_ptr<QElapsedTimer> _timer;

  unsigned int _convertDegreesToNanodegrees(const double degrees) const;
  QString _formatPotentiallyLargeNumber(const long number);
  QString _escapeCopyToData(const QString stringToOutput) const;
  QString _secondsToDhms(const qint64 durationInMilliseconds) const;

  void _reset();

  void _logStats(const bool debug = false);
  long _getTotalRecordsWritten() const;
  void _retainOutputFiles();
  void _retainOutputFilesOffline();
  void _retainOutputFilesOnline();
  void _verifyDependencies();
  void _verifyMode();
  void _verifyOutputCopySettings();
  void _closeOutputFiles();

  void _createNodeOutputFiles();
  QStringList _createSectionNameList();
  void _createWayOutputFiles();
  void _createRelationOutputFiles();
  void _createOutputFile(const QString tableName, const QString header = "",
                         const bool addByteOrderMarker = false);

  void _writeSequenceUpdates(const long changesetId, const long nodeId, const long wayId,
                             const long relationId, QString& outputStr);
  void _writeChangesetToStream();
  void _writeRelationToStream(const long relationDbId);
  void _writeRelationMembersToStream(const ConstRelationPtr& relation);
  void _writeRelationMemberToStream(const long sourceRelation,
                                    const RelationData::Entry& memberEntry, const long memberDbId,
                                    const unsigned int memberSequenceIndex);
  void _writeWayToStream(const long wayDbId);
  void _writeWayNodesToStream(const long wayId, const vector<long>& waynodeIds);
  void _writeNodeToStream(const ConstNodePtr& node, const long nodeDbId);
  void _writeTagsToStream(const Tags& tags, const ElementType::Type& elementType,
                          const long dbId, shared_ptr<QTextStream>& currentTable,
                          shared_ptr<QTextStream>& historicalTable);

  void _incrementAndGetLatestIdsFromDb();
  void _incrementChangesInChangeset();
  long _establishNewIdMapping(const ElementId& sourceId);
  void _checkUnresolvedReferences(const ConstElementPtr& element, const long elementDbId);
  void _updateRecordLineWithIdOffset(const QString tableName, QString& sqlRecordLine);
  void _writeCombinedSqlFile();
  void _reserveIds();
  void _writeDataToDb();
  void _writeDataToDbOffline();
  void _writeDataToDbOnline();
};

}

#endif // OSMAPIDBBULKWRITER_H
