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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/conflate/NoInformationElementRemover.h>
#include <hoot/core/OsmMap.h>
using namespace hoot;


// Boost
using namespace boost;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>

#include "../TestUtils.h"

namespace hoot
{

class NoInformationElementRemoverTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(NoInformationElementRemoverTest);
  CPPUNIT_TEST(runWayWithInfoOneNodeWithoutInfoTest);
  CPPUNIT_TEST(runStandAloneNodesWithAndWithoutInfoTest);
  CPPUNIT_TEST(runWayWithoutInfoOneNodeWithInfoTest);
  CPPUNIT_TEST(runWayWithoutInfoAllNodesWithoutInfoTest);
  CPPUNIT_TEST(runRelationWithInfoOneElementWithoutInfoTest);
  CPPUNIT_TEST(runRelationWithoutInfoOneNodeElementWithInfoTest);
  CPPUNIT_TEST(runRelationWithoutInfoOneWayElementWithInfoTest);
  CPPUNIT_TEST(runRelationWithoutInfoAllElementsWithoutInfoTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runWayWithInfoOneNodeWithoutInfoTest()
  {
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());

    QList<NodePtr> nodes;
    NodePtr node1(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node1->getTags().appendValue("hoot:test", "test1");
    node1->getTags().appendValue("test", "test1");
    nodes.append(node1);
    NodePtr node2(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node2->getTags().appendValue("hoot:test", "test2");
    nodes.append(node2);

    WayPtr way = TestUtils::createWay(map, nodes);
    way->getTags().appendValue("hoot:test", "test1");
    way->getTags().appendValue("test", "test1");

    NoInformationElementRemover().apply(map);

    //No elements should be removed.  Node 2 should not be removed, even though it has no info,
    //since its still owned by the way.

    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());

    WayPtr parsedWay = map->getWay(way->getElementId());
    CPPUNIT_ASSERT_EQUAL(2, (int)parsedWay->getNodeCount());
  }

  void runStandAloneNodesWithAndWithoutInfoTest()
  {
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node1->getTags().appendValue("hoot:test", "test1");
    node1->getTags().appendValue("test", "test1");
    map->addNode(node1);
    NodePtr node2(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node2->getTags().appendValue("hoot:test", "test2");
    map->addNode(node2);

    NoInformationElementRemover().apply(map);

    //Node 2 has no info and isn't owned by a way, so it should be removed.

    CPPUNIT_ASSERT_EQUAL(1, (int)map->getNodes().size());

    boost::shared_ptr<const Node> parsedNode1 = map->getNode(node1->getElementId().getId());
    const Tags& node1Tags = parsedNode1->getTags();
    CPPUNIT_ASSERT_EQUAL(2, node1Tags.size());
    CPPUNIT_ASSERT_EQUAL(QString("test1"), node1Tags.get("hoot:test"));
    CPPUNIT_ASSERT_EQUAL(QString("test1"), node1Tags.get("test"));

    CPPUNIT_ASSERT(!map->containsNode(node2->getElementId().getId()));
  }

  void runWayWithoutInfoOneNodeWithInfoTest()
  {
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());

    QList<NodePtr> nodes;
    NodePtr node1(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node1->getTags().appendValue("hoot:test", "test1");
    node1->getTags().appendValue("test", "test1");
    nodes.append(node1);
    NodePtr node2(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node2->getTags().appendValue("hoot:test", "test2");
    nodes.append(node2);

    WayPtr way = TestUtils::createWay(map, nodes);
    way->getTags().appendValue("hoot:test", "test1");

    NoInformationElementRemover().apply(map);

    //The way owning both nodes has no info and should be removed.  Since the nodes are not
    //standalone and are part of a way being removed, they should be removed as well.

    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
  }

  void runWayWithoutInfoAllNodesWithoutInfoTest()
  {
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());

    QList<NodePtr> nodes;
    NodePtr node1(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node1->getTags().appendValue("hoot:test", "test1");
    nodes.append(node1);
    NodePtr node2(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node2->getTags().appendValue("hoot:test", "test2");
    nodes.append(node2);

    WayPtr way1 = TestUtils::createWay(map, nodes);
    way1->getTags().appendValue("hoot:test", "test");

    NoInformationElementRemover().apply(map);

    //Neither the way or its nodes have info, so all should be removed.

    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
  }

  void runRelationWithInfoOneElementWithoutInfoTest()
  {
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());

    QList<ElementPtr> elements;

    QList<NodePtr> nodes;
    NodePtr node1(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node1->getTags().appendValue("hoot:test", "test1");
    nodes.append(node1);
    elements.append(node1);

    NodePtr node2(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node2->getTags().appendValue("hoot:test", "test2");
    node1->getTags().appendValue("test", "test2");
    nodes.append(node2);
    elements.append(node2);

    WayPtr way1 = TestUtils::createWay(map, nodes);
    way1->getTags().appendValue("hoot:test", "test1");
    way1->getTags().appendValue("test", "test1");
    elements.append(way1);

    WayPtr way2 = TestUtils::createWay(map, nodes);
    way2->getTags().appendValue("hoot:test", "test2");
    elements.append(way2);

    RelationPtr relation = TestUtils::createRelation(map, elements);
    relation->getTags().appendValue("hoot:test", "test");
    relation->getTags().appendValue("test", "test");

    NoInformationElementRemover().apply(map);

    //No elements should be removed.  The node 1 and way 2 should not be removed, even though they
    //have no info, since they are still owned by the relation.

    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getRelations().size());
  }

  void runRelationWithoutInfoOneNodeElementWithInfoTest()
  {
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());

    QList<ElementPtr> elements;

    QList<NodePtr> nodes;
    NodePtr node1(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node1->getTags().appendValue("hoot:test", "test1");
    nodes.append(node1);
    elements.append(node1);

    NodePtr node2(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node2->getTags().appendValue("hoot:test", "test2");
    node1->getTags().appendValue("test", "test2");
    nodes.append(node2);
    elements.append(node2);

    WayPtr way1 = TestUtils::createWay(map, nodes);
    way1->getTags().appendValue("hoot:test", "test1");
    elements.append(way1);

    WayPtr way2 = TestUtils::createWay(map, nodes);
    way2->getTags().appendValue("hoot:test", "test2");
    elements.append(way2);

    RelationPtr relation = TestUtils::createRelation(map, elements);
    relation->getTags().appendValue("hoot:test", "test");

    NoInformationElementRemover().apply(map);

    //The relation and all of its elements should be removed.

    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());
  }

  void runRelationWithoutInfoOneWayElementWithInfoTest()
  {
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());

    QList<ElementPtr> elements;

    QList<NodePtr> nodes;
    NodePtr node1(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node1->getTags().appendValue("hoot:test", "test1");
    nodes.append(node1);
    elements.append(node1);

    NodePtr node2(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node2->getTags().appendValue("hoot:test", "test2");
    nodes.append(node2);
    elements.append(node2);

    WayPtr way1 = TestUtils::createWay(map, nodes);
    way1->getTags().appendValue("hoot:test", "test1");
    way1->getTags().appendValue("test", "test1");
    elements.append(way1);

    WayPtr way2 = TestUtils::createWay(map, nodes);
    way2->getTags().appendValue("hoot:test", "test2");
    elements.append(way2);

    RelationPtr relation = TestUtils::createRelation(map, elements);
    relation->getTags().appendValue("hoot:test", "test");

    NoInformationElementRemover().apply(map);

    //The relation and all of its elements should be removed.

    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());
  }

  void runRelationWithoutInfoAllElementsWithoutInfoTest()
  {
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());

    QList<ElementPtr> elements;

    QList<NodePtr> nodes;
    NodePtr node1(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node1->getTags().appendValue("hoot:test", "test1");
    nodes.append(node1);
    elements.append(node1);

    NodePtr node2(new Node(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15));
    node2->getTags().appendValue("hoot:test", "test2");
    nodes.append(node2);
    elements.append(node2);

    WayPtr way1 = TestUtils::createWay(map, nodes);
    way1->getTags().appendValue("hoot:test", "test1");
    elements.append(way1);

    WayPtr way2 = TestUtils::createWay(map, nodes);
    way2->getTags().appendValue("hoot:test", "test2");
    elements.append(way2);

    RelationPtr relation = TestUtils::createRelation(map, elements);
    relation->getTags().appendValue("hoot:test", "test");

    NoInformationElementRemover().apply(map);

    //The relation and all of its elements should be removed.

    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NoInformationElementRemoverTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NoInformationElementRemoverTest, "current");

}

