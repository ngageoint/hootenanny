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

#include "FeatureScoreFitnessFunction.h"

// Standard Includes
#include <math.h>
#include <vector>
using namespace std;

#include "../FeatureSelection/DataFrameDiscretizer.h"
#include "../FeatureSelection/FeatureScoreCalculator.h"
#include "../RandomForest/DataFrame.h"
#include "../TgsException.h"
#include "CalculatorGenome.h"

namespace Tgs
{
  FeatureScoreFitnessFunction::FeatureScoreFitnessFunction(boost::shared_ptr<DataFrame> df, 
    boost::shared_ptr<FeatureScoreCalculator> fsc)
  {
    _workingCopy.reset(new DataFrame());
    std::vector<std::string> factorLabels;
    factorLabels.push_back("uut");
    _workingCopy->setFactorLabels(factorLabels);
    vector<double> v;
    v.resize(1, 0);
    // fill it up w/ empty data
    for (unsigned int i = 0; i < df->getNumDataVectors(); i++)
    {
      _workingCopy->addDataVector(df->getTrainingLabel(i), v);
    }

    _fsc = fsc;
  }

  FeatureScoreFitnessFunction::~FeatureScoreFitnessFunction()
  {

  }

  double FeatureScoreFitnessFunction::calculateFitness(const Genome& genome)
  {
    const CalculatorGenome& cg = dynamic_cast<const CalculatorGenome&>(genome);
    _workingCopy->setFactorType(0, DataFrame::Numerical);
    for (unsigned int i = 0; i < _workingCopy->getNumDataVectors(); i++)
    {
      _workingCopy->setDataElement(i, 0, cg.calculateValue(i));
    }
    DataFrameDiscretizer dfd;
    try
    {
      dfd.discretize(*_workingCopy);
    }
    catch (const Tgs::Exception&)
    {
      // probably because of no valid values.
      return 0.0;
    }
    double score = _fsc->calculateScore(*_workingCopy, -1, *_workingCopy, 0);
    int nodeCount = cg.countNodes();
    if (nodeCount > 10)
    {
      score *= pow(.95, nodeCount - 10);
    }
    return score;
  }
}
