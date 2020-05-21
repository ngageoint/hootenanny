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
 * @copyright Copyright (C) 2012, 2013, 2014, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/ElementComparer.h>

namespace hoot
{

class ElementComparerTest : public HootTestFixture
{
  // TODO: cleanup
  CPPUNIT_TEST_SUITE(ElementComparerTest);
  CPPUNIT_TEST(runDifferentTypesTest);
  CPPUNIT_TEST(runNodeWithinDistanceThresholdTest1);
  CPPUNIT_TEST(runNodeOutsideOfDistanceThresholdTest);
  CPPUNIT_TEST(runNodeDifferentTagsTest);
  //CPPUNIT_TEST(runNodeDifferentCircularErrorTest);
  CPPUNIT_TEST(runWaySameTest);
  CPPUNIT_TEST(runWayDifferentIdsTest);
  CPPUNIT_TEST(runWayDifferentNodesTest);
  CPPUNIT_TEST(runWayDifferentNodeOrderTest);
  CPPUNIT_TEST(runWayDifferentTagsTest);
  //CPPUNIT_TEST(runWayDifferentCircularErrorTest);
  CPPUNIT_TEST(runWayDifferentVersionsTest);
  CPPUNIT_TEST(runRelationSameTest);
  CPPUNIT_TEST(runRelationDifferentIdsTest);
  CPPUNIT_TEST(runRelationDifferentTypesTest);
  CPPUNIT_TEST(runRelationDifferentMembersTest);
  CPPUNIT_TEST(runRelationDifferentMemberRolesTest);
  CPPUNIT_TEST(runRelationDifferentTagsTest);
  //CPPUNIT_TEST(runRelationDifferentCircularErrorTest);
  CPPUNIT_TEST(runRelationDifferentVersionsTest);
  // TODO: need tests for _ignoreElementId=true and _ignoreVersion=true
  CPPUNIT_TEST_SUITE_END();

public:

  void runDifferentTypesTest()
  {
    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));

    CPPUNIT_ASSERT(!ElementComparer().isSame(node1, way1));
  }

  void runNodeWithinDistanceThresholdTest1()
  {
    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
    node1->setTag("key1", "value1");

    NodePtr node2(new Node(Status::Unknown1, 1, 0.0, 0.00000001, 15.0));
    node2->setTag("key1", "value1");

    CPPUNIT_ASSERT(ElementComparer().isSame(node1, node2));
  }

  void runNodeOutsideOfDistanceThresholdTest()
  {
    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
    node1->setTag("key1", "value1");

    NodePtr node2(new Node(Status::Unknown1, 1, 0.0, 0.01, 15.0));
    node2->setTag("key1", "value1");

    CPPUNIT_ASSERT(!ElementComparer().isSame(node1, node2));
  }

  void runNodeDifferentTagsTest()
  {
    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
    node1->setTag("key1", "value1");

    NodePtr node2(new Node(Status::Unknown1, 1, 0.0, 0.00000001, 15.0));
    node2->setTag("key1", "value2");

    CPPUNIT_ASSERT(!ElementComparer().isSame(node1, node2));
  }

//  void runNodeDifferentStatusTest()
//  {
//    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
//    node1->setTag("key1", "value1");

//    NodePtr node2(new Node(Status::Unknown2, 1, 0.0, 0.00000001, 15.0));
//    node2->setTag("key1", "value1");

//    CPPUNIT_ASSERT(!ElementComparer().isSame(node1, node2));
//  }

//  void runNodeDifferentCircularErrorTest()
//  {
//    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
//    node1->setTag("key1", "value1");

//    NodePtr node2(new Node(Status::Unknown1, 1, 0.0, 0.00000001, 16.0));
//    node2->setTag("key1", "value1");

//    CPPUNIT_ASSERT(!ElementComparer().isSame(node1, node2));
//  }

  void runWaySameTest()
  {
    //OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
    //map->addElement(node1);
    NodePtr node2(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
    //map->addElement(node2);

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
    way1->setTag("key1", "value1");
    way1->addNode(node1->getId());
    way1->addNode(node2->getId());
    //map->addElement(way1);

    WayPtr way2(new Way(Status::Unknown1, 1, 15.0));
    way2->setTag("key1", "value1");
    way2->addNode(node1->getId());
    way2->addNode(node2->getId());
    //map->addElement(way2);

    ElementComparer uut;
    //uut.setOsmMap(map.get());
    CPPUNIT_ASSERT(uut.isSame(way1, way2));
  }

  void runWayDifferentIdsTest()
  {
    //OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
    //map->addElement(node1);
    NodePtr node2(new Node(Status::Unknown1, 2, 0.0, 0.0, 15.0));
    //map->addElement(node2);

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
    way1->setTag("key1", "value1");
    way1->addNode(node1->getId());
    way1->addNode(node2->getId());
    //map->addElement(way1);

    WayPtr way2(new Way(Status::Unknown1, 2, 15.0));
    way2->setTag("key1", "value1");
    way2->addNode(node1->getId());
    way2->addNode(node2->getId());
    //map->addElement(way2);

    ElementComparer uut;
    //uut.setOsmMap(map.get());
    CPPUNIT_ASSERT(!uut.isSame(way1, way2));
  }

  void runWayDifferentNodesTest()
  {
    //OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
    //map->addElement(node1);
    NodePtr node2(new Node(Status::Unknown1, 2, 0.0, 0.0, 15.0));
    //map->addElement(node2);

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
    way1->setTag("key1", "value1");
    way1->addNode(node1->getId());
    way1->addNode(node2->getId());
    //map->addElement(way1);

    WayPtr way2(new Way(Status::Unknown1, 1, 15.0));
    way2->setTag("key1", "value1");
    way2->addNode(node1->getId());
    //map->addElement(way2);

    ElementComparer uut;
    //uut.setOsmMap(map.get());
    CPPUNIT_ASSERT(!uut.isSame(way1, way2));
  }

  void runWayDifferentNodeOrderTest()
  {
    //OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
    //map->addElement(node1);
    NodePtr node2(new Node(Status::Unknown1, 2, 0.0, 0.0, 15.0));
    //map->addElement(node2);

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
    way1->setTag("key1", "value1");
    way1->addNode(node1->getId());
    way1->addNode(node2->getId());
    //map->addElement(way1);

    WayPtr way2(new Way(Status::Unknown1, 1, 15.0));
    way2->setTag("key1", "value1");
    way2->addNode(node2->getId());
    way2->addNode(node1->getId());
    //map->addElement(way2);

    ElementComparer uut;
    //uut.setOsmMap(map.get());
    CPPUNIT_ASSERT(!uut.isSame(way1, way2));
  }

  void runWayDifferentTagsTest()
  {
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
    //map->addElement(node1);
    NodePtr node2(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
    //map->addElement(node2);

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
    way1->setTag("key1", "value1");
    way1->addNode(node1->getId());
    way1->addNode(node2->getId());
    //map->addElement(way1);

    WayPtr way2(new Way(Status::Unknown1, 1, 15.0));
    way2->setTag("key1", "value2");
    way2->addNode(node1->getId());
    way2->addNode(node2->getId());
    //map->addElement(way2);

    ElementComparer uut;
    //uut.setOsmMap(map.get());
    CPPUNIT_ASSERT(!uut.isSame(way1, way2));
  }

//  void runWayDifferentStatusTest()
//  {
//    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
//    NodePtr node2(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));

//    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
//    way1->setTag("key1", "value1");
//    way1->addNode(node1->getId());
//    way1->addNode(node2->getId());

//    WayPtr way2(new Way(Status::Unknown2, 1, 15.0));
//    way2->setTag("key1", "value1");
//    way2->addNode(node1->getId());
//    way2->addNode(node2->getId());

//    CPPUNIT_ASSERT(!ElementComparer().isSame(way1, way2));
//  }

//  void runWayDifferentCircularErrorTest()
//  {
//    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
//    NodePtr node2(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));

//    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
//    way1->setTag("key1", "value1");
//    way1->addNode(node1->getId());
//    way1->addNode(node2->getId());

//    WayPtr way2(new Way(Status::Unknown1, 1, 16.0));
//    way2->setTag("key1", "value1");
//    way2->addNode(node1->getId());
//    way2->addNode(node2->getId());

//    CPPUNIT_ASSERT(!ElementComparer().isSame(way1, way2));
//  }

  void runWayDifferentVersionsTest()
  {
    //OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
    //map->addElement(node1);
    NodePtr node2(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
    //map->addElement(node2);

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0, ElementData::CHANGESET_EMPTY, 1));
    way1->setTag("key1", "value1");
    way1->addNode(node1->getId());
    way1->addNode(node2->getId());
    //map->addElement(way1);

    WayPtr way2(new Way(Status::Unknown1, 1, 15.0, ElementData::CHANGESET_EMPTY, 2));
    way2->setTag("key1", "value1");
    way2->addNode(node1->getId());
    way2->addNode(node2->getId());
    //map->addElement(way2);

    ElementComparer uut;
    //uut.setOsmMap(map.get());
    CPPUNIT_ASSERT(!uut.isSame(way1, way2));
  }

  void runRelationSameTest()
  {
    //OsmMapPtr map(new OsmMap());

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
    //map->addElement(way1);

    RelationPtr relation1(new Relation(Status::Unknown1, 1, 15.0, "type1"));
    relation1->setTag("key1", "value1");
    relation1->addElement("role1", way1);
    //map->addElement(relation1);

    RelationPtr relation2(new Relation(Status::Unknown1, 1, 15.0, "type1"));
    relation2->setTag("key1", "value1");
    relation2->addElement("role1", way1);
    //map->addElement(relation2);

    ElementComparer uut;
    //uut.setOsmMap(map.get());
    CPPUNIT_ASSERT(uut.isSame(relation1, relation2));
  }

  void runRelationDifferentIdsTest()
  {
    //OsmMapPtr map(new OsmMap());

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
    //map->addElement(way1);

    RelationPtr relation1(new Relation(Status::Unknown1, 1, 15.0, "type1"));
    relation1->setTag("key1", "value1");
    relation1->addElement("role1", way1);
    //map->addElement(relation1);

    RelationPtr relation2(new Relation(Status::Unknown1, 2, 15.0, "type1"));
    relation2->setTag("key1", "value1");
    relation2->addElement("role1", way1);
    //map->addElement(relation2);

    ElementComparer uut;
    //uut.setOsmMap(map.get());
    CPPUNIT_ASSERT(!uut.isSame(relation1, relation2));
  }

  void runRelationDifferentTypesTest()
  {
    //OsmMapPtr map(new OsmMap());

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
    //map->addElement(way1);

    RelationPtr relation1(new Relation(Status::Unknown1, 1, 15.0, "type1"));
    relation1->setTag("key1", "value1");
    relation1->addElement("role1", way1);
    //map->addElement(relation1);

    RelationPtr relation2(new Relation(Status::Unknown1, 1, 15.0, "type2"));
    relation2->setTag("key1", "value1");
    relation2->addElement("role1", way1);
    //map->addElement(relation2);

    ElementComparer uut;
    //uut.setOsmMap(map.get());
    CPPUNIT_ASSERT(!uut.isSame(relation1, relation2));
  }

  void runRelationDifferentMembersTest()
  {
    //OsmMapPtr map(new OsmMap());

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
    //map->addElement(way1);
    WayPtr way2(new Way(Status::Unknown1, 2, 15.0));
    //map->addElement(way2);

    RelationPtr relation1(new Relation(Status::Unknown1, 1, 15.0, "type1"));
    relation1->setTag("key1", "value1");
    relation1->addElement("role1", way1);
    //map->addElement(relation1);

    RelationPtr relation2(new Relation(Status::Unknown1, 1, 15.0, "type1"));
    relation2->setTag("key1", "value1");
    relation2->addElement("role1", way2);
    //map->addElement(relation2);

    ElementComparer uut;
    //uut.setOsmMap(map.get());
    CPPUNIT_ASSERT(!uut.isSame(relation1, relation2));
  }

  void runRelationDifferentMemberRolesTest()
  {
    //OsmMapPtr map(new OsmMap());

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
    //map->addElement(way1);

    RelationPtr relation1(new Relation(Status::Unknown1, 1, 15.0, "type1"));
    relation1->setTag("key1", "value1");
    relation1->addElement("role1", way1);
    //map->addElement(relation1);

    RelationPtr relation2(new Relation(Status::Unknown1, 1, 15.0, "type1"));
    relation2->setTag("key1", "value1");
    relation2->addElement("role2", way1);
    //map->addElement(relation2);

    ElementComparer uut;
    //uut.setOsmMap(map.get());
    CPPUNIT_ASSERT(!uut.isSame(relation1, relation2));
  }

  void runRelationDifferentTagsTest()
  {
    //OsmMapPtr map(new OsmMap());

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
    //map->addElement(way1);

    RelationPtr relation1(new Relation(Status::Unknown1, 1, 15.0, "type1"));
    relation1->setTag("key1", "value1");
    relation1->addElement("role1", way1);
    //map->addElement(relation1);

    RelationPtr relation2(new Relation(Status::Unknown1, 1, 15.0, "type1"));
    relation2->setTag("key1", "value2");
    relation2->addElement("role1", way1);
    //map->addElement(relation2);

    ElementComparer uut;
    //uut.setOsmMap(map.get());
    CPPUNIT_ASSERT(!uut.isSame(relation1, relation2));
  }

//  void runRelationDifferentStatusTest()
//  {
//    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));

//    RelationPtr relation1(new Relation(Status::Unknown1, 1, 15.0, "type1"));
//    relation1->setTag("key1", "value1");
//    relation1->addElement("role1", way1);

//    RelationPtr relation2(new Relation(Status::Unknown2, 1, 15.0, "type1"));
//    relation2->setTag("key1", "value1");
//    relation2->addElement("role1", way1);

//    CPPUNIT_ASSERT(!ElementComparer().isSame(relation1, relation2));
//  }

//  void runRelationDifferentCircularErrorTest()
//  {
//    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));

//    RelationPtr relation1(new Relation(Status::Unknown1, 1, 15.0, "type1"));
//    relation1->setTag("key1", "value1");
//    relation1->addElement("role1", way1);

//    RelationPtr relation2(new Relation(Status::Unknown1, 1, 16.0, "type1"));
//    relation2->setTag("key1", "value1");
//    relation2->addElement("role1", way1);

//    CPPUNIT_ASSERT(!ElementComparer().isSame(relation1, relation2));
//  }

  void runRelationDifferentVersionsTest()
  {
    //OsmMapPtr map(new OsmMap());

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
    //map->addElement(way1);

    RelationPtr relation1(
      new Relation(Status::Unknown1, 1, 15.0, "type1", ElementData::CHANGESET_EMPTY, 1));
    relation1->setTag("key1", "value1");
    relation1->addElement("role1", way1);
    //map->addElement(relation1);

    RelationPtr relation2(
      new Relation(Status::Unknown1, 1, 15.0, "type1", ElementData::CHANGESET_EMPTY, 2));
    relation2->setTag("key1", "value1");
    relation2->addElement("role1", way1);
    //map->addElement(relation2);

    ElementComparer uut;
    //uut.setOsmMap(map.get());
    CPPUNIT_ASSERT(!uut.isSame(relation1, relation2));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementComparerTest, "quick");

}
