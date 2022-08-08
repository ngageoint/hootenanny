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
 * @copyright Copyright (C) 2015, 2016, 2017, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "CalculatorGenome.h"

// Standard Includes
#include <iostream>
#include <list>
#include <sstream>
using namespace std;

#include "../TgsException.h"
#include "../Xml/XmlHelper.h"
#include "BasicMathCalculatorNode.h"
#include "ConstantCalculatorNodeSource.h"

// Tgs
#include <tgs/Statistics/Random.h>

namespace Tgs
{
  std::ostream& operator<<(std::ostream& s, const CalculatorGenome& df)
  {
    df.save(s);
    return s;
  }

  CalculatorGenome::CalculatorGenome()
  {
    _totalWeight = 0.0;
    _sourceWeight = 0.0;
    _depth = 3;
  }

  CalculatorGenome::~CalculatorGenome()
  {

  }

  void CalculatorGenome::addNodeType(CalculatorGenomeNode* node, const std::string& label,
    double weight)
  {
    addNodeType(boost::shared_ptr<CalculatorGenomeNode>(node), label, weight);
  }

  void CalculatorGenome::addNodeType(boost::shared_ptr<CalculatorGenomeNode> node,
    const std::string& label, double weight)
  {
    node->setId(label);
    AvailableNode an(boost::shared_ptr<CalculatorGenomeNode>(node), weight);
    _availableNodes[label] = an;

    // recalculate the whole weight just in case an entry was replaced in the map
    _calculateWeights();
  }

  void CalculatorGenome::addBasicMathNodeTypes()
  {
    addNodeType(new AddCalculatorNode(), "class Tgs::AddCalculatorNode", 1.0);
    addNodeType(new SubtractCalculatorNode(), "class Tgs::SubtractCalculatorNode", 1.0);
    addNodeType(new MultiplyCalculatorNode(), "class Tgs::MultiplyCalculatorNode", 1.0);
    addNodeType(new DivideCalculatorNode(), "class Tgs::DivideCalculatorNode", 1.0);
    addNodeType(new PowCalculatorNode(), "class Tgs::PowCalculatorNode", 0.1);
    addNodeType(new SqrtCalculatorNode(), "class Tgs::SqrtCalculatorNode", 0.25);
    addNodeType(new LogCalculatorNode(), "class Tgs::LogCalculatorNode", 0.1);
    addNodeType(new ConstantCalculatorNodeSource(10, 5),
      "class Tgs::ConstantCalculatorNodeSource", 2.0);
  }

  int CalculatorGenome::calculateDepth() const
  {
    return _root->calculateDepth();
  }

  double CalculatorGenome::calculateValue(int uid) const
  {
    return _root->getOutput(uid);
  }
  
  void CalculatorGenome::_calculateWeights()
  {
    _totalWeight = 0.0;
    _sourceWeight = 0.0;
    for (map<string, AvailableNode>::const_iterator it = _availableNodes.begin(); 
      it != _availableNodes.end(); ++it)
    {
      _totalWeight += it->second.weight;
      if (it->second.node->getInputs().size() == 0)
      {
        _sourceWeight += it->second.weight;
      }
    }
  }

  boost::shared_ptr<CalculatorGenomeNode> CalculatorGenome::_chooseRandomNode()
  {
    int count = _countNodes(_root);
    int nodeIndex = Tgs::Random::instance()->generateInt(count);
    return _findNode(nodeIndex);
  }

  boost::shared_ptr<Genome> CalculatorGenome::clone() const
  {
    boost::shared_ptr<CalculatorGenome> result(new CalculatorGenome());
    result->_availableNodes = _availableNodes;
    result->_totalWeight = _totalWeight;
    result->_sourceWeight = _sourceWeight;
    if (_root)
    {
      result->_root = boost::dynamic_pointer_cast<CalculatorGenomeNode>(_root->clone());
    }
    result->setScore(getScore());
    return result;
  }
  
  int CalculatorGenome::countNodes() const
  {
    return _countNodes(_root);
  }

  int CalculatorGenome::_countNodes(const boost::shared_ptr<CalculatorGenomeNode> node) const
  {
    if (node == NULL)
    {
      return 0;
    }
    int result = 1;
    const map<string, string>& inputs = node->getInputs();
    for (map<string, string>::const_iterator it = inputs.begin(); it != inputs.end(); ++it)
    {
      result += _countNodes(node->getInput(it->first));
    }
    return result;
  }

  boost::shared_ptr<CalculatorGenomeNode> CalculatorGenome::_createNewNode(bool sourceOnly) const
  {
    double weight = _totalWeight;
    if (sourceOnly)
    {
      weight = _sourceWeight;
    }
    double r = Tgs::Random::instance()->generateUniform() * weight;
    double last = 0.0;
    double sum = 0.0;
    for (map<string, AvailableNode>::const_iterator it = _availableNodes.begin(); 
      it != _availableNodes.end(); ++it)
    {
      if (sourceOnly == false || it->second.node->getInputs().size() == 0)
      {
        sum += it->second.weight;
        if (r <= sum && r >= last)
        {
          return boost::dynamic_pointer_cast<CalculatorGenomeNode>(it->second.node->clone());
        }
        last = sum;
      }
    }
    // shouldn't get here
    throw Exception("CalculatorGenome::_createNewNode() Shouldn't get here.");
  }

  boost::shared_ptr<CalculatorGenomeNode> CalculatorGenome::_createNewTree(int maxDepth) const
  {
    bool sourceOnly = (maxDepth == 1);
    boost::shared_ptr<CalculatorGenomeNode> newNode = _createNewNode(sourceOnly);
    const map<string, string>& inputs = newNode->getInputs();
    for (map<string, string>::const_iterator it = inputs.begin(); it != inputs.end(); ++it)
    {
      newNode->setInput(it->first, _createNewTree(maxDepth - 1));
    }
    return newNode;
  }

  boost::shared_ptr<CalculatorGenomeNode> CalculatorGenome::createNode(const std::string& id) const
  {
    map<string, AvailableNode>::const_iterator it = _availableNodes.find(id);
    if (it == _availableNodes.end())
    {
      cout << "available nodes: " << endl;
      for (it = _availableNodes.begin(); it != _availableNodes.end(); ++it)
      {
        cout << "  " << it->first << endl;
      }
      stringstream strm;
      strm << "Error finding CalculatorGenomeNode. (" << id << ")";
      throw Tgs::Exception(strm.str());
    }
    boost::shared_ptr<CalculatorGenomeNode> node = it->second.node;
    return boost::dynamic_pointer_cast<CalculatorGenomeNode>(node->clone());
  }

  void CalculatorGenome::crossoverSexually(const Genome& father, const Genome& mother, 
    boost::shared_ptr<Genome>& brother, boost::shared_ptr<Genome>& sister)
  {
    boost::shared_ptr<CalculatorGenome> bro = boost::dynamic_pointer_cast<CalculatorGenome>(father.clone());
    boost::shared_ptr<CalculatorGenome> sis = boost::dynamic_pointer_cast<CalculatorGenome>(mother.clone());

    // find the cut points
    boost::shared_ptr<CalculatorGenomeNode> broSubtree = bro->_chooseRandomNode();
    boost::shared_ptr<CalculatorGenomeNode> sisSubtree = sis->_chooseRandomNode();

    boost::shared_ptr<CalculatorGenomeNode> broSubtreeParent = bro->_findParent(broSubtree);
    boost::shared_ptr<CalculatorGenomeNode> sisSubtreeParent = sis->_findParent(sisSubtree);

    // move the sister subtree over to the bro
    if (broSubtreeParent == NULL)
    {
      bro->_root = sisSubtree;
    }
    else
    {
      std::string broLabel = broSubtreeParent->findInput(broSubtree);
      broSubtreeParent->setInput(broLabel, sisSubtree);
    }

    // move the brother subtree over to the bro
    if (sisSubtreeParent == NULL)
    {
      sis->_root = broSubtree;
    }
    else
    {
      std::string sisLabel = sisSubtreeParent->findInput(sisSubtree);
      sisSubtreeParent->setInput(sisLabel, broSubtree);
    }

    brother = bro;
    brother->setScore(-1);
    sister = sis;
    sister->setScore(-1);
  }

  boost::shared_ptr<CalculatorGenomeNode> CalculatorGenome::_findParent(
    boost::shared_ptr<CalculatorGenomeNode> node) const
  {
    return _findParent(node, _root);
  }

  boost::shared_ptr<CalculatorGenomeNode> CalculatorGenome::_findParent(
    boost::shared_ptr<CalculatorGenomeNode> node, boost::shared_ptr<CalculatorGenomeNode> current) const
  {
    if (current == NULL)
    {
      current = _root;
    }
    if (node == _root)
    {
      return boost::shared_ptr<CalculatorGenomeNode>();
    }
    boost::shared_ptr<CalculatorGenomeNode> result;
    const map<string, string>& inputs = current->getInputs();
    for (map<string, string>::const_iterator it = inputs.begin(); it != inputs.end(); ++it)
    {
      boost::shared_ptr<CalculatorGenomeNode> child = current->getInput(it->first);
      if (node == child)
      {
        return current;
      }
      result = _findParent(node, child);
      if (result)
      {
        break;
      }
    }
    return result;
  }

  boost::shared_ptr<CalculatorGenomeNode> CalculatorGenome::_findNode(int index) const
  {
    int count = -1;
    boost::shared_ptr<CalculatorGenomeNode> node;
    list<boost::shared_ptr<CalculatorGenomeNode> > pending;
    pending.push_back(_root);
    // does a breadth first search for the index'th item
    while (count < index && pending.size() > 0)
    {
      node = pending.front();
      pending.pop_front();
      const map<string, string>& inputs = node->getInputs();
      for (map<string, string>::const_iterator it = inputs.begin(); it != inputs.end(); ++it)
      {
        boost::shared_ptr<CalculatorGenomeNode> child = node->getInput(it->first);
        if (child != NULL)
        {
          pending.push_back(child);
        }
      }
      count++;
    }
    return node;
  }

  void CalculatorGenome::initialize()
  {
    _root = _createNewTree(_depth);
    mutate(1);
  }

  void CalculatorGenome::load(std::istream& s)
  {
    string buffer;
    string firstStr;
    getline(s, buffer);

    stringstream strm0(buffer);
    strm0 >> firstStr;
    if(firstStr == "<Genome")
    {
      getline(s, buffer);
      stringstream strm1(buffer);
      strm1 >> firstStr;
    }

    //<CalculatorGenomeNode id='class Tgs::AddCalculatorNode'>
    if(firstStr == "<CalculatorGenomeNode")
    {
      string id = Tgs::XmlHelper::parseAttributes(buffer)["id"];
      boost::shared_ptr<Tgs::CalculatorGenomeNode> cgn(createNode(id));
      cgn->load(s, *this);
      _root = cgn;
    }
    else
    {
      cout << "Error parsing GA features, expected <CalculatorGenomeNode> " << buffer << endl;
    }

    setInvalidScore();
  }

  void CalculatorGenome::mutate(double p)
  {
    boost::shared_ptr<CalculatorGenomeNode> node = _chooseRandomNode();
    boost::shared_ptr<CalculatorGenomeNode> parent = _findParent(node);
    if (parent)
    {
      std::string nodeLabel = parent->findInput(node);
      int depth = (int)std::max(2.0, ceil(calculateDepth() * .15));
      parent->setInput(nodeLabel, _createNewTree(depth));
    }
    else
    {
      _root = _createNewTree((int)(node->calculateDepth() * 1.15));
    }

    _mutate(p, _root);
    setInvalidScore();
  }

  void CalculatorGenome::_mutate(double p, boost::shared_ptr<CalculatorGenomeNode> node)
  {
    double r = Tgs::Random::instance()->generateUniform();

    if (r <= p)
    {
      node->mutate(p);
    }
    const map<string, string>& inputs = node->getInputs();
    for (map<string, string>::const_iterator it = inputs.begin(); it != inputs.end(); ++it)
    {
      _mutate(p, node->getInput(it->first));
    }
  }

  void CalculatorGenome::resetWeights(double v)
  {
    for (AvailableNodeMap::iterator it = _availableNodes.begin(); it != _availableNodes.end();
      ++it)
    {
      it->second.weight = v;
    }
  }

  void CalculatorGenome::save(std::ostream& s, const std::string& indent) const
  {
    s << indent << "<Genome type='CalculatorGenome'>" << endl;
    if (_root)
    {
      _root->save(s, indent + "  ");
    }
    s << indent << "</Genome>" << endl;
  }

  void CalculatorGenome::setWeight(const std::string& label, double weight)
  {
    if (_availableNodes.find(label) == _availableNodes.end())
    {
      throw Exception("Internal Error: CalculatorGenome::setWeight -- Couldn't find node.");
    }
    _availableNodes[label].weight = weight;
    _calculateWeights();
  }

  std::string CalculatorGenome::toString() const
  {
    return _root->toString();
  }
}
