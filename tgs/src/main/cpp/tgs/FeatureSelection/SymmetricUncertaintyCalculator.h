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

#ifndef __TGS__SYMMETRIC_UNCERTAINTY_CALCULATOR_H__
#define __TGS__SYMMETRIC_UNCERTAINTY_CALCULATOR_H__

// Standard Includes
#include <string>

#include "../HashMap.h"
#include "../TgsExport.h"
#include "FeatureScoreCalculator.h"

namespace Tgs
{
  class DataFrame;

  /**
   * Calculates the symmetric uncertainty as described in [1] section 4.2.1
   *
   * [1] M. Hall 1999, Correlation-based Feature Selection for Machine Learning
   * (http://www.cs.waikato.ac.nz/~mhall/thesis.pdf)
   * [2] Wikipedia, Conditional Entropy (http://en.wikipedia.org/wiki/Conditional_entropy)
   * [3] Wikipedia, Information Entropy (http://en.wikipedia.org/wiki/Information_entropy)
   */
  class TGS_EXPORT SymmetricUncertaintyCalculator : public FeatureScoreCalculator
  {
  public:

    SymmetricUncertaintyCalculator() {}

    virtual ~SymmetricUncertaintyCalculator() {}

    virtual double calculateScore(const DataFrame& df1, int factorIndex1,
      const DataFrame& df2, int factorIndex2);

    double calculateUncertainty(const DataFrame& df1, int factorIndex1,
      const DataFrame& df2, int factorIndex2);

  protected:
    typedef HashMap<int, int> ClassCounts;
    typedef HashMap<int, ClassCounts > CondClassCounts;

    /**
     * See [3] and [1] section 4.2.1
     */
    double _calculateEntropy(const DataFrame& df, int factorIndex);

    /**
     * See [2] and [1] section 4.2.1
     */
    double _calculateConditionalEntropy(const DataFrame& dfY, int factorIndexY,
      const DataFrame& dfX, int factorIndexX);

  };
}

#endif

