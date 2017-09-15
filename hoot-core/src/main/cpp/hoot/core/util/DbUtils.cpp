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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "DbUtils.h"

// Qt
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

QSqlQuery DbUtils::execNoPrepare(QSqlDatabase& database, const QString sql)
{
  QSqlQuery q(database);

  LOG_VART(sql);
  if (q.exec(sql) == false)
  {
    throw HootException(
      QString("Error executing query: %1 (%2)").arg(q.lastError().text().left(500)).arg(sql));
  }
  LOG_VART(q.numRowsAffected());

  return q;
}

long DbUtils::getRowCount(const QSqlDatabase& database, const QString tableName)
{
  QSqlQuery query(database);
  if (!query.exec("SELECT COUNT(*) FROM " + tableName))
  {
    throw HootException(
      QString("Error executing row count query: %1 (%2)")
        .arg(query.lastError().text())
        .arg(tableName));
  }

  long result = -1;
  if (query.next())
  {
    bool ok;
    result = query.value(0).toLongLong(&ok);
    if (!ok)
    {
      throw HootException("Error executing row count query for " + tableName);
    }
  }
  else
  {
    throw HootException(
      "Error retrieving table row count. type: " + tableName + " Error: " +
      query.lastError().text());
  }
  query.finish();

  return result;
}

QStringList DbUtils::getConstraintsForTable(const QSqlDatabase& database, const QString tableName)
{
  QStringList constraints;

  const QString sql =
    QString("SELECT c.conname as constraint_name ") +
    QString("FROM pg_constraint c ") +
    QString("JOIN pg_class t on c.conrelid = t.oid ") +
    QString("JOIN pg_namespace n on t.relnamespace = n.oid ") +
    QString("WHERE t.relname = '%1'")
      .arg(tableName);
  QSqlQuery query(database);
  if (!query.exec(sql))
  {
    throw HootException(
      QString("Error executing constraints query: %1 (%2)")
        .arg(query.lastError().text())
        .arg(tableName));
  }

  while (query.next())
  {
    constraints.append(query.value(0).toString());
  }

  LOG_VARD(constraints);
  return constraints;
}

void DbUtils::disableTableConstraints(QSqlDatabase& database, const QString tableName)
{
  _modifyTableConstraints(database, tableName, true);
}

void DbUtils::enableTableConstraints(QSqlDatabase& database, const QString tableName)
{
  _modifyTableConstraints(database, tableName, false);
}

void DbUtils::_modifyTableConstraints(QSqlDatabase& database, const QString tableName,
                                      const bool disable)
{
  LOG_VART(database.isOpen());

  //TODO: do we want to do the style that checks constraints at the end here instead?
  QString operation = "DISABLE";
  if (!disable)
  {
    operation = "ENABLE";
  }
  LOG_DEBUG(operation << " constraints on " << tableName);

  QString sql =
    QString("ALTER TABLE %1 %2 TRIGGER ALL\n")
      .arg(tableName)
      .arg(operation);
  LOG_VARD(sql);

  QSqlQuery query(database);
  if (!query.exec(sql))
  {
    QString operationStr = operation.toLower();
    operationStr.chop(1);
    operationStr += "ing";
    throw HootException(
      QString("Error %1 constraints: %2 (%3)")
        .arg(operationStr)
        .arg(query.lastError().text())
        .arg(tableName));
  }
}

QString DbUtils::getPostgresDbVersion(const QSqlDatabase& database)
{
  QString version;

  QSqlQuery query(database);
  if (!query.exec("SELECT version()"))
  {
    throw HootException(
      QString("Error executing version query: %1")
        .arg(query.lastError().text()));
  }

  if (query.next())
  {
    version = query.value(0).toString();
    if (version.trimmed().isEmpty())
    {
      throw HootException("Error retrieving version.");
    }
  }
  else
  {
    throw HootException(
      "Error retrieving version. Error: " + query.lastError().text());
  }
  query.finish();

  return version;
}

}
