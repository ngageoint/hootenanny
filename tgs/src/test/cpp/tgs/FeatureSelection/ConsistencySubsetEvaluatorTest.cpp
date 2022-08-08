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
 * @copyright Copyright (C) 2015, 2016, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Standard Includes
#include <float.h>
#include <stdio.h>

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt Includes
#include <QTime>

#include <tgs/RandomForest/DataFrame.h>
#include <tgs/FeatureSelection/ConsistencySubsetEvaluator.h>
#include "../PluginFactory.h"
#include "GeneratedData.h"

class DblVec : public std::vector<double>
{
public:
  DblVec(double f1, double f2, double f3)
  {
    resize(3);
    operator[](0) = f1;
    operator[](1) = f2;
    operator[](2) = f3;
  }
};

class IntVec : public std::vector<int>
{
public:
  IntVec(int f1)
  {
    resize(1);
    operator[](0) = f1;
  }

  IntVec(int f1, int f2)
  {
    resize(2);
    operator[](0) = f1;
    operator[](1) = f2;
  }

  IntVec(int f1, int f2, int f3)
  {
    resize(3);
    operator[](0) = f1;
    operator[](1) = f2;
    operator[](2) = f3;
  }
};

#define ASSERT_DBL_EQL(expected, actual) CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, 1e-4)

namespace Tgs
{
  class ConsistencySubsetEvaluatorTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(ConsistencySubsetEvaluatorTest);
    CPPUNIT_TEST(sanityCheck);
    CPPUNIT_TEST(testGeneratedData20);
    //CPPUNIT_TEST(testGeneratedData1000);
    CPPUNIT_TEST_SUITE_END();
  public:
    void populateSimpleDataFrame(DataFrame& df)
    {
      // create a very simple data frame were the first factor is perfect and the others are 
      // garbage
      df.addDataVector("a", DblVec(0, 1, 3));
      df.addDataVector("a", DblVec(0, 2, 2));
      df.addDataVector("a", DblVec(0, 3, 1));
      df.addDataVector("b", DblVec(1, 1, 3));
      df.addDataVector("b", DblVec(1, 2, 2));
      df.addDataVector("b", DblVec(1, 3, 1));
      std::vector<std::string> factorLabels;
      factorLabels.push_back("f1");
      factorLabels.push_back("f2");
      factorLabels.push_back("f3");
      df.setFactorLabels(factorLabels);
    }

    void sanityCheck()
    {
      DataFrame df;
      populateSimpleDataFrame(df);

      ConsistencySubsetEvaluator uut;
      uut.setDataFrame(df);
      ASSERT_DBL_EQL(0, uut.evaluateSubset(IntVec(0, 1, 2)));
      ASSERT_DBL_EQL(0, uut.evaluateSubset(IntVec(0, 1)));
      ASSERT_DBL_EQL(0, uut.evaluateSubset(IntVec(0, 2)));
      ASSERT_DBL_EQL(-.5, uut.evaluateSubset(IntVec(1)));
      ASSERT_DBL_EQL(-.5, uut.evaluateSubset(IntVec(2)));
      ASSERT_DBL_EQL(-.5, uut.evaluateSubset(IntVec(1, 2)));
    }

    void testGeneratedData20()
    {
      DataFrame df;
      generatedData20(df);

      ConsistencySubsetEvaluator uut;
      uut.setDataFrame(df);
      int factorsArr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
      std::vector<int> factors(factorsArr, factorsArr + 9);

      // the important part is that these two lines give the same and lowest value.
      ASSERT_DBL_EQL(0, uut.evaluateSubset(factors));
      ASSERT_DBL_EQL(0, uut.evaluateSubset(IntVec(2, 3, 4)));

      ASSERT_DBL_EQL(-.1, uut.evaluateSubset(IntVec(0, 1, 2)));
      ASSERT_DBL_EQL(-.1, uut.evaluateSubset(IntVec(0, 2)));
      ASSERT_DBL_EQL(-.5, uut.evaluateSubset(IntVec(0, 1, 6)));
      ASSERT_DBL_EQL(-.5, uut.evaluateSubset(IntVec(6, 7, 8)));
    }

    // disabled cause the results weren't verified and it was only being used as a benchmark
//     void testGeneratedData1000()
//     {
//       DataFrame df;
//       generatedData1000(df);
// 
//       QTime t;
//       t.start();
//       ConsistencySubsetEvaluator uut;
//       uut.setDataFrame(df);
//       int factorsArr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
//       std::vector<int> factors(factorsArr, factorsArr + 9);
// 
//       qDebug("testGeneratedData1000 %dms", t.restart());
//       // the important part is that these two lines give the same and lowest value.
//       for (int i = 0; i < 10; i++) uut.evaluateSubset(factors);
// 
//       qDebug("testGeneratedData1000 %dms", t.elapsed());
//       ASSERT_DBL_EQL(0, uut.evaluateSubset(IntVec(2, 3, 4)));
// 
//       ASSERT_DBL_EQL(-.1, uut.evaluateSubset(IntVec(0, 1, 2)));
//       ASSERT_DBL_EQL(-.1, uut.evaluateSubset(IntVec(0, 2)));
//       ASSERT_DBL_EQL(-.5, uut.evaluateSubset(IntVec(0, 1, 6)));
//       ASSERT_DBL_EQL(-.5, uut.evaluateSubset(IntVec(6, 7, 8)));
//     }
  };
}

// @todo Disabled. See r7251 for details.
// CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::ConsistencySubsetEvaluatorTest, PluginFactory::testName());
