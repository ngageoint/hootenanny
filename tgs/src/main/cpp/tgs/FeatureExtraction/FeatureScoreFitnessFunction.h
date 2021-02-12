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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__FEATURE_SCORE_FITNESS_FUNCTION_H__
#define __TGS__FEATURE_SCORE_FITNESS_FUNCTION_H__

#include "../SharedPtr.h"
#include "../FeatureSelection/FeatureScoreCalculator.h"
#include "FitnessFunction.h"

namespace Tgs
{
  class DataFrame;

  /**
   * Calculates the fitness of a given CalculatorGenome
   */
  class TGS_EXPORT FeatureScoreFitnessFunction : public FitnessFunction
  {
  public:

    FeatureScoreFitnessFunction(boost::shared_ptr<DataFrame> df, boost::shared_ptr<FeatureScoreCalculator> fsc);

    virtual ~FeatureScoreFitnessFunction();

    virtual double calculateFitness(const Genome& genome);
  
  private:

    boost::shared_ptr<DataFrame> _workingCopy;
    boost::shared_ptr<FeatureScoreCalculator> _fsc;
  };
}


#endif
