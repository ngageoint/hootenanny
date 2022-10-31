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
 * @copyright Copyright (C) 2015, 2016, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Status.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmXmlDiskCache.h>

namespace hoot
{

class OsmXmlDiskCacheTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmXmlDiskCacheTest);
  CPPUNIT_TEST(runAddSingleNodeTest);
  CPPUNIT_TEST(runAddAllTypesTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runAddSingleNodeTest()
  {
    OsmXmlDiskCache myCache("/tmp/hoot.disk.cache");

    // Add new node
    Status nodeStatus;
    Meters circularError(3.0);
    ConstElementPtr newNode = std::make_shared<const Node>(nodeStatus, 1, 2.0, 3.0, circularError);
    myCache.addElement(newNode);

    // Get element and check it
    ElementPtr elmnt = myCache.getElement(newNode->getElementId());
    CPPUNIT_ASSERT( elmnt->toString() == newNode->toString() );

    //Get node and check it
    NodePtr testNode = myCache.getNode(newNode->getId());
    CPPUNIT_ASSERT( testNode->toString() == newNode->toString() );
  }

  void runAddAllTypesTest()
  {
    OsmXmlDiskCache myCache("/tmp/hoot.disk.cache");

    // Make sure we're empty
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), true );
    // Add new node
    Status status;
    Meters circularError(3.0);
    ConstElementPtr newNode = std::make_shared<const Node>(status, 1, 2.0, 3.0, circularError);
    myCache.addElement(newNode);
    // Now we should be not empty, iterators have work
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), true );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(1) );

    // Add new way
    ConstElementPtr newWay = std::make_shared<const Way>(status, 10, circularError);
    myCache.addElement(newWay);
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), true );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(2) );

    // Add new relation
    ConstElementPtr newRelation = std::make_shared<Relation>(status, 20, circularError);
    myCache.addElement(newRelation);
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), true );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(3) );

    // Read out node
    ConstNodePtr emptyNode;
    ConstNodePtr myNodeCopy = myCache.getNextNode();
    CPPUNIT_ASSERT( emptyNode != myNodeCopy );
    CPPUNIT_ASSERT( myNodeCopy->toString() == newNode->toString() );

    // Check cache status
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), true );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(3) );

    // Try to read one more node, should be empty
    myNodeCopy = myCache.getNextNode();
    CPPUNIT_ASSERT( emptyNode == myNodeCopy );

    // Check cache status
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), true );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(3) );

    // Read out way
    ConstWayPtr emptyWay;
    ConstWayPtr myWayCopy = myCache.getNextWay();
    CPPUNIT_ASSERT( emptyWay != myWayCopy );
    CPPUNIT_ASSERT( myWayCopy->toString() == newWay->toString() );

    // Check cache status
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), true );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(3) );

    // Try to read one more way, should be empty
    myWayCopy = myCache.getNextWay();
    CPPUNIT_ASSERT( emptyWay == myWayCopy );

    // Check cache status
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), true );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(3) );

    // Read out relation
    ConstRelationPtr emptyRelation;
    ConstRelationPtr myRelationCopy = myCache.getNextRelation();
    CPPUNIT_ASSERT( emptyRelation != myRelationCopy );
    CPPUNIT_ASSERT( myRelationCopy->toString() == newRelation->toString() );

    // Check cache status -- all iterators are exhausted
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(3) );

    // Try to read one more way, should be empty
    myRelationCopy = myCache.getNextRelation();
    CPPUNIT_ASSERT( emptyRelation == myRelationCopy );

    // Check cache status
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(3) );
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmXmlDiskCacheTest, "quick");

}
