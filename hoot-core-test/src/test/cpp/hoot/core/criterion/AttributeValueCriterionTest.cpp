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
 * @copyright Copyright (C) 2019 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/AttributeValueCriterion.h>

namespace hoot
{

class AttributeValueCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(AttributeValueCriterionTest);
  CPPUNIT_TEST(runNumericEqualToTest);
  CPPUNIT_TEST(runNumericLessThanTest);
  CPPUNIT_TEST(runNumericLessThanOrEqualToTest);
  CPPUNIT_TEST(runNumericGreaterThanTest);
  CPPUNIT_TEST(runNumericGreaterThanOrEqualToTest);
  CPPUNIT_TEST(runTextEqualToTest);
  CPPUNIT_TEST(runTextContainsTest);
  CPPUNIT_TEST(runTextStartsWithTest);
  CPPUNIT_TEST(runTextEndsWithTest);
  CPPUNIT_TEST(runInvalidNumericAttributeTypeTest);
  CPPUNIT_TEST(runInvalidTextAttributeTypeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runNumericEqualToTest()
  {
    AttributeValueCriterion uut(ElementAttributeType::Version, 1.0, NumericComparisonType::EqualTo);
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->setVersion(1);
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->setVersion(2);
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runNumericLessThanTest()
  {
    AttributeValueCriterion uut(ElementAttributeType::Version, 1.0, NumericComparisonType::LessThan);
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->setVersion(0);
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->setVersion(1);
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runNumericLessThanOrEqualToTest()
  {
    AttributeValueCriterion uut(
      ElementAttributeType::Version, 1.0, NumericComparisonType::LessThanOrEqualTo);
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->setVersion(1);
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->setVersion(2);
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runNumericGreaterThanTest()
  {
    AttributeValueCriterion uut(
      ElementAttributeType::Version, 1.0, NumericComparisonType::GreaterThan);
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->setVersion(2);
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->setVersion(1);
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runNumericGreaterThanOrEqualToTest()
  {
    AttributeValueCriterion uut(
      ElementAttributeType::Version, 1.0, NumericComparisonType::GreaterThan);
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->setVersion(2);
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->setVersion(0);
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runTextEqualToTest()
  {
    AttributeValueCriterion uut(
      ElementAttributeType::User, "test1", TextComparisonType::EqualTo);
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->setUser("test1");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->setUser("test2");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runTextContainsTest()
  {
    AttributeValueCriterion uut(
      ElementAttributeType::User, "test", TextComparisonType::Contains);
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->setUser("test1");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->setUser("blah");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runTextStartsWithTest()
  {
    AttributeValueCriterion uut(
      ElementAttributeType::User, "test", TextComparisonType::StartsWith);
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->setUser("test1");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->setUser("1test");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runTextEndsWithTest()
  {
    AttributeValueCriterion uut(
      ElementAttributeType::User, "test", TextComparisonType::EndsWith);
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->setUser("1test");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->setUser("test1");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runInvalidNumericAttributeTypeTest()
  {
    QString exceptionMsg("");

    try
    {
      AttributeValueCriterion uut(ElementAttributeType::Uid, "1", TextComparisonType::EqualTo);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid comparison type"));

    try
    {
      AttributeValueCriterion uut(ElementAttributeType::Version, "1", TextComparisonType::EqualTo);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid comparison type"));
  }

  void runInvalidTextAttributeTypeTest()
  {
    QString exceptionMsg("");

    try
    {
      AttributeValueCriterion uut(
        ElementAttributeType::Timestamp, 1.0, NumericComparisonType::EqualTo);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid comparison type"));

    try
    {
      AttributeValueCriterion uut(ElementAttributeType::User, 1.0, NumericComparisonType::EqualTo);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid comparison type"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AttributeValueCriterionTest, "quick");

}
