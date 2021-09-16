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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "ValidatedOutputCleaner.h"

// Qt
#include <QDirIterator>

namespace hoot
{

int ValidatedOutputCleaner::clean(const QDir& dir)
{
  LOG_STATUS(
    "Deleting validated outputs from root dir: ... " << dir.absolutePath().right(50) << "...");

  int numDeleted = 0;
  QDirIterator itr(dir.absolutePath(), QStringList(), QDir::NoFilter, QDirIterator::Subdirectories);
  while (itr.hasNext())
  {
    const QString input = itr.next();
    if (input.contains("-validated"))
    {
      QDir dir(input);
      if (dir.exists())
      {
        LOG_TRACE("Deleting ... " << dir.dirName().right(50) << "...");
        dir.removeRecursively();
        numDeleted++;
      }
      else
      {
        QFile file(input);
        if (file.exists())
        {
          LOG_TRACE("Deleting ... " << file.fileName().right(50) << "...");
          file.remove();
          numDeleted++;
        }
      }
    }
  }
  return numDeleted;
}

}
