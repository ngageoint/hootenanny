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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/Log.h>
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

class RecursiveElementRemoverTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RecursiveElementRemoverTest);
  CPPUNIT_TEST(removeRelationTest);
  CPPUNIT_TEST(removeWayTest);
  CPPUNIT_TEST_SUITE_END();

public:

  boost::shared_ptr<OsmMap> createTestMap()
  {
    boost::shared_ptr<OsmMap> result(new OsmMap());

    for (long nid = 1; nid <= 13; nid++)
    {
      boost::shared_ptr<Node> n(new Node(Status::Unknown1, nid, 0.0, 0.0, -1));
      result->addNode(n);
    }

    for (long wid = 1; wid <= 5; wid++)
    {
      boost::shared_ptr<Way> w(new Way(Status::Unknown1, wid, -1));
      result->addWay(w);
    }

    for (long rid = 1; rid <= 4; rid++)
    {
      boost::shared_ptr<Relation> r(new Relation(Status::Unknown1, rid, -1));
      result->addRelation(r);
    }

    boost::shared_ptr<Way> w1 = result->getWay(1);
    w1->addNode(1);
    w1->addNode(2);
    w1->addNode(3);

    boost::shared_ptr<Way> w2 = result->getWay(2);
    w2->addNode(3);
    w2->addNode(4);
    w2->addNode(5);

    boost::shared_ptr<Way> w3 = result->getWay(3);
    w3->addNode(5);
    w3->addNode(6);
    w3->addNode(7);

    boost::shared_ptr<Way> w4 = result->getWay(4);
    w4->addNode(8);
    w4->addNode(9);
    w4->addNode(10);

    boost::shared_ptr<Way> w5 = result->getWay(5);
    w5->addNode(11);
    w5->addNode(12);
    w5->addNode(13);

    boost::shared_ptr<Relation> r1 = result->getRelation(1);
    r1->addElement("", w2);
    r1->addElement("", w3);
    r1->addElement("", w4);

    boost::shared_ptr<Relation> r2 = result->getRelation(2);
    r2->addElement("", w4);

    boost::shared_ptr<Relation> r3 = result->getRelation(3);
    r3->addElement("", r2);
    r3->addElement("", w5);

    return result;
  }

  void removeRelationTest()
  {
    boost::shared_ptr<OsmMap> base = createTestMap();
    boost::shared_ptr<OsmMap> map;
    map = createTestMap();

    RecursiveElementRemover uut(ElementId::relation(1));
    uut.apply(map);

    // it should remove two ways, 5 nodes and one relation
    CPPUNIT_ASSERT_EQUAL(base->getWays().size() - 2, map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(base->getNodes().size() - 4, map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(base->getRelations().size() - 1, map->getRelations().size());
    CPPUNIT_ASSERT_EQUAL(true, map->containsNode(3));
    CPPUNIT_ASSERT_EQUAL(false, map->containsNode(4));
    CPPUNIT_ASSERT_EQUAL(false, map->containsNode(5));
    CPPUNIT_ASSERT_EQUAL(false, map->containsWay(2));
    CPPUNIT_ASSERT_EQUAL(false, map->containsWay(3));
    CPPUNIT_ASSERT_EQUAL(false, map->containsRelation(1));

    map = createTestMap();

    RecursiveElementRemover uut2(ElementId::relation(3));
    uut2.apply(map);

    // it should remove two ways, 5 nodes and one relation
    CPPUNIT_ASSERT_EQUAL(base->getWays().size() - 1, map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(base->getNodes().size() - 3, map->getNodes().size());
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
    boost::shared_ptr<OsmMap> base = createTestMap();
    boost::shared_ptr<OsmMap> map;
    map = createTestMap();

    RecursiveElementRemover uut(ElementId::way(1));
    uut.apply(map);

    // it should remove one way, but leave node 3 (shared w/ way 2)
    CPPUNIT_ASSERT_EQUAL(base->getWays().size() - 1, map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(base->getNodes().size() - 2, map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(false, map->containsNode(1));
    CPPUNIT_ASSERT_EQUAL(false, map->containsNode(2));
    CPPUNIT_ASSERT_EQUAL(false, map->containsWay(1));


    map = createTestMap();

    RecursiveElementRemover uut2(ElementId::way(2));
    uut2.apply(map);

    // There should be no change
    CPPUNIT_ASSERT_EQUAL(base->getWays().size(), map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(base->getNodes().size(), map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(true, map->containsNode(3));
    CPPUNIT_ASSERT_EQUAL(true, map->containsNode(4));
    CPPUNIT_ASSERT_EQUAL(true, map->containsWay(2));
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RecursiveElementRemoverTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RecursiveElementRemoverTest, "quick");

