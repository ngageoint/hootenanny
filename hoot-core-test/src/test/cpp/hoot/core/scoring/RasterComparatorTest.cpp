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
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/scoring/RasterComparator.h>
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

// Standard
#include <stdio.h>

namespace hoot
{

class RasterComparatorTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(RasterComparatorTest);
    CPPUNIT_TEST(runTest);
    CPPUNIT_TEST_SUITE_END();

public:

    void runTest()
    {
        OsmXmlReader reader;

        boost::shared_ptr<OsmMap> map(new OsmMap());
        reader.read("test-files/ToyTestA.osm", map);

        boost::shared_ptr<OsmMap> map2(new OsmMap());
        reader.read("test-files/ToyTestB.osm", map2);

        RasterComparator uut(map, map2);
        uut.setPixelSize(3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.884, uut.compareMaps(), 0.0025);

        uut.setPixelSize(2);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.892, uut.compareMaps(), 0.002);

        uut.setPixelSize(1);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.894, uut.compareMaps(), 0.002);
    }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RasterComparatorTest, "quick");

}
