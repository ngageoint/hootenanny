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
 * @copyright Copyright (C) 2016, 2017, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef BULKDELETE_H
#define BULKDELETE_H

// Qt
#include <QVariant>

namespace hoot
{

/**
 * Interface for bulk deletion of elements from a database
 */
class BulkDelete
{
public:

  virtual ~BulkDelete() {}

  virtual void flush() = 0;

  virtual void deleteElement(const long id) = 0;

  virtual int getPendingCount() const = 0;

  virtual QString getTableName() const = 0;

};

}

#endif // BULKDELETE_H
