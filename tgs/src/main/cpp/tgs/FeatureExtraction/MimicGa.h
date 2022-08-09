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

#ifndef __TGS__MIMIC_GA_H__
#define __TGS__MIMIC_GA_H__

#include "GeneticAlgorithm.h"

namespace Tgs
{
  class CalculatorGenome;
  class CalculatorGenomeNode;

  /**
   * Hybrid implementation of MIMIC [1] and a Genetic Algorithm. This was necessary because 
   * MIMIC doesn't have an obvious way of operating directly on tree genomes. According to [1] 
   * MIMIC converges faster than typical GAs.
   *
   * **** For now this is a simple hack 
   * 
   * [1] Jeremy S. De Bonet, Charles L. Isbell, Jr., Paul Viola, 1997, 
   * "MIMIC: Finding Optima by Estimating Probability Densities", 
   * Advances in Neural Information Processing Systems 1997 MIT Press, Cambridge, MA
   * <http://www.debonet.com/research/publications/1996/DeBonet-NIPS96-MIMIC.pdf>
   */
  class TGS_EXPORT MimicGa : public GeneticAlgorithm
  {
  public:

    MimicGa(boost::shared_ptr<Genome> seed, boost::shared_ptr<FitnessFunction> fitness);

    virtual ~MimicGa();

    void setParameters(const std::map<std::string, Parameter>& params);

  protected:
    virtual void _initializeGenome(boost::shared_ptr<Genome> genome);

    virtual void _mutate(boost::shared_ptr<Genome> genome);

    virtual void _updateScores();

  private:

    double _learningRate;

    int _stagnation;
    double _lastBest;

    /**
     * [1] references using the Nth percentile of the data for determining theta. They also 
     * suggest the median value (50%). I will default to .5 (50%), but the value can be changed
     * via set parameters and a key of "thetaPercentile".
     */
    double _thetaPercentile;

    typedef std::map<std::string, double> WeightMap;
    WeightMap _weights;

    void _normalize(WeightMap& wm);

    void _setGenomeWeights(boost::shared_ptr<CalculatorGenome> genome);

    void _populateWeights(const boost::shared_ptr<CalculatorGenomeNode> node);
  };
}


#endif
