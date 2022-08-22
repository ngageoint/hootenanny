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
 * @copyright Copyright (C) 2015, 2017, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "DbUtils.h"

// Qt
#include <QSqlQuery>
#include <QSqlError>

// Hoot
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

QSqlQuery DbUtils::execNoPrepare(const QSqlDatabase& database, const QString& sql)
{
  QSqlQuery q(database);

  LOG_VART(sql);
  if (q.exec(sql) == false)
    throw HootException(QString("Error executing query. Error: %1 ...\nSQL: %2 ...").arg(q.lastError().text().left(200), sql.left(200)));

  LOG_VART(q.numRowsAffected());

  return q;
}

long DbUtils::getRowCount(const QSqlDatabase& database, const QString& tableName)
{
  QSqlQuery query(database);
  if (!query.exec("SELECT COUNT(*) FROM " + tableName))
    throw HootException(QString("Error executing row count query: %1 (%2)").arg(query.lastError().text(), tableName));

  long result = -1;
  if (query.next())
  {
    bool ok;
    result = query.value(0).toLongLong(&ok);
    if (!ok)
      throw HootException("Error executing row count query for " + tableName);
  }
  else
    throw HootException("Error retrieving table row count. type: " + tableName + " Error: " + query.lastError().text());

  query.finish();

  return result;
}

void DbUtils::disableTableConstraints(const QSqlDatabase& database, const QString& tableName)
{
  _modifyTableConstraints(database, tableName, true);
}

void DbUtils::enableTableConstraints(const QSqlDatabase& database, const QString& tableName)
{
  _modifyTableConstraints(database, tableName, false);
}

void DbUtils::_modifyTableConstraints(const QSqlDatabase& database, const QString& tableName,
                                      const bool disable)
{
  LOG_VART(database.isOpen());

  // Do we want to do the style that checks constraints at the end here instead?
  QString operation = "DISABLE";
  if (!disable)
    operation = "ENABLE";

  LOG_DEBUG(operation << " constraints on " << tableName);

  QString sql = QString("ALTER TABLE %1 %2 TRIGGER ALL\n").arg(tableName, operation);
  LOG_VARD(sql);

  QSqlQuery query(database);
  if (!query.exec(sql))
  {
    QString operationStr = operation.toLower();
    operationStr.chop(1);
    operationStr += "ing";
    throw HootException(QString("Error %1 constraints: %2 (%3)").arg(operationStr, query.lastError().text(), tableName));
  }
}

QString DbUtils::getPostgresDbVersion(const QSqlDatabase& database)
{
  QString version;

  QSqlQuery query(database);
  if (!query.exec("SELECT version()"))
    throw HootException(QString("Error executing version query: %1").arg(query.lastError().text()));

  if (query.next())
  {
    version = query.value(0).toString();
    if (version.trimmed().isEmpty())
      throw HootException("Error retrieving version.");
  }
  else
    throw HootException("Error retrieving version. Error: " + query.lastError().text());
  query.finish();

  return version;
}

bool DbUtils::isDbUrl(const QString& url)
{
  return isOsmApiDbUrl(url) || isHootApiDbUrl(url);
}

bool DbUtils::isHootApiDbUrl(const QString& url)
{
  return url.startsWith(MetadataTags::HootApiDbScheme() + "://", Qt::CaseInsensitive);
}

bool DbUtils::isOsmApiDbUrl(const QString& url)
{
  return url.startsWith(MetadataTags::OsmApiDbScheme() + "://", Qt::CaseInsensitive);
}

}
