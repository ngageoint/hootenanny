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

#ifndef __TGS__DATA_FRAME_SUBSAMPLER_H__
#define __TGS__DATA_FRAME_SUBSAMPLER_H__

// Standard Includes
#include <string>

#include "../HashMap.h"
#include "../RandomForest/DataFrame.h"

namespace Tgs
{
  class TgsProgress;

  /**
   * 
   */
  class TGS_EXPORT DataFrameSubsampler
  {
  public:

    /**
     * Modifies the specified data frame in place by making all numeric factors nominal.
     * @param p Portion of values to keep. A value from 0 to 1.
     */
    void subsample(DataFrame& df, double p, TgsProgress* progress = NULL);

  private: 
    DataFrame* _df;
  };
}

#endif

