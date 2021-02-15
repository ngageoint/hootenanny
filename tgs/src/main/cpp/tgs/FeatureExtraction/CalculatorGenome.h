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

#ifndef __TGS__CALCULATOR_GENOME_H__
#define __TGS__CALCULATOR_GENOME_H__

// Standard Includes
#include <map>
#include <string>

#include "CalculatorGenomeNode.h"
#include "TreeGenome.h"

namespace Tgs
{
  class TGS_EXPORT CalculatorGenome :
    public TreeGenome,
    public CalculatorGenomeNodeFactory
  {
  public:

    class AvailableNode
    {
    public:
      AvailableNode() { weight = -1; }

      AvailableNode(boost::shared_ptr<CalculatorGenomeNode> node, double weight)
      {
        this->node = node;
        this->weight = weight;
      }

      AvailableNode(const AvailableNode& an)
      {
        node = an.node;
        weight = an.weight;
      }

      boost::shared_ptr<CalculatorGenomeNode> node;
      double weight;
    };

    typedef std::map<std::string, AvailableNode> AvailableNodeMap;

    CalculatorGenome();

    virtual ~CalculatorGenome();

    /**
     * A typical weight (e.g. addition) is 1.
     */
    void addNodeType(CalculatorGenomeNode* node, const std::string& label, double weight);
    void addNodeType(boost::shared_ptr<CalculatorGenomeNode> node, const std::string& label,
      double weight);

    void addBasicMathNodeTypes();

    int calculateDepth() const;

    /**
    * Calculates and returns the result of the calculation on the specified UID.
    */
    double calculateValue(int uid) const;

    virtual void clearCache() { _root->clearCache(); }

    virtual boost::shared_ptr<Genome> clone() const;

    int countNodes() const;

    boost::shared_ptr<CalculatorGenomeNode> createNode(const std::string& id) const;

    virtual void crossoverSexually(const Genome& father, const Genome& mother,
      boost::shared_ptr<Genome>& brother, boost::shared_ptr<Genome>& sister);

    const AvailableNodeMap& getAvailableNodes() const { return _availableNodes; }

    int getNodeTypeCount() const { return _availableNodes.size(); }

    const boost::shared_ptr<CalculatorGenomeNode> getRoot() const { return _root; }

    virtual void initialize();

    virtual void load(std::istream& s);

    virtual void mutate(double p);

    /**
     * Resets all the weights to v. If at least one terminal weight isn't a positive, non-zero
     * values before a mutation or initialization bad things will happen.
     */
    void resetWeights(double v);

    /**
     * Save this genome to the specified stream as XML.
     */
    void save(std::ostream& s, const std::string& indent = "") const;

    void setInitializationDepth(int depth) { _depth = depth; }

    void setWeight(const std::string& label, double weight);

    virtual std::string toString() const;

  private:
    AvailableNodeMap _availableNodes;

    boost::shared_ptr<CalculatorGenomeNode> _root;

    double _totalWeight, _sourceWeight;
    int _depth;

    void _calculateWeights();

    boost::shared_ptr<CalculatorGenomeNode> _chooseRandomNode();

    /**
     * Returns the total number of nodes in this tree.
     */
    int _countNodes(const boost::shared_ptr<CalculatorGenomeNode> node) const;

    boost::shared_ptr<CalculatorGenomeNode> _createNewNode(bool sourceOnly) const;

    boost::shared_ptr<CalculatorGenomeNode> _createNewTree(int maxDepth) const;

    /**
     * I realize this is inefficient, but it makes maintenance and creation of the
     * CalculatorGenomeNode much easier. Also, compared to the other computations that will be
     * occurring, this is small potatoes.
     */
    boost::shared_ptr<CalculatorGenomeNode> _findParent(boost::shared_ptr<CalculatorGenomeNode> node) const;

    boost::shared_ptr<CalculatorGenomeNode> _findParent(boost::shared_ptr<CalculatorGenomeNode> node,
      boost::shared_ptr<CalculatorGenomeNode> current) const;

    boost::shared_ptr<CalculatorGenomeNode> _findNode(int index) const;

    void _mutate(double p, boost::shared_ptr<CalculatorGenomeNode> node);
  };

  TGS_EXPORT std::ostream& operator<<(std::ostream& s, const CalculatorGenome& df);
}


#endif
