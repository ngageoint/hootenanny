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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include <tgs/RStarTree/HilbertCurve.h>

// Standard Includes
#include <iostream>

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include "../PluginFactory.h"

using namespace std;
using namespace Tgs;

class HilbertCurveTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(HilbertCurveTest);
  CPPUNIT_TEST(test1);
  CPPUNIT_TEST(sanityCheck);
  CPPUNIT_TEST(sanityCheck2);
  CPPUNIT_TEST_SUITE_END();

public:

  void test1()
  {
    int values2[] = {0, 3, 4, 5, 1, 2, 7, 6, 14, 13, 8, 9, 15, 12, 11, 10};
    int values3[] = {
       0,  1, 14, 15, 16, 19, 20, 21,
       3,  2, 13, 12, 17, 18, 23, 22,
       4,  7,  8, 11, 30, 29, 24, 25,
       5,  6,  9, 10, 31, 28, 27, 26,
      58, 57, 54, 53, 32, 35, 36, 37,
      59, 56, 55, 52, 33, 34, 39, 38,
      60, 61, 50, 51, 46, 45, 40, 41,
      63, 62, 49, 48, 47, 44, 43, 42
      };

    _testCurve(2, 2, values2);
    _testCurve(2, 3, values3);
  }

  void sanityCheck()
  {
    std::set<int> values;

    int order = 7;
    HilbertCurve uut(2, order);
    int orderBorder = 1 << order;
    for (int x = 0; x < orderBorder; x++)
    {
      for (int y = 0; y < orderBorder; y++)
      {
        int a[] = {x, y};
        int r = uut.encode(a);
        CPPUNIT_ASSERT(r >= 0 && r < orderBorder * orderBorder);
        CPPUNIT_ASSERT(values.find(r) == values.end());
        values.insert(r);
      }
    }
  }

  void sanityCheck2()
  {
    std::set<int64_t> values;

    int order = 31;
    HilbertCurve uut(2, order);
    int64_t orderBorder = 1 << order;
    for (int x = 0; x < orderBorder; x += (orderBorder / 877))
    {
      for (int y = 0; y < orderBorder; y += (orderBorder / 997))
      {
        int a[] = {x, y};
        int64_t r = uut.encode(a);
        CPPUNIT_ASSERT(r >= 0 && r < orderBorder * orderBorder);
        CPPUNIT_ASSERT(values.find(r) == values.end());
        values.insert(r);
      }
    }
  }

  void _testCurve(int dimensions, int order, int values[])
  {
    HilbertCurve uut(dimensions, order);
    int orderBorder = 1 << order;
    for (int x = 0; x < orderBorder; x++)
    {
      for (int y = 0; y < orderBorder; y++)
      {
        int a[] = {x, y};
        int r = uut.encode(a);
        CPPUNIT_ASSERT_EQUAL(values[y + x * orderBorder], r);
      }
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HilbertCurveTest, PluginFactory::testName());
