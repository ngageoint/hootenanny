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
#ifndef APIDB_H
#define APIDB_H

// GEOS
#include <geos/geom/Envelope.h>

// hoot
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/io/ElementCache.h>

// Qt
#include <QUrl>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

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

/**
 * This class abstracts out all SQL calls to interact with the services DB. It also strives to
 * abstract out some of the storage details such as the conversion of lat/lng to integers and the
 * calculation of quad tiles.
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
  static QString expectedDbVersion() { return "17:jason.surratt"; }
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
  virtual shared_ptr<QSqlQuery> selectElements(const ElementType& elementType) = 0;

  /**
   * Returns a vector with all the OSM node ID's for a given way
   */
  virtual vector<long> selectNodeIdsForWay(long wayId) = 0;

  vector<long> selectNodeIdsForWay(long wayId, const QString& sql);

  /**
   * Returns a query results with node_id, lat, and long with all the OSM node ID's for a given way
   */
  virtual shared_ptr<QSqlQuery> selectNodesForWay(long wayId) = 0;

  shared_ptr<QSqlQuery> selectNodesForWay(long wayId, const QString& sql);

  /**
   * Returns a vector with all the relation members for a given relation
   */
  virtual vector<RelationData::Entry> selectMembersForRelation(long relationId) = 0;

  /**
   * Returns the user ID if the email is found. If throwWhenMissing is false then -1 is returned
   * if the user doesn't exist.
   */
  virtual long getUserId(const QString& email, bool throwWhenMissing);

  /**
   * Inserts a user.
   */
  virtual long insertUser(const QString& email, const QString& displayName);

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
  QSqlDatabase getDB() { return _db; }

  static long round(double x);

  /**
   * Calculates the QuadTile for a point.
   * http://wiki.openstreetmap.org/wiki/QuadTiles
   *
   * The code was taken from quad_tile.h and quad_tile.c in the openstreetmap-website
   * https://github.com/openstreetmap/openstreetmap-website
   */
  static unsigned int tileForPoint(double lat, double lon);

protected:

  virtual QSqlQuery _exec(const QString& sql, QVariant v1 = QVariant(), QVariant v2 = QVariant(),
                  QVariant v3 = QVariant()) const;

  /**
   * @brief Executes the provided SQL statement without calling prepare. This is handy when creating
   * constraints, tables, etc.
   * @param sql SQL to execute.
   */
  virtual QSqlQuery _execNoPrepare(const QString& sql) const;

  static void _unescapeString(QString& s);

  QSqlDatabase _db;
  shared_ptr<QSqlQuery> _selectUserByEmail;
  shared_ptr<QSqlQuery> _insertUser;
  shared_ptr<QSqlQuery> _selectNodeIdsForWay;
};

}

#endif // APIDB_H
