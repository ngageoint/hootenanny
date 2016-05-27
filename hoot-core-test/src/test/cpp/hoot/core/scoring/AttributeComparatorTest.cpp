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
 * @copyright Copyright (C) 2012, 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */


// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/scoring/AttributeComparator.h>
#include <hoot/core/util/OpenCv.h>
using namespace hoot;

// Tgs
#include <tgs/Statistics/Random.h>

// Boost
using namespace boost;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>

// Standard
#include <stdio.h>

class AttributeComparatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(AttributeComparatorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    OsmReader reader;

    Tgs::Random::instance()->seed(0);

    shared_ptr<OsmMap> map(new OsmMap());
    reader.read("test-files/DcGisRoads.osm", map);

    shared_ptr<OsmMap> map2(new OsmMap());
    reader.read("test-files/DcTigerRoads.osm", map2);

    AttributeComparator uut(map, map2);
    uut.setIterations(10);
    uut.compareMaps();
//    LOG_INFO("mean: " << uut.getMeanScore());
//    LOG_INFO("ci: " << uut.getConfidenceInterval());
//    Log::getInstance().setLevel(Log::Warn);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.57142, uut.getMeanScore(), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1175, uut.getConfidenceInterval(), 0.001);
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(AttributeComparatorTest);



