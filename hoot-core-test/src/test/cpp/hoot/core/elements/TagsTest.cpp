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
  CPPUNIT_TEST(unitsTest);
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

  void unitsTest()
  {
    Tags t1, t2, t3, t4, t5;
    t1.setList("maxspeed", QStringList() << "60 mph");
    CPPUNIT_ASSERT_EQUAL(26.82, QString::number(t1.getVelocity("maxspeed"),'g',4).toDouble());

    t2.setList("length", QStringList() << "12'5\"");
    CPPUNIT_ASSERT_EQUAL(3.78, QString::number(t2.getLength("length"), 'g', 3).toDouble());

    t3.setList("length", QStringList() << "60 mi");
    CPPUNIT_ASSERT_EQUAL(96560.6, QString::number(t3.getLength("length"), 'g', 6).toDouble());

    t4.setList("length", QStringList() << "10 nmi");
    CPPUNIT_ASSERT_EQUAL(18520.0, QString::number(t4.getLength("length"), 'g', 6).toDouble());

    t5.setList("maxspeed", QStringList() << "10 knots");
    CPPUNIT_ASSERT_EQUAL(5.14, QString::number(t5.getVelocity("maxspeed"), 'g', 3).toDouble());
  }
};

}

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagsTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagsTest, "quick");

