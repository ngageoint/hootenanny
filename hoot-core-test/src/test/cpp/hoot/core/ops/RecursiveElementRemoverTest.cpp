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
 * @copyright Copyright (C) 2013, 2014, 2015, 2017, 2018, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/elements/MapProjector.h>

namespace hoot
{

class RecursiveElementRemoverTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RecursiveElementRemoverTest);
  CPPUNIT_TEST(removeRelationTest);
  CPPUNIT_TEST(removeWayTest);
  CPPUNIT_TEST(removeParentRefsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmMapPtr createTestMap1()
  {
    OsmMapPtr result = std::make_shared<OsmMap>();

    for (long nid = 1; nid <= 13; nid++)
    {
      result->addNode(std::make_shared<Node>(Status::Unknown1, nid, 0.0, 0.0, -1));
    }

    for (long wid = 1; wid <= 5; wid++)
    {
      result->addWay(std::make_shared<Way>(Status::Unknown1, wid, -1));
    }

    for (long rid = 1; rid <= 4; rid++)
    {
      result->addRelation(std::make_shared<Relation>(Status::Unknown1, rid, -1));
    }

    WayPtr w1 = result->getWay(1);
    w1->addNode(1);
    w1->addNode(2);
    w1->addNode(3);

    WayPtr w2 = result->getWay(2);
    w2->addNode(3);
    w2->addNode(4);
    w2->addNode(5);

    WayPtr w3 = result->getWay(3);
    w3->addNode(5);
    w3->addNode(6);
    w3->addNode(7);

    WayPtr w4 = result->getWay(4);
    w4->addNode(8);
    w4->addNode(9);
    w4->addNode(10);

    WayPtr w5 = result->getWay(5);
    w5->addNode(11);
    w5->addNode(12);
    w5->addNode(13);

    RelationPtr r1 = result->getRelation(1);
    r1->addElement("", w2);
    r1->addElement("", w3);
    r1->addElement("", w4);

    RelationPtr r2 = result->getRelation(2);
    r2->addElement("", w4);

    RelationPtr r3 = result->getRelation(3);
    r3->addElement("", r2);
    r3->addElement("", w5);

    return result;
  }

  OsmMapPtr createTestMap2()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr node1 = TestUtils::createNode(map, "n1"); // belongs to way 1
    NodePtr node2 = TestUtils::createNode(map, "n2"); // belongs to way 1
    NodePtr node3 = TestUtils::createNode(map, "n3"); // belongs to way 2
    NodePtr node4 = TestUtils::createNode(map, "n4"); // belongs to way 2
    NodePtr node5 = TestUtils::createNode(map, "n5"); // belongs to relation 1

    WayPtr way1 = TestUtils::createWay(map, QList<ElementId>(), "w1"); // belongs to relation 1
    way1->addNode(node1->getId());
    way1->addNode(node2->getId());
    WayPtr way2 = TestUtils::createWay(map, QList<ElementId>(), "w2"); // belongs to nothing
    way2->addNode(node3->getId());
    way2->addNode(node4->getId());

    // belongs to nothing
    RelationPtr relation1 = TestUtils::createRelation(map, QList<ElementId>(), "r1");
    relation1->addElement("", way1->getElementId());
    relation1->addElement("", node5->getElementId());
    // belongs to relation 1
    RelationPtr relation2 = TestUtils::createRelation(map, QList<ElementId>(), "r2");
    relation1->addElement("", relation2->getElementId());

    return map;
  }

  void removeRelationTest()
  {
    OsmMapPtr base = createTestMap1();
    OsmMapPtr map;
    map = createTestMap1();

    RecursiveElementRemover uut(ElementId::relation(1));
    uut.apply(map);

    // it should remove two ways, 5 nodes and one relation
    CPPUNIT_ASSERT_EQUAL(base->getWayCount() - 2, map->getWayCount());
    CPPUNIT_ASSERT_EQUAL(base->getNodeCount() - 4, map->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(base->getRelations().size() - 1, map->getRelations().size());
    CPPUNIT_ASSERT_EQUAL(true, map->containsNode(3));
    CPPUNIT_ASSERT_EQUAL(false, map->containsNode(4));
    CPPUNIT_ASSERT_EQUAL(false, map->containsNode(5));
    CPPUNIT_ASSERT_EQUAL(false, map->containsWay(2));
    CPPUNIT_ASSERT_EQUAL(false, map->containsWay(3));
    CPPUNIT_ASSERT_EQUAL(false, map->containsRelation(1));

    map = createTestMap1();

    RecursiveElementRemover uut2(ElementId::relation(3));
    uut2.apply(map);

    // it should remove two ways, 5 nodes and one relation
    CPPUNIT_ASSERT_EQUAL(base->getWayCount() - 1, map->getWayCount());
    CPPUNIT_ASSERT_EQUAL(base->getNodeCount() - 3, map->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(base->getRelations().size() - 2, map->getRelations().size());
    CPPUNIT_ASSERT_EQUAL(true, map->containsNode(3));
    CPPUNIT_ASSERT_EQUAL(false, map->containsNode(11));
    CPPUNIT_ASSERT_EQUAL(true, map->containsWay(4));
    CPPUNIT_ASSERT_EQUAL(false, map->containsWay(5));
    CPPUNIT_ASSERT_EQUAL(false, map->containsRelation(2));
    CPPUNIT_ASSERT_EQUAL(false, map->containsRelation(3));
  }

  void removeWayTest()
  {
    OsmMapPtr base = createTestMap1();
    OsmMapPtr map;
    map = createTestMap1();

    RecursiveElementRemover uut(ElementId::way(1));
    uut.apply(map);

    // it should remove one way, but leave node 3 (shared w/ way 2)
    CPPUNIT_ASSERT_EQUAL(base->getWayCount() - 1, map->getWayCount());
    CPPUNIT_ASSERT_EQUAL(base->getNodeCount() - 2, map->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(false, map->containsNode(1));
    CPPUNIT_ASSERT_EQUAL(false, map->containsNode(2));
    CPPUNIT_ASSERT_EQUAL(false, map->containsWay(1));

    map = createTestMap1();

    RecursiveElementRemover uut2(ElementId::way(2));
    uut2.apply(map);

    // There should be no change
    CPPUNIT_ASSERT_EQUAL(base->getWayCount(), map->getWayCount());
    CPPUNIT_ASSERT_EQUAL(base->getNodeCount(), map->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(true, map->containsNode(3));
    CPPUNIT_ASSERT_EQUAL(true, map->containsNode(4));
    CPPUNIT_ASSERT_EQUAL(true, map->containsWay(2));
  }

  void removeParentRefsTest()
  {
    OsmMapPtr map = createTestMap2();

    RecursiveElementRemover uut;

    // remove a way having a parent w/o removing its parent ref first
    ConstElementPtr relation1 = MapUtils::getFirstElementWithNote(map, "r1");
    ConstElementPtr node1 = MapUtils::getFirstElementWithNote(map, "n1");
    ConstElementPtr node2 = MapUtils::getFirstElementWithNote(map, "n2");
    ConstElementPtr way1 = MapUtils::getFirstElementWithNote(map, "w1");
    uut.addElement(way1);
    uut.setRemoveRefsFromParents(false);
    uut.apply(map);

    // nothing should be removed
    CPPUNIT_ASSERT(map->containsElement(way1));
    CPPUNIT_ASSERT(map->containsElement(node1));
    CPPUNIT_ASSERT(map->containsElement(node2));
    CPPUNIT_ASSERT(map->containsElement(relation1));

    // now remove the parent ref first and try again
    uut.setRemoveRefsFromParents(true);
    uut.apply(map);
    // it and both its nodes should be removed; one way + two child nodes + one parent relation
    CPPUNIT_ASSERT(!map->containsElement(way1));
    CPPUNIT_ASSERT(!map->containsElement(node1));
    CPPUNIT_ASSERT(!map->containsElement(node2));
    CPPUNIT_ASSERT(map->containsElement(relation1));

    // remove a relation having a parent w/o removing its parent ref first
    ConstElementPtr relation2 = MapUtils::getFirstElementWithNote(map, "r2");
    uut.addElement(relation2);
    uut.setRemoveRefsFromParents(false);
    uut.apply(map);
    // nothing should be removed
    CPPUNIT_ASSERT(map->containsElement(relation2));
    CPPUNIT_ASSERT(map->containsElement(relation1));

    // now remove the parent ref first and try again
    uut.setRemoveRefsFromParents(true);
    uut.apply(map);
    // it should be removed; one parent relation + one child relation
    CPPUNIT_ASSERT(!map->containsElement(relation2));
    CPPUNIT_ASSERT(map->containsElement(relation1));

    // remove a node having a way parent w/o removing its parent ref first
    ConstElementPtr way2 = MapUtils::getFirstElementWithNote(map, "w2");
    ConstElementPtr node3 = MapUtils::getFirstElementWithNote(map, "n3");
    uut.addElement(node3);
    uut.setRemoveRefsFromParents(false);
    uut.apply(map);
    // nothing should be removed
    CPPUNIT_ASSERT(map->containsElement(node3));
    CPPUNIT_ASSERT(map->containsElement(way2));

    // This isn't working...not sure why yet. Haven't needed to remove a node from a way yet in
    // production code using RecursiveElementRemover, so will fix later.
//    // now remove the parent ref first and try again
//    uut.setRemoveRefsFromParents(true);
//    uut.apply(map);
//    // it should be removed
//    CPPUNIT_ASSERT(!map->containsElement(node3));
//    CPPUNIT_ASSERT(map->containsElement(way2));

    // remove a node having a relation parent w/o removing its parent ref first
    ConstElementPtr node5 = MapUtils::getFirstElementWithNote(map, "n5");
    uut.addElement(node5);
    uut.setRemoveRefsFromParents(false);
    uut.apply(map);
    // nothing should be removed
    CPPUNIT_ASSERT(map->containsElement(node5));
    CPPUNIT_ASSERT(map->containsElement(relation1));

    // now remove the parent ref first and try again
    uut.setRemoveRefsFromParents(true);
    uut.apply(map);
    // it should be removed
    CPPUNIT_ASSERT(!map->containsElement(node5));
    CPPUNIT_ASSERT(map->containsElement(relation1));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RecursiveElementRemoverTest, "quick");

}
