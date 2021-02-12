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
 * @copyright Copyright (C) 2015, 2017, 2021 Maxar (http://www.maxar.com/)
 */

#include "MimicGa.h"

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
  MimicGa::MimicGa(boost::shared_ptr<Genome> seed, boost::shared_ptr<FitnessFunction> fitness) :
    GeneticAlgorithm(seed, fitness)
  {
    _thetaPercentile = .5;
    _learningRate = .5;
    _stagnation = 0;
    _lastBest = -numeric_limits<double>::infinity();
  }

  MimicGa::~MimicGa()
  {
  }

  void MimicGa::_initializeGenome(boost::shared_ptr<Genome> genome)
  {
    boost::shared_ptr<CalculatorGenome> cg = boost::dynamic_pointer_cast<CalculatorGenome>(genome);
    assert(cg);
    _setGenomeWeights(cg);
    genome->initialize();
  }

  void MimicGa::_mutate(boost::shared_ptr<Genome> genome)
  {
    boost::shared_ptr<CalculatorGenome> cg = boost::dynamic_pointer_cast<CalculatorGenome>(genome);
    assert(cg);
    _setGenomeWeights(cg);
    genome->mutate(1.0);
  }

  void MimicGa::_normalize(WeightMap& wm)
  {
    // normalize the weights so they sum to 1
    double sum = 0.0;
    for (WeightMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
    {
      sum += it->second;
    }

    if (sum != 0.0)
    {
      for (WeightMap::iterator it = wm.begin(); it != wm.end(); 
        ++it)
      {
        it->second /= sum;
      }
    }
    else
    {
      cout << "MimicGa::_updateScores: We shouldn't really be here." << endl;
      assert(false);
      // clear the weights so we don't modify any probabilities
      _weights.clear();
    }
  }

  void MimicGa::_populateWeights(const boost::shared_ptr<CalculatorGenomeNode> node)
  {
    _weights[node->getId()]++;
    const std::map<std::string, std::string>& inputs = node->getInputs();
    for (std::map<std::string, std::string>::const_iterator it = inputs.begin(); 
      it != inputs.end(); ++it)
    {
      boost::shared_ptr<CalculatorGenomeNode> child = node->getInput(it->first);
      _populateWeights(child);
    }
  }

  void MimicGa::_setGenomeWeights(boost::shared_ptr<CalculatorGenome> genome)
  {
    if (_weights.size() > 0)
    {
      // we never want the probability to be zero
      genome->resetWeights(1e-5);
      // the first time through _weights will be empty and the genome won't be modified, after
      // that this will updated all the genome weights.
      for (WeightMap::const_iterator it = _weights.begin(); it != _weights.end(); ++it)
      {
        genome->setWeight(it->first, it->second);
      }
    }
  }

  void MimicGa::setParameters(const std::map<std::string, Parameter>& params)
  {
    GeneticAlgorithm::setParameters(params);
    _loadParameter(params, "thetaPercentile", _thetaPercentile);
    _loadParameter(params, "learningRate", _learningRate);
  }

  void MimicGa::_updateScores()
  {
    GeneticAlgorithm::_updateScores();

    // sort in descending order
    sort(_population.begin(), _population.end(), CompareGenomes());

    WeightMap oldWeights = _weights;
    _weights.clear();

    // @todo test this
//     // if we're stagnating, start bringing the weights back together. This should help broaden
//     // the search a bit.
//     if (_lastBest == _population[0]->getScore())
//     {
//       _stagnation++;
//       for (WeightMap::iterator it = oldWeights.begin(); it != oldWeights.end(); ++it)
//       {
//         it->second = pow(it->second, 1.0 / (double)_stagnation);
//       }
//       _normalize(oldWeights);
//     }

    boost::shared_ptr<CalculatorGenome> cg = boost::dynamic_pointer_cast<CalculatorGenome>(_population[0]);

    const CalculatorGenome::AvailableNodeMap& anm = cg->getAvailableNodes();
    for (CalculatorGenome::AvailableNodeMap::const_iterator it = anm.begin(); it != anm.end(); 
      ++it)
    {
      _weights[it->first] = 1e-5;
    }

    for (unsigned int i = 0; i < _population.size() * _thetaPercentile; i++)
    {
      cg = boost::dynamic_pointer_cast<CalculatorGenome>(_population[i]);
      assert(cg);
      _populateWeights(cg->getRoot());
    }

    // normalize the weights so they sum to 1
    _normalize(_weights);

    if (oldWeights.size() > 0)
    {
      for (WeightMap::iterator it = _weights.begin(); it != _weights.end(); ++it)
      {
        it->second = it->second * _learningRate + oldWeights[it->first] * (1 - _learningRate);
        //cout << it->first << "\t" << it->second << endl;
      }
    }

//     for (WeightMap::iterator it = _weights.begin(); it != _weights.end(); ++it)
//     {
//       cout << it->first << "\t" << it->second << endl;
//     }
  }
}

