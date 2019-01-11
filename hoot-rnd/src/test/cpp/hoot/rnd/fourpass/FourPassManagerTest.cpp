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
 * @copyright Copyright (C) 2013, 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/tile/TileConflator.h>
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
#include <hoot/rnd/fourpass/FourPassManager.h>
#include <hoot/rnd/fourpass/LocalTileWorker2.h>

// Tgs
#include <tgs/Statistics/Random.h>

using namespace geos::geom;

namespace hoot
{

class FourPassManagerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(FourPassManagerTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  FourPassManagerTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath("test-output/fourpass");
  }

  void runToyTest()
  {
    FileUtils::removeDir("test-output/fourpass/FourPassManagerTest.osm-cache");

    boost::shared_ptr<TileWorker2> worker(new LocalTileWorker2());
    FourPassManager uut(worker);
    // ~240m
    uut.setBuffer(8.0 / 3600.0);
    uut.setMaxNodesPerBox(5000);
    Envelope env(-77.039, -77.033, 38.892, 38.896);
    boost::shared_ptr<OpList> op(new OpList());
    op->addOp(boost::shared_ptr<OsmMapOperation>(new MapCropper(env)));
    op->addOp(boost::shared_ptr<OsmMapOperation>(new SuperfluousNodeRemover()));
    op->addOp(boost::shared_ptr<OsmMapOperation>(new MergeNearbyNodes(10)));
    uut.setOperation(op);

    uut.setSources("test-files/DcGisRoads.osm", "test-files/DcTigerRoads.osm");

    uut.apply("test-output/fourpass/FourPassManagerTest.osm");

    HOOT_FILE_EQUALS("test-files/fourpass/FourPassManagerTest.osm",
                     "test-output/fourpass/FourPassManagerTest.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FourPassManagerTest, "glacial");

}
