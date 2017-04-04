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
#include <QFile>
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
static const QString CHANGESETS_OUTPUT_FORMAT_STRING_DEFAULT =
  "%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8\t%9\n";
static const QString CURRENT_NODES_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\tt\t%5\t%6\t1\n";
static const QString HISTORICAL_NODES_OUTPUT_FORMAT_STRING_DEFAULT =
  "%1\t%2\t%3\t%4\tt\t%5\t%6\t1\t\\N\n";
static const QString CURRENT_WAYS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\tt\t1\n";
static const QString HISTORICAL_WAYS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t1\tt\t\\N\n";
static const QString CURRENT_WAY_NODES_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\n";
static const QString HISTORICAL_WAY_NODES_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t1\t%3\n";
static const QString CURRENT_RELATIONS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\tt\t1\n";
static const QString HISTORICAL_RELATIONS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t1\tt\t\\N\n";
static const QString CURRENT_RELATION_MEMBERS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\t%5\n";
static const QString HISTORICAL_RELATION_MEMBERS_OUTPUT_FORMAT_STRING_DEFAULT =
  "%1\t%2\t%3\t%4\t1\t%5\n";
static const QString CURRENT_TAGS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\n";
static const QString HISTORICAL_TAGS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t1\n";
//for whatever strange reason, the historical node tags table column order in the API datbase
//is different than the other historical tags tables; this makes a difference when using the
//offline loader, since it is sensitive to ordering
static const QString HISTORICAL_NODE_TAGS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t1\t%2\t%3\n";

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
 * * allows for directly writing to a target database or an generating output SQL file that can be
 *   manually written to a database
 *
 * * has the ability to guarantee element ID uniqueness against a live database if the record ID
 *   reservation option is used.
 *
 * * is transaction safe
 *
 * * requires two passes over the input data *before* writing it to the database
 */
class OsmApiDbBulkWriter : public PartialOsmMapWriter, public Configurable
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
    shared_ptr<BigMap<long, unsigned long> > nodeIdMap;

    unsigned long startingWayId;
    unsigned long currentWayId;
    shared_ptr<BigMap<long, unsigned long> > wayIdMap;

    unsigned long startingRelationId;
    unsigned long currentRelationId;
    shared_ptr<BigMap<long, unsigned long> > relationIdMap;
  };

  struct ChangesetData
  {
    long changesetUserId; //keep this as signed b/c the default is -1 to force users to update it
    unsigned long currentChangesetId;
    unsigned long changesetsWritten;
    unsigned int changesInChangeset;
    Envelope changesetBounds;
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

  void setFileOutputElementBufferSize(long size) { _fileOutputElementBufferSize = size; }
  void setStatusUpdateInterval(long interval) { _statusUpdateInterval = interval; }
  void setOutputFilesCopyLocation(QString location) { _outputFilesCopyLocation = location; }
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

private:

  // for white box testing.
  friend class ServiceOsmApiDbBulkWriterTest;

  ElementWriteStats _writeStats;
  ChangesetData _changesetData;
  IdMappings _idMappings;
  UnresolvedReferences _unresolvedRefs;

  long _fileOutputElementBufferSize;
  long _statusUpdateInterval;
  long _maxChangesetSize;
  QString _outputFilesCopyLocation;
  QString _outputUrl;
  QString _outputDelimiter;
  shared_ptr<QFile> _sqlOutputCombinedFile;
  bool _reserveRecordIdsBeforeWritingData;
  unsigned int _fileDataPassCtr;
  long _stxxlMapMinSize;
  bool _validateData;

  QMap<QString, QString> _outputFormatStrings;

  //ended up not going with temp files here, since the file outputs aren't always temporary
  map<QString, pair<shared_ptr<QFile>, shared_ptr<QTextStream> > > _outputSections;
  QStringList _sectionNames;

  OsmApiDb _database;

  shared_ptr<QElapsedTimer> _timer;

  unsigned int _convertDegreesToNanodegrees(const double degrees) const;
  QString _formatPotentiallyLargeNumber(const unsigned long number);
  QString _escapeCopyToData(const QString stringToOutput) const;
  QString _secondsToDhms(const qint64 durationInMilliseconds) const;

  void _reset();
  void _clearIdCollections();
  unsigned int _numberOfFileDataPasses() const;
  bool _destinationIsDatabase() const;

  void _logStats(const bool debug = false);
  unsigned long _getTotalRecordsWritten() const;
  void _verifyDependencies();
  void _verifyOutputCopySettings();
  void _verifyStartingIds();
  void _verifyFileOutputs();
  void _closeOutputFiles();
  void _flushStreams(const bool writeClosingMark = false);

  //creates the output files containing the data
  void _createNodeOutputFiles();
  QStringList _createSectionNameList();
  void _createWayOutputFiles();
  void _createRelationOutputFiles();
  void _createOutputFile(const QString tableName, const QString header = "",
                         const bool addByteOrderMarker = false);
  QString _getCombinedSqlFileName() const;
  QString _getTableOutputFileName(const QString tableName) const;
  void _initOutputFormatStrings();

  void _writeSequenceUpdatesToStream(unsigned long changesetId, const unsigned long nodeId,
                                     const unsigned long wayId,
                                     const unsigned long relationId, QString& outputStr);
  void _writeChangesetToStream();
  void _writeRelationToStream(const unsigned long relationDbId);
  void _writeRelationMembersToStream(const ConstRelationPtr& relation,
                                     const unsigned long dbRelationId);
  void _writeRelationMemberToStream(const unsigned long sourceRelationDbId,
                                    const RelationData::Entry& memberEntry,
                                    const unsigned long memberDbId,
                                    const unsigned int memberSequenceIndex);
  void _writeWayToStream(const unsigned long wayDbId);
  void _writeWayNodesToStream(const unsigned long wayId, const vector<long>& wayNodeIds);
  void _writeNodeToStream(const ConstNodePtr& node, const unsigned long nodeDbId);
  void _writeTagsToStream(const Tags& tags, const ElementType::Type& elementType,
                          const unsigned long dbId, shared_ptr<QTextStream>& currentTable,
                          shared_ptr<QTextStream>& historicalTable);

  void _incrementAndGetLatestIdsFromDb();
  void _incrementChangesInChangeset();
  /*
   * Since we're converting the input element IDs to our own sequence, we need to keep a mapping
   * between the two for reference.
   */
  unsigned long _establishNewIdMapping(const ElementId& sourceId);
  void _checkUnresolvedReferences(const ConstElementPtr& element, const unsigned long elementDbId);
  void _updateRecordLineWithIdOffset(const QString tableName, QString& recordLine);
  void _writeCombinedSqlFile();
  void _reserveIdsInDb();
  void _writeDataToDb();
  void _writeDataToDbPsql();
};

}

#endif // OSMAPIDBBULKWRITER_H
