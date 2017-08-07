#ifndef SHAREDPTRPOOL_H
#define SHAREDPTRPOOL_H

// Boost
#include <boost/bind.hpp>
#include <boost/pool/object_pool.hpp>

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
    return boost::shared_ptr<T>(_pool.construct(),
      boost::bind(&SharedPtrPool<T>::_destroy, this, _1));
  }

  static SharedPtrPool<T>& getInstance() { return _theInstance; }

private:

  static SharedPtrPool<T> _theInstance;
  boost::object_pool<T> _pool;

  void _destroy(T* v)
  {
    _pool.destroy(v);
  }
};

}

#endif // SHAREDPTRPOOL_H
