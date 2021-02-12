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

#include "SymmetricUncertaintyCalculator.h"

// Standard Includes
#include <assert.h>
#include <float.h>
#include <iostream>
#include <math.h>
using namespace std;

#include "../RandomForest/DataFrame.h"
#include "../TgsException.h"

namespace Tgs
{
  double log2v = log(2.0);
  inline double log2(const double v) { return log(v) / log2v; }

  double SymmetricUncertaintyCalculator::calculateScore(const DataFrame& df1, int factorIndex1,
    const DataFrame& df2, int factorIndex2)
  {
    return calculateUncertainty(df1, factorIndex1, df2, factorIndex2);
  }

  double SymmetricUncertaintyCalculator::calculateUncertainty(const DataFrame& df1,
    int factorIndex1, const DataFrame& df2, int factorIndex2)
  {
    assert(df1.isNominal(factorIndex1));
    assert(df2.isNominal(factorIndex2));

    double hy = _calculateEntropy(df1, factorIndex1);
    double hyx = _calculateConditionalEntropy(df1, factorIndex1, df2, factorIndex2);

    double gain = hy - hyx;

    double hx = _calculateEntropy(df2, factorIndex2);

    if (hy + hx == 0.0)
    {
      return 1.0;
    }
    double result = 2.0 * (gain / (hy + hx));

    return result;
  }

  double SymmetricUncertaintyCalculator::_calculateConditionalEntropy(const DataFrame& dfY,
    int factorIndexY, const DataFrame& dfX, int factorIndexX)
  {
    CondClassCounts ccc;
    ClassCounts cc;

    for(unsigned int i = 0; i < dfX.getNumDataVectors(); i++)
    {
      double vx = dfX.getDataElement(i, factorIndexX);
      if (DataFrame::isNull(vx) == true)
      {
        throw Tgs::Exception("Null values are not supported by SymmetricUncertaintyCalculator");
      }
      int ex = (int)(vx + 0.5); // x enumeration

      double vy = dfY.getDataElement(i, factorIndexY);
      if (DataFrame::isNull(vy) == true)
      {
        throw Tgs::Exception("Null values are not supported by SymmetricUncertaintyCalculator");
      }
      int ey = (int)(vy + 0.5); // y enumeration

      ccc[ex][ey]++;
      cc[ex]++;
    }

    double sumX = 0.0;
    double totalSize = dfX.getNumDataVectors();
    for (CondClassCounts::const_iterator condIt = ccc.begin(); condIt != ccc.end(); ++condIt)
    {
      const ClassCounts& classCounts = condIt->second;
      double px = (double)cc[condIt->first] / totalSize; // p(x)
      double sumY = 0.0;
      for (ClassCounts::const_iterator classIt = classCounts.begin();
        classIt != classCounts.end(); ++classIt)
      {
        double count = classIt->second;
        double pyx = count / (double)cc[condIt->first]; // p(y | x)
        sumY += pyx * log2(pyx);
      }
      sumX += px * sumY;
    }

    return -sumX;
  }

  double SymmetricUncertaintyCalculator::_calculateEntropy(const DataFrame& df, int factorIndex)
  {
    ClassCounts cc;

    for(unsigned int i = 0; i < df.getNumDataVectors(); i++)
    {
      double v = df.getDataElement(i, factorIndex);
      // null values are not supported Use the DataFrameDiscretizer to "fix" nulls
      if (DataFrame::isNull(v) == true)
      {
        throw Tgs::Exception("Null values are not supported by SymmetricUncertaintyCalculator");
      }
      cc[(int)(v + .5)]++;
    }

    double sum = 0.0;
    double totalSize = df.getNumDataVectors();
    for (ClassCounts::const_iterator classIt = cc.begin(); classIt != cc.end(); ++classIt)
    {
      double count = classIt->second;
      sum += count / totalSize * log2(count / totalSize);
    }

    return -sum;
  }

}
