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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/RelationMemberUtils.h>

namespace hoot
{

class RelationMemberUtilsTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(RelationMemberUtilsTest);
    CPPUNIT_TEST(runTypeTest);
    CPPUNIT_TEST(runTagKeyTest);
    CPPUNIT_TEST_SUITE_END();

public:

  RelationMemberUtilsTest() = default;

  void runTypeTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    QList<ElementPtr> elements;
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    elements.append(node1);
    // don't add the second node to the elements list
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, 2, geos::geom::Coordinate(0.0, 5.0), 15.0);
    RelationPtr relation = TestUtils::createRelation(map, elements);

    relation->setType("foo");
    CPPUNIT_ASSERT(
      RelationMemberUtils::isMemberOfRelationWithType(map, node1->getElementId(), "foo"));
    CPPUNIT_ASSERT(
      !RelationMemberUtils::isMemberOfRelationWithType(map, node2->getElementId(), "foo"));

    relation->setType("bar");
    CPPUNIT_ASSERT(
      !RelationMemberUtils::isMemberOfRelationWithType(map, node1->getElementId(), "foo"));
    CPPUNIT_ASSERT(
      !RelationMemberUtils::isMemberOfRelationWithType(map, node2->getElementId(), "foo"));
  }

  void runTagKeyTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    QList<ElementPtr> elements;
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    elements.append(node1);
    // don't add the second node to the elements list
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, 2, geos::geom::Coordinate(0.0, 5.0), 15.0);
    RelationPtr relation = TestUtils::createRelation(map, elements);

    relation->setTag("foo", "bar");
    CPPUNIT_ASSERT(
      RelationMemberUtils::isMemberOfRelationWithTagKey(map, node1->getElementId(), "foo"));
    CPPUNIT_ASSERT(
      !RelationMemberUtils::isMemberOfRelationWithTagKey(map, node2->getElementId(), "foo"));

    relation->clear();
    relation->setTag("bar", "baz");
    CPPUNIT_ASSERT(
      !RelationMemberUtils::isMemberOfRelationWithTagKey(map, node1->getElementId(), "foo"));
    CPPUNIT_ASSERT(
      !RelationMemberUtils::isMemberOfRelationWithTagKey(map, node2->getElementId(), "foo"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RelationMemberUtilsTest, "quick");

}
