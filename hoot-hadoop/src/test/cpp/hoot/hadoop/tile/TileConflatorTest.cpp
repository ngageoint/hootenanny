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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/splitter/LargeWaySplitter.h>
#include <hoot/core/conflate/tile/LocalTileWorker.h>
#include <hoot/core/conflate/cleaning/MapCleaner.h>
#include <hoot/core/conflate/tile/TileConflator.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class TileConflatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(TileConflatorTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  TileConflatorTest()
  {
    setResetType(ResetAll);
    TestUtils::mkpath("test-output/conflate");
  }

  void runToyTest()
  {
    conf().set(ConfigOptions().getUuidHelperRepeatableKey(), true);
    conf().set(ConfigOptions().getUnifyOptimizerTimeLimitKey(), -1);

    FileUtils::removeDir("test-output/conflate/TileConflatorTest.osm-cache");

    boost::shared_ptr<TileWorker> worker(new LocalTileWorker());
    TileConflator uut(worker);
    // ~240m
    uut.setBuffer(8.0 / 3600.0);
    uut.setMaxNodesPerBox(5000);

    uut.setSources("test-files/DcGisRoads.osm", "test-files/DcTigerRoads.osm");

    uut.conflate("test-output/conflate/TileConflatorTest.osm");

    HOOT_FILE_EQUALS("test-files/conflate/TileConflatorTest.osm",
                     "test-output/conflate/TileConflatorTest.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TileConflatorTest, "glacial");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TileConflatorTest, "current");

}
