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
 * @copyright Copyright (C) 2015, 2018, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __INFO_GAIN_CALCULATOR_H__
#define __INFO_GAIN_CALCULATOR_H__

#include <tgs/TgsExport.h>
#include <tgs/RandomForest/DataFrame.h>

namespace Tgs
{
  /**
  *  This class handles all the information gain calculations
  * to find entropy with a dataset or subsets of the data
  */
  class TGS_EXPORT InfoGainCalculator
  {
  public:
    /**
    *  Constructor
    */
    InfoGainCalculator() = default;

    /**
    *  Destructor
    */
    ~InfoGainCalculator() = default;

    /**
    *  Computes the entropy of a dataset based only on the class label
    *
    *  @param df the source data set
    *  @param indices a list of data vector indices in the data set
    */
    double computeEntropyByClass(DataFrame & df, std::vector<unsigned int> & indices) const;

    /**
    * Using the list of data vector indices as input this method
    * uses max information gain to find the best split resulting in
    * the best information gain.
    *
    * @param df the source data set
    * @param fIndices the list of factor indices to consider for this data split
    * @param dIndices the input data vectors indices subset sorted on the factor fIdx
    * @param splitIdx an output variable to hold the split index in indices
    * @param fIdx an output variable to hold the factor index to split on
    * @param splitVal an output variable to hold the factor value to split on
    * @param purityDelta the contribution of the data split in increasing the info gain
    * @return false if a split is not possible (all factor values are same across all factors)
    */
    bool findDataSplit(DataFrame & df, std::vector<unsigned int> & fIndices, 
      std::vector<unsigned int> & dIndices, unsigned int & splitIdx, unsigned int & fIdx,
      double & splitVal, double & purityDelta);

    /**
    * This method looks at a single factor and computes the maximum gain against
    * all possible data splits and also provides the index for the best split
    *
    * @param df the source data set
    * @param indices a subset of data vectors by their index value in the dataset sorted
    * on the factor value specified by fIdx
    * @param fIdx the factor index in the data vectors
    * @param totalEntropy entropy for the subset of data referenced by indices
    * @param bestSplit the index for the split in indices corresponding to the max gain
    * @return the information gain corresponding to bestSplit
    */
    double getMaxInfoGainByFactor(DataFrame & df, std::vector<unsigned int> & indices, 
      unsigned int fIdx, double totalEntropy, unsigned int & bestSplit) const;

    /**
    * This method looks at a single factor and computes the maximum gain ratio against
    * all possible data splits and also provides the index for the best split
    *
    * @param df the source data set
    * @param indices a subset of data vectors by their index value in the dataset sorted
    * on the factor value specified by fIdx
    * @param fIdx the factor index in the data vectors
    * @param totalEntropy entropy for the subset of data referenced by indices
    * @param bestSplit the index for the split in indices corresponding to the max gain
    * @return the information gain ratio corresponding to bestSplit
    */
    double getMaxInfoGainRatioByFactor(DataFrame & df, std::vector<unsigned int> & indices, 
      unsigned int fIdx, double totalEntropy, unsigned int & bestSplit);

  private:
    /**
    *  Computes the entropy function -n * log2(n)
    */
    double _calcLogFunc(double n) const;

    /**
    * This methods takes a subset of data vectors held by indices and finds
    * a list of candidate splits based on indices sorted on the factor value
    * indexed by fIdx
    *
    * @param df the source data set
    * @param indices a subset of data vectors by their index value in the dataset 
    * @param fIdx the factor index in the data vectors
    * @param splits the potential splits in the data to consider 
    */
    void _findCandidateSplits(DataFrame & df, std::vector<unsigned int> & indices, 
      unsigned int fIdx, std::vector<unsigned int> & splits) const;

  };
}  //End Namespace
#endif 
