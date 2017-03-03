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
#ifndef SQLBULKINSERT_H
#define SQLBULKINSERT_H

// Qt
#include <QSqlQuery>
#include <QStringList>

#include "BulkInsert.h"

class QSqlDatabase;

namespace hoot
{

/**
 * This is designed for combining multiple insert SQL operations into a single insert SQL statement.
 */
class SqlBulkInsert : public BulkInsert
{
public:
  SqlBulkInsert(QSqlDatabase& db, const QString& tableName, const QStringList& columns);

  virtual ~SqlBulkInsert();

  virtual void flush();

  virtual int getPendingCount() const { return _pending.size(); }

  virtual QString getTableName() const { return _tableName; }

  virtual void insert(const QList<QVariant> l);

private:
  QList< QList<QVariant> > _pending;
  QSqlQuery _query;
  QSqlDatabase _db;
  QString _tableName;
  QStringList _columns;
  double _time;
  QString _true, _false;

  inline QString _escape(const QVariant& v) const;
};

}

#endif // SQLBULKINSERT_H
