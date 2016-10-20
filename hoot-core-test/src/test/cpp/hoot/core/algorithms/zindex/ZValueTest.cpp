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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/geom/Coordinate.h>

// Hoot
#include <hoot/core/algorithms/zindex/ZValue.h>

#include "../../TestUtils.h"

namespace hoot
{

class ZValueTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ZValueTest);
  CPPUNIT_TEST(testBasics);
  CPPUNIT_TEST(testDecompose);
  CPPUNIT_TEST(testGeographic);
  CPPUNIT_TEST(test4D);
  CPPUNIT_TEST_SUITE_END();

public:

  void testBasics()
  {
    vector<double> minV;
    minV.push_back(0.0);
    minV.push_back(0.0);
    vector<double> maxV;
    maxV.push_back(7.0);
    maxV.push_back(7.0);
    ZValue zv(2, 3, minV, maxV);

    vector<long int> test1;
    test1.push_back(0);
    test1.push_back(0);
    CPPUNIT_ASSERT_EQUAL((long int)0, zv.calculate(test1));

    vector<long int> test2;
    test2.push_back(1);
    test2.push_back(0);
    CPPUNIT_ASSERT_EQUAL((long int)1, zv.calculate(test2));

    vector<long int> test3;
    test3.push_back(0);
    test3.push_back(1);
    CPPUNIT_ASSERT_EQUAL((long int)2, zv.calculate(test3));

    vector<long int> test4;
    test4.push_back(1);
    test4.push_back(1);
    CPPUNIT_ASSERT_EQUAL((long int)3, zv.calculate(test4));

    vector<long int> test5;
    test5.push_back(5);
    test5.push_back(7);
    CPPUNIT_ASSERT_EQUAL((long int)59, zv.calculate(test5));

    vector<long int> test6;
    test6.push_back(6);
    test6.push_back(7);
    CPPUNIT_ASSERT_EQUAL((long int)62, zv.calculate(test6));

    vector<long int> test7;
    test7.push_back(7);
    test7.push_back(7);
    CPPUNIT_ASSERT_EQUAL((long int)63, zv.calculate(test7));
    //hex value for 63 is 0x3f. Below is the qt conversion
    //QString hexvalue = QString("%1").arg(zv.calculate(test7), 4, 16, QLatin1Char( '0' ));
  }

  void testDecompose()
  {
    vector<double> minV;
    minV.push_back(0.0);
    minV.push_back(0.0);
    vector<double> maxV;
    maxV.push_back(7.0);
    maxV.push_back(7.0);
    ZValue zv(2, 3, minV, maxV);

    vector<long int> ll;
    ll.push_back(2);
    ll.push_back(2);

    zv.decompose(37, ll);
    CPPUNIT_ASSERT_EQUAL((long int)3, ll[0]);
    CPPUNIT_ASSERT_EQUAL((long int)4, ll[1]);

    zv.decompose(25, ll);
    CPPUNIT_ASSERT_EQUAL((long int)5, ll[0]);
    CPPUNIT_ASSERT_EQUAL((long int)2, ll[1]);
  }

  void testGeographic()
  {
    vector<double> minV;
    minV.push_back(-180.0);
    minV.push_back(-90.0);
    vector<double> maxV;
    maxV.push_back(180.0);
    maxV.push_back(90.0);
    ZValue zv(2, 4, minV, maxV);

    vector<double> test1;
    test1.push_back(-180.0);
    test1.push_back(-90.0);
    CPPUNIT_ASSERT_EQUAL((long int)0, zv.calculate(test1));

    vector<double> test2;
    test2.push_back(0.0);
    test2.push_back(0.0);
    CPPUNIT_ASSERT_EQUAL((long int)192, zv.calculate(test2));

    vector<double> test3;
    test3.push_back(180.0);
    test3.push_back(90.0);
    CPPUNIT_ASSERT_EQUAL((long int)255, zv.calculate(test3));

    vector<double> test4;
    test4.push_back(0.0);
    test4.push_back(90.0);
    CPPUNIT_ASSERT_EQUAL((long int)234, zv.calculate(test4));
  }

  void test4D()
  {
    vector<double> minV;
    minV.push_back(-180.0);
    minV.push_back(-90.0);
    minV.push_back(-180.0);
    minV.push_back(-90.0);
    vector<double> maxV;
    maxV.push_back(180.0);
    maxV.push_back(90.0);
    maxV.push_back(180.0);
    maxV.push_back(90.0);
    ZValue zv(4, 4, minV, maxV);

    vector<double> test1;
    test1.push_back(-180.0);
    test1.push_back(-90.0);
    test1.push_back(-180.0);
    test1.push_back(-90.0);
    CPPUNIT_ASSERT_EQUAL((long int)0, zv.calculate(test1));

    vector<double> test2;
    test2.push_back(180.0);
    test2.push_back(90.0);
    test2.push_back(180.0);
    test2.push_back(90.0);
    CPPUNIT_ASSERT_EQUAL((long int)65535, zv.calculate(test2));

    vector<double> test3;
    test3.push_back(0.0);
    test3.push_back(0.0);
    test3.push_back(0.0);
    test3.push_back(0.0);
    CPPUNIT_ASSERT_EQUAL((long int)61440, zv.calculate(test3));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ZValueTest, "quick");

}

