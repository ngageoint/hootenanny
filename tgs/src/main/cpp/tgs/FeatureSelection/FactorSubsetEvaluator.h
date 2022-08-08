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

#ifndef __FACTOR_SUBSET_EVALUATOR_H__
#define __FACTOR_SUBSET_EVALUATOR_H__

#include "../TgsExport.h"

namespace Tgs
{
  class DataFrame;
  class TgsProgress;

  /**
  * Factor Evaluators evaluate a single factor based solely on the class/RFD information.
  */
  class TGS_EXPORT FactorSubsetEvaluator
  {
  public:

    virtual ~FactorSubsetEvaluator() {}

    /**
     * Evaluates the specified set of columns in the data frame and returns a value that
     * describes how "good" the data frame is. Higher values are better.
     * @returns the quality of the data frame.
     */
    virtual double evaluateSubset(const std::vector<int>& columns,
      TgsProgress* progress = NULL) = 0;

    /**
     * Specifies the data frame on which to operate. The data frame should not be modified after
     * it is set.
     */
    virtual void setDataFrame(const DataFrame& dataFrame, TgsProgress* progress = NULL) = 0;

  };
}

#endif
