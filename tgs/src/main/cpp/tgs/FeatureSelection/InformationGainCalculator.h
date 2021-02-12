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
 * @copyright Copyright (C) 2015, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__INFORMATION_GAIN_CALCULATOR_H__
#define __TGS__INFORMATION_GAIN_CALCULATOR_H__

// Standard Includes
#include <string>

#include "../HashMap.h"
#include "../TgsExport.h"
#include "SymmetricUncertaintyCalculator.h"

namespace Tgs
{
  class DataFrame;

  /**
   * Calculates the information gain relative to the class.
   * InfoGain(class, attribute) = H(Class) - H(Class | Attribute)
   * 
   * [1] Wikipedia, Information Gain (http://en.wikipedia.org/wiki/Information_gain)
   * [2] Wikipedia, Information Entropy (http://en.wikipedia.org/wiki/Information_entropy)
   */
  class TGS_EXPORT InformationGainCalculator : private SymmetricUncertaintyCalculator
  {
  public:

    /**
     * All values must be nominal w/ no nulls
     */
    double calculateInformationGain(const DataFrame& df1, int factorIndex1, 
      const DataFrame& df2, int factorIndex2);

    virtual double calculateScore(const DataFrame& df1, int factorIndex1, 
      const DataFrame& df2, int factorIndex2);

  };
}

#endif

