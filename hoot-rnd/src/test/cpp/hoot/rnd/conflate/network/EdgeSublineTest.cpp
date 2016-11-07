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
#include <hoot/rnd/conflate/network/EdgeSubline.h>
#include <hoot/core/TestUtils.h>

namespace hoot
{

class EdgeSublineTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(EdgeSublineTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(invalidTest);
  CPPUNIT_TEST(zeroLengthTest);
  CPPUNIT_TEST(backwardTest);
  CPPUNIT_TEST(overlapsTest);
  CPPUNIT_TEST(reverseTest);
  CPPUNIT_TEST(createFullSublineTest);
  CPPUNIT_TEST(equalTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void basicTest()
  {
    OsmMapPtr map(new OsmMap());

    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge(new NetworkEdge(vertex1, vertex2, true));
    ConstEdgeLocationPtr edgeLocStart(new EdgeLocation(edge, 0.0));
    ConstEdgeLocationPtr edgeLocEnd(new EdgeLocation(edge, 0.7));

    EdgeSubline edgeSubline(edgeLocStart, edgeLocEnd);

    CPPUNIT_ASSERT(edgeSubline.isValid());
    CPPUNIT_ASSERT(!edgeSubline.isZeroLength());
    CPPUNIT_ASSERT(!edgeSubline.isBackwards());

    CPPUNIT_ASSERT(edgeSubline.getEdge() == edge);
    CPPUNIT_ASSERT(edgeSubline.getStart() == edgeLocStart);
    CPPUNIT_ASSERT(edgeSubline.getEnd() == edgeLocEnd);
    CPPUNIT_ASSERT(edgeSubline.getFormer() == edgeLocStart);
    CPPUNIT_ASSERT(edgeSubline.getLatter() == edgeLocEnd);

    CPPUNIT_ASSERT(edgeSubline.contains(vertex1));
    CPPUNIT_ASSERT(!edgeSubline.contains(vertex2));
  }

  void invalidTest()
  {
    OsmMapPtr map(new OsmMap());

    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge(new NetworkEdge(vertex1, vertex2, true));
    ConstEdgeLocationPtr edgeLocStart(new EdgeLocation(edge, -0.1));
    ConstEdgeLocationPtr edgeLocEnd(new EdgeLocation(edge, 0.7));

    EdgeSubline edgeSubline(edgeLocStart, edgeLocEnd);

    CPPUNIT_ASSERT(!edgeSubline.isValid());
  }

  void zeroLengthTest()
  {
    OsmMapPtr map(new OsmMap());

    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstEdgeLocationPtr edgeLocStart(new EdgeLocation(edge1, 0.0));

    EdgeSubline edgeSubline(edgeLocStart, edgeLocStart);

    CPPUNIT_ASSERT(edgeSubline.isZeroLength());
  }

  void backwardTest()
  {
    OsmMapPtr map(new OsmMap());

    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge(new NetworkEdge(vertex1, vertex2, true));
    ConstEdgeLocationPtr edgeLocStart(new EdgeLocation(edge, 0.7));
    ConstEdgeLocationPtr edgeLocEnd(new EdgeLocation(edge, 0.0));

    EdgeSubline edgeSubline(edgeLocStart, edgeLocEnd);

    CPPUNIT_ASSERT(edgeSubline.isBackwards());
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
    ConstEdgeLocationPtr edgeLoc2(new EdgeLocation(edge1, 0.7));
    ConstEdgeSublinePtr edgeSubline1(new EdgeSubline(edgeLoc1, edgeLoc2));
    ConstEdgeLocationPtr edgeLoc3(new EdgeLocation(edge1, 0.2));
    ConstEdgeSublinePtr edgeSubline2(new EdgeSubline(edgeLoc1, edgeLoc3));
    CPPUNIT_ASSERT(edgeSubline1->overlaps(edgeSubline2));

    ConstEdgeLocationPtr edgeLoc4(new EdgeLocation(edge1, 0.8));
    ConstEdgeLocationPtr edgeLoc5(new EdgeLocation(edge1, 1.0));
    ConstEdgeSublinePtr edgeSubline3(new EdgeSubline(edgeLoc4, edgeLoc5));
    CPPUNIT_ASSERT(!edgeSubline1->overlaps(edgeSubline3));
  }

  void reverseTest()
  {
    OsmMapPtr map(new OsmMap());

    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));
    ConstEdgeLocationPtr edgeLocStart(new EdgeLocation(edge1, 0.0));
    ConstEdgeLocationPtr edgeLocEnd(new EdgeLocation(edge1, 0.7));

    EdgeSubline edgeSubline(edgeLocStart, edgeLocEnd);
    edgeSubline.reverse();

    CPPUNIT_ASSERT(edgeSubline.isValid());
    CPPUNIT_ASSERT(!edgeSubline.isZeroLength());
    CPPUNIT_ASSERT(edgeSubline.isBackwards());

    CPPUNIT_ASSERT(edgeSubline.getEdge() == edge1);
    CPPUNIT_ASSERT(edgeSubline.getStart() == edgeLocEnd);
    CPPUNIT_ASSERT(edgeSubline.getEnd() == edgeLocStart);
    CPPUNIT_ASSERT(edgeSubline.getFormer() == edgeLocStart);
    CPPUNIT_ASSERT(edgeSubline.getLatter() == edgeLocEnd);
  }

  void createFullSublineTest()
  {
    OsmMapPtr map(new OsmMap());

    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge1(new NetworkEdge(vertex1, vertex2, true));

    EdgeSublinePtr edgeSubline = EdgeSubline::createFullSubline(edge1);

    CPPUNIT_ASSERT(edgeSubline->isValid());
    CPPUNIT_ASSERT(!edgeSubline->isZeroLength());
    CPPUNIT_ASSERT(!edgeSubline->isBackwards());

    CPPUNIT_ASSERT(edgeSubline->getEdge() == edge1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, edgeSubline->getStart()->getPortion(), 0.0);
    CPPUNIT_ASSERT(edgeSubline->getStart()->isFirst());
    CPPUNIT_ASSERT(!edgeSubline->getStart()->isLast());
    CPPUNIT_ASSERT(edgeSubline->getStart()->isExtreme());
    CPPUNIT_ASSERT(edgeSubline->getStart()->isValid());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, edgeSubline->getEnd()->getPortion(), 0.0);
    CPPUNIT_ASSERT(!edgeSubline->getEnd()->isFirst());
    CPPUNIT_ASSERT(edgeSubline->getEnd()->isLast());
    CPPUNIT_ASSERT(edgeSubline->getEnd()->isExtreme());
    CPPUNIT_ASSERT(edgeSubline->getEnd()->isValid());
  }

  void equalTest()
  {
    OsmMapPtr map(new OsmMap());

    ConstNetworkVertexPtr vertex1(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 0, 0)));
    ConstNetworkVertexPtr vertex2(
      new NetworkVertex(TestUtils::createNode(map, Status::Unknown1, 10, 0)));
    ConstNetworkEdgePtr edge(new NetworkEdge(vertex1, vertex2, true));
    ConstEdgeLocationPtr edgeLocStart(new EdgeLocation(edge, 0.0));
    ConstEdgeLocationPtr edgeLocEnd(new EdgeLocation(edge, 0.7));

    ConstEdgeSublinePtr edgeSubline1(new EdgeSubline(edgeLocStart, edgeLocEnd));
    ConstEdgeSublinePtr edgeSubline2(new EdgeSubline(edgeLocStart, edgeLocEnd));
    CPPUNIT_ASSERT(edgeSubline1 == edgeSubline2);

    ConstEdgeSublinePtr edgeSubline3(new EdgeSubline(edgeLocStart, edgeLocEnd));
    ConstEdgeSublinePtr edgeSubline4(new EdgeSubline(edgeLocEnd, edgeLocStart));
    CPPUNIT_ASSERT(edgeSubline3 != edgeSubline4);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EdgeSublineTest, "quick");

}
