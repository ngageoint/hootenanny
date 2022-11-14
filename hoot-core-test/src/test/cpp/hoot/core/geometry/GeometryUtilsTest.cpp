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
 * @copyright Copyright (C) 2013, 2015, 2016, 2017, 2018, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/geometry/GeometryUtils.h>

using namespace geos::geom;

namespace hoot
{

class GeometryUtilsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(GeometryUtilsTest);
  CPPUNIT_TEST(calculateDestinationTest);
  CPPUNIT_TEST(envelopeTest);
  CPPUNIT_TEST(polygonTest);
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

  void envelopeTest()
  { 
    QString exceptionMsg;

    CPPUNIT_ASSERT(GeometryUtils::isEnvelopeString("1,2,3,4"));
    CPPUNIT_ASSERT(!GeometryUtils::isEnvelopeString(" "));
    CPPUNIT_ASSERT(!GeometryUtils::isEnvelopeString("-10,-10,10,"));
    CPPUNIT_ASSERT(!GeometryUtils::isEnvelopeString("-10,-10,a,10"));

    HOOT_STR_EQUALS("Env[1:3,2:4]", GeometryUtils::envelopeFromString("1,2,3,4").toString());

    exceptionMsg = "";
    try
    {
      GeometryUtils::envelopeFromString("-10,-10,10,");
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid envelope string"));

    exceptionMsg = "";
    try
    {
      GeometryUtils::envelopeFromString("-10,-10,a,10");
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid envelope string"));

    HOOT_STR_EQUALS("-71.4809000000000054,-71.4568799999999982,42.4808000000000021,42.4930000000000021",
                    GeometryUtils::toLonLatString(Envelope("-71.4809,42.4808,-71.45688,42.49368")));
  }

  void polygonTest()
  {
    QString exceptionMsg;

    // closed poly
    CPPUNIT_ASSERT(GeometryUtils::isPolygonString("-71.4745,42.4841;-71.4669,42.4918;-71.4619,42.4839;-71.4745,42.4841"));
    // not closed but valid b/c the parser will automatically close it up
    CPPUNIT_ASSERT(GeometryUtils::isPolygonString("-71.4745,42.4841;-71.4669,42.4918;-71.4619,42.4839"));
    CPPUNIT_ASSERT(!GeometryUtils::isPolygonString(" "));
    // not enough points
    CPPUNIT_ASSERT(!GeometryUtils::isPolygonString("-71.4745,42.4841;-71.4669,42.4918"));
    CPPUNIT_ASSERT(!GeometryUtils::isPolygonString("-71.4745,42.4841;-71.4669,a;-71.4619,42.4839"));

    HOOT_STR_EQUALS("POLYGON ((-71.4745000000000061 42.4840999999999980, -71.4668999999999954 42.4917999999999978, -71.4619000000000000 42.4838999999999984, -71.4745000000000061 42.4840999999999980))",
                    GeometryUtils::polygonFromString("-71.4745,42.4841;-71.4669,42.4918;-71.4619,42.4839;-71.4745,42.4841")->toString());

    exceptionMsg = "";
    try
    {
      GeometryUtils::polygonFromString("-71.4745,42.4841;-71.4669,42.4918");
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Polygon string must have at least three points"));

    exceptionMsg = "";
    try
    {
      GeometryUtils::polygonFromString("-71.4745,42.4841;a,42.4918;-71.4619,42.4839");
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid polygon x coordinate value"));

    exceptionMsg = "";
    try
    {
      GeometryUtils::polygonFromString("-71.4745,42.4841;-71.4669,a;-71.4619,42.4839");
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid polygon y coordinate value"));

    HOOT_STR_EQUALS("POLYGON ((-71.4745000000000061 42.4840999999999980, -71.4668999999999954 42.4917999999999978, -71.4619000000000000 42.4838999999999984, -71.4745000000000061 42.4840999999999980))",
                    GeometryUtils::polygonFromString("-71.4745,42.4841;-71.4669,42.4918;-71.4619,42.4839;-71.4745,42.4841")->toString());

    HOOT_STR_EQUALS("POLYGON ((-71.4809000000000054 -71.4568799999999982, -71.4809000000000054 42.4930000000000021, 42.4808000000000021 42.4930000000000021, 42.4808000000000021 -71.4568799999999982, -71.4809000000000054 -71.4568799999999982))",
                    GeometryUtils::envelopeToPolygon(Envelope("-71.4809,42.4808,-71.45688,42.49368"))->toString());

    HOOT_STR_EQUALS("-71.4745000000000061,42.4838999999999984,-71.4619000000000000,42.4917999999999978",
                    GeometryUtils::polygonStringToLonLatString("-71.4745,42.4841;-71.4669,42.4918;-71.4619,42.4839;-71.4745,42.4841"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GeometryUtilsTest, "quick");

}

