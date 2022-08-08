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
 * @copyright Copyright (C) 2015, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__FEATURE_SCORE_CALCULATOR_H__
#define __TGS__FEATURE_SCORE_CALCULATOR_H__

#include "../TgsExport.h"

namespace Tgs
{
  class DataFrame;

  /**
   * Calculates the score for a given feature
   */
  class TGS_EXPORT FeatureScoreCalculator
  {
  public:

    FeatureScoreCalculator() {};

    virtual ~FeatureScoreCalculator() {};

    /**
     * The score calculated must be non-negative and a higher score is better. Zero implies that
     * there is no information in the given factor.
     */
    virtual double calculateScore(const DataFrame& df1, int factorIndex1, 
      const DataFrame& df2, int factorIndex2) = 0;
  };
}

#endif

