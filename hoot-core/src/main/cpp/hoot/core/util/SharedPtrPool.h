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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SHAREDPTRPOOL_H
#define SHAREDPTRPOOL_H

// Boost
#include <boost/bind.hpp>
#include <boost/pool/pool_alloc.hpp>

// Hoot
#include <hoot/core/util/Log.h>

// Standard
#include <deque>

namespace hoot
{

/**
 * A super simple wrapper around boost's object pool and shared pointers. This should provide
 * more efficient allocation/deleting when dealing with large numbers of elements. E.g. nodes.
 */
template <class T>
class SharedPtrPool
{
public:

  SharedPtrPool() {}

  boost::shared_ptr<T> allocate()
  {
    T* v = new (_pool.allocate()) T();

    return boost::shared_ptr<T>(v,
      boost::bind(&SharedPtrPool<T>::_destroy, this, _1));
  }

  static SharedPtrPool<T>& getInstance() { return _theInstance; }

private:

  static SharedPtrPool<T> _theInstance;
  boost::fast_pool_allocator<T> _pool;

  void _destroy(T* v)
  {
    _pool.destroy(v);
    _pool.deallocate(v);
  }
};

}

#endif // SHAREDPTRPOOL_H
