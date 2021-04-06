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
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/ChildElementCriterion.h>

namespace hoot
{

class ChildElementCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ChildElementCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    ChildElementCriterion uut(map);

    // Unnecessary for this test but adds a line of test coverage.
    uut.setOsmMap(map.get());

    NodePtr node1(new Node(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0));
    map->addNode(node1);
    CPPUNIT_ASSERT(!uut.isSatisfied(node1));
    NodePtr node2(new Node(Status::Unknown1, 2, geos::geom::Coordinate(0.0, 10.0), 15.0));
    map->addNode(node2);
    CPPUNIT_ASSERT(!uut.isSatisfied(node2));
    NodePtr node3(new Node(Status::Unknown1, 3, geos::geom::Coordinate(0.0, 15.0), 15.0));
    map->addNode(node3);
    CPPUNIT_ASSERT(!uut.isSatisfied(node2));
    NodePtr node4(new Node(Status::Unknown1, 4, geos::geom::Coordinate(0.0, 20.0), 15.0));
    map->addNode(node4);
    CPPUNIT_ASSERT(!uut.isSatisfied(node4));

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
    way1->getTags().set("bridge", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(way1));
    way1->addNode(node1->getId());
    way1->addNode(node2->getId());
    map->addWay(way1);
    CPPUNIT_ASSERT(!uut.isSatisfied(way1));
    CPPUNIT_ASSERT(uut.isSatisfied(node1));
    CPPUNIT_ASSERT(uut.isSatisfied(node2));

    RelationPtr relation1 = std::make_shared<Relation>(Relation(Status::Unknown1, 1));
    relation1->addElement("test", way1->getElementId());
    relation1->addElement("test", node3->getElementId());
    map->addRelation(relation1);
    CPPUNIT_ASSERT(uut.isSatisfied(way1));
    CPPUNIT_ASSERT(uut.isSatisfied(node1));
    CPPUNIT_ASSERT(uut.isSatisfied(node2));
    CPPUNIT_ASSERT(uut.isSatisfied(node3));
    CPPUNIT_ASSERT(!uut.isSatisfied(node4));
    CPPUNIT_ASSERT(!uut.isSatisfied(relation1));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChildElementCriterionTest, "quick");

}
