#ifndef MISSINGDATAHANDLER_H
#define MISSINGDATAHANDLER_H

#include "DataFrame.h"

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
      boost::shared_ptr<DataFrame> data);

  private:

    /**
     * @brief _replaceMissingNominalValuesFast
     * @param data the data frame
     * @param factorIndex the index of the factor of interest
     */
    static void _replaceMissingNominalValuesFast(double missingDataValue,
      boost::shared_ptr<DataFrame> data, unsigned int factorIndex);

    /**
     * @brief _replaceMissingNumericValuesFast
     * @param data the data frame
     * @param factorIndex the index of the factor of interest
     */
    static void _replaceMissingNumericValuesFast(double missingDataValue,
      boost::shared_ptr<DataFrame> data, unsigned int factorIndex);


  };
} //End namespace

#endif // MISSINGDATAHANDLER_H
