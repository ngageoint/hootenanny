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
#ifndef APIDB_H
#define APIDB_H

// GEOS
#include <geos/geom/Envelope.h>

// hoot
#include <hoot/core/elements/RelationData.h>
#include <hoot/core/elements/Status.h>

// Qt
#include <QUrl>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMap>

// Standard
#include <vector>
#include <map>
#include <set>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{
using namespace boost;
using namespace geos::geom;
using namespace std;

class BulkInsert;
class InternalIdReserver;
class HootApiDbWriterTest;
class Element;
class Tags;
class ElementId;
class OsmMap;
class Node;
class Way;
class Relation;
class ElementType;
class Range;
class TableType;

/**
 * This class abstracts out all SQL calls to interact with an API DB (either hoot or OSM). It also
 * strives to abstract out some of the storage details such as the conversion of lat/lng to integers
 * and the calculation of quad tiles.
 */
class ApiDb
{

public:

  static const int COORDINATE_SCALE = 1e7;

  // below are the column indexes when calling select*Elements()
  // Not all parts of the code use these consts. Please convert "magic numbers" when you find
  // them.
  static const int NODES_LATITUDE = 1;
  static const int NODES_LONGITUDE = 2;
  static const int NODES_CHANGESET = 3;
  static const int NODES_TIMESTAMP = 5;
  static const int NODES_VERSION = 7;
  static const int NODES_TAGS = 8;

  static const int WAYS_CHANGESET = 1;
  static const int WAYS_TIMESTAMP = 2;
  static const int WAYS_VERSION = 4;
  static const int WAYS_TAGS = 5;

  static const int RELATIONS_CHANGESET = 1;
  static const int RELATIONS_TIMESTAMP = 2;
  static const int RELATIONS_VERSION = 4;
  static const int RELATIONS_TAGS = 5;

  /**
   * This value should be updated after the DB is upgraded and all tests run successfully.
   */
  static QString expectedDbVersion() { return "18:dmitriy.mylov"; }
  static int maximumChangeSetEdits() { return 50000; }

  static const Status DEFAULT_ELEMENT_STATUS;

  ApiDb();

  virtual ~ApiDb();

  /**
   * Check if the url is supported.
   */
  virtual bool isSupported(const QUrl& url);

  /**
   * Open the database.
   */
  virtual void open(const QUrl& url);

  /**
   * Transact the data.
   */
  virtual void transaction() = 0;

  /**
   * Rollback the current transaction.
   */
  virtual void rollback() = 0;

  /**
   * Commit the current transaction.
   */
  virtual void commit() = 0;

  /**
   * Close the database.
   */
  virtual void close() = 0;

  /**
   * Returns a results iterator to all OSM elements for a given element type in the database.
   */
  virtual boost::shared_ptr<QSqlQuery> selectElements(const ElementType& elementType) = 0;

  /**
   * Returns a vector with all the OSM node ID's for a given way
   */
  virtual vector<long> selectNodeIdsForWay(long wayId) = 0;

  vector<long> selectNodeIdsForWay(long wayId, const QString sql);

  /**
   * Returns a query results with node_id, lat, and long with all the OSM node ID's for a given way
   */
  virtual boost::shared_ptr<QSqlQuery> selectNodesForWay(long wayId) = 0;

  boost::shared_ptr<QSqlQuery> selectNodesForWay(long wayId, const QString sql);

  /**
   * Returns a vector with all the relation members for a given relation
   */
  virtual vector<RelationData::Entry> selectMembersForRelation(long relationId) = 0;

  /**
   * Returns the user ID if the email is found. If throwWhenMissing is false then -1 is returned
   * if the user doesn't exist.
   */
  virtual long getUserId(const QString email, bool throwWhenMissing);

  /**
   * Inserts a user.
   */
  virtual long insertUser(const QString email, const QString displayName);

  /**
   * Deletes a user.
   */
  virtual void deleteUser(long userId) = 0;

  /**
   * Given a QVariant (string), unscape the tags into a full tag map.
   */
  static Tags unescapeTags(const QVariant& v);

  /**
   * Returns database
   */
  QSqlDatabase& getDB() { return _db; }

  static long round(double x);

  /**
   * Calculates the QuadTile for a point.
   * http://wiki.openstreetmap.org/wiki/QuadTiles
   *
   * The code was taken from quad_tile.h and quad_tile.c in the openstreetmap-website
   * https://github.com/openstreetmap/openstreetmap-website
   */
  static unsigned int tileForPoint(double lat, double lon);

  /**
   * Returns all nodes that fall within a geospatial bounds
   *
   * @param bounds the query bounds
   * @return a SQL results iterator
   */
  boost::shared_ptr<QSqlQuery> selectNodesByBounds(const Envelope& bounds);

  /**
   * Returns the IDs of all ways that own the input node IDs
   *
   * @param nodeIds a collection of node IDs
   * @return a SQL results iterator
   */
  boost::shared_ptr<QSqlQuery> selectWayIdsByWayNodeIds(const QSet<QString>& nodeIds);

  /**
   * Returns all elements by type with IDs in the input ID list
   *
   * @param elementIds a collection of element IDs
   * @param tableType the type of database table to query
   * @return a SQL results iterator
   */
  boost::shared_ptr<QSqlQuery> selectElementsByElementIdList(const QSet<QString>& elementIds,
                                                      const TableType& tableType);

  /**
   * Returns all the IDs of all nodes owned by the input way IDs
   *
   * @param wayIds a collection of way IDs
   * @return a SQL results iterator
   */
  boost::shared_ptr<QSqlQuery> selectWayNodeIdsByWayIds(const QSet<QString>& wayIds);

  /**
   * Returns the IDs of all relations which own the typed input member IDs
   *
   * @param memberIds a collection of member IDs of the same element type
   * @param memberElementType the element type of the associated relation member
   * @return a SQL results iterator
   */
  boost::shared_ptr<QSqlQuery> selectRelationIdsByMemberIds(const QSet<QString>& memberIds,
                                                     const ElementType& memberElementType);

  virtual QString tableTypeToTableName(const TableType& tableType) const = 0;

  /**
   * Returns all changesets created after the specified time.
   *
   * @param timeStr time string for which to search for changesets created after; should be of the
   * format specified by the TIME_FORMAT constant
   * @return a SQL results iterator
   */
  boost::shared_ptr<QSqlQuery> getChangesetsCreatedAfterTime(const QString timeStr);

  /**
   * Gets the next sequence ID for the given element type
   *
   * @param elementType element type
   * @return an element ID
   */
  virtual long getNextId(const ElementType& elementType) = 0;

  QSqlError getLastError() const { return _db.lastError(); }

  inline static QString getChangesetsTableName()                { return "changesets"; }
  inline static QString getChangesetsSubscribersTableName()     { return "changesets_subscribers"; }
  inline static QString getChangesetTagsTableName()             { return "changeset_tags"; }
  inline static QString getCurrentNodeTagsTableName()           { return "current_node_tags"; }
  inline static QString getCurrentNodesTableName()              { return "current_nodes"; }
  inline static QString getCurrentRelationMembersTableName()    { return "current_relation_members"; }
  inline static QString getCurrentRelationTagsTableName()       { return "current_relation_tags"; }
  inline static QString getCurrentRelationsTableName()          { return "current_relations"; }
  inline static QString getCurrentWayNodesTableName()           { return "current_way_nodes"; }
  inline static QString getCurrentWayTagsTableName()            { return "current_way_tags"; }
  inline static QString getCurrentWaysTableName()               { return "current_ways"; }

  inline static QString getNodeTagsTableName()                  { return "node_tags"; }
  inline static QString getNodesTableName()                     { return "nodes"; }
  inline static QString getRelationMembersTableName()           { return "relation_members"; }
  inline static QString getRelationTagsTableName()              { return "relation_tags"; }
  inline static QString getRelationsTableName()                 { return "relations"; }
  inline static QString getWayNodesTableName()                  { return "way_nodes"; }
  inline static QString getWayTagsTableName()                   { return "way_tags"; }
  inline static QString getWaysTableName()                      { return "ways"; }

  inline static QString getMapsTableName()                      { return "maps"; }
  inline static QString getUsersTableName()                     { return "users"; }

  inline static QString getSequenceId()                         { return "_id_seq"; }

  inline static QString getChangesetsSequenceName()             { return getChangesetsTableName() + getSequenceId(); }
  inline static QString getCurrentNodesSequenceName()           { return getCurrentNodesTableName() + getSequenceId(); }
  inline static QString getCurrentRelationMembersSequenceName() { return getCurrentRelationMembersTableName() + getSequenceId(); }
  inline static QString getCurrentRelationsSequenceName()       { return getCurrentRelationsTableName() + getSequenceId(); }
  inline static QString getCurrentWayNodesSequenceName()        { return getCurrentWayNodesTableName() + getSequenceId(); }
  inline static QString getCurrentWaysSequenceName()            { return getCurrentWaysTableName() + getSequenceId(); }

  inline static QString getMapsSequenceName()                   { return getMapsTableName() + getSequenceId(); }
  inline static QString getUsersSequenceName()                  { return getUsersTableName() + getSequenceId(); }

  static QMap<QString, QString> getDbUrlParts(const QString url);
  static QString getPsqlString(const QString url);

protected:

  //osm api db stores coords as integers and hoot api db as floating point
  bool _floatingPointCoords;
  //osm api db expects the relation member type to have the first capitalized, while the hoot
  //api db expects the value to be all lower case; by default the value is always passed around with
  //the first letter capitalized, since its taken from ElementType::toString()
  bool _capitalizeRelationMemberType;

  QSqlDatabase _db;
  boost::shared_ptr<QSqlQuery> _selectUserByEmail;
  boost::shared_ptr<QSqlQuery> _insertUser;
  boost::shared_ptr<QSqlQuery> _selectNodeIdsForWay;

  QSqlQuery _exec(const QString sql, QVariant v1 = QVariant(), QVariant v2 = QVariant(),
                  QVariant v3 = QVariant()) const;

  static void _unescapeString(QString& s);

  virtual void _resetQueries();

private:

  //element bounds related queries
  boost::shared_ptr<QSqlQuery> _selectNodesByBounds;
  boost::shared_ptr<QSqlQuery> _selectWayIdsByWayNodeIds;
  boost::shared_ptr<QSqlQuery> _selectElementsByElementIdList;
  boost::shared_ptr<QSqlQuery> _selectWayNodeIdsByWayIds;
  boost::shared_ptr<QSqlQuery> _selectRelationIdsByMemberIds;

  boost::shared_ptr<QSqlQuery> _selectChangesetsCreatedAfterTime;

  QString _getTileWhereCondition(const vector<Range>& tileIdRanges) const;
  vector<Range> _getTileRanges(const Envelope& env) const;
};

}

#endif // APIDB_H
