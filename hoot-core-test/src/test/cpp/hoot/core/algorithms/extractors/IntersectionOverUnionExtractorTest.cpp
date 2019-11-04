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
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/extractors/IntersectionOverUnionExtractor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class IntersectionOverUnionExtractorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(IntersectionOverUnionExtractorTest);
  CPPUNIT_TEST(runBuildingsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  IntersectionOverUnionExtractorTest() :
  HootTestFixture(
    "test-files/algorithms/extractors/IntersectionOverUnionExtractorTest/",
    "test-output/algorithms/extractors/IntersectionOverUnionExtractorTest/")
  {
    setResetType(ResetBasic);
  }

  void runBuildingsTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "IntersectionOverUnionExtractorTest-buildings-in.osm");
    IntersectionOverUnionExtractor uut;

    ConstElementPtr building1 = TestUtils::getElementWithTag(map, "name", "Building 1");
    ConstElementPtr building2 = TestUtils::getElementWithTag(map, "name", "Building 2");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.02, uut.extract(*map, building1, building2), 1e-3);

    ConstElementPtr building3 = TestUtils::getElementWithTag(map, "name", "Building 3");
    ConstElementPtr building4 = TestUtils::getElementWithTag(map, "name", "Building 4");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.764, uut.extract(*map, building3, building4), 1e-3);

    ConstElementPtr building5 = TestUtils::getElementWithTag(map, "name", "Building 5");
    ConstElementPtr building6 = TestUtils::getElementWithTag(map, "name", "Building 6");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.978, uut.extract(*map, building5, building6), 1e-3);

    ConstElementPtr building7 = TestUtils::getElementWithTag(map, "name", "Building 7");
    ConstElementPtr building8 = TestUtils::getElementWithTag(map, "name", "Building 8");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.185, uut.extract(*map, building7, building8), 1e-3);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IntersectionOverUnionExtractorTest, "quick");

}
