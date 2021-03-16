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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */


#ifndef __TGS__FILE_PAGE_STORE_H__
#define __TGS__FILE_PAGE_STORE_H__

// Standard Includes
#include <vector>
#include <stdio.h>
#include <string>

#include <tgs/HashMap.h>
#include <tgs/RStarTree/PageStore.h>

namespace Tgs
{
    
/**
 */
class TGS_EXPORT FilePageStore : public PageStore
{
public:
  FilePageStore(int pageSize, const char * fileName, bool readOnly = false);

  virtual ~FilePageStore();

  virtual std::shared_ptr<Page> createPage();

  virtual void flush();

  virtual std::shared_ptr<Page> getPage(int id);

  virtual int getPageCount() const;

  virtual int getPageSize() const;

  virtual void save();

private:

  int _determinePageCount();
  void _writePage(const int, char *);
  bool _readPage(const int, char *);

  int _pageSize;

  using PageMap = HashMap<int, std::weak_ptr<Page>>;
  PageMap _pagesMap;
  bool _readOnly;
  int _pageCount;

  virtual void _savePage(int, char *);
  std::string _getError(const std::string& error);

  bool _bDestructing;

  FILE * _pageFile;

  std::string _fileName;
};

}

#endif
