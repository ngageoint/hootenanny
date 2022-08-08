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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__PAGE_STORE_H__
#define __TGS__PAGE_STORE_H__

// Standard
#include <memory>

// Tgs
#include <tgs/RStarTree/Page.h>

namespace Tgs
{

/**
 * PageStore is an abstraction that allows the saving and retrieving of blocks of data. These
 * blocks of data are typically stored out to file, but could conceivably be any form of
 * persistent store.
 */
class TGS_EXPORT PageStore
{
public:

  /**
   * All page shared pointers should be freed before the PageStore is destroyed.
   */
  virtual ~PageStore() = default;

  /**
   * Creates a new page w/ a new id and returns the page. All page shared pointers should be
   * freed before the PageStore is destroyed.
   */
  virtual std::shared_ptr<Page> createPage() = 0;

  /**
   * Saves all dirty pages and frees all memory.
   */
  virtual void flush() = 0;

  /**
   * Returns the page with the given id.
   */
  virtual std::shared_ptr<Page> getPage(int id) = 0;
  /**
   * Returns the number of pages in this page store
   */
  virtual int getPageCount() const = 0;
  /**
   * Returns the size of the pages in bytes.
   */
  virtual int getPageSize() const = 0;

  /**
   * Saves all dirty pages, but doesn't necessarily free the associated memory.
   */
  virtual void save() = 0;

protected:

  Page* _createPage(PageStore* parent, int id, char* data, int size)
  { return new Page(parent, id, data, size); }
  void _setPageClean(Page * p) const {p->_setClean();}

private:

  friend class Page;

  /**
   * Save the page with the specified page ID. This is only called by Page right before it
   * is destructed.
   */
  virtual void _savePage(int id, char *) = 0;
};

}

#endif
