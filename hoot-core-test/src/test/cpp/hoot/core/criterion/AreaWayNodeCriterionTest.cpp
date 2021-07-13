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
#include <hoot/core/criterion/AreaWayNodeCriterion.h>

namespace hoot
{

class AreaWayNodeCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(AreaWayNodeCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    AreaWayNodeCriterion uut(map);

    NodePtr node1(new Node(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0));
    map->addNode(node1);
    CPPUNIT_ASSERT(!uut.isSatisfied(node1));
    NodePtr node2(new Node(Status::Unknown1, 2, geos::geom::Coordinate(0.0, 10.0), 15.0));
    map->addNode(node2);
    CPPUNIT_ASSERT(!uut.isSatisfied(node2));

    WayPtr way1 = std::make_shared<Way>(Status::Unknown1, 1, 15.0);
    way1->getTags().set("area", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(way1));
    way1->addNode(node1->getId());
    way1->addNode(node2->getId());
    map->addWay(way1);
    CPPUNIT_ASSERT(!uut.isSatisfied(way1));
    CPPUNIT_ASSERT(uut.isSatisfied(node1));
    CPPUNIT_ASSERT(uut.isSatisfied(node2));

    NodePtr node3(new Node(Status::Unknown1, 3, geos::geom::Coordinate(0.0, 0.0), 15.0));
    map->addNode(node3);
    CPPUNIT_ASSERT(!uut.isSatisfied(node3));
    NodePtr node4(new Node(Status::Unknown1, 4, geos::geom::Coordinate(0.0, 10.0), 15.0));
    map->addNode(node4);
    CPPUNIT_ASSERT(!uut.isSatisfied(node4));

    WayPtr way2 = std::make_shared<Way>(Status::Unknown1, 2, 15.0);
    CPPUNIT_ASSERT(!uut.isSatisfied(way2));
    way2->addNode(node3->getId());
    way2->addNode(node4->getId());
    map->addWay(way2);
    CPPUNIT_ASSERT(!uut.isSatisfied(way2));
    CPPUNIT_ASSERT(!uut.isSatisfied(node3));
    CPPUNIT_ASSERT(!uut.isSatisfied(node4));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AreaWayNodeCriterionTest, "quick");

}
