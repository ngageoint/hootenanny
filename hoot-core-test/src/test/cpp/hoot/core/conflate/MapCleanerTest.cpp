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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/ops/MergeNearbyNodes.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/GeometryUtils.h>
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

class MapCleanerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MapCleanerTest);
    CPPUNIT_TEST(runBasicTest);
    CPPUNIT_TEST_SUITE_END();

public:

    void runBasicTest()
    {
      Settings::getInstance().clear();
      OsmReader reader;
      OsmMap::resetCounters();
      OsmSchema::getInstance().loadDefault();
      shared_ptr<OsmMap> map(new OsmMap());
      reader.setDefaultStatus(Status::Unknown1);
      reader.read("test-files/DcTigerRoads.osm", map);

      MapCleaner().apply(map);

      MapProjector::reprojectToWgs84(map);

      QDir().mkpath("test-output/conflate");
      OsmWriter writer;
      writer.write(map, "test-output/conflate/MapCleaner.osm");

      HOOT_FILE_EQUALS("test-files/conflate/MapCleaner.osm",
                       "test-output/conflate/MapCleaner.osm");
    }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MapCleanerTest, "slow");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MapCleanerTest, "current");

}
