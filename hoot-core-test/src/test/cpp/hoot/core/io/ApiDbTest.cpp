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

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/util/ConfigOptions.h>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"

namespace hoot
{

class ApiDbTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ApiDbTest);
  CPPUNIT_TEST(runOsmApiToHootApiCoordConvertTest);
  CPPUNIT_TEST(runHootApiToOsmApiCoordConvertTest);
  CPPUNIT_TEST(runHootApiToOsmApiCoordConvert2Test);
  CPPUNIT_TEST_SUITE_END();

public:

  void runOsmApiToHootApiCoordConvertTest()
  {
    const long osmApiDbCoordValue = 388543319;
    double hootApiCoordValue = osmApiDbCoordValue / (double)ApiDb::COORDINATE_SCALE;
    CPPUNIT_ASSERT_EQUAL(hootApiCoordValue, (double)38.8543319);
  }

  void runHootApiToOsmApiCoordConvertTest()
  {
    const double hootApiCoordValue = 38.8543319;
    long osmApiDbCoordValue = (long)ApiDb::round(hootApiCoordValue * ApiDb::COORDINATE_SCALE, 7);
    CPPUNIT_ASSERT_EQUAL(osmApiDbCoordValue, (long)388543319);
  }

  void runHootApiToOsmApiCoordConvert2Test()
  {
    const double hootApiCoordValue = 38.8543319;
    long osmApiDbCoordValue = (long)ApiDb::round2(hootApiCoordValue * ApiDb::COORDINATE_SCALE, 7);
    CPPUNIT_ASSERT_EQUAL(osmApiDbCoordValue, (long)388543319);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ApiDbTest, "quick");

}
