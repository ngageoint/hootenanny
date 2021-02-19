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
#include <hoot/core/criterion/TagValueNumericRangeCriterion.h>

using namespace geos::geom;

namespace hoot
{

class TagValueNumericRangeCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(TagValueNumericRangeCriterionTest);
  CPPUNIT_TEST(runInRangeTest);
  CPPUNIT_TEST(runAboveRangeTest);
  CPPUNIT_TEST(runBelowRangeTest);
  CPPUNIT_TEST(runKeyDoesntExistTest);
  CPPUNIT_TEST(runNonNumericValTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runInRangeTest()
  {
    NodePtr node(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node->getTags().set("key1", "5");
    node->getTags().set("key2", "5");
    node->getTags().set("key3", "6");

    QStringList keys;
    keys.append("key1");
    keys.append("key2");
    TagValueNumericRangeCriterion uut(keys, 4, 5);
    CPPUNIT_ASSERT(uut.isSatisfied(node));
  }

  void runAboveRangeTest()
  {
    NodePtr node(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node->getTags().set("key1", "5");
    node->getTags().set("key2", "4");
    node->getTags().set("key3", "6");

    QStringList keys;
    keys.append("key1");
    keys.append("key2");
    TagValueNumericRangeCriterion uut(keys, 2, 4);
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runBelowRangeTest()
  {
    NodePtr node(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node->getTags().set("key1", "8");
    node->getTags().set("key2", "4");
    node->getTags().set("key3", "6");

    QStringList keys;
    keys.append("key1");
    keys.append("key2");
    TagValueNumericRangeCriterion uut(keys, 6, 8);
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runKeyDoesntExistTest()
  {
    NodePtr node(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node->getTags().set("key1", "8");
    node->getTags().set("key2", "4");
    node->getTags().set("key3", "6");

    QStringList keys;
    keys.append("blah");
    TagValueNumericRangeCriterion uut(keys, LONG_MIN, LONG_MAX);
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runNonNumericValTest()
  {
    NodePtr node(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node->getTags().set("key1", "blah");
    node->getTags().set("key2", "5");
    node->getTags().set("key3", "6");

    QStringList keys;
    keys.append("key1");
    keys.append("key2");
    TagValueNumericRangeCriterion uut(keys, 4, 5);
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagValueNumericRangeCriterionTest, "quick");

}
