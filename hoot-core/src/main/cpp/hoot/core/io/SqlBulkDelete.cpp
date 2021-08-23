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
 * @copyright Copyright (C) 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "SqlBulkDelete.h"

// hoot
#include <hoot/core/util/HootException.h>


// Qt
#include <QtSql/QSqlDatabase>
#include <QSqlError>
#include <QStringBuilder>

// tgs
#include <tgs/System/Time.h>

namespace hoot
{

SqlBulkDelete::SqlBulkDelete(const QSqlDatabase& db, const QString& tableName) :
_db(db),
_tableName(tableName)
{
  _time = 0;
}

SqlBulkDelete::~SqlBulkDelete()
{
  LOG_DEBUG("(" << _tableName << ") Total time deleting: " << _time);
  if (!_pending.empty())
  {
    LOG_WARN("(" << _tableName << ") There are pending deletes in SqlBulkDelete. You should call "
             "flush before destruction.");
  }
}

void SqlBulkDelete::flush()
{
  LOG_TRACE("Flushing bulk delete...");
  LOG_VART(_pending.size());

  if (!_pending.empty())
  {
    double start = Tgs::Time::getTime();
    QString sql;
    sql.reserve(_pending.size());
    sql.append(QLatin1Literal("DELETE FROM ") %
        _tableName %
        QLatin1Literal(" WHERE id IN (") %
        _pending.join(",") %
        QLatin1Literal(")"));

    LOG_VART(sql);
    QSqlQuery q(_db);
    if (q.exec(sql) == false)
    {
      LOG_ERROR(q.executedQuery());
      LOG_ERROR(q.lastError().text());
      throw HootException(QString("Error executing bulk delete: %1 (%2)").arg(q.lastError().text()).
                          arg(sql.left(500)));
    }

    q.finish();

    _pending.clear();
    double elapsed = Tgs::Time::getTime() - start;
    _time += elapsed;
  }
}

void SqlBulkDelete::deleteElement(const long id)
{
  _pending.append(QString::number(id));
}

}
