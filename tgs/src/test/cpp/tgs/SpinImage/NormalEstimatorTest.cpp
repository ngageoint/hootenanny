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
 * @copyright Copyright (C) 2015, 2016 Maxar (http://www.maxar.com/)
 */

// Standard Includes
#include <iostream>
#include <math.h>
using namespace std;

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include <tgs/SpinImage/NormalEstimator.h>
#include <tgs/SpinImage/Points.h>
#include <tgs/Statistics/Random.h>

#include "../PluginFactory.h"

namespace Tgs
{
  class NormalEstimatorTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(NormalEstimatorTest);
    CPPUNIT_TEST(testTangentPlane);
    CPPUNIT_TEST(testNormal);
    CPPUNIT_TEST(testCircle);
    CPPUNIT_TEST_SUITE_END();
  public:

    void testTangentPlane()
    {
      PointCloud pc;
      pc.addPoint(CloudPoint(0, 0, 0));
      pc.addPoint(CloudPoint(2, 0, 0));
      pc.addPoint(CloudPoint(0, 1, 0));
      pc.addPoint(CloudPoint(2, 1, 0));
      pc.addPoint(CloudPoint(-2, 0, 0));
      pc.addPoint(CloudPoint(0, -1, 0));
      pc.addPoint(CloudPoint(-2, -1, 0));

      NormalEstimator uut;

      uut.calculateNormals(pc);
      Point3d normal = uut._calculateTangentPlane(0);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(0, normal.p1, 1e-6);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(0, normal.p2, 1e-6);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(1, normal.p3, 1e-6);

      pc.clear();
      pc.addPoint(CloudPoint(0, 0, 0));
      pc.addPoint(CloudPoint(2, 1, 1));
      pc.addPoint(CloudPoint(2, -1, 1));
      pc.addPoint(CloudPoint(-2, 1, -1));
      pc.addPoint(CloudPoint(-2, -1, -1));

      uut.calculateNormals(pc);
      normal = uut._calculateTangentPlane(0);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(-.447214, normal.p1, 1e-6);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(0, normal.p2, 1e-6);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(.894427, normal.p3, 1e-6);
    }

    void testNormal()
    {
      PointCloud pc;
      pc.addPoint(CloudPoint(0, 0, 1));
      pc.addPoint(CloudPoint(3, 0, 0));
      pc.addPoint(CloudPoint(2, 1, 0));
      pc.addPoint(CloudPoint(2, -1, 0));
      pc.addPoint(CloudPoint(-2, 1, 0));
      pc.addPoint(CloudPoint(-2, -1, 0));

      NormalEstimator uut;
      uut.calculateNormals(pc);
      for (unsigned int i = 0; i < pc.getNormals().size(); i++)
      {
        // make sure they're all pointing mostly up
        CPPUNIT_ASSERT(pc.getNormals()[i].p3 > .95);
      }
    }

    void testCircle()
    {
      int testSize = 200;
      PointCloud pc;
      // create a sphere of points, almost uniformly distributed
      for (int i = 0; i < testSize; i++)
      {
        Point3d p(Tgs::Random::instance()->generateInt() - RAND_MAX / 2.0,
                  Tgs::Random::instance()->generateInt() - RAND_MAX / 2.0,
                  Tgs::Random::instance()->generateInt() - RAND_MAX / 2.0);
        p.normalize();
        p = p * 10.0;
        pc.addPoint(CloudPoint(p.p1, p.p2, p.p3));
      }

      NormalEstimator uut;
      uut.calculateNormals(pc);
      for (unsigned int i = 0; i < pc.getNormals().size(); i++)
      {
        Point3d p = pc.getPoints()[i].toPoint();
        const Point3d& normal = pc.getNormals()[i];
        p.normalize();
        double dp = fabs(dotProduct(p, normal));
        if (dp < .95)
        {
          cout << i << " dp:\t" << dp << "\tactual: " << p << "\t estimate: " << normal << endl;
        }
        CPPUNIT_ASSERT(dp >= .95);
      }
    }
  };
}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::NormalEstimatorTest, PluginFactory::testName());
