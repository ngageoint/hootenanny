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
 * @copyright Copyright (C) 2015, 2016, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/io/OsmMapReaderFactory.h>

namespace hoot
{

class BuildingCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(BuildingCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  BuildingCriterionTest() : HootTestFixture("test-files/criterion/", UNUSED_PATH)
  {
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "ComplexBuildings.osm");

    BuildingCriterion uut;
    uut.setOsmMap(map.get());
    HOOT_STR_EQUALS(1, uut.isSatisfied(TestUtils::getElementWithNote(map, "targetandbestbuy")));
    HOOT_STR_EQUALS(0, uut.isSatisfied(TestUtils::getElementWithNote(map, "target")));
    HOOT_STR_EQUALS(0, uut.isSatisfied(TestUtils::getElementWithNote(map, "bestbuy")));
    HOOT_STR_EQUALS(0, uut.isSatisfied(TestUtils::getElementWithNote(map, "pho")));
    HOOT_STR_EQUALS(0, uut.isSatisfied(TestUtils::getElementWithNote(map, "panera")));
    HOOT_STR_EQUALS(0, uut.isSatisfied(TestUtils::getElementWithNote(map, "freddys")));
    HOOT_STR_EQUALS(0, uut.isSatisfied(TestUtils::getElementWithNote(map, "jewelry")));
    HOOT_STR_EQUALS(1, uut.isSatisfied(TestUtils::getElementWithNote(map, "paneragroup")));
    HOOT_STR_EQUALS(1, uut.isSatisfied(TestUtils::getElementWithNote(map, "jewelryandfreddys")));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingCriterionTest, "quick");

}
