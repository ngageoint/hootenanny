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
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
 */

#include "InformationGainCalculator.h"

// Standard Includes
#include <assert.h>
#include <float.h>
#include <iostream>
#include <math.h>
using namespace std;

#include "../RandomForest/DataFrame.h"

namespace Tgs
{
  static double log2v = log(2.0);
  inline double log2(const double v) { return log(v) / log2v; }

  double InformationGainCalculator::calculateInformationGain(const DataFrame& df1, 
    int factorIndex1, const DataFrame& df2, int factorIndex2)
  {
    assert(df1.isNominal(factorIndex1));
    assert(df2.isNominal(factorIndex2));

    double hy = _calculateEntropy(df1, factorIndex1);
    double hyx = _calculateConditionalEntropy(df1, factorIndex1, df2, factorIndex2);

    double gain = hy - hyx;

    return gain;
  }  

  double InformationGainCalculator::calculateScore(const DataFrame& df1, int factorIndex1, 
    const DataFrame& df2, int factorIndex2) 
  { 
    return calculateUncertainty(df1, factorIndex1, df2, factorIndex2); 
  }
}

