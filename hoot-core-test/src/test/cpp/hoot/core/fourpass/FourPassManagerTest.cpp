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
 * @copyright Copyright (C) 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/conflate/TileConflator.h>
#include <hoot/core/fourpass/FourPassManager.h>
#include <hoot/core/fourpass/LocalTileWorker2.h>
#include <hoot/core/ops/OpList.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/ops/MergeNearbyNodes.h>
#include <hoot/core/ops/ReprojectToPlanarOp.h>
#include <hoot/core/ops/ReprojectToGeographicOp.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
using namespace hoot;

// Tgs
#include <tgs/Statistics/Random.h>

// Qt
#include <QDebug>
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{

class FourPassManagerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(FourPassManagerTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runToyTest()
  {
    Tgs::Random::instance()->seed(0);
    OsmMap::resetCounters();
    Settings::getInstance().clear();

    FileUtils::removeDir("test-output/conflate/TileConflatorTest.osm-cache");

    shared_ptr<TileWorker2> worker(new LocalTileWorker2());
    FourPassManager uut(worker);
    // ~240m
    uut.setBuffer(8.0 / 3600.0);
    uut.setMaxNodesPerBox(5000);
    Envelope env(-77.039, -77.033, 38.892, 38.896);
    shared_ptr<OpList> op(new OpList());
    op->addOp(shared_ptr<OsmMapOperation>(new MapCropper(env)));
    op->addOp(shared_ptr<OsmMapOperation>(new SuperfluousNodeRemover()));
    op->addOp(shared_ptr<OsmMapOperation>(new MergeNearbyNodes(10)));
    uut.setOperation(op);

    uut.setSources("test-files/DcGisRoads.osm", "test-files/DcTigerRoads.osm");

    uut.apply("test-output/fourpass/FourPassManagerTest.osm");

    HOOT_FILE_EQUALS("test-files/fourpass/FourPassManagerTest.osm",
                     "test-output/fourpass/FourPassManagerTest.osm");
  }

};

}

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FourPassManagerTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FourPassManagerTest, "glacial");

