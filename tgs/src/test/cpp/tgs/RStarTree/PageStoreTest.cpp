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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Tgs
#include <tgs/RStarTree/FilePageStore.h>
#include <tgs/RStarTree/MemoryPageStore.h>
#include <tgs/RStarTree/Page.h>

// CPPUnit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QFile>

#include "../PluginFactory.h"

namespace Tgs
{

class PageStoreTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PageStoreTest);
  CPPUNIT_TEST(fileTest);
  CPPUNIT_TEST(memoryTest);
  CPPUNIT_TEST_SUITE_END();
public:

  std::vector<int> _ids;

  void fileTest()
  {
    if (QFile::exists("dum.dat"))
    {
      QFile::remove("dum.dat");
    }
    {
      FilePageStore uut(100, "dum.dat");

      CPPUNIT_ASSERT_EQUAL(100, uut.getPageSize());

      testPageStore(uut);
    }
    {
      FilePageStore uut(100, "dum.dat", true);

      for (int i = 0; i < 100; i++)
      {
        std::shared_ptr<Page> p = uut.getPage(_ids[i]);
        verifyPage(p);
      }
    }
  }

  void memoryTest()
  {
    MemoryPageStore uut(100);

    CPPUNIT_ASSERT_EQUAL(100, uut.getPageSize());
    
    testPageStore(uut);
  }

  void populatePage(const std::shared_ptr<Page>& p)
  {
    char* data = p->getData();
    for (int i = 0; i < p->getDataSize(); i++)
    {
      data[i] = i ^ p->getId();
    }
    p->setDirty();
  }

  void testPageStore(PageStore& uut)
  {
    CPPUNIT_ASSERT_EQUAL(0, uut.getPageCount());
    int pageSize = uut.getPageSize();

    std::shared_ptr<Page> p = uut.createPage();
    CPPUNIT_ASSERT_EQUAL(pageSize, p->getDataSize());

    populatePage(p);
    verifyPage(p);

    
    _ids.push_back(p->getId());

    for (int i = 1; i < 100; i++)
    {
      CPPUNIT_ASSERT_EQUAL(i, uut.getPageCount());
      p = uut.createPage();
      populatePage(p);
      _ids.push_back(i);
    }
    CPPUNIT_ASSERT_EQUAL(100, uut.getPageCount());

    for (int i = 0; i < 100; i++)
    {
      p = uut.getPage(_ids[i]);
      verifyPage(p);
    }

    uut.save();

    for (int i = 0; i < 100; i++)
    {
      p = uut.getPage(_ids[i]);
      verifyPage(p);
    }

    uut.flush();

    for (int i = 0; i < 100; i++)
    {
      p = uut.getPage(_ids[i]);
      verifyPage(p);
    }
  }

  void verifyPage(const std::shared_ptr<Page>& p)
  {
    char* data = p->getData();
    for (int i = 0; i < p->getDataSize(); i++)
    {
      CPPUNIT_ASSERT_EQUAL((char)(i ^ p->getId()), data[i]);
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PageStoreTest, PluginFactory::testName());

}
