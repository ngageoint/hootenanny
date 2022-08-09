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
 * @copyright Copyright (C) 2015, 2017, 2021 Maxar (http://www.maxar.com/)
 */
#include "OgrOptions.h"

namespace hoot
{

OgrOptions::OgrOptions()
  : _cryptic(nullptr)
{
}

OgrOptions::~OgrOptions()
{
  deleteCrypticOptions();
}

void OgrOptions::deleteCrypticOptions()
{
  if (_cryptic)
  {
    for (size_t i = 0; _cryptic[i] != 0; ++i)
      delete[] _cryptic[i];
    delete[] _cryptic;
  }
}

/**
 * Returns an old school C encoding of the options suitable for OGR parameters.
 */
char** OgrOptions::getCrypticOptions()
{
  deleteCrypticOptions();

  if (empty())
    return nullptr;

  _cryptic = new char*[size() + 1];
  _cryptic[size()] = nullptr;
  size_t i = 0;
  for (auto it = constBegin(); it != constEnd(); ++it)
  {
    QByteArray s = (it.key() + "=" + it.value()).toUtf8();
    _cryptic[i] = new char[s.size() + 1];
    memset(_cryptic[i], 0, s.size() + 1);
    strncpy(_cryptic[i], s.data(), s.size());
    ++i;
  }

  return _cryptic;
}

}
