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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// GEOS
#include <geos/geom/LineString.h>
// Hoot
#include <hoot/rnd/conflate/frechet/FrechetSublineMatcher.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
#include <hoot/core/util/MapProjector.h>

#include <hoot-core-test/src/test/cpp/hoot/core/TestUtils.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class FrechetSublineMatcherTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(FrechetSublineMatcherTest);
  CPPUNIT_TEST(singleSublineTest);
  CPPUNIT_TEST(doubleSublineTest);
  CPPUNIT_TEST(runCircleTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void singleSublineTest()
  {
    Settings s;
    OsmMapPtr map = createMap();

    Coordinate c1[] = {
      Coordinate(0.0, 0.0), Coordinate(10.0, 0.0), Coordinate(15.0, 37.0), Coordinate(20.0, 0.0), Coordinate(30.0, 0.0), Coordinate(40.0, 0.0),
      Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 3, "w1");

    Coordinate c2[] = {
      Coordinate(0.0, 3.0), Coordinate(10.0, 3.0), Coordinate(15.0, 40.0), Coordinate(20.0, 3.0), Coordinate(30.0, 3.0), Coordinate(40.0, 3.0),
      Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown1, c2, 3, "w2");

    FrechetSublineMatcher uut;
    uut.setConfiguration(s);

    double score;
    vector<WaySublineMatch> m = uut.findMatch(map, w1, w2, score, 5).getMatches();

    HOOT_STR_EQUALS(1, m.size());
    HOOT_STR_EQUALS(
      "subline 1: start: way: -1 index: 0 fraction: 0 end: way: -1 index: 5 fraction: 0\n"
      "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 5 fraction: 0",
      m[0].toString());
  }

  void doubleSublineTest()
  {
    Settings s;
    OsmMapPtr map = createMap();

    Coordinate c1[] = {
      Coordinate(0.0, 0.0), Coordinate(10.0, 0.0), Coordinate(20.0, 0.0), Coordinate(30.0, 0.0), Coordinate(40.0, 0.0),
      Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 3, "w1");

    Coordinate c2[] = {
      Coordinate(0.0, 3.0), Coordinate(10.0, 3.0), Coordinate(15.0, 40.0), Coordinate(20.0, 3.0), Coordinate(30.0, 3.0), Coordinate(40.0, 3.0),
      Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown1, c2, 3, "w2");

    FrechetSublineMatcher uut;
    uut.setConfiguration(s);

    double score;
    vector<WaySublineMatch> m = uut.findMatch(map, w1, w2, score, 5).getMatches();

    HOOT_STR_EQUALS(2, m.size());
    HOOT_STR_EQUALS(
      "subline 1: start: way: -1 index: 2 fraction: 0 end: way: -1 index: 4 fraction: 0\n"
      "subline 2: start: way: -2 index: 3 fraction: 0 end: way: -2 index: 5 fraction: 0",
      m[0].toString());
    HOOT_STR_EQUALS(
      "subline 1: start: way: -1 index: 0 fraction: 0 end: way: -1 index: 1 fraction: 0\n"
      "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 1 fraction: 0",
      m[1].toString());
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
   OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    OsmXmlReader reader;
    reader.read("test-files/algorithms/MaximalSublineCircleTestIn.osm", map);
    double score;

    MapProjector::projectToPlanar(map);

    WayPtr w1 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "1")[0]);
    WayPtr w2 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "2")[0]);

    FrechetSublineMatcher uut;
    uut.setConfiguration(s);

    vector<WaySublineMatch> m = uut.findMatch(map, w1, w2, score).getMatches();
    HOOT_STR_EQUALS(1, m.size());
    HOOT_STR_EQUALS(
      "subline 1: start: way: -1 index: 87 fraction: 0 end: way: -1 index: 90 fraction: 0\n"
      "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 4 fraction: 0",
      m[0].toString());
  }

  OsmMapPtr createMap()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    MapProjector::projectToPlanar(map);
    return map;
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FrechetSublineMatcherTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FrechetSublineMatcherTest, "quick");

}
