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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/subline-matching/MaximalNearestSublineMatcher.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Log.h>

// Tgs
#include <tgs/StreamUtils.h>

using namespace geos::geom;

namespace hoot
{

/**
 * all the hard core tests are in MaximalNearestSublineTest. This just makes sure the interface is
 * wrapped properly.
 */
class MaximalNearestSublineMatcherTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MaximalNearestSublineMatcherTest);
  CPPUNIT_TEST(runSimpleTest);
  CPPUNIT_TEST(runNoOverlapTest);
  CPPUNIT_TEST_SUITE_END();

public:

  /**
   * Two identical ways are 5m apart and have a 10% overlap. All straight and 100m long.
   * x and o signifiy Unknown1 and Unknown2, respectively.
   *
   * x---------x
   *          o---------o
   */
  void runSimpleTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, w1c, "", Status::Unknown1);

    Coordinate w2c[] = { Coordinate(90, 5), Coordinate(190, 5), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, w2c, "", Status::Unknown2);

    std::shared_ptr<MaximalNearestSublineMatcher> matcher =
      std::make_shared<MaximalNearestSublineMatcher>();

    double score;
    WaySublineMatchString match = matcher->findMatch(map, w1, w2, score, 10.0);

    HOOT_STR_EQUALS(
      "matches:\n"
      "subline 1: start: way(-1) index: 0 fraction: 0.9 end: way(-1) index: 1 fraction: 0\n"
      "subline 2: start: way(-2) index: 0 fraction: 0 end: way(-2) index: 0 fraction: 0.1",
      match.toString());

    CPPUNIT_ASSERT_EQUAL(true, match.isValid());
  }

  /**
   * Two identical ways are 5m apart and have a 0% overlap. All straight and 100m long.
   * x and o signifiy Unknown1 and Unknown2, respectively.
   *
   * x---------x
   *             o---------o
   */
  void runNoOverlapTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, w1c, "", Status::Unknown1);

    Coordinate w2c[] = { Coordinate(110, 5), Coordinate(210, 5), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, w2c, "", Status::Unknown2);

    std::shared_ptr<MaximalNearestSublineMatcher> matcher =
      std::make_shared<MaximalNearestSublineMatcher>();

    double score;
    WaySublineMatchString match = matcher->findMatch(map, w1, w2, score, 10.0);

    CPPUNIT_ASSERT_EQUAL(false, match.isValid());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MaximalNearestSublineMatcherTest, "quick");

}
