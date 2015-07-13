/***************************************************************************
* Copyright (c) 2005-2008 by SPADAC Inc. (formerly Spatial Data Analytics Corporation).  All rights reserved.
****************************************************************************/

#ifndef __RANDOM_FOREST_H__
#define __RANDOM_FOREST_H__

//STL Includes
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

//Urgent Includes
#include "../TgsExport.h"
#include "BaseRandomForest.h"
#include "DataFrame.h"
#include "RandomTree.h"

namespace Tgs
{
  /**
  *  The RandomForest is an implementation of the Leo Breiman
  * Random Forest Classification algorithm
  *
  * For more information see:
  * http://oz.berkeley.edu/~breiman/RandomForests/
  */
  class TGS_EXPORT RandomForest : public BaseRandomForest
  {
  public:
    /**
    * Constructor
    */
    RandomForest();

    /**
    * Destructor
    */
    ~RandomForest();

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
}  //End namespace
#endif


