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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "ShortestPath.h"

#include <hoot/core/scoring/DirectedGraph.h>

// Standard
#include <cassert>

namespace hoot
{

ShortestPath::ShortestPath(const std::shared_ptr<const DirectedGraph>& graph)
{
  _graph = graph;
}

void ShortestPath::calculateCost()
{
  assert(_queue.empty() == true);

  for (QHash<long, double>::const_iterator it = _cost.begin(); it != _cost.end(); ++it)
  {
    _queue.push(Node(it.key(), it.value()));
  }

  const QMultiHash<long, DirectedGraph::Edge>& edges = _graph->getEdges();
  while (_queue.empty() == false)
  {
    Node top = _queue.top();
    _queue.pop();

    for (QMultiHash<long, DirectedGraph::Edge>::const_iterator it = edges.find(top.id);
         it != edges.end() && it.key() == top.id; ++it)
    {
      double c = top.cost + it.value().weight;
      long nId = it.value().to;
      if (_cost.contains(nId) == false || _cost[nId] > c)
      {
        _cost[nId] = c;
        _queue.push(Node(nId, c));
      }
    }
  }
}

}
