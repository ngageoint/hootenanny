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
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/conflate/LargeWaySplitter.h>
#include <hoot/core/conflate/LocalTileWorker.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/conflate/TileConflator.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
using namespace hoot;

// Boost
using namespace boost;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{

class TileConflatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TileConflatorTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runToyTest()
  {
    srand(0);
    OsmMap::resetCounters();
    Settings::getInstance().clear();
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

    CPPUNIT_ASSERT_EQUAL(true, TestUtils::compareMaps("test-files/conflate/TileConflatorTest.osm",
      "test-output/conflate/TileConflatorTest.osm"));
  }

  virtual void tearDown()
  {
    Settings::getInstance().clear();
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TileConflatorTest, "glacial");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TileConflatorTest, "current");

}
