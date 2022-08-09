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

#include "FilePageStore.h"

#include <errno.h>
#include <tgs/TgsException.h>
#include <tgs/RStarTree/Page.h>

using namespace std;

namespace Tgs
{

  FilePageStore::FilePageStore(int pageSize,  const char * fileName, bool readOnly)
    : _pageSize(pageSize),
      _readOnly(readOnly),
      _bDestructing(false),
      _fileName(fileName)
  {
    if (readOnly == true)
    {
      _pageFile = fopen(fileName, "rb");
      if (_pageFile != nullptr)
      {
        _pageCount = _determinePageCount();
      }
    }
    else
    {
      _pageFile = fopen(fileName, "wb+");
      if (_pageFile != nullptr)
      {
        _pageCount = _determinePageCount();
      }
    }
    if (_pageFile == nullptr)
    {
      throw Tgs::Exception(_getError("Error opening file."));
    }

  }

  FilePageStore::~FilePageStore()
  {
    _bDestructing = true;
    fclose(_pageFile);
  }

  std::shared_ptr<Page> FilePageStore::createPage()
  {
    if (_readOnly == true)
    {
      throw Tgs::Exception("Error creating page -- index is read only.");
    }
    // first create in file
    char * pData = Page::allocateAligned(_pageSize);
    _writePage(_pageCount, pData);
    std::shared_ptr<Page> newPage(_createPage(this, _pageCount, pData,
      _pageSize));

    _pagesMap[_pageCount] = newPage;
    _pageCount++;

    return newPage;
  }

  int FilePageStore::_determinePageCount()
  {
    if (_fseeki64(_pageFile, 0, SEEK_END) == 0)
    {
      __int64 fileSize64 = _ftelli64(_pageFile);
      assert(fileSize64 % (__int64)getPageSize() == 0);
      return (int)(fileSize64 / (__int64)getPageSize());
    }
    else
    {
      throw Tgs::Exception(_getError("Error in determining page count"));
    }
    return 0;
  }

  std::shared_ptr<Page> FilePageStore::getPage(int id)
  {
    // If the page does not resides in memory then get it from file and call createPage()

    if (_pagesMap.find(id) != _pagesMap.end())
    {
      if (_pagesMap[id].expired() != true)
      {
        return _pagesMap[id].lock();
      }
    }

    char * pData = Page::allocateAligned(_pageSize);
    _readPage(id, pData);
    std::shared_ptr<Page> newPage(_createPage(this, id, pData, _pageSize));
    _pagesMap[id] = newPage;
    return newPage;
  }

  int FilePageStore::getPageCount() const
  {
    return _pageCount;
  }

  int FilePageStore::getPageSize() const
  {
    return _pageSize;
  }

  void FilePageStore::flush()
  {
    _pagesMap.clear();
    if (!_readOnly)
    {
      fflush(_pageFile);
    }
  }

  void FilePageStore::save()
  {
    if (!_readOnly)
    {
      PageMap::const_iterator it;
      for (it = _pagesMap.begin(); it != _pagesMap.end(); ++it)
      {
        if (!(*it).second.expired())
        {
          if ((*it).second.lock()->isDirty())
          {
            _writePage((*it).second.lock()->getId(), (*it).second.lock()->getDataRaw());
            _setPageClean((*it).second.lock().get());
          }
        }
      }
      fflush(_pageFile);
    }
    // Just Write out to file
  }

  void FilePageStore::_savePage(int id, char * pData)
  {
    // Just Write out to file
    if (!_bDestructing)
    {
      _writePage(id, pData);
      _pagesMap.erase(_pagesMap.find(id));
    }
  }


  void FilePageStore::_writePage(const int id, char * data)
  {
    if (_readOnly == true)
    {
      throw Tgs::Exception("Error writing page -- index is read only.");
    }

    __int64 i64 = ((__int64)id)*((__int64)_pageSize);
    if (_fseeki64(_pageFile, i64, SEEK_SET) == 0)
    {
      fwrite(data, 1, _pageSize, _pageFile);

      if (ferror(_pageFile)!= 0)
      {
        throw Tgs::Exception(_getError("Error writing page data."));
      }
    }
    else
    {
      throw Tgs::Exception(_getError("Error seeking page data for writing."));
    }

  }

  bool FilePageStore::_readPage(const int id, char * data)
  {
    bool bRet = false;
    char * buffer = new char[_pageSize];
    __int64 i64 = ((__int64)id)*((__int64)_pageSize);
    if (_fseeki64(_pageFile, i64, SEEK_SET) == 0)
    {
      size_t result = fread(buffer, 1, _pageSize, _pageFile);
      if (result == _pageSize)
      {
        memcpy(data, buffer,_pageSize);
        delete [] buffer;
        bRet = true;
      }

      if (ferror(_pageFile)!= 0)
      {
        throw Tgs::Exception(_getError("Error reading page data."));
      }
    }
    else
    {
      throw Tgs::Exception(_getError("Error seeking page data for reading."));
    }

    return bRet;
  }

  std::string FilePageStore::_getError(const std::string& error)
  {
    std::string errStr = error;
    errStr += _fileName;
    errStr += " - ";
    errStr += strerror(errno);

    return errStr;
  }
}
