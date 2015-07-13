#ifndef MULTITHREADEDRANDOMFORESTMANAGER_H
#define MULTITHREADEDRANDOMFORESTMANAGER_H

#include "BaseRandomForestManager.h"
#include "MultithreadedRandomForest.h"

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
