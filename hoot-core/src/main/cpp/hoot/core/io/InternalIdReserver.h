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
#ifndef INTERNALIDRESERVER_H
#define INTERNALIDRESERVER_H

// Qt
#include <QSqlDatabase>

namespace hoot
{

/**
 * Returns a unique ID (internal to this instance) starting at 1. When the user is done allocating
 * IDs, call close and the specified sequence will be updated such that its value will be >= the
 * max value this reserver created + margin.
 *
 * This is most useful within the current_* tables where the node_id column must be unique for a
 * given map and we know that the services will not be modifying the map at the same time we're
 * creating new IDs. When this is done we have to make sure that services will only create new IDs
 * greater than the max ID we've assigned so we'll update the sequence. Updating the sequence is
 * not atomic, so by adding a margin of 10000 we're pretty much guaranteed that nothing will be
 * stomped on (10000 callers would have to call nextval between our currval and setval calls, very
 * unlikely).
 *
 * Limitations:
 *  - If somebody calls NEXTVAL *margin* times while close() is running then the sequence
 *    value will be too small and you'll get a duplicate ID error in the DB when inserting.
 *    Very unlikely.
 *  - If somebody calls SETVAL at anytime while close() is running all bets are off.
 *  - Whether or not it is needed the sequence will be incremented by at least 1 when close() is
 *    called.
 */
class InternalIdReserver
{
public:
  InternalIdReserver(QSqlDatabase& db, const QString& sequenceName, int margin = 10000);

  ~InternalIdReserver();

  void close();

  int getNextId();

private:

  QSqlDatabase& _db;
  int _margin;
  long _nextId;
  QString _sequenceName;
  bool _closed;
};

}

#endif // INTERNALIDRESERVER_H
