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
#include <hoot/rnd/conflate/network/NetworkEdge.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/MapProjector.h>

namespace hoot
{

TEST_UTILS_REGISTER_RESET(NetworkVertex)

class NetworkEdgeTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(NetworkEdgeTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  /**
   * Test basic functionality of NetworkEdge class
   */
  void basicTest()
  {
    // Reset static IDs for testing
    NetworkVertex::reset();

    // Make some nodes and Vertexes
    NodePtr pN1(new Node(Status::Unknown1, -100, Coordinate(1.0, 1.0), 10.0));
    NodePtr pN2(new Node(Status::Unknown1, -200, Coordinate(2.0, 2.0), 10.0));
    NodePtr pN3(new Node(Status::Unknown1, -300, Coordinate(3.0, 3.0), 10.0));
    ConstNetworkVertexPtr pNV1(new NetworkVertex(pN1));
    ConstNetworkVertexPtr pNV2(new NetworkVertex(pN2));
    ConstNetworkVertexPtr pNV3(new NetworkVertex(pN3));

    // Make a map
    OGREnvelope env;
    env.MinX = env.MinY = 0.0;
    env.MaxX = env.MaxY = 2.0;
    shared_ptr<OGRSpatialReference> sref = MapProjector::getInstance().
        createPlanarProjection(env);
    OsmMapPtr pMap(new OsmMap(sref));
    pMap->addElement(pN1);
    pMap->addElement(pN2);
    pMap->addElement(pN3);

    // Test directed edge, contains, addMember
    NetworkEdge myDirectedEdge(pNV1, pNV2, true);
    CPPUNIT_ASSERT_EQUAL(pNV1, myDirectedEdge.getFrom());
    CPPUNIT_ASSERT_EQUAL(pNV2, myDirectedEdge.getTo());
    CPPUNIT_ASSERT(myDirectedEdge.contains(pNV1));
    CPPUNIT_ASSERT(myDirectedEdge.contains(pNV2));
    CPPUNIT_ASSERT(!myDirectedEdge.contains(pNV3));
    myDirectedEdge.addMember(pNV3->getElement());

    // .contains does not check members
    //CPPUNIT_ASSERT(myDirectedEdge.contains(pNV3));

    // calculateLength
    Meters len = myDirectedEdge.calculateLength(pMap);
    CPPUNIT_ASSERT_EQUAL(0.0, len);

    // getMembers
    CPPUNIT_ASSERT_EQUAL(1, myDirectedEdge.getMembers().count());
    CPPUNIT_ASSERT(myDirectedEdge.isDirected());

    // Make a non-directed stub
    NetworkEdge myNonDirectedEdge(pNV1, pNV1, false);
    CPPUNIT_ASSERT(!myNonDirectedEdge.isDirected());

    // isStub
    CPPUNIT_ASSERT(!myDirectedEdge.isStub());
    CPPUNIT_ASSERT(myNonDirectedEdge.isStub());

    // Make the same edge
    // Test equality operator
    NetworkEdge myNonDirectedEdge2(pNV1, pNV1, false);
    CPPUNIT_ASSERT(myNonDirectedEdge == myNonDirectedEdge2);

    // Members test
    QList<ConstElementPtr> members;
    members.push_back(pNV3->getElement());
    myNonDirectedEdge.setMembers(members);
    CPPUNIT_ASSERT_EQUAL(myNonDirectedEdge.getMembers(), members);

    // ToString test
    HOOT_STR_EQUALS("(0) Node:-100 -- Node:-300 --> (1) Node:-200", myDirectedEdge.toString());
    HOOT_STR_EQUALS("(0) Node:-100 -- Node:-300 -- (0) Node:-100", myNonDirectedEdge.toString());

  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NetworkEdgeTest, "quick");

}
