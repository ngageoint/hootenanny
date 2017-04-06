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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// boost
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/algorithms/WayHeading.h>
using namespace hoot;

// Tbs
#include <tbs/stats/SampleStats.h>

#include "TestUtils.h"

namespace hoot
{

class MapProjectorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MapProjectorTest);
  CPPUNIT_TEST(runErrorTest);
  CPPUNIT_TEST(runCreatePlanarProjectionTest);
  CPPUNIT_TEST_SUITE_END();

public:

  boost::minstd_rand rng;
  ofstream fs;

  void setUp()
  {
    TestUtils::resetEnvironment();
    // we are testing the map reprojector so we don't want to force to a single projection.
    conf().set(ConfigOptions::getTestForceOrthographicProjectionKey(), false);
  }

  void tearDown()
  {
    TestUtils::resetEnvironment();
  }


  Radians calculateAngle(Coordinate p1, Coordinate p2, Coordinate p3)
  {
    Radians theta1 = atan2(p2.y - p1.y, p2.x - p1.x);
    Radians theta2 = atan2(p3.y - p1.y, p3.x - p1.x);

    return theta1 - theta2;
  }

  void evaluateProjection(const OGREnvelope& env, boost::shared_ptr<OGRSpatialReference> srs)
  {
    boost::shared_ptr<OGRSpatialReference> wgs84 = MapProjector::createWgs84Projection();

    OGRCoordinateTransformation* t(OGRCreateCoordinateTransformation(wgs84.get(), srs.get()));

    boost::shared_ptr<OsmMap> map(new OsmMap());

    if (t == 0)
    {
      throw HootException(QString("Error creating transformation object: ") + CPLGetLastErrorMsg());
    }

    Meters distance = 1000.0;
    double width = env.MaxX - env.MinX;
    double height = env.MaxY - env.MinY;
    double stepSize = min(width, height) / 20.0;
    int stepsX = (width) / stepSize;
    int stepsY = (height) / stepSize;
    double stepSizeX = (width) / (double)stepsX;
    double stepSizeY = (height) / (double)stepsY;
    auto_ptr<geos::geom::Envelope> e(GeometryUtils::toEnvelope(env));

    vector<double> errors;
    vector<double> deltaErrors;
    bool success = true;

    for (double x = env.MinX; x <= env.MaxX; x += stepSizeX)
    {
      for (double y = env.MinY; y <= env.MaxY; y += stepSizeY)
      {
        Coordinate c1(x, y);
        Coordinate p1 = c1;
        success &= t->Transform(1, &p1.x, &p1.y);
        boost::shared_ptr<Node> n1(new Node(Status::Unknown1, map->createNextNodeId(), c1, 10));
        map->addNode(n1);

        Coordinate upc = GeometryUtils::calculateDestination(c1, 0.0, distance);
        Coordinate up = upc;
        success &= t->Transform(1, &up.x, &up.y);

        for (double bearing = 0.0; bearing < 360.0; bearing += 20.0)
        {
          Coordinate c2 = GeometryUtils::calculateDestination(c1, bearing, distance);
          Coordinate p2 = c2;
          success &= t->Transform(1, &p2.x, &p2.y);

          if (e->contains(c2))
          {
            boost::shared_ptr<Node> n2(new Node(Status::Unknown1, map->createNextNodeId(), c2, 10));
            map->addNode(n2);
            boost::shared_ptr<Way> w(new Way(Status::Unknown1, map->createNextWayId(), 10));
            map->addWay(w);
            w->addNode(n1->getId());
            w->addNode(n2->getId());

            Meters d = p1.distance(p2);
            //Meters d = up.distance(p1);
            errors.push_back(fabs(d - distance));

            w->setTag("d", QString::number(d, 'g', 15));

            if (d > 3000)
            {
              LOG_INFO("c1: " << c1.toString() << " c2: " << c2.toString());
              LOG_INFO("p1: " << p1.toString() << " p2: " << p2.toString());
              LOG_INFO("d: " << d);
            }

            if (e->contains(upc))
            {
              Radians theta = calculateAngle(p1, up, p2);
              Degrees delta = toDegrees(WayHeading::deltaMagnitude(toRadians(bearing), theta));
              deltaErrors.push_back(delta);
              w->setTag("delta", QString::number(delta));
            }
          }
        }
      }
    }

    OsmXmlWriter writer;
    writer.write(map, "tmp/ErrorMap.osm");

    tbs::SampleStats statsError(errors);
    tbs::SampleStats statsDeltaError(deltaErrors);

    if (success)
    {
      fs << "\t" << statsError.calculateMin() + 1e-6
        << "\t" << statsError.calculateQ1() + 1e-6
        << "\t" << statsError.calculateMedian() + 1e-6
        << "\t" << statsError.calculateQ3() + 1e-6
        << "\t" << statsError.calculateMax() + 1e-6
        << "\t" << statsDeltaError.calculateMin() + 1e-6
        << "\t" << statsDeltaError.calculateQ1() + 1e-6
        << "\t" << statsDeltaError.calculateMedian() + 1e-6
        << "\t" << statsDeltaError.calculateQ3() + 1e-6
        << "\t" << statsDeltaError.calculateMax() + 1e-6
        << endl;
    }
    else
    {
      fs << endl;
    }

  }


  void testRegion(const OGREnvelope& env, QString name)
  {
    boost::shared_ptr<OGRSpatialReference> sinusoidal = MapProjector::createSinusoidalProjection(env);
    boost::shared_ptr<OGRSpatialReference> mollweide(new OGRSpatialReference());
    if (mollweide->importFromEPSG(54009) != OGRERR_NONE)
    {
      throw HootException("Error creating mollweide projection.");
    }
    boost::shared_ptr<OGRSpatialReference> orthographic = MapProjector::createOrthographic(env);

    boost::shared_ptr<OGRSpatialReference> eckertVI(new OGRSpatialReference());
    if (eckertVI->importFromEPSG(53010) != OGRERR_NONE)
    {
      throw HootException("Error creating mollweide projection.");
    }

    boost::shared_ptr<OGRSpatialReference> sphereBoone(new OGRSpatialReference());
    if (sphereBoone->importFromEPSG(53024) != OGRERR_NONE)
    {
      throw HootException("Error creating mollweide projection.");
    }

    static double o = -0.1;
    o += 0.1;

    QString fn = ("tmp/" + name + ".dat");
    fs.exceptions(ios_base::failbit | ios_base::badbit);
    fs.open(fn.toUtf8().data(), ios_base::out | ios_base::trunc);
    fs << 1 + o << "\tSinusoidal";
    evaluateProjection(env, sinusoidal);
//    fs << 2 + o << "\tMollweide";
//    evaluateProjection(env, mollweide);
//    fs << 3 + o << "\t\"Eckert VI\"";
//    evaluateProjection(env, eckertVI);
//    fs << 4 + o << "\t\"Sphere Boone\"";
//    evaluateProjection(env, sphereBoone);
//    fs << 5 + o << "\tAEAC";
//    if ((env.MinY >= 0 && env.MaxY >= 0) || (env.MinY <= 0 && env.MaxY <= 0))
//    {
//      boost::shared_ptr<OGRSpatialReference> aeac = MapProjector::createAeacProjection(env);
//      evaluateProjection(env, aeac);
//    }
//    else
//    {
//      fs << endl;
//    }
//    fs << 6 + o << "\tOrthographic";
//    evaluateProjection(env, orthographic);
//    fs.close();
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

      boost::shared_ptr<OGRSpatialReference> srs = MapProjector::getInstance().
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

      boost::shared_ptr<OGRSpatialReference> srs = MapProjector::getInstance().
        createPlanarProjection(env);
      CPPUNIT_ASSERT_EQUAL(true,
        (bool)MapProjector::toWkt(srs).contains("Orthographic"));
    }

    {
      OGREnvelope env;
      env.MinX = 130.0;
      env.MinY = 65;
      env.MaxX = 150;
      env.MaxY = 85;

      boost::shared_ptr<OGRSpatialReference> srs = MapProjector::getInstance().
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

      boost::shared_ptr<OGRSpatialReference> srs = MapProjector::getInstance().
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

      boost::shared_ptr<OGRSpatialReference> srs = MapProjector::getInstance().
        createPlanarProjection(env, toRadians(45), 10.0);
      CPPUNIT_ASSERT_EQUAL(true,
        (bool)MapProjector::toWkt(srs).contains("Lambert_Azimuthal_Equal_Area"));
    }
  }

  void runErrorTest()
  {
//    {
//      OGREnvelope env;
//      env.MinX = 0.0;
//      env.MinY = 80.0;
//      env.MaxX = 1.0;
//      env.MaxY = 81.0;

//      auto_ptr<geos::geom::Envelope> e(GeometryUtils::toEnvelope(env));
//      LOG_INFO("Envelope:" << e->toString())
//      testRegion(env, "SmallNorth");
//    }
//    {
//      OGREnvelope env;
//      env.MinX = -180.0;
//      env.MinY = 0.0;
//      env.MaxX = 180.0;
//      env.MaxY = 89.5;

//      auto_ptr<geos::geom::Envelope> e(GeometryUtils::toEnvelope(env));
//      LOG_INFO("Envelope:" << e->toString())
//      testRegion(env, "BigNorth");
//    }

    {
      // don't print the warning or error messages.
      DisableLog dl(Log::Fatal);

      OGREnvelope env;
      env.MinX = -180.0;
      env.MinY = 0.0;
      env.MaxX = 180.0;
      env.MaxY = 89.5;
      boost::shared_ptr<OGRSpatialReference> srs = MapProjector::createOrthographic(env);
      Coordinate c(500, 200);
      CPPUNIT_ASSERT_THROW(MapProjector::project(c, MapProjector::createWgs84Projection(), srs),
                           IllegalArgumentException);
    }

//    {
//      OGREnvelope env;
//      env.MinX = -180.0;
//      env.MinY = 0.0;
//      env.MaxX = 180.0;
//      env.MaxY = 65.0;

//      auto_ptr<geos::geom::Envelope> e(GeometryUtils::toEnvelope(env));
//      LOG_INFO("Envelope:" << e->toString())
//      testRegion(env, "ReasonableNorth");
//    }

//    {
//      OGREnvelope env;
//      env.MinX = -180.0;
//      env.MinY = -89.5;
//      env.MaxX = 180.0;
//      env.MaxY = 89.5;

//      auto_ptr<geos::geom::Envelope> e(GeometryUtils::toEnvelope(env));
//      LOG_INFO("Envelope:" << e->toString())
//      testRegion(env, "AlmostGlobe");
//    }

//    {
//      OGREnvelope env;
//      env.MinX = -180.0;
//      env.MinY = -65;
//      env.MaxX = 180.0;
//      env.MaxY = 65;

//      auto_ptr<geos::geom::Envelope> e(GeometryUtils::toEnvelope(env));
//      LOG_INFO("Envelope:" << e->toString())
//      testRegion(env, "ReasonableGlobe");
//    }

//    {
//      OGREnvelope env;
//      env.MinX = 0.0;
//      env.MinY = 0;
//      env.MaxX = 1.0;
//      env.MaxY = 1;

//      auto_ptr<geos::geom::Envelope> e(GeometryUtils::toEnvelope(env));
//      LOG_INFO("Envelope:" << e->toString())
//      testRegion(env, "SmallMiddle");
//    }

//    {
//      OGREnvelope env;
//      env.MinX = -5.0;
//      env.MinY = 0;
//      env.MaxX = 5.0;
//      env.MaxY = 10;

//      auto_ptr<geos::geom::Envelope> e(GeometryUtils::toEnvelope(env));
//      LOG_INFO("Envelope:" << e->toString())
//      testRegion(env, "MediumMiddle");
//    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MapProjectorTest, "slow");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MapProjectorTest, "current");

}
