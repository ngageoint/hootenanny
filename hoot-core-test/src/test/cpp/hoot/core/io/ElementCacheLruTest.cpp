
// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>
// Hoot
#include <hoot/core/io/ElementCacheLRU.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Status.h>

#include "../TestUtils.h"

namespace hoot
{

class ElementCacheLruTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ElementCacheLruTest);
  CPPUNIT_TEST(runCacheSizeTest);
  CPPUNIT_TEST(runCacheEmptyTest);
  CPPUNIT_TEST(runAddSingleNodeTest);
  CPPUNIT_TEST(runAddAllTypesTest);
  CPPUNIT_TEST(runForceNodeReplacementTest);
  CPPUNIT_TEST(runForceWayReplacementTest);
  CPPUNIT_TEST(runForceRelationReplacementTest);
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
    ConstElementPtr newNode(new Node(nodeStatus, 1, 2.0, 3.0, circularError) );
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
    ConstElementPtr newNode(new Node(status, 1, 2.0, 3.0, circularError) );
    myCache.addElement(newNode);
    // Now we should be not empty, iterators have work
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), true );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(1) );

    // Add new way
    ConstElementPtr newWay(new Way(status, 10, circularError));
    myCache.addElement(newWay);
    CPPUNIT_ASSERT_EQUAL( myCache.hasMoreElements(), true );
    CPPUNIT_ASSERT_EQUAL( myCache.isEmpty(), false );
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(2) );

    // Add new relation
    ConstElementPtr newRelation(new Relation(status, 20, circularError));
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
    ConstElementPtr newNode1(new Node(status, 1, 2.0, 3.0, circularError) );
    myCache.addElement(newNode1);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(1) );

    // Add second node
    ConstElementPtr newNode2 = ConstElementPtr(new Node(status, 5, 6.0, 7.0, circularError));
    myCache.addElement(newNode2);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(2) );

    // Add third node -- force first node out
    ConstElementPtr newNode3 = ConstElementPtr(new Node(status, 8, 9.0, 10.0, circularError));
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
    ConstElementPtr newWay1(new Way(status, 1, circularError));
    myCache.addElement(newWay1);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(1) );

    // Add second way
    ConstElementPtr newWay2 = ConstElementPtr(new Way(status, 2, circularError));
    myCache.addElement(newWay2);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(2) );

    // Add third way -- force first way out
    ConstElementPtr newWay3 = ConstElementPtr(new Way(status, 3, circularError));
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
    ConstElementPtr newRelation1(new Relation(status, 1, circularError));
    myCache.addElement(newRelation1);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(1) );

    // Add second relation
    ConstElementPtr newRelation2 = ConstElementPtr(new Relation(status, 2, circularError));
    myCache.addElement(newRelation2);
    CPPUNIT_ASSERT_EQUAL( myCache.size(), static_cast<unsigned long>(2) );

    // Add third relation -- force first relation out
    ConstElementPtr newRelation3 = ConstElementPtr(new Relation(status, 3, circularError));
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

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementCacheLruTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementCacheLruTest, "quick");

}
