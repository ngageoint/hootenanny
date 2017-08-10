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
