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
 * @copyright Copyright (C) 2006 Pedro Felzenszwalb
 * @copyright Copyright (C) 2015, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef DISJOINTMAP_H
#define DISJOINTMAP_H

// Standard
#include <deque>
#include <vector>

// tgs
#include <tgs/HashMap.h>

namespace Tgs
{

/**
 * Similar to the DisjointSet, but can use a class other than an int for defining groups.
 *
 * taken from:
 * http://cs.brown.edu/~pff/segment/disjoint-set forests using union-by-rank and path compression
 * (sort of).
 */
template<class T>
class DisjointSetMap
{
public:
  using UserMap = typename HashMap<T, int>;
  using AllGroups = HashMap<int, std::vector<T>>;

  void clear()
  {
    _e.clear();
    _tMap.clear();
    _lastGroup.clear();
  }

  /**
   * Find the group for a given x. The group is an internal group ID. This is useful for comparison.
   * E.g. if (ds.findT("A") == ds.findT("B")) then they're in the same group.
   */
  int find(int x);
  int findT(const T& x) { return find(toIndex(x)); }

  const AllGroups& getAllGroups()
  {
    _lastGroup.clear();
    for (size_t i = 0; i < _e.size(); i++)
    {
      int p = find(static_cast<int>(i));
      _lastGroup[p].push_back(_e[i].user);
    }
    return _lastGroup;
  }

  void join(int x, int y);
  void joinT(const T& x, const T& y)
  {
    int ix = toIndex(x);
    int iy = toIndex(y);
    join(ix, iy);
  }

  int size() const { return static_cast<int>(_e.size()); }
  int toIndex(const T& x)
  {
    int result;
    typename UserMap::const_iterator it = _tMap.find(x);
    if (it == _tMap.end())
    {
      result = static_cast<int>(_e.size());
      _e.push_back(Element(x, result));
      _tMap[x] = result;
    }
    else
      result = it->second;
    return result;
  }

  const T& toUser(int x) const { return _e[x].user; }

private:

  struct Element
  {
    Element(T user_, int p_)
    {
      user = user_;
      p = p_;
    }

    T user;
    int p;
  };

  std::deque<Element> _e;
  UserMap _tMap;
  AllGroups _lastGroup;
};

template<class T>
inline int DisjointSetMap<T>::find(int x)
{
  int y = x;
  while (y != _e[y].p)
    y = _e[y].p;
  _e[x].p = y;
  return y;
}

template<class T>
inline void DisjointSetMap<T>::join(int x, int y)
{
  x = find(x);
  y = find(y);
  if (x < y)
    _e[y].p = x;
  else
    _e[x].p = y;
}

}

#endif // DISJOINTMAP_H
