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

#ifndef __TGS__PAGE_H__
#define __TGS__PAGE_H__

#include <tgs/TgsExport.h>

namespace Tgs
{

class PageStore;

/**
* This class is tightly linked with the PageStore and a circular reference should be expected.
* I believe that in this instance it is acceptable.
*/
class TGS_EXPORT Page
{
public:

  ~Page();

  /**
  * Returns the identifier for this Page. This ID is guaranteed to be unique to the PageStore
  * that contains this page.
  */
  int getId() const { return _id; }

  /**
  * Returns the data contained by this page store. It is expected that this data will be cast
  * to the appropriate type before operations are performed. You are almost guaranteed that this
  * will not actually be a null terminated string.
  */
  char* getData();

  /**
  * Returns the data size in bytes.
  */
  int getDataSize() const { return _size; }

  /**
  * Returns true if this data has changed since it was last loaded.
  */
  bool isDirty() const { return _dirty; }

  /**
  * Call this when the data is changed. If this method is not called the data may not be saved
  * appropriately.
  */
  void setDirty() { _dirty = true; }

  char * getDataRaw(){return _data;}

  PageStore* getPageStore(){return _parent;}

  static char* allocateAligned(int size);

  static void freeAligned(char* ptr);

private:
  friend class PageStore;

  /**
  * Constructs the page and initializes it w/ it's id, data and size. The page will take
  * ownership of the data.
  */
  Page(PageStore* parent, int id, char* data, int size);

  /**
  * Called by the PageStore when this page has been saved out as persistent data.
  */
  void _setClean() { _dirty = false; }

  int _id;
  int _size;
  char* _data;
  bool _dirty;
  PageStore* _parent;
};

}

#endif
