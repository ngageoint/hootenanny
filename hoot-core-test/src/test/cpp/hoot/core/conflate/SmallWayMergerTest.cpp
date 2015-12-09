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
#include <hoot/core/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/SmallWayMerger.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/util/Log.h>
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

class SmallWayMergerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SmallWayMergerTest);
    CPPUNIT_TEST(runBasicTest);
    CPPUNIT_TEST_SUITE_END();

public:

    void runBasicTest()
    {
      OsmReader reader;

      shared_ptr<OsmMap> map(new OsmMap());
      reader.setDefaultStatus(Status::Unknown1);
      reader.read("test-files/conflate/SmallWayMergerInput1.osm", map);

      MapProjector::reprojectToPlanar(map);
      SmallWayMerger::mergeWays(map, 15.0);
      MapProjector::reprojectToWgs84(map);

      OsmWriter writer;
      writer.write(map, "test-output/conflate/SmallWayMergerOutput1.osm");

    }

};

CPPUNIT_TEST_SUITE_REGISTRATION(SmallWayMergerTest);

