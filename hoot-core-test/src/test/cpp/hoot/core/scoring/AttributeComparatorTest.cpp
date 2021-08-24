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
 * @copyright Copyright (C) 2012, 2013, 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */


// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/scoring/AttributeComparator.h>
#include <hoot/core/util/OpenCv.h>

// Tgs
#include <tgs/Statistics/Random.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>

// Standard
#include <stdio.h>

namespace hoot
{

class AttributeComparatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(AttributeComparatorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  AttributeComparatorTest()
    : HootTestFixture("test-files/",
                      UNUSED_PATH)
  {
  }

  void runTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.read(_inputPath + "DcGisRoads.osm", map);

    OsmMapPtr map2 = std::make_shared<OsmMap>();
    reader.read(_inputPath + "DcTigerRoads.osm", map2);

    AttributeComparator uut(map, map2);
    uut.setIterations(10);
    uut.compareMaps();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5625, uut.getMeanScore(), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1028125, uut.getConfidenceInterval(), 0.001);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AttributeComparatorTest, "quick");

}

