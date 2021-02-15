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
 * @copyright Copyright (C) 2015, 2016, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/BuildingWayNodeCriterion.h>
#include <hoot/core/io/OsmMapReaderFactory.h>

namespace hoot
{

class BuildingWayNodeCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(BuildingWayNodeCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  BuildingWayNodeCriterionTest() :
  HootTestFixture("test-files/criterion/", UNUSED_PATH)
  {
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "BuildingWayNodeCriterionTest.osm");

    BuildingWayNodeCriterion uut;
    uut.setOsmMap(map.get());

    HOOT_STR_EQUALS(1, uut.isSatisfied(map->getNode(-20420)));
    HOOT_STR_EQUALS(1, uut.isSatisfied(map->getNode(-20421)));
    HOOT_STR_EQUALS(1, uut.isSatisfied(map->getNode(-74242)));

    HOOT_STR_EQUALS(0, uut.isSatisfied(map->getNode(-271160)));
    HOOT_STR_EQUALS(0, uut.isSatisfied(map->getNode(-271159)));
    HOOT_STR_EQUALS(0, uut.isSatisfied(map->getNode(-271173)));

    HOOT_STR_EQUALS(1, uut.isSatisfied(map->getNode(-271161)));

    HOOT_STR_EQUALS(0, uut.isSatisfied(map->getWay(-3295)));
    HOOT_STR_EQUALS(0, uut.isSatisfied(map->getWay(-27044)));
    HOOT_STR_EQUALS(0, uut.isSatisfied(map->getWay(-27037)));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingWayNodeCriterionTest, "quick");

}
