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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef BIGMAPSTXXL_H
#define BIGMAPSTXXL_H

#include "BasicBloomFilter.h"

// Standard
#include <memory>
#include <sstream>

// STXXL
#include <stxxl/bits/mng/diskallocator.h>
#include <stxxl/map>
#include <stxxl/vector>

#include <tgs/TgsConfig.h>
#include <tgs/System/DisableCout.h>
#include <tgs/TgsException.h>

namespace Tgs
{

/**
 * This is not thread safe or re-entrant.
 */
template <typename K, typename V>
class BigMap
{
public:
  BigMap(long nodeSize = 1024 * 1024 * 128, long leafSize = 1024 * 1024 * 32)
  {
    {
      // temporarily disable cout to avoid informational STXXL messages.
      DisableCout d;
      // default to disabling logging, if the environment variables are already set then they won't
      // be overwritting (that is the 0 at the end).
      setenv("STXXLLOGFILE", "/dev/null", 0);
      setenv("STXXLERRLOGFILE", "/dev/null", 0);
      _map.reset(new MapType(nodeSize, leafSize));
    }
    _bloom.reset(new BloomFilter());
    //_map.reset(new MapType(nodeSize, leafSize));
  }

  const V& at(const K& k) const
  {
    // check the least expensive operations first.
#   ifdef DEBUG
      if (_bloom->probably_contains(k) == false)
      {
        std::stringstream ss;
        ss << "The key '" << k << "' was not found in the map.";
        throw Exception(ss.str());
      }
#   endif
    typename MapType::const_iterator it = _map->find(k);
    if (it == _map->end())
    {
      std::stringstream ss;
      ss << "The key '" << k << "' was not found in the map.";
      throw Exception(ss.str());
    }

    return it->second;
  }

  bool contains(const K& k) const { return count(k) > 0; }

  size_t count(const K& k) const
  {
    // check the least expensive operations first.
    if (_bloom->probably_contains(k) == false)
    {
      return 0;
    }
    typename MapType::const_iterator it = _map->find(k);
    return it == _map->end() ? (size_t)0 : (size_t)1;
  }

  void insert(const K& k, const V& v) { _bloom->insert(k); _map->insert(std::pair<K, V>(k, v)); }

  V& operator[](const K& k) { _bloom->insert(k); return _map->operator[](k); }

  void clear() { _map->clear(); }

private:

  template <typename A>
  struct CompareLess
  {
    bool operator() (const A& a, const A& b) const
    {
      return a < b;
    }

    static long max_value() { return std::numeric_limits<A>::max(); }
  };

  typedef stxxl::map<K, V, CompareLess<K>, 4096, 4096> MapType;

  // scaled for ~100M entries and FPR of 0.001
  // http://hur.st/bloomfilter?n=100000000&p=0.001
  typedef boost::bloom_filters::basic_bloom_filter<long, 1437 * 1000 * 1000, boost::mpl::vector<
      boost::bloom_filters::boost_hash<size_t, 0x327B23C66B8B4567>,
      boost::bloom_filters::boost_hash<size_t, 0x66334873643C9869>,
      boost::bloom_filters::boost_hash<size_t, 0x19495CFF74B0DC51>,
      boost::bloom_filters::boost_hash<size_t, 0x625558EC2AE8944A>,
      boost::bloom_filters::boost_hash<size_t, 0x46E87CCD238E1F29>,
      boost::bloom_filters::boost_hash<size_t, 0x507ED7AB3D1B58BA>,
      boost::bloom_filters::boost_hash<size_t, 0x41B71EFB2EB141F2>,
      boost::bloom_filters::boost_hash<size_t, 0x7545E14679E2A9E3>,
      boost::bloom_filters::boost_hash<size_t, 0x5BD062C2515F007C>,
      boost::bloom_filters::boost_hash<size_t, 0x4DB127F812200854> > > BloomFilter;

  std::auto_ptr<MapType> _map;
  std::auto_ptr<BloomFilter> _bloom;
};

}

#endif // BIGMAPSTXXL_H
