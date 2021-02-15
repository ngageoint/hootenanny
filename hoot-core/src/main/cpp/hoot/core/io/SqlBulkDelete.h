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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef SQLBULKDELETE_H
#define SQLBULKDELETE_H

// Qt
#include <QSqlQuery>
#include <QStringList>

#include <hoot/core/io/BulkDelete.h>

class QSqlDatabase;

namespace hoot
{

/**
 * This is designed for combining multiple delete SQL operations into a single delete SQL statement.
 */
class SqlBulkDelete : public BulkDelete
{
public:

  SqlBulkDelete(const QSqlDatabase& db, const QString& tableName);

  virtual ~SqlBulkDelete();

  virtual void flush();

  virtual int getPendingCount() const { return _pending.size(); }

  virtual QString getTableName() const { return _tableName; }

  virtual void deleteElement(const long id);

private:

  QStringList _pending;
  QSqlQuery _query;
  QSqlDatabase _db;
  QString _tableName;
  double _time;
};

}

#endif // SQLBULKDELETE_H
