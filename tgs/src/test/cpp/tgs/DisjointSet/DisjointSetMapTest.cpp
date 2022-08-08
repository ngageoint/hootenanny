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
#include <tgs/StreamUtils.h>
#include <tgs/DisjointSet/DisjointSetMap.h>

namespace Tgs
{

class DisjointSetMapTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(DisjointSetMapTest);
  CPPUNIT_TEST(simpleTest);
  CPPUNIT_TEST(stringTest);
  CPPUNIT_TEST_SUITE_END();

public:

  /**
   * A simple test that mimics the DisjointSet test using the same values for user and index.
   */
  void simpleTest()
  {
    DisjointSetMap<int> uut;

    uut.joinT(0, 1);
    CPPUNIT_ASSERT_EQUAL(0, uut.findT(0));
    uut.joinT(1, 2);
    CPPUNIT_ASSERT_EQUAL(0, uut.findT(0));
    uut.joinT(2, 0);
    CPPUNIT_ASSERT_EQUAL(0, uut.findT(0));
    uut.joinT(3, 4);
    uut.joinT(3, 5);
    uut.joinT(3, 6);
    CPPUNIT_ASSERT_EQUAL(3, uut.findT(4));
    uut.joinT(6, 0);
    CPPUNIT_ASSERT_EQUAL(0, uut.findT(0));
    CPPUNIT_ASSERT_EQUAL(0, uut.findT(3));
    CPPUNIT_ASSERT_EQUAL(0, uut.findT(4));
    CPPUNIT_ASSERT_EQUAL(0, uut.findT(6));
  }

  void stringTest()
  {
    DisjointSetMap<string> uut;
    DisjointSetMap<string>::AllGroups groups;
    stringstream ss;

    uut.joinT("a", "b");
    CPPUNIT_ASSERT_EQUAL(0, uut.findT("a"));
    uut.joinT("b", "c");
    CPPUNIT_ASSERT_EQUAL(0, uut.findT("c"));
    uut.joinT("c", "a");
    CPPUNIT_ASSERT_EQUAL(0, uut.findT("a"));
    uut.joinT("d", "e");

    ss.str("");
    ss << uut.getAllGroups();
    CPPUNIT_ASSERT_EQUAL(string("[2]{(0, [3]{a, b, c}), (3, [2]{d, e})}"), ss.str());

    uut.joinT("d", "f");
    uut.joinT("d", "g");

    ss.str("");
    ss << uut.getAllGroups();
    CPPUNIT_ASSERT_EQUAL(string("[2]{(0, [3]{a, b, c}), (3, [4]{d, e, f, g})}"), ss.str());

    CPPUNIT_ASSERT_EQUAL(3, uut.findT("e"));
    uut.joinT("g", "a");
    CPPUNIT_ASSERT_EQUAL(0, uut.findT("a"));
    CPPUNIT_ASSERT_EQUAL(0, uut.findT("d"));
    CPPUNIT_ASSERT_EQUAL(0, uut.findT("e"));
    CPPUNIT_ASSERT_EQUAL(0, uut.findT("f"));

    ss.str("");
    ss << uut.getAllGroups();
    CPPUNIT_ASSERT_EQUAL(string("[1]{(0, [7]{a, b, c, d, e, f, g})}"), ss.str());
  }

};

}


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::DisjointSetMapTest, "quick");

