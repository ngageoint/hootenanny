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
 * @copyright Copyright (C) 2014, 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/geom/Coordinate.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/linearreference/LocationOfPoint.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>

#include "../../TestUtils.h"

namespace hoot
{

class LocationOfPointTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(LocationOfPointTest);
  //TODO: fixme or remove me - #1737
  //CPPUNIT_TEST(runLocateAfterTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runLocateAfterTest()
  {
//    OsmMap::resetCounters();
//    shared_ptr<OsmMap> map(new OsmMap());

//    Coordinate c[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
//                       Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
//                       Coordinate::getNull() };
//    WayPtr w = TestUtils::createWay(map, Status::Unknown1, c, 1, "");

//    WayLocation wl(w, 0, 0);
//    HOOT_STR_EQUALS("way: -1 index: 0 fraction: 0",
//      LocationOfPoint(w).locateAfter(Coordinate(0,0), wl));
//    HOOT_STR_EQUALS("way: -1 index: 0 fraction: 0.1",
//      LocationOfPoint(w).locateAfter(Coordinate(0,0), WayLocation(w, 10.0)));
//    HOOT_STR_EQUALS("way: -1 index: 0 fraction: 0.1",
//      LocationOfPoint(w).locateAfter(Coordinate(10,0), WayLocation(w, 0.0)));
//    HOOT_STR_EQUALS("way: -1 index: 0 fraction: 0.2",
//      LocationOfPoint(w).locateAfter(Coordinate(20,0), WayLocation(w, 0.0)));
//    HOOT_STR_EQUALS("way: -1 index: 0 fraction: 0.5",
//      LocationOfPoint(w).locateAfter(Coordinate(50,0), WayLocation(w, 20.0)));
//    HOOT_STR_EQUALS("way: -1 index: 2 fraction: 0.5",
//      LocationOfPoint(w).locateAfter(Coordinate(50,10), WayLocation(w, 0.0)));

//    HOOT_STR_EQUALS("way: -1 index: 2 fraction: 0.5",
//      LocationOfPoint(w).locateAfter(Coordinate(50,0), WayLocation(w, 100.0)));
//    HOOT_STR_EQUALS("way: -1 index: 1 fraction: 0",
//      LocationOfPoint(w).locateAfter(Coordinate(100,0), WayLocation(w, 0.0)));
//    HOOT_STR_EQUALS("way: -1 index: 1 fraction: 0.2",
//      LocationOfPoint(w).locateAfter(Coordinate(100,0), WayLocation(w, 102.0)));
//    HOOT_STR_EQUALS("way: -1 index: 1 fraction: 0.2",
//      LocationOfPoint(w).locateAfter(Coordinate(100,2), WayLocation(w, 100.0)));
//    HOOT_STR_EQUALS("way: -1 index: 2 fraction: 0",
//      LocationOfPoint(w).locateAfter(Coordinate(100,10), WayLocation(w, 110.0)));

//    HOOT_STR_EQUALS("way: -1 index: 2 fraction: 0.5",
//      LocationOfPoint(w).locateAfter(Coordinate(50,10), WayLocation(w, 110.0)));
//    HOOT_STR_EQUALS("way: -1 index: 3 fraction: 0",
//      LocationOfPoint(w).locateAfter(Coordinate(0,10), WayLocation(w, 0.0)));
//    HOOT_STR_EQUALS("way: -1 index: 3 fraction: 0",
//      LocationOfPoint(w).locateAfter(Coordinate(0,0), WayLocation(w, 3, 0)));
//    HOOT_STR_EQUALS("way: -1 index: 3 fraction: 0",
//      LocationOfPoint(w).locateAfter(Coordinate(0,10), WayLocation(w, 2, 0.3)));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LocationOfPointTest, "quick");

}

