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
 * @copyright Copyright (C) 2014, 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// geos
#include <geos/geom/Coordinate.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/linearreference/WaySublineCollection.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

using namespace geos::geom;

namespace hoot
{

class WaySublineStringTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WaySublineStringTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmMapPtr createTestMap()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate c1[] = { Coordinate(0, 0), Coordinate(10, 0), Coordinate(20, 0), Coordinate(30, 0),
                       Coordinate::getNull() };
    TestUtils::createWay(map, c1, "w1", Status::Unknown1, 5);

    Coordinate c2[] = { Coordinate(0, 10), Coordinate(10, 10), Coordinate(20, 10),
                        Coordinate(30, 10), Coordinate::getNull() };
    TestUtils::createWay(map, c2, "w2", Status::Unknown1, 5);

    return map;
  }

  void runTest()
  {
    OsmMapPtr map = createTestMap();
    WayPtr w1 =
      map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "w1")[0]);
    WayPtr w2 =
      map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "w2")[0]);

    {
      WaySublineCollection uut;
      uut.addSubline(WaySubline(WayLocation(map, w1, 0.0), WayLocation(map, w1, 30.0)));

      HOOT_STR_EQUALS("", uut.invert());
    }

    {
      WaySublineCollection uut;
      uut.addSubline(WaySubline(WayLocation(map, w1, 0.0), WayLocation(map, w1, 20.0)));

      HOOT_STR_EQUALS("start: way(-1) index: 2 fraction: 0 end: way(-1) index: 3 fraction: 0",
                      uut.invert());
    }

    {
      WaySublineCollection uut;
      uut.addSubline(WaySubline(WayLocation(map, w1, 20.0), WayLocation(map, w1, 30.0)));

      HOOT_STR_EQUALS("start: way(-1) index: 0 fraction: 0 end: way(-1) index: 2 fraction: 0",
                      uut.invert());
    }

    {
      WaySublineCollection uut;
      uut.addSubline(WaySubline(WayLocation(map, w1, 10.0), WayLocation(map, w1, 20.0)));

      HOOT_STR_EQUALS("start: way(-1) index: 0 fraction: 0 end: way(-1) index: 1 fraction: 0\n"
        "start: way(-1) index: 2 fraction: 0 end: way(-1) index: 3 fraction: 0",
        uut.invert());
    }

    {
      WaySublineCollection uut;
      uut.addSubline(WaySubline(WayLocation(map, w1, 10.0), WayLocation(map, w1, 20.0)));
      uut.addSubline(WaySubline(WayLocation(map, w2, 10.0), WayLocation(map, w2, 20.0)));

      HOOT_STR_EQUALS("start: way(-2) index: 0 fraction: 0 end: way(-2) index: 1 fraction: 0\n"
                      "start: way(-2) index: 2 fraction: 0 end: way(-2) index: 3 fraction: 0\n"
                      "start: way(-1) index: 0 fraction: 0 end: way(-1) index: 1 fraction: 0\n"
                      "start: way(-1) index: 2 fraction: 0 end: way(-1) index: 3 fraction: 0",
                      uut.invert());
    }

    {
      WaySublineCollection uut;
      uut.addSubline(WaySubline(WayLocation(map, w1, 10.0), WayLocation(map, w1, 30.0)));
      uut.addSubline(WaySubline(WayLocation(map, w2, 0.0), WayLocation(map, w2, 20.0)));

      HOOT_STR_EQUALS("start: way(-2) index: 2 fraction: 0 end: way(-2) index: 3 fraction: 0\n"
                      "start: way(-1) index: 0 fraction: 0 end: way(-1) index: 1 fraction: 0",
                      uut.invert());
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WaySublineStringTest, "quick");

}

