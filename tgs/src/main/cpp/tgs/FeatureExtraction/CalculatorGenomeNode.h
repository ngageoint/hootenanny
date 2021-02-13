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
 * @copyright Copyright (C) 2015, 2017, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__CALCULATOR_GENOME_NODE_H__
#define __TGS__CALCULATOR_GENOME_NODE_H__

// Standard Includes
#include <map>
#include <string>

#include "../SharedPtr.h"
#include "TreeGenomeNode.h"

namespace Tgs
{
  class CalculatorGenomeNode;

  class TGS_EXPORT CalculatorGenomeNodeFactory
  {
  public:
    virtual ~CalculatorGenomeNodeFactory() {}

    virtual boost::shared_ptr<CalculatorGenomeNode> createNode(const std::string& id) const = 0;
  };

  class TGS_EXPORT CalculatorGenomeNode : public TreeGenomeNode
  {
  public:
    static const char* const DOUBLE;

    virtual ~CalculatorGenomeNode();

    virtual int calculateDepth() const;

    virtual void clearCache() {}

    virtual boost::shared_ptr<TreeGenomeNode> clone() const;

    virtual void copy(const TreeGenomeNode& from);

    std::string findInput(boost::shared_ptr<CalculatorGenomeNode> node);

    virtual std::string getClassName() const;

    /**
     * Returns a map of names of all the inputs this calculator takes. Each input must have a
     * unique name and specify a class that provides the necessary input. These inputs must
     * be constant for the class (no adding or removing inputs during its life cycle).
     * @return At this point the type is ignored.
     */
    virtual const std::map<std::string, std::string> getInputs() const = 0;

    std::string getId() const { return _id; }

    boost::shared_ptr<CalculatorGenomeNode> getInput(const std::string& name) const;

    /**
     * Gets the output for the specified unique identifier. Typically this will call getOutput
     * on all of its children, manipulate those outputs and return the result.
     */
    virtual double getOutput(const int uid) const = 0;

    virtual void load(std::istream& s, CalculatorGenomeNodeFactory& factory);

    virtual void save(std::ostream& s, const std::string& indent = "");

    virtual void setInput(const std::string& name, CalculatorGenomeNode* node);
    virtual void setInput(const std::string& name, boost::shared_ptr<CalculatorGenomeNode> node);

    virtual void setId(const std::string& id) { _id = id; }

    virtual std::string toString() const;

  protected:

    /**
     * Copies all the internal class data (e.g. parameters). Everything but the children.
     */
    virtual void _copyInternalData(const CalculatorGenomeNode& /*node*/) {}

    virtual CalculatorGenomeNode* _createNew() const = 0;

    /**
     * To be overloaded by subclasses. This is for loading any class specific information
     */
    virtual void _loadInternals(std::istream& /*s*/) {}

    /**
     * To be overloaded by subclasses. This is for saving any class specific information
     */
    virtual void _saveInternals(std::ostream& /*s*/, const std::string& /*indent = ""*/) const {}

    virtual std::string _toLabel() const = 0;

  private:
    std::map<std::string, boost::shared_ptr<CalculatorGenomeNode> > _inputs;
    std::string _id;
  };
}

#endif
