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
 * @copyright Copyright (C) 2018 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/visitors/RemoveDuplicateRelationMembersVisitor.h>

namespace hoot
{

class RemoveDuplicateRelationMembersVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RemoveDuplicateRelationMembersVisitorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RemoveDuplicateRelationMembersVisitorTest()
  {
    setResetType(ResetBasic);
  }

  void runTest()
  {
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, 1, 0, 0, 0));
    map->addNode(node1);
    NodePtr node2(new Node(Status::Unknown1, 2, 0, 0, 0));
    map->addNode(node2);
    NodePtr node3(new Node(Status::Unknown1, 3, 0, 0, 0));
    map->addNode(node3);

    std::vector<long> nodeIds;

    WayPtr way1(new Way(Status::Unknown1, 1, 15.0));
    nodeIds.push_back(1);
    nodeIds.push_back(2);
    nodeIds.push_back(3);
    way1->setNodes(nodeIds);
    map->addWay(way1);
    nodeIds.clear();

    WayPtr way2(new Way(Status::Unknown1, 2, 15.0));
    nodeIds.push_back(2);
    nodeIds.push_back(1);
    nodeIds.push_back(3);
    way2->setNodes(nodeIds);
    map->addWay(way2);

    WayPtr way3(new Way(Status::Unknown1, 3, 15.0));
    nodeIds.push_back(3);
    nodeIds.push_back(1);
    nodeIds.push_back(2);
    way3->setNodes(nodeIds);
    map->addWay(way3);

    RelationPtr relation1(new Relation(Status::Unknown1, 1, 15.0));
    relation1->addElement("role1", node1);
    map->addRelation(relation1);

    RelationPtr relation2(new Relation(Status::Unknown1, 2, 15.0));
    relation2->addElement("role1", node2);
    map->addRelation(relation2);

    RelationPtr relation3(new Relation(Status::Unknown1, 3, 15.0));
    relation3->addElement("role1", node3);
    map->addRelation(relation3);

    RelationPtr relation4(new Relation(Status::Unknown1, 4, 15.0));
    relation4->addElement("role1", node1);
    relation4->addElement("role1", node1);
    relation4->addElement("role2", node2);
    relation4->addElement("role2", node3);
    relation4->addElement("role3", node3);
    relation4->addElement("role1", way1);
    relation4->addElement("role2", way2);
    relation4->addElement("role2", way2);
    relation4->addElement("role2", way3);
    relation4->addElement("role3", way2);
    relation4->addElement("role1", relation1);
    relation4->addElement("role1", relation1);
    relation4->addElement("role1", relation2);
    relation4->addElement("role2", relation3);
    relation4->addElement("role3", relation3);
    map->addRelation(relation4);

    RemoveDuplicateRelationMembersVisitor v;
    map->visitRw(v);

    CPPUNIT_ASSERT_EQUAL((size_t)12, relation4->getMembers().size());
    CPPUNIT_ASSERT(relation4->contains(node1->getElementId()));
    CPPUNIT_ASSERT(relation4->contains(node2->getElementId()));
    CPPUNIT_ASSERT(relation4->contains(node3->getElementId()));
    CPPUNIT_ASSERT(relation4->contains(way1->getElementId()));
    CPPUNIT_ASSERT(relation4->contains(way2->getElementId()));
    CPPUNIT_ASSERT(relation4->contains(way3->getElementId()));
    CPPUNIT_ASSERT(relation4->contains(relation1->getElementId()));
    CPPUNIT_ASSERT(relation4->contains(relation2->getElementId()));
    CPPUNIT_ASSERT(relation4->contains(relation3->getElementId()));
    CPPUNIT_ASSERT_EQUAL(4, relation4->numElementsByRole("role1"));
    CPPUNIT_ASSERT_EQUAL(5, relation4->numElementsByRole("role2"));
    CPPUNIT_ASSERT_EQUAL(3, relation4->numElementsByRole("role3"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveDuplicateRelationMembersVisitorTest, "quick");

}


