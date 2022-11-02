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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "ApiDb.h"

// hoot
#include <hoot/core/algorithms/zindex/BBox.h>
#include <hoot/core/algorithms/zindex/Range.h>
#include <hoot/core/algorithms/zindex/ZCurveRanger.h>
#include <hoot/core/algorithms/zindex/ZValue.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/InternalIdReserver.h>
#include <hoot/core/io/SqlBulkInsert.h>
#include <hoot/core/io/TableType.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/FileUtils.h>

// Qt
#include <QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QRegularExpression>

// Standard
#include <math.h>
#include <cmath>
#include <fstream>

// tgs
#include <tgs/System/Time.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

const Status ApiDb::DEFAULT_ELEMENT_STATUS(Status::Invalid);

int ApiDb::logWarnCount = 0;

ApiDb::ApiDb()
  : _maxElementsPerPartialMap(ConfigOptions().getMaxElementsPerPartialMap())
{
}

ApiDb::~ApiDb()
{
  _resetQueries();

  if (_db.isOpen())
    _db.close();
}

void ApiDb::_resetQueries()
{
  if (_selectUserByEmail != nullptr)
    _selectUserByEmail.reset();
  if (_insertUser != nullptr)
    _insertUser.reset();
  if (_selectNodeIdsForWay != nullptr)
    _selectNodeIdsForWay.reset();
  _selectNodesByBounds.reset();
  _selectWayIdsByWayNodeIds.reset();
  _selectElementsByElementIdList.reset();
  _selectWayNodeIdsByWayIds.reset();
  _selectRelationIdsByMemberIds.reset();
  _selectChangesetsCreatedAfterTime.reset();
  _userExists.reset();
  _getUserIdByName.reset();
  _getUserNameById.reset();

  for (auto itr = _maxIdQueries.begin(); itr != _maxIdQueries.end(); ++itr)
    itr.value().reset();

  for (auto itr = _numElementsQueries.begin(); itr != _numElementsQueries.end(); ++itr)
    itr.value().reset();

  for (auto itr = _numEstimatedElementsQueries.begin(); itr != _numEstimatedElementsQueries.end(); ++itr)
    itr.value().reset();

  for (auto itr = _selectQueries.begin(); itr != _selectQueries.end(); ++itr)
    itr.value().reset();

  for (auto itr = _selectAllQueries.begin(); itr != _selectAllQueries.end(); ++itr)
    itr.value().reset();
}

bool ApiDb::isSupported(const QUrl& url) const
{
  bool valid = url.isValid();
  if (url.scheme() == "postgresql")
  {
     LOG_WARN("postgresql:// is deprecated.")
  }
  return valid;
}

void ApiDb::open(const QUrl& url)
{
  QStringList pList = url.path().split("/");
  QString db = pList[1];

  QString connectionName = url.toString() + " 0x" + QString::number((qulonglong)this, 16);
  if (QSqlDatabase::contains(connectionName) == false)
    _db = QSqlDatabase::addDatabase("QPSQL", connectionName);
  else
    _db = QSqlDatabase::database(connectionName);

  if (_db.isOpen() == false)
  {
    _db.setDatabaseName(db);
    if (url.host() == "local")
      _db.setHostName("/var/run/postgresql");
    else
      _db.setHostName(url.host());

    _db.setPort(url.port());
    _db.setUserName(url.userName());
    _db.setPassword(url.password());

    if (_db.open() == false)
      throw HootException(QString("Error opening _db: %1").arg(_db.lastError().text()));
  }

  if (_db.tables().empty())
  {
    throw HootException(
      QString("Attempting to open _db %1 but found zero tables. Does the DB exist? Has it been populated?")
        .arg(url.toString(QUrl::RemoveUserInfo)));
  }

  QSqlQuery query("SET client_min_messages TO WARNING", _db);
  // if there was an error
  if (query.lastError().isValid())
  {
    LOG_WARN("Error disabling Postgresql INFO messages.");
  }

  LOG_DEBUG("Successfully opened db: " << url.toString(QUrl::RemoveUserInfo));
  LOG_DEBUG("Postgres database version: " << DbUtils::getPostgresDbVersion(_db));
}

void ApiDb::transaction()
{
  LOG_TRACE("Starting transaction...");

  // Queries must be created from within the current transaction.
  _resetQueries();
  if (!_db.transaction())
    throw HootException(_db.lastError().text());

  _inTransaction = true;
}

void ApiDb::rollback()
{
  LOG_TRACE("Rolling back transaction...");

  _resetQueries();

  if (!_db.rollback())
    throw HootException(QString("Error rolling back transaction: %1").arg(_db.lastError().text()));

  _inTransaction = false;
}

long ApiDb::getUserIdByName(const QString& userName)
{
  LOG_VART(userName);
  if (_getUserIdByName == nullptr)
  {
    _getUserIdByName = std::make_shared<QSqlQuery>(_db);
    _getUserIdByName->prepare(QString("SELECT id FROM %1 WHERE display_name = :displayName").arg(ApiDb::getUsersTableName()));
  }
  _getUserIdByName->bindValue(":displayName", userName);
  if (!_getUserIdByName->exec())
    throw HootException(QString("Error finding user with user name: %1 %2").arg(userName, _getUserIdByName->lastError().text()));

  long userId = -1;
  //should only be one result
  if (_getUserIdByName->next())
  {
    bool ok;
    userId = _getUserIdByName->value(0).toLongLong(&ok);
    if (!ok)
      throw HootException(QString("Error executing user ID query for %1").arg(userName));
  }
  else
  {
    LOG_TRACE("No user ID available for user name: " << userName);
    _getUserIdByName->finish();
    return -1;
  }
  _getUserIdByName->finish();

  LOG_VART(userId);
  return userId;
}

QString ApiDb::getUserNameById(const long userId)
{
  LOG_VART(userId);
  if (_getUserNameById == nullptr)
  {
    _getUserNameById = std::make_shared<QSqlQuery>(_db);
    _getUserNameById->prepare(QString("SELECT display_name FROM %1 WHERE id = :userId").arg(ApiDb::getUsersTableName()));
  }
  _getUserNameById->bindValue(":userId", (qlonglong)userId);
  if (!_getUserNameById->exec())
    throw HootException(QString("Error finding user with ID: %1 %2").arg(QString::number(userId), _getUserNameById->lastError().text()));

  QString userName = "";
  //should only be one result
  if (_getUserNameById->next())
    userName = _getUserNameById->value(0).toString();
  else
  {
    LOG_TRACE("No user name available for ID: " << userId);
    _getUserNameById->finish();
    return "";
  }
  _getUserNameById->finish();

  LOG_VART(userName);
  return userName;
}

bool ApiDb::userExists(const QString& userName)
{
  return userExists(getUserIdByName(userName));
}

bool ApiDb::userExists(const long id)
{
  LOG_VART(id);
  if (_userExists == nullptr)
  {
    _userExists = std::make_shared<QSqlQuery>(_db);
    _userExists->prepare(QString("SELECT id FROM %1 WHERE id = :id").arg(ApiDb::getUsersTableName()));
  }
  _userExists->bindValue(":id", (qlonglong)id);
  if (!_userExists->exec())
  {
    throw HootException(
      QString("Error finding user with id: %1 %2").arg(QString::number(id), _userExists->lastError().text()));
  }

  long result = -1;
  if (_userExists->next())
  {
    bool ok;
    result = _userExists->value(0).toLongLong(&ok);
    LOG_VART(result);
    if (!ok || result == -1)
      return false;
  }
  else
    return false;

  LOG_VART(result);

  _userExists->finish();

  return true;
}

long ApiDb::getUserId(const QString& email, bool throwWhenMissing)
{
  if (_selectUserByEmail == nullptr)
  {
    _selectUserByEmail = std::make_shared<QSqlQuery>(_db);
    _selectUserByEmail->prepare(QString("SELECT email, id, display_name FROM %1 WHERE email LIKE :email").arg(ApiDb::getUsersTableName()));
  }
  _selectUserByEmail->bindValue(":email", email);
  if (_selectUserByEmail->exec() == false)
    throw HootException(QString("Error finding user with the email: %1 %2").arg(email, _selectUserByEmail->lastError().text()));

  long result = -1;
  if (_selectUserByEmail->next())
  {
    bool ok;
    result = _selectUserByEmail->value(1).toLongLong(&ok);
    if (!ok && throwWhenMissing)
      throw HootException("Specified user was not found.");
  }
  else if (throwWhenMissing)
  {
    QString error = QString("No user found with the email: %1 (maybe specify `%2=true`?)")
        .arg(email).arg(ConfigOptions::getHootapiDbWriterCreateUserKey());
    throw HootException(error);
  }

  _selectUserByEmail->finish();

  return result;
}

long ApiDb::insertUser(const QString& email, const QString& displayName)
{
  LOG_TRACE("Inserting user with email: " << email << " and displayName: " << displayName);

  long id = -1;

  if (_insertUser == nullptr)
  {
    _insertUser = std::make_shared<QSqlQuery>(_db);
    _insertUser->prepare(
      QString("INSERT INTO %1 (email, display_name) VALUES (:email, :display_name) RETURNING id")
        .arg(ApiDb::getUsersTableName()));
  }
  _insertUser->bindValue(":email", email);
  _insertUser->bindValue(":display_name", displayName);

  // if we failed to execute the query the first time
  if (_insertUser->exec() == false)
  {
    // it may be that another process beat us to it and the user was already inserted. This can
    // happen if a bunch of converts are run in parallel. See #3588
    id = getUserId(email, false);

    // nope, there is something else wrong. Report an error.
    if (id == -1)
    {
      QString err = QString("Error executing query: %1 (%2)")
          .arg(_insertUser->executedQuery(), _insertUser->lastError().text());
      throw HootException(err);
    }
    else
    {
      LOG_DEBUG("Did not insert user, queried a previously created user.")
    }
  }
  // if the insert succeeded
  else
  {
    bool ok = false;
    if (_insertUser->next())
      id = _insertUser->value(0).toLongLong(&ok);

    if (!ok || id == -1)
    {
      LOG_ERROR("query bound values: ");
      LOG_ERROR(_insertUser->boundValues());
      LOG_ERROR("\n");
      throw HootException(QString("Error retrieving new ID %1 Query: %2")
                          .arg(_insertUser->lastError().text(), _insertUser->executedQuery()));
    }
    _insertUser->finish();
  }
  return id;
}

vector<long> ApiDb::_selectNodeIdsForWaySql(long wayId, const QString& sql)
{
  vector<long> result;

  if (!_selectNodeIdsForWay)
  {
    _selectNodeIdsForWay = std::make_shared<QSqlQuery>(_db);
    _selectNodeIdsForWay->setForwardOnly(true);
    _selectNodeIdsForWay->prepare(sql);
  }

  _selectNodeIdsForWay->bindValue(":wayId", (qlonglong)wayId);
  if (_selectNodeIdsForWay->exec() == false)
  {
    throw HootException(QString("Error selecting node ID's for way with ID: %1 Error: %2")
                        .arg(QString::number(wayId), _selectNodeIdsForWay->lastError().text()));
  }

  while (_selectNodeIdsForWay->next())
  {
    bool ok;
    result.push_back(_selectNodeIdsForWay->value(0).toLongLong(&ok));

    if (!ok)
    {
      QString err = QString("Error converting node ID to long. (%1)")
          .arg(_selectNodeIdsForWay->value(0).toString());
      throw HootException(err);
    }
  }

  return result;
}

Tags ApiDb::unescapeTags(const QVariant &v)
{
  assert(v.type() == QVariant::String);
  QString str = v.toString();

  Tags result;
  QRegularExpression rxKeyValue("\"(.*?)\"=>\"((?:(?!\",).)*)\"(?:, )?");
  QRegularExpressionMatchIterator matches = rxKeyValue.globalMatch(str);

  while (matches.hasNext())
  {
    QRegularExpressionMatch match = matches.next();
    QString key = match.captured(1);
    QString value = match.captured(2).trimmed();

    if (!value.isEmpty())
    {
      // Unescape the actual key/value pairs
      _unescapeString(key);
      _unescapeString(value);
      result.insert(key, value);
    }
  }
  return result;
}

void ApiDb::_unescapeString(QString& s)
{
  if (s.startsWith("\""))
    s.remove(0, 1);
  if (s.endsWith("\""))
    s.remove(s.size() - 1, 1);

  s.replace("\\042", "\"");
  s.replace("\\\"", "\"");
  s.replace("\\\\", "\\");
  s.replace("\\134", "\\");
}

QSqlQuery ApiDb::_exec(const QString& sql, QVariant v1, QVariant v2, QVariant v3) const
{
  QSqlQuery q(_db);

  if (q.prepare(sql) == false)
    throw HootException(QString("Error preparing query: %1 (%2)").arg(q.lastError().text(), sql));

  if (v1.isValid())
    q.bindValue(0, v1);

  if (v2.isValid())
    q.bindValue(1, v2);

  if (v3.isValid())
    q.bindValue(2, v3);

  if (q.exec() == false)
    throw HootException(QString("Error executing query: %1 (%2)").arg(q.lastError().text(), sql));

  return q;
}

unsigned int ApiDb::tileForPoint(double lat, double lon)
{
  int lonInt = static_cast<int>(round((lon + 180.0) * 65535.0 / 360.0));
  int latInt = static_cast<int>(round((lat + 90.0) * 65535.0 / 180.0));

  unsigned int tile = 0;

  for (int i = 15; i >= 0; i--)
  {
    tile = (tile << 1) | ((lonInt >> i) & 1);
    tile = (tile << 1) | ((latInt >> i) & 1);
  }

  return tile;
}

long ApiDb::round(double x)
{
  return static_cast<long>(x + 0.5);
}

std::shared_ptr<QSqlQuery> ApiDb::selectNodesByBounds(const Envelope& bounds)
{
  LOG_VART(bounds);
  const vector<Range> tileRanges = _getTileRanges(bounds);
  LOG_VART(tileRanges.size());

  // I'm not sure yet if the number of tile ID ranges is constant or not.  If it is, then we could
  // modify this to add placeholders for the tile ID ranges and make it so the query only gets
  // prepared once.
  if (!_selectNodesByBounds)
  {
    _selectNodesByBounds = std::make_shared<QSqlQuery>(_db);
    _selectNodesByBounds->setForwardOnly(true);
  }
  QString sql = QString("SELECT * FROM %1 WHERE visible = true"
                        " AND (%2)"
                        " AND longitude >= :minLon AND longitude <= :maxLon AND latitude >= :minLat "
                        " AND latitude <= :maxLat"
                        " ORDER BY id DESC").arg(tableTypeToTableName(TableType::Node), _getTileWhereCondition(tileRanges));
  _selectNodesByBounds->prepare(sql);
  if (!_floatingPointCoords)
  {
    _selectNodesByBounds->bindValue(":minLon", (qlonglong)(bounds.getMinX() * COORDINATE_SCALE));
    _selectNodesByBounds->bindValue(":maxLon", (qlonglong)(bounds.getMaxX() * COORDINATE_SCALE));
    _selectNodesByBounds->bindValue(":minLat", (qlonglong)(bounds.getMinY() * COORDINATE_SCALE));
    _selectNodesByBounds->bindValue(":maxLat", (qlonglong)(bounds.getMaxY() * COORDINATE_SCALE));
  }
  else
  {
    _selectNodesByBounds->bindValue(":minLon", bounds.getMinX());
    _selectNodesByBounds->bindValue(":maxLon", bounds.getMaxX());
    _selectNodesByBounds->bindValue(":minLat", bounds.getMinY());
    _selectNodesByBounds->bindValue(":maxLat", bounds.getMaxY());
  }
  LOG_VART(_selectNodesByBounds->lastQuery());
  LOG_VART(_selectNodesByBounds->boundValues());

  if (_selectNodesByBounds->exec() == false)
  {
    throw HootException(
      QString("Error selecting nodes by bounds: %1 Error: ")
        .arg(QString::fromStdString(bounds.toString()), _selectNodesByBounds->lastError().text()));
  }
  LOG_VART(_selectNodesByBounds->numRowsAffected());
  return _selectNodesByBounds;
}

std::shared_ptr<QSqlQuery> ApiDb::selectWayIdsByWayNodeIds(const QSet<QString>& nodeIds)
{
  if (nodeIds.empty())
    throw HootException("Empty node ID list.");

  if (!_selectWayIdsByWayNodeIds)
  {
    _selectWayIdsByWayNodeIds = std::make_shared<QSqlQuery>(_db);
    _selectWayIdsByWayNodeIds->setForwardOnly(true);
  }
  //this has to be prepared every time due to the varying number of IDs passed in
  QString sql = QString("SELECT DISTINCT way_id FROM %1 WHERE node_id IN (%2)")
      .arg(tableTypeToTableName(TableType::WayNode), nodeIds.toList().join(","));
  _selectWayIdsByWayNodeIds->prepare(sql);
  LOG_VART(_selectWayIdsByWayNodeIds->lastQuery().right(100));

  if (_selectWayIdsByWayNodeIds->exec() == false)
  {
    throw HootException(
      QString("Error selecting way IDs by way node IDs.  Error: %1").arg(_selectWayIdsByWayNodeIds->lastError().text()));
  }
  LOG_VART(_selectWayIdsByWayNodeIds->numRowsAffected());
  return _selectWayIdsByWayNodeIds;
}

QSet<QString> ApiDb::selectConnectedWayIds(const QSet<QString>& wayIds)
{
  if (wayIds.empty())
    throw IllegalArgumentException("Empty way ID list.");

  // There may be a more clever way to get this into a single query via a join, etc...going with
  // this method for now.

  // select all the way nodes contained in all of the input way ids
  std::shared_ptr<QSqlQuery> wayNodeIdItr = selectWayNodeIdsByWayIds(wayIds);
  QSet<QString> wayNodeIds;
  while (wayNodeIdItr->next())
    wayNodeIds.insert((*wayNodeIdItr).value(0).toString());

  LOG_VART(wayNodeIds);

  // select all ways that contain any of the way nodes
  std::shared_ptr<QSqlQuery> wayIdItr = selectWayIdsByWayNodeIds(wayNodeIds);
  QSet<QString> allWayIds;
  while (wayIdItr->next())
    allWayIds.insert((*wayIdItr).value(0).toString());

  LOG_VART(allWayIds);

  // subtract out any way ids in the input list
  const QSet<QString> connectedWayIds = allWayIds.subtract(wayIds);
  LOG_VART(connectedWayIds);

  return connectedWayIds;
}

std::shared_ptr<QSqlQuery> ApiDb::selectElementsByElementIdList(const QSet<QString>& elementIds,
                                                                const TableType& tableType)
{
  if (elementIds.empty())
    throw HootException("Empty element ID list.");

  if (!_selectElementsByElementIdList)
  {
    _selectElementsByElementIdList = std::make_shared<QSqlQuery>(_db);
    _selectElementsByElementIdList->setForwardOnly(true);
  }
  //this has to be prepared every time due to the varying number of IDs passed in
  QString sql = QString("SELECT * FROM %1 WHERE visible = true AND id IN (%2) ORDER BY id DESC")
      .arg(tableTypeToTableName(tableType), elementIds.toList().join(","));
  _selectElementsByElementIdList->prepare(sql);
  LOG_VART(_selectElementsByElementIdList->lastQuery().right(100));

  if (_selectElementsByElementIdList->exec() == false)
  {
    throw HootException(
      QString("Error selecting elements by element ID list:  Error: %1")
        .arg(_selectElementsByElementIdList->lastError().text()));
  }
  LOG_VART(_selectElementsByElementIdList->numRowsAffected());
  return _selectElementsByElementIdList;
}

std::shared_ptr<QSqlQuery> ApiDb::selectWayNodeIdsByWayIds(const QSet<QString>& wayIds)
{
  if (wayIds.empty())
    throw HootException("Empty way ID list.");

  if (!_selectWayNodeIdsByWayIds)
  {
    _selectWayNodeIdsByWayIds = std::make_shared<QSqlQuery>(_db);
    _selectWayNodeIdsByWayIds->setForwardOnly(true);
  }
  //this has to be prepared every time due to the varying number of IDs passed in
  QString sql = QString("SELECT DISTINCT node_id FROM %1 WHERE way_id IN (%2)")
      .arg(tableTypeToTableName(TableType::WayNode), QStringList(wayIds.toList()).join(","));
  //sql += " ORDER BY sequence_id";
  _selectWayNodeIdsByWayIds->prepare(sql);
  LOG_VART(_selectWayNodeIdsByWayIds->lastQuery().right(100));

  if (_selectWayNodeIdsByWayIds->exec() == false)
  {
    throw HootException(
      QString("Error selecting way node IDs by way IDs.  Error: %1").arg(_selectWayNodeIdsByWayIds->lastError().text()));
  }
  LOG_VART(_selectWayNodeIdsByWayIds->numRowsAffected());
  return _selectWayNodeIdsByWayIds;
}

std::shared_ptr<QSqlQuery> ApiDb::selectRelationIdsByMemberIds(const QSet<QString>& memberIds,
                                                               const ElementType& memberElementType)
{
  if (memberIds.empty())
    throw HootException("Empty member ID list.");

  if (!_selectRelationIdsByMemberIds)
  {
    _selectRelationIdsByMemberIds = std::make_shared<QSqlQuery>(_db);
    _selectRelationIdsByMemberIds->setForwardOnly(true);
  }
  //this has to be prepared every time due to the varying number of IDs passed in
  QString sql = QString("SELECT DISTINCT relation_id FROM %1 WHERE member_type = :elementType AND member_id IN (%2)")
                  .arg(tableTypeToTableName(TableType::RelationMember), memberIds.toList().join(","));
  _selectRelationIdsByMemberIds->prepare(sql);
  if (!_capitalizeRelationMemberType)
    _selectRelationIdsByMemberIds->bindValue(":elementType", memberElementType.toString().toLower());
  else
    _selectRelationIdsByMemberIds->bindValue(":elementType", memberElementType.toString());

  LOG_VART(_selectRelationIdsByMemberIds->lastQuery().right(100));
  LOG_VART(_selectRelationIdsByMemberIds->boundValues());

  if (_selectRelationIdsByMemberIds->exec() == false)
  {
    throw HootException(
      QString("Error selecting relation IDs by member IDs.  Error: %1").arg(_selectRelationIdsByMemberIds->lastError().text()));
  }
  LOG_VART(_selectRelationIdsByMemberIds->numRowsAffected());
  return _selectRelationIdsByMemberIds;
}

vector<Range> ApiDb::_getTileRanges(const Envelope& env) const
{
  const double minLat = env.getMinY();
  const double minLon = env.getMinX();
  const double maxLat = env.getMaxY();
  const double maxLon = env.getMaxX();

  vector<double> minV;
  minV.push_back(-90.0);
  minV.push_back(-180.0);
  vector<double> maxV;
  maxV.push_back(90.0);
  maxV.push_back(180.0);
  ZValue zv(2, 16, minV, maxV);
  ZCurveRanger ranger(zv);

  vector<double> minB;
  minB.push_back(minLat);
  minB.push_back(minLon);
  vector<double> maxB;
  maxB.push_back(maxLat);
  maxB.push_back(maxLon);

  BBox b(minB, maxB);
  return ranger.decomposeRange(b, 1);
}

QString ApiDb::_getTileWhereCondition(const vector<Range>& tileIdRanges) const
{
  QStringList sql;
  for (uint i = 0; i < tileIdRanges.size(); i++)
  {
    sql.append(QString("(tile between %1 and %2)").arg(QString::number(tileIdRanges[i].getMin()), QString::number(tileIdRanges[i].getMax())));
    if (i != tileIdRanges.size() - 1)
      sql.append(" or ");
  }
  return sql.join("");
}

std::shared_ptr<QSqlQuery> ApiDb::getChangesetsCreatedAfterTime(const QString& timeStr)
{
  LOG_VART(timeStr);
  if (!_selectChangesetsCreatedAfterTime)
  {
    _selectChangesetsCreatedAfterTime = std::make_shared<QSqlQuery>(_db);
    _selectChangesetsCreatedAfterTime->prepare(
      QString("SELECT min_lon, max_lon, min_lat, max_lat FROM %1 WHERE created_at > :createdAt")
        .arg(ApiDb::getChangesetsTableName()));
    _selectChangesetsCreatedAfterTime->bindValue(":createdAt", "'" + timeStr + "'");
  }

  if (_selectChangesetsCreatedAfterTime->exec() == false)
  {
    LOG_ERROR(_selectChangesetsCreatedAfterTime->executedQuery());
    LOG_ERROR(_selectChangesetsCreatedAfterTime->lastError().text());
    throw HootException(
      QString("Could not execute changesets query: %1")
        .arg(_selectChangesetsCreatedAfterTime->lastError().text()));
  }
  LOG_VART(_selectChangesetsCreatedAfterTime->executedQuery());
  LOG_VART(_selectChangesetsCreatedAfterTime->numRowsAffected());

  return _selectChangesetsCreatedAfterTime;
}

QMap<QString, QString> ApiDb::getDbUrlParts(const QString& url)
{
  QMap<QString, QString> dbUrlParts;

  QStringList dbUrlPartsList = url.split("/");
  dbUrlParts["database"] = dbUrlPartsList[dbUrlPartsList.size() - 1];
  QStringList userParts = dbUrlPartsList[dbUrlPartsList.size() - 2].split(":");
  dbUrlParts["user"] = userParts[0];
  dbUrlParts["password"] = userParts[1].split("@")[0];
  dbUrlParts["host"] = userParts[1].split("@")[1];
  dbUrlParts["port"] = userParts[2];

  return dbUrlParts;
}

QString ApiDb::getPsqlString(const QString& url)
{
  const QMap<QString, QString> dbUrlParts = getDbUrlParts(url);
  return QString("-h %1 -p %2 -U %3 -d %4").arg(dbUrlParts["host"], dbUrlParts["port"], dbUrlParts["user"], dbUrlParts["database"]);
}

void ApiDb::execSqlFile(const QString& dbUrl, const QString& sqlFile)
{
  const QMap<QString, QString> dbUrlParts = ApiDb::getDbUrlParts(dbUrl);
  QString cmd =
      QString("export PGPASSWORD=%1; psql -v ON_ERROR_STOP=1 %2 -f %3 2>&1").arg(dbUrlParts["password"], getPsqlString(dbUrl), sqlFile);
  if (Log::getInstance().getLevel() > Log::Debug)
    cmd += " > /dev/null";

  LOG_VARD(cmd);
  LOG_VART(FileUtils::readFully(sqlFile));
  const int retval = system(cmd.toStdString().c_str());
  if (retval != 0)
    throw HootException(QString("Failed executing SQL file against the database.  Status: %1").arg(QString::number(retval)));
}

Settings ApiDb::readDbConfig()
{
  Settings result;
  //  Read in the default values
  QString defaults = ConfPath::getHootHome() + "/conf/database/DatabaseConfigDefault.sh";
  readDbConfig(result, defaults);
  //  Read in the local values if the file exists
  QString local = ConfPath::getHootHome() + "/conf/database/DatabaseConfigLocal.sh";
  if (QFile::exists(local))
    readDbConfig(result, local);

  return result;
}

void ApiDb::readDbConfig(Settings& settings, const QString& config_path)
{
  QFile fp(config_path);
  if (fp.open(QIODevice::ReadOnly) == false)
    throw HootException(QString("Error opening: %1").arg(fp.fileName()));

  QString s = QString::fromUtf8(fp.readAll());

  QStringList sl = s.split('\n', QString::SkipEmptyParts);

  for (const auto& split : qAsConst(sl))
  {
    QString key = split.section("=", 0, 0).remove("export ").trimmed();
    QString value = split.section("=", 1).trimmed();
    if (!key.startsWith("#") && key.length() > 0)
      settings.set(key, value);
  }
}

long ApiDb::maxId(const ElementType& elementType)
{
  const QString elementTableName = elementTypeToElementTableName(elementType);
  if (!_maxIdQueries[elementTableName])
  {
    _maxIdQueries[elementTableName] = std::make_shared<QSqlQuery>(_db);
    _maxIdQueries[elementTableName]->prepare(
      QString("SELECT id FROM %1 ORDER BY id DESC LIMIT 1").arg(elementTableName));
  }
  LOG_VARD(_maxIdQueries[elementTableName]->lastQuery());

  if (_maxIdQueries[elementTableName]->exec() == false)
  {
    LOG_ERROR(_maxIdQueries[elementTableName]->executedQuery());
    LOG_ERROR(_maxIdQueries[elementTableName]->lastError().text());
    throw HootException(_maxIdQueries[elementTableName]->lastError().text());
  }

  long result = std::numeric_limits<long>::min();
  if (_maxIdQueries[elementTableName]->next())
  {
    bool ok;
    result = _maxIdQueries[elementTableName]->value(0).toLongLong(&ok);
    if (!ok)
      throw HootException(QString("Count not retrieve max ID for element type: %1").arg(elementType.toString()));
  }
  _maxIdQueries[elementTableName]->finish();
  return result;
}

long ApiDb::numElements(const ElementType& elementType)
{
  const QString elementTableName = elementTypeToElementTableName(elementType);
  if (!_numElementsQueries[elementTableName])
  {
    _numElementsQueries[elementTableName] = std::make_shared<QSqlQuery>(_db);
    _numElementsQueries[elementTableName]->prepare(
      QString("SELECT COUNT(*) FROM %1 WHERE visible = true").arg(elementTableName));
  }
  LOG_VARD(_numElementsQueries[elementTableName]->lastQuery());

  if (_numElementsQueries[elementTableName]->exec() == false)
  {
    LOG_ERROR(_numElementsQueries[elementTableName]->executedQuery());
    LOG_ERROR(_numElementsQueries[elementTableName]->lastError().text());
    throw HootException(_numElementsQueries[elementTableName]->lastError().text());
  }

  long result = -1;
  if (_numElementsQueries[elementTableName]->next())
  {
    bool ok;
    result = _numElementsQueries[elementTableName]->value(0).toLongLong(&ok);
    if (!ok)
      throw HootException(QString("Count not retrieve count for element type: %1").arg(elementType.toString()));
  }
  _numElementsQueries[elementTableName]->finish();
  return result;
}

long ApiDb::numEstimatedElements(const ElementType& elementType)
{
  const QString elementTableName = elementTypeToElementTableName(elementType);
  if (!_numEstimatedElementsQueries[elementTableName])
  {
    _numEstimatedElementsQueries[elementTableName] = std::make_shared<QSqlQuery>(_db);
    _numEstimatedElementsQueries[elementTableName]->prepare(
      QString("SELECT reltuples AS approximate_row_count FROM pg_class WHERE relname = '%1'").arg(elementTableName));
  }
  LOG_VARD(_numEstimatedElementsQueries[elementTableName]->lastQuery());

  if (_numEstimatedElementsQueries[elementTableName]->exec() == false)
  {
    LOG_ERROR(_numEstimatedElementsQueries[elementTableName]->executedQuery());
    LOG_ERROR(_numEstimatedElementsQueries[elementTableName]->lastError().text());
    throw HootException(_numEstimatedElementsQueries[elementTableName]->lastError().text());
  }

  long result = -1;
  if (_numEstimatedElementsQueries[elementTableName]->next())
  {
    bool ok;
    result = _numEstimatedElementsQueries[elementTableName]->value(0).toLongLong(&ok);
    if (!ok)
      throw HootException(QString("Count not retrieve count for element type: %1").arg(elementType.toString()));
  }
  _numEstimatedElementsQueries[elementTableName]->finish();
  return result;
}

std::shared_ptr<QSqlQuery> ApiDb::selectAllElements(const ElementType& elementType)
{
  const QString elementTableName = elementTypeToElementTableName(elementType);
  if (!_selectAllQueries[elementTableName])
  {
    _selectAllQueries[elementTableName] = std::make_shared<QSqlQuery>(_db);
    _selectAllQueries[elementTableName]->setForwardOnly(true);
    const QString sql = QString("SELECT * FROM %1 WHERE visible = true ORDER BY id").arg(elementTableName);
    LOG_VARD(sql);
    _selectAllQueries[elementTableName]->prepare(sql);
  }
  LOG_VARD(_selectAllQueries[elementTableName]->lastQuery());

  if (_selectAllQueries[elementTableName]->exec() == false)
  {
    const QString err =
      QString("Error selecting all elements of type: %1 Error: %2").arg(elementType.toString(), _selectAllQueries[elementTableName]->lastError().text());
    LOG_ERROR(err);
    throw HootException(err);
  }
  LOG_VARD(_selectAllQueries[elementTableName]->numRowsAffected());
  LOG_VART(_selectAllQueries[elementTableName]->executedQuery());

  return _selectAllQueries[elementTableName];
}

std::shared_ptr<QSqlQuery> ApiDb::selectElements(const ElementType& elementType, const long minId)
{
  const QString elementTableName = elementTypeToElementTableName(elementType);
  if (!_selectQueries[elementTableName])
  {
    _selectQueries[elementTableName] = std::make_shared<QSqlQuery>(_db);
    _selectQueries[elementTableName]->setForwardOnly(true);
    //This query uses the ORDER BY with LIMIT to ensure that we consistently see unique pages
    //of data returned. However, the data coming back won't necessarily be strictly sorted by
    //element ID.  I *believe* this is because we have no index on element ID (or maybe this just
    //how PG optimizes it?).  This is ok, though, b/c if we needed all elements returned strictly
    //sorted by ID (e.g. changeset derivation) we can just do that after retrieving them. - BDW
    const QString sql =
      QString("SELECT * FROM %1 WHERE visible = true AND id > :minId ORDER BY id LIMIT %2")
        .arg(elementTableName, QString::number(_maxElementsPerPartialMap));
    LOG_VARD(sql);
    if (!_selectQueries[elementTableName]->prepare(sql))
      throw HootException(QString("Unable to prepare query: %1").arg(sql));
  }
  _selectQueries[elementTableName]->bindValue(":minId", (qlonglong)minId);
  LOG_VARD(_selectQueries[elementTableName]->lastQuery());

  if (_selectQueries[elementTableName]->exec() == false)
  {
    const QString err =
      QString("Error selecting elements of type: %1 Error: %2")
        .arg(elementType.toString(), _selectQueries[elementTableName]->lastError().text());
    LOG_ERROR(err);
    throw HootException(err);
  }
  LOG_VARD(_selectQueries[elementTableName]->numRowsAffected());
  LOG_VART(_selectQueries[elementTableName]->executedQuery());

  return _selectQueries[elementTableName];
}

std::vector<RelationData::Entry> ApiDb::_selectRelationMembers(const std::shared_ptr<QSqlQuery>& relation_query) const
{
  vector<RelationData::Entry> results;
  if (!relation_query)
    return results;

  if (relation_query->exec() == false)
  {
    throw HootException(
      QString("Error selecting members for relation with ID: %1 Error: %2")
        .arg(relation_query->boundValue(0).toString(), relation_query->lastError().text()));
  }
  LOG_VART(relation_query->numRowsAffected());
  LOG_VART(relation_query->executedQuery());

  while (relation_query->next())
  {
    const QString memberType = relation_query->value(0).toString();
    LOG_VART(memberType);
    if (ElementType::isValidTypeString(memberType))
    {
      RelationData::Entry member =
        RelationData::Entry(
          relation_query->value(2).toString(),
          ElementId(ElementType::fromString(memberType),
          relation_query->value(1).toLongLong()));
      LOG_VART(member);
      results.push_back(member);
    }
    else
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Invalid relation member type: " + memberType + ".  Skipping relation member.");
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
  }

  return results;
}

}
