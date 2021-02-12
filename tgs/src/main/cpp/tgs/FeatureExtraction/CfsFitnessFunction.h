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

#ifndef __TGS__CFS_FITNESS_FUNCTION_H__
#define __TGS__CFS_FITNESS_FUNCTION_H__

// Standard Includes
#include <vector>

#include "../SharedPtr.h"
#include "../FeatureSelection/FeatureScoreCalculator.h"
#include "FitnessFunction.h"

namespace Tgs
{
  class CalculatorGenome;
  class DataFrame;

  /**
   * Calculates the fitness of a given CalculatorGenome
   */
  class TGS_EXPORT CfsFitnessFunction : public FitnessFunction
  {
  public:

    CfsFitnessFunction(boost::shared_ptr<DataFrame> df, const std::vector<int>& uids);

    virtual ~CfsFitnessFunction();

    void addFeature(boost::shared_ptr<Genome> genome);

    virtual double calculateFitness(const Genome& genome);

    double getBaseScore() const { return _baseScore; }

    const std::vector<boost::shared_ptr<CalculatorGenome> >& getGenomes() const { return _features; }

    void initializeNewGeneration();
  
  private:

    double _baseScore;
    boost::shared_ptr<DataFrame> _baseCopy;
    std::vector<int> _baseUids;
    boost::shared_ptr<DataFrame> _workingCopy;
    std::vector<int> _workingUids;
    std::vector<boost::shared_ptr<CalculatorGenome> > _features;
    int _workingFactor;

    /**
     * Add one empty factor to the specified data frame.
     */
    boost::shared_ptr<DataFrame> _addOneFactor(boost::shared_ptr<DataFrame> df);

    void _loadGenome(const CalculatorGenome& genome);
  };
}


#endif
