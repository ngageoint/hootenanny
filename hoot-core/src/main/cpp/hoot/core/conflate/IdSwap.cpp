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
 * @copyright Copyright (C) 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "IdSwap.h"

namespace hoot
{

void IdSwap::add(ElementId id_1, ElementId id_2)
{
  LOG_TRACE("Adding " << id_1.getType().toString() << " " << id_1.getId()
            << " to " << id_2.getType().toString() << " " << id_2.getId());
  if (_idMap.find(id_1) != _idMap.end())
  {
    LOG_DEBUG("ID Swap already contains primary mapping for " << id_1.toString());
    return;
  }
  if (_idMapReversed.find(id_2) != _idMapReversed.end())
  {
    LOG_DEBUG("ID Swap already contains secondary mapping for " << id_2.toString());
    return;
  }
  //  Keep track of both directions 1 -> 2 and 2 -> 1 so that there aren't conflicts
  _idMap[id_1] = id_2;
  _idMapReversed[id_2] = id_1;
}

IdSwap::iterator IdSwap::begin()
{
  //  Begin iterator for the type 'elementType'
  return _idMap.begin();
}

IdSwap::iterator IdSwap::end()
{
  //  End iterator for the type 'elementType'
  return _idMap.end();
}

size_t IdSwap::size() const
{
  //  Total # of elements to be swapped
  return _idMap.size();
}

}
