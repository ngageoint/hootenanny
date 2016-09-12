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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/rnd/conflate/network/EdgeString.h>
#include <hoot/core/elements/Node.h>
#include <hoot/rnd/conflate/network/EdgeLocation.h>
#include <hoot/rnd/conflate/network/NetworkVertex.h>
#include <hoot/core/TestUtils.h>

namespace hoot
{

class EdgeStringTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(EdgeStringTest);
  //CPPUNIT_TEST(runBasicTest);
  //CPPUNIT_TEST(equalTest);
  //CPPUNIT_TEST(addTest);
  //CPPUNIT_TEST(appendTest);
  //CPPUNIT_TEST(calculateLengthTest);
  //CPPUNIT_TEST(containsTest);
  //CPPUNIT_TEST(containsInteriorVertexTest);
  //CPPUNIT_TEST(getAllEdgesTest);
  //CPPUNIT_TEST(getCountTest);
  //CPPUNIT_TEST(getEdgeTest);
  //CPPUNIT_TEST(getEdgeSublineTest);
  //CPPUNIT_TEST(getEdgeSetTest);
  //CPPUNIT_TEST(getEdgeAtOffsetTest);
  //CPPUNIT_TEST(getFromTest);
  //CPPUNIT_TEST(getFromVertexTest);
  //CPPUNIT_TEST(getFirstEdgeTest);
  //CPPUNIT_TEST(getLastEdgeTest);
  //CPPUNIT_TEST(getMembersTest);
  //CPPUNIT_TEST(getToTest);
  //CPPUNIT_TEST(getToVertexTest);
  //CPPUNIT_TEST(isEdgeClosedTest);
  //CPPUNIT_TEST(isAtExtremeTest);
  //CPPUNIT_TEST(isFromOnVertexTest);
  //CPPUNIT_TEST(isFullPartialTest);
  //CPPUNIT_TEST(isPartialTest);
  //CPPUNIT_TEST(isStubTest);
  //CPPUNIT_TEST(isToOnVertexTest);
  //CPPUNIT_TEST(overlapsTest);
  //CPPUNIT_TEST(prependEdgeTest);
  //CPPUNIT_TEST(reverseTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    shared_ptr<OsmMap> map(new OsmMap());

    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));

    ConstNetworkVertexPtr vertex3(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 20, 0)));
    ConstNetworkEdgePtr edge2(new NetworkEdge(vertex2, vertex3, true));

    ConstNetworkVertexPtr vertex4(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkVertexPtr vertex5(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 30, 0)));
    ConstNetworkEdgePtr edge3(new NetworkEdge(vertex4, vertex5, true));

    EdgeString edgeStr;
    edgeStr.appendEdge(edge1);
    LOG_VARD(edgeStr.getToVertex());
    edgeStr.appendEdge(edge2);

    LOG_VARD(edgeStr.getAllEdges());
    LOG_VARD(edgeStr.getCount());
    LOG_VARD(edgeStr.getEdge(0));
    LOG_VARD(edgeStr.getEdgeSubline(0));
    LOG_VARD(edgeStr.getEdgeSet());
    //LOG_VARD(edgeStr.calculateLength(map));
    LOG_VARD(edgeStr.getEdgeSet());
    //LOG_VARD(edgeStr.getEdgeAtOffset(map, 10));
    LOG_VARD(edgeStr.getFrom());
    LOG_VARD(edgeStr.getTo());
    LOG_VARD(edgeStr.getFromVertex());
    LOG_VARD(edgeStr.getToVertex());
    LOG_VARD(edgeStr.getFirstEdge());
    LOG_VARD(edgeStr.getLastEdge());
    LOG_VARD(edgeStr.getMembers());
    LOG_VARD(edgeStr.isEdgeClosed());
    LOG_VARD(edgeStr.isAtExtreme(vertex1));
    LOG_VARD(edgeStr.isAtExtreme(vertex2));
    LOG_VARD(edgeStr.isFromOnVertex());
    LOG_VARD(edgeStr.isToOnVertex());
    LOG_VARD(edgeStr.isFullPartial());
    LOG_VARD(edgeStr.isStub());
  }

  void equalTest()
  {

  }

  void addTest()
  {

  }

  void appendTest()
  {

  }

  void calculateLengthTest()
  {

  }

  void containsTest()
  {

  }

  void containsInteriorVertexTest()
  {

  }

  void getAllEdgesTest()
  {

  }

  void getCountTest()
  {

  }

  void getEdgeTest()
  {

  }

  void getEdgeSublineTest()
  {

  }

  void getEdgeSetTest()
  {

  }

  void getEdgeAtOffsetTest()
  {

  }

  void getFromTest()
  {

  }

  void getFromVertexTest()
  {

  }

  void getFirstEdgeTest()
  {

  }

  void getLastEdgeTest()
  {

  }

  void getMembersTest()
  {

  }

  void getToTest()
  {

  }

  void getToVertexTest()
  {

  }

  void isEdgeClosedTest()
  {

  }

  void isAtExtremeTest()
  {

  }

  void isFromOnVertexTest()
  {

  }

  void isFullPartialTest()
  {

  }

  void isPartialTest()
  {

  }

  void isStubTest()
  {

  }

  void isToOnVertexTest()
  {

  }

  void overlapsTest()
  {

  }

  void prependEdgeTest()
  {

  }

  void reverseTest()
  {

  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EdgeStringTest, "quick");

}
