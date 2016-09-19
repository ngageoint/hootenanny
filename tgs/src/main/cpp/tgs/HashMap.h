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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef __TGS__HASH_MAP__
#define __TGS__HASH_MAP__

// using a #define is a bit of a hack, but it is much simpler than the alternative.
#ifdef _WIN32
# include <hash_map>
# include <hash_set>
# define HashCompare stdext::hash_compare
# define HashMap stdext::hash_map
# define HashSet stdext::hash_set
#else
# undef __DEPRECATED
# include <ext/hash_map>  //hash_map is an SGI extension to STL
# include <ext/hash_set>  //hash_map is an SGI extension to STL
# define HashCompare __gnu_cxx::hash
# define HashMap __gnu_cxx::hash_map
# define HashSet __gnu_cxx::hash_set

#include <QHash>
#include <QString>

#include "SharedPtr.h"

namespace Tgs
{

/**
 * Cantor pairing function:
 * http://en.wikipedia.org/wiki/Cantor_pairing_function#Cantor_pairing_function
 */
template<class T>
size_t cantorPairing(const T& k1, const T& k2)
{
  T pi = ((k1 + k2) / 2) * (k1 + k2 + 1) + k2;
  return __gnu_cxx::hash<T>()(pi);
}

template<class T>
size_t cantorPairing(const std::pair<T, T>& k)
{
  return cantorPairing(k.first, k.second);
}

}

namespace __gnu_cxx
{
/**
 * Explicit template specialization of hash of a string class,
 * which just uses the internal char* representation as a wrapper.
 */
template <>
struct hash<std::string> {
  size_t operator() (const std::string& x) const {
    return hash<const char*>()(x.c_str());
  }
};

template <>
struct hash<QString> {
  size_t operator() (const QString& x) const {
    return qHash(x);
  }
};

template <typename T>
struct hash< boost::shared_ptr<T> > {
  size_t operator() (const boost::shared_ptr<T>& x) const {
    return qHash(x.get());
  }
};

template<>
  struct hash<std::pair<long, long> >
  {
    size_t
    operator()(const std::pair<long, long>& k) const
    { return Tgs::cantorPairing(k); }
  };

template<>
  struct hash<std::pair<unsigned long, unsigned long> >
  {
    size_t
    operator()(const std::pair<unsigned long, unsigned long>& k) const
    { return Tgs::cantorPairing(k); }
  };

template<>
  struct hash<std::pair<int, int> >
  {
    size_t
    operator()(const std::pair<int, int>& k) const
    { return Tgs::cantorPairing(k); }
  };

template<>
  struct hash<std::pair<unsigned int, unsigned int> >
  {
    size_t
    operator()(const std::pair<unsigned int, unsigned int>& k) const
    { return Tgs::cantorPairing(k); }
  };

/**
 * Does a fast, but inexact hash. This should work on all platforms, but not all instances of
 * values that are technically the same. For example, -0.0 and 0.0 will get different hash values
 * even though they're technically the same value.
 */
struct fastHashDouble
{
  size_t
  operator()(const double& k) const
  {
    if (sizeof(double) > sizeof(size_t))
    {
      union { double v; size_t i[2]; } x;
      x.i[0] = 0;
      x.i[1] = 0;
      x.v = k;
      return Tgs::cantorPairing(x.i[0], x.i[1]);
    }
    else
    {
      union { double v; size_t i; } x;
      x.i = 0;
      x.v = k;
      return x.i;
    }
  }
};

template<>
  struct hash<std::pair<double, double> >
  {
    size_t
    operator()(const std::pair<double, double>& k) const
    {
      size_t long1 = fastHashDouble().operator ()(k.first);
      size_t long2 = fastHashDouble().operator ()(k.first);
      return Tgs::cantorPairing(long1, long2);
    }
  };


}
#endif


#endif

