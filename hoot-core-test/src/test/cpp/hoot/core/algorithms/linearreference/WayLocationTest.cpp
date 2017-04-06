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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/geom/Coordinate.h>

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>

#include "../../TestUtils.h"

namespace hoot
{

class WayLocationTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(WayLocationTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runDistanceOnWayTest);
  CPPUNIT_TEST(runMoveTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmMapPtr createMap()
  {
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    return map;
  }

  void runTest()
  {
    boost::shared_ptr<OsmMap> map(new OsmMap());

    Coordinate c[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                       Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                       Coordinate::getNull() };
    WayPtr w = TestUtils::createWay(map, Status::Unknown1, c, 1, "");

    WayLocation wl(map, w, 0, 1);
    CPPUNIT_ASSERT_EQUAL(wl.getSegmentIndex(), 1);
    HOOT_STR_EQUALS(0, wl.getSegmentFraction());

  }

  void runDistanceOnWayTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(0, 0),
                         Coordinate(30, 0),
                         Coordinate(70, 0),
                         Coordinate(120, 0),
                         Coordinate(180, 0),
                         Coordinate(250, 0),
                         Coordinate(330, 0),
                         Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c, 1, "");

    WayLocation wl(map, w1, 90.0);
    HOOT_STR_EQUALS("way: -1 index: 2 fraction: 0.4", wl);
    HOOT_STR_EQUALS(90, wl.calculateDistanceOnWay());
  }

  void runMoveTest()
  {
    boost::shared_ptr<OsmMap> map(new OsmMap());

    Coordinate c[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                       Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                       Coordinate::getNull() };
    WayPtr w = TestUtils::createWay(map, Status::Unknown1, c, 1, "");

    WayLocation wl;
    wl = WayLocation(map, w, 0, 1);
    HOOT_STR_EQUALS(100, wl.calculateDistanceOnWay());
    HOOT_STR_EQUALS(101, wl.move(1.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(110, wl.move(10.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(200, wl.move(100.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(210, wl.move(200.0).calculateDistanceOnWay());

    wl = WayLocation(map, w, 0, 0);
    HOOT_STR_EQUALS(0, wl.calculateDistanceOnWay());
    HOOT_STR_EQUALS(1, wl.move(1.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(10, wl.move(10.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(100, wl.move(100.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(150, wl.move(150.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(200, wl.move(200.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(201, wl.move(201.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(210, wl.move(210.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(210, wl.move(300.0).calculateDistanceOnWay());

    wl = WayLocation(map, w, 34.0);
    HOOT_STR_EQUALS(34, wl.calculateDistanceOnWay());
    HOOT_STR_EQUALS(35, wl.move(1.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(44, wl.move(10.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(134, wl.move(100.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(210, wl.move(200.0).calculateDistanceOnWay());

    wl = WayLocation(map, w, 34.0);
    HOOT_STR_EQUALS(34, wl.calculateDistanceOnWay());
    HOOT_STR_EQUALS(33, wl.move(-1.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(24, wl.move(-10.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(0, wl.move(-100.0).calculateDistanceOnWay());

    wl = WayLocation(map, w, 205.0);
    HOOT_STR_EQUALS(205, wl.calculateDistanceOnWay());
    HOOT_STR_EQUALS(204, wl.move(-1.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(195, wl.move(-10.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(105, wl.move(-100.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(5, wl.move(-200.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(0, wl.move(-205.0).calculateDistanceOnWay());

    wl = WayLocation::createAtEndOfWay(map, w);
    HOOT_STR_EQUALS(209, wl.move(-1.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(205, wl.move(-5.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(200, wl.move(-10.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(195, wl.move(-15.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(100, wl.move(-110.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(50, wl.move(-160.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(0, wl.move(-210.0).calculateDistanceOnWay());
    HOOT_STR_EQUALS(0, wl.move(-300.0).calculateDistanceOnWay());

    // this came from a real world situation where floating point errors caused the segmentFraction
    // to be greater than one. It is fixed now.
    Coordinate c2[] = { Coordinate(-205.9686747586594038, -3753.0596443131830711),
                        Coordinate(-205.8037612833784067, -3753.9176291990520440),
                        Coordinate(-210.2661283208896066, -3804.4654461097293279),
                        Coordinate(-216.9595467377828300, -3857.2599030030505673),
                        Coordinate(-219.7621528089831600, -3889.8777209144068365),
                        Coordinate(-220.4189617007821482, -3896.2709067124583271),
                        Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown1, c2, 1, "");

    wl = WayLocation(map, w2, 4, 0.6888048860567166);
    HOOT_STR_EQUALS(144, wl.move(2).calculateDistanceOnWay());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayLocationTest, "quick");

}

