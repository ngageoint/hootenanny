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
#include <float.h>
#include <iostream>
#include <set>
#include <stdio.h>
using namespace std;

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include <tgs/RandomForest/DataFrame.h>
#include <tgs/FeatureSelection/DataFrameDiscretizer.h>
#include "../PluginFactory.h"

#define ASSERT_DBL_EQL(expected, actual) CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, 1e-4)

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
  class DataFrameDiscretizerTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(DataFrameDiscretizerTest);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST_SUITE_END();
  public:
    void populateSimpleDataFrame(DataFrame& df)
    {
      double n = DataFrame::null();

      // create a very simple data frame were the first factor is perfect and the others are 
      // garbage
      df.addDataVector("a", DblVec(0, n, 3, 1));
      df.addDataVector("a", DblVec(0, n, 2, 1));
      df.addDataVector("a", DblVec(n, 1, 1, 1));
      df.addDataVector("b", DblVec(n, n, 3, 2));
      df.addDataVector("b", DblVec(n, 2, 2, 2));
      df.addDataVector("b", DblVec(n, n, 1, n));
      std::vector<std::string> factorLabels;
      factorLabels.push_back("f1");
      factorLabels.push_back("f2");
      factorLabels.push_back("f3");
      factorLabels.push_back("f4");
      df.setFactorLabels(factorLabels);
    }

    void test1()
    {
      DataFrame df;
      populateSimpleDataFrame(df);
      df.setFactorType(0, DataFrame::Numerical);
      df.setNullTreatment(0, DataFrame::NullAsValue);
      df.setFactorType(1, DataFrame::Numerical);
      df.setNullTreatment(1, DataFrame::NullAsMissingValue);
      df.setFactorType(3, DataFrame::Numerical);
      df.setNullTreatment(3, DataFrame::NullAsMissingValue);

      DataFrameDiscretizer uut;
      uut.discretize(df);
      CPPUNIT_ASSERT_EQUAL((int)DataFrame::Nominal, df.getFactorTypes()[0]);
      CPPUNIT_ASSERT_EQUAL((int)DataFrame::NullAsValue, df.getNullTreatment(0));
      ASSERT_DBL_EQL(1.0, df.getDataElement(0, 0));
      ASSERT_DBL_EQL(1.0, df.getDataElement(1, 0));
      ASSERT_DBL_EQL(0.0, df.getDataElement(2, 0));
      ASSERT_DBL_EQL(0.0, df.getDataElement(3, 0));
      ASSERT_DBL_EQL(0.0, df.getDataElement(4, 0));
      ASSERT_DBL_EQL(0.0, df.getDataElement(5, 0));

      CPPUNIT_ASSERT_EQUAL((int)DataFrame::Nominal, df.getFactorTypes()[1]);
      CPPUNIT_ASSERT_EQUAL((int)DataFrame::NullAsMissingValue, df.getNullTreatment(1));
      ASSERT_DBL_EQL(1.0, df.getDataElement(0, 1));
      ASSERT_DBL_EQL(1.0, df.getDataElement(1, 1));
      ASSERT_DBL_EQL(1.0, df.getDataElement(2, 1));
      ASSERT_DBL_EQL(2.0, df.getDataElement(3, 1));
      ASSERT_DBL_EQL(2.0, df.getDataElement(4, 1));
      ASSERT_DBL_EQL(2.0, df.getDataElement(5, 1));

      CPPUNIT_ASSERT_EQUAL((int)DataFrame::Nominal, df.getFactorTypes()[2]);
      CPPUNIT_ASSERT_EQUAL((int)DataFrame::NullAsMissingValue, df.getNullTreatment(2));
      ASSERT_DBL_EQL(1.0, df.getDataElement(0, 2));
      ASSERT_DBL_EQL(1.0, df.getDataElement(1, 2));
      ASSERT_DBL_EQL(1.0, df.getDataElement(2, 2));
      ASSERT_DBL_EQL(1.0, df.getDataElement(3, 2));
      ASSERT_DBL_EQL(1.0, df.getDataElement(4, 2));
      ASSERT_DBL_EQL(1.0, df.getDataElement(5, 2));

      CPPUNIT_ASSERT_EQUAL((int)DataFrame::Nominal, df.getFactorTypes()[3]);
      CPPUNIT_ASSERT_EQUAL((int)DataFrame::NullAsMissingValue, df.getNullTreatment(3));
      ASSERT_DBL_EQL(1.0, df.getDataElement(0, 3));
      ASSERT_DBL_EQL(1.0, df.getDataElement(1, 3));
      ASSERT_DBL_EQL(1.0, df.getDataElement(2, 3));
      ASSERT_DBL_EQL(2.0, df.getDataElement(3, 3));
      ASSERT_DBL_EQL(2.0, df.getDataElement(4, 3));
      ASSERT_DBL_EQL(2.0, df.getDataElement(5, 3));
    }
  };
}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::DataFrameDiscretizerTest, PluginFactory::testName());
