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

#include "ClosePointHash.h"

// Standard
#include <assert.h>

namespace hoot
{

ClosePointHash::ClosePointHash(double distance)
{
  _distance = distance;
  _binSize = _distance * 2;
}

void ClosePointHash::addPoint(double x, double y, long id)
{
  _bins[_toBin(x + _distance, y + _distance)].push_back(id);
  _bins[_toBin(x + _distance, y - _distance)].push_back(id);
  _bins[_toBin(x - _distance, y + _distance)].push_back(id);
  _bins[_toBin(x - _distance, y - _distance)].push_back(id);
}

const vector<long>& ClosePointHash::getMatch()
{
  return _match;
}

bool ClosePointHash::next()
{
  while (_it != _bins.end())
  {
    if (_it->second.size() > 1)
    {
      _match = _it->second;
      ++_it;
      return true;
    }
    ++_it;
  }

  return false;
}

void ClosePointHash::resetIterator()
{
  _it = _bins.begin();
}

int64_t ClosePointHash::_toBin(double x, double y)
{
  union
  {
    struct
    {
      int32_t x;
      int32_t y;
    } p;
    int64_t h;
  } i;
  i.p.x = (int32_t)(x / _binSize + 0.5);
  i.p.y = (int32_t)(y / _binSize + 0.5);
  return i.h;
}

}
