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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/conflate/river/RiverMaximalSublineSettingOptimizer.h>
#include <hoot/core/elements/MapProjector.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class RiverMaximalSublineSettingOptimizerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RiverMaximalSublineSettingOptimizerTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RiverMaximalSublineSettingOptimizerTest() :
  HootTestFixture("test-files/conflate/generic/rivers/", UNUSED_PATH)
  {
    setResetType(ResetBasic);
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "Haiti_CNIGS_Rivers_REF1-cropped-2.osm", false, Status::Unknown1);
    OsmMapReaderFactory::read(
      map, _inputPath + "Haiti_osm_waterway_ss_REF2-cropped-2.osm", false, Status::Unknown2);
    MapProjector::projectToPlanar(map);

    RiverMaximalSublineSettingOptimizer uut;

    // This should return a scaled recursion value based on the set river length min/max.
    uut.setMaxIterationsScalingValue(450);
    uut.setMaxRiverLengthScalingValue(500000.0);
    uut.setMinIterationsScalingValue(5);
    uut.setMinRiverLengthScalingValue(15000.0);
    CPPUNIT_ASSERT_EQUAL(403, uut.getFindBestMatchesMaxRecursions(map));

    // This should return the recursion value that indicates MaximalSubline will automatically
    // determine the upper limit (-1).
    uut.setMinRiverLengthScalingValue(DBL_MAX);
    uut.setMaxRiverLengthScalingValue(500000.0);
    CPPUNIT_ASSERT_EQUAL(-1, uut.getFindBestMatchesMaxRecursions(map));

    // This should return the min possible recursion value.
    uut.setMinRiverLengthScalingValue(15000.0);
    uut.setMaxRiverLengthScalingValue(0.0);
    CPPUNIT_ASSERT_EQUAL(5, uut.getFindBestMatchesMaxRecursions(map));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RiverMaximalSublineSettingOptimizerTest, "quick");

}
