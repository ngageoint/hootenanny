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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "OsmApiDb.h"

// hoot
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/InternalIdReserver.h>
#include <hoot/core/io/SqlBulkInsert.h>
#include <hoot/core/io/TableType.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/DbUtils.h>

// Qt
#include <QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QTextStream>

// Standard
#include <math.h>
#include <cmath>
#include <fstream>

// tgs
#include <tgs/System/Time.h>

using namespace std;

namespace hoot
{

int OsmApiDb::logWarnCount = 0;

const QString OsmApiDb::TIME_FORMAT = "yyyy-MM-dd HH:mm:ss.zzz";
const QString OsmApiDb::TIMESTAMP_FUNCTION = "(now() at time zone 'utc')";

OsmApiDb::OsmApiDb()
{
  _init();
}

OsmApiDb::~OsmApiDb()
{
  close();
}

void OsmApiDb::_init()
{
  _floatingPointCoords = false;
  _capitalizeRelationMemberType = true;
  _inTransaction = false;
  _resetQueries();
}

void OsmApiDb::close()
{
  LOG_TRACE("Closing database connection...");

  _resetQueries();

  if (_inTransaction)
  {
    LOG_WARN("Closing database before transaction is committed. Rolling back transaction.");
    rollback();
  }

  // Seeing this? "Unable to free statement: connection pointer is NULL"
  // Make sure all queries are listed in _resetQueries.
  _db.close();
}

void OsmApiDb::deleteData() const
{
  LOG_TRACE("Deleting all data...");

  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getCurrentRelationMembersTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getCurrentRelationTagsTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getCurrentRelationsTableName() + " RESTART IDENTITY CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getRelationMembersTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getRelationTagsTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getRelationsTableName() + " CASCADE");

  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getCurrentWayNodesTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getCurrentWayTagsTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getCurrentWaysTableName() + " RESTART IDENTITY CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getWayNodesTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getWayTagsTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getWaysTableName() + " CASCADE");

  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getCurrentNodeTagsTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getCurrentNodesTableName() + " RESTART IDENTITY CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getNodeTagsTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getNodesTableName() + " CASCADE");

  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getChangesetsSubscribersTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getChangesetTagsTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getChangesetsTableName() + " RESTART IDENTITY CASCADE");

  // delete users
  DbUtils::execNoPrepare(_db, "TRUNCATE TABLE " + ApiDb::getUsersTableName() + " RESTART IDENTITY CASCADE");
}

bool OsmApiDb::isSupported(const QUrl& url) const
{
  bool valid = ApiDb::isSupported(url);

  //postgresql is deprecated but still supported
  if (url.scheme() != MetadataTags::OsmApiDbScheme() && url.scheme() != "postgresql")
    valid = false;

  if (valid)
  {
    QString path = url.path();
    QStringList plist = path.split("/");

    // OSM API will have plist.size of 2.
    if ( plist.size() != 2 )
      valid = false;
  }

  return valid;
}

void OsmApiDb::open(const QUrl& url)
{
  LOG_DEBUG("Opening database connection: " << url.toString(QUrl::RemoveUserInfo) << "...");
  if (!isSupported(url))
    throw HootException(QString("An unsupported URL was passed into OsmApiDb: %1").arg(url.toString(QUrl::RemoveUserInfo)));
  ApiDb::open(url);
}

void OsmApiDb::deleteUser(long userId)
{
  LOG_TRACE("Deleting user: " << userId << "...");
  _exec(QString("DELETE FROM %1 WHERE id=:id").arg(ApiDb::getUsersTableName()), (qlonglong)userId);
}

void OsmApiDb::_resetQueries()
{
  LOG_TRACE("Resetting queries...");

  ApiDb::_resetQueries();

  _selectTagsForNode.reset();
  _selectTagsForWay.reset();
  _selectTagsForRelation.reset();
  _selectNodeIdsForWay.reset();
  _selectMembersForRelation.reset();
  for (auto itr = _seqQueries.begin(); itr != _seqQueries.end(); ++itr)
    itr.value().reset();
}

void OsmApiDb::commit()
{
  LOG_TRACE("Committing transaction...");

  if ( _db.isOpen() == false )
    throw HootException("Tried to commit a transaction on a closed database");

  if ( _inTransaction == false )
    throw HootException("Tried to commit but weren't in a transaction");

  _resetQueries();
  if (!_db.commit())
    throw HootException("Error committing transaction: " + _db.lastError().text());

  _inTransaction = false;
}

QString OsmApiDb::tableTypeToTableName(const TableType& tableType) const
{
  switch(tableType.getEnum())
  {
  case TableType::Node:             return ApiDb::getCurrentNodesTableName();
  case TableType::Way:              return ApiDb::getCurrentWaysTableName();
  case TableType::Relation:         return ApiDb::getCurrentRelationsTableName();
  case TableType::WayNode:          return ApiDb::getCurrentWayNodesTableName();
  case TableType::RelationMember:   return ApiDb::getCurrentRelationMembersTableName();
  default:                          throw HootException("Unsupported table type.");
  }
}

QString OsmApiDb::elementTypeToElementTableName(const ElementType& elementType,
                                                const bool historical, const bool tags)
{
  switch (elementType.getEnum())
  {
  case ElementType::Node:
    if (historical)
    {
      if (tags)
        return ApiDb::getNodeTagsTableName();
      else
        return ApiDb::getNodesTableName();
    }
    else
    {
      if (tags)
        return ApiDb::getCurrentNodeTagsTableName();
      else
        return ApiDb::getCurrentNodesTableName();
    }
    break;
  case ElementType::Way:
    if (historical)
    {
      if (tags)
        return ApiDb::getWayTagsTableName();
      else
        return ApiDb::getWaysTableName();
    }
    else
    {
      if (tags)
        return ApiDb::getCurrentWayTagsTableName();
      else
        return ApiDb::getCurrentWaysTableName();
    }
    break;
  case ElementType::Relation:
    if (historical)
    {
      if (tags)
        return ApiDb::getRelationTagsTableName();
      else
        return ApiDb::getRelationsTableName();
    }
    else
    {
      if (tags)
        return ApiDb::getCurrentRelationTagsTableName();
      else
        return ApiDb::getCurrentRelationsTableName();
    }
    break;
  default:
    throw HootException("Unknown element type");
  }
}
vector<long> OsmApiDb::selectNodeIdsForWay(long wayId)
{
  QString sql =  "SELECT node_id FROM " +
                  getCurrentWayNodesTableName() +
                 " WHERE way_id = :wayId ORDER BY sequence_id";
  return _selectNodeIdsForWaySql(wayId, sql);
}

vector<RelationData::Entry> OsmApiDb::selectMembersForRelation(long relationId)
{
  if (!_selectMembersForRelation)
  {
    _selectMembersForRelation = std::make_shared<QSqlQuery>(_db);
    _selectMembersForRelation->setForwardOnly(true);
    _selectMembersForRelation->prepare(
      "SELECT member_type, member_id, member_role FROM " + getCurrentRelationMembersTableName() +
      " WHERE relation_id = :relationId ORDER BY sequence_id");
  }
  _selectMembersForRelation->bindValue(":relationId", (qlonglong)relationId);

  return ApiDb::_selectRelationMembers(_selectMembersForRelation);
}

QString OsmApiDb::elementTypeToElementTableName(const ElementType& elementType) const
{
  return elementTypeToElementTableName(elementType, false, false);
}

std::shared_ptr<QSqlQuery> OsmApiDb::selectTagsForRelation(long relId)
{
  if (!_selectTagsForRelation)
  {
    _selectTagsForRelation = std::make_shared<QSqlQuery>(_db);
    _selectTagsForRelation->setForwardOnly(true);
    QString sql = "SELECT relation_id, k, v FROM " + ApiDb::getCurrentRelationTagsTableName() +
                  " WHERE relation_id = :relId";
    _selectTagsForRelation->prepare( sql );
  }

  _selectTagsForRelation->bindValue(":relId", (qlonglong)relId);
  if (_selectTagsForRelation->exec() == false)
  {
    throw HootException("Error selecting tags for relation with ID: " + QString::number(relId) +
      " Error: " + _selectTagsForRelation->lastError().text());
  }
  LOG_VART(_selectTagsForRelation->executedQuery());
  LOG_VART(_selectTagsForRelation->numRowsAffected());

  return _selectTagsForRelation;
}

std::shared_ptr<QSqlQuery> OsmApiDb::selectTagsForWay(long wayId)
{
  if (!_selectTagsForWay)
  {
    _selectTagsForWay = std::make_shared<QSqlQuery>(_db);
    _selectTagsForWay->setForwardOnly(true);
    QString sql =  "SELECT way_id, k, v FROM " + ApiDb::getCurrentWayTagsTableName() + " WHERE way_id = :wayId";
    _selectTagsForWay->prepare( sql );
  }

  _selectTagsForWay->bindValue(":wayId", (qlonglong)wayId);
  if (_selectTagsForWay->exec() == false)
  {
    throw HootException("Error selecting tags for way with ID: " + QString::number(wayId) +
      " Error: " + _selectTagsForWay->lastError().text());
  }
  LOG_VART(_selectTagsForWay->executedQuery());
  LOG_VART(_selectTagsForWay->numRowsAffected());

  return _selectTagsForWay;
}

std::shared_ptr<QSqlQuery> OsmApiDb::selectTagsForNode(long nodeId)
{
  if (!_selectTagsForNode)
  {
    _selectTagsForNode = std::make_shared<QSqlQuery>(_db);
    _selectTagsForNode->setForwardOnly(true);
    QString sql = "SELECT node_id, k, v FROM " + ApiDb::getCurrentNodeTagsTableName() + " WHERE node_id = :nodeId";
    _selectTagsForNode->prepare( sql );
  }

  _selectTagsForNode->bindValue(":nodeId", (qlonglong)nodeId);
  if (_selectTagsForNode->exec() == false)
  {
    throw HootException("Error selecting tags for node with ID: " + QString::number(nodeId) +
      " Error: " + _selectTagsForNode->lastError().text());
  }
  LOG_VART(_selectTagsForNode->executedQuery());
  LOG_VART(_selectTagsForNode->numRowsAffected());

  return _selectTagsForNode;
}

long OsmApiDb::getNextId(const ElementType& elementType)
{
  return _getIdFromSequence(elementType, "next");
}

long OsmApiDb::getNextId(const QString& tableName)
{
  return _getIdFromSequence(tableName, "next");
}

long OsmApiDb::_getIdFromSequence(const ElementType& elementType, const QString& sequenceType)
{
  LOG_TRACE(
    "Retrieving " << sequenceType << " " << elementType.toString() << " ID from sequence...");
  switch (elementType.getEnum())
  {
    case ElementType::Node:
    case ElementType::Way:
    case ElementType::Relation:
      return _getIdFromSequence("current_" + elementType.toString().toLower() + "s", sequenceType);
    default:
      throw HootException("Unknown element type");
  }
}

long OsmApiDb::_getIdFromSequence(const QString& tableName, const QString& sequenceType)
{
  long result;
  if (_seqQueries[tableName].get() == nullptr)
  {
    _seqQueries[tableName] = std::make_shared<QSqlQuery>(_db);
    _seqQueries[tableName]->setForwardOnly(true);
    //valid sequence types are "next" and "current"
    QString sql = QString("SELECT NEXTVAL('%1%2')").arg(tableName.toLower(), ApiDb::getSequenceId());
    if (sequenceType.toLower() == "current")
      sql = sql.replace("NEXTVAL", "CURRVAL");

    _seqQueries[tableName]->prepare(sql);
  }

  std::shared_ptr<QSqlQuery> query = _seqQueries[tableName];
  if (query->exec() == false)
    throw HootException("Error reserving IDs. type: " + tableName + " Error: " + query->lastError().text());

  if (query->next())
  {
    bool ok;
    result = query->value(0).toLongLong(&ok);
    if (!ok)
      throw HootException("Did not retrieve starting reserved ID.");
  }
  else
    throw HootException("Error retrieving sequence value. type: " + tableName + " Error: " + query->lastError().text());

  query->finish();

  return result;
}

long OsmApiDb::toOsmApiDbCoord(const double x)
{
  return round(x * COORDINATE_SCALE);
}

double OsmApiDb::fromOsmApiDbCoord(const long x)
{
  return (double)x / COORDINATE_SCALE;
}

QStringList OsmApiDb::_getTables()
{
  //The table ordering doesn't matter for constraint enabling/disabling.  It would, however,
  //matter for constraint adding/removing (would need to reverse this ordering for dropping,
  //I think).

  QStringList tableNames;

  tableNames.append(ApiDb::getCurrentRelationMembersTableName());
  tableNames.append(ApiDb::getCurrentRelationTagsTableName());
  tableNames.append(ApiDb::getCurrentRelationsTableName());
  tableNames.append(ApiDb::getRelationMembersTableName());
  tableNames.append(ApiDb::getRelationTagsTableName());
  tableNames.append(ApiDb::getRelationsTableName());

  tableNames.append(ApiDb::getCurrentWayNodesTableName());
  tableNames.append(ApiDb::getCurrentWayTagsTableName());
  tableNames.append(ApiDb::getCurrentWaysTableName());
  tableNames.append(ApiDb::getWayNodesTableName());
  tableNames.append(ApiDb::getWayTagsTableName());
  tableNames.append(ApiDb::getWaysTableName());

  tableNames.append(ApiDb::getCurrentNodeTagsTableName());
  tableNames.append(ApiDb::getCurrentNodesTableName());
  tableNames.append(ApiDb::getNodeTagsTableName());
  tableNames.append(ApiDb::getNodesTableName());

  tableNames.append(ApiDb::getChangesetCommentsTableName());
  tableNames.append(ApiDb::getChangesetsSubscribersTableName());
  tableNames.append(ApiDb::getChangesetTagsTableName());
  tableNames.append(ApiDb::getChangesetsTableName());

  return tableNames;
}

void OsmApiDb::disableConstraints()
{
  LOG_DEBUG("Disabling OSMAPI database constraints...");

  _modifyConstraints(_getTables(), true);
}

void OsmApiDb::enableConstraints()
{
  LOG_DEBUG("Enabling OSMAPI database constraints...");

  _modifyConstraints(_getTables(), false);
}

void OsmApiDb::_modifyConstraints(const QStringList& tableNames, bool disable)
{
  for (const auto& table : qAsConst(tableNames))
  {
    if (disable)
      DbUtils::disableTableConstraints(getDB(), table);
    else
      DbUtils::enableTableConstraints(getDB(), table);
  }
}

void OsmApiDb::dropIndexes()
{
  LOG_DEBUG("Dropping OSMAPI database indexes...");

  //changesets
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_bbox_idx").arg(ApiDb::getChangesetsTableName()));
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_closed_at_idx").arg(ApiDb::getChangesetsTableName()));
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_created_at_idx").arg(ApiDb::getChangesetsTableName()));
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_user_id_created_at_idx").arg(ApiDb::getChangesetsTableName()));
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_user_id_id_idx").arg(ApiDb::getChangesetsTableName()));

  //current nodes
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_tile_idx").arg(ApiDb::getCurrentNodesTableName()));
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_timestamp_idx").arg(ApiDb::getCurrentNodesTableName()));

  //nodes
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_tile_idx").arg(ApiDb::getNodesTableName()));
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_timestamp_idx").arg(ApiDb::getNodesTableName()));
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_changeset_id_idx").arg(ApiDb::getNodesTableName()));

  //current ways
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_timestamp_idx").arg(ApiDb::getCurrentWaysTableName()));

  //ways
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_changeset_id_idx").arg(ApiDb::getWaysTableName()));
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_timestamp_idx").arg(ApiDb::getWaysTableName()));

  //current way nodes
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_node_idx").arg(ApiDb::getCurrentWayNodesTableName()));

  //way nodes
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_node_idx").arg(ApiDb::getWayNodesTableName()));

  //current relations
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_timestamp_idx").arg(ApiDb::getCurrentRelationsTableName()));

  //relations
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_changeset_id_idx").arg(ApiDb::getRelationsTableName()));
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_timestamp_idx").arg(ApiDb::getRelationsTableName()));

  //current relation members
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_member_idx").arg(ApiDb::getCurrentRelationMembersTableName()));

  //relation members
  DbUtils::execNoPrepare(getDB(), QString("DROP INDEX %1_member_idx").arg(ApiDb::getRelationMembersTableName()));
}

void OsmApiDb::createIndexes()
{
  LOG_INFO("Creating OSMAPI database indexes...");

  //changesets
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_bbox_idx ON %1 USING gist (min_lat, max_lat, min_lon, max_lon)")
      .arg(ApiDb::getChangesetsTableName()));
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_closed_at_idx ON %1 USING btree (closed_at)")
      .arg(ApiDb::getChangesetsTableName()));
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_created_at_idx ON %1 USING btree (created_at)")
      .arg(ApiDb::getChangesetsTableName()));
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_user_id_created_at_idx ON %1 USING btree (user_id, created_at)")
      .arg(ApiDb::getChangesetsTableName()));
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_user_id_id_idx ON %1 USING btree (user_id, id)")
      .arg(ApiDb::getChangesetsTableName()));

  //current nodes
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_tile_idx ON %1 USING btree (tile)")
      .arg(ApiDb::getCurrentNodesTableName()));
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_timestamp_idx ON %1 USING btree (\"timestamp\")")
      .arg(ApiDb::getCurrentNodesTableName()));

  //nodes
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_tile_idx ON %1 USING btree (tile)")
      .arg(ApiDb::getNodesTableName()));
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_timestamp_idx ON %1 USING btree (\"timestamp\")")
      .arg(ApiDb::getNodesTableName()));
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_changeset_id_idx ON %1 USING btree (changeset_id)")
      .arg(ApiDb::getNodesTableName()));

  //current ways
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_timestamp_idx ON %1 USING btree (\"timestamp\")")
      .arg(ApiDb::getCurrentWaysTableName()));

  //ways
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_changeset_id_idx ON %1 USING btree (changeset_id)")
      .arg(ApiDb::getWaysTableName()));
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_timestamp_idx ON %1 USING btree (\"timestamp\")")
      .arg(ApiDb::getWaysTableName()));

  //current way nodes
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_node_idx ON %1 USING btree (node_id)")
      .arg(ApiDb::getCurrentWayNodesTableName()));

  //way nodes
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_node_idx ON %1 USING btree (node_id)")
      .arg(ApiDb::getWayNodesTableName()));

  //current relations
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_timestamp_idx ON %1 USING btree (\"timestamp\")")
      .arg(ApiDb::getCurrentRelationsTableName()));

  //relations
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_changeset_id_idx ON %1 USING btree (changeset_id)")
      .arg(ApiDb::getRelationsTableName()));
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_timestamp_idx ON %1 USING btree (\"timestamp\")")
      .arg(ApiDb::getRelationsTableName()));

  //current relation members
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_member_idx ON %1 USING btree (member_type, member_id)")
      .arg(ApiDb::getCurrentRelationMembersTableName()));

  //relation members
  DbUtils::execNoPrepare(getDB(), QString("CREATE INDEX %1_member_idx ON %1 USING btree (member_type, member_id)")
      .arg(ApiDb::getRelationMembersTableName()));
}

}
