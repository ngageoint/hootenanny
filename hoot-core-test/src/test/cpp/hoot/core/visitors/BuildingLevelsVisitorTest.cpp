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
 * @copyright Copyright (C) 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/visitors/BuildingLevelsVisitor.h>

namespace hoot
{

class BuildingLevelsVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(BuildingLevelsVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  BuildingLevelsVisitorTest()
    : HootTestFixture("test-files/visitors/BuildingHeightVisitorTest/",
                      UNUSED_PATH)
  {
    setResetType(ResetBasic);
  }

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    // reusing BuildingHeightVisitorTest's input
    OsmMapReaderFactory::read(
      map, _inputPath + "BuildingHeightVisitorTestInput.osm", false, Status::Unknown1);

    BuildingLevelsVisitor uut;
    map->visitRo(uut);

    CPPUNIT_ASSERT_EQUAL(5L, uut.numWithStat());
    CPPUNIT_ASSERT_EQUAL(20, (int)uut.getStat());
    CPPUNIT_ASSERT_EQUAL(1, (int)uut.getMin());
    CPPUNIT_ASSERT_EQUAL(10, (int)uut.getMax());
    CPPUNIT_ASSERT_EQUAL(4.0, uut.getAverage());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingLevelsVisitorTest, "quick");

}
