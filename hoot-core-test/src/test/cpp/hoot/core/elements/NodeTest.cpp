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
 * @copyright Copyright (C) 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/geom/Geometry.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Log.h>
using namespace hoot;

// Qt
#include <QDebug>
#include <QDir>

// Tgs
#include <tgs/HashMap.h>

#include "../TestUtils.h"

namespace hoot
{

class NodeTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(NodeTest);
  CPPUNIT_TEST(runCopyTest);
  CPPUNIT_TEST(runSetTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runCopyTest()
  {
    NodePtr n(new Node(Status::Unknown1, 123, 1.2, 2.3, 3.14));
    n->setTag("foo", "bar");

    HOOT_STR_EQUALS(Status::Unknown1, (Status::TypeEnum)n->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(123l, n->getId());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.3, n->getY(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.14, n->getCircularError(), 1e-3);
    CPPUNIT_ASSERT_EQUAL(QString("bar"), n->getTags()["foo"]);

    NodePtr n2(new Node(*n));
    HOOT_STR_EQUALS(Status::Unknown1, (Status::TypeEnum)n2->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(123l, n2->getId());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2, n2->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.3, n2->getY(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.14, n2->getCircularError(), 1e-3);
    CPPUNIT_ASSERT_EQUAL(QString("bar"), n2->getTags()["foo"]);
  }

  void runSetTest()
  {
    OsmMapPtr map(new OsmMap());

    NodePtr n(new Node(Status::Unknown1, 123, 1.2, 2.3, 3.14));
    n->setTag("foo", "bar");

    HOOT_STR_EQUALS(Status::Unknown1, (Status::TypeEnum)n->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(123l, n->getId());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.3, n->getY(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.14, n->getCircularError(), 1e-3);
    CPPUNIT_ASSERT_EQUAL(QString("bar"), n->getTags()["foo"]);

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
    CPPUNIT_ASSERT_EQUAL(QString("cheese"), n->getTags()["foo"]);
  }

};

}

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NodeTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NodeTest, "quick");

