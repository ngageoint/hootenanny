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
 * @copyright Copyright (C) 2017, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "TempTestFileName.h"

// hoot
#include <hoot/core/util/Log.h>

// Tgs
#include <tgs/Statistics/Random.h>

// Qt
#include <QFile>

namespace hoot
{

TempTestFileName::TempTestFileName()
{
  do
  {
    int r = Tgs::Random::instance()->generateInt();
    _name = QString("/tmp/HootConflictsTest-%1.conf").arg(r, 10, 10, QChar('0'));
  }
  while (QFile(_name).exists());
}

TempTestFileName::~TempTestFileName()
{
  if (QFile(_name).exists())
  {
    if (!QFile(_name).remove())
    {
      LOG_WARN("Failure removing: " << _name);
    }
  }
}

}
