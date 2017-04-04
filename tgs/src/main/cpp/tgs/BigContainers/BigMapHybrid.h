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
#ifndef BIGMAPHYBRID_H
#define BIGMAPHYBRID_H

// hackish, I know.
#define BigMap BigMapStxxl
#include "BigMapStxxl.h"
#undef BigMap

// Standard
#include <stdlib.h>
#include <map>

namespace Tgs
{

/**
 * Uses a STL map while the data size is small. When it gets above a threshold it switches over
 * entirely to a BigMapStxxl.
 */
template <typename K, typename V>
class BigMap
{

public:

  BigMap(size_t maxEntriesInRam = 10000000)
  {
    _inMemory = true;
    _maxEntriesInRam = maxEntriesInRam;
  }

  const V& at(const K& k) const
  {
    if (_inMemory)
    {
      return _smallMap.at(k);
    }
    else
    {
      return _bigMap->at(k);
    }
  }

  bool contains(const K& k) const { return count(k) > 0; }

  size_t count(const K& k) const
  {
    if (_inMemory)
    {
      return _smallMap.count(k);
    }
    else
    {
      return _bigMap->count(k);
    }
  }

  void insert(const K& k, const V& v) { operator[](k) = v; }

  V& operator[](const K& k)
  {
    _checkSize();
    if (_inMemory)
    {
      return _smallMap[k];
    }
    else
    {
      return (*_bigMap)[k];
    }
  }

  void clear()
  {
    if (_inMemory)
    {
      return _smallMap.clear();
    }
    else
    {
      return _bigMap->clear();
    }
  }

private:

  bool _inMemory;
  std::map<K,V> _smallMap;
  std::auto_ptr< BigMapStxxl<K,V> > _bigMap;
  size_t _maxEntriesInRam;

  void _checkSize()
  {
    if (_inMemory && _smallMap.size() > _maxEntriesInRam)
    {
      //std::cerr << "Creating stxxl" << std::endl;
      _bigMap.reset(new BigMapStxxl<K,V>());
      for (typename std::map<K, V>::const_iterator it = _smallMap.begin(); it != _smallMap.end();
           ++it)
      {
        _bigMap->insert(it->first, it->second);
      }
      //std::cerr << "Created stxxl" << std::endl;
      _smallMap.clear();
      _inMemory = false;
    }
  }
};

}


#endif // BIGMAPHYBRID_H
