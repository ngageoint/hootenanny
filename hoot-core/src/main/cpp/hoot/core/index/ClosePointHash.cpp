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
 * @copyright Copyright (C) 2015, 2017, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "ClosePointHash.h"

// Standard
#include <assert.h>

using namespace std;

namespace hoot
{

/** init_match is an empty vector used only for the initialization of the
 *  reference ClosePointHash::_match
 */
static vector<long> init_match;

ClosePointHash::ClosePointHash(double distance)
  : _binSize(distance * 2),
    _distance(distance),
    _match(init_match)
{
}

void ClosePointHash::addPoint(double x, double y, long id)
{
  int64_t binIx = _toBin(x + _distance, y + _distance);
  _bins[binIx].emplace_back(id);
  _idTobin[id].emplace_back(binIx);

  binIx = _toBin(x + _distance, y - _distance);
  _bins[binIx].emplace_back(id);
  _idTobin[id].emplace_back(binIx);

  binIx =_toBin(x - _distance, y + _distance);
  _bins[binIx].emplace_back(id);
  _idTobin[id].emplace_back(binIx);

  binIx = _toBin(x - _distance, y - _distance);
  _bins[binIx].emplace_back(id);
  _idTobin[id].emplace_back(binIx);
}

void ClosePointHash::updatePoint(double x, double y, long id)
{
  //  Iterate all bins associated with this ID
  foreach (int64_t binIx, _idTobin[id])
  {
    vector<long>& v = _bins[binIx];
    std::remove(v.begin(), v.end(), id);
  }
  //  Clear out the hashes for this ID
  _idTobin[id].clear();
  //  Add the point at the end
  addPoint(x, y, id);
}

const vector<long>& ClosePointHash::getMatch() const
{
  return _match;
}

vector<long> ClosePointHash::getMatchesFor(long id)
{
  vector<long> ids;

  foreach (int64_t binIx, _idTobin[id])
  {
    ids.insert(ids.end(), _bins[binIx].begin(), _bins[binIx].end());
  }

  // remove duplicates
  sort(ids.begin(), ids.end());
  ids.erase(unique(ids.begin(), ids.end()), ids.end());
  return ids;
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

int64_t ClosePointHash::_toBin(double x, double y) const
{
  uint64_t xi = (uint32_t)(x / _binSize + 0.5);
  uint64_t yi = (uint32_t)(y / _binSize + 0.5);

  return (yi << 32) | xi;
}

}
