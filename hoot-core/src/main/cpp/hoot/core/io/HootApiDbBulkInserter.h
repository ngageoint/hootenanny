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
#ifndef HOOTAPIDBBULKINSERTER_H
#define HOOTAPIDBBULKINSERTER_H

#include <string>
#include <map>
#include <vector>

#include <QString>
#include <QTemporaryFile>
#include <QElapsedTimer>

#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>

#include <tgs/BigContainers/BigMap.h>

namespace hoot
{

class HootApiDbSqlStatementFormatter;

/**
 * See OsmApiDbBulkInserter.  This class implements the offline workflow only and does not allow for
 * writing to a SQL file.
 *
 * TODO: duplicated code between OsmApiDbBulkInserter and this class needs to be vastly reduced
 * with a shared abstract parent class
 */
class HootApiDbBulkInserter : public PartialOsmMapWriter, public Configurable
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
    boost::shared_ptr<Tgs::BigMap<long, unsigned long> > nodeIdMap;

    unsigned long startingWayId;
    unsigned long currentWayId;
    boost::shared_ptr<Tgs::BigMap<long, unsigned long> > wayIdMap;

    unsigned long startingRelationId;
    unsigned long currentRelationId;
    boost::shared_ptr<Tgs::BigMap<long, unsigned long> > relationIdMap;
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
    boost::shared_ptr<std::map<ElementId, UnresolvedRelationReference> > unresolvedRelationRefs;
  };

public:

  static std::string className() { return "hoot::HootApiDbBulkInserter"; }

  static unsigned int logWarnCount;

  HootApiDbBulkInserter();
  virtual ~HootApiDbBulkInserter();

  virtual bool isSupported(QString url);
  virtual void open(QString url);
  void close();

  virtual void finalizePartial();
  virtual void writePartial(const ConstNodePtr& node);
  virtual void writePartial(const ConstWayPtr& way);
  virtual void writePartial(const ConstRelationPtr& relation);

  virtual void setConfiguration(const Settings& conf);

  long getMapId() const { return _database.getMapId(); }

  void setFileOutputElementBufferSize(long size) { _fileOutputElementBufferSize = size; }
  void setStatusUpdateInterval(long interval) { _statusUpdateInterval = interval; }
  void setOutputFilesCopyLocation(QString location) { _outputFilesCopyLocation = location; }
  void setChangesetUserId(long id) { _changesetData.changesetUserId = id; }
  void setMaxChangesetSize(long size) { _maxChangesetSize = size; }
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
  void setTempDir(QString location) { _tempDir = location; }
  void setCreateUser(bool createIfNotFound) { _createUserIfNotFound = createIfNotFound; }
  void setUserEmail(QString email) { _userEmail = email; }
  void setOverwriteMap(bool overwriteMap) { _overwriteMap = overwriteMap; }
  void setFastBulkInsertActivated(bool activated) { _fastBulkInsertActivated = activated; }

private:

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
  boost::shared_ptr<QFile> _sqlOutputCombinedFile;
  unsigned int _fileDataPassCtr;
  long _stxxlMapMinSize;
  bool _validateData;
  bool _includeDebugTags;
  QString _tempDir;
  QString _userEmail;
  bool _createUserIfNotFound;
  bool _overwriteMap;

  std::map<QString, boost::shared_ptr<QTemporaryFile> > _outputSections;
  QStringList _sectionNames;

  HootApiDb _database;
  boost::shared_ptr<HootApiDbSqlStatementFormatter> _sqlFormatter;

  bool _fastBulkInsertActivated;

  boost::shared_ptr<QElapsedTimer> _timer;

  void _reset();
  void _clearIdCollections();
  unsigned int _numberOfFileDataPasses() const;

  void _logStats(const bool debug = false);
  unsigned long _getTotalRecordsWritten() const;
  unsigned long _getTotalFeaturesWritten() const;
  void _verifyDependencies();
  void _verifyStartingIds();
  void _closeOutputFiles();
  void _flush();

  //creates the output files containing the data
  void _createNodeOutputFiles();
  QStringList _createSectionNameList();
  void _createWayOutputFiles();
  void _createRelationOutputFiles();
  void _createOutputFile(const QString tableName, const QString header = "");
  QString _getCombinedSqlFileName() const;

  void _writeSequenceUpdates(long changesetId, const unsigned long nodeId,
                             const unsigned long wayId, const unsigned long relationId,
                             QString& outputStr);
  void _writeChangeset();
  void _writeRelation(const unsigned long relationDbId, const Tags& tags);
  void _writeRelationMembers(const ConstRelationPtr& relation,
                             const unsigned long dbRelationId);
  void _writeRelationMember(const unsigned long sourceRelationDbId,
                            const RelationData::Entry& member,
                            const unsigned long memberDbId,
                            const unsigned int memberSequenceIndex);
  void _writeWay(const unsigned long wayDbId, const Tags& tags);
  void _writeWayNodes(const unsigned long wayId, const std::vector<long>& wayNodeIds);
  void _writeNode(const ConstNodePtr& node, const unsigned long nodeDbId);

  void _incrementChangesInChangeset();
  /*
   * Since we're converting the input element IDs to our own sequence, we need to keep a mapping
   * between the two for reference.
   */
  unsigned long _establishIdMapping(const ElementId& sourceId);
  void _checkUnresolvedReferences(const ConstElementPtr& element, const unsigned long elementDbId);
  void _writeCombinedSqlFile();
  void _writeDataToDb();
  void _writeDataToDbPsql();
};

}

#endif // HOOTAPIDBBULKINSERTER_H
