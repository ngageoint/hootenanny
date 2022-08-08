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

#ifndef __TGS__DATA_FRAME_DISCRETIZER_H__
#define __TGS__DATA_FRAME_DISCRETIZER_H__

// Standard Includes
#include <string>

#include "../HashMap.h"
#include "../RandomForest/DataFrame.h"
#include "../TgsException.h"

namespace Tgs
{
  class TgsProgress;

  /**
   * Uses the FayyadMdlDiscretizer to discretize all numeric values in the data frame.
   * Nulls are hot-deck imputed [1] or put into their own category depending on the null 
   * treatment in the data frame.
   *
   * It may make sense to build a classifier that uses regression to estimate the missing
   * value is rather than replace it with random data. Unfortunately this adds a bit of 
   * time and complexity.
   *
   * [1] Wikipedia, Imputation (statistics) 
   * (http://en.wikipedia.org/wiki/Imputation_%28statistics%29)
   */
  class TGS_EXPORT DataFrameDiscretizer
  {
  public:

    class AllNullsException : public Exception
    {
    public:
      AllNullsException(const std::string& error) : Exception(error) {}
    };

    DataFrameDiscretizer() : _df(NULL) { }

    /**
     * Modifies the specified data frame in place by making all numeric factors nominal.
     */
    void discretize(DataFrame& df, TgsProgress* progress = NULL);

  private:
    DataFrame* _df;

    void _discretizeColumn(int column);

    /**
     * Use imputation (randomly sample from good data) to generate values for the missing data.
     */
    void _replaceNulls(int column);
  };
}

#endif

