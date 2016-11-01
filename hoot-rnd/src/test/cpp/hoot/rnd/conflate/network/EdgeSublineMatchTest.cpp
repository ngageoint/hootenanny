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
#include <hoot/rnd/conflate/network/EdgeSublineMatch.h>
#include <hoot/core/TestUtils.h>

namespace hoot
{

class EdgeSublineMatchTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(EdgeSublineMatchTest);
  CPPUNIT_TEST(basicTest);
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
    ConstNetworkEdgePtr edge(new NetworkEdge(vertex1, vertex2, true));
    ConstEdgeLocationPtr edgeLocStart(new EdgeLocation(edge, 0.0));
    ConstEdgeLocationPtr edgeLocEnd(new EdgeLocation(edge, 0.7));
    ConstEdgeSublinePtr edgeSubline1(new EdgeSubline(edgeLocStart, edgeLocEnd));
    ConstEdgeSublinePtr edgeSubline2(new EdgeSubline(edgeLocEnd, edgeLocStart));

    EdgeSublineMatch edgeSublineMatch(edgeSubline1, edgeSubline2);
    CPPUNIT_ASSERT(edgeSublineMatch.getSubline1() == edgeSubline1);
    CPPUNIT_ASSERT(edgeSublineMatch.getSubline2() == edgeSubline2);
    HOOT_STR_EQUALS(
      "{subline1: { _start: { _e: (0) Node:-1 --  --> (1) Node:-2, _portion: 0 }, _end: { _e: (0) Node:-1 --  --> (1) Node:-2, _portion: 0.7 } }, subline2: { _start: { _e: (0) Node:-1 --  --> (1) Node:-2, _portion: 0.7 }, _end: { _e: (0) Node:-1 --  --> (1) Node:-2, _portion: 0 } }}",
      edgeSublineMatch.toString());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EdgeSublineMatchTest, "quick");

}
