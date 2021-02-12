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
 * @copyright Copyright (C) 2015, 2016, 2017 Maxar (http://www.maxar.com/)
 */

// Standard Includes
#include <float.h>
#include <stdio.h>

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include <tgs/RandomForest/DataFrame.h>
#include <tgs/FeatureSelection/ConsistencySubsetEvaluator.h>
#include <tgs/FeatureSelection/GreedyStepwiseSearch.h>
#include "../PluginFactory.h"
#include "GeneratedData.h"

#define ASSERT_DBL_EQL(expected, actual) CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, 1e-4)

using namespace std;

namespace Tgs
{
  class GreedyStepwiseSearchTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(GreedyStepwiseSearchTest);
    CPPUNIT_TEST(testGeneratedData20);
    CPPUNIT_TEST_SUITE_END();
  public:

    void testGeneratedData20()
    {
      DataFrame df;
      generatedData20(df);

      GreedyStepwiseSearch uut;
      boost::shared_ptr<ConsistencySubsetEvaluator> evaluator(new ConsistencySubsetEvaluator());
      uut.setEvaluator(evaluator);

      // this has been validated against weka
      vector<string> result = uut.findSubset(df);
      CPPUNIT_ASSERT_EQUAL((int)result.size(), 3);
      // order isn't important here, but easy
      CPPUNIT_ASSERT_EQUAL(result[0], string("age"));
      CPPUNIT_ASSERT_EQUAL(result[1], string("elevel"));
      CPPUNIT_ASSERT_EQUAL(result[2], string("car"));

      generatedData1000(df);
      // these results have been validated against weka
      result = uut.findSubset(df);
      CPPUNIT_ASSERT_EQUAL((int)result.size(), 5);
      // order isn't important here, but easy
      CPPUNIT_ASSERT_EQUAL(result[0], string("salary"));
      CPPUNIT_ASSERT_EQUAL(result[1], string("commission"));
      CPPUNIT_ASSERT_EQUAL(result[2], string("elevel"));
      CPPUNIT_ASSERT_EQUAL(result[3], string("car"));
      CPPUNIT_ASSERT_EQUAL(result[4], string("zipcode"));
    }
  };
}


// @todo Disabled. See r7251 for details.
// CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::GreedyStepwiseSearchTest, PluginFactory::testName());
