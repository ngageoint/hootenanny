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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/criterion/NonConflatableCriterion.h>
#include <hoot/core/criterion/ConflatableElementCriterion.h>
#include <hoot/core/TestUtils.h>

namespace hoot
{

class NonConflatableCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NonConflatableCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    NonConflatableCriterion uut;
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
    CPPUNIT_ASSERT_EQUAL(9, ConflatableElementCriterion::getConflatableCriteria().size());

    // Criteria satisfaction is negated, as we're checking to see if the element is *not*
    // conflatable.

    ConstNodePtr poi =
      TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, Tags("poi", "yes"));
    CPPUNIT_ASSERT(!uut.isSatisfied(poi));
    const QStringList poiConflatableCriteria =
      ConflatableElementCriterion::getConflatableCriteriaForElement(poi);
    CPPUNIT_ASSERT_EQUAL(2, poiConflatableCriteria.size());
    CPPUNIT_ASSERT(poiConflatableCriteria.contains("hoot::PoiCriterion"));
    CPPUNIT_ASSERT(poiConflatableCriteria.contains("hoot::PoiPolygonPoiCriterion"));

    ConstWayPtr building =
      TestUtils::createWay(
        map, wayCoords, Status::Unknown1, 15.0, Tags("building", "yes"));
    CPPUNIT_ASSERT(!uut.isSatisfied(building));
    const QStringList buildingConflatableCriteria =
      ConflatableElementCriterion::getConflatableCriteriaForElement(building);
    CPPUNIT_ASSERT_EQUAL(3, buildingConflatableCriteria.size());
    CPPUNIT_ASSERT(buildingConflatableCriteria.contains("hoot::AreaCriterion"));
    CPPUNIT_ASSERT(buildingConflatableCriteria.contains("hoot::BuildingCriterion"));
    CPPUNIT_ASSERT(buildingConflatableCriteria.contains("hoot::PoiPolygonPolyCriterion"));

    ConstWayPtr nonsense =
      TestUtils::createWay(map, wayCoords, Status::Unknown1, 15.0, Tags("blah", "blah"));
    CPPUNIT_ASSERT(uut.isSatisfied(nonsense));;
    CPPUNIT_ASSERT_EQUAL(
      0, ConflatableElementCriterion::getConflatableCriteriaForElement(nonsense).size());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NonConflatableCriterionTest, "quick");

}
