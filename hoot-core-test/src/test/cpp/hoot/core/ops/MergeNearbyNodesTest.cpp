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
 * @copyright Copyright (C) 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/ops/MergeNearbyNodes.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Progress.h>
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

#include "../TestUtils.h"

namespace hoot
{

class MergeNearbyNodesTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MergeNearbyNodesTest);
    CPPUNIT_TEST(runBasicTest);
    CPPUNIT_TEST_SUITE_END();

public:

    void runBasicTest()
    {
      OgrReader uut;

      Progress progress("MergeNearbyNodesTest");
      boost::shared_ptr<OsmMap> map(new OsmMap());
      uut.read("test-files/jakarta_raya_coastline.shp", "", map, progress);

      MapProjector::projectToOrthographic(map);

      CPPUNIT_ASSERT_EQUAL(604, (int)map->getNodes().size());

      // merge all nodes within a meter.
      MergeNearbyNodes::mergeNodes(map, 1.0);

      CPPUNIT_ASSERT_EQUAL(601, (int)map->getNodes().size());

      MapProjector::projectToWgs84(map);

      OsmXmlWriter writer;
      writer.write(map, "output.osm");

    }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MergeNearbyNodesTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MergeNearbyNodesTest, "quick");

}
