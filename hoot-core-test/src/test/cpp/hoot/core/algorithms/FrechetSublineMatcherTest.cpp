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

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// GEOS
#include <geos/geom/LineString.h>
// Hoot
#include <hoot/core/algorithms/FrechetSublineMatcher.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
#include <hoot/core/MapProjector.h>

#include "../TestUtils.h"

using namespace hoot;


namespace hoot
{

class FrechetSublineMatcherTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(FrechetSublineMatcherTest);
  CPPUNIT_TEST(sublineTest);
  CPPUNIT_TEST(runVTest);
  CPPUNIT_TEST(runCircleTest);
  CPPUNIT_TEST_SUITE_END();

public:
  void sublineTest()
  {

  }

  OsmMapPtr createMap()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    MapProjector::projectToPlanar(map);
    return map;
  }

  void runVTest()
  {
    Settings s;
    OsmMapPtr map = createMap();

    /*
     * Create ways like this:
     * w1 \    / w2
     *     \  /
     *      \/
     */
    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(-15.0, 50.0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 10.0, "w1");

    Coordinate c2[] = { Coordinate(0.0, 0.0), Coordinate(15.0, 50.0), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, c2, 10.0, "w2");

    FrechetSublineMatcher uut;
    uut.setConfiguration(s);

    double score;
    vector<WaySublineMatch> m = uut.findMatch(map, w1, w2, score, 53).getMatches();

    HOOT_STR_EQUALS(1, m.size());
    HOOT_STR_EQUALS(
      "subline 1: start: way: -1 index: 0 fraction: 0 end: way: -1 index: 1 fraction: 0\n"
      "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 1 fraction: 0",
      m[0].toString());
  }

  /**
   * This is a nasty little situation that showed up in the DC data when doing some real world
   * testing.
   *
   * This creates a really poor match, but tests for an edge condition in MaximalSubline
   */
  void runCircleTest()
  {
    Settings s;
    shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    OsmReader reader;
    reader.read("test-files/algorithms/MaximalSublineCircleTestIn.osm", map);
    double score;

    MapProjector::projectToPlanar(map);

    {
      WayPtr w1 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "1")[0]);
      WayPtr w2 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "2")[0]);

      FrechetSublineMatcher uut;
      uut.setConfiguration(s);

      vector<WaySublineMatch> m = uut.findMatch(map, w1, w2, score).getMatches();
      HOOT_STR_EQUALS(1, m.size());
      HOOT_STR_EQUALS(
        "subline 1: start: way: -1 index: 87 fraction: 0 end: way: -1 index: 90 fraction: 0\n"
        "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 5 fraction: 0",
        m[0].toString());
    }
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FrechetSublineMatcherTest, "current");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FrechetSublineMatcherTest, "quick");

}
