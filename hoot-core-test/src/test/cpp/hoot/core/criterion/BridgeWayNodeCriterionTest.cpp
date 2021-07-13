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
#include <hoot/core/criterion/BridgeWayNodeCriterion.h>

namespace hoot
{

class BridgeWayNodeCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(BridgeWayNodeCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    BridgeWayNodeCriterion uut(map);

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    map->addNode(node1);
    CPPUNIT_ASSERT(!uut.isSatisfied(node1));
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, 2, geos::geom::Coordinate(0.0, 10.0), 15.0);
    map->addNode(node2);
    CPPUNIT_ASSERT(!uut.isSatisfied(node2));

    WayPtr way1 = std::make_shared<Way>(Status::Unknown1, 1, 15.0);
    way1->getTags().set("bridge", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(way1));
    way1->addNode(node1->getId());
    way1->addNode(node2->getId());
    map->addWay(way1);
    CPPUNIT_ASSERT(!uut.isSatisfied(way1));
    CPPUNIT_ASSERT(uut.isSatisfied(node1));
    CPPUNIT_ASSERT(uut.isSatisfied(node2));

    uut.setOsmMap(map.get());

    NodePtr node3 =
      std::make_shared<Node>(Status::Unknown1, 3, geos::geom::Coordinate(0.0, 0.0), 15.0);
    map->addNode(node3);
    CPPUNIT_ASSERT(!uut.isSatisfied(node3));
    NodePtr node4 =
      std::make_shared<Node>(Status::Unknown1, 4, geos::geom::Coordinate(0.0, 10.0), 15.0);
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

    NodePtr node5 =
      std::make_shared<Node>(Status::Unknown1, 5, geos::geom::Coordinate(0.0, 0.0), 15.0);
    map->addNode(node5);
    CPPUNIT_ASSERT(!uut.isSatisfied(node5));
    NodePtr node6 =
      std::make_shared<Node>(Status::Unknown1, 6, geos::geom::Coordinate(0.0, 10.0), 15.0);
    map->addNode(node6);
    CPPUNIT_ASSERT(!uut.isSatisfied(node6));

    WayPtr way3 = std::make_shared<Way>(Status::Unknown1, 3, 15.0);
    way3->getTags().set("bridge", "no");
    CPPUNIT_ASSERT(!uut.isSatisfied(way3));
    way3->addNode(node5->getId());
    way3->addNode(node6->getId());
    map->addWay(way3);
    CPPUNIT_ASSERT(!uut.isSatisfied(way3));
    CPPUNIT_ASSERT(!uut.isSatisfied(node5));
    CPPUNIT_ASSERT(!uut.isSatisfied(node5));

    NodePtr node7 =
      std::make_shared<Node>(Status::Unknown1, 7, geos::geom::Coordinate(0.0, 0.0), 15.0);
    map->addNode(node7);
    CPPUNIT_ASSERT(!uut.isSatisfied(node7));
    NodePtr node8(new Node(Status::Unknown1, 8, geos::geom::Coordinate(0.0, 10.0), 15.0));
    map->addNode(node8);
    CPPUNIT_ASSERT(!uut.isSatisfied(node8));

    WayPtr way4 = std::make_shared<Way>(Status::Unknown1, 4, 15.0);
    way4->getTags().set("bridge", "");
    CPPUNIT_ASSERT(!uut.isSatisfied(way4));
    way4->addNode(node7->getId());
    way4->addNode(node8->getId());
    map->addWay(way4);
    CPPUNIT_ASSERT(!uut.isSatisfied(way3));
    CPPUNIT_ASSERT(!uut.isSatisfied(node7));
    CPPUNIT_ASSERT(!uut.isSatisfied(node8));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BridgeWayNodeCriterionTest, "quick");

}
