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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SERVICESDB_H
#define SERVICESDB_H

// GEOS
#include <geos/geom/Envelope.h>

// hoot
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Relation.h>

// Qt
#include <QUrl>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

// Standard
#include <vector>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{
using namespace boost;
using namespace geos::geom;
using namespace std;

class BulkInsert;
class InternalIdReserver;
class ServicesDbWriterTest;
class Element;
class Tags;
class ElementId;
class OsmMap;
class Node;
class Way;
class Relation;

/**
 * This class abstracts out all SQL calls to interact with the services DB. It also strives to
 * abstract out some of the storage details such as the conversion of lat/lng to integers and the
 * calculation of quad tiles.
 *
 * This class is most efficient if it is used for on map ID repeatedly and then changed to a new
 * map ID. If you're going to be reading from one map and writing to another it is probably best
 * to have two instances of this class. See _checkLastMapId for some details.
 */
class ServicesDb
{
public:

  /**
   * Describes what type of Postgres the object is connected to
   *
   * @note The type is set automatically in ::open by determining by
   *    what tables are present in the schema
   */
  enum DbType
  {
    DBTYPE_UNSUPPORTED = 0,
    DBTYPE_SERVICES,
    DBTYPE_OSMAPI
  };

  static const int COORDINATE_SCALE = 1e7;
  // below are the column indexes when calling select*Elements()
  // Not all parts of the code use these consts. Please convert "magic numbers" when you find
  // them.
  static const int NODES_LATITUDE = 1;
  static const int NODES_LONGITUDE = 2;
  static const int NODES_TAGS = 8;
  static const int RELATIONS_TAGS = 5;
  static const int WAYS_TAGS = 5;


  /**
   * This value should be updated after the DB is upgraded and all tests run successfully.
   */
  static QString expectedDbVersion() { return "13:josh.sisskind"; }
  static int maximumChangeSetEdits() { return 50000; }

  static const Status DEFAULT_ELEMENT_STATUS;
  static const Meters DEFAULT_ELEMENT_CIRCULAR_ERROR = 0.0;

  ServicesDb();

  virtual ~ServicesDb();

  /**
   * Called after open. This will read the bounds of the specified layer in a relatively efficient
   * manner. (e.g. SELECT min(x)...)
   */
  virtual Envelope calculateEnvelope() const;

  void close();

  void commit();

  QString getDbVersion();

  QUrl getDefaultUrl();

  bool isCorrectDbVersion() { return getDbVersion() == expectedDbVersion(); }

  bool isSupported(QUrl url);

  bool isSupported(const QUrl& url, const DbType dbType);

  void open(QUrl url);

  void transaction();

  //reading

  /**
   * Returns the user ID if the email is found. If throwWhenMissing is false then -1 is returned
   * if the user doesn't exist.
   */
  long getUserId(QString email, bool throwWhenMissing = true);

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

  /**
   * Returns a results iterator to all OSM elements for a given map and element type in the services
   * database
   */
  shared_ptr<QSqlQuery> selectAllElements(const ElementType& elementType);


  shared_ptr<QSqlQuery> selectAllElements(const long elementId, const ElementType& elementType);

  /**
   * Returns a results iterator to all OSM elements for a given map and element type in the services
   * database.  If limit = 0, no limit will be placed on the number of elements returned.  If offset
   * = 0, no records will be skipped in the returned result set.
   */
  shared_ptr<QSqlQuery> selectElements(const long elementId, const ElementType& elementType,
                                       const long limit, const long offset);

  /**
   * Returns a vector with all the OSM node ID's for a given way
   */
  vector<long> selectNodeIdsForWay(long wayId);

  /**
   * Returns a vector with all the relation members for a given relation
   */
  vector<RelationData::Entry> selectMembersForRelation(long relationId);

  //writing

  void closeChangeSet(long changeSetId, Envelope env, int numChanges);

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

  void deleteUser(long userId);

  long insertChangeSet(const Tags& tags = Tags(), Envelope env = Envelope());

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

  long insertNode(long id, double lat, double lon, long changeSetId,
    const Tags &tags, bool createNewId = false);

  long insertRelation(long relationId, long changeSetId, const Tags& tags,
    bool createNewId = false);

  void insertRelationMembers(long relationId, ElementType type, long elementId,
    QString role, int sequenceId);

  void insertRelationTag(long relationId, const QString& k, const QString& v);

  long insertUser(QString email, QString displayName);

  long insertWay(long wayId, long changeSetId, const Tags& tags,
                 /*const vector<long>& nids,*/ bool createNewId = false);

  void insertWayNodes(long wayId, const vector<long>& nodeIds);

  /**
   * Rollback the current transaction.
   */
  void rollback();

  set<long> selectMapIds(QString name);

  /**
   * Given a QVariant (string), unscape the tags into a full tag map.
   */
  static Tags unescapeTags(const QVariant& v);

  void updateNode(long id, double lat, double lon, long changeSetId, const Tags& tags);

  void updateRelation(long id, long changeSetId, const Tags& tags);

  void updateWay(long id, long changeSetId, const Tags& tags);

  DbType getDatabaseType() const { return _connectionType; }

private:

  QSqlDatabase _db;
  bool _inTransaction;
  shared_ptr<QSqlQuery> _closeChangeSet;
  shared_ptr<QSqlQuery> _insertChangeSet;
  shared_ptr<QSqlQuery> _insertChangeSetTag;
  shared_ptr<QSqlQuery> _insertMap;
  shared_ptr<QSqlQuery> _insertRelationMembers;
  shared_ptr<QSqlQuery> _insertUser;
  shared_ptr<QSqlQuery> _insertWayNodes;
  shared_ptr<QSqlQuery> _selectDbVersion;
  shared_ptr<QSqlQuery> _selectUserByEmail;
  shared_ptr<QSqlQuery> _mapExists;
  shared_ptr<QSqlQuery> _changesetExists;
  shared_ptr<QSqlQuery> _numTypeElementsForMap;
  shared_ptr<QSqlQuery> _selectReserveNodeIds;
  shared_ptr<QSqlQuery> _selectElementsForMap;
  shared_ptr<QSqlQuery> _selectNodeIdsForWay;
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
  DbType _connectionType;

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

  /**
   * Drops the specified table and cascades (removes depedants). No warning or error will be given
   * if the table does not exist.
   *
   * No validation is done on the table names. In other words, don't pass in user provided strings.
   *
   * @param tableName
   */
  void _dropTable(QString tableName);

  QString _elementTypeToElementTableName(long mapId, const ElementType& elementType) const;

  friend class ServicesDbTest;
  friend class ServicesDbTestUtils;
  friend class ServicesDbReaderTest;
  friend class ServicesDbWriterTest;

  QString _escapeIds(const vector<long>& v) const;

  QString _escapeTags(const Tags& tags) const;

  QSqlQuery _exec(QString sql, QVariant v1 = QVariant(), QVariant v2 = QVariant(),
                  QVariant v3 = QVariant()) const;

  /**
   * @brief Executes the provided SQL statement without calling prepare. This is handy when creating
   * constraints, tables, etc.
   * @param sql SQL to execute.
   */
  QSqlQuery _execNoPrepare(QString sql) const;

  /**
   * Very handy for testing.
   */
  QString _execToString(QString sql, QVariant v1 = QVariant(), QVariant v2 = QVariant(),
                        QVariant v3 = QVariant());

  void _flushBulkInserts();

  static QString _getNodeSequenceName(long mapId)
  { return "current_nodes" + _getMapIdString(mapId) + "_id_seq"; }
  static QString _getRelationSequenceName(long mapId)
  { return "current_relations" + _getMapIdString(mapId) + "_id_seq"; }
  static QString _getWaySequenceName(long mapId)
  { return "current_ways" + _getMapIdString(mapId) + "_id_seq"; }

  static QString _getChangesetsTableName(long mapId)
  { return "changesets" + _getMapIdString(mapId); }
  static QString _getNodesTableName(long mapId)
  { return "current_nodes" + _getMapIdString(mapId); }
  static QString _getRelationMembersTableName(long mapId)
  { return "current_relation_members" + _getMapIdString(mapId); }
  static QString _getRelationsTableName(long mapId)
  { return "current_relations" + _getMapIdString(mapId); }
  static QString _getWayNodesTableName(long mapId)
  { return "current_way_nodes" + _getMapIdString(mapId); }
  static QString _getWaysTableName(long mapId)
  { return "current_ways" + _getMapIdString(mapId); }

  /**
   * Returns a map ID string suitable for using in table names. E.g. _1
   */
  static QString _getMapIdString(long id) { return QString("_%1").arg(id); }

  long _getNextNodeId(long mapId);
  long _getNextRelationId(long mapId);
  long _getNextWayId(long mapId);

  bool _hasTable(QString tableName);

  void _init();

  void _lazyFlushBulkInsert();

  void _resetQueries();

  /**
   * Executes the insert, performs error checking and returns the new ID.
   */
  long _insertRecord(QSqlQuery& query);

  /**
   * Calculates the QuadTile for a point.
   * http://wiki.openstreetmap.org/wiki/QuadTiles
   *
   * The code was taken from quad_tile.h and quad_tile.c in the openstreetmap-website
   * https://github.com/openstreetmap/openstreetmap-website
   */
  unsigned int _tileForPoint(double lat, double lon);

  long _round(double x);
  long _round(double x, int precision);

  static void _unescapeString(QString& s);

  /**
   * Examine the schema to find out by what tables exist what the supported DB type
   *    is, if any.
   *
   * @return One of the supported database types or DBTYPE_UNSUPPORTED
   */
  DbType _determineDbType();
};

}

#endif // SERVICESDB_H

