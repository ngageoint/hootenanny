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
 * @copyright Copyright (C) 2015, 2016, 2017, 2021 Maxar (http://www.maxar.com/)
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
#include <tgs/FeatureExtraction/FitnessFunction.h>
#include <tgs/FeatureExtraction/MimicGa.h>
#include <tgs/FeatureExtraction/VectorCalculatorNodeSource.h>
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
    SimpleFitness(const vector<double>& v1, const vector<double>& v2)
    {
      setValues(v1, v2);
    }

    virtual double calculateFitness(const Genome& genome)
    {
      const int MAX_REASONABLE_SIZE = 7;
      const CalculatorGenome& cg = dynamic_cast<const CalculatorGenome&>(genome);
      double result = 0.0;
      std::string tmp = cg.toString();
      // calculate mean squared error
      for (unsigned int i = 0; i < _values1.size(); i++)
      {
        double vt = cg.calculateValue(i);
        double truth = _values1[i] / (_values1[i] + _values2[i]);
        //double truth = _values1[i] * 3 + sqrt(_values2[i]);
        result += fabs(vt - truth) * fabs(vt - truth);
      }
      int count = cg.countNodes();

      result /= (double)_values1.size();

      // if there is no error, the score is infinitely high
      if (fabs(result) <= 1e-6)
      {
        result = numeric_limits<double>::infinity();
      }
      // otherwise, invert the score to make higher errors smaller scores
      else
      {
        //const double reasonable = .01;
        //result = reasonable / (result + reasonable);
        result = sqrt(1 / result);
      }
      if (count > MAX_REASONABLE_SIZE)
      {
        // penalize for large trees
        result *= pow(.99, count - MAX_REASONABLE_SIZE);
      }

      return result;
    }

    void setValues(const vector<double>& v1, const vector<double>& v2)
    {
      _values1 = v1;
      _values2 = v2;
    }

  private:
    vector<double> _values1, _values2;
  };

  class CalculatorGenomeTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(CalculatorGenomeTest);
    //CPPUNIT_TEST(initializeTest);
    //CPPUNIT_TEST(deriveTest);
    CPPUNIT_TEST(serializationTest);
    CPPUNIT_TEST_SUITE_END();
  public:
    // I can't come up w/ a non-brittle way of automatically testing this. I have just visually
    // inspected. I hope to revisit.
    void initializeTest()
    {
      Tgs::Random::instance()->seed(0);

      std::vector<double> v;
      for (unsigned int i = 0; i < 10; i++)
      {
        v.push_back(Tgs::Random::instance()->generateInt(1000));
      }

      for (unsigned int i = 0; i < 10; i++)
      {
        boost::shared_ptr<CalculatorGenome> father(new CalculatorGenome());
        father->addBasicMathNodeTypes();
        father->addNodeType(new VectorCalculatorNodeSource(v), "VectorCalculatorNodeSource", 1.0);
        father->initialize();
        cout << "dad: " << father->toString() << endl;

        boost::shared_ptr<CalculatorGenome> mother =
          boost::dynamic_pointer_cast<CalculatorGenome>(father->clone());
        mother->initialize();
        cout << "mom: " << mother->toString() << endl;

        boost::shared_ptr<Genome> brother, sister;
        father->crossoverSexually(*father, *mother, brother, sister);

        cout << "bro: " << brother->toString() << endl;
        cout << "sis: " << sister->toString() << endl << endl;
      }
    }

    double calculateMedianScore(const map<string, Parameter>& params, int maxPopulation,
      int VARIABLE_SIZE, int& individuals)
    {
      const int RUNS = 7;
      vector<double> scores;
      vector<int> rawIndividuals;
      double indvSum = 0;
      for (int i = 0; i < RUNS; i++)
      {
        double score = deriveTestRun(params, maxPopulation, VARIABLE_SIZE, individuals);
        scores.push_back(score);
        rawIndividuals.push_back(individuals);
        indvSum += individuals;
      }

      for (unsigned int i = 0; i < scores.size(); i++)
      {
        for (unsigned int j = 0; j < scores.size(); j++)
        {
          if ((scores[i] == scores[j] && rawIndividuals[i] < rawIndividuals[j]) ||
            scores[i] > scores[j])
          {
            std::swap(scores[i], scores[j]);
            std::swap(rawIndividuals[i], rawIndividuals[j]);
          }
        }
      }

      individuals = int(indvSum / RUNS);
      return scores[RUNS / 2];
    }

    /**
     * Test to see if the GA can derive the correct answer
     * score, evals, population size,  bad variables
     * .00724 27163  2000              120
     * .00727 108065 10000             120
     */
    void deriveTest()
    {
      fstream results("results.txt", ios::out);
      if (results.bad())
      {
        cout << "Error opening results file." << endl;
        return;
      }

      map<string, Parameter> params;
      params["population"] = Parameter(2, 100000, 1000, 1500);
      params["variableWeight"] = Parameter(1e-6, 100, 10, .5);
      params["mutationRate"] = Parameter(0, 1, .9, .3);
      params["mutationSeverity"] = Parameter(0, 1, .303, .3);
      params["keepBest"] = Parameter(0, 100, 6.2, 10);
      params["freshMeat"] = Parameter(0, 100, 35.3, 10);
      params["thetaPercentile"] = Parameter(.01, .99, .5, .3);
      params["learningRate"] = Parameter(.01, .99, .2, .3);

      results << "score\tindividuals\tvariableSize";
      for (map<string, Parameter>::iterator it = params.begin(); it != params.end(); ++it)
      {
        results << "\t" << it->first;
      }
      results << endl;

      int maxPopulation = 100000;

      for (int vs = 8; vs < 130; vs += 16)
      {
        maxPopulation *= 2;
        double bestScore = -1;
        double bestIndividuals = 1e9;
        map<string, Parameter> bestParams = params;
        double temp = 1.0;
        while (temp >= 0.01)
        {
          int individuals;
          cout << "Temp: " << temp << endl;
          cout << "Parameters:" << endl;
          for (map<string, Parameter>::iterator it = params.begin(); it != params.end(); ++it)
          {
            cout << "\t" << it->first << ": \t" << it->second.value << endl;
          }
          double score = calculateMedianScore(params, maxPopulation, vs, individuals);
          cout << "Individuals: " << individuals << "\tScore: " << score << endl;
          if (score > bestScore ||
            (score == bestScore && individuals < bestIndividuals))
          {
            cout << "*** Best Score: " << score << " Indv: " << individuals << " ***" << endl;
            bestScore = score;
            bestIndividuals = individuals;
            maxPopulation = (int)(individuals * 2.5);
            bestParams = params;
          }
          else
          {
            cout << "Old Best: " << bestScore << " Indv: " << bestIndividuals << endl;
          }

          results << score << "\t" << individuals << "\t" << vs;
          for (map<string, Parameter>::iterator it = params.begin(); it != params.end(); ++it)
          {
            results << "\t" << it->second.value;
          }
          results << endl;

          temp *= .8;
          params = bestParams;
          for (map<string, Parameter>::iterator it = params.begin(); it != params.end(); ++it)
          {
            it->second.mutate(temp);
          }
        }
      }
    }

    double deriveTestRun(const map<string, Parameter>& params,
      int maxIndividuals, const int VARIABLE_SIZE, int& individuals)
    {
      const int SAMPLE_SIZE = 100;
      std::vector<double> v1, v2;
      v1.resize(SAMPLE_SIZE);
      v2.resize(SAMPLE_SIZE);
      for (int i = 0; i < SAMPLE_SIZE; i++)
      {
        v1[i] = Tgs::Random::instance()->generateInt();
        v2[i] = Tgs::Random::instance()->generateInt();
      }

      double vweight = 1.0 / (VARIABLE_SIZE + 2) * (params.find("variableWeight")->second.value);

      boost::shared_ptr<SimpleFitness> fitness(new SimpleFitness(v1, v2));

      boost::shared_ptr<CalculatorGenome> genome(new CalculatorGenome());
      genome->addBasicMathNodeTypes();
      VectorCalculatorNodeSource* good1 = new VectorCalculatorNodeSource(v1);
      good1->setLabel("v1[]");
      genome->addNodeType(good1, "v1", vweight);
      VectorCalculatorNodeSource* good2 = new VectorCalculatorNodeSource(v2);
      good2->setLabel("v2[]");
      genome->addNodeType(good2, "v2", vweight);
      genome->setInitializationDepth(4);

      genome->setWeight("class Tgs::ConstantCalculatorNodeSource", vweight * .5);

      std::vector<boost::shared_ptr<VectorCalculatorNodeSource> > badGenome;
      for (int j = 0; j < VARIABLE_SIZE; j++)
      {
        stringstream strm;
        strm << "bad" << j;
        std::vector<double> bad;
        for (int i = 0; i < SAMPLE_SIZE; i++)
        {
          bad.push_back(Tgs::Random::instance()->generateInt());
        }
        boost::shared_ptr<VectorCalculatorNodeSource> src(new VectorCalculatorNodeSource(bad));
        src->setLabel(strm.str() + "[]");
        genome->addNodeType(src, strm.str(), vweight);
        badGenome.push_back(src);
      }


      MimicGa ga(genome, fitness);
      //ga.enableScoreCaching();
      ga.setParameters(params);

      int c = 0;
      double score = -1;
      double oldScore = -1;
      int stagnant = 0;
      while (c < maxIndividuals)
      {
        for (int i = 0; i < SAMPLE_SIZE; i++)
        {
          v1[i] = Tgs::Random::instance()->generateInt();
          v2[i] = Tgs::Random::instance()->generateInt();
        }
        fitness->setValues(v1, v2);
        good1->setSource(v1);
        good2->setSource(v2);
        for (int j = 0; j < VARIABLE_SIZE; j++)
        {
          std::vector<double> bad;
          for (int i = 0; i < SAMPLE_SIZE; i++)
          {
            bad.push_back(Tgs::Random::instance()->generateInt());
          }
          badGenome[j]->setSource(bad);
        }


        ga.step();
        c+= ga.getPopulation().size();
        boost::shared_ptr<CalculatorGenome> best =
          boost::dynamic_pointer_cast<CalculatorGenome>(ga.getBestGenome());
        score = best->getScore();
        //cout << "individuals: " << c << "\r";
        stagnant++;
        if (score > oldScore)
        {
          cout << c << "\t" << best->getScore() << "\t" << best->toString() << "              \r";
          stagnant = 0;
//           ga.setPopulationSize(populationSize);
        }
        oldScore = score;
//         if (stagnant >= 5)
//         {
//           ga.setPopulationSize(ga.getPopulation().size() * 1.1);
//         }
//         if (best->getScore() > .99)
//         {
//           cout << "Unique genomes: " << ga.getUniqueEvalCount() << endl;
//           break;
//         }
        if (score == numeric_limits<double>::infinity())
        {
          break;
        }
        //cout << endl;
//         for (unsigned int i = 0; i < ga.getPopulation().size() && i < 15; i++)
//         {
//           cout << "  " << ga.getPopulation()[i]->getScore() << "\t" <<
//             ga.getPopulation()[i]->toString() << endl;
//         }
      }
      cout << endl << "Evals: " << c << " Unique genomes: " << ga.getUniqueEvalCount() << endl;
      individuals = c;
      return score;
    }

    void serializationTest()
    {
      for (unsigned int i = 0; i < 10; i++)
      {
        boost::shared_ptr<CalculatorGenome> father(new CalculatorGenome());
        father->addBasicMathNodeTypes();
        father->initialize();

        stringstream strm;
        father->save(strm);

        boost::shared_ptr<CalculatorGenome> loader(new CalculatorGenome());
        //stringstream strm2(strm.str());
        loader->addBasicMathNodeTypes();
        loader->load(strm);
        stringstream strm2;
        loader->save(strm2);
        CPPUNIT_ASSERT_EQUAL(strm.str(), strm2.str());
        CPPUNIT_ASSERT_EQUAL(father->toString(), loader->toString());
      }
    }
  };
}


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::CalculatorGenomeTest, PluginFactory::testName());
