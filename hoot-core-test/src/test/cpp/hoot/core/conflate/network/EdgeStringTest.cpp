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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/network/EdgeString.h>
#include <hoot/core/elements/MapProjector.h>

namespace hoot
{

class EdgeStringTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(EdgeStringTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(calculateLengthTest);
  CPPUNIT_TEST(edgeWithInvalidNumMembersTest);
  CPPUNIT_TEST(getEdgeAtOffsetTest);
  CPPUNIT_TEST(overlapTest);
  CPPUNIT_TEST(overlapEdgeTest);
  CPPUNIT_TEST(overlapSublineTest);
  CPPUNIT_TEST(reverseTest);
  CPPUNIT_TEST(partialTest);
  CPPUNIT_TEST(stubTest);
  CPPUNIT_TEST(addDisconnectedEdgeTest);
  CPPUNIT_TEST(equalsTest);
  CPPUNIT_TEST(containsEdgeStringTest);
  CPPUNIT_TEST(appendSublineTest);
  CPPUNIT_TEST(hashTest);
  CPPUNIT_TEST_SUITE_END();

public:

  EdgeStringTest()
  {
    setResetType(ResetEnvironment);
  }

  void basicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ConstNetworkVertexPtr vertex1 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 0, 0));
    ConstNetworkVertexPtr vertex2 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 10, 0));
    ConstNetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    ConstNetworkVertexPtr vertex3 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 20, 0));
    ConstNetworkEdgePtr edge2 = std::make_shared<NetworkEdge>(vertex2, vertex3, true);

    EdgeString edgeStr;
    edgeStr.appendEdge(edge1);
    edgeStr.appendEdge(edge2);

    CPPUNIT_ASSERT_EQUAL(2, edgeStr.getCount());
    CPPUNIT_ASSERT(!edgeStr.isEdgeClosed());
    CPPUNIT_ASSERT(edgeStr.contains(edge1));
    CPPUNIT_ASSERT(edgeStr.contains(edge2));
    ConstEdgeLocationPtr edgeLoc1 = std::make_shared<EdgeLocation>(edge1, 0.0);
    CPPUNIT_ASSERT(edgeStr.getFrom() == edgeLoc1);
    ConstEdgeLocationPtr edgeLoc2 = std::make_shared<EdgeLocation>(edge2, 1.0);
    CPPUNIT_ASSERT(edgeStr.getTo() == edgeLoc2);
    CPPUNIT_ASSERT(edgeStr.getFirstEdge() == edge1);
    CPPUNIT_ASSERT(edgeStr.getLastEdge() == edge2);

    CPPUNIT_ASSERT(edgeStr.getFromVertex() == vertex1);
    CPPUNIT_ASSERT(edgeStr.getToVertex() == vertex3);
    CPPUNIT_ASSERT(edgeStr.isAtExtreme(vertex1));
    CPPUNIT_ASSERT(!edgeStr.isAtExtreme(vertex2));
    CPPUNIT_ASSERT(edgeStr.isAtExtreme(vertex3));
    CPPUNIT_ASSERT(edgeStr.isFromOnVertex());
    CPPUNIT_ASSERT(edgeStr.isToOnVertex());
    CPPUNIT_ASSERT(edgeStr.contains(vertex1));
    CPPUNIT_ASSERT(edgeStr.contains(vertex2));
    CPPUNIT_ASSERT(edgeStr.contains(vertex3));
    CPPUNIT_ASSERT(!edgeStr.containsInteriorVertex(vertex1));
    CPPUNIT_ASSERT(edgeStr.containsInteriorVertex(vertex2));
    CPPUNIT_ASSERT(!edgeStr.containsInteriorVertex(vertex3));

    CPPUNIT_ASSERT(!edgeStr.isFullPartial());
    CPPUNIT_ASSERT(!edgeStr.isStub());

    HOOT_STR_EQUALS(
      "{ _start: { _e: (0) Node(-1) --  --> (1) Node(-2), _portion: 0 }, _end: { _e: (0) Node(-1) --  --> (1) Node(-2), _portion: 1 } }",
      edgeStr.getAllEdges()[0].getSubline()->toString());
    HOOT_STR_EQUALS(
      "{ _start: { _e: (1) Node(-2) --  --> (2) Node(-3), _portion: 0 }, _end: { _e: (1) Node(-2) --  --> (2) Node(-3), _portion: 1 } }",
      edgeStr.getAllEdges()[1].getSubline()->toString());
  }

  void calculateLengthTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    QList<NodePtr> nodes;
    NodePtr node1 = TestUtils::createNode(map, "", Status::Unknown1, 0, 0);
    nodes.append(node1);
    ConstNetworkVertexPtr vertex1 = std::make_shared<NetworkVertex>(node1);
    NodePtr node2 = TestUtils::createNode(map, "", Status::Unknown1, 10, 0);
    nodes.append(node2);
    ConstNetworkVertexPtr vertex2 = std::make_shared<NetworkVertex>(node2);
    NetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    ConstWayPtr way = TestUtils::createWay(map, nodes, "", Status::Unknown1);
    edge1->addMember(way);
    MapProjector::projectToPlanar(map);

    EdgeString edgeStr;
    edgeStr.appendEdge(edge1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1111782.53516264, edgeStr.calculateLength(map), 0.0001);
  }

  void edgeWithInvalidNumMembersTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    QList<NodePtr> nodes;
    NodePtr node1 = TestUtils::createNode(map, "", Status::Unknown1, 0, 0);
    nodes.append(node1);
    ConstNetworkVertexPtr vertex1 = std::make_shared<NetworkVertex>(node1);
    NodePtr node2 = TestUtils::createNode(map, "", Status::Unknown1, 10, 0);
    nodes.append(node2);
    ConstNetworkVertexPtr vertex2 = std::make_shared<NetworkVertex>(node2);
    NetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    ConstWayPtr way1 = TestUtils::createWay(map, nodes, "", Status::Unknown1);
    edge1->addMember(way1);
    ConstWayPtr way2 = TestUtils::createWay(map, nodes, "", Status::Unknown1);
    edge1->addMember(way2);

    const QString expectedErrMsg = "Edges with multiple members are not yet supported.";

    //edge with multiple members

    EdgeString edgeStr1;
    edgeStr1.appendEdge(edge1);

    QString exceptionMsg("");
    try
    {
      edgeStr1.calculateLength(map);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(expectedErrMsg, exceptionMsg.toStdString());

    //edge with no members

    NetworkEdgePtr edge2 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    EdgeString edgeStr2;
    edgeStr2.appendEdge(edge2);
    exceptionMsg = "";
    try
    {
      edgeStr2.calculateLength(map);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(expectedErrMsg, exceptionMsg.toStdString());
  }

  void getEdgeAtOffsetTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    QList<NodePtr> nodes;
    NodePtr node1 = TestUtils::createNode(map, "", Status::Unknown1, 0, 0);
    nodes.append(node1);
    ConstNetworkVertexPtr vertex1 = std::make_shared<NetworkVertex>(node1);
    NodePtr node2 = TestUtils::createNode(map, "", Status::Unknown1, 10, 0);
    nodes.append(node2);
    ConstNetworkVertexPtr vertex2 = std::make_shared<NetworkVertex>(node2);
    NetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    ConstWayPtr way = TestUtils::createWay(map, nodes, "", Status::Unknown1);
    edge1->addMember(way);
    ConstNetworkVertexPtr vertex3 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 20, 0));
    NetworkEdgePtr edge2 = std::make_shared<NetworkEdge>(vertex2, vertex3, true);
    edge2->addMember(way);
    MapProjector::projectToPlanar(map);

    EdgeString edgeStr;
    edgeStr.appendEdge(edge1);
    edgeStr.appendEdge(edge2);
    CPPUNIT_ASSERT(edge1 == edgeStr.getEdgeAtOffset(map, 0.0));
    CPPUNIT_ASSERT(edge2 == edgeStr.getEdgeAtOffset(map, edgeStr.calculateLength(map)));
  }

  void overlapTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ConstNetworkVertexPtr vertex1 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 0, 0));
    ConstNetworkVertexPtr vertex2 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 10, 0));
    ConstNetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    ConstEdgeLocationPtr edgeLoc1 = std::make_shared<EdgeLocation>(edge1, 0.0);
    ConstEdgeLocationPtr edgeLoc2 = std::make_shared<EdgeLocation>(edge1, 0.5);
    ConstEdgeLocationPtr edgeLoc3 = std::make_shared<EdgeLocation>(edge1, 0.7);
    ConstEdgeSublinePtr edgeSubline1 = std::make_shared<EdgeSubline>(edgeLoc1, edgeLoc2);
    ConstEdgeSublinePtr edgeSubline2 = std::make_shared<EdgeSubline>(edgeLoc1, edgeLoc3);
    ConstEdgeSublinePtr edgeSubline3 = std::make_shared<EdgeSubline>(edgeLoc2, edgeLoc3);

    EdgeStringPtr edgeStr1 = std::make_shared<EdgeString>();
    edgeStr1->appendEdge(edgeSubline1);
    EdgeStringPtr edgeStr2 = std::make_shared<EdgeString>();
    edgeStr2->appendEdge(edgeSubline2);
    EdgeStringPtr edgeStr3 = std::make_shared<EdgeString>();
    edgeStr3->appendEdge(edgeSubline3);

    CPPUNIT_ASSERT(edgeStr1->overlaps(edgeStr2));
    CPPUNIT_ASSERT(!edgeStr1->overlaps(edgeStr3));
  }

  void overlapEdgeTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ConstNetworkVertexPtr vertex1 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 0, 0));
    ConstNetworkVertexPtr vertex2 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 10, 0));
    ConstNetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    ConstNetworkVertexPtr vertex3 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 20, 0));
    ConstNetworkEdgePtr edge2 = std::make_shared<NetworkEdge>(vertex2, vertex3, true);
    ConstEdgeLocationPtr edgeLoc1 = std::make_shared<EdgeLocation>(edge1, 0.0);
    ConstEdgeLocationPtr edgeLoc2 = std::make_shared<EdgeLocation>(edge1, 0.5);
    ConstEdgeLocationPtr edgeLoc3 = std::make_shared<EdgeLocation>(edge2, 0.7);
    ConstEdgeSublinePtr edgeSubline = std::make_shared<EdgeSubline>(edgeLoc1, edgeLoc2);

    EdgeStringPtr edgeStr1 = std::make_shared<EdgeString>();
    edgeStr1->appendEdge(edgeSubline);

    CPPUNIT_ASSERT(edgeStr1->overlaps(edgeLoc1->getEdge()));
    CPPUNIT_ASSERT(!edgeStr1->overlaps(edgeLoc3->getEdge()));
  }

  void overlapSublineTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, "", Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 10, 0));
    ConstNetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    ConstEdgeLocationPtr edgeLoc1 = std::make_shared<EdgeLocation>(edge1, 0.0);
    ConstEdgeLocationPtr edgeLoc2 = std::make_shared<EdgeLocation>(edge1, 0.5);
    ConstEdgeLocationPtr edgeLoc3 = std::make_shared<EdgeLocation>(edge1, 0.7);
    ConstEdgeSublinePtr edgeSubline1 = std::make_shared<EdgeSubline>(edgeLoc1, edgeLoc2);
    ConstEdgeSublinePtr edgeSubline2 = std::make_shared<EdgeSubline>(edgeLoc1, edgeLoc3);
    ConstEdgeSublinePtr edgeSubline3 = std::make_shared<EdgeSubline>(edgeLoc2, edgeLoc3);

    EdgeStringPtr edgeStr1 = std::make_shared<EdgeString>();
    edgeStr1->appendEdge(edgeSubline1);

    CPPUNIT_ASSERT(edgeStr1->overlaps(edgeSubline2));
    CPPUNIT_ASSERT(!edgeStr1->overlaps(edgeSubline3));
  }

  void reverseTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ConstNetworkVertexPtr vertex1 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 0, 0));
    ConstNetworkVertexPtr vertex2 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 10, 0));
    ConstNetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    ConstNetworkVertexPtr vertex3 =
       std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 20, 0));
    ConstNetworkEdgePtr edge2 = std::make_shared<NetworkEdge>(vertex2, vertex3, true);

    EdgeString edgeStr;
    edgeStr.appendEdge(edge1);
    edgeStr.appendEdge(edge2);
    edgeStr.reverse();

    CPPUNIT_ASSERT_EQUAL(2, edgeStr.getCount());
    CPPUNIT_ASSERT(edgeStr.contains(edge1));
    CPPUNIT_ASSERT(edgeStr.contains(edge2));
    ConstEdgeLocationPtr edgeLoc1 = std::make_shared<EdgeLocation>(edge1, 0.0);
    CPPUNIT_ASSERT(edgeStr.getTo() == edgeLoc1);
    ConstEdgeLocationPtr edgeLoc2 = std::make_shared<EdgeLocation>(edge2, 1.0);
    CPPUNIT_ASSERT(edgeStr.getFrom() == edgeLoc2);
    CPPUNIT_ASSERT(edgeStr.getFirstEdge() == edge2);
    CPPUNIT_ASSERT(edgeStr.getLastEdge() == edge1);

    CPPUNIT_ASSERT(edgeStr.getFromVertex() == vertex3);
    CPPUNIT_ASSERT(edgeStr.getToVertex() == vertex1);
    CPPUNIT_ASSERT(edgeStr.isAtExtreme(vertex1));
    CPPUNIT_ASSERT(!edgeStr.isAtExtreme(vertex2));
    CPPUNIT_ASSERT(edgeStr.isAtExtreme(vertex3));
    CPPUNIT_ASSERT(edgeStr.isFromOnVertex());
    CPPUNIT_ASSERT(edgeStr.isToOnVertex());
    CPPUNIT_ASSERT(edgeStr.contains(vertex1));
    CPPUNIT_ASSERT(edgeStr.contains(vertex2));
    CPPUNIT_ASSERT(edgeStr.contains(vertex3));
    CPPUNIT_ASSERT(!edgeStr.containsInteriorVertex(vertex1));
    CPPUNIT_ASSERT(edgeStr.containsInteriorVertex(vertex2));
    CPPUNIT_ASSERT(!edgeStr.containsInteriorVertex(vertex3));
  }

  void partialTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ConstNetworkVertexPtr vertex1 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 0, 0));
    ConstNetworkVertexPtr vertex2 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 10, 0));
    ConstNetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    ConstNetworkVertexPtr vertex3 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 20, 0));
    ConstEdgeLocationPtr edgeLocStart1 = std::make_shared<EdgeLocation>(edge1, 0.0);
    ConstEdgeLocationPtr edgeLocStart2 = std::make_shared<EdgeLocation>(edge1, 0.1);
    ConstEdgeLocationPtr edgeLocEnd = std::make_shared<EdgeLocation>(edge1, 0.9);
    ConstEdgeSublinePtr edgeSubline1 = std::make_shared<EdgeSubline>(edgeLocStart1, edgeLocEnd);
    ConstEdgeSublinePtr edgeSubline2 = std::make_shared<EdgeSubline>(edgeLocStart2, edgeLocEnd);

    EdgeString edgeStr1;
    edgeStr1.appendEdge(edgeSubline1);
    CPPUNIT_ASSERT(edgeStr1.isPartial());
    CPPUNIT_ASSERT(!edgeStr1.isFullPartial());

    EdgeString edgeStr2;
    edgeStr2.appendEdge(edgeSubline2);
    CPPUNIT_ASSERT(edgeStr2.isPartial());
    CPPUNIT_ASSERT(edgeStr2.isFullPartial());
  }

  void stubTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ConstNetworkVertexPtr vertex1 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 0, 0));
    ConstNetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex1, true);

    EdgeString edgeStr;
    edgeStr.appendEdge(edge1);
    CPPUNIT_ASSERT(edgeStr.isStub());
  }

  void addDisconnectedEdgeTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ConstNetworkVertexPtr vertex1 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 0, 0));
    ConstNetworkVertexPtr vertex2 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 10, 0));
    ConstNetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    ConstNetworkVertexPtr vertex3 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 20, 0));
    ConstNetworkVertexPtr vertex4 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 30, 0));
    ConstNetworkEdgePtr edge2 = std::make_shared<NetworkEdge>(vertex3, vertex4, true);

    EdgeString edgeStr;
    edgeStr.appendEdge(edge1);
    QString exceptionMsg("");
    try
    {
      edgeStr.appendEdge(edge2);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "Error attempting to append an edge that isn't connected.", exceptionMsg.toStdString());
  }

  void equalsTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ConstNetworkVertexPtr vertex1 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 0, 0));
    ConstNetworkVertexPtr vertex2 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 10, 0));
    ConstNetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    ConstNetworkVertexPtr vertex3 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 20, 0));
    ConstNetworkEdgePtr edge2 = std::make_shared<NetworkEdge>(vertex2, vertex3, true);

    EdgeStringPtr edgeStr1 = std::make_shared<EdgeString>();
    edgeStr1->appendEdge(edge1);
    edgeStr1->appendEdge(edge2);

    EdgeStringPtr edgeStr2 = std::make_shared<EdgeString>();
    edgeStr2->appendEdge(edge1);
    edgeStr2->appendEdge(edge2);

    ConstEdgeStringPtr edgeStr4 = edgeStr1->clone();
    ConstEdgeStringPtr edgeStr5 = edgeStr2->clone();
    CPPUNIT_ASSERT(edgeStr4 == edgeStr5);

    EdgeStringPtr edgeStr3 = std::make_shared<EdgeString>();
    edgeStr3->appendEdge(edge1);

    ConstEdgeStringPtr edgeStr6 = edgeStr3->clone();
    CPPUNIT_ASSERT(!(edgeStr4 == edgeStr6));
  }

  void containsEdgeStringTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ConstNetworkVertexPtr vertex1 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 0, 0));
    ConstNetworkVertexPtr vertex2 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 10, 0));
    ConstNetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    ConstNetworkVertexPtr vertex3 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 20, 0));
    ConstNetworkEdgePtr edge2 = std::make_shared<NetworkEdge>(vertex2, vertex3, true);
    ConstNetworkVertexPtr vertex4 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 30, 0));
    ConstNetworkEdgePtr edge3 = std::make_shared<NetworkEdge>(vertex3, vertex4, true);

    EdgeStringPtr edgeStr1 = std::make_shared<EdgeString>();
    edgeStr1->appendEdge(edge1);
    edgeStr1->appendEdge(edge2);

    EdgeStringPtr edgeStr2 = std::make_shared<EdgeString>();
    edgeStr2->appendEdge(edge2);

    EdgeStringPtr edgeStr3 = std::make_shared<EdgeString>();
    edgeStr3->appendEdge(edge3);

    CPPUNIT_ASSERT(edgeStr1->contains(edgeStr2));
    CPPUNIT_ASSERT(!edgeStr1->contains(edgeStr3));
  }

  void appendSublineTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ConstNetworkVertexPtr vertex1 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 0, 0));
    ConstNetworkVertexPtr vertex2 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 10, 0));
    ConstNetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    ConstNetworkVertexPtr vertex3 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 20, 0));
    ConstNetworkEdgePtr edge2 = std::make_shared<NetworkEdge>(vertex2, vertex3, true);
    ConstEdgeLocationPtr edgeLocStart = std::make_shared<EdgeLocation>(edge1, 0.0);
    ConstEdgeLocationPtr edgeLocEnd = std::make_shared<EdgeLocation>(edge1, 1.0);
    ConstEdgeSublinePtr edgeSubline1 = std::make_shared<EdgeSubline>(edgeLocStart, edgeLocEnd);
    ConstEdgeLocationPtr edgeLocStart2 = std::make_shared<EdgeLocation>(edge2, 0.0);
    ConstEdgeLocationPtr edgeLocEnd2 = std::make_shared<EdgeLocation>(edge2, 1.0);
    ConstEdgeSublinePtr edgeSubline2 = std::make_shared<EdgeSubline>(edgeLocStart2, edgeLocEnd2);

    EdgeString edgeStr;
    edgeStr.appendEdge(edgeSubline1);
    edgeStr.appendEdge(edgeSubline2);

    CPPUNIT_ASSERT_EQUAL(2, edgeStr.getCount());
    CPPUNIT_ASSERT(!edgeStr.isEdgeClosed());
    CPPUNIT_ASSERT(edgeStr.contains(edge1));
    CPPUNIT_ASSERT(edgeStr.contains(edge2));
    CPPUNIT_ASSERT(edgeStr.getFrom() == edgeLocStart);
    CPPUNIT_ASSERT(edgeStr.getTo() == edgeLocEnd2);
    CPPUNIT_ASSERT(edgeStr.getFirstEdge() == edge1);
    CPPUNIT_ASSERT(edgeStr.getLastEdge() == edge2);

    CPPUNIT_ASSERT(edgeStr.getFromVertex() == vertex1);
    CPPUNIT_ASSERT(edgeStr.getToVertex() == vertex3);
    CPPUNIT_ASSERT(edgeStr.isAtExtreme(vertex1));
    CPPUNIT_ASSERT(!edgeStr.isAtExtreme(vertex2));
    CPPUNIT_ASSERT(edgeStr.isAtExtreme(vertex3));
    CPPUNIT_ASSERT(edgeStr.isFromOnVertex());
    CPPUNIT_ASSERT(edgeStr.isToOnVertex());
    CPPUNIT_ASSERT(edgeStr.contains(vertex1));
    CPPUNIT_ASSERT(edgeStr.contains(vertex2));
    CPPUNIT_ASSERT(edgeStr.contains(vertex3));
    CPPUNIT_ASSERT(!edgeStr.containsInteriorVertex(vertex1));
    CPPUNIT_ASSERT(edgeStr.containsInteriorVertex(vertex2));
    CPPUNIT_ASSERT(!edgeStr.containsInteriorVertex(vertex3));

    CPPUNIT_ASSERT(!edgeStr.isFullPartial());
    CPPUNIT_ASSERT(!edgeStr.isStub());
  }

  void hashTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ConstNetworkVertexPtr vertex1 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 0, 0));
    ConstNetworkVertexPtr vertex2 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 10, 0));
    ConstNetworkEdgePtr edge1 = std::make_shared<NetworkEdge>(vertex1, vertex2, true);
    ConstNetworkVertexPtr vertex3 =
      std::make_shared<NetworkVertex>(TestUtils::createNode(map, "", Status::Unknown1, 20, 0));
    ConstNetworkEdgePtr edge2 = std::make_shared<NetworkEdge>(vertex2, vertex3, true);

    EdgeStringPtr edgeStr = std::make_shared<EdgeString>();
    edgeStr->appendEdge(edge1);
    edgeStr->appendEdge(edge2);

    QHash<EdgeStringPtr, QString> edgeStrings;
    edgeStrings.insert(edgeStr, "test");
    CPPUNIT_ASSERT_EQUAL(1, edgeStrings.size());
    CPPUNIT_ASSERT(edgeStrings.value(edgeStr) == "test");
    CPPUNIT_ASSERT(edgeStrings.key("test") == edgeStr);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EdgeStringTest, "quick");

}
