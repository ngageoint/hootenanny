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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/elements/Tags.h>
using namespace hoot;

// Qt
#include <QRegExp>

// Tgs
#include <tgs/HashMap.h>

#include "../TestUtils.h"

namespace hoot
{

class TagsTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TagsTest);
  CPPUNIT_TEST(runSplitTest);
  CPPUNIT_TEST(runEqualsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runEqualsTest()
  {
    Tags t1, t2, t3;
    t1.setList("foo", QStringList() << "bar" << "baz");
    t2.setList("foo", QStringList() << "baz" << "bar");
    t3.setList("foo", QStringList() << "bar" << "bar");

    HOOT_STR_EQUALS(true, (t1 == t2));
    HOOT_STR_EQUALS(false, (t1 == t3));
  }

  void runSplitTest()
  {
    HOOT_STR_EQUALS("[4]{a;b, c, foo;bar, baz;qux}", Tags::split("a;;b;c;foo;;bar;baz;;qux"));
    HOOT_STR_EQUALS("[4]{a;b, c, foo;bar, baz;qux}", Tags::split(";a;;b;c;foo;;bar;baz;;qux;"));
    HOOT_STR_EQUALS("[2]{a, b}", Tags::split(";a;b;"));
    HOOT_STR_EQUALS("[2]{a, b}", Tags::split(";a;b"));
    HOOT_STR_EQUALS("[2]{a, b}", Tags::split("a;b;"));
    HOOT_STR_EQUALS("[1]{a}", Tags::split("a"));
    HOOT_STR_EQUALS("[0]{}", Tags::split(""));
    HOOT_STR_EQUALS("[0]{}", Tags::split(";"));

    QStringList l;
    l << "foo;bar" << "baz;qux" << "b";
    Tags t;
    t.setList("a", l);
    HOOT_STR_EQUALS("[3]{foo;bar, baz;qux, b}", t.getList("a"));
    HOOT_STR_EQUALS("foo;;bar;baz;;qux;b", t["a"]);
  }
};

}

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagsTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagsTest, "quick");

