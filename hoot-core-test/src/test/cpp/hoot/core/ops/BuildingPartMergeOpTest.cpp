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
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/BuildingPartMergeOp.h>
#include <hoot/core/util/Log.h>
using namespace hoot;

// Boost
using namespace boost;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/io/WKTReader.h>
#include <geos/geom/Point.h>

// hoot
#include <hoot/core/util/MapProjector.h>

// Qt
#include <QDebug>
#include <QDir>

// TGS
#include <tgs/Statistics/Random.h>

#include "../TestUtils.h"

namespace hoot
{
using namespace Tgs;

class BuildingPartMergeOpTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BuildingPartMergeOpTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runToyTest()
  {
    OsmXmlReader reader;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ops/BuildingPartMergeOp/ToyBuildings.osm", map);

    BuildingPartMergeOp uut;
    uut.apply(map);

    MapProjector::projectToWgs84(map);

    QDir().mkpath("test-output/ops/BuildingPartMergeOp/");
    OsmXmlWriter writer;
    writer.write(map, "test-output/ops/BuildingPartMergeOp/ToyBuildings.osm");
    HOOT_FILE_EQUALS("test-files/ops/BuildingPartMergeOp/ToyBuildingsOutput.osm",
                     "test-output/ops/BuildingPartMergeOp/ToyBuildings.osm");
  }

};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingPartMergeOpTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingPartMergeOpTest, "current");


