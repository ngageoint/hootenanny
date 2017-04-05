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
      QString("Error executing query: %1 (%2)").arg(q.lastError().text()).arg(sql));
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

}
