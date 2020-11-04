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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/util/Settings.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class MapCleanerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MapCleanerTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MapCleanerTest()
    : HootTestFixture("test-files/ops/MapCleanerTest/",
                      "test-output/ops/MapCleanerTest/")
  {
    setResetType(ResetAll);
  }

  void runBasicTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcTigerRoads.osm", map);

    MapCleaner().apply(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.write(map, _outputPath + "MapCleaner.osm");

    HOOT_FILE_EQUALS( _inputPath + "MapCleaner.osm",
                     _outputPath + "MapCleaner.osm");
  }

};

//putting this in the quick test suite causes MatchComparatorTest to fail
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MapCleanerTest, "slow");

}
