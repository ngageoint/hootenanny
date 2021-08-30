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
 * @copyright Copyright (C) 2015, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "IoSingleStat.h"

// Qt
#include <QTextStream>

namespace hoot
{

IoSingleStat::IoSingleStat(Type t)
{
  QString typeString;
  value = 0;

  switch (t)
  {
  case RChar:
    typeString = "rchar";
    name = "Bytes Read";
    break;
  case WChar:
    typeString = "wchar";
    name = "Bytes Written";
    break;
  case SysCr:
    typeString = "syscr";
    name = "Number of Read Syscalls";
    break;
  case SysCw:
    typeString = "syscw";
    name = "Number of Write Syscalls";
    break;
  case ReadBytes:
    typeString = "read_bytes";
    name = "Bytes Read From Storage";
    break;
  case WriteBytes:
    typeString = "write_bytes";
    name = "Bytes Written To Storage";
    break;
  case CancelledWriteBytes:
    typeString = "cancelled_write_bytes";
    name = "Cancelled Write Bytes";
    break;
  default:
    throw IllegalArgumentException("Unexpected type provided.");
  }

  // this only works on linux
  QFile fp("/proc/self/io");

  if (!fp.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    throw HootException("Error opening /proc/self/io for reading. Are you running Linux?");
  }

  bool found = false;
  // If you call atEnd() before a read it returns true. Maybe because it is a proc file? Dunno.
  do
  {
    QString line = QString::fromUtf8(fp.readLine());
    if (line.contains(typeString))
    {
      QStringList l = line.split(":");

      bool ok;
      value = l[1].trimmed().toLong(&ok);
      if (!ok)
      {
        throw HootException(QString("Error parsing %1 from /proc/self/io (%2).").arg(typeString).
          arg(l[1]));
      }
      found = true;
    }
  } while (!fp.atEnd());

  if (!found)
  {
    throw HootException(QString("Could not find (%1) in /proc/self/io.").arg(typeString));
  }
}

}
