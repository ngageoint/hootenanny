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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef __RANDOM_TREE_H__
#define __RANDOM_TREE_H__

//Qt Includes
#include <QDomElement>

//STL Includes
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../TgsException.h"
#include "DataFrame.h"
#include "InfoGainCalculator.h"
#include "TreeNode.h"

namespace Tgs
{
  
  /**
  *  The RandomTree is an implementation of the Leo Breiman
  * Random Forest Classification algorithm.  It represents
  * a single tree in the forest of classifiers
  *
  * For more information see:
  * http://oz.berkeley.edu/~breiman/RandomForests/
  */
  class TGS_EXPORT RandomTree
  {
  public:
    /**
    *  Constructor
    */
    RandomTree();

    /**
    *  Destructor
    */
    ~RandomTree();

    /**
    * Builds a tree without bootstrapping or out of bagging to generate a 
    * tree for test validation
    *
    * @param data this data frame contains all the data vectors in the training set
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    */
    void buildTest(boost::shared_ptr<DataFrame> data, unsigned int numFactors);

    /**
    *  This classifies a data vector against the trained tree
    *
    *  @param dataVector a data vector of the same size as the training vectors
    *  @param outputClass a string to hold the trees classification
    *  @return the id number for the terminal node in the tree for the classification 
    *
    */
    unsigned int classifyDataVector(const std::vector<double> & dataVector, 
      std::string & outputClass) const;

    /**
    *  Classifies the vectors in the out of bag set and computes the error rate as
    *  the number incorrectly classified / total classified
    *
    * @param data this data frame contains all the data vectors in the training set
    */
    double computeErrorRate(boost::shared_ptr<DataFrame> data);

    /**
    * Computes the proximity of the data vectors in the data set by running the 
    * complete data set through the tree and then tracking which vectors
    * were classified to the same node
    * 
    * @param data the set of data vectors
    * @param proximity a n x n (where n is the number of total data vectors) adjacency matrix
    */
    void findProximity(boost::shared_ptr<DataFrame> data, std::vector<unsigned int> & proximity);

    /**
    * Finds the factor importance as the sum of the info gain at each tree node per factor
    *
    * @param purityDeltaSum a map of a factor's index to it info gain sum 
    */
    void getFactorImportance(std::map<unsigned int, double> & purityDeltaSum);

    /**
    *  Exports a tree to the file stream
    *
    * @param fileStream the stream to export the data
    * @param tabDepth set of tabs to indent the exported content
    */
    void exportTree(std::ostream & fileStream, std::string tabDepth);

    /**
     * @brief exportTree exports the random tree to an XML doc
     * @param modelDoc the source document
     * @param parentNode the node to append tree to
     */
    void exportTree(QDomDocument & modelDoc, QDomElement & parentNode);

    /**
    *  Imports a tree from the file stream
    *
    * @param fileStream the stream to import the data
    */
    void import(std::istream & fileStream);

    /**
     * @brief import imports a tree
     * @param e the XML DOM element containing the tree
     */
    void import(QDomElement & e);

    /**
    * Builds a tree 
    *
    * @param data this data frame contains all the data vectors in the training set
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param posClass the class name for the positive class
    * @param nodeSize the minimum number of data vectors in a set to split a node 
    * @param balanced true if the tree should used balanced data
    */
    void trainBinary(boost::shared_ptr<DataFrame> data, unsigned int numFactors, std::string posClass,
      unsigned int nodeSize = 1, bool balanced = false);

    /**
    * Builds a tree 
    *
    * @param data this data frame contains all the data vectors in the training set
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param nodeSize the minimum number of data vectors in a set to split a node 
    * @param balanced true if the tree should used balanced data
    */
    void trainMulticlass(boost::shared_ptr<DataFrame> data, unsigned int numFactors,
      unsigned int nodeSize = 1, bool balanced = false);

    /**
    * Builds a tree 
    *
    * @param data this data frame contains all the data vectors in the training set
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param posClass the class name for the positive class
    * @param negClass the class name for the negative class
    * @param nodeSize the minimum number of data vectors in a set to split a node 
    * @param balanced true if the tree should used balanced data
    */
    void trainRoundRobin(boost::shared_ptr<DataFrame> data, unsigned int numFactors, std::string posClass,
      std::string negClass, unsigned int nodeSize = 1, bool balanced = false);
    /**
    * Resets the id counter.  Only needed in build-model when a model is loaded, increasing the ids,
    * and then a model is created, subsequently creating a new model where the random forest's first tree doesn't start
    * with an id of zero like it should
    */
    static void resetIds() { _idCtr = 0; }

  private:
    /**
    * Recursive function for building random tree nodes
    */
    void _build(boost::shared_ptr<DataFrame> data, std::vector<unsigned int> & dataSet,
      boost::shared_ptr<TreeNode> &node, unsigned int nodeSize);

    /**
    * A recursive function to sum the purity gain per node based for its splitting factor
    *
    *  @param node the node of interest
    *  @param factorPurity the results container as a map of factor index to current purity gain 
    *  sum
    */
    void _calcFactorPurity(boost::shared_ptr<TreeNode> &node,
      std::map<unsigned int, double> & factorPurity);
    
    /**
    *  Destroys the tree by recursively deleting its child nodes
    *
    * @param node the node to consider
    */
    void _destroyTree(boost::shared_ptr<TreeNode> &node);

    /**
    *  A recursive method for exporting nodes for writing
    *
    * @param fileStream the stream to export the data
    * @param node the current node under consideration
    * @param tabDepth set of tabs to indent the exported content
    */
    void _exportNode(std::ostream & fileStream, boost::shared_ptr<TreeNode> &node, std::string tabDepth);

    /**
     * @brief _exportNode a recursive function to write a trees nodes
     *
     * The tree is pre-order traversed and each node's info is appended to the parentNode as encountered
     *
     * @param modelDoc the active DOM document
     * @param parentNode the parent XML node to append the tree nodes
     * @param node the current tree node
     */
    void _exportNode(QDomDocument & modelDoc, QDomElement & parentNode,
      boost::shared_ptr<TreeNode> & node);

    /**
    *  Exports the out of bag set indices
    *
    * @param fileStream the stream to export the data
    * @param tabDepth set of tabs to indent the exported content
    */
    void _exportOobSet(std::ostream & fileStream, std::string tabDepth);

    /**
    *  A recursive method for importing nodes for creation
    *
    * @param fileStream the stream to import the data
    * @param node the current node under consideration
    */
    void _importNode(std::istream & fileStream, boost::shared_ptr<TreeNode> &node);

    /**
     * @brief _importNode imports nodes for creation
     * @param treeNode the XML DOM node containing a tree node
     * @param node the tree node object to fill
     */
    QDomElement _importNode(QDomElement & treeNode, boost::shared_ptr<TreeNode> &node);

    /**
    * Imports the out of bag set indices
    *
    * @param fileStream the stream to export the data
    */
    void _importOobSet(std::istream & fileStream);

    /**
     * @brief _importOobSet parses the oobSet
     * @param oobString the space delimited OOB set
     */
    void _importOobSet(QString & oobString);

    
    unsigned int _factPerNode;  ///The number of factors to consider per node split

    InfoGainCalculator _igc; ///Object to provide info gain functionality

    unsigned int _nodeCounter;  

    std::vector<unsigned int> _oobSet;   ///The out of bag set for this tree

    unsigned int _treeId;  ///A unique id for this tree and used to set the random seed

    boost::shared_ptr<TreeNode> _root; /// The root node for the tree

    static unsigned int _idCtr;  //Each new tree is assigned a unique id
    

  };
}
#endif


