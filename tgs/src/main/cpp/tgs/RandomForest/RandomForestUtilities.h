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
#ifndef RANDOMFORESTUTILITIES_H
#define RANDOMFORESTUTILITIES_H

//Std Includes
#include <map>
#include <string>
#include <vector>

#include <tgs/RandomForest/RandomForestManager.h>
#include <tgs/RandomForest/MultithreadedRandomForestManager.h>

namespace Tgs
{
  struct RandomForestInputs
  {
    std::vector<std::string> featureLabels;  //The feature labels matching each element in _dataVectors
    std::vector<std::string> classLabels;  //The ordered class labels
    std::map<std::string, std::vector<std::vector<double>>> trainingVectors; //The data for each training vector
  };

  class RandomForestUtilities
  {
  public:

    /**
     * @brief createDataSets takes the input data and splits it by classes into output indices
     *
     * If you want one the first set with 30% and the rest in the second then splitPercentages would contain 0.3 and 1.0.
     * A 30%, 40%, 30% split would be 0.3, 0.7 and 1.0.
     *
     * @param rfInputs the struct of training data
     * @param splitPercentages the percentages values for splitting data (should be size of number of splits and final value should be 1.0)
     * @return sets of data vector indices by classes
     */
    static std::vector<std::map<std::string, std::pair<unsigned int, unsigned int>>> createDataSets(
      RandomForestInputs rfInputs, std::vector<double> splitPercentages);

    /**
     * @brief generateModel generates a random forest model
     * @param rfInputs the struct of training data
     * @param numTrees the number of trees in the model
     * @param trainingRangeMap the map of classNames to indices within the rfInput trainingVectors to use for training
     * @return the trained random forest model
     */
    static RandomForestManager generateModel(RandomForestInputs & rfInputs, unsigned int numTrees,
      std::map<std::string, std::pair<unsigned int, unsigned int>> & trainingRangeMap);

    /**
     * @brief generateModel generates a multithreaded random forest model
     * @param rfInputs the struct of training data
     * @param numTrees the number of trees in the model
     * @param trainingRangeMap the map of classNames to indices within the rfInput trainingVectors to use for training
     * @return the trained random forest model
     */
    static MultithreadedRandomForestManager generateMultithreadModel(RandomForestInputs & rfInputs,
      unsigned int numTrees,
      std::map<std::string, std::pair<unsigned int, unsigned int>> & trainingRangeMap);

    /**
     * @brief generateTrainingDataFromFile creates a random forest from a file containing labeled training data
     * @param filename the path to the target file
     * @return a set of training vectors
     */
    static RandomForestInputs generateTrainingDataFromFile(std::string filename);

  private:
    /**
     * @brief _generateTrainingDataFromAARFFile reads a WEKA AARF file
     * @param filename the name of the file
     * @return a set of training vectors
     */
    static RandomForestInputs _generateTrainingDataFromAARFFile(std::string filename);
  };
}


#endif // RANDOMFORESTUTILITIES_H
