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
 * @copyright Copyright (C) 2015, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */
#include "Sparse2dMatrix.h"

namespace hoot
{

QString Sparse2dMatrix::toString() const
{
  QStringList sl;

  for (auto it = begin(); it != end(); ++it)
    sl << QString("(%1, %2): %3").arg(it->first.row()).arg(it->first.col()).arg(it->second);
  return "{ " + sl.join("\n") + " }";
}

double Sparse2dMatrix::get(const CellId& cid) const
{
  auto it = _map.find(cid);
  if (it != _map.end())
    return it->second;
  else
    return 0.0;
}

}
