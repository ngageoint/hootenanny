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
#include <hoot/rnd/conflate/network/EdgeMatch.h>
#include <hoot/core/TestUtils.h>

namespace hoot
{

class EdgeMatchTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(EdgeMatchTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(containsEdgeMatchTest);
  CPPUNIT_TEST(containsPartialTest);
  CPPUNIT_TEST(containsStubTest);
  CPPUNIT_TEST(overlapsTest);
  CPPUNIT_TEST(reverseTest);
  CPPUNIT_TEST(hashTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void basicTest()
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
    edgeStr2->appendEdge(edge3);

    EdgeMatch edgeMatch(edgeStr1, edgeStr2);

    CPPUNIT_ASSERT(edgeMatch.contains(edge1));
    CPPUNIT_ASSERT(edgeMatch.contains(edge2));
    CPPUNIT_ASSERT(edgeMatch.contains(edge3));
    CPPUNIT_ASSERT(edgeMatch.getString1()->toString() == edgeStr1->toString());
    CPPUNIT_ASSERT(edgeMatch.getString2()->toString() == edgeStr2->toString());

    CPPUNIT_ASSERT(edgeMatch.getString1()->contains(edge1));
    CPPUNIT_ASSERT(edgeMatch.getString1()->contains(edge2));
    ConstEdgeLocationPtr edgeLoc1(new EdgeLocation(edge1, 0.0));
    CPPUNIT_ASSERT(edgeMatch.getString1()->getFrom() == edgeLoc1);
    ConstEdgeLocationPtr edgeLoc2(new EdgeLocation(edge2, 1.0));
    CPPUNIT_ASSERT(edgeMatch.getString1()->getTo() == edgeLoc2);
    CPPUNIT_ASSERT(edgeMatch.getString1()->getFirstEdge() == edge1);
    CPPUNIT_ASSERT(edgeMatch.getString1()->getLastEdge() == edge2);

    CPPUNIT_ASSERT(edgeMatch.getString2()->contains(edge2));
    CPPUNIT_ASSERT(edgeMatch.getString2()->contains(edge3));
    ConstEdgeLocationPtr edgeLoc3(new EdgeLocation(edge2, 0.0));
    CPPUNIT_ASSERT(edgeMatch.getString2()->getFrom() == edgeLoc3);
    ConstEdgeLocationPtr edgeLoc4(new EdgeLocation(edge3, 1.0));
    CPPUNIT_ASSERT(edgeMatch.getString2()->getTo() == edgeLoc4);
    CPPUNIT_ASSERT(edgeMatch.getString2()->getFirstEdge() == edge2);
    CPPUNIT_ASSERT(edgeMatch.getString2()->getLastEdge() == edge3);

    CPPUNIT_ASSERT(edgeMatch.contains(vertex1));
    CPPUNIT_ASSERT(edgeMatch.contains(vertex2));
    CPPUNIT_ASSERT(edgeMatch.contains(vertex3));
    CPPUNIT_ASSERT(edgeMatch.contains(vertex4));

    CPPUNIT_ASSERT(!edgeMatch.containsPartial());
    CPPUNIT_ASSERT(!edgeMatch.containsStub());
  }

  void containsEdgeMatchTest()
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
    ConstNetworkVertexPtr vertex5(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 30, 0)));
    ConstNetworkEdgePtr edge4(new NetworkEdge(vertex4, vertex5, true));
    EdgeStringPtr edgeStr1(new EdgeString());
    edgeStr1->appendEdge(edge1);
    edgeStr1->appendEdge(edge2);
    EdgeStringPtr edgeStr2(new EdgeString());
    edgeStr2->appendEdge(edge2);
    edgeStr2->appendEdge(edge3);
    EdgeStringPtr edgeStr3(new EdgeString());
    edgeStr3->appendEdge(edge3);
    edgeStr3->appendEdge(edge4);

    ConstEdgeMatchPtr edgeMatch1(new EdgeMatch(edgeStr1, edgeStr2));
    ConstEdgeMatchPtr edgeMatch2(new EdgeMatch(edgeStr1, edgeStr2));
    ConstEdgeMatchPtr edgeMatch3(new EdgeMatch(edgeStr2, edgeStr3));

    CPPUNIT_ASSERT(edgeMatch1->contains(edgeMatch2));
    CPPUNIT_ASSERT(!edgeMatch1->contains(edgeMatch3));
  }

  void containsPartialTest()
  {
    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstEdgeLocationPtr edgeLocStart1(new EdgeLocation(edge1, 0.0));
    ConstEdgeLocationPtr edgeLocEnd1(new EdgeLocation(edge1, 0.9));
    ConstEdgeLocationPtr edgeLocEnd2(new EdgeLocation(edge1, 1.0));
    ConstEdgeSublinePtr edgeSubline1(new EdgeSubline(edgeLocStart1, edgeLocEnd1));
    ConstEdgeSublinePtr edgeSubline2(new EdgeSubline(edgeLocStart1, edgeLocEnd2));
    EdgeStringPtr edgeStr1(new EdgeString());
    edgeStr1->appendEdge(edgeSubline1);
    EdgeStringPtr edgeStr2(new EdgeString());
    edgeStr2->appendEdge(edgeSubline2);

    EdgeMatch edgeMatch1(edgeStr1, edgeStr2);
    CPPUNIT_ASSERT(edgeMatch1.containsPartial());

    EdgeMatch edgeMatch2(edgeStr2, edgeStr2);
    CPPUNIT_ASSERT(!edgeMatch2.containsPartial());
  }

  void containsStubTest()
  {
    OsmMapPtr map(new OsmMap());
    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex1, true));
    ConstNetworkEdgePtr edge2(new NetworkEdge(vertex1, vertex2, true));
    EdgeStringPtr edgeStr1(new EdgeString());
    edgeStr1->appendEdge(edge1);
    EdgeStringPtr edgeStr2(new EdgeString());
    edgeStr2->appendEdge(edge2);

    EdgeMatch edgeMatch(edgeStr1, edgeStr2);
    CPPUNIT_ASSERT(edgeMatch.containsStub());
  }

  void overlapsTest()
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

    ConstEdgeMatchPtr edgeMatch1(new EdgeMatch(edgeStr1, edgeStr1));
    ConstEdgeMatchPtr edgeMatch2(new EdgeMatch(edgeStr2, edgeStr2));
    ConstEdgeMatchPtr edgeMatch3(new EdgeMatch(edgeStr3, edgeStr3));

    CPPUNIT_ASSERT(edgeMatch1->overlaps(edgeMatch2));
    CPPUNIT_ASSERT(!edgeMatch1->overlaps(edgeMatch3));
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
    ConstNetworkVertexPtr vertex4(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 30, 0)));
    ConstNetworkEdgePtr edge3(new NetworkEdge(vertex3, vertex4, true));
    EdgeStringPtr edgeStr1(new EdgeString());
    edgeStr1->appendEdge(edge1);
    edgeStr1->appendEdge(edge2);
    EdgeStringPtr edgeStr2(new EdgeString());
    edgeStr2->appendEdge(edge2);
    edgeStr2->appendEdge(edge3);

    EdgeMatch edgeMatch(edgeStr1, edgeStr2);
    edgeMatch.reverse();

    CPPUNIT_ASSERT(edgeMatch.contains(edge1));
    CPPUNIT_ASSERT(edgeMatch.contains(edge2));
    CPPUNIT_ASSERT(edgeMatch.contains(edge3));

    CPPUNIT_ASSERT(edgeMatch.getString1()->contains(edge1));
    CPPUNIT_ASSERT(edgeMatch.getString1()->contains(edge2));
    ConstEdgeLocationPtr edgeLoc1(new EdgeLocation(edge1, 0.0));
    ConstEdgeLocationPtr edgeLoc2(new EdgeLocation(edge2, 1.0));
    CPPUNIT_ASSERT(edgeMatch.getString1()->getFrom() == edgeLoc2);
    CPPUNIT_ASSERT(edgeMatch.getString1()->getTo() == edgeLoc1);
    CPPUNIT_ASSERT(edgeMatch.getString1()->getFirstEdge() == edge2);
    CPPUNIT_ASSERT(edgeMatch.getString1()->getLastEdge() == edge1);

    CPPUNIT_ASSERT(edgeMatch.getString2()->contains(edge2));
    CPPUNIT_ASSERT(edgeMatch.getString2()->contains(edge3));
    ConstEdgeLocationPtr edgeLoc3(new EdgeLocation(edge2, 0.0));
    ConstEdgeLocationPtr edgeLoc4(new EdgeLocation(edge3, 1.0));
    CPPUNIT_ASSERT(edgeMatch.getString2()->getFrom() == edgeLoc4);
    CPPUNIT_ASSERT(edgeMatch.getString2()->getTo() == edgeLoc3);
    CPPUNIT_ASSERT(edgeMatch.getString2()->getFirstEdge() == edge3);
    CPPUNIT_ASSERT(edgeMatch.getString2()->getLastEdge() == edge2);

    CPPUNIT_ASSERT(edgeMatch.contains(vertex1));
    CPPUNIT_ASSERT(edgeMatch.contains(vertex2));
    CPPUNIT_ASSERT(edgeMatch.contains(vertex3));
    CPPUNIT_ASSERT(edgeMatch.contains(vertex4));

    CPPUNIT_ASSERT(!edgeMatch.containsPartial());
    CPPUNIT_ASSERT(!edgeMatch.containsStub());
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
    ConstNetworkVertexPtr vertex4(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 30, 0)));
    ConstNetworkEdgePtr edge3(new NetworkEdge(vertex3, vertex4, true));
    EdgeStringPtr edgeStr1(new EdgeString());
    edgeStr1->appendEdge(edge1);
    edgeStr1->appendEdge(edge2);
    EdgeStringPtr edgeStr2(new EdgeString());
    edgeStr2->appendEdge(edge2);
    edgeStr2->appendEdge(edge3);

    EdgeMatchPtr edgeMatch(new EdgeMatch(edgeStr1, edgeStr2));

    QHash<EdgeMatchPtr, QString> edgeMatches;
    edgeMatches.insert(edgeMatch, "test");
    CPPUNIT_ASSERT_EQUAL(1, edgeMatches.size());
    CPPUNIT_ASSERT(edgeMatches.value(edgeMatch) == "test");
    CPPUNIT_ASSERT(edgeMatches.key("test") == edgeMatch);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EdgeMatchTest, "quick");

}
