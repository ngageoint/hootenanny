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
 * @copyright Copyright (C) 2015, 2018, 2019 Maxar (http://www.maxar.com/)
 */
#ifndef MISSINGDATAHANDLER_H
#define MISSINGDATAHANDLER_H

#include <tgs/RandomForest/DataFrame.h>

namespace Tgs
{
  /**
   * @brief The MissingDataHandler class resolves missing values within a DataFrame
   */
  class MissingDataHandler
  {
  public:
    /**
     * @brief replaceMissingValuesFast
     *
     * Numeric missing feature values are replaced with the median value of that
     * feature of other objects in their class while categorical missing values
     * are replaced with the most frequent category value
     *
     * @param missingDataValue the value representing missing data
     * @param data the input data set object
     */
    static void replaceMissingValuesFast(double missingDataValue,
      std::shared_ptr<DataFrame> data);

  private:

    /**
     * @brief _replaceMissingNominalValuesFast
     * @param data the data frame
     * @param factorIndex the index of the factor of interest
     */
    static void _replaceMissingNominalValuesFast(double missingDataValue,
      std::shared_ptr<DataFrame> data, unsigned int factorIndex);

    /**
     * @brief _replaceMissingNumericValuesFast
     * @param data the data frame
     * @param factorIndex the index of the factor of interest
     */
    static void _replaceMissingNumericValuesFast(double missingDataValue,
      std::shared_ptr<DataFrame> data, unsigned int factorIndex);


  };
} //End namespace

#endif // MISSINGDATAHANDLER_H
