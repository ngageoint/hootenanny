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

#ifndef __GREEDY_STEPWISE_SEARCH_H__
#define __GREEDY_STEPWISE_SEARCH_H__

// Standard Includes
#include <map>
#include <vector>

#include "../SharedPtr.h"
#include "../TgsExport.h"
#include "SearchMethod.h"

namespace Tgs
{
  class TgsProgress;

  /**
   * Greedy Stepwise searches iteratively through the available set of factors.
   * See http://en.wikipedia.org/wiki/Greedy_algorithm for a more detailed description.
   */
  class TGS_EXPORT GreedyStepwiseSearch : public SearchMethod
  {
  public:

    GreedyStepwiseSearch();

    virtual ~GreedyStepwiseSearch() {}

    enum {
      /// Start w/ 0 factors and add the best one iteratively
      Forward,
      /// Start w/ all factors and remove the worst one iteratively
      Backward
    };

    /**
    * Searches for a good subset of factors.
    */
    virtual std::vector<std::string> findSubset(const DataFrame& df, Tgs::TgsProgress* p = NULL);

    /**
     * Set the direction to one of Forward or Backward. Defaults to Backward.
     */
    void setDirection(int dir) { _direction = dir; }

    /**
    * This evaluator will be used to evaluate the fitness of a factor subset.
    */
    virtual void setEvaluator(boost::shared_ptr<FactorSubsetEvaluator> fse) { _fse = fse; }

  private:

    int _direction;
    boost::shared_ptr<FactorSubsetEvaluator> _fse;
    int _iteration;
    int _totalIterations;
    Tgs::TgsProgress* _progress;

    /**
     * Evaluates and removes the worst column from the available columns
     */
    double _removeWorst(std::vector<int>& columns);
  };
}

#endif
