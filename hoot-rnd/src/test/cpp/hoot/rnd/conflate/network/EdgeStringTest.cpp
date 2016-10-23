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
#include <hoot/core/MapProjector.h>
#include <hoot/rnd/conflate/network/EdgeString.h>
#include <hoot/core/TestUtils.h>

namespace hoot
{

class EdgeStringTest : public CppUnit::TestFixture
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
  CPPUNIT_TEST(edgeClosedTest);
  CPPUNIT_TEST(addDisconnectedEdgeTest);
  CPPUNIT_TEST(equalsTest);
  CPPUNIT_TEST(containsEdgeStringTest);
  CPPUNIT_TEST(appendSublineTest);
  CPPUNIT_TEST(hashTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void basicTest()
  {
    TestUtils::resetEnvironment();

    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstNetworkVertexPtr vertex3(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 20, 0)));
    ConstNetworkEdgePtr edge2(new NetworkEdge(vertex2, vertex3, true));

    EdgeString edgeStr;
    edgeStr.appendEdge(edge1);
    edgeStr.appendEdge(edge2);

    CPPUNIT_ASSERT_EQUAL(2, edgeStr.getCount());
    CPPUNIT_ASSERT(!edgeStr.isEdgeClosed());
    CPPUNIT_ASSERT(edgeStr.contains(edge1));
    CPPUNIT_ASSERT(edgeStr.contains(edge2));
    ConstEdgeLocationPtr edgeLoc1(new EdgeLocation(edge1, 0.0));
    CPPUNIT_ASSERT(edgeStr.getFrom() == edgeLoc1);
    ConstEdgeLocationPtr edgeLoc2(new EdgeLocation(edge2, 1.0));
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
      "{ _start: { _e: (0) Node:-1 --  --> (1) Node:-2, _portion: 0 }, _end: { _e: (0) Node:-1 --  --> (1) Node:-2, _portion: 1 } }",
      edgeStr.getAllEdges()[0].getSubline()->toString());
    HOOT_STR_EQUALS(
      "{ _start: { _e: (1) Node:-2 --  --> (2) Node:-3, _portion: 0 }, _end: { _e: (1) Node:-2 --  --> (2) Node:-3, _portion: 1 } }",
      edgeStr.getAllEdges()[1].getSubline()->toString());
  }

  void calculateLengthTest()
  {
    OsmMapPtr map(new OsmMap());
    QList<NodePtr> nodes;
    NodePtr node1 = TestUtils::createNode(map, Status::Unknown1, 0, 0);
    nodes.append(node1);
    ConstNetworkVertexPtr vertex1(new NetworkVertex(node1));
    NodePtr node2 = TestUtils::createNode(map, Status::Unknown1, 10, 0);
    nodes.append(node2);
    ConstNetworkVertexPtr vertex2(new NetworkVertex(node2));
    NetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstWayPtr way = TestUtils::createWay(map, nodes);
    edge1->addMember(way);
    MapProjector::projectToPlanar(map);

    EdgeString edgeStr;
    edgeStr.appendEdge(edge1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1111782.53516264, edgeStr.calculateLength(map), 0.0001);
  }

  void edgeWithInvalidNumMembersTest()
  {
    OsmMapPtr map(new OsmMap());
    QList<NodePtr> nodes;
    NodePtr node1 = TestUtils::createNode(map, Status::Unknown1, 0, 0);
    nodes.append(node1);
    ConstNetworkVertexPtr vertex1(new NetworkVertex(node1));
    NodePtr node2 = TestUtils::createNode(map, Status::Unknown1, 10, 0);
    nodes.append(node2);
    ConstNetworkVertexPtr vertex2(new NetworkVertex(node2));
    NetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstWayPtr way1 = TestUtils::createWay(map, nodes);
    edge1->addMember(way1);
    ConstWayPtr way2 = TestUtils::createWay(map, nodes);
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
    catch (HootException e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(expectedErrMsg, exceptionMsg.toStdString());

    //edge with no members

    NetworkEdgePtr edge2(new NetworkEdge(vertex1, vertex2, true));
    EdgeString edgeStr2;
    edgeStr2.appendEdge(edge2);
    exceptionMsg = "";
    try
    {
      edgeStr2.calculateLength(map);
    }
    catch (HootException e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(expectedErrMsg, exceptionMsg.toStdString());
  }

  void getEdgeAtOffsetTest()
  {
    OsmMapPtr map(new OsmMap());
    QList<NodePtr> nodes;
    NodePtr node1 = TestUtils::createNode(map, Status::Unknown1, 0, 0);
    nodes.append(node1);
    ConstNetworkVertexPtr vertex1(new NetworkVertex(node1));
    NodePtr node2 = TestUtils::createNode(map, Status::Unknown1, 10, 0);
    nodes.append(node2);
    ConstNetworkVertexPtr vertex2(new NetworkVertex(node2));
    NetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstWayPtr way = TestUtils::createWay(map, nodes);
    edge1->addMember(way);
    ConstNetworkVertexPtr vertex3(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 20, 0)));
    NetworkEdgePtr edge2(new NetworkEdge(vertex2, vertex3, true));
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
    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstEdgeLocationPtr edgeLoc1(new EdgeLocation(edge1, 0.0));
    ConstEdgeLocationPtr edgeLoc2(new EdgeLocation(edge1, 0.5));
    ConstEdgeLocationPtr edgeLoc3(new EdgeLocation(edge1, 0.7));
    ConstEdgeSublinePtr edgeSubline1(new EdgeSubline(edgeLoc1, edgeLoc2));
    ConstEdgeSublinePtr edgeSubline2(new EdgeSubline(edgeLoc1, edgeLoc3));
    ConstEdgeSublinePtr edgeSubline3(new EdgeSubline(edgeLoc2, edgeLoc3));

    EdgeStringPtr edgeStr1(new EdgeString());
    edgeStr1->appendEdge(edgeSubline1);
    EdgeStringPtr edgeStr2(new EdgeString());
    edgeStr2->appendEdge(edgeSubline2);
    EdgeStringPtr edgeStr3(new EdgeString());
    edgeStr3->appendEdge(edgeSubline3);

    CPPUNIT_ASSERT(edgeStr1->overlaps(edgeStr2));
    CPPUNIT_ASSERT(!edgeStr1->overlaps(edgeStr3));
  }

  void overlapEdgeTest()
  {
    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstNetworkVertexPtr vertex3(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 20, 0)));
    ConstNetworkEdgePtr edge2(new NetworkEdge(vertex2, vertex3, true));
    ConstEdgeLocationPtr edgeLoc1(new EdgeLocation(edge1, 0.0));
    ConstEdgeLocationPtr edgeLoc2(new EdgeLocation(edge1, 0.5));
    ConstEdgeLocationPtr edgeLoc3(new EdgeLocation(edge2, 0.7));
    ConstEdgeSublinePtr edgeSubline(new EdgeSubline(edgeLoc1, edgeLoc2));

    EdgeStringPtr edgeStr1(new EdgeString());
    edgeStr1->appendEdge(edgeSubline);

    CPPUNIT_ASSERT(edgeStr1->overlaps(edgeLoc1->getEdge()));
    CPPUNIT_ASSERT(!edgeStr1->overlaps(edgeLoc3->getEdge()));
  }

  void overlapSublineTest()
  {
    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstEdgeLocationPtr edgeLoc1(new EdgeLocation(edge1, 0.0));
    ConstEdgeLocationPtr edgeLoc2(new EdgeLocation(edge1, 0.5));
    ConstEdgeLocationPtr edgeLoc3(new EdgeLocation(edge1, 0.7));
    ConstEdgeSublinePtr edgeSubline1(new EdgeSubline(edgeLoc1, edgeLoc2));
    ConstEdgeSublinePtr edgeSubline2(new EdgeSubline(edgeLoc1, edgeLoc3));
    ConstEdgeSublinePtr edgeSubline3(new EdgeSubline(edgeLoc2, edgeLoc3));

    EdgeStringPtr edgeStr1(new EdgeString());
    edgeStr1->appendEdge(edgeSubline1);

    CPPUNIT_ASSERT(edgeStr1->overlaps(edgeSubline2));
    CPPUNIT_ASSERT(!edgeStr1->overlaps(edgeSubline3));
  }

  void reverseTest()
  {
    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstNetworkVertexPtr vertex3(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 20, 0)));
    ConstNetworkEdgePtr edge2(new NetworkEdge(vertex2, vertex3, true));

    EdgeString edgeStr;
    edgeStr.appendEdge(edge1);
    edgeStr.appendEdge(edge2);
    edgeStr.reverse();

    CPPUNIT_ASSERT_EQUAL(2, edgeStr.getCount());
    CPPUNIT_ASSERT(edgeStr.contains(edge1));
    CPPUNIT_ASSERT(edgeStr.contains(edge2));
    ConstEdgeLocationPtr edgeLoc1(new EdgeLocation(edge1, 0.0));
    CPPUNIT_ASSERT(edgeStr.getTo() == edgeLoc1);
    ConstEdgeLocationPtr edgeLoc2(new EdgeLocation(edge2, 1.0));
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
    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstNetworkVertexPtr vertex3(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 20, 0)));
    ConstEdgeLocationPtr edgeLocStart1(new EdgeLocation(edge1, 0.0));
    ConstEdgeLocationPtr edgeLocStart2(new EdgeLocation(edge1, 0.1));
    ConstEdgeLocationPtr edgeLocEnd(new EdgeLocation(edge1, 0.9));
    ConstEdgeSublinePtr edgeSubline1(new EdgeSubline(edgeLocStart1, edgeLocEnd));
    ConstEdgeSublinePtr edgeSubline2(new EdgeSubline(edgeLocStart2, edgeLocEnd));

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
    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex1, true));

    EdgeString edgeStr;
    edgeStr.appendEdge(edge1);
    CPPUNIT_ASSERT(edgeStr.isStub());
  }

  void edgeClosedTest()
  {
    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge(new NetworkEdge(vertex1, vertex2, true));
    ConstEdgeLocationPtr edgeLocStart(new EdgeLocation(edge, 0.0));
    ConstEdgeSublinePtr edgeSubline(new EdgeSubline(edgeLocStart, edgeLocStart));

    EdgeString edgeStr;
    edgeStr.appendEdge(edgeSubline);
    CPPUNIT_ASSERT(edgeStr.isEdgeClosed());

    QString exceptionMsg("");
    try
    {
      edgeStr.appendEdge(edgeSubline);
    }
    catch (HootException e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "Illegal argument, you attempted to add an edge to an edge string that is already closed. Ends in a partial match?",
      exceptionMsg.toStdString());
  }

  void addDisconnectedEdgeTest()
  {
    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstNetworkVertexPtr vertex3(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 20, 0)));
    ConstNetworkVertexPtr vertex4(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 30, 0)));
    ConstNetworkEdgePtr edge2(new NetworkEdge(vertex3, vertex4, true));

    EdgeString edgeStr;
    edgeStr.appendEdge(edge1);
    QString exceptionMsg("");
    try
    {
      edgeStr.appendEdge(edge2);
    }
    catch (HootException e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "Error attempting to append an edge that isn't connected.", exceptionMsg.toStdString());
  }

  void equalsTest()
  {
    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstNetworkVertexPtr vertex3(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 20, 0)));
    ConstNetworkEdgePtr edge2(new NetworkEdge(vertex2, vertex3, true));

    EdgeStringPtr edgeStr1(new EdgeString());
    edgeStr1->appendEdge(edge1);
    edgeStr1->appendEdge(edge2);

    EdgeStringPtr edgeStr2(new EdgeString());
    edgeStr2->appendEdge(edge1);
    edgeStr2->appendEdge(edge2);

    ConstEdgeStringPtr edgeStr4 = edgeStr1->clone();
    ConstEdgeStringPtr edgeStr5 = edgeStr2->clone();
    CPPUNIT_ASSERT(edgeStr4 == edgeStr5);

    EdgeStringPtr edgeStr3(new EdgeString());
    edgeStr3->appendEdge(edge1);

    ConstEdgeStringPtr edgeStr6 = edgeStr3->clone();
    CPPUNIT_ASSERT(!(edgeStr4 == edgeStr6));
  }

  void containsEdgeStringTest()
  {
    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstNetworkVertexPtr vertex3(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 20, 0)));
    ConstNetworkEdgePtr edge2(new NetworkEdge(vertex2, vertex3, true));
    ConstNetworkVertexPtr vertex4(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 30, 0)));
    ConstNetworkEdgePtr edge3(new NetworkEdge(vertex3, vertex4, true));

    EdgeStringPtr edgeStr1(new EdgeString());
    edgeStr1->appendEdge(edge1);
    edgeStr1->appendEdge(edge2);

    EdgeStringPtr edgeStr2(new EdgeString());
    edgeStr2->appendEdge(edge2);

    EdgeStringPtr edgeStr3(new EdgeString());
    edgeStr3->appendEdge(edge3);

    CPPUNIT_ASSERT(edgeStr1->contains(edgeStr2));
    CPPUNIT_ASSERT(!edgeStr1->contains(edgeStr3));
  }

  void appendSublineTest()
  {
    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstNetworkVertexPtr vertex3(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 20, 0)));
    ConstNetworkEdgePtr edge2(new NetworkEdge(vertex2, vertex3, true));
    ConstEdgeLocationPtr edgeLocStart(new EdgeLocation(edge1, 0.0));
    ConstEdgeLocationPtr edgeLocEnd(new EdgeLocation(edge1, 1.0));
    ConstEdgeSublinePtr edgeSubline1(new EdgeSubline(edgeLocStart, edgeLocEnd));
    ConstEdgeLocationPtr edgeLocStart2(new EdgeLocation(edge2, 0.0));
    ConstEdgeLocationPtr edgeLocEnd2(new EdgeLocation(edge2, 1.0));
    ConstEdgeSublinePtr edgeSubline2(new EdgeSubline(edgeLocStart2, edgeLocEnd2));

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
    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstNetworkVertexPtr vertex3(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 20, 0)));
    ConstNetworkEdgePtr edge2(new NetworkEdge(vertex2, vertex3, true));

    EdgeStringPtr edgeStr(new EdgeString());
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
