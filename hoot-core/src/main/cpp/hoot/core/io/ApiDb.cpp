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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ApiDb.h"

// hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/db/SqlBulkInsert.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/ElementCacheLRU.h>
#include <hoot/core/util/OsmUtils.h>

// qt
#include <QStringList>
#include <QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

// Standard
#include <math.h>
#include <cmath>
#include <fstream>

// tgs
#include <tgs/System/Time.h>

#include "db/InternalIdReserver.h"

namespace hoot
{

const Status ApiDb::DEFAULT_ELEMENT_STATUS(Status::Invalid);

ApiDb::ApiDb()
{
}

ApiDb::~ApiDb()
{
  if (_selectUserByEmail != 0)
  {
    _selectUserByEmail.reset();
  }
  if (_insertUser != 0)
  {
    _insertUser.reset();
  }
}

bool ApiDb::isSupported(const QUrl& url)
{
  bool valid = url.isValid();
  return valid && url.scheme() == "postgresql";
}

void ApiDb::open(const QUrl& url, QSqlDatabase& database)
{
  QStringList pList = url.path().split("/");
  QString db = pList[1];

  QString connectionName = url.toString() + " 0x" + QString::number((qulonglong)this, 16);
  if (QSqlDatabase::contains(connectionName) == false)
  {
    database = QSqlDatabase::addDatabase("QPSQL", connectionName);
  }
  else
  {
    database = QSqlDatabase::database(connectionName);
  }

  if (database.isOpen() == false)
  {
    database.setDatabaseName(db);
    if (url.host() == "local")
    {
      database.setHostName("/var/run/postgresql");
    }
    else
    {
      database.setHostName(url.host());
    }
    database.setPort(url.port());
    database.setUserName(url.userName());
    database.setPassword(url.password());

    if (database.open() == false)
    {
      throw HootException("Error opening database: " + database.lastError().text());
    }
  }

  if (database.tables().size() == 0)
  {
    throw HootException("Attempting to open database " + url.toString() +
                        " but found zero tables. Does the DB exist? Has it been populated?");
  }

  QSqlQuery query("SET client_min_messages TO WARNING", database);
  // if there was an error
  if (query.lastError().isValid())
  {
    LOG_WARN("Error disabling Postgresql INFO messages.");
  }

  LOG_DEBUG("Successfully opened database: " << url.toString());
}

long ApiDb::getUserId(const QString& email, const QSqlDatabase& database, bool throwWhenMissing)
{
    LOG_DEBUG("debug email = "+email);
    LOG_DEBUG("debug throwwhenmissing = "+QString::number(throwWhenMissing));

  if (_selectUserByEmail == 0)
  {
    _selectUserByEmail.reset(new QSqlQuery(database));
    _selectUserByEmail->prepare("SELECT email, id, display_name FROM users WHERE email LIKE :email");
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
        .arg(email).arg(ConfigOptions::getServicesDbWriterCreateUserKey());
    LOG_WARN(error);
    throw HootException(error);
  }

  _selectUserByEmail->finish();

  return result;
}

long ApiDb::insertUser(const QString& email, const QSqlDatabase& database,
                       const QString& displayName)
{
  long id = -1;

  LOG_DEBUG("Inside insert user");
  if (_insertUser == 0)
  {
    _insertUser.reset(new QSqlQuery(database));
    _insertUser->prepare("INSERT INTO users (email, display_name) "
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
    id = getUserId(email, database, false);

    // nope, there is something else wrong. Report an error.
    if (id == -1)
    {
      QString err = QString("Error executing query: %1 (%2)").arg(_insertUser->executedQuery()).
          arg(_insertUser->lastError().text());
      LOG_WARN(err)
      throw HootException(err);
    }
    else
    {
      LOG_DEBUG("Did not insert user, queryied a previously created user.")
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


vector<long> ApiDb::selectNodeIdsForWay(long wayId,
                                        const QString& sql,
                                        const QSqlDatabase& database,
                                        shared_ptr<QSqlQuery>& sqlQuery)
{
  vector<long> result;

  if (!sqlQuery)
  {
    sqlQuery.reset(new QSqlQuery(database));
    sqlQuery->setForwardOnly(true);
    sqlQuery->prepare(sql);
  }

  sqlQuery->bindValue(":wayId", (qlonglong)wayId);
  if (sqlQuery->exec() == false)
  {
    throw HootException("Error selecting node ID's for way with ID: " + QString::number(wayId) +
      " Error: " + sqlQuery->lastError().text());
  }

  while (sqlQuery->next())
  {
    bool ok;
    result.push_back(sqlQuery->value(0).toLongLong(&ok));

    if (!ok)
    {
      QString err = QString("Error converting node ID to long. (%1)").
          arg(sqlQuery->value(0).toString());
      throw HootException(err);
    }
  }

  return result;
}

shared_ptr<QSqlQuery> selectNodesForWay(long wayId, const QString& sql,
                                        const QSqlDatabase& database,
                                        shared_ptr<QSqlQuery>& sqlQuery)
{
  if (!sqlQuery)
  {
    sqlQuery.reset(new QSqlQuery(database));
    sqlQuery->setForwardOnly(true);
    sqlQuery->prepare(sql);
  }

  sqlQuery->bindValue(":wayId", (qlonglong)wayId);
  if (sqlQuery->exec() == false)
  {
    throw HootException("Error selecting node ID's for way with ID: " + QString::number(wayId) +
      " Error: " + sqlQuery->lastError().text());
  }

  return sqlQuery;
}

Tags ApiDb::unescapeTags(const QVariant &v)
{
  assert(v.type() == QVariant::String);
  QString s = v.toString();
  // convert backslash and double quotes to their octal form so we can safely split on double quotes
  s.replace("\\\\", "\\134");
  s.replace("\\\"", "\\042");

  Tags result;
  QStringList l = s.split("\"");
  for (int i = 1; i < l.size(); i+=4)
  {
    _unescapeString(l[i]);
    _unescapeString(l[i + 2]);

    result.insert(l[i], l[i + 2]);
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

QSqlQuery ApiDb::_exec(const QString& sql, const QSqlDatabase& database,
                       QVariant v1, QVariant v2, QVariant v3) const
{
  QSqlQuery q(database);

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

QSqlQuery ApiDb::_execNoPrepare(const QString& sql, const QSqlDatabase& database) const
{
  // inserting strings in this fashion is safe b/c it is private and we closely control the table
  // names.
  QSqlQuery q(database);

  //LOG_VARD(sql);

  if (q.exec(sql) == false)
  {
    QString error = QString("Error executing query: %1 (%2)").arg(q.lastError().text()).
        arg(sql);
    LOG_WARN(error);
    throw HootException(error);
  }

  return q;
}

}
