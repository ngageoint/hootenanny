#ifndef MULTITHREADEDRANDOMFOREST_H
#define MULTITHREADEDRANDOMFOREST_H

//Qt Includes
#include <QDomElement>

#include "BaseRandomForest.h"

namespace Tgs
{


  /**
   * @brief The MultithreadedRandomForest class
   */
  class MultithreadedRandomForest : public BaseRandomForest
  {
  public:
    /**
     * @brief MultithreadedRandomForest Constructor
     */
    MultithreadedRandomForest();

    /**
     * @brief ~MultithreadedRandomForest destructor
     */
    virtual ~MultithreadedRandomForest(){}

    /**
    * Build the forest from a data set
    *
    * @param data the data set to train on
    * @param numTrees the number of random trees to create
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param posClass the name of the positive class
    * @param nodeSize the minimum number of data vectors in a set to split a node
    * @param retrain fraction of top factors to use in retraining model (1.0 means use all factors and no retraining)
    * @param balanced true if the forest will be balanced
    */
    void trainBinary(boost::shared_ptr<DataFrame> data, unsigned int numTrees,
      unsigned int numFactors, std::string posClass, unsigned int nodeSize = 1,
      double retrain = 1.0, bool balanced = false);

    /**
    * Build the forest from a data set
    *
    * @param data the data set to train on
    * @param numTrees the number of random trees to create
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param nodeSize the minimum number of data vectors in a set to split a node
    * @param retrain fraction of top factors to use in retraining model (1.0 means use all factors and no retraining)
    * @param balanced true if the forest will be balanced
    */
    void trainMulticlass(boost::shared_ptr<DataFrame> data, unsigned int numTrees,
      unsigned int numFactors, unsigned int nodeSize = 1, double retrain = 1.0,
      bool balanced = false);

    /**
     * @brief trainMulticlass the map class to train a tree
     * @param tree the input tree
     * @return the trained tree
     */
    static boost::shared_ptr<RandomTree> train(const boost::shared_ptr<RandomTree> & tree);

    /**
    * Build the forest from a data set
    *
    * @param data the data set to train on
    * @param numTrees the number of random trees to create
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param posClass the name of the positive class
    * @param negClass the name of the negative class
    * @param nodeSize the minimum number of data vectors in a set to split a node
    * @param retrain fraction of top factors to use in retraining model (1.0 means use all factors and no retraining)
    * @param balanced true if the forest will be balanced
    */
    void trainRoundRobin(boost::shared_ptr<DataFrame> data, unsigned int numTrees,
      unsigned int numFactors, std::string posClass, std::string negClass,
      unsigned int nodeSize = 1, double retrain = 1.0, bool balanced = false);
  private:


  };
}


#endif // MULTITHREADEDRANDOMFOREST_H
