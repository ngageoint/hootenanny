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
 * @copyright Copyright (C) 2013, 2014, 2015, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/geom/Geometry.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/TagCountVisitor.h>

// Tgs
#include <tgs/HashMap.h>


namespace hoot
{

class NodeTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NodeTest);
  CPPUNIT_TEST(runCopyTest);
  CPPUNIT_TEST(runSetTest);
  CPPUNIT_TEST(runClearTest);
  CPPUNIT_TEST(runVisitTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runCopyTest()
  {
    NodePtr n = std::make_shared<Node>(Status::Unknown1, 123, 1.2, 2.3, 3.14);
    n->setTag("foo", "bar");

    HOOT_STR_EQUALS(Status::Unknown1, (Status::TypeEnum)n->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(123l, n->getId());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.3, n->getY(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.14, n->getCircularError(), 1e-3);
    HOOT_STR_EQUALS(QString("bar"), n->getTags()["foo"]);

    NodePtr n2 = std::make_shared<Node>(*n));
    HOOT_STR_EQUALS(Status::Unknown1, (Status::TypeEnum)n2->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(123l, n2->getId());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2, n2->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.3, n2->getY(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.14, n2->getCircularError(), 1e-3);
    HOOT_STR_EQUALS(QString("bar"), n2->getTags()["foo"]);
  }

  void runSetTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr n = std::make_shared<Node>(Status::Unknown1, 123, 1.2, 2.3, 3.14);
    n->setTag("foo", "bar");

    HOOT_STR_EQUALS(Status::Unknown1, (Status::TypeEnum)n->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(123l, n->getId());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.3, n->getY(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.14, n->getCircularError(), 1e-3);
    HOOT_STR_EQUALS(QString("bar"), n->getTags()["foo"]);

    n->setX(3.4);
    n->setY(4.5);
    n->setCircularError(2.718);
    n->setTag("foo", "cheese");
    n->setStatus(Status::Unknown2);

    HOOT_STR_EQUALS(Status::Unknown2, (Status::TypeEnum)n->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(123l, n->getId());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.4, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.5, n->getY(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.718, n->getCircularError(), 1e-3);
    HOOT_STR_EQUALS(QString("cheese"), n->getTags()["foo"]);
  }

  void runClearTest()
  {
    NodePtr uut =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    uut->setTag("foo", "bar");

    uut->clear();

    CPPUNIT_ASSERT_EQUAL(0, uut->getTagCount());
  }

  void runVisitTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    NodePtr uut =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    uut->setTag("foo", "bar");
    map->addNode(uut);

    TagCountVisitor vis;
    uut->visitRw(*map, vis);

    CPPUNIT_ASSERT_EQUAL(1.0, vis.getStat());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NodeTest, "quick");

}
