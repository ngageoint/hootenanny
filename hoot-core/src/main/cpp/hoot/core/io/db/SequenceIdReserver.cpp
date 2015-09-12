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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SequenceIdReserver.h"

// Qt
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

SequenceIdReserver::SequenceIdReserver(QSqlDatabase& db, const QString& sequenceName) :
  _db(db),
  _sequenceName(sequenceName)
{
  _idsReserved = 0;
  _nextId = 0;
  _bulkSize = 500;
}

int SequenceIdReserver::getNextId()
{
  if (_idsReserved == 0)
  {
    _nextId = _reserveIds();
    _idsReserved = _bulkSize;
  }
  _idsReserved--;
  return _nextId++;
}

long SequenceIdReserver::_reserveIds()
{
  if (_query == 0)
  {
    _query.reset(new QSqlQuery(_db));
    _query->setForwardOnly(true);
    ////
    // This query contains a race condition. It is possible that one process could call SETVAL
    // between the call to CURRVAL and SETVAL. This can result in a smaller value than expected
    // and duplicate IDs. See #3607 for details.
    //
    //LOG_WARN("This query contains a race condition and may not do what you want.");
    _query->prepare(
      QString("SELECT NEXTVAL('%1'), "
              "SETVAL('%1', CURRVAL('%1') + :count)").arg(_sequenceName));
  }

  _query->bindValue(0, (qlonglong)_bulkSize - 1);
  if (_query->exec() == false)
  {
    throw HootException("Error reserving IDs. count: " +
      QString::number(_bulkSize) + " Error: " + _query->lastError().text());
  }

  long result = -1;
  if (_query->next())
  {
    bool ok;
    result = _query->value(1).toLongLong(&ok);
    if (!ok)
    {
      throw HootException("Did not retrieve starting reserved ID.");
    }
  }
  else
  {
    LOG_VARW(_nextId);
    LOG_VARW(result);
    LOG_VARW(_bulkSize);
    throw HootException("Error retrieving sequence value. count: " +
      QString::number(_bulkSize) + " Error: " + _query->lastError().text());
  }

  if (result < _nextId)
  {
    LOG_VARW(_nextId);
    LOG_VARW(result);
    LOG_VARW(_bulkSize);
    QString err = QString("Error allocating new sequence value. Expected to retrieve a value "
                          ">= %1, but got %2.").arg(_nextId).arg(result);
    LOG_WARN(err);
    throw HootException(err);
  }

  _query->finish();

  return result;
}

}
