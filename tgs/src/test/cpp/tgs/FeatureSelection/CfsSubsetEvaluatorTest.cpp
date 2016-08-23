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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
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

#include <tgs/StreamUtils.h>
#include <tgs/RandomForest/DataFrame.h>
#include <tgs/FeatureSelection/CfsSubsetEvaluator.h>
#include <tgs/FeatureSelection/GreedyStepwiseSearch.h>
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
  class CfsSubsetEvaluatorTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(CfsSubsetEvaluatorTest);
    CPPUNIT_TEST(sanityCheck);
    CPPUNIT_TEST(testGeneratedData20);
    CPPUNIT_TEST(testGeneratedData1000);
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

      CfsSubsetEvaluator uut;
      uut.setDataFrame(df);
      // these values seem reasonable, but have not been manually validated. If there is a 
      // discrepancy at some time these should be manually verified.
      ASSERT_DBL_EQL(.447214, uut.evaluateSubset(IntVec(0, 1, 2)));
      ASSERT_DBL_EQL(.707107, uut.evaluateSubset(IntVec(0, 1)));
      ASSERT_DBL_EQL(1, uut.evaluateSubset(IntVec(0)));
    }
    
    void testGeneratedData20()
    {
      DataFrame df;
      generatedData20(df);

      GreedyStepwiseSearch uut;
      shared_ptr<CfsSubsetEvaluator> evaluator(new CfsSubsetEvaluator());
      uut.setEvaluator(evaluator);

      // this has been validated against weka
      vector<string> result = uut.findSubset(df);
      CPPUNIT_ASSERT_EQUAL((int)result.size(), 1);
      // This is slightly different from the Weka output. Weka lists both age and car, but also
      // says the highest score is .619, the score for age alone, not the score for car and age.
      // I wonder if they have some extra logic in there apart from what is in the paper. By
      // removing car from the list Weka puts in the next highest correlated variable that is not 
      // zero. It may be a bug in my code, but I can't find it. It is reassuring that the 1000 
      // test is spot on.
      CPPUNIT_ASSERT_EQUAL(result[0], string("age"));
    }

    void testGeneratedData1000()
    {
      DataFrame df;
      generatedData1000(df);

      GreedyStepwiseSearch uut;
      shared_ptr<CfsSubsetEvaluator> evaluator(new CfsSubsetEvaluator());
      uut.setEvaluator(evaluator);

      // this has been validated against weka
      vector<string> result = uut.findSubset(df);
//       for (unsigned int i = 0; i < result.size(); i++)
//       {
//         cout << result[i] << endl;
//       }
      CPPUNIT_ASSERT_EQUAL((int)result.size(), 1);
      // order isn't important here, but easy
      CPPUNIT_ASSERT_EQUAL(result[0], string("salary"));
    }
  };
}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::CfsSubsetEvaluatorTest, PluginFactory::testName());
