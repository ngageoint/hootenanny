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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */


#ifndef __TGS__MEMORY_PAGE_STORE_H__
#define __TGS__MEMORY_PAGE_STORE_H__

// Standard Includes
#include <vector>

#include "PageStore.h"
#include "../TgsExport.h"

namespace Tgs
{
  /**
   * A PageStore that simply stores all pages in memory. Obviously, when this page store is 
   * destroyed all the pages are lost.
   */
  class TGS_EXPORT MemoryPageStore : public PageStore
  {
  public:
    MemoryPageStore(int pageSize);

    virtual ~MemoryPageStore();

    virtual boost::shared_ptr<Page> createPage();

    virtual void flush() {};

    virtual boost::shared_ptr<Page> getPage(int id);

    virtual int getPageCount() const { return (int)_pages.size(); }

    virtual int getPageSize() const;

    virtual void save() {};

  private:

    int _pageSize;
    std::vector<boost::shared_ptr<Page> > _pages;

    virtual void _savePage(int, char *) {};

  };
}

#endif
