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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HOOTAPIDB_H
#define HOOTAPIDB_H

#include <hoot/core/io/ApiDb.h>

namespace hoot
{

class HootApiDb : public ApiDb
{
public:

  HootApiDb();

  virtual ~HootApiDb();

  /**
   * Called after open. This will read the bounds of the specified layer in a relatively efficient
   * manner. (e.g. SELECT min(x)...)
   */
  virtual Envelope calculateEnvelope() const;

  virtual void close();

  virtual bool isCorrectDbVersion();

  virtual QString getDbVersion();

  virtual bool isSupported(QUrl url);

  virtual void open(const QUrl& url);

  virtual void transaction();

  virtual void rollback();

  virtual void commit();

  //reading

  /**
   * Returns a results iterator to all OSM elements for a given map and element type in the services
   * database.
   */
  virtual shared_ptr<QSqlQuery> selectElements(const ElementType& elementType);

  /**
   * Returns a vector with all the OSM node ID's for a given way
   */
  virtual vector<long> selectNodeIdsForWay(long wayId);

  /**
   * Returns a query results with node_id, lat, and long with all the OSM node ID's for a given way
   */
  shared_ptr<QSqlQuery> selectNodesForWay(long wayId);

  /**
   * Returns a vector with all the relation members for a given relation
   */
  vector<RelationData::Entry> selectMembersForRelation(long relationId);

  /**
   * Returns true if the map with the specified ID exists in the services database
   */
  bool mapExists(const long id);

  /**
   * Returns true if the changeset with the specified ID exists in the services database
   */
  bool changesetExists(const long id);

  /**
   * Returns the number of OSM elements of a given type for a particular map in the services
   * database
   */
  long numElements(const ElementType& elementType);

  //writing

  void endChangeset();

  /**
   * Creates necessary indexes and constraints on all maps that don't have indexes/constraints
   * already. Delaying this creation can leave the DB in a wonky state until this is called. Calling
   * commit() (implicitly called by destructor) will also call this method so the committed state
   * should never be inconsistent.
   *
   * This step is broken out solely for optimization purposes.
   */
  void createPendingMapIndexes();

  long getOrCreateUser(QString email, QString displayName);

  /**
   * Set user ID for current session, will be passed to DB in all places user ID is required
   *
   * @param sessionUserId The user ID to establish for this session
   */
  void setUserId(const long sessionUserId);

  /**
   * Set the Map ID for this session
   * @param sessionMapId The ID to set for the current map
   */
  void setMapId(const long sessionMapId);

  /**
   * Obtain current map ID for database
   * @return value of current map ID
   */
  long getMapId() const { return _currMapId; }

  /**
   * Deletes a map and all of it's dependencies.
   */
  void deleteMap(long mapId);

  virtual void deleteUser(long userId);

  /**
   * Start a new changeset
   *
   * @note The changeset will not have any tags associated with it
   */
  void beginChangeset();


  /**
   * Starts a new changeset
   * @param tags Tags for the new changeset
   *
   * @note to Obtain the current changeset Id, call getChangesetId
   */
  void beginChangeset(const Tags& tags);

  /**
   * @brief Insert a map into the database. This does not create the constraints and indexes.
   *
   * @sa createPendingMapIndexes();
   *
   * @param mapName Name of the map to create.
   * @param userId User id of the map owner.
   * @param publicVisibility Is the map publicly visible?
   * @return
   */
  long insertMap(QString mapName, bool publicVisibility = true);

  bool insertNode(const double lat, const double lon, const Tags &tags, long& assignedId);

  bool insertNode(const long id, const double lat, const double lon, const Tags &tags);

  bool insertWay(const Tags& tags, long& assignedId);

  bool insertWay( const long wayId, const Tags& tags);

  void insertWayNodes(long wayId, const vector<long>& nodeIds);

  bool insertRelation(const Tags& tags, long& assignedId);

  bool insertRelation(const long relationId, const Tags& tags);

  /**
   * Insert a new member into an existing relation
   * @param relationId Which relation we're adding a member to
   * @param type The type of element being added
   * @param elementId The ID for the element being added
   * @param role Role for the relation
   * @param sequenceId Sequence for the relation
   * @return True if success, else false
   */
  bool insertRelationMember(const long relationId, const ElementType& type,
    const long elementId, const QString& role, const int sequenceId);

  void insertRelationTag(long relationId, const QString& k, const QString& v);

  set<long> selectMapIds(QString name);

  void updateNode(const long id, const double lat, const double lon, const long version,
                  const Tags& tags);

  void updateRelation(const long id, const long version, const Tags& tags);

  void updateWay(const long id, const long version, const Tags& tags);

  long getChangesetId() const { return _currChangesetId; }

  void incrementChangesetChangeCount();

  /**
   * Reserve a unique indentifier for an element of the specified type
   * @param type The type of element that a unique ID is being requested for
   * @return the ID that was reserved for the element
   *
   * @note The returned ID is guaranteed to be unique for the specified type
   *  and will never be reused
   */
  long reserveElementId(const ElementType::Type type);

  /**
   * Drops the specified database. No warning or error will be given if the DB doesn't exist.
   *
   * No validation is done on the DB name. In other words, don't pass in user provided strings.
   *
   * @param databaseName
   */
  void dropDatabase(const QString& databaseName);

  /**
   * Drops the specified table and cascades (removes depedants). No warning or error will be given
   * if the table does not exist.
   *
   * No validation is done on the table names. In other words, don't pass in user provided strings.
   *
   * @param tableName
   */
  void dropTable(const QString& tableName);

  /**
   * Check if the database has table.
   */
  bool hasTable(const QString& tableName);

  /**
   * Returns a map ID string suitable for using in table names. E.g. _1
   */
  static QString getMapIdString(long id) { return QString("_%1").arg(id); }

  // Services DB table strings
  static QString getNodeSequenceName(long mapId)
  { return "current_nodes" + getMapIdString(mapId) + "_id_seq"; }
  static QString getRelationSequenceName(long mapId)
  { return "current_relations" + getMapIdString(mapId) + "_id_seq"; }
  static QString getWaySequenceName(long mapId)
  { return "current_ways" + getMapIdString(mapId) + "_id_seq"; }

  static QString getChangesetsTableName(long mapId)
  { return "changesets" + getMapIdString(mapId); }
  static QString getNodesTableName(long mapId)
  { return "current_nodes" + getMapIdString(mapId); }
  static QString getRelationMembersTableName(long mapId)
  { return "current_relation_members" + getMapIdString(mapId); }
  static QString getRelationsTableName(long mapId)
  { return "current_relations" + getMapIdString(mapId); }
  static QString getWayNodesTableName(long mapId)
  { return "current_way_nodes" + getMapIdString(mapId); }
  static QString getWaysTableName(long mapId)
  { return "current_ways" + getMapIdString(mapId); }

  /**
   * Very handy for testing.
   */
  QString execToString(QString sql, QVariant v1 = QVariant(), QVariant v2 = QVariant(),
                        QVariant v3 = QVariant());

private:

  bool _inTransaction;
  shared_ptr<QSqlQuery> _closeChangeSet;
  shared_ptr<QSqlQuery> _insertChangeSet;
  shared_ptr<QSqlQuery> _insertChangeSetTag;
  shared_ptr<QSqlQuery> _insertMap;
  shared_ptr<QSqlQuery> _insertRelationMembers;
  shared_ptr<QSqlQuery> _insertWayNodes;
  shared_ptr<QSqlQuery> _selectDbVersion;
  shared_ptr<QSqlQuery> _mapExists;
  shared_ptr<QSqlQuery> _changesetExists;
  shared_ptr<QSqlQuery> _numTypeElementsForMap;
  shared_ptr<QSqlQuery> _selectReserveNodeIds;
  shared_ptr<QSqlQuery> _selectElementsForMap;
  shared_ptr<QSqlQuery> _selectMapIds;
  shared_ptr<QSqlQuery> _selectMembersForRelation;
  shared_ptr<QSqlQuery> _updateNode;
  shared_ptr<QSqlQuery> _updateRelation;
  shared_ptr<QSqlQuery> _updateWay;

  shared_ptr<BulkInsert> _nodeBulkInsert;
  long _nodesPerBulkInsert;
  double _nodesInsertElapsed;
  shared_ptr<InternalIdReserver> _nodeIdReserver;

  shared_ptr<BulkInsert> _wayBulkInsert;
  long _waysPerBulkInsert;
  double _wayInsertElapsed;
  shared_ptr<InternalIdReserver> _wayIdReserver;

  shared_ptr<BulkInsert> _wayNodeBulkInsert;
  long _wayNodesPerBulkInsert;
  double _wayNodesInsertElapsed;

  shared_ptr<BulkInsert> _relationBulkInsert;
  long _relationsPerBulkInsert;
  shared_ptr<InternalIdReserver> _relationIdReserver;

  /// A vector of map ids that are pending index creation
  QVector<long> _pendingMapIndexes;

  long _lastMapId;

  long _currUserId;
  long _currMapId;
  long _currChangesetId;
  Envelope _changesetEnvelope;
  long _changesetChangeCount;

  unsigned long _nodesAddedToCache;
  unsigned long _nodesFlushedFromCache;

  /**
   * There are some statements that cannot be executed within a transaction
   * (like DROP DATABASE). There are times (when deleting a map) where we
   * want to do such things, if the current transaction succeeds. So we build
   * this list, and after a successful commit, all of these statements will
   * be executed.
   */
  QVector<QString> _postTransactionStatements;

  /**
   * This is here to improve query caching. In most cases users open a single ServiceDb and then
   * access one map ID over and over again. In these cases this class should perform pretty well
   * by lazily creating queries and caching them. This method checks the last map id against the
   * current map ID. If there is a change then all cached queries are flushed/cleared and we start
   * over.
   */
  void _checkLastMapId(long mapId);

  /**
   * Copies the structure of one table (from) to another table (to). The table (to) will be created
   * during this process. No data will be copied.
   *
   * No validation is done on the table names. In other words, don't pass in user provided strings.
   *
   * @param from Copy structure from this table.
   * @param to Copy structure to this table.
   */
  void _copyTableStructure(QString from, QString to);

  QString _elementTypeToElementTableName(long mapId, const ElementType& elementType) const;

  QString _escapeTags(const Tags& tags) const;

  void _flushBulkInserts();
  long _getNextNodeId();
  long _getNextRelationId();
  long _getNextWayId();
  void _init();
  void _lazyFlushBulkInsert();
  void _resetQueries();

  /**
   * Executes the insert, performs error checking and returns the new ID.
   */
  long _insertRecord(QSqlQuery& query);

  // Flushes all elements
  void _flushElementCacheToDb();

  // Only flushes specified type
  void _flushElementCacheToDb(const ElementType::Type type);

  void _updateChangesetEnvelope( const ConstNodePtr node );

  void _updateChangesetEnvelope(const ConstWayPtr way);

  void _updateChangesetEnvelopeWayIds(const std::vector<long>& wayIds);

  void _updateChangesetEnvelopeRelationIds(const std::vector<long>& relationIds);

  void _updateChangesetEnvelopeRelationNodes(const std::vector<long>& relationIds);

  void _updateChangesetEnvelopeRelationWays(const std::vector<long>& relationIds);

  /**
   * Builds the expected renderdb name from our current database & map
   * id.
   *
   * @param mapId id of the map for which we want the associated renderdb
   * @return should be <dbname>_renderdb_<map_id>
   */
  QString _getRenderDBName(long mapId);
};

}

#endif // HOOTAPIDB_H
