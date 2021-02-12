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
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
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

#include <tgs/FeatureSelection/FayyadMdlDiscretizer.h>
#include "../PluginFactory.h"

#define ASSERT_DBL_EQL(expected, actual) CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, 1e-4)

namespace Tgs
{
  class FayyadMdlDiscretizerTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(FayyadMdlDiscretizerTest);
    CPPUNIT_TEST(testCalculateSplitPoints);
    CPPUNIT_TEST(testDelta);
    CPPUNIT_TEST(testEntropy);
    CPPUNIT_TEST(testFindBestSplitPoint);
    CPPUNIT_TEST_SUITE_END();
  public:

    void testCalculateSplitPoints()
    {
      // validation points below were verified in weka
      int c[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0 };
      vector<int> classes(c, c + 20);
      vector<double> values;
      for (unsigned int i = 0; i < classes.size(); i++)
      {
        values.push_back(i);
      }
      

      FayyadMdlDiscretizer uut;
      // call this just to set up internal data structures
      FayyadMdlDiscretizer::SplitPoints points = uut.calculateSplitPoints(classes, values);
      CPPUNIT_ASSERT_EQUAL(1, (int)points.size());
      CPPUNIT_ASSERT_EQUAL(7, points[0]);

      int c2[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
      std::vector<int> classes2(c2, c2 + 1000);
      vector<double> values2;
      for (unsigned int i = 0; i < classes2.size(); i++)
      {
        values2.push_back(i);
      }
      points = uut.calculateSplitPoints(classes2, values2);
      CPPUNIT_ASSERT_EQUAL(3, (int)points.size());
      CPPUNIT_ASSERT_EQUAL(85, points[0]);
      CPPUNIT_ASSERT_EQUAL(729, points[1]);
      CPPUNIT_ASSERT_EQUAL(848, points[2]);
    }

    void testDelta()
    {
      int c[] = { 1, 1, 0, 0, 1 };
      std::vector<int> classes(c, c + 5);
      vector<double> values;
      for (unsigned int i = 0; i < classes.size(); i++)
      {
        values.push_back(i);
      }

      FayyadMdlDiscretizer uut;
      // call this just to set up internal data structures
      uut.calculateSplitPoints(classes, values);

      // calculate it manually
      double k = 2, k1 = 1, k2 = 2;
      double ent = 0.970950594, entLeft = 0.0, entRight = 0.918295834;
      double delta = log(pow(3.0, k) - 2) / log(2.0) -
        (k * ent - k1 * entLeft - k2 * entRight);
      ASSERT_DBL_EQL(delta, uut._calculateDelta(0, 5, 1));
    }

    void testEntropy()
    {
      int c[] = { 1, 1, 0, 0, 1 };
      std::vector<int> classes(c, c + 5);
      vector<double> values;
      for (unsigned int i = 0; i < classes.size(); i++)
      {
        values.push_back(i);
      }

      FayyadMdlDiscretizer uut;
      // call this just to set up internal data structures
      uut.calculateSplitPoints(classes, values);

      ASSERT_DBL_EQL(0.970950594, uut._calculateEntropy(0, 5));
      ASSERT_DBL_EQL(0, uut._calculateEntropy(0, 2));
      ASSERT_DBL_EQL(0, uut._calculateEntropy(0, 0));
      ASSERT_DBL_EQL(0, uut._calculateEntropy(0, 1));
      ASSERT_DBL_EQL(1.0, uut._calculateEntropy(0, 4));
      ASSERT_DBL_EQL(1.0, uut._calculateEntropy(1, 4));
      ASSERT_DBL_EQL(0.918295834, uut._calculateEntropy(2, 3));
    }

    void testFindBestSplitPoint()
    {
      int c[] = { 1, 1, 0, 0, 1 };
      std::vector<int> classes(c, c + 5);
      vector<double> values;
      for (unsigned int i = 0; i < classes.size(); i++)
      {
        values.push_back(i);
      }

      FayyadMdlDiscretizer uut;
      // call this just to set up internal data structures
      uut.calculateSplitPoints(classes, values);

      double gain;
      ASSERT_DBL_EQL(1, uut._findBestSplitPoint(0, 5, gain));
      ASSERT_DBL_EQL(0.970950594 - (2.0 * 0.0 + 3.0 * 0.918295834) / 5.0, gain);
      
      ASSERT_DBL_EQL(0, uut._findBestSplitPoint(0, 2, gain));
      ASSERT_DBL_EQL(0.0, gain);
      
      ASSERT_DBL_EQL(1, uut._findBestSplitPoint(1, 2, gain));
      ASSERT_DBL_EQL(1.0, gain);
      
      ASSERT_DBL_EQL(3, uut._findBestSplitPoint(2, 3, gain));
      ASSERT_DBL_EQL(0.918295834, gain);
    }
  };
}


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::FayyadMdlDiscretizerTest, PluginFactory::testName());
