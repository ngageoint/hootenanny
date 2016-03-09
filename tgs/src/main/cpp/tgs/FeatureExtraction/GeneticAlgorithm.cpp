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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "GeneticAlgorithm.h"

// Standard Includes
#include <iostream>
#include <limits>
#include <math.h>
using namespace std;

#include "FitnessFunction.h"
#include "CalculatorGenome.h"
#include "Genome.h"
#include "../Statistics/Random.h"
#include "../Statistics/Distribution.h"

namespace Tgs
{
  GeneticAlgorithm::GeneticAlgorithm(shared_ptr<Genome> seed, shared_ptr<FitnessFunction> fitness)
  {
    _seed = seed;
    _fitness = fitness;
    _populationSize = 20;
    _keepBest = 1;
    _initialized = false;
    _mutationProb = .5;
    _mutationSeverity = .8;
    _freshMeat = 3;
    _scoreCaching = false;
  }

  GeneticAlgorithm::~GeneticAlgorithm()
  {

  }

  void GeneticAlgorithm::initialize()
  {
    int maxDepth = 6;
    while ((int)_population.size() < _populationSize)
    {
      shared_ptr<Genome> g = _seed->clone();

      shared_ptr<CalculatorGenome> cg = dynamic_pointer_cast<CalculatorGenome>(g);
      // use a ramped grow method, see page 12 in 
      // http://www.lulu.com/items/volume_63/2167000/2167025/2/print/book.pdf
      if (cg)
      {
        cg->setInitializationDepth((_population.size() * maxDepth / _populationSize) + 1);
      }
      _initializeGenome(g);
      if (_used.find(g->toString()) == _used.end())
      {
        _population.push_back(g);
        _used.insert(g->toString());
      }
    }
    //cout << "Updating scores: " << _population.size() << endl;
    _updateScores();
    _initialized = true;
  }

  void GeneticAlgorithm::_initializeGenome(shared_ptr<Genome> genome)
  {
    genome->initialize();
  }

  void GeneticAlgorithm::_mutate(shared_ptr<Genome> genome)
  {
    genome->mutate(1.0);
  }

  // hmm, doesn't seem to work. :(
  int GeneticAlgorithm::_selectMateGaussian()
  {
    double sigma = _population.size() / 7;
    double sum = 0.0;
    for (unsigned int i = 0; i < _population.size(); i++)
    {
      double score = _population[i]->getScore();
      if (score > -numeric_limits<double>::infinity())
      {
        sum += Distribution::calculationGaussian(0, sigma, 
          (double)i / (double)_population.size());
      }
    }


    double pick = Random::instance()->generateUniform() * sum;
    double s = 0;
    for (unsigned int i = 0; i < _population.size(); i++)
    {
      double score = _population[i]->getScore();
      if (score > -numeric_limits<double>::infinity())
      {
        s += Distribution::calculationGaussian(0, sigma, 
          (double)i / (double)_population.size());
        if (pick <= s)
        {
          return i;
        }
      }
    }

    cout << "Shouldn't be here. :( " << sum << endl;
    return Tgs::Random::instance()->s_generateInt() % _population.size();
  }

  int GeneticAlgorithm::_selectMateRoulette()
  {
    double popMin = numeric_limits<double>::infinity();
    double popMax = -numeric_limits<double>::infinity();
    for (unsigned int i = 0; i < _population.size(); i++)
    {
      double score = _population[i]->getScore();
      if (score > -numeric_limits<double>::infinity())
      {
        popMin = std::min(popMin, score);
        popMax = std::max(popMax, score);
      }
    }
    if (popMin == popMax)
    {
      return Tgs::Random::instance()->s_generateInt() % _population.size();
    }
    if (popMin > 0)
    {
      popMin = 0;
    }
    double popSum = 0.0;
    for (unsigned int i = 0; i < _population.size(); i++)
    {
      double score = _population[i]->getScore();
      if (score > -numeric_limits<double>::infinity())
      {
        popSum += score - popMin;
      }
    }

    double pick = Tgs::Random::instance()->s_generateUniform() * popSum;
    double s = 0;
    for (unsigned int i = 0; i < _population.size(); i++)
    {
      double score = _population[i]->getScore();
      if (score > -numeric_limits<double>::infinity())
      {
        s += _population[i]->getScore() - popMin;
        if (pick <= s)
        {
          return i;
        }
      }
    }

    cout << "Shouldn't be here. :( " << popSum << endl;
    return Tgs::Random::instance()->s_generateInt() % _population.size();
  }

  int GeneticAlgorithm::_selectMateTournament()
  {
    int m1 = _selectMateRoulette();
    int m2 = _selectMateRoulette();
    if (_population[m1]->getScore() > _population[m2]->getScore())
    {
      return m1;
    }
    else
    {
      return m2;
    }
  }

  void GeneticAlgorithm::setParameters(const std::map<std::string, Parameter>& params)
  {
    _loadParameter<int>(params, "population", _populationSize);
    _loadParameter<double>(params, "mutationRate", _mutationProb);
    _loadParameter<double>(params, "mutationSeverity", _mutationSeverity);
    _loadParameter<int>(params, "keepBest", _keepBest);
    _loadParameter<int>(params, "freshMeat", _freshMeat);
  }

  void GeneticAlgorithm::step()
  {
    // the first step is just initialization
    if (_initialized == false)
    {
      initialize();
      sort(_population.begin(), _population.end(), CompareGenomes());
      return;
    }
    if (_best->getScore() == numeric_limits<double>::infinity())
    {
      return;
    }

    // sort in descending order
    sort(_population.begin(), _population.end(), CompareGenomes());

    vector< shared_ptr<Genome> > nextGen;
    nextGen.reserve(_populationSize);

    for (int i = 0; i < _keepBest && i < (int)_population.size(); i++)
    {
      nextGen.push_back(_population[i]->clone());
    }

    for (int i = 0; i < _keepBest && i < (int)_population.size(); i++)
    {
      shared_ptr<Genome> g = _population[i]->clone();
      g->mutate(_mutationSeverity);
      nextGen.push_back(g);
    }

    for (int i = 0; i < _freshMeat; i++)
    {
      shared_ptr<Genome> g = _seed->clone();
      _initializeGenome(g);
      if (_used.find(g->toString()) == _used.end())
      {
        nextGen.push_back(g);
        _used.insert(g->toString());
      }
    }

    while ((int)nextGen.size() < _populationSize)
    {
//       if (nextGen.size() % 500 == 0 && nextGen.size() > 0)
//       {
//         cout << nextGen.size() << "/" << _populationSize << "\r";
//         cout.flush();
//       }
      // choose the act of 'god'
      double act = Tgs::Random::instance()->s_generateUniform();
      //double mutate = Tgs::Random::instance()->s_generateUniform();
      // if they get a free pass, randomly select a genome and pass it on to the next gen
      if (act < _mutationProb)
      {
        int index = _selectMateRoulette();
        shared_ptr<Genome> genome = _population[index]->clone();
        _mutate(genome);
//         // make sure at least half of the new generation is unique
//         if ((int)nextGen.size() < _populationSize / 4)
//         {
//           if (_used.find(genome->toString()) == _used.end())
//           {
//             nextGen.push_back(genome);
//           }
//         }
//         else
        {
          nextGen.push_back(genome);
        }
      }
      // otherwise we're mating something
      else
      {
        int fatherIndex = _selectMateTournament();
        int motherIndex = _selectMateTournament();
        if (fatherIndex != motherIndex)
        {
          shared_ptr<Genome> father = _population[fatherIndex];
          shared_ptr<Genome> mother = _population[motherIndex];
          shared_ptr<Genome> brother, sister;
          father->crossoverSexually(*father, *mother, brother, sister);
//           if (mutate < _mutationProb)
//           {
//             brother->mutate(_mutationSeverity);
//             sister->mutate(_mutationSeverity);
//           }
//           // make sure at least one fourth of the new generation is unique
//           if ((int)nextGen.size() < _populationSize / 4)
//           {
//             if (_used.find(brother->toString()) == _used.end())
//             {
//               nextGen.push_back(brother);
//               _used.insert(brother->toString());
//             }
//             if ((int)nextGen.size() < _populationSize)
//             {
//               if (_used.find(sister->toString()) == _used.end())
//               {
//                 nextGen.push_back(sister);
//                 _used.insert(sister->toString());
//               }
//             }
//           }
//           else
          {
            nextGen.push_back(brother);
            _used.insert(brother->toString());
            nextGen.push_back(sister);
            _used.insert(sister->toString());
          }
        }
      }
    }
    //cout << "                         \r";
    _population = nextGen;
    _updateScores();
//     // sort in descending order
//     sort(_population.begin(), _population.end(), CompareGenomes());
//     nextGen.clear();
//     string lastStr = _population[0]->toString();
//     nextGen.push_back(_population[0]);
//     for (unsigned int i = 1; i < _population.size(); i++)
//     {
//       if (_population[i]->toString() != lastStr)
//       {
//         nextGen.push_back(_population[i]);
//         lastStr = _population[i]->toString();
//       }
//     }
//     _population = nextGen;
  }

  void GeneticAlgorithm::_updateScores()
  {
    bool printedProgress = false;
    for (unsigned int i = 0; i < _population.size(); i++)
    {
      shared_ptr<Genome> g = _population[i];
      double score = g->getScore();
      string str = g->toString();
      _used.insert(str);
//       if (i % 500 == 0 && i > 0)
//       {
//         cout << i + 1 << " / " << _population.size() << "                  \r";
//         cout.flush();
//         printedProgress = true;
//       }
      //cout << "Updating score: " << str << endl << "  ";
      if (_scoreCaching == false || g->isValidScore() == false)
      {
        HashMap<string, double>::const_iterator it = _allScores.find(str);
        if (_scoreCaching == false || it == _allScores.end())
        {
          score = _fitness->calculateFitness(*g);
          //cout << "Updating... ";
          _allScores[str] = score;
        }
        else
        {
          score = it->second;
        }
        //cout << score << endl;
        g->setScore(score);
      }
      if (_best == NULL || score > _best->getScore())
      {
        _best = g;
      }
    }
    if (printedProgress)
    {
      cout << "                       \r";
    }
  }
}

