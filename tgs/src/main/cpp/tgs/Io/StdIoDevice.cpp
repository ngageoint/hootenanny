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
 * @copyright Copyright (C) 2015, 2017, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "StdIoDevice.h"

#include <tgs/TgsException.h>

using namespace std;

namespace Tgs
{

StdIoDevice::StdIoDevice(istream& in) : _in(&in)
{
  open(QIODevice::ReadOnly);
}

bool StdIoDevice::atEnd() const
{
  return _in->eof();
}

qint64 StdIoDevice::readData(char* data, qint64 maxlen)
{
  if (_in == nullptr)
  {
    return 0;
  }
  _in->read(data, maxlen);
  return _in->gcount();
}

qint64 StdIoDevice::writeData(const char *, qint64)
{
  throw Exception("Not Implemented.");
}

}
