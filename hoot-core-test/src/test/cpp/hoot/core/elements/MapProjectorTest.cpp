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
 * @copyright Copyright (C) 2013, 2014, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// boost
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/geometry/GeometryUtils.h>

#include <hoot/core/elements/MapProjector.h>

// Tbs
#include <tbs/stats/SampleStats.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class MapProjectorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MapProjectorTest);
  CPPUNIT_TEST(runErrorTest);
  CPPUNIT_TEST(runCreatePlanarProjectionTest);
  CPPUNIT_TEST_SUITE_END();

public:

  boost::minstd_rand rng;
  ofstream fs;

  MapProjectorTest()
  {
    setResetType(ResetAllNoMatchFactory);
  }

  virtual void setUp()
  {
    HootTestFixture::setUp();
    // we are testing the map reprojector so we don't want to force to a single projection.
    conf().set(ConfigOptions::getTestForceOrthographicProjectionKey(), false);
  }

  virtual void tearDown()
  {
    conf().set(
      ConfigOptions::getTestForceOrthographicProjectionKey(),
      ConfigOptions::getTestForceOrthographicProjectionDefaultValue());
  }

  Radians calculateAngle(Coordinate p1, Coordinate p2, Coordinate p3)
  {
    Radians theta1 = atan2(p2.y - p1.y, p2.x - p1.x);
    Radians theta2 = atan2(p3.y - p1.y, p3.x - p1.x);

    return theta1 - theta2;
  }

  /**
   * Test a number of bounds for the best projections.
   */
  void runCreatePlanarProjectionTest()
  {
    if (GeometryUtils::highPrecisionHaversine() == false)
    {
      LOG_WARN("This platform does not support 128bit float so the Haversine calculation on "
               "this platform is only good to about 9cm.");
    }

    {
      OGREnvelope env;
      env.MinX = 30.0;
      env.MinY = 30;
      env.MaxX = 160;
      env.MaxY = 65;

      std::shared_ptr<OGRSpatialReference> srs = MapProjector::getInstance().
          createPlanarProjection(env, toRadians(2), 61);
      CPPUNIT_ASSERT_EQUAL(true,
        (bool)MapProjector::toWkt(srs).contains("Lambert_Conformal_Conic"));
    }

    {
      OGREnvelope env;
      env.MinX = 150.0;
      env.MinY = 70;
      env.MaxX = 160;
      env.MaxY = 71;

      std::shared_ptr<OGRSpatialReference> srs = MapProjector::getInstance().
        createPlanarProjection(env);
      CPPUNIT_ASSERT_EQUAL(true,
        (bool)MapProjector::toWkt(srs).contains("Two_Point_Equidistant"));
    }

    {
      OGREnvelope env;
      env.MinX = 130.0;
      env.MinY = 65;
      env.MaxX = 150;
      env.MaxY = 85;

      std::shared_ptr<OGRSpatialReference> srs = MapProjector::getInstance().
        createPlanarProjection(env);
      CPPUNIT_ASSERT_EQUAL(true,
        (bool)MapProjector::toWkt(srs).contains("Polyconic"));
    }

    {
      OGREnvelope env;
      env.MinX = 130;
      env.MinY = 65;
      env.MaxX = 130;
      env.MaxY = 65;

      std::shared_ptr<OGRSpatialReference> srs = MapProjector::getInstance().
        createPlanarProjection(env);
      CPPUNIT_ASSERT_EQUAL(true,
        (bool)MapProjector::toWkt(srs).contains("Orthographic"));
    }

    // This relates to an error we're seeing in #6898. I was trying to get a cleaner projection.
    {
      DisableLog dl(Log::Error);
      OGREnvelope env;
      env.MinX = -74;
      env.MinY = -35;
      env.MaxX = -30;
      env.MaxY = 6;

      std::shared_ptr<OGRSpatialReference> srs = MapProjector::getInstance().
        createPlanarProjection(env, toRadians(45), 10.0);
      CPPUNIT_ASSERT_EQUAL(true,
        (bool)MapProjector::toWkt(srs).contains("Lambert_Azimuthal_Equal_Area"));
    }
  }

  void runErrorTest()
  {
    {
      // don't print the warning or error messages.
      DisableLog dl(Log::Fatal);

      OGREnvelope env;
      env.MinX = -180.0;
      env.MinY = 0.0;
      env.MaxX = 180.0;
      env.MaxY = 89.5;
      std::shared_ptr<OGRSpatialReference> srs = MapProjector::createOrthographic(env);
      Coordinate c(500, 200);
      CPPUNIT_ASSERT_THROW(MapProjector::project(c, MapProjector::createWgs84Projection(), srs),
                           IllegalArgumentException);
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MapProjectorTest, "slow");

}
