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
 * @copyright Copyright (C) 2015, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Standard Includes
#include <float.h>
#include <iostream>
#include <stdio.h>
using namespace std;

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include <tgs/RandomForest/DataFrame.h>
#include <tgs/FeatureSelection/DataFrameDiscretizer.h>
#include <tgs/FeatureSelection/SymmetricUncertaintyCalculator.h>
#include "../PluginFactory.h"
#include "GeneratedData.h"

#define ASSERT_DBL_EQL(expected, actual) CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, 1e-2)

class DblVec : public std::vector<double>
{
public:
  DblVec(double f1, double f2, double f3, double f4)
  {
    resize(4);
    operator[](0) = f1;
    operator[](1) = f2;
    operator[](2) = f3;
    operator[](3) = f4;
  }
};

namespace Tgs
{
  class SymmetricUncertaintyCalculatorTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(SymmetricUncertaintyCalculatorTest);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST_SUITE_END();
  public:

    void test1()
    {
      DataFrame df;
      generatedData20(df);

      DataFrameDiscretizer dfd;
      dfd.discretize(df);

      SymmetricUncertaintyCalculator uut;
      vector<double> results;
      results.resize(9);
      results[0] = 0;
      results[1] = 0;
      results[2] = .619;
      results[3] = .123;
      results[4] = .231;
      results[5] = .21;
      results[6] = 0;
      results[7] = 0;
      results[8] = 0;
      for (unsigned int i = 0; i < df.getNumFactors(); i++)
      {
        double v = uut.calculateUncertainty(df, -1, df, i);
        // -1 maps to the training label
        ASSERT_DBL_EQL(results[i], v);
      }
    }
  };
}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::SymmetricUncertaintyCalculatorTest, PluginFactory::testName());
