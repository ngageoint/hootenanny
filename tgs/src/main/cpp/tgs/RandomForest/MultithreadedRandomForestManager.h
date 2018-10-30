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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MULTITHREADEDRANDOMFORESTMANAGER_H
#define MULTITHREADEDRANDOMFORESTMANAGER_H

#include <tgs/RandomForest/BaseRandomForestManager.h>
#include <tgs/RandomForest/MultithreadedRandomForest.h>

namespace Tgs
{
  /**
   * @brief The MultithreadedRandomForestManager class wraps the random forest
   * to allow for multithreaded training
   */
  class MultithreadedRandomForestManager : public BaseRandomForestManager
  {
  public:
    /**
     * @brief MultithreadedRandomForestManager Constructor
     */
    MultithreadedRandomForestManager();

    /**
    *  @brief ~MultithreadedRandomForestManager Destructor
    */
    virtual ~MultithreadedRandomForestManager();

  protected:
    /**
     * @brief _initForests initializes the list of random forests
     * @param numForests the number of forests to create
     */
    virtual void _initForests(int numForests);

    /**
     * @brief _parseXmlForestNodes loads the XML form of a fores
     * @param forestsNode the list of <RandomForest> DOM nodes
     */
    virtual void _parseXmlForestNodes(QDomNodeList & forestNodes);

    /**
    *  Trains a set of binary 1 to all random forests on the training vectors previously added
    *
    * @param numTrees the number of random trees to create
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param nodeSize the minimum number of data vectors in a set to split a node
    * @param retrain fraction of top factors to use in retraining model (1.0 means use all factors and no retraining)
    * @param balanced true if the random forests should be balanced
    */
    void _trainBinary(unsigned int numTrees, unsigned int numFactors, unsigned int nodeSize,
      double retrain, bool balanced);

    /**
    *  Trains a single multi-class random forest on the training vectors previously added
    *
    * @param numTrees the number of random trees to create
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param nodeSize the minimum number of data vectors in a set to split a node
    * @param retrain fraction of top factors to use in retraining model (1.0 means use all factors and no retraining)
    * @param balanced true if the random forests should be balanced
    */
    void _trainMultiClass(unsigned int numTrees, unsigned int numFactors, unsigned int nodeSize,
      double retrain, bool balanced);

    /**
    *  Trains a set of pair wise class to class random forests on the training vectors previously added
    *
    * @param numTrees the number of random trees to create
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param nodeSize the minimum number of data vectors in a set to split a node
    * @param retrain fraction of top factors to use in retraining model (1.0 means use all factors and no retraining)
    * @param balanced true if the random forests should be balanced
    */
    void _trainRoundRobin(unsigned int numTrees, unsigned int numFactors, unsigned int nodeSize,
      double retrain, bool balanced);
  };
}

#endif // MULTITHREADEDRANDOMFORESTMANAGER_H
