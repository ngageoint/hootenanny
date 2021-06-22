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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2012, 2013, 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */


// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/splitter/IntersectionSplitter.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/scoring/GraphComparator.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/util/OpenCv.h>

// Tgs
#include <tgs/Statistics/Random.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class GraphComparatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(GraphComparatorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  GraphComparatorTest()
    : HootTestFixture("test-files/",
                      UNUSED_PATH)
  {
    setResetType(ResetBasic);
  }

  void runTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.read(_inputPath + "ToyTestA.osm", map);

    OsmMapPtr map2(new OsmMap());
    reader.read(_inputPath + "ToyTestB.osm", map2);

    const WayMap& w1 = map->getWays();
    for (WayMap::const_iterator it = w1.begin(); it != w1.end(); ++it)
    {
      WayPtr w = map->getWay(it->second->getId());
      w->setTag("highway", "road");
    }

    const WayMap& w2 = map2->getWays();
    for (WayMap::const_iterator it = w2.begin(); it != w2.end(); ++it)
    {
      WayPtr w = map2->getWay(it->second->getId());
      w->setTag("highway", "road");
    }

    GraphComparator uut(map, map2);
    uut.setIterations(3);
    uut.setMaxThreads(2);
    uut.setPixelSize(10);
    uut.compareMaps();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.975078263893709, uut.getMeanScore(), 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00161314619733044, uut.getConfidenceInterval(), 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.975158195237589, uut.getMedianScore(), 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00169851135186191, uut.getStandardDeviation(), 0.00001);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GraphComparatorTest, "quick");

}

