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

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/visitors/KeepNodesVisitor.h>

// Qt
#include <QDebug>
#include <QDir>

// TGS
#include <tgs/Statistics/Random.h>

#include "../TestUtils.h"

namespace hoot
{
using namespace Tgs;

class KeepNodesVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(KeepNodesVisitorTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runToyTest()
  {
    OsmMapPtr map(new OsmMap());

    NodePtr n1(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 5));
    n1->setTag("foo", "bar");
    map->addNode(n1);

    NodePtr n2(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 5));
    n2->setTag("hoot:dumb", "dumb");
    map->addNode(n2);

    NodePtr n3(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 5));
    map->addNode(n3);

    NodePtr w1n1(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 5));
    w1n1->setTag("foo", "bar");
    map->addNode(w1n1);

    NodePtr w1n2(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 5));
    map->addNode(w1n2);

    WayPtr w1(new Way(Status::Unknown1, map->createNextWayId(), 5));
    w1->addNode(w1n1->getId());
    w1->addNode(w1n2->getId());
    map->addWay(w1);

    NodePtr r1n1(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 5));
    r1n1->setTag("foo", "bar");
    map->addNode(r1n1);

    NodePtr r1n2(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 5));
    map->addNode(r1n2);

    RelationPtr r(new Relation(Status::Unknown1, map->createNextRelationId(), 5));
    r->addElement("", r1n1->getElementId());
    r->addElement("", r1n2->getElementId());
    map->addRelation(r);

    KeepNodesVisitor v;
    map->visitRw(v);

    HOOT_STR_EQUALS(3, map->getNodes().size());
    HOOT_STR_EQUALS(0, map->getWays().size());
    HOOT_STR_EQUALS(0, map->getRelations().size());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(KeepNodesVisitorTest, "quick");

}


