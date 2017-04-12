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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef BIGMAPSTL_H
#define BIGMAPSTL_H

// Standard
#include <stdlib.h>
#include <map>

namespace Tgs
{

#warning "Using STL implementation of big map. It works, but not on very large data sets."

template <typename K, typename V>
class BigMap
{
public:
  BigMap() {}

  //constructor to appease --without-stxxl; input parameter is ignored
  BigMap(size_t maxEntriesInRam) {}

  const V& at(const K& k) const { return _map.at(k); }

  bool contains(const K& k) const { return count(k) > 0; }

  size_t count(const K& k) const { return _map.count(k); }

  void insert(const K& k, const V& v) { _map[k] = v; }

  V& operator[](const K& k) { return _map[k]; }

  void clear() { _map.clear(); }

private:
  std::map<K,V> _map;
};

}

#endif // BIGMAPSTL_H
