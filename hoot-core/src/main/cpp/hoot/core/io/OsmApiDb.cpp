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
#include "OsmApiDb.h"

// hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/SqlBulkInsert.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/io/TableType.h>
#include <hoot/core/util/DbUtils.h>

// qt
#include <QStringList>
#include <QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QFile>
#include <QTextStream>

// Standard
#include <math.h>
#include <cmath>
#include <fstream>

// tgs
#include <tgs/System/Time.h>

#include "InternalIdReserver.h"

namespace hoot
{

unsigned int OsmApiDb::logWarnCount = 0;

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

void OsmApiDb::deleteData()
{
  // delete ways data first
  DbUtils::execNoPrepare(
    _db,
    "DELETE FROM " + ApiDb::getCurrentRelationMembersTableName() + " CASCADE");
  DbUtils::execNoPrepare(
    _db, "DELETE FROM " + ApiDb::getCurrentRelationTagsTableName() + " CASCADE");
  DbUtils::execNoPrepare(
    _db, "DELETE FROM " + ApiDb::getCurrentRelationsTableName() + " CASCADE");
  DbUtils::execNoPrepare(
    _db, "ALTER SEQUENCE " + ApiDb::getCurrentRelationsSequenceName() + " RESTART WITH 1");
  DbUtils::execNoPrepare(
    _db, "DELETE FROM " + ApiDb::getRelationMembersTableName() + " CASCADE");
  DbUtils::execNoPrepare(
    _db, "DELETE FROM " + ApiDb::getRelationTagsTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "DELETE FROM " + ApiDb::getRelationsTableName() + " CASCADE");

  // delete relations data 2nd
  DbUtils::execNoPrepare(
    _db, "DELETE FROM " + ApiDb::getCurrentWayNodesTableName() + " CASCADE");
  DbUtils::execNoPrepare(
    _db, "DELETE FROM " + ApiDb::getCurrentWayTagsTableName() + " CASCADE");
  DbUtils::execNoPrepare(
    _db, "DELETE FROM " + ApiDb::getCurrentWaysTableName() + " CASCADE");
  DbUtils::execNoPrepare(
    _db, "ALTER SEQUENCE " + ApiDb::getCurrentWaysSequenceName() + " RESTART WITH 1");
  DbUtils::execNoPrepare(_db, "DELETE FROM " + ApiDb::getWayNodesTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "DELETE FROM " + ApiDb::getWayTagsTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "DELETE FROM " + ApiDb::getWaysTableName() + " CASCADE");

  // delete nodes data 3rd
  DbUtils::execNoPrepare(
    _db, "DELETE FROM " + ApiDb::getCurrentNodeTagsTableName() + " CASCADE");
  DbUtils::execNoPrepare(
    _db, "DELETE FROM " + ApiDb::getCurrentNodesTableName() + " CASCADE");
  DbUtils::execNoPrepare(
    _db, "ALTER SEQUENCE " + ApiDb::getCurrentNodesSequenceName() + " RESTART WITH 1");
  DbUtils::execNoPrepare(_db, "DELETE FROM " + ApiDb::getNodeTagsTableName() + " CASCADE");
  DbUtils::execNoPrepare(_db, "DELETE FROM " + ApiDb::getNodesTableName() + " CASCADE");

  // delete changesets
  DbUtils::execNoPrepare(
    _db, "DELETE FROM " + ApiDb::getChangesetsSubscribersTableName() + " CASCADE");
  DbUtils::execNoPrepare(
    _db, "DELETE FROM " + ApiDb::getChangesetTagsTableName() + " CASCADE");
  DbUtils::execNoPrepare(
    _db, "DELETE FROM " + ApiDb::getChangesetsTableName() + " CASCADE");
  DbUtils::execNoPrepare(
    _db, "ALTER SEQUENCE " + ApiDb::getChangesetsSequenceName() + " RESTART WITH 1");

  // delete users
  DbUtils::execNoPrepare(_db, "DELETE FROM " + ApiDb::getUsersTableName() + " CASCADE");
  DbUtils::execNoPrepare(
    _db, "ALTER SEQUENCE " + ApiDb::getUsersSequenceName() + " RESTART WITH 1");
}

bool OsmApiDb::isSupported(const QUrl& url)
{
  bool valid = ApiDb::isSupported(url);

  //postgresql is deprecated but still supported
  if (url.scheme() != "osmapidb" && url.scheme() != "postgresql")
  {
    valid = false;
  }

  if (valid)
  {
    QString path = url.path();
    QStringList plist = path.split("/");

    // OSM API will have plist.size of 2.
    if ( plist.size() != 2 )
    {
      valid = false;
    }
  }

  return valid;
}

void OsmApiDb::open(const QUrl& url)
{
  if (!isSupported(url))
  {
    throw HootException("An unsupported URL was passed into OsmApiDb: " + url.toString());
  }
  ApiDb::open(url);
}

void OsmApiDb::deleteUser(long userId)
{
  _exec("DELETE FROM " + ApiDb::getUsersTableName() + " WHERE id=:id", (qlonglong)userId);
}

void OsmApiDb::_resetQueries()
{
  ApiDb::_resetQueries();

  _selectElementsForMap.reset();
  _selectTagsForNode.reset();
  _selectTagsForWay.reset();
  _selectTagsForRelation.reset();
  _selectNodeIdsForWay.reset();
  _selectMembersForRelation.reset();
  _selectNodeById.reset();
  _selectUserByEmail.reset();
  _insertUser.reset();
  for (QHash<QString, boost::shared_ptr<QSqlQuery> >::iterator itr = _seqQueries.begin();
       itr != _seqQueries.end(); ++itr)
  {
    itr.value().reset();
  }
}

void OsmApiDb::rollback()
{
  _resetQueries();

  if (!_db.rollback())
  {
    throw HootException("Error rolling back transaction: " + _db.lastError().text());
  }
  _inTransaction = false;
}

void OsmApiDb::transaction()
{
  // Queries must be created from within the current transaction.
  _resetQueries();
  if (!_db.transaction())
  {
    throw HootException(_db.lastError().text());
  }
  _inTransaction = true;
}

void OsmApiDb::commit()
{
  if ( _db.isOpen() == false )
  {
    throw HootException("Tried to commit a transaction on a closed database");
  }

  if ( _inTransaction == false )
  {
    throw HootException("Tried to commit but weren't in a transaction");
  }

  _resetQueries();
  if (!_db.commit())
  {
    throw HootException("Error committing transaction: " + _db.lastError().text());
  }
  _inTransaction = false;
}

QString OsmApiDb::tableTypeToTableName(const TableType& tableType) const
{
  if (tableType == TableType::Node)
  {
    return ApiDb::getCurrentNodesTableName();
  }
  else if (tableType == TableType::Way)
  {
    return ApiDb::getCurrentWaysTableName();
  }
  else if (tableType == TableType::Relation)
  {
    return ApiDb::getCurrentRelationsTableName();
  }
  else if (tableType == TableType::WayNode)
  {
    return ApiDb::getCurrentWayNodesTableName();
  }
  else if (tableType == TableType::RelationMember)
  {
    return ApiDb::getCurrentRelationMembersTableName();
  }
  else
  {
    throw HootException("Unsupported table type.");
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
          {
            return ApiDb::getNodeTagsTableName();
          }
          else
          {
            return ApiDb::getNodesTableName();
          }
        }
        else
        {
          if (tags)
          {
            return ApiDb::getCurrentNodeTagsTableName();
          }
          else
          {
            return ApiDb::getCurrentNodesTableName();
          }
        }
      break;

    case ElementType::Way:
      if (historical)
      {
        if (tags)
        {
          return ApiDb::getWayTagsTableName();
        }
        else
        {
          return ApiDb::getWaysTableName();
        }
      }
      else
      {
        if (tags)
        {
          return ApiDb::getCurrentWayTagsTableName();
        }
        else
        {
          return ApiDb::getCurrentWaysTableName();
        }
      }
      break;

    case ElementType::Relation:
      if (historical)
      {
        if (tags)
        {
          return ApiDb::getRelationTagsTableName();
        }
        else
        {
          return ApiDb::getRelationsTableName();
        }
      }
      else
      {
        if (tags)
        {
          return ApiDb::getCurrentRelationTagsTableName();
        }
        else
        {
          return ApiDb::getCurrentRelationsTableName();
        }
      }
      break;

    default:
      throw HootException("Unknown element type");
  }
}

QString OsmApiDb::_elementTypeToElementTableNameStr(const ElementType& elementType) const
{
  if (elementType == ElementType::Node)
  {
    return QString("id, latitude, longitude, changeset_id, visible, timestamp, tile, version, k, v ") +
      QString("FROM %1 LEFT OUTER JOIN %2 ON %1.id=%2.node_id")
      .arg(ApiDb::getCurrentNodesTableName())
      .arg(ApiDb::getCurrentNodeTagsTableName());
  }
  else if (elementType == ElementType::Way)
  {
    return QString("id, changeset_id, timestamp, visible, version, k, v ") +
      QString("FROM %1 LEFT OUTER JOIN %2 ON %1.id=%2.way_id")
      .arg(ApiDb::getCurrentWaysTableName())
      .arg(ApiDb::getCurrentWayTagsTableName());
  }
  else if (elementType == ElementType::Relation)
  {
    return QString("id, changeset_id, timestamp, visible, version, k, v ") +
      QString("FROM %1 LEFT OUTER JOIN %2 ON %1.id=%2.relation_id")
      .arg(ApiDb::getCurrentRelationsTableName())
      .arg(ApiDb::getCurrentRelationTagsTableName());
  }
  else
  {
    throw HootException("Unsupported element type.");
  }
}

vector<long> OsmApiDb::selectNodeIdsForWay(long wayId)
{
  QString sql =  "SELECT node_id FROM " +
                  getCurrentWayNodesTableName() +
                 " WHERE way_id = :wayId ORDER BY sequence_id";
  return ApiDb::selectNodeIdsForWay(wayId, sql);
}

boost::shared_ptr<QSqlQuery> OsmApiDb::selectNodesForWay(long wayId)
{
  QString sql =  QString("SELECT node_id, latitude, longitude FROM %1 INNER JOIN %2 ON "
                         "%1.node_id=%2.id AND way_id = :wayId ORDER BY sequence_id")
                         .arg(ApiDb::getCurrentWayNodesTableName())
                         .arg(ApiDb::getCurrentNodesTableName());
  return ApiDb::selectNodesForWay(wayId, sql);
}

vector<RelationData::Entry> OsmApiDb::selectMembersForRelation(long relationId)
{
  vector<RelationData::Entry> result;

  if (!_selectMembersForRelation)
  {
    _selectMembersForRelation.reset(new QSqlQuery(_db));
    _selectMembersForRelation->setForwardOnly(true);
    _selectMembersForRelation->prepare(
      "SELECT member_type, member_id, member_role FROM " + getCurrentRelationMembersTableName() +
      " WHERE relation_id = :relationId ORDER BY sequence_id");
  }

  _selectMembersForRelation->bindValue(":relationId", (qlonglong)relationId);
  if (_selectMembersForRelation->exec() == false)
  {
    throw HootException("Error selecting members for relation: " + QString::number(relationId) +
      " Error: " + _selectMembersForRelation->lastError().text());
  }
  LOG_VARD(_selectMembersForRelation->executedQuery());
  LOG_VARD(_selectMembersForRelation->numRowsAffected());

  while (_selectMembersForRelation->next())
  {
    const QString memberType = _selectMembersForRelation->value(0).toString();
    LOG_VART(memberType);
    if (ElementType::isValidTypeString(memberType))
    {
      RelationData::Entry member =
        RelationData::Entry(
          _selectMembersForRelation->value(2).toString(),
          ElementId(ElementType::fromString(memberType),
          _selectMembersForRelation->value(1).toLongLong()));
      LOG_VART(member);
      result.push_back(member);
    }
    else
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Invalid relation member type: " + memberType + ".  Skipping relation member.");
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
  }

  return result;
}

boost::shared_ptr<QSqlQuery> OsmApiDb::selectNodeById(const long elementId)
{
  _selectNodeById.reset(new QSqlQuery(_db));
  _selectNodeById->setForwardOnly(true);
  QString sql =
    "SELECT " + _elementTypeToElementTableNameStr(ElementType::Node) +
    " WHERE (id=:elementId) ORDER BY id DESC";
  _selectNodeById->prepare(sql);
  _selectNodeById->bindValue(":elementId", (qlonglong)elementId);

  // execute the query on the DB and get the results back
  if (_selectNodeById->exec() == false)
  {
    QString err = _selectNodeById->lastError().text();
    throw HootException("Error selecting node by id: " + QString::number(elementId) +
      " Error: " + err);
  }
  LOG_VARD(_selectNodeById->executedQuery());
  LOG_VARD(_selectNodeById->numRowsAffected());

  return _selectNodeById;
}

boost::shared_ptr<QSqlQuery> OsmApiDb::selectElements(const ElementType& elementType)
{
  _selectElementsForMap.reset(new QSqlQuery(_db));
  _selectElementsForMap->setForwardOnly(true);

  // setup base sql query string
  QString sql =  "SELECT " + _elementTypeToElementTableNameStr(elementType);

  // sort them in descending order, set limit and offset
  sql += " WHERE visible = true ORDER BY id DESC";

  _selectElementsForMap->prepare(sql);

  // execute the query on the DB and get the results back
  if (_selectElementsForMap->exec() == false)
  {
    QString err = _selectElementsForMap->lastError().text();
    throw HootException("Error selecting elements of type: " + elementType.toString() +
      " Error: " + err);
  }
  LOG_VARD(_selectElementsForMap->executedQuery());
  LOG_VARD(_selectElementsForMap->numRowsAffected());

  return _selectElementsForMap;
}

boost::shared_ptr<QSqlQuery> OsmApiDb::selectTagsForRelation(long relId)
{
  if (!_selectTagsForRelation)
  {
    _selectTagsForRelation.reset(new QSqlQuery(_db));
    _selectTagsForRelation->setForwardOnly(true);
    QString sql =
      "SELECT relation_id, k, v FROM " + ApiDb::getCurrentRelationTagsTableName() +
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

boost::shared_ptr<QSqlQuery> OsmApiDb::selectTagsForWay(long wayId)
{
  if (!_selectTagsForWay)
  {
    _selectTagsForWay.reset(new QSqlQuery(_db));
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

boost::shared_ptr<QSqlQuery> OsmApiDb::selectTagsForNode(long nodeId)
{
  if (!_selectTagsForNode)
  {
    _selectTagsForNode.reset(new QSqlQuery(_db));
    _selectTagsForNode->setForwardOnly(true);
    QString sql =
      "SELECT node_id, k, v FROM " + ApiDb::getCurrentNodeTagsTableName() + " WHERE node_id = :nodeId";
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

QString OsmApiDb::extractTagFromRow(boost::shared_ptr<QSqlQuery> row, const ElementType::Type type)
{
  QString tag = "";
  int pos = -1;
  if (type == ElementType::Node) pos = OsmApiDb::NODES_TAGS;
  else if (type == ElementType::Way) pos = OsmApiDb::WAYS_TAGS;
  else if (type == ElementType::Relation) pos = OsmApiDb::RELATIONS_TAGS;
  else throw HootException("extractTagFromRow_OsmApi called with unknown Type");

  // test for blank tag
  QString val1 = row->value(pos).toString();
  QString val2 = row->value(pos+1).toString();
  if (val1 != "" || val2 != "") tag = "\"" + val1 + "\"=>\"" + val2 + "\"";

  return tag;
}

long OsmApiDb::getNextId(const ElementType& elementType)
{
  return _getIdFromSequence(elementType, "next");
}

long OsmApiDb::getNextId(const QString tableName)
{
  return _getIdFromSequence(tableName, "next");
}

long OsmApiDb::_getIdFromSequence(const ElementType& elementType, const QString sequenceType)
{
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

long OsmApiDb::_getIdFromSequence(const QString tableName, const QString sequenceType)
{
  long result;
  if (_seqQueries[tableName].get() == 0)
  {
    _seqQueries[tableName].reset(new QSqlQuery(_db));
    _seqQueries[tableName]->setForwardOnly(true);
    //valid sequence types are "next" and "current"
    QString sql =
      QString("SELECT NEXTVAL('%1%2')").arg(tableName.toLower()).arg(ApiDb::getSequenceId());
    if (sequenceType.toLower() == "current")
    {
      sql = sql.replace("NEXTVAL", "CURRVAL");
    }
    _seqQueries[tableName]->prepare(sql);
  }

  boost::shared_ptr<QSqlQuery> query = _seqQueries[tableName];
  if (query->exec() == false)
  {
    throw HootException("Error reserving IDs. type: " +
      tableName + " Error: " + query->lastError().text());
  }

  if (query->next())
  {
    bool ok;
    result = query->value(0).toLongLong(&ok);
    if (!ok)
    {
      throw HootException("Did not retrieve starting reserved ID.");
    }
  }
  else
  {
    throw HootException("Error retrieving sequence value. type: " +
      tableName + " Error: " + query->lastError().text());
  }
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

}
