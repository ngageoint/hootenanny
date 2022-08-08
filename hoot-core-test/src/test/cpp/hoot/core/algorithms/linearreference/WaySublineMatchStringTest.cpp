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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/linearreference/WaySublineMatchString.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class WaySublineMatchStringTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WaySublineMatchStringTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runTouchesTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate c[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                       Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                       Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map,  c, "", Status::Unknown1, 1);
    WayPtr w2 = TestUtils::createWay(map, c, "", Status::Unknown2, 1);

    OsmMapPtr copy = std::make_shared<OsmMap>();
    set<ElementId> eids;
    eids.insert(w1->getElementId());
    eids.insert(w2->getElementId());
    CopyMapSubsetOp(map, eids).apply(copy);

    WaySublineMatchString::MatchCollection mc;
    mc.push_back(WaySublineMatch(
                   WaySubline(WayLocation(map, w1, 10), WayLocation(map, w1, 50)),
                   WaySubline(WayLocation(map, w2, 20.0), WayLocation(map, w2, 150.0))));
    mc.push_back(WaySublineMatch(
                   WaySubline(WayLocation(map, w1, 70), WayLocation(map, w1, 90)),
                   WaySubline(WayLocation(map, w2, 170.0), WayLocation(map, w2, 210.0))));
    WaySublineMatchString uut(mc);

    WaySublineMatchString uut2(uut, copy);
    HOOT_STR_EQUALS(uut.toString(), uut2.toString());
  }

  void runTouchesTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate c[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                       Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                       Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, c, "", Status::Unknown1, 1);
    WayPtr w2 = TestUtils::createWay(map, c, "", Status::Unknown2, 1);

    OsmMapPtr copy = std::make_shared<OsmMap>();
    set<ElementId> eids;
    eids.insert(w1->getElementId());
    eids.insert(w2->getElementId());
    CopyMapSubsetOp(map, eids).apply(copy);

    WaySublineMatchString::MatchCollection mc1;
    mc1.push_back(WaySublineMatch(
                   WaySubline(WayLocation(map, w1, 10), WayLocation(map, w1, 50)),
                   WaySubline(WayLocation(map, w2, 20.0), WayLocation(map, w2, 150.0))));
    mc1.push_back(WaySublineMatch(
                   WaySubline(WayLocation(map, w1, 70), WayLocation(map, w1, 90)),
                   WaySubline(WayLocation(map, w2, 170.0), WayLocation(map, w2, 210.0))));
    WaySublineMatchString uut1(mc1);

    HOOT_STR_EQUALS(1, uut1.touches(uut1));

    WaySublineMatchString::MatchCollection mc2;
    mc2.push_back(WaySublineMatch(
                   WaySubline(WayLocation(map, w1, 50), WayLocation(map, w1, 60)),
                   WaySubline(WayLocation(map, w2, 151.0), WayLocation(map, w2, 152.0))));
    mc2.push_back(WaySublineMatch(
                   WaySubline(WayLocation(map, w1, 91), WayLocation(map, w1, 92)),
                   WaySubline(WayLocation(map, w2, 160.0), WayLocation(map, w2, 161.0))));
    WaySublineMatchString uut2(mc2);

    HOOT_STR_EQUALS(1, uut2.touches(uut2));
    HOOT_STR_EQUALS(1, uut1.touches(uut2));
    HOOT_STR_EQUALS(1, uut2.touches(uut1));

    WaySublineMatchString::MatchCollection mc3;
    mc3.push_back(WaySublineMatch(
                   WaySubline(WayLocation(map, w1, 51), WayLocation(map, w1, 52)),
                   WaySubline(WayLocation(map, w2, 151.0), WayLocation(map, w2, 152.0))));
    mc3.push_back(WaySublineMatch(
                   WaySubline(WayLocation(map, w1, 91), WayLocation(map, w1, 92)),
                   WaySubline(WayLocation(map, w2, 160.5), WayLocation(map, w2, 163.0))));
    WaySublineMatchString uut3(mc3);

    HOOT_STR_EQUALS(0, uut1.touches(uut3));
    HOOT_STR_EQUALS(0, uut3.touches(uut1));
    HOOT_STR_EQUALS(1, uut2.touches(uut3));
    HOOT_STR_EQUALS(1, uut3.touches(uut2));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WaySublineMatchStringTest, "quick");

}


