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
#ifndef IDRESERVER_H
#define IDRESERVER_H

// Tgs
#include <tgs/SharedPtr.h>

// Qt
#include <QSqlDatabase>

namespace hoot
{

/**
 * @todo If it is 2015 and this class still isn't in use, delete it.
 *
 * This class will reserve a number of ids at one time for a given sequence name and then return
 * one at a time. When it runs out another block will be reserved.
 *
 * #############
 * ## CAUTION ##
 * #############
 *
 * This class contains a race condition. If you have multiple threads/processes all reserving ids
 * at the same time (likely) then you can occassionally (like 1 in 20000 bulk reservers on my
 * machine) will result in a bad reservation and possibly duplicate ids being assigned.
 *
 * From this you should read "Don't use me unless you really know what is going on!" I'm leaving
 * the code here in case it is useful in the future when we determine a better approach.
 */
class SequenceIdReserver
{
public:
  SequenceIdReserver(QSqlDatabase& db, const QString& sequenceName);

  int getNextId();

private:

  int _bulkSize;
  QSqlDatabase _db;
  int _idsReserved;
  shared_ptr<QSqlQuery> _query;
  long _nextId;
  QString _sequenceName;

  long _reserveIds();
};

}

#endif // IDRESERVER_H
