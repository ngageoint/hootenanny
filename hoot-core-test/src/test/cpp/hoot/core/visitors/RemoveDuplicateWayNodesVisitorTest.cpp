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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/visitors/RemoveDuplicateWayNodesVisitor.h>

#include "../TestUtils.h"

namespace hoot
{

class RemoveDuplicateWayNodesVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RemoveDuplicateWayNodesVisitorTest);
  CPPUNIT_TEST(runInvalidWayTest1);
  CPPUNIT_TEST(runInvalidWayTest2);
  CPPUNIT_TEST(runValidStartAndEndNodeSameTest);
  CPPUNIT_TEST(runValidUnclosedWayTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runInvalidWayTest1()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, 1, 0, 0, 0));
    map->addNode(node1);
    NodePtr node2(new Node(Status::Unknown1, 2, 0, 0, 0));
    map->addNode(node2);
    NodePtr node3(new Node(Status::Unknown1, 3, 0, 0, 0));
    map->addNode(node3);

    WayPtr way(new Way(Status::Unknown1, 1, 15.0));
    std::vector<long> nodeIds;
    nodeIds.push_back(1);
    nodeIds.push_back(2);
    nodeIds.push_back(3); //invalid
    nodeIds.push_back(3);
    way->setNodes(nodeIds);
    map->addWay(way);

    RemoveDuplicateWayNodesVisitor v;
    map->visitRw(v);

    //the third node ref should have been removed from the way, but that node should still be in
    //the map
    CPPUNIT_ASSERT_EQUAL((size_t)3, way->getNodeIds().size());
    CPPUNIT_ASSERT_EQUAL(1L, way->getNodeIds().at(0));
    CPPUNIT_ASSERT_EQUAL(2L, way->getNodeIds().at(1));
    CPPUNIT_ASSERT_EQUAL(3L, way->getNodeIds().at(2));
    CPPUNIT_ASSERT(map->containsNode(1));
    CPPUNIT_ASSERT(map->containsNode(2));
    CPPUNIT_ASSERT(map->containsNode(3));
  }

  void runInvalidWayTest2()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, 1, 0, 0, 0));
    map->addNode(node1);
    NodePtr node2(new Node(Status::Unknown1, 2, 0, 0, 0));
    map->addNode(node2);
    NodePtr node3(new Node(Status::Unknown1, 3, 0, 0, 0));
    map->addNode(node3);

    WayPtr way(new Way(Status::Unknown1, 1, 15.0));
    std::vector<long> nodeIds;
    nodeIds.push_back(1);
    nodeIds.push_back(1); //invalid
    nodeIds.push_back(2);
    nodeIds.push_back(3);
    way->setNodes(nodeIds);
    map->addWay(way);

    RemoveDuplicateWayNodesVisitor v;
    map->visitRw(v);

    //the second node ref should have been removed from the way, but that node should still be in
    //the map
    CPPUNIT_ASSERT_EQUAL((size_t)3, way->getNodeIds().size());
    CPPUNIT_ASSERT_EQUAL(1L, way->getNodeIds().at(0));
    CPPUNIT_ASSERT_EQUAL(2L, way->getNodeIds().at(1));
    CPPUNIT_ASSERT_EQUAL(3L, way->getNodeIds().at(2));
    CPPUNIT_ASSERT(map->containsNode(1));
    CPPUNIT_ASSERT(map->containsNode(2));
    CPPUNIT_ASSERT(map->containsNode(3));
  }

  void runValidStartAndEndNodeSameTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, 1, 0, 0, 0));
    map->addNode(node1);
    NodePtr node2(new Node(Status::Unknown1, 2, 0, 0, 0));
    map->addNode(node2);
    NodePtr node3(new Node(Status::Unknown1, 3, 0, 0, 0));
    map->addNode(node3);

    WayPtr way(new Way(Status::Unknown1, 1, 15.0));
    std::vector<long> nodeIds;
    nodeIds.push_back(1);
    nodeIds.push_back(2);
    nodeIds.push_back(3);
    nodeIds.push_back(1);
    way->setNodes(nodeIds);
    map->addWay(way);

    RemoveDuplicateWayNodesVisitor v;
    map->visitRw(v);

    //no nodes should be removed from the way or map
    CPPUNIT_ASSERT_EQUAL((size_t)4, way->getNodeIds().size());
    CPPUNIT_ASSERT_EQUAL(1L, way->getNodeIds().at(0));
    CPPUNIT_ASSERT_EQUAL(2L, way->getNodeIds().at(1));
    CPPUNIT_ASSERT_EQUAL(3L, way->getNodeIds().at(2));
    CPPUNIT_ASSERT_EQUAL(1L, way->getNodeIds().at(3));
    CPPUNIT_ASSERT(map->containsNode(1));
    CPPUNIT_ASSERT(map->containsNode(2));
    CPPUNIT_ASSERT(map->containsNode(3));
  }

  void runValidUnclosedWayTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, 1, 0, 0, 0));
    map->addNode(node1);
    NodePtr node2(new Node(Status::Unknown1, 2, 0, 0, 0));
    map->addNode(node2);
    NodePtr node3(new Node(Status::Unknown1, 3, 0, 0, 0));
    map->addNode(node3);
    NodePtr node4(new Node(Status::Unknown1, 4, 0, 0, 0));
    map->addNode(node4);

    WayPtr way(new Way(Status::Unknown1, 1, 15.0));
    std::vector<long> nodeIds;
    nodeIds.push_back(1);
    nodeIds.push_back(2);
    nodeIds.push_back(3);
    nodeIds.push_back(4);
    way->setNodes(nodeIds);
    map->addWay(way);

    RemoveDuplicateWayNodesVisitor v;
    map->visitRw(v);

    //no nodes should be removed from the way or map
    CPPUNIT_ASSERT_EQUAL((size_t)4, way->getNodeIds().size());
    CPPUNIT_ASSERT_EQUAL(1L, way->getNodeIds().at(0));
    CPPUNIT_ASSERT_EQUAL(2L, way->getNodeIds().at(1));
    CPPUNIT_ASSERT_EQUAL(3L, way->getNodeIds().at(2));
    CPPUNIT_ASSERT_EQUAL(4L, way->getNodeIds().at(3));
    CPPUNIT_ASSERT(map->containsNode(1));
    CPPUNIT_ASSERT(map->containsNode(2));
    CPPUNIT_ASSERT(map->containsNode(3));
    CPPUNIT_ASSERT(map->containsNode(4));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveDuplicateWayNodesVisitorTest, "quick");

}


