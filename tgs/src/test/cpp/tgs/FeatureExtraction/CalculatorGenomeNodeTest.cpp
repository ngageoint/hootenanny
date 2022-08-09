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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// Standard Includes
#include <float.h>
#include <iostream>
#include <limits>
#include <stdio.h>
using namespace std;

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Tgs
#include <tgs/StreamUtils.h>
#include <tgs/FeatureExtraction/BasicMathCalculatorNode.h>
#include <tgs/FeatureExtraction/CalculatorGenome.h>
#include <tgs/FeatureExtraction/ConstantCalculatorNodeSource.h>
#include <tgs/FeatureExtraction/FitnessFunction.h>
#include <tgs/FeatureExtraction/VectorCalculatorNodeSource.h>
#include <tgs/Statistics/Random.h>

#include "../PluginFactory.h"

#define ASSERT_DBL_EQL(expected, actual) CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, 1e-4)

namespace Tgs
{

  /**
   * Optimizes for: ((3 * v) + sqrt(v)) - 2
   */
  class SimpleFitness : public FitnessFunction
  {
  public:
    SimpleFitness(const vector<double>& v) : _values(v) { }

    virtual double calculateFitness(const Genome& genome)
    {
      const CalculatorGenome& cg = dynamic_cast<const CalculatorGenome&>(genome);
      double result = 0.0;
      // calculate mean squared error
      for (unsigned int i = 0; i < _values.size(); i++)
      {
        double vt = cg.calculateValue(i);
        double truth = ((3 * _values[i]) + sqrt(_values[i])) - 2;
        result += fabs(vt - truth) * fabs(vt - truth);
      }
      result /= (double)_values.size();
      // if there is no error, the score is infinately high
      if (result == 0.0)
      {
        result = numeric_limits<double>::infinity();
      }
      // otherwise, invert the score to make higher errors smaller scores
      else
      {
        result = 1.0 / result;
      }
      return result;
    }

  private:
    vector<double> _values;
  };

  class CalculatorGenomeNodeTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(CalculatorGenomeNodeTest);
    CPPUNIT_TEST(simpleTest);
    CPPUNIT_TEST_SUITE_END();
  public:
    void simpleTest()
    {
      std::vector<double> v;
      for (unsigned int i = 0; i < 10; i++)
      {
        v.push_back(Tgs::Random::instance()->generateInt(100));
      }

      // testing equation ((3 * v) + sqrt(v)) - 2
      boost::shared_ptr<SubtractCalculatorNode> root(new SubtractCalculatorNode());
      AddCalculatorNode* add = new AddCalculatorNode();
      MultiplyCalculatorNode* mult = new MultiplyCalculatorNode();
      SqrtCalculatorNode* sqrtNode = new SqrtCalculatorNode();

      // ((3 * v) + sqrt(v)) - 2
      root->setInput(BinaryCalculatorNode::V1(), add);
      root->setInput(BinaryCalculatorNode::V2(), new ConstantCalculatorNodeSource(2));

      // ((3 * v) + sqrt(v))
      add->setInput(BinaryCalculatorNode::V1(), mult);
      add->setInput(BinaryCalculatorNode::V2(), sqrtNode);

      // (3 * v)
      mult->setInput(BinaryCalculatorNode::V1(), new ConstantCalculatorNodeSource(3));
      mult->setInput(BinaryCalculatorNode::V2(), new VectorCalculatorNodeSource(v));

      // sqrt(v)
      sqrtNode->setInput(UnaryCalculatorNode::V1(), new VectorCalculatorNodeSource(v));

      for (unsigned int i = 0; i < 10; i++)
      {
        double r = root->getOutput(i);
        double e = ((3 * v[i]) + sqrt(v[i])) - 2;
        ASSERT_DBL_EQL(e, r);
      }
    }
  };
}


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::CalculatorGenomeNodeTest, PluginFactory::testName());
