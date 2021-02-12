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
 * @copyright Copyright (C) 2015, 2016, 2017 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__GENETIC_ALGORITHM_H__
#define __TGS__GENETIC_ALGORITHM_H__

// Standard Includes
#include <map>
#include <vector>

#include "../HashMap.h"
#include "../SharedPtr.h"
#include "../Statistics/Random.h"
#include "../TgsExport.h"
#include "Genome.h"

namespace Tgs
{
  class FitnessFunction;

  class TGS_EXPORT Parameter
  {
  public:
    Parameter() {}
    Parameter(double min, double max, double mean, double sigma)
    {
      this->min = min;
      this->max = max;
      this->value = mean;
      this->sigma = sigma;
    }

    void mutate(double temp)
    {
      double v;
      // try to generate a valid value from the gaussian distribution 10 times...
      int count = 0;
      do
      {
        v = Random::instance()->generateGaussian(value, sigma * temp);
        count++;
      } while ((v < min || v > max) && count < 10);
      // force the value within bounds, just in case we couldn't do it in ten tries
      value = std::min(max, std::max(min, v));
    }

    double min, max;
    double value;
    double sigma;
  };

  /**
   * An implementation of a generic genetic algorithm (GA). This is not modeled after any
   * specific GA.
   */
  class TGS_EXPORT GeneticAlgorithm
  {
  public:

    GeneticAlgorithm(boost::shared_ptr<Genome> seed, boost::shared_ptr<FitnessFunction> fitness);

    virtual ~GeneticAlgorithm();

    /**
     * clear out the scores in case the fitness function changes between generations.
     */
    void clearScoreCache() { _allScores.clear(); }

    void enableScoreCaching() { _scoreCaching = true; }

    boost::shared_ptr<Genome> getBestGenome() const { return _best; }

    const std::vector<boost::shared_ptr<Genome> >& getPopulation() const { return _population; }

    int getUniqueEvalCount() const { return _used.size(); }

    void initialize();

    void setParameters(const std::map<std::string, Parameter>& params);

    void setPopulationSize(int size) { _populationSize = size; }

    void step();

  protected:

    class CompareGenomes
    {
    public:
      bool operator() (const boost::shared_ptr<Genome>& a, const boost::shared_ptr<Genome>& b) const
      {
        return a->getScore() > b->getScore();
      }
    };

    std::vector<boost::shared_ptr<Genome> > _population;

    virtual void _initializeGenome(boost::shared_ptr<Genome> genome);

    template <class T>
    void _loadParameter(const std::map<std::string, Parameter>& params, const std::string& key,
      T& value)
    {
      std::map<std::string, Parameter>::const_iterator it = params.find(key);
      if (it != params.end())
      {
        value = (T)it->second.value;
      }
    }

    virtual void _mutate(boost::shared_ptr<Genome> genome);

    virtual void _updateScores();

  private:
    /// the very first genome
    boost::shared_ptr<Genome> _seed;
    /// the best genome
    boost::shared_ptr<Genome> _best;
    /// number of new genomes to create at each iteration
    int _freshMeat;
    boost::shared_ptr<FitnessFunction> _fitness;
    bool _initialized;

    int _populationSize;
    /// The number of best genomes to keep at each generation
    int _keepBest;
    /// Probability of mutation after mating, free pass, etc. has occurred
    double _mutationProb;
    double _mutationSeverity;

    HashMap<std::string, double> _allScores;
    HashSet<std::string> _used;

    bool _scoreCaching;

    virtual int _selectMateGaussian();

    virtual int _selectMateRoulette();

    virtual int _selectMateTournament();
  };
}


#endif
