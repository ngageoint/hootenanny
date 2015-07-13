/***************************************************************************
* Copyright (c) 2005-2008 by SPADAC Inc. (formerly Spatial Data Analytics Corporation).  All rights reserved.
****************************************************************************/

#ifndef __TREE_NODE_H__
#define __TREE_NODE_H__

//Boost Includes
#include <boost/shared_ptr.hpp>

//STL Includes
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
    boost::shared_ptr<TreeNode> leftChild;  //The right child of a split node else NULL
    boost::shared_ptr<TreeNode> rightChild; //The left child of a split node else NULL

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

