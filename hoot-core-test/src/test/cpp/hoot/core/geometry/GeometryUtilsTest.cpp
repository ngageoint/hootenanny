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
 * @copyright Copyright (C) 2013, 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/Log.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>


using namespace geos::geom;

namespace hoot
{

class GeometryUtilsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(GeometryUtilsTest);
  CPPUNIT_TEST(calculateDestinationTest);
  CPPUNIT_TEST(envelopeFromConfigStringTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void calculateDestinationTest()
  {
    // Validated against http://www.movable-type.co.uk/scripts/latlong.html
    Coordinate start(1, 0);
    Coordinate dest = GeometryUtils::calculateDestination(start, 90, 200000);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.798, dest.x, 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, dest.y, 1e-3);

    start = Coordinate(-104, 38);
    dest = GeometryUtils::calculateDestination(start, 23, 200000);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-103.087, dest.x, 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(39.652, dest.y, 1e-3);
  }

  void envelopeFromConfigStringTest()
  {
    HOOT_STR_EQUALS("Env[1:3,2:4]", GeometryUtils::envelopeFromConfigString("1,2,3,4").toString());

    QString exceptionMsg;

    try
    {
      GeometryUtils::envelopeFromConfigString("-10,-10,10,");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid envelope string"));

    try
    {
      GeometryUtils::envelopeFromConfigString("-10,-10,a,10");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid envelope string"));

    try
    {
      GeometryUtils::envelopeFromConfigString(" ");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid envelope string"));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GeometryUtilsTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GeometryUtilsTest, "current");

}

