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

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Status.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/ElementCacheLRU.h>

namespace hoot
{

class ElementCacheLruTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ElementCacheLruTest);
  CPPUNIT_TEST(runCacheSizeTest);
  CPPUNIT_TEST(runCacheEmptyTest);
  CPPUNIT_TEST(runAddSingleNodeTest);
  CPPUNIT_TEST(runAddAllTypesTest);
  CPPUNIT_TEST(runForceNodeReplacementTest);
  CPPUNIT_TEST(runForceWayReplacementTest);
  CPPUNIT_TEST(runForceRelationReplacementTest);
  CPPUNIT_TEST(runAlgorithmTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runCacheEmptyTest()
  {
    ElementCacheLRU myCache(10, 10, 10);  // Max ten elements of each type

    // Make sure a new cache is empty
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), true );
    ConstNodePtr emptyNode;
    ConstWayPtr emptyWay;
    ConstRelationPtr emptyRelation;
    ElementPtr emptyElement;
    CPPUNIT_ASSERT_EQUAL( myCache.getNextNode(), emptyNode);
    CPPUNIT_ASSERT_EQUAL( myCache.getNextWay(), emptyWay);
    CPPUNIT_ASSERT_EQUAL( myCache.getNextRelation(), emptyRelation);
    CPPUNIT_ASSERT_EQUAL( myCache.readNextElement(), emptyElement);
  }

  void runCacheSizeTest()
  {
    ElementCacheLRU myCache(1, 2, 3);

    CPPUNIT_ASSERT_EQUAL(myCache.getNodeCacheSize(), static_cast<unsigned long>(1));
    CPPUNIT_ASSERT_EQUAL(myCache.getWayCacheSize(), static_cast<unsigned long>(2));
    CPPUNIT_ASSERT_EQUAL(myCache.getRelationCacheSize(), static_cast<unsigned long>(3));
  }

  void runAddSingleNodeTest()
  {
    ElementCacheLRU myCache(10, 10, 10);  // Max ten elements of each type

    // Make sure we're empty
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), true );
    // Add new node
    Status nodeStatus;
    Meters circularError(3.0);
    ConstElementPtr newNode = std::make_shared<const Node>(nodeStatus, 1, 2.0, 3.0, circularError);
    myCache.addElement(newNode);
    // Now we should be not empty
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), true );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), false );

    // Make sure we don't have any ways or relations, however
    ConstWayPtr emptyWay;
    ConstRelationPtr emptyRelation;
    CPPUNIT_ASSERT_EQUAL( myCache.getNextWay(), emptyWay);
    CPPUNIT_ASSERT_EQUAL( myCache.getNextRelation(), emptyRelation);

    ConstNodePtr emptyNode;
    ConstNodePtr myNodeCopy = myCache.getNextNode();
    CPPUNIT_ASSERT( emptyNode != myNodeCopy );
    CPPUNIT_ASSERT( myNodeCopy->toString() == newNode->toString() );

    // Make sure cache is not empty, but iterators are exhausted
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), false );
  }

  void runAddAllTypesTest()
  {
    ElementCacheLRU myCache(10, 10, 10);  // Max ten elements of each type

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

  void runForceNodeReplacementTest()
  {
    ElementCacheLRU myCache(2, 2, 2);  // Max two elements of each type

    // Add first node
    Status status;
    Meters circularError(4.0);
    ConstElementPtr newNode1 = std::make_shared<const Node>(status, 1, 2.0, 3.0, circularError);
    myCache.addElement(newNode1);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(1) );

    // Add second node
    ConstElementPtr newNode2 = std::make_shared<const Node>(status, 5, 6.0, 7.0, circularError);
    myCache.addElement(newNode2);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(2) );

    // Add third node -- force first node out
    ConstElementPtr newNode3 = std::make_shared<Node>(status, 8, 9.0, 10.0, circularError);
    myCache.addElement(newNode3);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(2) );

    // Read out contents
    ConstNodePtr myNodeCopy = myCache.getNextNode();
    CPPUNIT_ASSERT( myNodeCopy->toString() == newNode2->toString() );
    myNodeCopy = myCache.getNextNode();
    CPPUNIT_ASSERT( myNodeCopy->toString() == newNode3->toString() );

    ConstNodePtr emptyNode;
    myNodeCopy = myCache.getNextNode();
    CPPUNIT_ASSERT( myNodeCopy == emptyNode );
  }

  void runForceWayReplacementTest()
  {
    ElementCacheLRU myCache(2, 2, 2);  // Max two elements of each type

    // Add first way
    Status status;
    Meters circularError(4.0);
    ConstElementPtr newWay1 = std::make_shared<const Way>(status, 1, circularError);
    myCache.addElement(newWay1);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(1) );

    // Add second way
    ConstElementPtr newWay2 = std::make_shared<const Way>(status, 2, circularError);
    myCache.addElement(newWay2);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(2) );

    // Add third way -- force first way out
    ConstElementPtr newWay3 = std::make_shared<const Way>(status, 3, circularError);
    myCache.addElement(newWay3);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(2) );

    // Read out contents
    ConstWayPtr myWayCopy = myCache.getNextWay();
    CPPUNIT_ASSERT( myWayCopy->toString() == newWay2->toString() );
    myWayCopy = myCache.getNextWay();
    CPPUNIT_ASSERT( myWayCopy->toString() == newWay3->toString() );

    ConstWayPtr emptyWay;
    myWayCopy = myCache.getNextWay();
    CPPUNIT_ASSERT( myWayCopy == emptyWay );
  }

  void runForceRelationReplacementTest()
  {
    ElementCacheLRU myCache(2, 2, 2);  // Max two elements of each type

    // Add first relation
    Status status;
    Meters circularError(4.0);
    ConstElementPtr newRelation1 = std::make_shared<const Relation>(status, 1, circularError);
    myCache.addElement(newRelation1);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(1) );

    // Add second relation
    ConstElementPtr newRelation2 = std::make_shared<const Relation>(status, 2, circularError);
    myCache.addElement(newRelation2);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(2) );

    // Add third relation -- force first relation out
    ConstElementPtr newRelation3 = std::make_shared<const Relation>(status, 3, circularError);
    myCache.addElement(newRelation3);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(2) );

    // Read out contents
    ConstRelationPtr myRelationCopy = myCache.getNextRelation();
    CPPUNIT_ASSERT( myRelationCopy->toString() == newRelation2->toString() );
    myRelationCopy = myCache.getNextRelation();
    CPPUNIT_ASSERT( myRelationCopy->toString() == newRelation3->toString() );

    ConstRelationPtr emptyRelation;
    myRelationCopy = myCache.getNextRelation();
    CPPUNIT_ASSERT( myRelationCopy == emptyRelation );
  }

  // Checks to make sure LRU list functions as it should
  void runAlgorithmTest()
  {
    std::string list = "";
    ElementCacheLRU myCache(10, 10, 10);

    // Add 10 nodes
    for (int i = 1; i <= 10; i++)
    {
      Status status(Status::Unknown1);
      Meters circularError(15.0);
      ConstElementPtr newNode = std::make_shared<const Node>(status, i, 2.0, 3.0, circularError);
      myCache.addElement(newNode);
    }

    // Verify LRU order
    // 10 should be most recent, 1 least recent (10 ... 1)
    list = myCache.getLRUString(ElementType::Node);
    CPPUNIT_ASSERT_EQUAL(list, std::string("10,9,8,7,6,5,4,3,2,1,"));

    // Access some stuff
    myCache.getNode(5);
    myCache.getNode(5);
    myCache.getNode(5);
    myCache.getNode(4);

    // Verify LRU order
    // Verify 5 only shows up once
    // Verify 4 is most recent, 5, then prev. order
    list = myCache.getLRUString(ElementType::Node);
    CPPUNIT_ASSERT_EQUAL(list, std::string("4,5,10,9,8,7,6,3,2,1,"));

    // Add more nodes
    for (int i = 11; i <= 15; i++)
    {
      Status status(Status::Unknown1);
      Meters circularError(15.0);
      ConstElementPtr newNode = std::make_shared<const Node>(status, i, 2.0, 3.0, circularError);
      myCache.addElement(newNode);
    }

    // Verify LRU order
    // Make sure the least recently used stuff (7, 6, 5, ect.) got pushed out.
    list = myCache.getLRUString(ElementType::Node);
    CPPUNIT_ASSERT_EQUAL(list, std::string("15,14,13,12,11,4,5,10,9,8,"));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementCacheLruTest, "quick");

}
