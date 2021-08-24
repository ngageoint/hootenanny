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

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/linearreference/LocationOfPoint.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>

namespace hoot
{

class LocationOfPointTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(LocationOfPointTest);
  CPPUNIT_TEST(runLocateAfterTest);
  CPPUNIT_TEST_SUITE_END();

public:

  LocationOfPointTest()
  {
    setResetType(ResetBasic);
  }

  void runLocateAfterTest()
  {
    std::shared_ptr<OsmMap> map = std::make_shared<OsmMap>();

    geos::geom::Coordinate c[] =
    {
      geos::geom::Coordinate(0.0, 0.0), geos::geom::Coordinate(100.0, 0.0),
      geos::geom::Coordinate(100.0, 10.0), geos::geom::Coordinate(0.0, 10.0),
      geos::geom::Coordinate::getNull()
    };
    WayPtr w = TestUtils::createWay(map, c, "", Status::Unknown1, 1);
    WayLocation wl(map, w, 0, 0);

    HOOT_STR_EQUALS("way(-1) index: 0 fraction: 0",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(0,0), wl));
    HOOT_STR_EQUALS("way(-1) index: 0 fraction: 0.1",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(0,0), WayLocation(map, w, 10.0)));
    HOOT_STR_EQUALS("way(-1) index: 0 fraction: 0.1",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(10,0), WayLocation(map, w, 0.0)));
    HOOT_STR_EQUALS("way(-1) index: 0 fraction: 0.2",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(20,0), WayLocation(map, w, 0.0)));
    HOOT_STR_EQUALS("way(-1) index: 0 fraction: 0.5",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(50,0), WayLocation(map, w, 20.0)));
    HOOT_STR_EQUALS("way(-1) index: 2 fraction: 0.5",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(50,10), WayLocation(map, w, 0.0)));

    HOOT_STR_EQUALS("way(-1) index: 2 fraction: 0.5",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(50,0), WayLocation(map, w, 100.0)));
    HOOT_STR_EQUALS("way(-1) index: 1 fraction: 0",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(100,0), WayLocation(map, w, 0.0)));
    HOOT_STR_EQUALS("way(-1) index: 1 fraction: 0.2",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(100,0), WayLocation(map, w, 102.0)));
    HOOT_STR_EQUALS("way(-1) index: 1 fraction: 0.2",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(100,2), WayLocation(map, w, 100.0)));
    HOOT_STR_EQUALS("way(-1) index: 2 fraction: 0",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(100,10), WayLocation(map, w, 110.0)));

    HOOT_STR_EQUALS("way(-1) index: 2 fraction: 0.5",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(50,10), WayLocation(map, w, 110.0)));
    HOOT_STR_EQUALS("way(-1) index: 3 fraction: 0",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(0,10), WayLocation(map, w, 0.0)));
    HOOT_STR_EQUALS("way(-1) index: 3 fraction: 0",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(0,0), WayLocation(map, w, 3, 0)));
    HOOT_STR_EQUALS("way(-1) index: 3 fraction: 0",
      LocationOfPoint(map, w).locateAfter(geos::geom::Coordinate(0,10), WayLocation(map, w, 2, 0.3)));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LocationOfPointTest, "quick");

}

