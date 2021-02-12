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
 * @copyright Copyright (C) 2014, 2015, 2016, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

// geos
#include <geos/geom/Coordinate.h>
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/linearreference/WaySubline.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

using namespace geos::geom;

namespace hoot
{

class WaySublineTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WaySublineTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  WaySublineTest()
  {
    setResetType(ResetBasic);
  }

  OsmMapPtr createTestMap()
  {
    OsmMapPtr map(new OsmMap());

    Coordinate c1[] = { Coordinate(0, 0), Coordinate(10, 0), Coordinate(20, 0), Coordinate(30, 0),
                       Coordinate::getNull() };
    TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");

    Coordinate c2[] = { Coordinate(50, 0), Coordinate(150, 0), Coordinate::getNull() };
    TestUtils::createWay(map, Status::Unknown1, c2, 5, "w2");

    return map;
  }

  void runTest()
  {
    OsmMapPtr map = createTestMap();
    ElementToGeometryConverter ec(map);
    WayPtr w1 =
      map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "w1")[0]);
    WayPtr w2 =
      map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "w2")[0]);

    {
      WaySubline uut(WayLocation(map, w1, 0), WayLocation(map, w1, 5));

      HOOT_STR_EQUALS(
        "LINESTRING (0.0000000000000000 0.0000000000000000, 5.0000000000000000 0.0000000000000000)",
        ec.convertToLineString(uut.toWay(map))->toString());
    }

    {
      WaySubline uut(WayLocation(map, w2, 0, 0), WayLocation(map, w2, 0, 0.5));

      HOOT_STR_EQUALS(
        "LINESTRING (50.0000000000000000 0.0000000000000000, 100.0000000000000000 0.0000000000000000)",
        ec.convertToLineString(uut.toWay(map))->toString());
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WaySublineTest, "quick");

}

