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

//These match up exclusively with the v0.6 OSM API database and shouldn't be changed.
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

/**
 * OSM element writer optimized for bulk element additions to an OSM API v0.6 database.
 *
 * If you need to write small amounts of elements to an OSM API database or modify data in an
 * existing database, you may be better off creating a new writer class or using the Rails Port.
 *
 * This writer:
 *
 * * has two intended workflows for use: online and offline.  The appropriate workflow can be set
 *   up by configuring the writer with the appropriate options.  See the user documentation for
 *   details on the two workflows and examples of how to use them.
 *
 * * gives the option of two underlying database writer applications: psql and pg_bulkload;
 *   currently, pg_bulkload is considered experimental (in the future it may be possible to
 *   discontinue use of psql and use pg_bulkload exclusively)
 *
 * * allows for directly writing to a target database or generating output files that can be
 *   manually written to a database; SQL files are written for the psql writer and CSV files are
 *   written for the pg_bulkload writer
 *
 * * has the ability to guarantee element ID uniqueness against a live database if the record ID
 *   reservation option is used.
 *
 * * is transaction safe when using the psql writer but not transaction safe with the pg_bulkload
 *   writer
 *
 * * requires two passes over the input data *before* writing it to the database if the psql writer
 *   is used OR the record ID reservation option is selected; only one pass over the data beforehand
 *   otherwise
 *
 * Originally, the psql and pg_bulkload sets of logic were separated into two classes
 * (pg_bulkload subclassing the psql logic).  In some ways, this resulted in cleaner code but in
 * other ways the code was harder to maintain.  The decision was made to collapse the logic into
 * the same class.  However, separating them again at some point may make sense if the two logic
 * paths end up being permanent
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
    long startingNodeId;
    long currentNodeId;
    shared_ptr<BigMap<long, long> > nodeIdMap;

    long startingWayId;
    long currentWayId;
    shared_ptr<BigMap<long, long> > wayIdMap;

    long startingRelationId;
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
    //keeps track of unresolved way nodees, which is a deal breaker when writing to the db
    // Schema: node ID -> vector of entries w/ type: pair(way ID for waynode, 1-based sequence
    // order for waynode)
    shared_ptr<BigMap<long, vector<pair<long, unsigned long> > > > unresolvedWaynodeRefs;
    //keeps track of unresolved relations, which aren't a deal breaker when writing to the db
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
  void setOutputFilesCopyLocation(QString location) { _outputFilesCopyLocation = location; }
  void setChangesetUserId(long id) { _changesetData.changesetUserId = id; }
  void setMaxChangesetSize(long size) { _maxChangesetSize = size; }
  void setDisableWriteAheadLogging(bool disable) { _disableWriteAheadLogging = disable; }
  void setWriteMultithreaded(bool multithreaded) { _writeMultiThreaded = multithreaded; }
  void setDisableConstraints(bool disable) { _disableConstraints = disable; }
  void setPgBulkloadLogPath(QString path) { _pgBulkLogPath = path; }
  void setPgBulkloadBadRecordsLogPath(QString path) { _pgBulkBadRecordsLogPath = path; }
  void setWriterApp(QString app) { _writerApp = app; }
  void setReserveRecordIdsBeforeWritingData(bool reserve)
  { _reserveRecordIdsBeforeWritingData = reserve; }
  void setStartingNodeId(long id) { _idMappings.startingNodeId = id; }
  void setStartingWayId(long id) { _idMappings.startingWayId = id; }
  void setStartingRelationId(long id) { _idMappings.startingRelationId = id; }

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
  QString _outputFilesCopyLocation;
  bool _disableWriteAheadLogging;
  bool _writeMultiThreaded;
  QString _outputUrl;
  bool _disableConstraints;
  QString _outputDelimiter;
  shared_ptr<QTemporaryFile> _sqlOutputMasterFile;
  QString _pgBulkLogPath;
  QString _pgBulkBadRecordsLogPath;
  QString _writerApp;
  bool _reserveRecordIdsBeforeWritingData;
  unsigned int _tempFileDataPassCtr;

  map<QString, pair<shared_ptr<QTemporaryFile>, shared_ptr<QTextStream> > > _outputSections;
  QStringList _sectionNames;

  OsmApiDb _database;

  shared_ptr<QElapsedTimer> _timer;

  unsigned int _convertDegreesToNanodegrees(const double degrees) const;
  QString _formatPotentiallyLargeNumber(const long number);
  QString _escapeCopyToData(const QString stringToOutput) const;
  QString _secondsToDhms(const qint64 durationInMilliseconds) const;

  void _reset();
  unsigned int _numberOfTempFileDataPasses() const;
  bool _destinationIsDatabase() const;

  void _logStats(const bool debug = false);
  long _getTotalRecordsWritten() const;
  void _retainOutputFiles(const QString finalLocation);
  void _retainOutputFilesPgBulk(const QString finalLocation);
  void _retainOutputFilesPsql(const QString finalLocation);
  void _verifyDependencies();
  void _verifyApp();
  void _verifyOutputCopySettings();
  void _verifyStartingIds();
  void _verifyFileOutputs();
  void _closeOutputFiles();
  void _flushTempStreams();
  void _handleFileOutputs();

  //creates the temporary output files containing the data
  void _createNodeOutputFiles();
  QStringList _createSectionNameList();
  void _createWayOutputFiles();
  void _createRelationOutputFiles();
  void _createOutputFile(const QString tableName, const QString header = "",
                         const bool addByteOrderMarker = false);

  void _writeSequenceUpdatesToStream(const long changesetId, const long nodeId, const long wayId,
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
  /*
   * Since we're converting the input element IDs to our own sequence, we need to keep a mapping
   * between the two for reference.
   */
  long _establishNewIdMapping(const ElementId& sourceId);
  void _checkUnresolvedReferences(const ConstElementPtr& element, const long elementDbId);
  void _updateRecordLineWithIdOffset(const QString tableName, QString& recordLine);
  void _writeCombinedSqlFile();
  void _updateRecordLinesWithIdOffsetInCsvFiles();
  void _reserveIdsInDb();
  void _writeDataToDb();
  void _writeDataToDbPgBulk();
  void _writeDataToDbPsql();
};

}

#endif // OSMAPIDBBULKWRITER_H
