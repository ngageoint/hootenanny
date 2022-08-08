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
 * @copyright Copyright (C) 2015, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Standard
#include <iostream>
using namespace std;

// TGS
#include <tgs/DisjointSet/DisjointSet.h>

namespace Tgs
{

class DisjointSetTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(DisjointSetTest);
  CPPUNIT_TEST(simpleTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void simpleTest()
  {
    DisjointSet uut(10);

    uut.join(0, 1);
    CPPUNIT_ASSERT_EQUAL(1, uut.find(0));
    uut.join(1, 2);
    CPPUNIT_ASSERT_EQUAL(1, uut.find(0));
    uut.join(2, 0);
    CPPUNIT_ASSERT_EQUAL(1, uut.find(0));
    uut.join(4, 3);
    uut.join(5, 3);
    uut.join(6, 3);
    CPPUNIT_ASSERT_EQUAL(3, uut.find(4));
    uut.join(6, 0);
    CPPUNIT_ASSERT_EQUAL(1, uut.find(0));
    CPPUNIT_ASSERT_EQUAL(1, uut.find(3));
    CPPUNIT_ASSERT_EQUAL(1, uut.find(4));
    CPPUNIT_ASSERT_EQUAL(1, uut.find(6));
  }
};

}


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::DisjointSetTest, "quick");

