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

#include "CfsFitnessFunction.h"

// Standard Includes
#include <iostream>
#include <math.h>
#include <sstream>
#include <time.h>
#include <vector>
using namespace std;

#include "../FeatureSelection/CfsSubsetEvaluator.h"
#include "../FeatureSelection/DataFrameDiscretizer.h"
#include "../FeatureSelection/FeatureScoreCalculator.h"
#include "../RandomForest/DataFrame.h"
#include "../Statistics/Random.h"
#include "../System/Time.h"
#include "../TgsException.h"
#include "CalculatorGenome.h"

namespace Tgs
{
  CfsFitnessFunction::CfsFitnessFunction(boost::shared_ptr<DataFrame> df, const std::vector<int>& uids)
  {
    _baseCopy.reset(new DataFrame());
    std::vector<std::string> factorLabels;
    factorLabels.push_back("uut");
    _baseCopy->setFactorLabels(factorLabels);
    if (df->getNumDataVectors() == 0)
    {
      throw Exception("Data frame must have more than one vector.");
    }

    vector<double> v;
    v.resize(1, 0);
    _baseUids.resize(df->getNumDataVectors());
    // fill it up w/ empty data and real class labels
    for (unsigned int i = 0; i < df->getNumDataVectors(); i++)
    {
      _baseCopy->addDataVector(df->getTrainingLabel(i), v);
      if (uids.size() > 0)
      {
        _baseUids[i] = uids[i];
      }
      else
      {
        _baseUids[i] = i;
      }
    }

    initializeNewGeneration();
    _workingFactor = 0;
    _baseScore = 0;
  }

  CfsFitnessFunction::~CfsFitnessFunction()
  {

  }

  boost::shared_ptr<DataFrame> CfsFitnessFunction::_addOneFactor(boost::shared_ptr<DataFrame> df)
  {
    boost::shared_ptr<DataFrame> result(new DataFrame());
    std::vector<std::string> factorLabels = df->getFactorLabels();
    stringstream strm;
    strm << "f" << factorLabels.size();
    factorLabels.push_back(strm.str());
    result->setFactorLabels(factorLabels);
    vector<double> v;
    // fill it up w/ a copy of df data and one new column of all zeros
    for (unsigned int i = 0; i < df->getNumDataVectors(); i++)
    {
      v = df->getDataVector(i);
      v.push_back(0);
      result->addDataVector(df->getTrainingLabel(i), v);
    }
    return result;
  }

  void CfsFitnessFunction::addFeature(boost::shared_ptr<Genome> genome)
  {
    boost::shared_ptr<CalculatorGenome> cg = boost::dynamic_pointer_cast<CalculatorGenome>(genome);
    _baseScore = calculateFitness(*cg);
    _loadGenome(*cg);
    _baseCopy = _addOneFactor(_baseCopy);
    _workingFactor++;
    initializeNewGeneration();
    _features.push_back(cg);
  }

  double CfsFitnessFunction::calculateFitness(const Genome& genome)
  {
    const CalculatorGenome& cg = dynamic_cast<const CalculatorGenome&>(genome);
    _workingCopy->setFactorType(_workingFactor, DataFrame::Numerical);
    double start = Time::getTime();
    //cout << cg.toString() << endl;
    // one tile per 3 minute
    const double MIN_EVAL_PER_SECOND = 250000 / 180;
    for (unsigned int i = 0; i < _workingCopy->getNumDataVectors(); i++)
    {
      double elapsed = Time::getTime() - start;
      if (elapsed >= 0.5)
      {
        double speed = i / elapsed;
        cout << "speed " << speed << "\r";
        if (speed < MIN_EVAL_PER_SECOND || elapsed >= 10)
        {
          //cout << endl << "Too slow: " << cg.toString() << endl;
          return -1e9;
        }
        cout.flush();
      }
      double v;
      v = cg.calculateValue(_workingUids[i]);
      _workingCopy->setDataElement(i, _workingFactor, v);
    }
    DataFrameDiscretizer dfd;
    try
    {
      dfd.discretize(*_workingCopy);
    }
    catch (const DataFrameDiscretizer::AllNullsException&)
    {
      // because of no valid values.
      return -1e9;
    }

    CfsSubsetEvaluator cse;
    cse.setDataFrame(*_workingCopy);
    std::vector<int> v;
    v.resize(_workingCopy->getNumFactors());
    for (unsigned int i = 0; i < _workingCopy->getNumFactors(); i++)
    {
      v[i] = i;
    }
    double score = cse.evaluateSubset(v) - _baseScore;
    int nodeCount = cg.countNodes();
    // penalize for large trees
    const int MAX_DESIRABLE_SIZE = 10;
    if (nodeCount > MAX_DESIRABLE_SIZE)
    {
      double m = 1 - pow(nodeCount - MAX_DESIRABLE_SIZE, 1.4) * .05;
      //cout << nodeCount << ", " << m << ", " << score << ", " << score * m << endl;
      if (m <= 0)
      {
        score = -numeric_limits<double>::infinity();
      }
      else if (score > 0.0)
      {
        score *= m;
      }
      else
      {
        score /= m;
      }
    }
    return score;
  }

  void CfsFitnessFunction::initializeNewGeneration()
  {
    _workingCopy.reset(new DataFrame());
    std::vector<std::string> factorLabels = _baseCopy->getFactorLabels();
    _workingCopy->setFactorLabels(factorLabels);

    // select a random sampling of the data
    int randomSample = std::min((unsigned int)2000, _baseCopy->getNumDataVectors());
    std::vector<int> samples;
    samples.resize(_baseCopy->getNumDataVectors());
    for (unsigned int i = 0; i < _baseCopy->getNumDataVectors(); i++)
    {
      samples[i] = i;
    }
    Random::randomizeVector(samples);
    samples.resize(randomSample);
    _workingUids.resize(randomSample);
    // fill it up w/ empty data and real class labels
    for (unsigned int i = 0; i < samples.size(); i++)
    {
      _workingCopy->addDataVector(_baseCopy->getTrainingLabel(samples[i]), 
        _baseCopy->getDataVector(samples[i]));
      _workingUids[i] = _baseUids[samples[i]];
    }
  }

  void CfsFitnessFunction::_loadGenome(const CalculatorGenome& cg)
  {
    _baseCopy->setFactorType(_workingFactor, DataFrame::Numerical);
    for (unsigned int i = 0; i < _baseCopy->getNumDataVectors(); i++)
    {
      _baseCopy->setDataElement(i, _workingFactor, cg.calculateValue(i));
    }
  }
}
