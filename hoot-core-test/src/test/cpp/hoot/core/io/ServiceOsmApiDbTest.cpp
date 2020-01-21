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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmApiDb.h>

using namespace std;

namespace hoot
{

class ServiceOsmApiDbTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServiceOsmApiDbTest);
  CPPUNIT_TEST(runToOsmApiCoordConvertTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runToOsmApiCoordConvertTest()
  {
    //convert precision from 0.01 nanodegrees to 100 nanodegrees

    double hootApiCoordValue = 38.85433192782;
    long osmApiDbCoordValue = OsmApiDb::toOsmApiDbCoord(hootApiCoordValue);
    CPPUNIT_ASSERT_EQUAL(osmApiDbCoordValue, (long)388543319);

    hootApiCoordValue = 38.85433199782;
    osmApiDbCoordValue = OsmApiDb::toOsmApiDbCoord(hootApiCoordValue);
    CPPUNIT_ASSERT_EQUAL(osmApiDbCoordValue, (long)388543320);
  }
};

#ifdef HOOT_HAVE_SERVICES
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceOsmApiDbTest, "slow");
#endif  // HOOT_HAVE_SERVICES

}
