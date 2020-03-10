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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/criterion/ConflatableElementCriterion.h>
#include <hoot/core/TestUtils.h>

namespace hoot
{

class ConflatableElementCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ConflatableElementCriterionTest);
  // TODO: need ignoreGenericConflators=true test
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    geos::geom::Coordinate wayCoords[] = {
      geos::geom::Coordinate(0.0, 0.0),
      geos::geom::Coordinate(1.0, 0.0),
      geos::geom::Coordinate(0.0, 1.0),
      geos::geom::Coordinate(0.0, 0.0),
      geos::geom::Coordinate::getNull() };

    // This is a bit maintenance-prone as it will require an updating with each new
    // ConflatableElementCriterion addition/subtraction, but that's a good thing as it will help
    // us keep track of whenever changes are made to the set of ConflatableElementCriterion.
    CPPUNIT_ASSERT_EQUAL(12, ConflatableElementCriterion::getConflatableCriteria().size());

    const QStringList poiConflatableCriteria =
      ConflatableElementCriterion::getConflatableCriteriaForElement(
        TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, Tags("poi", "yes")), map);
    CPPUNIT_ASSERT_EQUAL(2, poiConflatableCriteria.size());
    CPPUNIT_ASSERT(poiConflatableCriteria.contains("hoot::PoiCriterion"));
    CPPUNIT_ASSERT(poiConflatableCriteria.contains("hoot::PoiPolygonPoiCriterion"));

    const QStringList buildingConflatableCriteria =
      ConflatableElementCriterion::getConflatableCriteriaForElement(
        TestUtils::createWay(map, wayCoords, Status::Unknown1, 15.0, Tags("building", "yes")), map);
    CPPUNIT_ASSERT_EQUAL(3, buildingConflatableCriteria.size());
    CPPUNIT_ASSERT(buildingConflatableCriteria.contains("hoot::AreaCriterion"));
    CPPUNIT_ASSERT(buildingConflatableCriteria.contains("hoot::BuildingCriterion"));
    CPPUNIT_ASSERT(buildingConflatableCriteria.contains("hoot::PoiPolygonPolyCriterion"));

    CPPUNIT_ASSERT_EQUAL(
      0,
      ConflatableElementCriterion::getConflatableCriteriaForElement(
        TestUtils::createWay(map, wayCoords, Status::Unknown1, 15.0, Tags("blah", "blah")), map)
      .size());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflatableElementCriterionTest, "quick");

}
