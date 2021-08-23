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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/ops/NoInformationElementRemover.h>


using namespace geos::geom;

namespace hoot
{

class NoInformationElementRemoverTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NoInformationElementRemoverTest);
  CPPUNIT_TEST(runWayWithInfoOneNodeWithoutInfoTest);
  CPPUNIT_TEST(runStandAloneNodesWithAndWithoutInfoTest);
  CPPUNIT_TEST(runWayWithoutInfoOneNodeWithInfoTest);
  CPPUNIT_TEST(runEmptyWayOrphanNodesTest);
  CPPUNIT_TEST(runWayWithoutInfoAllNodesWithoutInfoTest);
  CPPUNIT_TEST(runRelationWithInfoOneElementWithoutInfoTest);
  CPPUNIT_TEST(runRelationWithoutInfoOneNodeElementWithInfoTest);
  CPPUNIT_TEST(runRelationWithoutInfoOneWayElementWithInfoTest);
  CPPUNIT_TEST(runEmptyRelationWithoutInfoTest);
  CPPUNIT_TEST(runRelationWithoutInfoAllElementsWithoutInfoTest);
  CPPUNIT_TEST(runUnrelatedElementsWithInfoTest);
  CPPUNIT_TEST_SUITE_END();

public:

  NoInformationElementRemoverTest()
  {
    setResetType(ResetBasic);
  }

  void runWayWithInfoOneNodeWithoutInfoTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    QList<NodePtr> nodes;
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node1->getTags().appendValue("hoot:test", "test1");
    node1->getTags().appendValue("test", "test1");
    nodes.append(node1);
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
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
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node1->getTags().appendValue("hoot:test", "test1");
    node1->getTags().appendValue("test", "test1");
    map->addNode(node1);
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node2->getTags().appendValue("hoot:test", "test2");
    map->addNode(node2);

    NoInformationElementRemover().apply(map);

    //Node 2 has no info and isn't owned by a way, so it should be removed.
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getNodes().size());

    ConstNodePtr parsedNode1 = map->getNode(node1->getElementId().getId());
    const Tags& node1Tags = parsedNode1->getTags();
    CPPUNIT_ASSERT_EQUAL(2, node1Tags.size());
    HOOT_STR_EQUALS(QString("test1"), node1Tags.get("hoot:test"));
    HOOT_STR_EQUALS(QString("test1"), node1Tags.get("test"));

    CPPUNIT_ASSERT(!map->containsNode(node2->getElementId().getId()));
  }

  void runWayWithoutInfoOneNodeWithInfoTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    QList<NodePtr> nodes;
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node1->getTags().appendValue("hoot:test", "test1");
    node1->getTags().appendValue("test", "test1");
    nodes.append(node1);
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node2->getTags().appendValue("hoot:test", "test2");
    nodes.append(node2);

    WayPtr way = TestUtils::createWay(map, nodes);
    way->getTags().appendValue("hoot:test", "test1");

    NoInformationElementRemover().apply(map);

    // Because the way contains nodes (even though they don't have any useful metadata)
    // neither the way nor the nodes should be removed
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());
  }

  void runEmptyWayOrphanNodesTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    QList<NodePtr> nodes;
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node1->getTags().appendValue("hoot:test", "test1");
    node1->getTags().appendValue("test", "test1");
    nodes.append(node1);
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node2->getTags().appendValue("hoot:test", "test2");
    nodes.append(node2);
    WayPtr way1 = std::make_shared<Way>(Status::Unknown1, 15, 15);
    way1->getTags().appendValue("hoot:test", "test3");
    map->addWay(way1);

    NoInformationElementRemover().apply(map);

    // Because the way doesn't contain any nodes or useful info, it should be removed.
    // Because the nodes don't belong to a way or relation, or have useful info, they should
    // be removed
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
  }

  void runWayWithoutInfoAllNodesWithoutInfoTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    QList<NodePtr> nodes;
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node1->getTags().appendValue("hoot:test", "test1");
    nodes.append(node1);
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node2->getTags().appendValue("hoot:test", "test2");
    nodes.append(node2);
    WayPtr way1 = TestUtils::createWay(map, nodes);
    way1->getTags().appendValue("hoot:test", "test");

    NoInformationElementRemover().apply(map);

    // Because the way contains nodes (even though they don't have any useful metadata)
    // neither the way nor the nodes should be removed
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());
  }

  void runRelationWithInfoOneElementWithoutInfoTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    QList<ElementPtr> elements;

    QList<NodePtr> nodes;
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node1->getTags().appendValue("hoot:test", "test1");
    nodes.append(node1);
    elements.append(node1);

    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
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
    OsmMapPtr map = std::make_shared<OsmMap>();

    QList<ElementPtr> elements;

    QList<NodePtr> nodes;
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node1->getTags().appendValue("hoot:test", "test1");
    nodes.append(node1);
    elements.append(node1);

    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
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

    // All elements are interrelated - should not be removed
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getRelations().size());
  }

  void runRelationWithoutInfoOneWayElementWithInfoTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    QList<ElementPtr> elements;

    QList<NodePtr> nodes;
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node1->getTags().appendValue("hoot:test", "test1");
    nodes.append(node1);
    elements.append(node1);

    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
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

    // All elements are interrelated - should not be removed
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getRelations().size());
  }

  void runEmptyRelationWithoutInfoTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    QList<ElementPtr> elements;

    QList<NodePtr> nodes;
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node1->getTags().appendValue("hoot:test", "test1");
    nodes.append(node1);
    elements.append(node1);

    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
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

    RelationPtr relation1 = std::make_shared<Relation>(Status::Unknown1, 15, 15);
    map->addRelation(relation1);
    relation1->getTags().appendValue("hoot:test", "test");

    NoInformationElementRemover().apply(map);

    // Nodes and Ways should not be removed, but relation should be
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());
  }

  void runRelationWithoutInfoAllElementsWithoutInfoTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node1->getTags().appendValue("hoot:test", "test1");

    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node2->getTags().appendValue("hoot:test", "test2");

    WayPtr way1 = std::make_shared<Way>(Status::Unknown1, 15, 15);
    way1->getTags().appendValue("hoot:test", "test3");
    map->addWay(way1);

    WayPtr way2 = std::make_shared<Way>(Status::Unknown1, 15, 15);
    way2->getTags().appendValue("hoot:test", "test4");
    map->addWay(way2);

    RelationPtr relation1 = std::make_shared<Relation>(Status::Unknown1, 15, 15);
    map->addRelation(relation1);
    relation1->getTags().appendValue("hoot:test", "test");

    NoInformationElementRemover().apply(map);

    // All map items are unrelated, and have no info. They should be removed.
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());
  }

  void runUnrelatedElementsWithInfoTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node1->getTags().appendValue("hoot:test", "test1");
    node1->getTags().appendValue("name", "red house");
    map->addNode(node1);

    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, map->createNextNodeId(), Coordinate(0.0, 0.0), 15);
    node2->getTags().appendValue("hoot:test", "test2");
    node2->getTags().appendValue("name", "blue house");
    map->addNode(node2);

    WayPtr way1 = std::make_shared<Way>(Status::Unknown1, 15, 15);
    way1->getTags().appendValue("hoot:test", "test3");
    way1->getTags().appendValue("name", "some way");
    map->addWay(way1);

    WayPtr way2 = std::make_shared<Way>(Status::Unknown1, 16, 15);
    way2->getTags().appendValue("hoot:test", "test4");
    way2->getTags().appendValue("name", "some other way");
    map->addWay(way2);


    RelationPtr relation1 = std::make_shared<Relation>(Status::Unknown1, 15, 15);
    map->addRelation(relation1);
    relation1->getTags().appendValue("hoot:test", "test");
    relation1->getTags().appendValue("name", "some relation");

    NoInformationElementRemover().apply(map);

    // All map items are not related, but have info. They should not be removed.
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getRelations().size());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NoInformationElementRemoverTest, "quick");

}

