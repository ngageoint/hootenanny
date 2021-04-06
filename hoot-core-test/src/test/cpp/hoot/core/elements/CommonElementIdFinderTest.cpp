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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/CommonElementIdFinder.h>
#include <hoot/core/io/OsmMapReaderFactory.h>

namespace hoot
{

class CommonElementIdFinderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(CommonElementIdFinderTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  CommonElementIdFinderTest() : HootTestFixture(UNUSED_PATH, UNUSED_PATH)
  {
    setResetType(ResetBasic);
  }

  void runBasicTest()
  {
    // Read both maps in with the default ID generator to ensure there is some ID overlap.

    OsmMapPtr map1 = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map1, "test-files/ToyTestA.osm", false);

    // Reset the map counter to ensure we start over with the IDs for the second map.
    TestUtils::resetBasic();

    OsmMapPtr map2 = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map2, "test-files/ToyTestB.osm", false);

    CPPUNIT_ASSERT_EQUAL(40, CommonElementIdFinder::findCommonElementIds(map1, map2).size());
    CPPUNIT_ASSERT_EQUAL(
      29, CommonElementIdFinder::findElementIdsInFirstAndNotSecond(map1, map2).size());
    CPPUNIT_ASSERT_EQUAL(
      0, CommonElementIdFinder::findElementIdsInSecondAndNotFirst(map1, map2).size());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CommonElementIdFinderTest, "quick");

}
