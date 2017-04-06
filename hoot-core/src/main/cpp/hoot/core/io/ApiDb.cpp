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
#include "ApiDb.h"

// hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/SqlBulkInsert.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/algorithms/zindex/ZValue.h>
#include <hoot/core/algorithms/zindex/ZCurveRanger.h>
#include <hoot/core/algorithms/zindex/BBox.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/algorithms/zindex/Range.h>
#include <hoot/core/io/TableType.h>

// qt
#include <QStringList>
#include <QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QSet>

// Standard
#include <math.h>
#include <cmath>
#include <fstream>

// tgs
#include <tgs/System/Time.h>

#include "InternalIdReserver.h"

namespace hoot
{

const Status ApiDb::DEFAULT_ELEMENT_STATUS(Status::Invalid);

ApiDb::ApiDb()
{
}

ApiDb::~ApiDb()
{
  _resetQueries();

  if (_db.isOpen())
  {
    _db.close();
  }
}

void ApiDb::_resetQueries()
{
  if (_selectUserByEmail != 0)
  {
    _selectUserByEmail.reset();
  }
  if (_insertUser != 0)
  {
    _insertUser.reset();
  }
  if (_selectNodeIdsForWay != 0)
  {
    _selectNodeIdsForWay.reset();
  }
  _selectNodesByBounds.reset();
  _selectWayIdsByWayNodeIds.reset();
  _selectElementsByElementIdList.reset();
  _selectWayNodeIdsByWayIds.reset();
  _selectRelationIdsByMemberIds.reset();
  _selectChangesetsCreatedAfterTime.reset();
}

bool ApiDb::isSupported(const QUrl& url)
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
  {
    _db = QSqlDatabase::addDatabase("QPSQL", connectionName);
  }
  else
  {
    _db = QSqlDatabase::database(connectionName);
  }

  if (_db.isOpen() == false)
  {
    _db.setDatabaseName(db);
    if (url.host() == "local")
    {
      _db.setHostName("/var/run/postgresql");
    }
    else
    {
      _db.setHostName(url.host());
    }
    _db.setPort(url.port());
    _db.setUserName(url.userName());
    _db.setPassword(url.password());

    if (_db.open() == false)
    {
      throw HootException("Error opening _db: " + _db.lastError().text());
    }
  }

  if (_db.tables().size() == 0)
  {
    throw HootException("Attempting to open _db " + url.toString() +
                        " but found zero tables. Does the DB exist? Has it been populated?");
  }

  QSqlQuery query("SET client_min_messages TO WARNING", _db);
  // if there was an error
  if (query.lastError().isValid())
  {
    LOG_WARN("Error disabling Postgresql INFO messages.");
  }

  LOG_DEBUG("Successfully opened db: " << url.toString());
}

long ApiDb::getUserId(const QString email, bool throwWhenMissing)
{
  if (_selectUserByEmail == 0)
  {
    _selectUserByEmail.reset(new QSqlQuery(_db));
    _selectUserByEmail->prepare(
      "SELECT email, id, display_name FROM " + ApiDb::getUsersTableName() + " WHERE email LIKE :email");
  }
  _selectUserByEmail->bindValue(":email", email);
  if (_selectUserByEmail->exec() == false)
  {
    throw HootException("Error finding user with the email: " + email + " " +
                        _selectUserByEmail->lastError().text());
  }

  long result = -1;
  if (_selectUserByEmail->next())
  {
    bool ok;
    result = _selectUserByEmail->value(1).toLongLong(&ok);
    if (!ok && throwWhenMissing)
    {
      throw HootException("Specified user was not found.");
    }
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

long ApiDb::insertUser(const QString email, const QString displayName)
{
  long id = -1;

  if (_insertUser == 0)
  {
    _insertUser.reset(new QSqlQuery(_db));
    _insertUser->prepare("INSERT INTO " + ApiDb::getUsersTableName() + " (email, display_name) "
                         "VALUES (:email, :display_name) "
                         "RETURNING id");
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
      QString err = QString("Error executing query: %1 (%2)").arg(_insertUser->executedQuery()).
          arg(_insertUser->lastError().text());
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
    {
      id = _insertUser->value(0).toLongLong(&ok);
    }

    if (!ok || id == -1)
    {
      LOG_ERROR("query bound values: ");
      LOG_ERROR(_insertUser->boundValues());
      LOG_ERROR("\n");
      throw HootException("Error retrieving new ID " + _insertUser->lastError().text() + " Query: " +
        _insertUser->executedQuery());
    }

    _insertUser->finish();
  }

  return id;
}


vector<long> ApiDb::selectNodeIdsForWay(long wayId, const QString sql)
{
  vector<long> result;

  if (!_selectNodeIdsForWay)
  {
    _selectNodeIdsForWay.reset(new QSqlQuery(_db));
    _selectNodeIdsForWay->setForwardOnly(true);
    _selectNodeIdsForWay->prepare(sql);
  }

  _selectNodeIdsForWay->bindValue(":wayId", (qlonglong)wayId);
  if (_selectNodeIdsForWay->exec() == false)
  {
    throw HootException("Error selecting node ID's for way with ID: " + QString::number(wayId) +
      " Error: " + _selectNodeIdsForWay->lastError().text());
  }

  while (_selectNodeIdsForWay->next())
  {
    bool ok;
    result.push_back(_selectNodeIdsForWay->value(0).toLongLong(&ok));

    if (!ok)
    {
      QString err = QString("Error converting node ID to long. (%1)").
          arg(_selectNodeIdsForWay->value(0).toString());
      throw HootException(err);
    }
  }

  return result;
}

boost::shared_ptr<QSqlQuery> ApiDb::selectNodesForWay(long wayId, const QString sql)
{
  if (!_selectNodeIdsForWay)
  {
    _selectNodeIdsForWay.reset(new QSqlQuery(_db));
    _selectNodeIdsForWay->setForwardOnly(true);
    _selectNodeIdsForWay->prepare(sql);
  }

  _selectNodeIdsForWay->bindValue(":wayId", (qlonglong)wayId);
  if (_selectNodeIdsForWay->exec() == false)
  {
    throw HootException("Error selecting node ID's for way with ID: " + QString::number(wayId) +
      " Error: " + _selectNodeIdsForWay->lastError().text());
  }
  LOG_VART(_selectNodeIdsForWay->numRowsAffected());
  LOG_VART(_selectNodeIdsForWay->executedQuery());

  return _selectNodeIdsForWay;
}

Tags ApiDb::unescapeTags(const QVariant &v)
{
  /** NOTE:  When we upgrade from Qt4 to Qt5 we can use the QRegularExpression
   *  classes that should enable the regex below that has both greedy matching
   *  and lazy matching in the same regex.  The QRegExp class doesn't allow this
   *  that is why there are two regex objects in this function.
   *
   *  Replace it with this:
   *    QRegularExpression rxKeyValue("\"(.*?)\"=>\"((?:(?!\",).)*)\"(?:, )?");
   */
  assert(v.type() == QVariant::String);
  QString str = v.toString();

  Tags result;
  QRegExp rxKey("\"(.*)\"=>\"");
  QRegExp rxValue("((?:(?!\",).)*)\"(?:, )?");
  //  The key regex needs to be minimal should be (.*?) but that doesn't work
  //  while the value regex needs to be maximal to consume quotes within the value
  rxKey.setMinimal(true);
  rxValue.setMinimal(false);
  int pos = 0;
  //  Match the key first
  while ((pos = rxKey.indexIn(str, pos)) != -1)
  {
    pos += rxKey.matchedLength();
    //  Then match the value, ignoring any key/value pairs that don't match
    if ((pos = rxValue.indexIn(str, pos)) != -1)
    {
      QString key = rxKey.cap(1);
      QString value = rxValue.cap(1);
      //  Unescape the actual key/value pairs
      _unescapeString(key);
      _unescapeString(value);
      result.insert(key, value);
      pos += rxValue.matchedLength();
    }
  }

  return result;
}

void ApiDb::_unescapeString(QString& s)
{
  if (s.startsWith("\""))
  {
    s.remove(0, 1);
  }
  if (s.endsWith("\""))
  {
    s.remove(s.size() - 1, 1);
  }

  s.replace("\\042", "\"");
  s.replace("\\\"", "\"");
  s.replace("\\\\", "\\");
  s.replace("\\134", "\\");
}

QSqlQuery ApiDb::_exec(const QString sql, QVariant v1, QVariant v2, QVariant v3) const
{
  QSqlQuery q(_db);

  if (q.prepare(sql) == false)
  {
    throw HootException(QString("Error preparing query: %1 (%2)").arg(q.lastError().text()).
                        arg(sql));
  }

  if (v1.isValid())
  {
    q.bindValue(0, v1);
  }
  if (v2.isValid())
  {
    q.bindValue(1, v2);
  }
  if (v3.isValid())
  {
    q.bindValue(2, v3);
  }

  if (q.exec() == false)
  {
    throw HootException(QString("Error executing query: %1 (%2)").arg(q.lastError().text()).
                        arg(sql));
  }

  return q;
}

unsigned int ApiDb::tileForPoint(double lat, double lon)
{
  int lonInt = round((lon + 180.0) * 65535.0 / 360.0);
  int latInt = round((lat + 90.0) * 65535.0 / 180.0);

  unsigned int tile = 0;
  int          i;

  for (i = 15; i >= 0; i--)
  {
    tile = (tile << 1) | ((lonInt >> i) & 1);
    tile = (tile << 1) | ((latInt >> i) & 1);
  }

  return tile;
}

long ApiDb::round(double x)
{
  return (long)(x + 0.5);
}

boost::shared_ptr<QSqlQuery> ApiDb::selectNodesByBounds(const Envelope& bounds)
{
  LOG_VARD(bounds);
  const vector<Range> tileRanges = _getTileRanges(bounds);
  LOG_VARD(tileRanges.size());

  //I'm not sure yet if the number of tile ID ranges is constant or not.  If it is, then we could
  //modify this to add placeholders for the tile ID ranges and make it so the query only gets
  //prepared once.
  if (!_selectNodesByBounds)
  {
    _selectNodesByBounds.reset(new QSqlQuery(_db));
    _selectNodesByBounds->setForwardOnly(true);
  }
  QString sql = "SELECT * FROM " + tableTypeToTableName(TableType::Node) + " WHERE";
  sql += " visible = true";
  sql += " AND (" + _getTileWhereCondition(tileRanges) + ")";
  sql += " AND longitude >= :minLon AND longitude <= :maxLon AND latitude >= :minLat ";
  sql += " AND latitude <= :maxLat";
  sql += " ORDER BY id DESC";
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
  LOG_VARD(_selectNodesByBounds->lastQuery());
  LOG_VARD(_selectNodesByBounds->boundValues());

  if (_selectNodesByBounds->exec() == false)
  {
    throw HootException(
      "Error selecting nodes by bounds: " + QString::fromStdString(bounds.toString()) + " Error: " +
      _selectNodesByBounds->lastError().text());
  }
  LOG_VARD(_selectNodesByBounds->numRowsAffected());
  return _selectNodesByBounds;
}

boost::shared_ptr<QSqlQuery> ApiDb::selectWayIdsByWayNodeIds(const QSet<QString>& nodeIds)
{
  if (nodeIds.size() == 0)
  {
    throw HootException("Empty node ID list.");
  }

  if (!_selectWayIdsByWayNodeIds)
  {
    _selectWayIdsByWayNodeIds.reset(new QSqlQuery(_db));
    _selectWayIdsByWayNodeIds->setForwardOnly(true);
  }
  //this has to be prepared every time due to the varying number of IDs passed in
  QString sql = "SELECT DISTINCT way_id FROM " + tableTypeToTableName(TableType::WayNode) + " WHERE";
  sql += " node_id IN (" + QStringList(nodeIds.toList()).join(",") + ")";
  //sql += " ORDER BY way_id desc";
  _selectWayIdsByWayNodeIds->prepare(sql);
  LOG_VARD(_selectWayIdsByWayNodeIds->lastQuery());

  if (_selectWayIdsByWayNodeIds->exec() == false)
  {
    throw HootException(
      "Error selecting way IDs by way node IDs.  Error: " +
      _selectWayIdsByWayNodeIds->lastError().text());
  }
  LOG_VARD(_selectWayIdsByWayNodeIds->numRowsAffected());
  return _selectWayIdsByWayNodeIds;
}

boost::shared_ptr<QSqlQuery> ApiDb::selectElementsByElementIdList(const QSet<QString>& elementIds,
                                                           const TableType& tableType)
{
  if (elementIds.size() == 0)
  {
    throw HootException("Empty element ID list.");
  }

  if (!_selectElementsByElementIdList)
  {
    _selectElementsByElementIdList.reset(new QSqlQuery(_db));
    _selectElementsByElementIdList->setForwardOnly(true);
  }
  //this has to be prepared every time due to the varying number of IDs passed in
  QString sql = "SELECT * FROM " + tableTypeToTableName(tableType) + " WHERE";
  sql += " visible = true";
  sql += " AND id IN (" + QStringList(elementIds.toList()).join(",") + ")";
  sql += " ORDER BY id DESC";
  _selectElementsByElementIdList->prepare(sql);
  LOG_VARD(_selectElementsByElementIdList->lastQuery());

  if (_selectElementsByElementIdList->exec() == false)
  {
    throw HootException(
      "Error selecting elements by element ID list:  Error: " +
      _selectElementsByElementIdList->lastError().text());
  }
  LOG_VARD(_selectElementsByElementIdList->numRowsAffected());
  return _selectElementsByElementIdList;
}

boost::shared_ptr<QSqlQuery> ApiDb::selectWayNodeIdsByWayIds(const QSet<QString>& wayIds)
{
  if (wayIds.size() == 0)
  {
    throw HootException("Empty way ID list.");
  }

  if (!_selectWayNodeIdsByWayIds)
  {
    _selectWayNodeIdsByWayIds.reset(new QSqlQuery(_db));
    _selectWayNodeIdsByWayIds->setForwardOnly(true);
  }
  //this has to be prepared every time due to the varying number of IDs passed in
  QString sql = "SELECT DISTINCT node_id FROM " + tableTypeToTableName(TableType::WayNode) + " WHERE";
  sql += " way_id IN (" + QStringList(wayIds.toList()).join(",") + ")";
  //sql += " ORDER BY sequence_id";
  _selectWayNodeIdsByWayIds->prepare(sql);
  LOG_VARD(_selectWayNodeIdsByWayIds->lastQuery());

  if (_selectWayNodeIdsByWayIds->exec() == false)
  {
    throw HootException(
      "Error selecting way node IDs by way IDs.  Error: " +
      _selectWayNodeIdsByWayIds->lastError().text());
  }
  LOG_VARD(_selectWayNodeIdsByWayIds->numRowsAffected());
  return _selectWayNodeIdsByWayIds;
}

boost::shared_ptr<QSqlQuery> ApiDb::selectRelationIdsByMemberIds(const QSet<QString>& memberIds,
                                                          const ElementType& memberElementType)
{
  if (memberIds.size() == 0)
  {
    throw HootException("Empty member ID list.");
  }

  if (!_selectRelationIdsByMemberIds)
  {
    _selectRelationIdsByMemberIds.reset(new QSqlQuery(_db));
    _selectRelationIdsByMemberIds->setForwardOnly(true);
  }
  //this has to be prepared every time due to the varying number of IDs passed in
  QString sql = "SELECT DISTINCT relation_id FROM " +
    tableTypeToTableName(TableType::RelationMember) + " WHERE";
  sql += " member_type = :elementType";
  sql += " AND member_id IN (" + QStringList(memberIds.toList()).join(",") + ")";
  //sql += " ORDER BY relation_id DESC";
  _selectRelationIdsByMemberIds->prepare(sql);
  if (!_capitalizeRelationMemberType)
  {
    _selectRelationIdsByMemberIds->bindValue(":elementType", memberElementType.toString().toLower());
  }
  else
  {
    _selectRelationIdsByMemberIds->bindValue(":elementType", memberElementType.toString());
  }
  LOG_VARD(_selectRelationIdsByMemberIds->lastQuery());
  LOG_VARD(_selectRelationIdsByMemberIds->boundValues());

  if (_selectRelationIdsByMemberIds->exec() == false)
  {
    throw HootException(
      "Error selecting relation IDs by member IDs.  Error: " +
      _selectRelationIdsByMemberIds->lastError().text());
  }
  LOG_VARD(_selectRelationIdsByMemberIds->numRowsAffected());
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
  QString sql = "";
  for (uint i = 0; i < tileIdRanges.size(); i++)
  {
    if (i == tileIdRanges.size() - 1)
    {
      sql += "(tile between " + QString::number(tileIdRanges[i].getMin()) + " and " +
          QString::number(tileIdRanges[i].getMax()) + ")";
    }
    else
    {
      sql += "(tile between " + QString::number(tileIdRanges[i].getMin()) + " and " +
          QString::number(tileIdRanges[i].getMax()) + ") or ";
    }
  }
  return sql;
}

boost::shared_ptr<QSqlQuery> ApiDb::getChangesetsCreatedAfterTime(const QString timeStr)
{
  LOG_VARD(timeStr);
  if (!_selectChangesetsCreatedAfterTime)
  {
    _selectChangesetsCreatedAfterTime.reset(new QSqlQuery(_db));
    _selectChangesetsCreatedAfterTime->prepare(
      QString("SELECT min_lon, max_lon, min_lat, max_lat FROM %1 ")
        .arg(ApiDb::getChangesetsTableName()) +
      QString("WHERE created_at > :createdAt"));
    _selectChangesetsCreatedAfterTime->bindValue(":createdAt", "'" + timeStr + "'");
  }

  if (_selectChangesetsCreatedAfterTime->exec() == false)
  {
    LOG_ERROR(_selectChangesetsCreatedAfterTime->executedQuery());
    LOG_ERROR(_selectChangesetsCreatedAfterTime->lastError().text());
    throw HootException(
      "Could not execute changesets query: " +
      _selectChangesetsCreatedAfterTime->lastError().text());
  }
  LOG_VARD(_selectChangesetsCreatedAfterTime->executedQuery());
  LOG_VARD(_selectChangesetsCreatedAfterTime->numRowsAffected());

  return _selectChangesetsCreatedAfterTime;
}

QMap<QString, QString> ApiDb::getDbUrlParts(const QString url)
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

QString ApiDb::getPsqlString(const QString url)
{
  const QMap<QString, QString> dbUrlParts = getDbUrlParts(url);
  return
    "-h " + dbUrlParts["host"] + " -p " + dbUrlParts["port"] +
    " -U " + dbUrlParts["user"] + " -d " + dbUrlParts["database"];
}

}
