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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HOOTAPIDB_H
#define HOOTAPIDB_H

#include <hoot/core/io/ApiDb.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/BulkDelete.h>
#include <hoot/core/io/BulkInsert.h>

namespace hoot
{

class InternalIdReserver;

/**
 * Used for interaction with an Hootenanny API database (Hootenanny's internal customized version
 * of the OSM API database).
 */
class HootApiDb : public ApiDb
{
public:

  static std::string className() { return "hoot::HootApiDb"; }

  static unsigned int logWarnCount;

  HootApiDb();

  virtual ~HootApiDb();

  /**
   * Called after open. This will read the bounds of the specified layer in a relatively efficient
   * manner. (e.g. SELECT min(x)...)
   */
  virtual geos::geom::Envelope calculateEnvelope() const;

  virtual void close();

  virtual bool isCorrectHootDbVersion();

  virtual QString getHootDbVersion();

  virtual bool isSupported(const QUrl& url) override;

  virtual void open(const QUrl& url) override;

  virtual void commit();

  /**
   * @see ApiDb::elementTypeToElementTableName
   */
  virtual QString elementTypeToElementTableName(const ElementType& elementType) const override;

  /**
   * Returns a vector with all the OSM node ID's for a given way
   */
  virtual std::vector<long> selectNodeIdsForWay(long wayId) override;

  /**
   * Returns a query results with node_id, lat, and long with all the OSM node ID's for a given way
   */
  boost::shared_ptr<QSqlQuery> selectNodesForWay(long wayId) override;

  /**
   * Returns a vector with all the relation members for a given relation
   */
  std::vector<RelationData::Entry> selectMembersForRelation(long relationId) override;

  /**
   * Returns true if the map with the specified ID exists in the services database
   */
  bool mapExists(const long id);

  /**
   * Returns true if any map with the specified name exists in the services database
   */
  bool mapExists(const QString name);

  /**
   * Returns true if the changeset with the specified ID exists in the services database
   */
  bool changesetExists(const long id);

  /**
   * Returns the number of changesets for the given map in the database;
   *
   * @return the number of changesets for the map
   */
  long numChangesets();

  /**
   * Closes an existing changeset
   */
  void endChangeset();

  /**
   * Inserts and closes a changeset
   *
   * @param bounds bounds to associate with the changeset
   * @param tags tags to associate with the changeset
   * @param numChanges the number of changes in the changeset
   * @return the ID of the inserted changeset
   */
  long insertChangeset(const geos::geom::Envelope& bounds, const Tags& tags, const long numChanges);

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
   * @return
   */
  long insertMap(QString mapName);

  bool insertNode(const double lat, const double lon, const Tags &tags, long& assignedId);

  bool insertNode(const long id, const double lat, const double lon, const Tags &tags);

  bool insertNode(ConstNodePtr node);

  bool insertWay(const Tags& tags, long& assignedId);

  bool insertWay( const long wayId, const Tags& tags);

  void insertWayNodes(long wayId, const std::vector<long>& nodeIds);

  bool insertRelation(const Tags& tags, long& assignedId);

  bool insertRelation(const long relationId, const Tags& tags);

  /**
   * Insert a new member into an existing relation
   *
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

  /**
   * Returns the ID of the map with the given name created by the currently configured user
   *
   * @param name map name
   * @return a map ID; -1 if no map is found
   */
  long selectMapIdForCurrentUser(QString name);

  /**
   * Returns all public map IDs for a map with the give name
   *
   * @param name map name
   * @return a collection of map IDs
   */
  std::set<long> selectPublicMapIds(QString name);

  /**
   * Returns the IDs of all maps with the given name
   *
   * @param name map name
   * @return a collection of map IDs
   */
  std::set<long> selectMapIds(QString name);

  void updateNode(const long id, const double lat, const double lon, const long version,
                  const Tags& tags);

  void updateNode(ConstNodePtr node);

  void deleteNode(ConstNodePtr node);

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
   * Drops the specified table and cascades (removes dependants). No warning or error will be given
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
   * Drops the specified sequences and cascades. No warning or error will be given
   * if the sequence does not exist.
   *
   * No validation is done on the sequence name. In other words, don't pass in user provided strings.
   *
   * @param sequenceName
   */
  void dropSequence(const QString& sequenceName);

  /**
   * Returns a map ID string suitable for using in table names. E.g. _1
   */
  inline static QString getMapIdString(long id) { return QString("_%1").arg(id); }

  // Services DB table strings
  inline static QString getChangesetsTableName(long mapId)
  { return ApiDb::getChangesetsTableName() + getMapIdString(mapId); }
  inline static QString getCurrentNodesTableName(long mapId)
  { return ApiDb::getCurrentNodesTableName() + getMapIdString(mapId); }
  inline static QString getCurrentRelationMembersTableName(long mapId)
  { return ApiDb::getCurrentRelationMembersTableName() + getMapIdString(mapId); }
  inline static QString getCurrentRelationsTableName(long mapId)
  { return ApiDb::getCurrentRelationsTableName() + getMapIdString(mapId); }
  inline static QString getCurrentWayNodesTableName(long mapId)
  { return ApiDb::getCurrentWayNodesTableName() + getMapIdString(mapId); }
  inline static QString getCurrentWaysTableName(long mapId)
  { return ApiDb::getCurrentWaysTableName() + getMapIdString(mapId); }

  inline static QString getChangesetsSequenceName(long mapId)
  { return ApiDb::getChangesetsTableName() + getMapIdString(mapId) + ApiDb::getSequenceId(); }
  inline static QString getCurrentNodesSequenceName(long mapId)
  { return ApiDb::getCurrentNodesTableName() + getMapIdString(mapId) + ApiDb::getSequenceId(); }
  inline static QString getCurrentRelationMembersSequenceName(long mapId)
  { return ApiDb::getCurrentRelationMembersTableName() + getMapIdString(mapId) + ApiDb::getSequenceId(); }
  inline static QString getCurrentRelationsSequenceName(long mapId)
  { return ApiDb::getCurrentRelationsTableName() + getMapIdString(mapId) + ApiDb::getSequenceId(); }
  inline static QString getCurrentWayNodesSequenceName(long mapId)
  { return ApiDb::getCurrentWayNodesTableName() + getMapIdString(mapId) + ApiDb::getSequenceId(); }
  inline static QString getCurrentWaysSequenceName(long mapId)
  { return ApiDb::getCurrentWaysTableName() + getMapIdString(mapId) + ApiDb::getSequenceId(); }
  inline static QString getMapsSequenceName()
  { return getMapsTableName() + getSequenceId(); }

  inline static QString getMapsTableName()                      { return "maps"; }
  inline static QString getFoldersTableName()                   { return "folders"; }
  inline static QString getFolderMapMappingsTableName()         { return "folder_map_mappings"; }

  inline static QString getJobStatusTableName() { return "job_status"; }

  inline static QString getUserSessionTableName() { return "spring_session"; }

  inline static QString currentTimestampAsBigIntSql()
  { return "extract('epoch' FROM CURRENT_TIMESTAMP)::bigint"; }

  /**
   * Very handy for testing.
   */
  QString execToString(QString sql, QVariant v1 = QVariant(), QVariant v2 = QVariant(),
                       QVariant v3 = QVariant());

  virtual QString tableTypeToTableName(const TableType& tableType) const override;

  virtual long getNextId(const ElementType& elementType);

  static QUrl getBaseUrl();

  /**
   * Given a map name, returns its ID; assumes only one map with the given name
   *
   * @param name name of the map to retrieve
   * @return a map ID
   */
  long getMapIdByName(const QString name);

  /**
   * Removes the layer name from a Hooteanny API database URL
   *
   * @param url input URL
   * @return a URL with the layer name removed
   */
  static QString removeLayerName(const QString url);

  void setCreateIndexesOnClose(bool create) { _createIndexesOnClose = create; }
  void setFlushOnClose(bool flush) { _flushOnClose = flush; }

  /**
   * Retrieve the HTTP session ID associated with a user
   *
   * @param userName user name of the user
   * @param accessToken public OAuth access token for the user
   * @param accessTokenSecret private OAuth access token for the user
   * @return an HTTP session ID or an empty string if no session ID was found for the given input
   */
  QString getSessionIdByAccessTokens(const QString userName, const QString accessToken,
                                     const QString accessTokenSecret);

  /**
   * Determines whether a set of access tokens are valid for a user
   *
   * @param userName user name of the user
   * @param accessToken public OAuth access token for the user
   * @param accessTokenSecret private OAuth access token for the user
   * @return true if the access tokens passed in are associated with the specified user; false
   * otherwise
   */
  bool accessTokensAreValid(const QString userName, const QString accessToken,
                            const QString accessTokenSecret);

  /**
   * Returns the HTTP session ID associated with a user
   *
   * @param userId ID of a user
   * @return an HTTP session ID or an empty string if no session ID was found
   */
  QString getSessionIdByUserId(const long userId);

  /**
   * Returns the public OAuth access token associated with a user
   *
   * @param userId ID of a user
   * @return a public OAuth access token or an empty string if no session ID was found
   */
  QString getAccessTokenByUserId(const long userId);

  /**
   * Returns the private OAuth access token associated with a user
   *
   * @param userId ID of a user
   * @return a private OAuth access token or an empty string if no session ID was found
   */
  QString getAccessTokenSecretByUserId(const long userId);

  /**
   * Creates a user session entry
   *
   * @param userId user ID
   * @param sessionId HTTP session ID
   */
  void insertUserSession(const long userId, const QString sessionId);

  /**
   * Updates the OAuth access tokens for a user
   *
   * @param userId user ID
   * @param accessToken OAuth public access token
   * @param accessTokenSecret OAuth private access token
   */
  void updateUserAccessTokens(const long userId, const QString accessToken,
                              const QString accessTokenSecret);

  /**
   * Determines whether a user has permission to access a map
   *
   * The map must either have been created by the current user assigned to this database or
   * be a public map.
   *
   * @param mapId ID of the map to determine access privileges
   * @param write
   * @return true if the user may access the map; false otherwise
   */
  bool currentUserCanAccessMap(const long mapId, const bool write = false);

  long getCurrentUserId() const { return _currUserId; }

  /**
   * Creates a data folder record
   *
   * @param displayName folder name
   * @param parentId ID of the folder's parent
   * @param userId ID of the user creating the folder
   * @param isPublic folder visibility
   * @returns ID of the created folder
   */
  long insertFolder(const QString displayName, const long parentId, const long userId,
                    const bool isPublic);

  /**
   * Creates a mapping between a map and a data folder
   *
   * @param mapId ID of the map
   * @param folderId ID of the folder
   */
  void insertFolderMapMapping(const long mapId, const long folderId);

  /**
   * Returns the IDs of all folders mapped to a particular map
   *
   * @param mapId a map ID
   * @return collection of folder IDs
   */
  std::set<long> getFolderIdsAssociatedWithMap(const long mapId);

  /**
   * Determines a single map ID associated with a Hootenanny API database URL
   *
   * First attempts to parse the numerical map ID from the URL string, then tries to parse a map
   * name from the URL and obtain an ID from it.  Throws if multiple maps are found.
   *
   * @param url URL to parse
   * @return a map ID or -1 of no map is found
   */
  long getMapIdFromUrl(const QUrl& url);

  /**
   * Verifies that a map may be accessed by the currently configured user
   *
   * @param mapId ID of the map for which to verify access
   * @param write if true, verifies map write access for the user; if false, verifies map read
   * access for the user
   */
  void verifyCurrentUserMapUse(const long mapId, const bool write = false);

  /**
   * Determines if the currently configured user owns a map with the given name
   *
   * @param mapName map name
   * @return true if the current user owns a map with the input name; false othersie
   */
  bool currentUserHasMapWithName(const QString mapName);

  /**
   * Returns the ID of a map with given name if owned by the currently configured user
   *
   * @param name map name
   * @return ID of the map; -1 if the map is not found
   */
  long getMapIdByNameForCurrentUser(const QString name);

  /**
   * Writes an associated resource ID for a job
   *
   * @param jobId the job's ID
   * @param resourceId the resource ID
   */
  void updateJobStatusResourceId(const QString jobId, const long resourceId);

  /**
   * Inserts a job record
   *
   * @param statusDetail optional status detail text
   * @return the ID of the inserted job
   */
  QString insertJob(const QString statusDetail = "");

  /**
   * Retrieves the resource ID associated with a job
   *
   * @param jobId the job's ID
   * @return a resource ID
   */
  long getJobStatusResourceId(const QString jobId);

protected:

  virtual void _resetQueries();

private:

  friend class ServiceHootApiDbReaderTest;
  friend class ServiceHootApiDbWriterTest;

  boost::shared_ptr<QSqlQuery> _closeChangeSet;
  boost::shared_ptr<QSqlQuery> _insertChangeSet;
  boost::shared_ptr<QSqlQuery> _insertChangeSetTag;
  boost::shared_ptr<QSqlQuery> _insertMap;
  boost::shared_ptr<QSqlQuery> _insertRelationMembers;
  boost::shared_ptr<QSqlQuery> _insertWayNodes;
  boost::shared_ptr<QSqlQuery> _selectHootDbVersion;
  boost::shared_ptr<QSqlQuery> _mapExistsById;
  boost::shared_ptr<QSqlQuery> _changesetExists;
  boost::shared_ptr<QSqlQuery> _selectReserveNodeIds;
  boost::shared_ptr<QSqlQuery> _selectMapIdsForCurrentUser;
  boost::shared_ptr<QSqlQuery> _selectPublicMapIds;
  boost::shared_ptr<QSqlQuery> _selectMembersForRelation;
  boost::shared_ptr<QSqlQuery> _updateNode;
  boost::shared_ptr<QSqlQuery> _updateRelation;
  boost::shared_ptr<QSqlQuery> _updateWay;
  boost::shared_ptr<QSqlQuery> _mapExistsByName;
  boost::shared_ptr<QSqlQuery> _getMapIdByName;
  boost::shared_ptr<QSqlQuery> _insertChangeSet2;
  boost::shared_ptr<QSqlQuery> _numChangesets;
  boost::shared_ptr<QSqlQuery> _getSessionIdByUserId;
  boost::shared_ptr<QSqlQuery> _accessTokensAreValid;
  boost::shared_ptr<QSqlQuery> _getAccessTokenByUserId;
  boost::shared_ptr<QSqlQuery> _getAccessTokenSecretByUserId;
  boost::shared_ptr<QSqlQuery> _insertUserSession;
  boost::shared_ptr<QSqlQuery> _updateUserAccessTokens;
  boost::shared_ptr<QSqlQuery> _insertFolder;
  boost::shared_ptr<QSqlQuery> _insertFolderMapMapping;
  boost::shared_ptr<QSqlQuery> _folderIdsAssociatedWithMap;
  boost::shared_ptr<QSqlQuery> _deleteFolders;
  boost::shared_ptr<QSqlQuery> _selectMapIds;
  boost::shared_ptr<QSqlQuery> _getMapPermissionsById;
  boost::shared_ptr<QSqlQuery> _getMapPermissionsByName;
  boost::shared_ptr<QSqlQuery> _currentUserHasMapWithName;
  boost::shared_ptr<QSqlQuery> _getMapIdByNameForCurrentUser;
  boost::shared_ptr<QSqlQuery> _updateJobStatusResourceId;
  boost::shared_ptr<QSqlQuery> _insertJob;
  boost::shared_ptr<QSqlQuery> _deleteJobById;
  boost::shared_ptr<QSqlQuery> _getJobStatusResourceId;

  boost::shared_ptr<BulkInsert> _nodeBulkInsert;
  long _nodesPerBulkInsert;
  double _nodesInsertElapsed;
  boost::shared_ptr<BulkDelete> _nodeBulkDelete;
  long _nodesPerBulkDelete;
  double _nodesDeleteElapsed;
  boost::shared_ptr<InternalIdReserver> _nodeIdReserver;

  boost::shared_ptr<BulkInsert> _wayBulkInsert;
  long _waysPerBulkInsert;
  double _wayInsertElapsed;
  boost::shared_ptr<InternalIdReserver> _wayIdReserver;

  boost::shared_ptr<BulkInsert> _wayNodeBulkInsert;
  long _wayNodesPerBulkInsert;
  double _wayNodesInsertElapsed;

  boost::shared_ptr<BulkInsert> _relationBulkInsert;
  long _relationsPerBulkInsert;
  boost::shared_ptr<InternalIdReserver> _relationIdReserver;

  /// A vector of map ids that are pending index creation
  QVector<long> _pendingMapIndexes;

  long _lastMapId;

  long _currUserId;
  long _currMapId;
  long _currChangesetId;
  geos::geom::Envelope _changesetEnvelope;
  long _changesetChangeCount;

  unsigned long _nodesAddedToCache;
  unsigned long _nodesFlushedFromCache;

  int _precision;

  /**
   * There are some statements that cannot be executed within a transaction
   * (like DROP DATABASE). There are times (when deleting a map) where we
   * want to do such things, if the current transaction succeeds. So we build
   * this list, and after a successful commit, all of these statements will
   * be executed.
   */
  QVector<QString> _postTransactionStatements;

  bool _createIndexesOnClose;
  bool _flushOnClose;

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

  void _flushBulkInserts();
  void _flushBulkDeletes();
  long _getNextNodeId();
  long _getNextRelationId();
  long _getNextWayId();
  void _init();
  void _lazyFlushBulkInsert();

  /**
   * Executes the insert, performs error checking and returns the new ID.
   */
  long _insertRecord(QSqlQuery& query);
  void _updateChangesetEnvelope( const ConstNodePtr node );
  void _updateChangesetEnvelope(const ConstWayPtr way);
  void _updateChangesetEnvelopeWayIds(const std::vector<long>& wayIds);
  void _updateChangesetEnvelopeRelationIds(const std::vector<long>& relationIds);
  void _updateChangesetEnvelopeRelationNodes(const std::vector<long>& relationIds);
  void _updateChangesetEnvelopeRelationWays(const std::vector<long>& relationIds);

  static QString _escapeTags(const Tags& tags);

  // These delete methods are for testing purposes only, as they don't do any checks for orphaned
  // relationships.  Feel free to harden them and promote to public members that can be used beyond
  // test code, if needed.
  void _deleteFolderMapMappingsByMapId(const long mapId);
  void _deleteAllFolders(const std::set<long>& folderIds);
  void _deleteJob(const QString id);
};

}

#endif // HOOTAPIDB_H
