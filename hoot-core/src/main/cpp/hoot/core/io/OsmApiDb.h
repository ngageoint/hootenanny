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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef OSMAPIDB_H
#define OSMAPIDB_H

// Hoot
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>

// Qt
#include <QFile>

namespace hoot
{

/**
 * Used for interaction with an OSM API database (official public OSM database).
 */
class OsmApiDb : public ApiDb
{

public:

  static QString className() { return "hoot::OsmApiDb"; }

  static int logWarnCount;

  static const QString TIME_FORMAT;
  static const QString TIMESTAMP_FUNCTION;

  OsmApiDb();

  ~OsmApiDb();

  void close() override;

  bool isSupported(const QUrl& url) override;

  void open(const QUrl& url) override;

  void commit() override;

  void deleteUser(long userId) override;

  /**
   * Returns a vector with all the OSM node ID's for a given way
   */
  std::vector<long> selectNodeIdsForWay(long wayId) override;

  /**
   * Returns a query results with node_id, lat, and long with all the OSM node ID's for a given way
   */
  std::shared_ptr<QSqlQuery> selectNodesForWay(long wayId) override;

  /**
   * Returns a vector with all the relation members for a given relation
   */
  std::vector<RelationData::Entry> selectMembersForRelation(long relationId) override;

  /**
    * Deletes data in the Osm Api db
    */
  void deleteData();

  /**
   * Purpose: to extract tags from the extra lines returned in the
   *   selectAll for OsmApi data
   * Input: apidb row in form with row[8]=k, row[9]=v
   * Output: "k"=>"v"
   * Note: this gets the tags in a form that is the same as how selectAll
   *       returns them for Services DB
   *
   * @param row
   * @param Type
   * @return
   */
  QString extractTagFromRow(const std::shared_ptr<QSqlQuery>& row, ElementType::Type Type);

  std::shared_ptr<QSqlQuery> selectTagsForNode(long nodeId);

  std::shared_ptr<QSqlQuery> selectTagsForWay(long wayId);

  std::shared_ptr<QSqlQuery> selectTagsForRelation(long wayId);

  /**
   * Increment the sequence ID for the given sequence and return it
   *
   * @param tableName element type associated with the sequence
   * @return the next sequence ID for the given type
   */
  long getNextId(const ElementType& elementType) override;

  /**
   * Increment the sequence ID for the given sequence and return it
   *
   * @param tableName database table name associated with the sequence
   * @return the next sequence ID for the given type
   * @todo need to make use of sequence and table strings more consistently here
   */
  long getNextId(const QString& tableName);

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

  // the three table name methods below may be able to be combined somehow

  /**
   * @see ApiDb::elementTypeToElementTableName
   */
  QString elementTypeToElementTableName(const ElementType& elementType) const override;

  /**
   * Converts a table type to a OSM API database table name
   *
   * @param tableType table type enum to convert
   * @return a database table name string
   */
  QString tableTypeToTableName(const TableType& tableType) const override;

  /**
   * Returns an OSM API database table name
   *
   * @param elementType type of the element
   * @param historical whether the requested table is current or historical
   * @param tags if true; returns the corresponding element tag table name
   * @return a database table name string
   */
  static QString elementTypeToElementTableName(const ElementType& elementType,
                                               const bool historical, const bool tags);

  /**
   * Disables all OSM table constraints
   */
  void disableConstraints();

  /**
   * Enables all OSM table constraints
   */
  void enableConstraints();

  /**
   * Drops all OSM table indexes
   */
  void dropIndexes();

  /**
   * Creates all OSM table indexes
   */
  void createIndexes();

protected:

  void _resetQueries() override;

private:

  std::shared_ptr<QSqlQuery> _selectTagsForNode;
  std::shared_ptr<QSqlQuery> _selectTagsForWay;
  std::shared_ptr<QSqlQuery> _selectTagsForRelation;
  std::shared_ptr<QSqlQuery> _selectMembersForRelation;

  QHash<QString, std::shared_ptr<QSqlQuery>> _seqQueries;

  void _init();

  long _getIdFromSequence(const ElementType& elementType, const QString& sequenceType);
  long _getIdFromSequence(const QString& tableName, const QString& sequenceType);

  static QStringList _getTables();
  void _modifyConstraints(const QStringList& tableNames, bool disable);
};

}

#endif // OSMAPIDB_H
