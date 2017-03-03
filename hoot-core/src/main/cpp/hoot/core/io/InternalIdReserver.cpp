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
#include "InternalIdReserver.h"

// Qt
#include <QHash>
#include <QSqlError>
#include <QSqlQuery>

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

InternalIdReserver::InternalIdReserver(QSqlDatabase& db, const QString& sequenceName, int margin) :
  _db(db),
  _margin(margin),
  _sequenceName(sequenceName)
{
  _nextId = 1;
  _closed = false;
}

InternalIdReserver::~InternalIdReserver()
{
  close();
}

void InternalIdReserver::close()
{
  if (!_closed)
  {
    ////
    // 1. Grab a lock so no other InternalIdReservers can stomp on us.
    // 2. If the currval is < then our max value then we know we need to increment the sequence.
    // 3. Increment the sequence and add on margin. The margin is just in case some other process
    //    outside of our control calls NEXTVAL when we aren't expecting it.
    // See the main header for limitations and a more thorough discussion.
    ////
    QSqlQuery lock(_db);
    lock.prepare("SELECT pg_advisory_lock(:seq)");
    lock.bindValue("seq", qHash(_sequenceName));
    if (lock.exec() == false)
    {
      throw HootException("Error acquiring lock. Error: " + lock.lastError().text());
    }

    QSqlQuery query(_db);
    query.prepare(
          QString("SELECT (CASE WHEN NEXTVAL(:seq1) < :maxId\n"
                  "  THEN SETVAL(:seq2, :newValue)\n"
                  "  ELSE 0\n"
                  "END)"));

    query.bindValue("seq1", _sequenceName);
    query.bindValue("maxId", (qlonglong)_nextId);
    query.bindValue("seq2", _sequenceName);
    query.bindValue("newValue", (qlonglong)(_nextId + _margin));
    if (query.exec() == false)
    {
      throw HootException("Error updating sequence. Error: " + query.lastError().text());
    }

    QSqlQuery unlock(_db);
    unlock.prepare("SELECT pg_advisory_unlock(:seq)");
    unlock.bindValue("seq", qHash(_sequenceName));
    if (unlock.exec() == false)
    {
      throw HootException("Error releasing lock. Error: " + unlock.lastError().text());
    }

    _closed = true;
  }
}

int InternalIdReserver::getNextId()
{
  if (_closed)
  {
    throw HootException("This reserver has already been closed.");
  }
  return _nextId++;
}

}
