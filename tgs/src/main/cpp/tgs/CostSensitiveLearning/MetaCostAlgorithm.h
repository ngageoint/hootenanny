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
#ifndef METACOSTALGORITHM_H
#define METACOSTALGORITHM_H

//Std Includes
#include <vector>
#include <string>

namespace Tgs
{
  /**
   * @brief The MetaCostAlgorithm class
   *
   * Implements the MetaCost algorithm for cost sensitive learning
   *
   * http://homes.cs.washington.edu/~pedrod/papers/kdd99.pdf
   *
   *
   * It uses a matrix containing the misclassification costs
   * where C(i,j) is the cost of predicting class j when it is actually Ci.
   *
   * The matrix is of the row major form below. In this example the cost
   * of predicting class j = 1 when it is actually i = 2 or C(2,1) = 20
   *           match miss  review
   *           C0i    C1i   C2i
   * match  C0j  0      5    10
   * miss   C1j 15      0    20
   * review C2j 25     30     0
   */
  class MetaCostAlgorithm
  {
  public:
    /**
     * @brief MetaCostAlgorithm constructor
     */
    MetaCostAlgorithm();

    /**
     * @brief generateResults generates the results of object reclassification
     * @param basename name to prepend to report name
     */
    void generateResults(std::string basename);

    /**
     * @brief initialize initializes the algorithm
     * @param classLabels set the class labels which correspond to the costMatrix entries and the input vectors
     * @param costMatrix the row major matrix containing the costs of predicting class i when it is actually in class j
     */
    void initialize(std::vector<std::string> classLabels, std::vector<float> costMatrix);

    /**
     * @brief reclassifyVector provides the minimum risk class label to the input vector
     * @param probabilityVector the vector containing the probabilities with the class labels corresponding to the initialized class labels
     * @return the minimum risk class label
     */
    std::string reclassifyVector(std::vector<double> probabilityVector, bool saveResults = false);

  private:
    std::vector<std::string> _classLabels;
    std::vector<float> _costMatrix;
    bool _initialized;
    std::vector<std::vector<float> > _riskValues;
    std::vector<int> _minRiskLabels;


  };
} //End namespace
#endif // METACOSTALGORITHM_H
