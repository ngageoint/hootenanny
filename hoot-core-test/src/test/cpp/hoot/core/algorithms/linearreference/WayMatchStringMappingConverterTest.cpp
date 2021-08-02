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
 * @copyright Copyright (C) 2014, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/linearreference/NaiveWayMatchStringMapping.h>
#include <hoot/core/algorithms/linearreference/WayMatchStringMappingConverter.h>

namespace hoot
{

class WayMatchStringMappingConverterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WayMatchStringMappingConverterTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  WayMatchStringMappingConverterTest()
  {
    setResetType(ResetAll);
  }

  /**
   * Create a couple of simple way strings and verify that the generated subline match is correct.
   */
  void runTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr n1a = TestUtils::createNode(map, "", Status::Unknown1, 0, 0);
    NodePtr n1b = TestUtils::createNode(map, "", Status::Unknown1, 100, 0);
    NodePtr n1c = TestUtils::createNode(map, "", Status::Unknown1, 200, 0);
    WayPtr w1 = TestUtils::createWay(map, QList<NodePtr>() << n1a << n1b, "", Status::Unknown1);
    WayPtr w2 = TestUtils::createWay(map, QList<NodePtr>() << n1b << n1c, "", Status::Unknown1);

    NodePtr n2a = TestUtils::createNode(map, "", Status::Unknown1, 0, 10);
    NodePtr n2b = TestUtils::createNode(map, "", Status::Unknown1, 150, 10);
    NodePtr n2c = TestUtils::createNode(map, "", Status::Unknown1, 210, 10);
    WayPtr w3 = TestUtils::createWay(map, QList<NodePtr>() << n2a << n2b, "", Status::Unknown1);
    WayPtr w4 = TestUtils::createWay(map, QList<NodePtr>() << n2c << n2b, "", Status::Unknown1);

    WayStringPtr wstr1 = std::make_shared<WayString>();
    wstr1->append(WaySubline(WayLocation(map, w1, 0), WayLocation::createAtEndOfWay(map, w1)));
    wstr1->append(WaySubline(WayLocation(map, w2, 0), WayLocation::createAtEndOfWay(map, w2)));
    WayStringPtr wstr2 = std::make_shared<WayString>();
    wstr2->append(WaySubline(WayLocation(map, w3, 0), WayLocation::createAtEndOfWay(map, w3)));
    wstr2->append(WaySubline(WayLocation::createAtEndOfWay(map, w4), WayLocation(map, w4, 0)));

    WayMatchStringMappingPtr mapping = std::make_shared<NaiveWayMatchStringMapping>(wstr1, wstr2);
    WaySublineMatchStringPtr wsms = WayMatchStringMappingConverter().
      toWaySublineMatchString(mapping);

    //LOG_VAR(TestUtils::toQuotedString(hoot::toString(wsms)));
    HOOT_STR_EQUALS("matches:\n"
      "subline 1: start: way(-1) index: 0 fraction: 0 end: way(-1) index: 1 fraction: 0\n"
      "subline 2: start: way(-3) index: 0 fraction: 0 end: way(-3) index: 0 fraction: 0.7\n"
      "subline 1: start: way(-2) index: 0 fraction: 0 end: way(-2) index: 0 fraction: 0.428571428571429\n"
      "subline 2: start: way(-3) index: 0 fraction: 0.7 end: way(-3) index: 1 fraction: 0\n"
      "reversed, subline 1: start: way(-2) index: 0 fraction: 0.428571428571429 end: way(-2) index: 1 fraction: 0\n"
      "subline 2: start: way(-4) index: 0 fraction: 0 end: way(-4) index: 1 fraction: 0",
      wsms);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayMatchStringMappingConverterTest, "quick");

}



