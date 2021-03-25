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
 * @copyright Copyright (C) 2018, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/TagKeyContainsCriterion.h>

using namespace geos::geom;

namespace hoot
{

class TagKeyContainsCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(TagKeyContainsCriterionTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    TagKeyContainsCriterion uut;
    NodePtr node(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node->getTags().set("source:phone", "blah");
    node->getTags().set("key2", "blah");

    uut.setText("phone");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    uut.setText("PHONE");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    uut.setText("source:phone");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    uut.setText("source:phone:2");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    uut.setText("something else");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    uut.setCaseSensitive(true);
    uut.setText("PHONE");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagKeyContainsCriterionTest, "quick");

}
