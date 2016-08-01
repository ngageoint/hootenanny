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
#ifndef OSMAPIDB_H
#define OSMAPIDB_H

// Hoot
#include <hoot/core/io/ApiDb.h>

// Qt
#include <QFile>


namespace hoot
{

class OsmApiDb : public ApiDb
{

public:

  static const QString TIME_FORMAT;
  static const QString TIMESTAMP_FUNCTION;

  OsmApiDb();

  virtual ~OsmApiDb();

  virtual void close();

  virtual bool isSupported(QUrl url);

  virtual void open(const QUrl& url);

  virtual void transaction();

  virtual void rollback();

  virtual void commit();

  virtual void deleteUser(long userId);

  /**
   * Returns a results iterator to all OSM elements for a given element type in the database.
   */
  virtual shared_ptr<QSqlQuery> selectElements(const ElementType& elementType);

  /**
   * Returns a vector with all the OSM node ID's for a given way
   */
  virtual vector<long> selectNodeIdsForWay(long wayId);

  /**
   * Returns a query results with node_id, lat, and long with all the OSM node ID's for a given way
   */
  virtual shared_ptr<QSqlQuery> selectNodesForWay(long wayId);

  /**
   * Returns a vector with all the relation members for a given relation
   */
  virtual vector<RelationData::Entry> selectMembersForRelation(long relationId);

  /**
   * Returns a results iterator to all OSM elements for a given bbox.
   */
  shared_ptr<QSqlQuery> selectBoundedElements(const long elementId, const ElementType& elementType, const QString& bbox);

  /**
   * Returns a results iterator to a node for a given node id.
   */
  shared_ptr<QSqlQuery> selectNodeById(const long elementId);

  /**
    * Deletes data in the Osm Api db
    */
  void deleteData();

  QString extractTagFromRow(shared_ptr<QSqlQuery> row, const ElementType::Type Type);

  shared_ptr<QSqlQuery> selectTagsForWay(long wayId);

  shared_ptr<QSqlQuery> selectTagsForRelation(long wayId);

  /**
   * Gets the next sequence ID for the given element type
   *
   * @param type element type
   * @return an element ID
   */
  long getNextId(const ElementType type);

  /**
   * Gets the next sequence ID for the given database table
   *
   * @param tableName database table name
   * @return an element ID
   */
  long getNextId(const QString tableName);

  /**
   * Returns all changesets created after the specified time.
   *
   * @param timeStr time string for which to search for changesets created after; should be of the
   * format specified by the TIME_FORMAT constant
   * @return a SQL results iterator
   */
  shared_ptr<QSqlQuery> getChangesetsCreatedAfterTime(const QString timeStr);

  /**
   * Converts a node coordinate from how its stored in a Hootenanny API database (0.01 nanodegrees
   * as a double precision floating point number) to how its stored in an OSM API database
   * (100 nanodegrees as a 64 bit integer)
   *
   * @param x the coordinate to convert
   * @return a converted coordinate
   */
  static long toOsmApiDbCoord(const double x);

  /**
   * Converts a node coordinate from how its stored in an OSMAPI database (100 nanodegrees
   * as a 64 bit integer) to how its stored in an OSM API database (0.01 nanodegrees
   * as a double precision floating point number)
   *
   * @param x the coordinate to convert
   * @return a converted coordinate
   */
  static double fromOsmApiDbCoord(const long x);

private:

  bool _inTransaction;

  shared_ptr<QSqlQuery> _selectElementsForMap;
  shared_ptr<QSqlQuery> _selectTagsForWay;
  shared_ptr<QSqlQuery> _selectTagsForRelation;
  shared_ptr<QSqlQuery> _selectMembersForRelation;
  shared_ptr<QSqlQuery> _selectNodeById;
  shared_ptr<QSqlQuery> _selectChangesetsCreatedAfterTime;

  QHash<QString, shared_ptr<QSqlQuery> > _seqQueries;

  void _resetQueries();

  void _init();

  QString _elementTypeToElementTableName(const ElementType& elementType) const;

  // Osm Api DB table strings
  static QString _getWayNodesTableName() { return "current_way_nodes"; }
  static QString _getRelationMembersTableName() { return "current_relation_members"; }

};

}

#endif // OSMAPIDB_H
