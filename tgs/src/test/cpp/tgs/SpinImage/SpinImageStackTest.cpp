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
 * @copyright Copyright (C) 2015, 2021 Maxar (http://www.maxar.com/)
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
#include <tgs/SpinImage/SpinImage.h>
#include <tgs/SpinImage/SpinImageGenerator.h>
#include <tgs/SpinImage/SpinImageStack.h>
#include <tgs/SpinImage/Points.h>

#include "../PluginFactory.h"

#define ASSERT_DBL_EQL(expected, actual) CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, 1e-4)

namespace Tgs
{
  class SpinImageStackTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(SpinImageStackTest);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST_SUITE_END();
  public:

    void test1()
    {
      SpinImageStack uut;
      SpinImageGenerator gen;
      gen.setSize(2, 2);
      gen.setKThreshold(5);

      PointCloud pc;
      pc.addPoint(CloudPoint(-5, 0, 0));
      pc.addPoint(CloudPoint(0, -5, 0));
      pc.addPoint(CloudPoint(0, 0, -5));
      pc.addPoint(CloudPoint(5, 0, 0));
      pc.addPoint(CloudPoint(0, 5, 0));
      pc.addPoint(CloudPoint(0, 0, 5));

      NormalEstimator ne;
      ne.calculateNormals(pc);
      gen.generateStack(pc, uut);

      // sanity check
      CPPUNIT_ASSERT_EQUAL(uut, uut);

      stringstream strm;
      strm << uut;

      strm.seekg(0);
      SpinImageStack copy;
      copy.importStack(strm);
      CPPUNIT_ASSERT_EQUAL(uut, copy);
    }
  };
}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::SpinImageStackTest, PluginFactory::testName());
