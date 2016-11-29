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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/conflate/network/OsmNetwork.h>
#include <hoot/core/MapProjector.h>

namespace hoot
{

class OsmNetworkTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(OsmNetworkTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  /**
   * @brief basicTest run basic unit tests against OsmNetwork
   */
  void basicTest()
  {
    // Reset static IDs for testing
    NetworkVertex::reset();

    // Setup some vertexes and edges
    NodePtr pN1(new Node(Status::Unknown1, -100, Coordinate(1.0, 1.0), 10.0));
    NodePtr pN2(new Node(Status::Unknown1, -200, Coordinate(2.0, 2.0), 10.0));
    WayPtr  pW1(new Way(Status::Unknown1, -1000, 10));
    NodePtr pN3(new Node(Status::Unknown1, -300, Coordinate(1.0, 2.0), 10.0));
    NodePtr pN4(new Node(Status::Unknown1, -400, Coordinate(2.0, 1.0), 10.0));
    WayPtr  pW2(new Way(Status::Unknown1, -1001, 10));
    ConstNetworkVertexPtr pNV1(new NetworkVertex(pN1));
    ConstNetworkVertexPtr pNV2(new NetworkVertex(pN2));
    ConstNetworkVertexPtr pNV3(new NetworkVertex(pN3));
    ConstNetworkVertexPtr pNV4(new NetworkVertex(pN4));

    NetworkEdgePtr pEdge1(new NetworkEdge(pNV1, pNV2, true));
    pEdge1->addMember(pW1);
    NetworkEdgePtr pEdge2(new NetworkEdge(pNV3, pNV4, true));
    pEdge2->addMember(pW2);

    // Test addVertex
    OsmNetwork ntwrk;
    ntwrk.addVertex(pNV1);
    ntwrk.addVertex(pNV2);
    ntwrk.addVertex(pNV3);
    ntwrk.addVertex(pNV4);

    // addEdge
    ntwrk.addEdge(pEdge1);
    ntwrk.addEdge(pEdge2);

    // toString
    HOOT_STR_EQUALS("(0) Node:-100 -- Way:-1000 --> (1) Node:-200\n"
                    "(2) Node:-300 -- Way:-1001 --> (3) Node:-400",
                    ntwrk.toString());

    // getEdgesFromVertex
    QList<ConstNetworkEdgePtr> pEdges =  ntwrk.getEdgesFromVertex(pNV1);
    CPPUNIT_ASSERT_EQUAL(1, pEdges.count());
    CPPUNIT_ASSERT(pEdge1 == pEdges[0]);

    // getEdgeMap
    OsmNetwork::EdgeMap eMap = ntwrk.getEdgeMap();
    CPPUNIT_ASSERT_EQUAL(2, eMap.count());

    // getSingleVertex
    ConstNetworkVertexPtr pNVTest = ntwrk.getSingleVertex(pNV1->getElementId());
    CPPUNIT_ASSERT(pNVTest == pNV1);

    // getVertexMap
    OsmNetwork::VertexMap vMap = ntwrk.getVertexMap();
    CPPUNIT_ASSERT_EQUAL(4, vMap.count());

    // removeEdge
    ntwrk.removeEdge(pEdge1);
    eMap = ntwrk.getEdgeMap();
    CPPUNIT_ASSERT_EQUAL(1, eMap.count());

    // removeVertex
    ntwrk.removeVertex(pNV1);
    vMap = ntwrk.getVertexMap();
    CPPUNIT_ASSERT_EQUAL(3, vMap.count());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmNetworkTest, "quick");

}
