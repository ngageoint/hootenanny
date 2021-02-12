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

#include <tgs/SpinImage/SpinImage.h>
#include <tgs/SpinImage/SpinImageGenerator.h>
#include <tgs/SpinImage/SpinImageStack.h>
#include <tgs/SpinImage/Points.h>

#include "../PluginFactory.h"

#define ASSERT_DBL_EQL(expected, actual) CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, 1e-4)

namespace Tgs
{
  class SpinImageTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(SpinImageTest);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST(testCompare);
    CPPUNIT_TEST_SUITE_END();
  public:

    void test1()
    {
      SpinImage uut;
      uut.resize(10, 10);
      uut.setBounds(10, 0, 10);

      ASSERT_DBL_EQL(0.5, uut._calculateAlphaCenter(0));
      ASSERT_DBL_EQL(1.5, uut._calculateAlphaCenter(1));
      ASSERT_DBL_EQL(7.5, uut._calculateAlphaCenter(7));

      ASSERT_DBL_EQL(0.5, uut._calculateBetaCenter(0));
      ASSERT_DBL_EQL(1.5, uut._calculateBetaCenter(1));
      ASSERT_DBL_EQL(7.5, uut._calculateBetaCenter(7));

      ASSERT_DBL_EQL(0, uut._calculateLowerAlpha(0));
      ASSERT_DBL_EQL(0, uut._calculateLowerAlpha(0.5));
      ASSERT_DBL_EQL(0, uut._calculateLowerAlpha(1.0));
      ASSERT_DBL_EQL(0, uut._calculateLowerAlpha(1.49));
      ASSERT_DBL_EQL(1, uut._calculateLowerAlpha(1.51));
      ASSERT_DBL_EQL(6, uut._calculateLowerAlpha(7.0));
      ASSERT_DBL_EQL(7, uut._calculateLowerAlpha(7.51));

      ASSERT_DBL_EQL(0, uut._calculateLowerBeta(0));
      ASSERT_DBL_EQL(0, uut._calculateLowerBeta(0.5));
      ASSERT_DBL_EQL(0, uut._calculateLowerBeta(1.0));
      ASSERT_DBL_EQL(0, uut._calculateLowerBeta(1.49));
      ASSERT_DBL_EQL(1, uut._calculateLowerBeta(1.51));
      ASSERT_DBL_EQL(6, uut._calculateLowerBeta(7.0));
      ASSERT_DBL_EQL(7, uut._calculateLowerBeta(7.51));

      ASSERT_DBL_EQL(0, uut._calculateIndex(0.5, 0.5));
      ASSERT_DBL_EQL(0, uut._calculateIndex(.99, 0.5));
      ASSERT_DBL_EQL(1, uut._calculateIndex(1.01, 0.5));
      ASSERT_DBL_EQL(3, uut._calculateIndex(3.5, 0.5));
      ASSERT_DBL_EQL(13, uut._calculateIndex(3.5, 1.99));
      ASSERT_DBL_EQL(23, uut._calculateIndex(3.5, 2.01));

      uut.addValue(0.50, 0.50);
      uut.addValue(0.501, 0.501);
      uut.addValue(3.5, 3.5);
      uut.addValue(3.501, 3.501);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, uut.getValue(0.5, 0.5), .01);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, uut.getValue(3.5, 3.5), .01);

      uut.addValue(6, 3);

      ASSERT_DBL_EQL(0.25, uut.getValue(5.5, 2.5));
      ASSERT_DBL_EQL(0.25, uut.getValue(5.5, 3.5));
      ASSERT_DBL_EQL(0.25, uut.getValue(6.5, 2.5));
      ASSERT_DBL_EQL(0.25, uut.getValue(6.5, 3.5));

//       const SpinImage* image = &uut;
//       for (int b = 0; b < image->getBetaBins(); b++)
//       {
//         printf("\n");
//         for (int a = 0; a < image->getAlphaBins(); a++)
//         {
//           float v = image->bin(a, b);
//           printf("%.1f\t", v);
//         }
//       }
//       printf("\n");
//       printf("\n");
    }

    void testCompare()
    {
      SpinImage i1;
      SpinImage i2;
      i1.resize(2, 5);
      i2.resize(2, 5);
      for (unsigned int i = 0; i < 10; i++)
      {
        if (i == 0)
        {
          i1._bins[i] = 100.0f;
          i2._bins[i] = 0.0f;
        }
        else
        {
          i1._bins[i] = (float)(i);
          i2._bins[i] = (float)(i) * 2.0f;
        }
      }
      int n;
      CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, i2._calculateCorrelation(i1, n), 1e-6);
      CPPUNIT_ASSERT_EQUAL(9, n);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(310.336, i2.compare(i1), 1e-2);

      // test case create in excel w/ the correl function
      float x[] = { 2, 10, 5, 4, 8, 3, 7, 6, 9, 7 };
      float y[] = { 5, 1, 1, 8, 3, 9, 3, 10, 1, 8 };
      for (unsigned int i = 0; i < 10; i++)
      {
        i1._bins[i] = x[i];
        i2._bins[i] = y[i];
      }
      CPPUNIT_ASSERT_DOUBLES_EQUAL(-.536757, i1._calculateCorrelation(i2, n), 1e-4);
      CPPUNIT_ASSERT_EQUAL(10, n);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(.359507, i1.compare(i2), 1e-5);
    }
  };
}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::SpinImageTest, PluginFactory::testName());
