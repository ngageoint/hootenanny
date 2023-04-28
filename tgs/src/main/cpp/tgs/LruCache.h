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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */
#ifndef LRUCACHE_H
#define LRUCACHE_H

// boost
#include <boost/unordered/unordered_map.hpp>

// standard
#include <list>

// tgs
#include <tgs/HashMap.h>

namespace Tgs
{

/**
 * Least recently used cache. Efficiently maintains a cache of the most recently used items to a
 * maximal specified size. The least recently used item will be dropped if the size becomes too
 * large.
 */
template<typename K, typename V>
class LruCache
{
public:

  using Entry = std::pair<K, V>;
  // back of the list is the most recently used.
  using CacheList = std::list<Entry>;
  using CacheMap = HashMap<K, typename CacheList::iterator>;

  LruCache(long maxSize = 1000) : _maxSize(maxSize) { }

  /**
   * Intert a new item. This becomes the most recently used item. After inserting check for
   * eviction.
   */
  void insert(const K& k, const V& v)
  {
    assert(_cacheMap.find(k) == _cacheMap.end());

    _cacheList.push_back(Entry(k, v));
    auto e = _cacheList.end();
    e--;
    _cacheMap[k] = e;

    _evict();
  }

  /**
   * Get a cached item.
   * @param k User specified key to search for.
   * @param v The value is only populated if the key is found and the function returns true.
   * @return True if the specified key was found, false otherwise.
   */
  bool get(const K& k, V& v)
  {
    bool result = false;
    auto it = _cacheMap.find(k);
    if (it != _cacheMap.end())
    {
      result = true;
      v = it->second->second;
      _cacheList.splice(_cacheList.end(), _cacheList, it->second);
    }

    return result;
  }

  void clear()
  {
    _cacheList.clear();
    _cacheMap.clear();
  }

private:

  size_t _maxSize;

  friend class LruCacheTest;
  CacheList _cacheList;
  CacheMap _cacheMap;

  void _evict()
  {
    while (_cacheList.size() > _maxSize)
    {
      _cacheMap.erase(_cacheList.front().first);
      _cacheList.pop_front();
    }
  }
};

}

#endif // LRUCACHE_H
