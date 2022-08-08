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
 * @copyright Copyright (C) 2015, 2017, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Standard Includes
#include <float.h>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdio.h>
#include <time.h>
using namespace std;

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include <tgs/StreamUtils.h>
#include <tgs/FeatureExtraction/BasicMathCalculatorNode.h>
#include <tgs/FeatureExtraction/CalculatorGenome.h>
#include <tgs/FeatureExtraction/ConstantCalculatorNodeSource.h>
#include <tgs/FeatureExtraction/DataFrameCalculatorNodeSource.h>
#include <tgs/FeatureExtraction/FeatureScoreFitnessFunction.h>
#include <tgs/FeatureExtraction/GeneticAlgorithm.h>
#include <tgs/FeatureExtraction/VectorCalculatorNodeSource.h>
#include <tgs/FeatureSelection/SymmetricUncertaintyCalculator.h>
#include <tgs/RandomForest/DataFrame.h>
#include "../PluginFactory.h"

#define ASSERT_DBL_EQL(expected, actual) CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, 1e-4)

namespace Tgs
{

  class GeneticAlgorithmTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(GeneticAlgorithmTest);
    //CPPUNIT_TEST(dataFrameTest);
    CPPUNIT_TEST_SUITE_END();
  public:

    /**
     * Test to see if the GA can derive the correct answer
     */
    void dataFrameTest()
    {
      // load data frame
      cout << "Importing data frame... ";
      boost::shared_ptr<DataFrame> df(new DataFrame);
      fstream fs("OttawaCars.xml", ios_base::in);
      if (fs.is_open() == false)
      {
        throw Exception("Error opening OttawaGroundCover.xml");
      }
      df->import(fs);
      cout << "done." << endl;

      boost::shared_ptr<CalculatorGenome> genome(new CalculatorGenome());
      genome->setInitializationDepth(4);
      genome->addBasicMathNodeTypes();

      // create source node for each factor
      for (unsigned int i = 0; i < df->getNumFactors(); i++)
      {
        boost::shared_ptr<DataFrameCalculatorNodeSource> src(new DataFrameCalculatorNodeSource(df, i));
        string label = df->getFactorLabelFromIndex(i);
        if (label != "MAX_Z_GROUND" && label != "MIN_Z_GROUND" && 
          label != "MAX_Z_AERIAL" && label != "MIN_Z_AERIAL")
        {
          genome->addNodeType(src, label, 2.0 / (double)df->getNumFactors());
        }
      }

      // create a fitness function using symmetric uncertainty
      boost::shared_ptr<FeatureScoreCalculator> fsc(new SymmetricUncertaintyCalculator());
      boost::shared_ptr<FeatureScoreFitnessFunction> fitness(new FeatureScoreFitnessFunction(df, fsc));

      GeneticAlgorithm ga(genome, fitness);
      ga.setPopulationSize(500);

      // population size
      // mutation rate
      // mutation severity
      // free pass
      // keep best
      // mating percent
      // fresh meat

      int c = 0;
      for (unsigned int s = 0; s < 300; s++)
      {
        ga.step();
        c+= ga.getPopulation().size();
        boost::shared_ptr<CalculatorGenome> best =
          boost::dynamic_pointer_cast<CalculatorGenome>(ga.getBestGenome());
        cout << c << "\t" << best->getScore() << "\t" << best->toString() << endl;
//         if (1 / best->getScore() < 1.0)
//         {
//           break;
//         }
        //cout << endl;
        for (unsigned int i = 0; i < ga.getPopulation().size() && i < 10; i++)
        {
          cout << "  " << ga.getPopulation()[i]->getScore() << "\t" << 
            ga.getPopulation()[i]->toString() << endl;
        }
      }
    }
  };
}


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::GeneticAlgorithmTest, PluginFactory::testName());
