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

#ifndef __CONSISTENCY_SUBSET_EVALUATOR_H__
#define __CONSISTENCY_SUBSET_EVALUATOR_H__

// Standard Includes
#include <vector>

#include "../HashMap.h"
#include "../TgsExport.h"
#include "../RandomForest/DataFrame.h"
#include "FactorSubsetEvaluator.h"

namespace Tgs
{
  class ConsistencySubsetEvaluatorTest;

  /**
   * Evaluates a set of features for inconsistency. The smallest set of factors with the same
   * inconsistency count as all the features is used.
   *
   * Zhao & Liu suggest using Consistency-based feature selection with a backward elimination
   * search routine (INTERACT), however this is a more flexible but less efficient implementation
   * that allows arbitrary search routines.
   *
   * See also Zhao, Liu, Searching for Interacting Features for a technical description
   * http://www.ijcai.org/papers07/Papers/IJCAI07-187.pdf (Section 2 Interaction and Data
   * Consistency is most relevant)
   *
   * See also wikipedia http://en.wikipedia.org/wiki/Feature_selection for a general overview of
   * feature selection.
   */
  class TGS_EXPORT ConsistencySubsetEvaluator : public FactorSubsetEvaluator
  {
  public:

    ConsistencySubsetEvaluator();

    virtual ~ConsistencySubsetEvaluator() {}

    /**
     * Evaluates the columns in the data frame and returns -inconsistency rate. Zero is the best
     * possible result. This function is _not_ re-entrant.
     * @param columns a vector of ints that refer to the columns in the data frame
     * @returns the quality of the data frame (bigger is better).
     */
    virtual double evaluateSubset(const std::vector<int>& columns,
      TgsProgress* progress = NULL);

    virtual void setDataFrame(const DataFrame& dataFrame, TgsProgress* progress = NULL);

  private:

    // for white box testing
    friend class ConsistencySubsetEvaluatorTest;

    /// defines a cell in feature space (if you have n features, the CellId has size n)
    typedef std::vector<int> CellId;
    typedef std::vector<int> InconsistentInstancesMap;
    DataFrame _dataFrame;

    /// Contains the bin that each data element falls in based on the column splits. row-major
    std::vector< std::vector<int> > _bins;

    /**
     * Maps class names to the associated enumeration
     */
    HashMap<std::string, int> _enumMap;
    int _enumCnt;

    /**
     * Calculates the bin that a particular value falls in for a given column.
     */
    int _calculateBin(double v, int column);

    void _calculateColumnSplit(int column);

    /**
     * Calculates the inconsistency count for an Inconsistent Instances Set. This boils down to
     * classes.size() - maxCountOfSingleEnumeration(classes)
     * Zhao & Liu give a much more complicated definition (def 4) in their paper if you're
     * interested.
     */
    int _calculateInconsistentCount(const InconsistentInstancesMap& classes) const;

    /**
     * Calculates the inconsistency rate of the provided data frame. This is essentially the sum
     * of all inconsistency counts divided by the number of data points.
     */
    double _calculateInconsistencyRate() { return -1; }

    void _putDataIntoBins();
  };
}


#endif
