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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QString>

// Standard
#include <iostream>
using namespace std;

#include <tgs/LruCache.h>
#include <tgs/StreamUtils.h>

namespace Tgs
{
  class LruCacheTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(LruCacheTest);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST_SUITE_END();
  public:

    void log(std::map<int, std::list<pair<int, int>>::iterator>& map)
    {
      int n = map.size();
      cout << "[" << n << "]{";

      for (std::map<int, std::list<pair<int, int>>::iterator>::const_iterator it = map.begin(); it != map.end();)
      {
        cout << "(" << it->first << ", " << *(it->second) << ")";
        ++it;
        if (it != map.end())
        {
          cout << ", ";
        }
      }
      cout << "}";
      cout << endl;
    }

    void test1()
    {
      LruCache<int, int> uut(3);

      int v;
      uut.insert(1, 11);
      CPPUNIT_ASSERT_EQUAL(uut.get(1, v), true);
      CPPUNIT_ASSERT_EQUAL(v, 11);
      uut.insert(2, 12);
      CPPUNIT_ASSERT_EQUAL(uut.get(2, v), true);
      CPPUNIT_ASSERT_EQUAL(v, 12);
      CPPUNIT_ASSERT_EQUAL(uut.get(2, v), true);
      CPPUNIT_ASSERT_EQUAL(v, 12);
      uut.insert(3, 13);
      uut.insert(4, 14);
      CPPUNIT_ASSERT_EQUAL(uut.get(1, v), false);
    }
  };
}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::LruCacheTest, "quick");
