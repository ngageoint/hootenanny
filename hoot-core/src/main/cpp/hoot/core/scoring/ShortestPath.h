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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

// Qt
#include <QHash>

// Standard
#include <queue>

// TGS
#include <tgs/SharedPtr.h>

namespace hoot
{

class DirectedGraph;

using namespace std;

class ShortestPath
{
public:
  ShortestPath(boost::shared_ptr<const DirectedGraph> graph);

  void calculateCost();

  double getNodeCost(long nodeId) { return _cost.contains(nodeId) ? _cost[nodeId] : -1; }

  /**
   * Set the cost for any starting nodes.
   */
  void setNodeCost(long nodeId, double cost) { _cost[nodeId] = cost; }

private:

  class Node
  {
  public:

    Node(long id, double cost)
    {
      this->id = id;
      this->cost = cost;
    }

    long id;
    double cost;
  };

  /**
   * Comparison struct for the priority queue. Puts the lowest scoring results on top.
   */
  class LesserNode
    : public std::binary_function<const Node&, const Node&, bool>
  {
  public:
    bool operator()(const Node& left,
                    const Node& right) const
    {
      return left.cost > right.cost;
    }
  };


 boost::shared_ptr<const DirectedGraph> _graph;

  QHash<long, double> _cost;
  priority_queue<Node, std::deque<Node>, LesserNode> _queue;
};

}

#endif // SHORTESTPATH_H
