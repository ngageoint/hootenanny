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
 * @copyright Copyright (C) 2015, 2021 Maxar (http://www.maxar.com/)
 */


// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include <tgs/RStarTree/Box.h>
#include "../PluginFactory.h"

namespace Tgs
{

class BoxTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BoxTest);
  CPPUNIT_TEST(test1);
  CPPUNIT_TEST_SUITE_END();

public:

  void test1()
  {
    Box empty(2);
    CPPUNIT_ASSERT(empty.isValid() == false);
    Box b1(2);
    b1.setBounds(0, 0, 0);
    b1.setBounds(1, 0, 0);
    empty.expand(b1);
    CPPUNIT_ASSERT(empty.isValid() == true);
    CPPUNIT_ASSERT(empty.calculateVolume() == 0.0);

    Box b2(2);
    b2.setBounds(0, 1, 3);
    b2.setBounds(1, 1, 5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, b2.calculateVolume(), 1e-5);

    b2.expand(b1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(15.0, b2.calculateVolume(), 1e-5);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, b2.calculateOverlap(b1), 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(15.0, b2.calculateOverlap(b2), 1e-5);
    Box b3(2);
    b3.setBounds(0, 2, 4);
    b3.setBounds(1, 4, 6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, b3.calculateOverlap(b2), 1e-5);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, b1.getLowerBound(0), 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, b1.getUpperBound(0), 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, b1.getLowerBound(1), 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, b1.getUpperBound(1), 1e-5);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BoxTest, PluginFactory::testName());

}
