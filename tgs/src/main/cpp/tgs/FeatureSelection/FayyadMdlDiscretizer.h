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

#ifndef __FAYYAD_MDL_DISCRETIZER_H__
#define __FAYYAD_MDL_DISCRETIZER_H__

// Standard Includes
#include <vector>

#include "../TgsExport.h"

namespace Tgs
{
  class FayyadMdlDiscretizerTest;

  /**
   * Given a set of continuous values the Fayyad & Irani MDL method attempts to find an optimal
   * set of split points to discretize the data. AKA Recursive Minimal Entropy Partitioning
   *
   * See Dougherty, Kohavi & Sahami "Supervised and Unsupervised Discretization of Continuous
   * Features" http://www.ifir.edu.ar/~redes/curso/disc.ps for a high level description.
   * See Fayyad & Irani "Multi-Interval Discretization of Continuous-Valued Attributes for
   * Classification Learning" http://hdl.handle.net/2014/35171 for the full description (OCR on
   * this link is particularly bad).
   */
  class TGS_EXPORT FayyadMdlDiscretizer
  {
  public:

    typedef std::vector<int> SplitPoints;

    FayyadMdlDiscretizer();

    virtual ~FayyadMdlDiscretizer() {}

    /**
     * Recursively tries to calculate good split points using the Fayyad & Irani MDL method.
     * @param classes The enumerated class labels associated with the values.
     * @param values A vector of doubles. Passing NaN will result in undefined behavior. The data
     *   _must_ be sorted.
     * @returns The split points. The first value is the upper bound of the first block, the
     *  second value is the upper bound of the second block, etc. There will be size() + 1 blocks
     *  in the result. If the result is empty, splitting the data provides no meaningful
     *  information. E.g. if data has 10 values and the result is { 2, 5, 8 }, then the blocks are
     *  {0 - 2}, {3 - 5}, {6 - 8}, {9}
     */
    SplitPoints calculateSplitPoints(const std::vector<int>& classes,
      const std::vector<double>& values);

  private:

    /// for white box testing.
    friend class FayyadMdlDiscretizerTest;

    const std::vector<int>* _classes;
    const std::vector<double>* _values;
    std::vector<int> _result;

    /**
     * Contains the entropy calculation for the input vector where
     * v[n] = entropy(data[0] to data[n])
     */
    std::vector<double> _entropyCacheUp;
    /**
     * Contains the entropy calculation for the input vector where
     * v[n] = entropy(data[n] to data[data.size() - 1])
     */
    std::vector<double> _entropyCacheDown;

    /**
     * Calculates the delta function as defined
     */
    double _calculateDelta(int start, int size, int split);

    /**
     * Calculate the entropy over the given range and return the result. _calculateEntropyCache()
     * must be called before this is called.
     */
    double _calculateEntropy(int start, int size);

    void _calculateSplits(int start, int size);

    /**
     * Finds the best split point within the given range and returns the result. The value
     * returned is the upper bound of the lower split point.
     * E.g. if start = 0 and end = 10 and the result is 3 the two sets are (0 - 3) and (4 - 10)
     * If there are multiple equal split points the first one is returned. If
     */
    int _findBestSplitPoint(int start, int size, double& informationGain);
  };
}


#endif
