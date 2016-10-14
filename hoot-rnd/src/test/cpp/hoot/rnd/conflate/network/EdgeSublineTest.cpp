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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/MapProjector.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/filters/TagCriterion.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/rnd/conflate/network/DebugNetworkMapCreator.h>
#include <hoot/rnd/conflate/network/EdgeMatchSetFinder.h>
#include <hoot/rnd/conflate/network/OsmNetworkExtractor.h>

namespace hoot
{

class EdgeSublineTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(EdgeSublineTest);
  CPPUNIT_TEST(touchesTest1);
  CPPUNIT_TEST(unionTest1);
  CPPUNIT_TEST_SUITE_END();

public:

  void touchesTest1()
  {
    TestUtils::resetEnvironment();

    WayPtr w1(new Way(Status::Invalid, -1, 15));
    NetworkVertexPtr v1(new NetworkVertex(w1));
    WayPtr w2(new Way(Status::Invalid, -2, 15));
    NetworkVertexPtr v2(new NetworkVertex(w2));

    NetworkEdgePtr e1(new NetworkEdge(v1, v2, false));

    {
      EdgeSublinePtr s1(new EdgeSubline(e1, 0.2, 0.3));
      EdgeSublinePtr s2(new EdgeSubline(e1, 0.25, 0.5));
      HOOT_STR_EQUALS(true, s1->intersects(s2));
    }

    {
      EdgeSublinePtr s1(new EdgeSubline(e1, 0.2, 0.3));
      EdgeSublinePtr s2(new EdgeSubline(e1, 0.3, 0.5));
      HOOT_STR_EQUALS(true, s1->intersects(s2));
    }

    {
      EdgeSublinePtr s1(new EdgeSubline(e1, 0.2, 0.3));
      EdgeSublinePtr s2(new EdgeSubline(e1, 0.30001, 0.5));
      HOOT_STR_EQUALS(false, s1->intersects(s2));
    }

    {
      EdgeSublinePtr s1(new EdgeSubline(e1, 0.2, 0.3));
      EdgeSublinePtr s2(new EdgeSubline(e1, 0.2, 0.3));
      HOOT_STR_EQUALS(true, s1->intersects(s2));
    }
  }

  void unionTest1()
  {
    TestUtils::resetEnvironment();

    WayPtr w1(new Way(Status::Invalid, -1, 15));
    NetworkVertexPtr v1(new NetworkVertex(w1));
    WayPtr w2(new Way(Status::Invalid, -2, 15));
    NetworkVertexPtr v2(new NetworkVertex(w2));

    NetworkEdgePtr e1(new NetworkEdge(v1, v2, false));

    {
      EdgeSublinePtr s1(new EdgeSubline(e1, 0.2, 0.3));
      EdgeSublinePtr s2(new EdgeSubline(e1, 0.25, 0.5));
      HOOT_STR_EQUALS("{ _start: { _e: (0) Way:-1 --  -- (1) Way:-2, _portion: 0.2 }, _end: { _e: (0) Way:-1 --  -- (1) Way:-2, _portion: 0.5 } }",
        s1->unionSubline(s2));
    }

    {
      // one line contains the other, should still work.
      EdgeSublinePtr s1(new EdgeSubline(e1, 0.2, 0.3));
      EdgeSublinePtr s2(new EdgeSubline(e1, 0.1, 0.5));
      HOOT_STR_EQUALS("{ _start: { _e: (0) Way:-1 --  -- (1) Way:-2, _portion: 0.1 }, _end: { _e: (0) Way:-1 --  -- (1) Way:-2, _portion: 0.5 } }",
        s1->unionSubline(s2));
    }

    {
      // both backwards, the result should be backwards.
      EdgeSublinePtr s1(new EdgeSubline(e1, 0.3, 0.2));
      EdgeSublinePtr s2(new EdgeSubline(e1, 1.0, 0.25));
      HOOT_STR_EQUALS("{ _start: { _e: (0) Way:-1 --  -- (1) Way:-2, _portion: 1 }, _end: { _e: (0) Way:-1 --  -- (1) Way:-2, _portion: 0.2 } }",
        s1->unionSubline(s2));
    }

    {
      DisableLog dl;
      // going in two different directions.
      EdgeSublinePtr s1(new EdgeSubline(e1, 0.3, 0.2));
      EdgeSublinePtr s2(new EdgeSubline(e1, 0.25, 1.0));
      CPPUNIT_ASSERT_THROW(s1->unionSubline(s2), IllegalArgumentException);
    }

    {
      DisableLog dl;
      // lines don't touch
      EdgeSublinePtr s1(new EdgeSubline(e1, 0.2, 0.3));
      EdgeSublinePtr s2(new EdgeSubline(e1, 0.5, 1.0));
      CPPUNIT_ASSERT_THROW(s1->unionSubline(s2), IllegalArgumentException);
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EdgeSublineTest, "slow");

}
