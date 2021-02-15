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
 * @copyright Copyright (C) 2015, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __RANDOM_FOREST_H__
#define __RANDOM_FOREST_H__

//STL Includes
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

//Urgent Includes
#include <tgs/TgsExport.h>
#include <tgs/RandomForest/BaseRandomForest.h>
#include <tgs/RandomForest/DataFrame.h>
#include <tgs/RandomForest/RandomTree.h>

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
  * Default Constructor/Destructor
  */
  RandomForest() = default;
  ~RandomForest() = default;

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
  virtual void trainBinary(const std::shared_ptr<DataFrame>& data, unsigned int numTrees,
    unsigned int numFactors, std::string posClass, unsigned int nodeSize = 1,
    double retrain = 1.0, bool balanced = false) override;

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
  virtual void trainMulticlass(const std::shared_ptr<DataFrame>& data, unsigned int numTrees,
    unsigned int numFactors, unsigned int nodeSize = 1, double retrain = 1.0,
    bool balanced = false) override;

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
  virtual void trainRoundRobin(const std::shared_ptr<DataFrame>& data, unsigned int numTrees,
    unsigned int numFactors, std::string posClass, std::string negClass,
    unsigned int nodeSize = 1, double retrain = 1.0, bool balanced = false) override;

};

}  //End namespace
#endif
