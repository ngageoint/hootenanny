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
#include "SqlBulkInsert.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QStringBuilder>

// tgs
#include <tgs/System/Time.h>

namespace hoot
{

SqlBulkInsert::SqlBulkInsert(QSqlDatabase& db, const QString &tableName,
  const QStringList &columns) :
    _db(db),
    _tableName(tableName),
    _columns(columns)
{
  _time = 0;
  _true = "TRUE";
  _false = "FALSE";
}

SqlBulkInsert::~SqlBulkInsert()
{
  LOG_DEBUG("(" << _tableName << ") Total time inserting: " << _time);
  if (_pending.size() > 0)
  {
    LOG_WARN("(" << _tableName << ") There are pending inserts in SqlBulkInsert. You should call "
             "flush before destruction.");
  }
}

inline QString SqlBulkInsert::_escape(const QVariant& v) const
{
  switch (v.type())
  {
  case QVariant::Int:
  case QVariant::UInt:
  case QVariant::Double:
  case QVariant::LongLong:
  case QVariant::ULongLong:
    {
      return v.toString();
    }
  case QVariant::String:
    {
      QString result = v.toString();
      if (!result.contains("hstore(ARRAY", Qt::CaseInsensitive) && result != "''") //check tags string return from HootApiDb::_escapeTags(tags)
      {
         result.replace("'", "''");
         result = "'" % result % "'";
      }
      return result;
    }
  case QVariant::Bool:
    {
      return v.toBool() ? _true : _false;
    }
  default:
    throw UnsupportedException();
  }
}

void SqlBulkInsert::flush()
{
  if (_pending.size() > 0)
  {
    double start = Tgs::Time::getTime();
    QString sql;
    // the value 22 was found experimentally
    sql.reserve(_pending.size() * _columns.size() * 22);
    sql.append(QLatin1Literal("INSERT INTO ") %
        _tableName %
        QLatin1Literal(" (") %
        _columns.join(",") %
        QLatin1Literal(") VALUES "));

    QLatin1String firstOpenParen("("), openParen(",("), closeParen(")"), comma(",");

    for (int i = 0; i < _pending.size(); i++)
    {
      if (i == 0)
      {
        sql.append(firstOpenParen);
      }
      else
      {
        sql.append(openParen);
      }

      for (int j = 0; j < _columns.size(); j++)
      {
        if (j == 0)
        {
          sql.append(_escape(_pending[i][j]));
        }
        else
        {
          sql.append(comma % _escape(_pending[i][j]));
        }
      }

      sql.append(closeParen);
    }

    //LOG_VAR(sql);

    //LOG_VAR(sql.size());
    QSqlQuery q(_db);
    if (q.exec(sql) == false)
    {
      throw HootException(QString("Error executing bulk insert: %1 (%2)").arg(q.lastError().text()).
                          arg(sql));
    }

    q.finish();

    _pending.clear();
    double elapsed = Tgs::Time::getTime() - start;
    _time += elapsed;
  }
}

void SqlBulkInsert::insert(const QList<QVariant> l)
{
  if (l.size() != _columns.size())
  {
    LOG_VAR(l);
    LOG_VAR(_columns);
    throw IllegalArgumentException("Expected l to have the same size as _columns.");
  }
  _pending.append(l);
}

}
