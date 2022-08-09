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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/criterion/NonConflatableCriterion.h>
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
    uut.setIgnoreGenericConflators(false);
    OsmMapPtr map = std::make_shared<OsmMap>();
    geos::geom::Coordinate wayCoords[] = {
      geos::geom::Coordinate(0.0, 0.0),
      geos::geom::Coordinate(1.0, 0.0),
      geos::geom::Coordinate(0.0, 1.0),
      geos::geom::Coordinate(0.0, 0.0),
      geos::geom::Coordinate::getNull() };

    // Criteria satisfaction is negated, as we're checking to see if the element is *not*
    // conflatable.

    CPPUNIT_ASSERT(
      !uut.isSatisfied(
        TestUtils::createNode(map, "", Status::Unknown1, 0.0, 0.0, 15.0, Tags("poi", "yes"))));

    CPPUNIT_ASSERT(
      !uut.isSatisfied(
        TestUtils::createWay(map, wayCoords, "", Status::Unknown1, 15.0, Tags(MetadataTags::Building(), "yes"))));

    // Untyped feature are now conflatable by default, if NonConflatableCriterion is set to consider
    // the generic geometry conflate scripts.
    CPPUNIT_ASSERT(
      !uut.isSatisfied(
        TestUtils::createWay(map, wayCoords, "", Status::Unknown1, 15.0, Tags("blah", "blah"))));

    // If its set to ignore the generic geometry scripts, then an untyped feature won't be
    // confltable.
    uut.setIgnoreGenericConflators(true);
    CPPUNIT_ASSERT(
      uut.isSatisfied(
        TestUtils::createWay(map, wayCoords, "", Status::Unknown1, 15.0, Tags("blah", "blah"))));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NonConflatableCriterionTest, "quick");

}
