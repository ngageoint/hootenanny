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
 * @copyright Copyright (C) 2015, 2018, 2021 Maxar (http://www.maxar.com/)
 */

#include "Page.h"

// Standard
#include <malloc.h>

// Tgs
#include <tgs/RStarTree/PageStore.h>

namespace Tgs
{
  Page::Page(PageStore* parent, int id, char* data, int size) 
  { 
    _id = id; 
    _size = size; 
    _data = data; 
    _dirty = false;
    _parent = parent;
  }

  Page::~Page() 
  {
    if (isDirty())
    {
      _parent->_savePage(getId(), _data);
    }
    freeAligned(_data);
  }

  char* Page::allocateAligned(int size)
  {
    char* result;
#   ifdef _WIN32
      result = (char*)_aligned_malloc(size, 64);
#   else
      result = (char*)memalign(64, size);
#   endif
    return result;
  }

  void Page::freeAligned(char* ptr)
  {
#   ifdef _WIN32
      _aligned_free(ptr);
#   else
      free(ptr);
#   endif
  }


  char* Page::getData()
  {
    return _data; 
  }
}
