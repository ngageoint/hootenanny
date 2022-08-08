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

#ifndef __TGS__CFS_SUBSET_EVALUATOR_H__
#define __TGS__CFS_SUBSET_EVALUATOR_H__

// Standard Includes
#include <vector>

#include "../HashMap.h"
#include "../RandomForest/DataFrame.h"
#include "../TgsExport.h"
#include "FactorSubsetEvaluator.h"

namespace Tgs
{
  class CfsSubsetEvaluatorTest;

  /**
   * Evaluates a set of factors for fitness by maximizing correlation with the class and
   * reducing intra-factor correlation. See [1] for more details.
   *
   * [1] M. Hall 1999, Correlation-based Feature Selection for Machine Learning
   * (http://www.cs.waikato.ac.nz/~mhall/thesis.pdf)
   */
  class TGS_EXPORT CfsSubsetEvaluator : public FactorSubsetEvaluator
  {
  public:

    virtual ~CfsSubsetEvaluator() {}

    /**
    * Evaluates the columns in the data frame and returns a correlation score.
    * @param columns a vector of ints that refer to the columns in the data frame
    * @returns the quality of the data frame (bigger is better). Zero should be the lower bound.
    */
    virtual double evaluateSubset(const std::vector<int>& columns,
      TgsProgress* progress = NULL);

    virtual void setDataFrame(const DataFrame& dataFrame, TgsProgress* progress = NULL);

  private:

    // for white box testing
    friend class CfsSubsetEvaluatorTest;

    DataFrame _df;

    std::vector<double> _classCorr;
    std::vector< std::vector<double> > _corrMatrix;

    void _calculateCorrelationMatrix(TgsProgress* progress);

    void _calculateClassCorrelations(TgsProgress* progress);
  };
}


#endif
