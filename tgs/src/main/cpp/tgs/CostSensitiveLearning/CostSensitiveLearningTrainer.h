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
#ifndef COSTSENSITIVELEARNINGTRAINER_H
#define COSTSENSITIVELEARNINGTRAINER_H

//Boost Includes

#include "../RandomForest/MultithreadedRandomForestManager.h"
#include "../RandomForest/RandomForestUtilities.h"

namespace Tgs
{
  /**
   * @brief The CostSensitiveLearningTrainer class
   *
   * Perform Cost Sensitive Learning for Random Forest
   */
  class CostSensitiveLearningTrainer
  {
  public:
    /**
     * @brief cslCrossValidation rotates using 1/3 of the data for intial training, 1/3 for training CSL and remainder for validation of CSL
     * @param filename the file containing the labelled data
     * @param numTrees the number of trees in the model
     * @param reportName an optional name to apply to the output reports
     */
    static void cslCrossValidation(std::string filename, unsigned int numTrees,
      std::vector<float> & costMatrix, std::string reportName = "");

    /**
     * @brief trainCslModel trains a cost sensitive learning random forest model
     *
     *
     * @param rfInputs a structure of training data
     * @param numTrees the number of trees in the random forest
     * @param costMatrix a row-major square cost matrix
     * @param initialTrainingRanges the map of ranges of training data by class for initial model where the first of the pair is the start index and the second is the range size
     * @param cslTrainingRanges the map of ranges of training data by class for CSL model where the first of the pair is the start index and the second is the range size
     * @param reportsBasename the name prepended to optional reports for the initial model (default = "" which does not generate reports)
     * @return a trained CSL random forest manager
     */
    static MultithreadedRandomForestManager trainCslModel(RandomForestInputs & rfInputs,
      unsigned int numTrees, const std::vector<float> & costMatrix,
      std::map<std::string, std::pair<unsigned int, unsigned int> > & initialTrainingRanges,
      std::map<std::string, std::pair<unsigned int, unsigned int> > & cslTrainingRanges,
      std::string reportsBasename = "");

    /**
     * @brief trainCslModel trains a cost sensitive learning random forest model
     * @param initialModel a previously trained model
     * @param rfInputs a structure of training data
     * @param numTrees the number of trees in the random forest
     * @param costMatrix a row-major square cost matrix
     * @param cslTrainingRanges the map of ranges of training data by class for CSL model where the first of the pair is the start index and the second is the range size
     * @return the CSL trained model
     */
    static MultithreadedRandomForestManager trainCslModel(
      MultithreadedRandomForestManager & initialModel,
      RandomForestInputs & rfInputs, unsigned int numTrees, const std::vector<float> & costMatrix,
      std::map<std::string, std::pair<unsigned int, unsigned int> > & cslTrainingRanges,
      std::string reportsBasename = "");
  };
}


#endif // COSTSENSITIVELEARNINGTRAINER_H
