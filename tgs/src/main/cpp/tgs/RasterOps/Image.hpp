/****************************************************************************
* Copyright (c) 2005-2008 by SPADAC Inc. (formerly Spatial Data Analytics Corporation).  All rights reserved.
*****************************************************************************/

#ifndef __TGS__IMAGE_HPP__
#define __TGS__IMAGE_HPP__

// Standard
#include <assert.h>
#include <ostream>
#include <vector>

namespace Tgs
{
  /**
  */
  template <class _T>
  class Image
  {
  public:

    Image();
    Image(const Image<_T>& image);
    Image(int width, int height) { resize(width, height); }
    ~Image() = default;

    _T* getData() { return &(_values[0]); }
    int getHeight() const { return _height; }
    int getPixelCount() const { return _values.size(); }
    const std::vector<_T>& getVector() const { return _values; }
    int getWidth() const { return _width; }

    bool isValid() const { return _width > 0 && _height > 0; }

    // this avoids a warning is swig about operator=
#   ifndef SWIG
      Image<_T>& operator=(const Image<_T>& from);
#   endif

    _T& pixel(int x, int y);

    const _T& pixel(int x, int y) const;

    void resize(int x, int y);

    /**
     * Resize the image and set all values to v
     */
    void resize(int x, int y, const _T& v);

    void setVector(const std::vector<_T>& v) { assert(_values.size() == v.size()); _values = v; }

  private:

    int _width, _height;
    std::vector<_T> _values;

    bool _isInBounds(int x, int y) const;

  };

  template<class _T> inline Image<_T>::Image(const Image<_T>& image)
  {
    _width = image._width;
    _height = image._height;
    _values = image._values;
  }

  template<class _T> inline Image<_T>::Image()
  {
    _width = 0;
    _height = 0;
  }

  template<class _T> inline bool Image<_T>::_isInBounds(int x, int y) const
  { 
    return x >= 0 && x < _width && y >= 0 && y < _height; 
  }

  // see declaration for an explanation
# ifndef SWIG
    template<class _T> inline Image<_T>& Image<_T>::operator=(const Image<_T>& from)
    {
      _width = from._width;
      _height = from._height;
      _values = from._values;
      return *this;
    }
# endif

  template<class _T> inline _T& Image<_T>::pixel(int x, int y)
  {
    assert(_isInBounds(x, y));
    return _values[y * _width + x];
  }

  template<class _T> inline const _T& Image<_T>::pixel(int x, int y) const
  {
    assert(_isInBounds(x, y));
    return _values[y * _width + x];
  }

  template<class _T> inline void Image<_T>::resize(int width, int height)
  {
    _width = width;
    _height = height;
    _values.resize(_width * _height);
  }

  template<class _T> inline void Image<_T>::resize(int width, int height, const _T& v)
  {
    _width = width;
    _height = height;
    _values.resize(_width * _height, v);
  }

  template <class _T>
  std::ostream& operator<<(std::ostream & o, const Image<_T>& image)
  {
    o << "[";
    for (int y = 0; y < image.getHeight(); y++)
    {
      o << "[";
      for (int x = 0; x < image.getWidth(); x++)
      {
        o << (x == 0 ? "" : ", ") << image.pixel(x, y);
      }
      o << "]" << std::endl;
    }
    o << "]";
    return o;
  }

}

#ifdef SWIG
%include "Image.i"
#endif

#endif
