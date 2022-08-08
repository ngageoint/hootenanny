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
 * @copyright Copyright (C) 2015, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef __TREE_NODE_H__
#define __TREE_NODE_H__

//STL Includes
#include <memory>
#include <vector>

namespace Tgs
{
  /**
  *  The TreeNode is part of an implementation of the Leo Breiman
  * Random Forest Classification algorithm.  This is a single node
  * in a RandomTree within a RandomForest.
  *
  * For more information see:
  * http://oz.berkeley.edu/~breiman/RandomForests/
  */
  struct TreeNode
  {
    std::shared_ptr<TreeNode> leftChild;  //The right child of a split node else NULL
    std::shared_ptr<TreeNode> rightChild; //The left child of a split node else NULL

    double splitValue;      //The factor value used to split a node
    double purityDelta;     //For a split node the information gain improvement derived from splitting the data by a factor
    std::string classLabel;     //For a pure node its classification label
    unsigned int factorIndex;   //The factor index for a split node
    std::vector<unsigned int> dataList;  //Pure: the indices for training vectors classified to this node
    bool isPure;            //True if the node is a leaf
    unsigned int nodeId;    //Split: 0,  Pure: the unique pure node id used for proximity calculations
    double rangeMin;        //Split: The lower interquartile value based on the bandwidth of the training data for the factor, Pure: 0
    double rangeMax;        //Split: The upperr interquartile value based on the bandwidth of the training data for the factor, Pure: 0
  };
}  //End Namespace
#endif

