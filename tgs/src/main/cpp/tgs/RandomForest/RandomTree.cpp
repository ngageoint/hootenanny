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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RandomTree.h"

//STD Includes
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

//Urgent Includes
#include "../TgsException.h"

namespace Tgs
{
  RandomTree::RandomTree()
  {
    _nodeCounter = 0;
    _factPerNode = 0;
    _randSeedCtr = 0;
    _root = NULL;
  }

  RandomTree::~RandomTree()
  {
    if(_root != NULL)
    {
      _destroyTree(_root);
    }
  }

  
  void RandomTree::_build(DataFrame & data, std::vector<unsigned int> & dataSet, TreeNode * node, unsigned int nodeSize)
  {
    static unsigned int idCtr = 0;
    node->leftChild = NULL;
    node->rightChild = NULL;

    if(data.isDataSetPure(dataSet) || dataSet.size() <= nodeSize)  //Data is pure
    { 
      //If data set is all of same class then it is pure and done
      //Give it a class label
      
      node->classLabel = data.getTrainingLabel(dataSet[0]);
      node->isPure = true;
      node->purityDelta = 0;
      node->rangeMin = node->rangeMax = 0;

      node->dataList = dataSet;
      idCtr++;
      node->nodeId = idCtr; 
    }
    else  //Data is not pure
    {
      std::vector<unsigned int> factors;
      unsigned int splitIdx = 0;
      unsigned int fIdx = 0;
      double splitVal = 0.0;
      double purityDelta = 0.0;

      data.selectRandomFactors(_factPerNode, factors);

      bool splitPossible = _igc.findDataSplit(data, factors, dataSet, splitIdx, fIdx, splitVal, purityDelta);

      if(splitPossible)  //Data is not all same value
      {
        node->isPure = false;

        std::vector<unsigned int> leftSplit;
        std::vector<unsigned int> rightSplit;
        node->leftChild = new TreeNode();
        node->rightChild = new TreeNode();

        node->splitValue = splitVal;
        node->factorIndex = fIdx;
        node->purityDelta = purityDelta;
        node->nodeId = 0;
       
        double minVal, maxVal, mean, q1, q3;

        double bandwidth = data.computeBandwidthByFactor(fIdx, dataSet, minVal, 
          maxVal, mean, q1, q3);
        bandwidth = bandwidth;
//         node->rangeMin = mean - (6 * bandwidth);
//         node->rangeMax = mean + (6 * bandwidth);
//           double midVal = (maxVal - minVal) / 2.0;
//           node->rangeMin = minVal - (0.5 *(maxVal - minVal));
//           node->rangeMax = maxVal + (0.5 * (maxVal - minVal));
         double iqr = q3 - q1;
         node->rangeMin = q1 - ( 3 * iqr);
         node->rangeMax = q3 + (3 * iqr);

       
        data.sortIndicesOnFactorValue(dataSet, fIdx);

        for(unsigned int i = 0; i < splitIdx; i++)
        {
          leftSplit.push_back(dataSet[i]);
        }
        _build(data, leftSplit, node->leftChild, nodeSize);

        for(unsigned int i = splitIdx; i < dataSet.size(); i++)
        {
          rightSplit.push_back(dataSet[i]);
        }

        _build(data, rightSplit, node->rightChild, nodeSize);
      }
      else  //Data is all same value
      {
        //No split possible (all factors values same across all factors)
        //Vote on classes and make pure node.
        
        node->classLabel = data.getMajorityTrainingLabel(dataSet);
        node->isPure = true;
        node->purityDelta = 0;

        node->dataList = dataSet;
        idCtr++;
        node->nodeId = idCtr;
      }
    }
  }

  void RandomTree::buildTest(DataFrame & data, unsigned int numFactors)
  {
    unsigned int nodeSize = 1;
    _factPerNode = numFactors;

    std::vector<unsigned int> indices;
    indices.resize(data.getNumDataVectors());

    for(unsigned int i = 0; i < indices.size(); i++)
    {
      indices[i] = i;
    }

    _root = new TreeNode();
    _root->leftChild = NULL;
    _root->rightChild = NULL;
    _root->isPure = false;

    _build(data, indices, _root, nodeSize);
  }

  void RandomTree::_calcFactorPurity(TreeNode * node, 
    std::map<unsigned int, double> & factorPurity)
  {
    if(!node->isPure)
    {
      _calcFactorPurity(node->leftChild, factorPurity);
      _calcFactorPurity(node->rightChild, factorPurity);
      factorPurity[node->factorIndex] += node->purityDelta;
    }
  }

  unsigned int RandomTree::classifyDataVector(const std::vector<double> & dataVector,
    std::string & outputClass) const
  {
    TreeNode * currentNode = _root;
    bool isPure = false;
    unsigned int nodeId = 0;
    
    do
    {
      isPure = currentNode->isPure;

      if(isPure)  //We are at leaf node and are done
      {
        outputClass = currentNode->classLabel;
        nodeId = currentNode->nodeId;
      }
      else
      {
//         if(dataVector[currentNode->factorIndex] < currentNode->rangeMin || dataVector[currentNode->factorIndex] > currentNode->rangeMax)
//         {
//           std::cout << " Node Val " << dataVector[currentNode->factorIndex] << " Min " << currentNode->rangeMin << " Max " << currentNode->rangeMax << std::endl;
//           outputClass = "zz00";
//           nodeId = currentNode->nodeId;
//           return nodeId;
//         }
        if(dataVector[currentNode->factorIndex] < currentNode->splitValue)
        {
          currentNode = currentNode->leftChild;
        }
        else
        {
          currentNode = currentNode->rightChild;
        }
      }
      
    }while(!isPure);
    

    return nodeId;
  }

  double RandomTree::computeErrorRate(DataFrame & data)
  {
    unsigned int correct = 0;
    unsigned int incorrect = 0;

    std::string result;

    for(unsigned int i = 0; i < _oobSet.size(); i++)
    {
      classifyDataVector(data.getDataVector(_oobSet[i]), result);

      if(result == data.getTrainingLabel(_oobSet[i]))
      {
        correct++;
      }
      else
      {
        incorrect++;
      }
    }

    return (double)incorrect / (double)(incorrect + correct);
  }

  void RandomTree::_destroyTree(TreeNode * node)
  {
    //Perform a post order traversal of the tree
    //Delete node after its children have been deleted
    
    if(node!=NULL)
    {
      _destroyTree(node->leftChild);
      _destroyTree(node->rightChild);
      delete node;
    }
    
  }

  void RandomTree::exportTree(std::ostream & fileStream, std::string tabDepth)
  {
    if(fileStream.good())
    {
      fileStream << tabDepth << "<RandomTree>" << std::endl;
      _exportOobSet(fileStream, tabDepth + "\t");
      _exportNode(fileStream, _root, tabDepth + "\t"); 
      fileStream << tabDepth << "</RandomTree>" << std::endl;
    }
    else
    {
      throw Exception("SaUrgent::RandomTree::export - file stream is not open for exporting");
    }
  }

  void RandomTree::findProximity(DataFrame & data, std::vector<unsigned int> & proximity)
  {
    std::vector<unsigned int> proxVec;

    unsigned int dSize = data.getNumDataVectors();
    proxVec.resize(dSize);

    //Find out which node each vector is classified as 
    for(unsigned int i = 0; i < dSize; i++)
    {
      std::string resultClass;
      unsigned int nodeId = classifyDataVector(data.getDataVector(i), resultClass);

      proxVec[i] = nodeId;
    }

    for(unsigned int j = 0; j < dSize; j++)
    {
      unsigned int tempId = proxVec[j];
      for(unsigned int k = j; k < dSize;j++)
      {
        if(proxVec[k] == tempId)
        {
          proximity[j * dSize + k] += 1;
        }
      }
    }
  }

  void RandomTree::_exportNode(std::ostream & fileStream, TreeNode * node, std::string tabDepth)
  {
    if(fileStream.good())
    {
      if(node != NULL)
      {
        fileStream << tabDepth + "<TreeNode>" << std::endl;
        if(node->isPure)
        {
          fileStream << tabDepth + "\t<NodeType>\tPure\t</NodeType>" << std::endl;
          fileStream << tabDepth + "\t<ClassName>\t" << node->classLabel << "\t</ClassName>" << std::endl;
          fileStream << tabDepth + "\t<Data>\t" << node->nodeId;
          std::set<unsigned int>::iterator itr;

          for(unsigned int i = 0; i < node->dataList.size(); i++)
          {
            fileStream << "\t" << node->dataList[i];
          }

          fileStream << "\t</Data>" << std::endl;

        }
        else
        {
          fileStream << tabDepth + "\t<NodeType>\tSplit\t</NodeType>" << std::endl;
          fileStream << tabDepth + "\t<Data>\t" << node->factorIndex << "\t" << node->splitValue << "\t" << node->purityDelta 
            << "\t" << node->rangeMin << "\t" << node->rangeMax << "\t</Data>" << std::endl;
        }    

        fileStream << tabDepth + "</TreeNode>" << std::endl;
        
        _exportNode(fileStream, node->leftChild, tabDepth);

        _exportNode(fileStream, node->rightChild, tabDepth);
      }
    }
    else
    {
      throw Exception("SaUrgent::RandomTree::_exportNode - file stream is not open for exporting");
    }

  }

  void RandomTree::_exportOobSet(std::ostream & fileStream, std::string tabDepth)
  {
    if(fileStream.good())
    {
      fileStream << tabDepth << "<OobSet>";

      for(unsigned int i = 0; i < _oobSet.size(); i++)
      {
        fileStream << "\t" << _oobSet[i];
      }

      fileStream << "\t</OobSet>" << std::endl;
    }
  }

  void RandomTree::import(std::istream & fileStream)
  {
    _root = NULL;

    if(fileStream.good())
    {
      _root = new TreeNode();
      _importOobSet(fileStream); 
      _importNode(fileStream, _root);
      //Discard end tag
      std::string endTag;
      std::getline(fileStream, endTag);
      //std::cout << "Random Tree Imported" << std::endl;
    }
    else
    {
      throw Exception("SaUrgent::RandomTree::import - file stream is not open for importing");
    }
  }

  void RandomTree::_importOobSet(std::istream & fileStream)
  {
    if(fileStream.good())
    {
      std::string buffer;
      std::getline(fileStream, buffer);
      std::stringstream ss(buffer);
      std::string firstStr;
      std::string nextStr;
      unsigned int idx;
      ss >> firstStr;

      if(firstStr == "<OobSet>")
      {
        ss >> nextStr;

        while(nextStr != "</OobSet>")
        {
          std::stringstream ss2(nextStr);
          ss2 >> idx;
          _oobSet.push_back(idx);
          ss >> nextStr;
        }
      }
    }
  }

  void RandomTree::_importNode(std::istream & fileStream, TreeNode * node)
  {
    std::string buffer;
    std::getline(fileStream, buffer);
    //std::cout << "F" << buffer << std::endl;
    std::stringstream ss(buffer);
    std::string firstStr;
    std::string nextStr;
    
    ss >> firstStr;
  
    if(firstStr == "<TreeNode>")
    {
      //std::cout << "<TreeNode>" << std::endl;
      std::string buffer2;
      std::getline(fileStream, buffer2);
      //std::cout << "F" << buffer2 << std::endl;

      std::string NodeType;
     
      while(buffer2.find("</TreeNode>") == std::string::npos)
      {  
        std::stringstream ss1(buffer2);
        ss1 >> firstStr;

        if(firstStr == "<NodeType>")
        {
          ss1 >> NodeType;
          //std::cout << "<NodeType> " << NodeType << std::endl;
        }
        else if(firstStr == "<ClassName>")
        {
          std::string nextStr;
          ss1 >> nextStr;
          
          bool first = true;
          while (nextStr != "</ClassName>")
          {
            if(first == false)
            {
              node->classLabel += " ";
            }
            else
            {
              first = false;
            }
            node->classLabel += nextStr;
            ss1 >> nextStr;
          }

          //std::cout << "<ClassName> " << className << std::endl;
        }
        else if(firstStr == "<Data>")
        {
          //std::cout << "<Data>" << "|" << NodeType << "|" << std::endl;
          if(NodeType == "Pure")
          {
            //std::cout << "Pure" << std::endl;
            node->isPure = true;
            ss1 >> node->nodeId;

            std::string nextStr;
            ss1 >> nextStr;

            while(nextStr != "</Data>")
            {
              unsigned int idx;
              std::stringstream ss2(nextStr);
              ss2 >> idx;
              node->dataList.push_back(idx);
              ss1 >> nextStr;
            }
          }
          else if(NodeType == "Split")
          {
            //std::cout << "Split" << std::endl;
            node->isPure = false;
            node->nodeId = 0;
            ss1 >> node->factorIndex >> node->splitValue >> node->purityDelta >> 
              node->rangeMin >> node->rangeMax;
            
          }
        }

        
        std::getline(fileStream, buffer2);
        //std::cout << "F" << buffer2 << std::endl;
      }

      //Assign the nodes children
      if(node->isPure == true)
      {
        node->leftChild = NULL;
        node->rightChild = NULL;
      }
      else
      {
        node->leftChild = new TreeNode();
        _importNode(fileStream, node->leftChild);
        node->rightChild = new TreeNode();
        _importNode(fileStream, node->rightChild);
      }
    }
  }


  void RandomTree::getFactorImportance(std::map<unsigned int, double> & purityDeltaSum)
  {
    _calcFactorPurity(_root, purityDeltaSum);
  }

  void RandomTree::trainBinary(DataFrame & data, unsigned int numFactors, std::string posClass, unsigned int nodeSize, bool balanced)
  {
    //Warning suppression
    posClass = posClass;

    //std::cout << "Train Tree" << std::endl;
    _factPerNode = numFactors;

    //Build bootstrap and oob sets (on data vector indices)
    std::vector<unsigned int> bootstrapSet;

    if(balanced)
    {
      data.makeBalancedBoostrapAndOobSets(bootstrapSet, _oobSet);
    }
    else
    {
      data.makeBoostrapAndOobSets(bootstrapSet, _oobSet);
    }

    _root = new TreeNode();
    _root->leftChild = NULL;
    _root->rightChild = NULL;
    _root->isPure = false;

    //std::cout << "Build Tree" << std::endl;
    _build(data, bootstrapSet, _root, nodeSize);

    
  }

  void RandomTree::trainMulticlass(DataFrame & data, unsigned int numFactors, unsigned int nodeSize, bool balanced)
  {
    //std::cout << "Train Tree" << std::endl;
    _factPerNode = numFactors;

    //Build bootstrap and oob sets (on data vector indices)
    std::vector<unsigned int> bootstrapSet;

    if(balanced)
    {
      data.makeBalancedBoostrapAndOobSets(bootstrapSet, _oobSet);
    }
    else
    {
      data.makeBoostrapAndOobSets(bootstrapSet, _oobSet);
    }

    _root = new TreeNode();
    _root->leftChild = NULL;
    _root->rightChild = NULL;
    _root->isPure = false;

    //std::cout << "Build Tree" << std::endl;
    _build(data, bootstrapSet, _root, nodeSize);
  }

  void RandomTree::trainRoundRobin(DataFrame & data, unsigned int numFactors, std::string posClass,
    std::string negClass, unsigned int nodeSize, bool balanced)
  {
    //Warning suppression
    balanced = balanced;

    //std::cout << "Train Tree" << std::endl;
    _factPerNode = numFactors;

    //Build bootstrap and oob sets (on data vector indices)
    std::vector<unsigned int> bootstrapSet;

    data.makeBalancedRoundRobinBootstrapAndOobSets(posClass, negClass, bootstrapSet, _oobSet);
    
    _root = new TreeNode();
    _root->leftChild = NULL;
    _root->rightChild = NULL;
    _root->isPure = false;

    //std::cout << "Build Tree" << std::endl;
    _build(data, bootstrapSet, _root, nodeSize);
  }

} //End Namespace
