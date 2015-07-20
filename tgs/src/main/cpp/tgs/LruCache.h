#ifndef LRUCACHE_H
#define LRUCACHE_H

// boost
#include <boost/unordered/unordered_map.hpp>

// standard
#include <list>

#include "HashMap.h"

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
  typedef std::pair<K, V> Entry;
  // back of the list is the most recently used.
  typedef std::list<Entry> CacheList;
  typedef HashMap<K, typename CacheList::iterator> CacheMap;

  LruCache(long maxSize = 1000) : _maxSize(maxSize) {}

  /**
   * Intert a new item. This becomes the most recently used item. After inserting check for
   * eviction.
   */
  void insert(const K& k, const V& v)
  {
    assert(_cacheMap.find(k) == _cache.end());

    _cacheList.push_back(Entry(k, v));
    typename CacheList::iterator e = _cacheList.end();
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
    typename CacheMap::iterator it = _cacheMap.find(k);
    if (it != _cacheMap.end())
    {
      result = true;
      v = it->second->second;
      _cacheList.splice(_cacheList.end(), _cacheList, it->second);
    }

    return result;
  }


private:

  long _maxSize;

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
