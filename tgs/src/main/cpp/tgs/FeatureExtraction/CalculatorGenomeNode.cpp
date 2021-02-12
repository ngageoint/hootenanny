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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */

#include "CalculatorGenomeNode.h"

// Standard Includes
#include <iostream>
#include <sstream>
using namespace std;

#include "../SharedPtr.h"
#include "../TgsException.h"
#include "../Xml/XmlHelper.h"

namespace Tgs
{
  const char* const CalculatorGenomeNode::DOUBLE = "double";

  CalculatorGenomeNode::~CalculatorGenomeNode()
  {

  }

  int CalculatorGenomeNode::calculateDepth() const
  {
    int result = 0;
    if (_inputs.size() != 0)
    {
      for (map<string, boost::shared_ptr<CalculatorGenomeNode> >::const_iterator it =
           _inputs.begin(); it != _inputs.end(); ++it)
      {
        if (it->second)
        {
          result = std::max(result, it->second->calculateDepth());
        }
      }
    }
    return 1 + result;
  }

  boost::shared_ptr<TreeGenomeNode> CalculatorGenomeNode::clone() const
  {
    // create a new node of the same type
    boost::shared_ptr<CalculatorGenomeNode> newNode(_createNew());
    // copy all the internal class data and children
    newNode->copy(*this);
    return newNode;
  }

  void CalculatorGenomeNode::copy(const TreeGenomeNode& from)
  {
    const CalculatorGenomeNode& cgnFrom = dynamic_cast<const CalculatorGenomeNode&>(from);
    for (map<string, boost::shared_ptr<CalculatorGenomeNode> >::const_iterator it =
         cgnFrom._inputs.begin(); it != cgnFrom._inputs.end(); ++it)
    {
      if (it->second)
      {
        _inputs[it->first] = boost::dynamic_pointer_cast<CalculatorGenomeNode>(it->second->clone());
      }
    }
    _id = cgnFrom._id;

    _copyInternalData(cgnFrom);
  }

  std::string CalculatorGenomeNode::findInput(boost::shared_ptr<CalculatorGenomeNode> node)
  {
    for (map<string, boost::shared_ptr<CalculatorGenomeNode> >::const_iterator it = _inputs.begin();
      it != _inputs.end(); ++it)
    {
      if (it->second == node)
      {
        return it->first;
      }
    }
    return "";
  }

  std::string CalculatorGenomeNode::getClassName() const
  {
    return typeid(*this).name();
  }

  boost::shared_ptr<CalculatorGenomeNode> CalculatorGenomeNode::getInput(const std::string& name) const
  {
    std::map<std::string, boost::shared_ptr<CalculatorGenomeNode> >::const_iterator it =
        _inputs.find(name);

    if (it != _inputs.end())
    {
      return it->second;
    }
    throw Exception("Expected input wasn't found.");
  }

  void CalculatorGenomeNode::load(std::istream& s, CalculatorGenomeNodeFactory& factory)
  {
    std::string buffer;
    std::string str;

    _loadInternals(s);

    map<string, string> inputs = getInputs();
    map<string, string>::const_iterator it = inputs.begin();
    std::getline(s, buffer);
    stringstream strm0(buffer);
    strm0 >> str;
    if(str == "<Children>")
    {
      while(buffer.find("</Children>") == std::string::npos && s.eof() == false)
      {
        std::getline(s, buffer);
        stringstream strm1(buffer);
        strm1 >> str;

        if(str == "<CalculatorGenomeNode")
        {
          string id = Tgs::XmlHelper::parseAttributes(buffer)["id"];
          boost::shared_ptr<Tgs::CalculatorGenomeNode> cg(factory.createNode(id));
          cg->load(s, factory);
          if (it == inputs.end())
          {
            throw Exception("Error, more inputs/children than expected.");
          }
          setInput(it->first, cg);
          ++it;
        }
      }
    }

    while(buffer.find("</CalculatorGenomeNode>") == std::string::npos && s.eof() == false)
    {
      std::getline(s, buffer);
    }
  }

  void CalculatorGenomeNode::save(std::ostream& s, const std::string& indent)
  {
    s << indent << "<CalculatorGenomeNode id='" << getId() << "'>" << endl;
    _saveInternals(s, indent + "  ");
    if (_inputs.size() > 0)
    {
      s << indent << "  <Children>" << endl;
      for (map<string, boost::shared_ptr<CalculatorGenomeNode> >::const_iterator it = _inputs.begin();
        it != _inputs.end(); ++it)
      {
        if (it->second)
        {
          it->second->save(s, indent + "    ");
        }
      }
      s << indent << "  </Children>" << endl;
    }
    s << indent << "</CalculatorGenomeNode>" << endl;
  }

  void CalculatorGenomeNode::setInput(const std::string& name, CalculatorGenomeNode* node)
  {
    setInput(name, boost::shared_ptr<CalculatorGenomeNode>(node));
  }

  void CalculatorGenomeNode::setInput(const std::string& name,
    boost::shared_ptr<CalculatorGenomeNode> node)
  {
    _inputs[name] = node;
  }

  string CalculatorGenomeNode::toString() const
  {
    stringstream strm;
    if (_inputs.size() == 1)
    {
      strm << _toLabel() << "(";
      if (_inputs.begin()->second)
      {
        strm << _inputs.begin()->second->toString();
      }
      strm << ")";
    }
    else if (_inputs.size() > 1)
    {
      strm << "(";
      bool first = true;
      for (map<string, boost::shared_ptr<CalculatorGenomeNode> >::const_iterator it = _inputs.begin();
        it != _inputs.end(); ++it)
      {
        if (!first)
        {
          strm << " " << _toLabel() << " ";
        }
        first = false;
        if (it->second)
        {
          strm << it->second->toString();
        }
      }
      strm << ")";
    }
    else
    {
      strm << _toLabel();
    }
    return strm.str();
  }
}
