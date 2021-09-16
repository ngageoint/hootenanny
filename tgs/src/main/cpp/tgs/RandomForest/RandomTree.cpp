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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "RandomTree.h"

// Qt
#include <QStringList>
#include <QtGlobal>

// Std
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

// Tgs
#include <tgs/TgsException.h>
#include <tgs/Statistics/Random.h>

namespace Tgs
{
  unsigned int RandomTree::_idCtr = 0;

  RandomTree::RandomTree()
  {
    try
    {
      _nodeCounter = 0;
      _factPerNode = 0;
      _treeId = _idCtr;
      _idCtr++;
      _root.reset();
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  RandomTree::~RandomTree()
  {
    if (_root != nullptr)
    {
      _destroyTree(_root);
      _root.reset();
    }
  }

  void RandomTree::buildTest(const std::shared_ptr<DataFrame>& data, unsigned int numFactors)
  {
    try
    {
      unsigned int nodeSize = 1;
      _factPerNode = numFactors;

      std::vector<unsigned int> indices;
      indices.resize(data->getNumDataVectors());

      for (unsigned int i = 0; i < indices.size(); i++)
      {
        indices[i] = i;
      }

      _root = std::make_shared<TreeNode>();
      _root->leftChild.reset();
      _root->rightChild.reset();
      _root->isPure = false;

      _build(data, indices, _root, nodeSize);
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  unsigned int RandomTree::classifyDataVector(const std::vector<double>& dataVector,
    std::string& outputClass) const
  {
    try
    {
      std::shared_ptr<TreeNode> currentNode = _root;
      bool isPure = false;
      unsigned int nodeId = 0;

      do
      {
        isPure = currentNode->isPure;

        if (isPure)  //We are at leaf node and are done
        {
          outputClass = currentNode->classLabel;
          nodeId = currentNode->nodeId;
        }
        else
        {
          if (dataVector[currentNode->factorIndex] < currentNode->splitValue)
          {
            currentNode = currentNode->leftChild;
          }
          else
          {
            currentNode = currentNode->rightChild;
          }
        }

      } while (!isPure);

      return nodeId;
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  double RandomTree::computeErrorRate(const std::shared_ptr<DataFrame>& data)
  {
    try
    {
      unsigned int correct = 0;
      unsigned int incorrect = 0;

      std::string result;

      for (unsigned int i = 0; i < _oobSet.size(); i++)
      {
        classifyDataVector(data->getDataVector(_oobSet[i]), result);

        if (result == data->getTrainingLabel(_oobSet[i]))
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
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomTree::exportTree(std::ostream& fileStream, const std::string& tabDepth)
  {
    try
    {
      throw Exception(__LINE__, "This function has been deprecated.");

      if (fileStream.good())
      {
        fileStream << tabDepth << "<RandomTree>" << std::endl;
        _exportOobSet(fileStream, tabDepth + "\t");
        _exportNode(fileStream, _root, tabDepth + "\t");
        fileStream << tabDepth << "</RandomTree>" << std::endl;
      }
      else
      {
        throw Exception("File stream is not open for exporting");
      }
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomTree::exportTree(QDomDocument & modelDoc, QDomElement & parentNode)
  {
    try
    {
      QDomElement randomTreeNode = modelDoc.createElement("RandomTree");
      randomTreeNode.setAttribute("id", _treeId);

      //Export OobSet
      QDomElement oobSetNode = modelDoc.createElement("OobSet");
      std::stringstream oobStream;
      for (unsigned int i = 0; i < _oobSet.size(); i++)
      {
        oobStream << _oobSet[i];

        if (i != _oobSet.size() - 1)
        {
          oobStream << " ";
        }
      }

      QDomText oobSetText = modelDoc.createTextNode(oobStream.str().c_str());
      oobSetNode.appendChild(oobSetText);
      randomTreeNode.appendChild(oobSetNode);

      //Export Trees Nodes
      QDomElement treeNodesNode = modelDoc.createElement("TreeNodes");
      _exportNode(modelDoc, treeNodesNode, _root);

      randomTreeNode.appendChild(treeNodesNode);
      parentNode.appendChild(randomTreeNode);
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomTree::findProximity(const std::shared_ptr<DataFrame>& data,
    std::vector<unsigned int> & proximity) const
  {
    try
    {
      std::vector<unsigned int> proxVec;

      unsigned int dSize = data->getNumDataVectors();
      proxVec.resize(dSize);

      //Find out which node each vector is classified as
      for (unsigned int i = 0; i < dSize; i++)
      {
        std::string resultClass;
        unsigned int nodeId = classifyDataVector(data->getDataVector(i), resultClass);

        proxVec[i] = nodeId;
      }

      for (unsigned int j = 0; j < dSize; j++)
      {
        unsigned int tempId = proxVec[j];
        for (unsigned int k = j; k < dSize;j++)
        {
          if (proxVec[k] == tempId)
          {
            proximity[j * dSize + k] += 1;
          }
        }
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }



  void RandomTree::import(std::istream & fileStream)
  {
    try
    {
      throw Exception(__LINE__, "This function has been deprecated.");

      _root.reset();

      if (fileStream.good())
      {
        _root = std::make_shared<TreeNode>();
        _importOobSet(fileStream);
        _importNode(fileStream, _root);
        //Discard end tag
        std::string endTag;
        std::getline(fileStream, endTag);
        //std::cout << "Random Tree Imported" << std::endl;
      }
      else
      {
        throw Exception(__LINE__, "File stream is not open for importing");
      }
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }

  }

  void RandomTree::import(const QDomElement& e)
  {
    try
    {

      _root.reset();
      _root = std::make_shared<TreeNode>();

      QDomNodeList childList = e.childNodes();

      for (unsigned int i = 0; i < (unsigned int)childList.size(); i++)
      {
        if (childList.at(i).nodeType() == QDomNode::CommentNode)
        {
          //Skip comments
          continue;
        }

        if (childList.at(i).isElement())
        {
          QString tag = childList.at(i).toElement().tagName().toUpper();

          if (tag == "OOBSET")
          {

            QString oobString = childList.at(i).toElement().text();
            _importOobSet(oobString);
          }
          else if (tag == "TREENODES")
          {

            QDomElement rootDomNode = childList.at(i).firstChild().toElement();

            _importNode(rootDomNode, _root);

          }
          else
          {
            std::stringstream ss;
            ss << "The tag " << tag.toLatin1().constData() <<
              " is not supported under the tag <RandomTree>";
            throw Exception(__LINE__, ss.str());
          }
        }
      }
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }

  }

  void RandomTree::getFactorImportance(std::map<unsigned int, double> & purityDeltaSum)
  {
    try
    {
      _calcFactorPurity(_root, purityDeltaSum);
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomTree::trainBinary(const std::shared_ptr<DataFrame>& data, unsigned int numFactors,
    const std::string& /*posClass*/, unsigned int nodeSize, bool balanced)
  {
    try
    {
      //std::cout << "Train Tree" << std::endl;
      _factPerNode = numFactors;

      //Build bootstrap and oob sets (on data vector indices)
      std::vector<unsigned int> bootstrapSet;

      if (balanced)
      {
        data->makeBalancedBoostrapAndOobSets(bootstrapSet, _oobSet);
      }
      else
      {
        data->makeBoostrapAndOobSets(bootstrapSet, _oobSet);
      }

      _root = std::make_shared<TreeNode>();
      _root->leftChild.reset();
      _root->rightChild.reset();
      _root->isPure = false;

      //std::cout << "Build Tree" << std::endl;
      _build(data, bootstrapSet, _root, nodeSize);
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomTree::trainMulticlass(const std::shared_ptr<DataFrame>& data, unsigned int numFactors,
    unsigned int nodeSize, bool balanced)
  {
    try
    {
      Tgs::Random::instance()->seed(_treeId);

      //std::cout << "Train Tree" << std::endl;
      _factPerNode = numFactors;

      //Build bootstrap and oob sets (on data vector indices)
      std::vector<unsigned int> bootstrapSet;

      if (balanced)
      {
        data->makeBalancedBoostrapAndOobSets(bootstrapSet, _oobSet);
      }
      else
      {
        data->makeBoostrapAndOobSets(bootstrapSet, _oobSet);
      }

      _root = std::make_shared<TreeNode>();
      _root->leftChild.reset();
      _root->rightChild.reset();
      _root->isPure = false;

      //std::cout << "Build Tree" << std::endl;
      _build(data, bootstrapSet, _root, nodeSize);
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomTree::trainRoundRobin(const std::shared_ptr<DataFrame>& data, unsigned int numFactors,
    const std::string& posClass, const std::string& negClass, unsigned int nodeSize,
    bool /*balanced*/)
  {
    try
    {
      //std::cout << "Train Tree" << std::endl;
      _factPerNode = numFactors;

      //Build bootstrap and oob sets (on data vector indices)
      std::vector<unsigned int> bootstrapSet;

      data->makeBalancedRoundRobinBootstrapAndOobSets(posClass, negClass, bootstrapSet, _oobSet);

      _root = std::make_shared<TreeNode>();
      _root->leftChild.reset();
      _root->rightChild.reset();
      _root->isPure = false;

      //std::cout << "Build Tree" << std::endl;
      _build(data, bootstrapSet, _root, nodeSize);
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }

  }

  void RandomTree::_build(
    const std::shared_ptr<DataFrame>& data, std::vector<unsigned int> & dataSet,
    const std::shared_ptr<TreeNode>& node, unsigned int nodeSize)
  {
    try
    {
      static unsigned int idCtr = 0;
      node->leftChild.reset();
      node->rightChild.reset();

      if (data->isDataSetPure(dataSet) || dataSet.size() <= nodeSize)  //Data is pure
      {
        //If data set is all of same class then it is pure and done
        //Give it a class label

        node->classLabel = data->getTrainingLabel(dataSet[0]);
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

        data->selectRandomFactors(_factPerNode, factors);

        bool splitPossible = _igc.findDataSplit(*data, factors, dataSet, splitIdx, fIdx, splitVal,
          purityDelta);

        if (splitPossible)  //Data is not all same value
        {
          node->isPure = false;

          std::vector<unsigned int> leftSplit;
          std::vector<unsigned int> rightSplit;
          node->leftChild = std::make_shared<TreeNode>();
          node->rightChild = std::make_shared<TreeNode>();

          node->splitValue = splitVal;
          node->factorIndex = fIdx;
          node->purityDelta = purityDelta;
          node->nodeId = 0;

          double minVal, maxVal, mean, q1, q3;

          //  bandwidth
          data->computeBandwidthByFactor(fIdx, dataSet, minVal,
          maxVal, mean, q1, q3);
          double iqr = q3 - q1;
          node->rangeMin = q1 - ( 3 * iqr);
          node->rangeMax = q3 + (3 * iqr);

          data->sortIndicesOnFactorValue(dataSet, fIdx);

          for (unsigned int i = 0; i < splitIdx; i++)
          {
            leftSplit.push_back(dataSet[i]);
          }
          _build(data, leftSplit, node->leftChild, nodeSize);

          for (unsigned int i = splitIdx; i < dataSet.size(); i++)
          {
            rightSplit.push_back(dataSet[i]);
          }

          _build(data, rightSplit, node->rightChild, nodeSize);
        }
        else  //Data is all same value
        {
          //No split possible (all factors values same across all factors)
          //Vote on classes and make pure node.

          node->classLabel = data->getMajorityTrainingLabel(dataSet);
          node->isPure = true;
          node->purityDelta = 0;

          node->dataList = dataSet;
          idCtr++;
          node->nodeId = idCtr;
        }
      }
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomTree::_calcFactorPurity(
    const std::shared_ptr<TreeNode>& node, std::map<unsigned int, double>& factorPurity)
  {
    try
    {
      if (!node->isPure)
      {
        _calcFactorPurity(node->leftChild, factorPurity);
        _calcFactorPurity(node->rightChild, factorPurity);
        factorPurity[node->factorIndex] += node->purityDelta;
      }
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomTree::_destroyTree(std::shared_ptr<TreeNode> & node)
  {

    //Perform a post order traversal of the tree
    //Delete node after its children have been deleted

    if (node.get() != nullptr)
    {
      _destroyTree(node->leftChild);
      _destroyTree(node->rightChild);
      node.reset();
    }
  }

  void RandomTree::_exportNode(
     std::ostream& fileStream, const std::shared_ptr<TreeNode>& node, const std::string& tabDepth)
  {
    try
    {
      throw Exception(__LINE__, "This function has been deprecated.");

      if (fileStream.good())
      {
        if (node != nullptr)
        {
          fileStream << tabDepth + "<TreeNode>" << std::endl;

          if (node->isPure)
          {
            fileStream << tabDepth + "\t<NodeType>\tPure\t</NodeType>" << std::endl;
            fileStream << tabDepth + "\t<ClassName>\t" << node->classLabel << "\t</ClassName>" <<
              std::endl;
            fileStream << tabDepth + "\t<Data>\t" << node->nodeId;

            for (unsigned int i = 0; i < node->dataList.size(); i++)
            {
              fileStream << "\t" << node->dataList[i];
            }

            fileStream << "\t</Data>" << std::endl;

          }
          else
          {
            fileStream << tabDepth + "\t<NodeType>\tSplit\t</NodeType>" << std::endl;
            fileStream << tabDepth + "\t<Data>\t" << node->factorIndex << "\t" <<
              node->splitValue << "\t" << node->purityDelta
              << "\t" << node->rangeMin << "\t" << node->rangeMax << "\t</Data>" << std::endl;
          }

          fileStream << tabDepth + "</TreeNode>" << std::endl;

          _exportNode(fileStream, node->leftChild, tabDepth);

          _exportNode(fileStream, node->rightChild, tabDepth);
        }
      }
      else
      {
        throw Exception("File stream is not open for exporting");
      }
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomTree::_exportNode(
    QDomDocument& modelDoc, QDomElement& parentNode, const std::shared_ptr<TreeNode>& node)
  {
    try
    {
      if (node)
      {
        QDomElement treeDomNode = modelDoc.createElement("TreeNode");

        treeDomNode.setAttribute("id", node->nodeId);

        if (node->isPure)
        {
          //Add Node Type
          QDomElement typeNode = modelDoc.createElement("NodeType");
          QDomText typeText = modelDoc.createTextNode("Pure");
          typeNode.appendChild(typeText);
          treeDomNode.appendChild(typeNode);

          //Add class label
          QDomElement classNode = modelDoc.createElement("ClassName");
          QDomText classText = modelDoc.createTextNode(node->classLabel.c_str());
          classNode.appendChild(classText);
          treeDomNode.appendChild(classNode);

          //Add Data
          QDomElement dataNode = modelDoc.createElement("Data");

          std::stringstream dataStream;
          for (unsigned int i = 0; i < node->dataList.size(); i++)
          {
            dataStream << node->dataList[i];

            if (i != node->dataList.size() - 1)
            {
              dataStream << " ";
            }
          }

          QDomText dataText = modelDoc.createTextNode(dataStream.str().c_str());
          dataNode.appendChild(dataText);
          treeDomNode.appendChild(dataNode);

        }
        else
        {
          //Add Node Type
          QDomElement typeNode = modelDoc.createElement("NodeType");
          QDomText typeText = modelDoc.createTextNode("Split");
          typeNode.appendChild(typeText);
          treeDomNode.appendChild(typeNode);

          //Add Data
          QDomElement dataNode = modelDoc.createElement("Data");
          std::stringstream dataStream;

          dataStream << node->factorIndex << " " << node->splitValue << " " << node->purityDelta <<
            " " << node->rangeMin << " " << node->rangeMax;

          QDomText dataText = modelDoc.createTextNode(dataStream.str().c_str());
          dataNode.appendChild(dataText);
          treeDomNode.appendChild(dataNode);

        }

        parentNode.appendChild(treeDomNode);

        //Export child tree nodes
        _exportNode(modelDoc, parentNode, node->leftChild);

        _exportNode(modelDoc, parentNode, node->rightChild);
      }
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomTree::_exportOobSet(std::ostream & fileStream, const std::string& tabDepth)
  {
    try
    {
      throw Exception(__LINE__, "This function has been deprecated.");

      if (fileStream.good())
      {
        fileStream << tabDepth << "<OobSet>";

        for (unsigned int i = 0; i < _oobSet.size(); i++)
        {
          fileStream << "\t" << _oobSet[i];
        }

        fileStream << "\t</OobSet>" << std::endl;
      }
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomTree::_importNode(std::istream& fileStream, const std::shared_ptr<TreeNode>& node)
  {
    try
    {
      throw Exception(__LINE__, "This function has been deprecated.");

      std::string buffer;
      std::getline(fileStream, buffer);
      //std::cout << "F" << buffer << std::endl;
      std::stringstream ss(buffer);
      std::string firstStr;

      ss >> firstStr;

      if (firstStr == "<TreeNode>")
      {
        //std::cout << "<TreeNode>" << std::endl;
        std::string buffer2;
        std::getline(fileStream, buffer2);
        //std::cout << "F" << buffer2 << std::endl;

        std::string NodeType;

        while (buffer2.find("</TreeNode>") == std::string::npos)
        {
          std::stringstream ss1(buffer2);
          ss1 >> firstStr;

          if (firstStr == "<NodeType>")
          {
            ss1 >> NodeType;
            //std::cout << "<NodeType> " << NodeType << std::endl;
          }
          else if (firstStr == "<ClassName>")
          {
            std::string nextStr;
            ss1 >> nextStr;

            bool first = true;
            while (nextStr != "</ClassName>")
            {
              if (first == false)
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
          else if (firstStr == "<Data>")
          {
            //std::cout << "<Data>" << "|" << NodeType << "|" << std::endl;
            if (NodeType == "Pure")
            {
              //std::cout << "Pure" << std::endl;
              node->isPure = true;
              ss1 >> node->nodeId;

              std::string nextStr;
              ss1 >> nextStr;

              while (nextStr != "</Data>")
              {
                unsigned int idx;
                std::stringstream ss2(nextStr);
                ss2 >> idx;
                node->dataList.push_back(idx);
                ss1 >> nextStr;
              }
            }
            else if (NodeType == "Split")
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
        if (node->isPure == true)
        {
          node->leftChild.reset();
          node->rightChild.reset();
        }
        else
        {
          node->leftChild = std::make_shared<TreeNode>();
          _importNode(fileStream, node->leftChild);
          node->rightChild = std::make_shared<TreeNode>();
          _importNode(fileStream, node->rightChild);
        }
      }
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  QDomElement RandomTree::_importNode(
    const QDomElement& treeNode, const std::shared_ptr<TreeNode>& node)
  {
    try
    {
      if (treeNode.isNull())
      {
        return treeNode;
      }

      bool parsedId;

      QString idString = treeNode.attribute("id", "0");
      node->nodeId = idString.toUInt(&parsedId);

      if (!parsedId)
      {
        std::stringstream ss;
        ss << "Unable to parse the tree node id value " << idString.toLatin1().constData();
        throw Exception(__LINE__, ss.str());
      }

      QDomNodeList nodeList = treeNode.childNodes();

      QString treeNodeType;

      for (unsigned int i = 0; i < (unsigned int)nodeList.size(); i++)
      {
        if (nodeList.at(i).nodeType() == QDomNode::CommentNode)
        {
          //Skip comments
          continue;
        }

        QString tag = nodeList.at(i).toElement().tagName().toUpper();
        QDomElement nodeElement = nodeList.at(i).toElement();

        if (tag == "NODETYPE")
        {
          treeNodeType = nodeElement.text().toUpper();
        }
        else if (tag == "CLASSNAME")
        {
          QStringList classList = nodeElement.text().split(" ");

          for (unsigned int j = 0; j < (unsigned int)classList.size(); j++)
          {
            node->classLabel += classList[j].toLatin1().constData();

            if (j != (unsigned int)classList.size() - 1)
            {
              node->classLabel += " ";
            }
          }
        }
        else if (tag == "DATA")
        {
          QStringList dataList = nodeElement.text().split(" ");

          if (treeNodeType == "PURE")
          {
            node->isPure = true;

            bool parsedOk;

            for (unsigned int j = 0; j < (unsigned int)dataList.size(); j++)
            {
              QString dataIndexString = dataList[j];

              unsigned int dataIndex = dataIndexString.toUInt(&parsedOk);

              if (!parsedOk)
              {
                std::stringstream ss;
                ss << "Error parsing index value of " << dataIndexString.toLatin1().constData() <<
                  " in TreeNode->Data from XML";

              }

              node->dataList.push_back(dataIndex);
            }
          }
          else if (treeNodeType == "SPLIT")
          {
            if (dataList.size() != 5)
            {
              std::stringstream ss;
              ss << "The DATA tag for each SPLIT TreeNode must have exactly 5 values.";
              throw Exception(__LINE__, ss.str());
            }

            node->isPure = false;
            node->nodeId = 0;

            bool parsedOk;

            unsigned int factorIndex = dataList[0].toUInt(&parsedOk);
            double splitValue = dataList[1].toDouble(&parsedOk);
            double purityDelta = dataList[2].toDouble(&parsedOk);
            double rangeMin = dataList[3].toDouble(&parsedOk);
            double rangeMax = dataList[4].toDouble(&parsedOk);

            if (!parsedOk)
            {
              std::stringstream ss;
              ss << "Unable to parse the DATA value for a SPLIT node " <<
                nodeElement.text().toLatin1().constData();
              throw Exception(__LINE__, ss.str());
            }

            node->factorIndex = factorIndex;
            node->splitValue = splitValue;
            node->purityDelta = purityDelta;
            node->rangeMin = rangeMin;
            node->rangeMax = rangeMax;
          }
          else
          {
            std::stringstream ss;
            ss << "The tree node type " << treeNodeType.toLatin1().constData() << " is not a valid type.";
            throw Exception(__LINE__, ss.str());
          }
        }
        else
        {
          std::stringstream ss;
          ss << "The tag " << tag.toLatin1().constData() <<
            " is not supported under tag <TreeNodes>";
          throw Exception(__LINE__, ss.str());
        }
      }

      //Assign the nodes children
      if (node->isPure == true)
      {
        node->leftChild.reset();
        node->rightChild.reset();
        QDomElement nextNode = treeNode.nextSibling().toElement();
        return nextNode;
      }
      else //Split Node
      {
        QDomElement leftNode = treeNode.nextSibling().toElement();
        if (leftNode.isNull())
        {
          return leftNode;
        }

        node->leftChild = std::make_shared<TreeNode>();

        QDomElement rightNode = _importNode(leftNode, node->leftChild);

        if (rightNode.isNull())
        {
          return rightNode;
        }

        node->rightChild = std::make_shared<TreeNode>();
        return _importNode(rightNode, node->rightChild);
      }

    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomTree::_importOobSet(std::istream & fileStream)
  {
    try
    {
      throw Exception(__LINE__, "This function has been deprecated.");

      if (fileStream.good())
      {
        std::string buffer;
        std::getline(fileStream, buffer);
        std::stringstream ss(buffer);
        std::string firstStr;
        std::string nextStr;
        ss >> firstStr;

        if (firstStr == "<OobSet>")
        {
          ss >> nextStr;

          while (nextStr != "</OobSet>")
          {
            unsigned int idx;
            std::stringstream ss2(nextStr);
            ss2 >> idx;
            _oobSet.push_back(idx);
            ss >> nextStr;
          }
        }
      }
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomTree::_importOobSet(const QString& oobString)
  {
    try
    {
      QStringList oobList = oobString.split(" ");
      //cout << oobList.size();
      //oobList = oobString.split("\t");
      //cout << oobList.size();

      bool parseOk = true;
      for (unsigned int i = 0; i < (unsigned int)oobList.size(); i++)
      {
        int oobValue = oobList[i].toInt(&parseOk);

        if (!parseOk)
        {
          std::stringstream ss;
          ss << "Unable to parse the OobSet from the XML file";
          throw Exception(__LINE__, ss.str());
        }

        _oobSet.push_back(oobValue);
      }
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }



} //End Namespace
