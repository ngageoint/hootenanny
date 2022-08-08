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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "DistanceIterator.h"

// Standard Includes
#include <cassert>
#include <cmath>

#include <tgs/RStarTree/RStarTree.h>
#include <tgs/RStarTree/RTreeNode.h>

namespace Tgs
{
  DistanceIterator::DistanceIterator(RStarTree* tree, const std::vector<double>& point,
    double distance)
    : nodeHits(1),
      distCalcs(0),
      _tree(tree),
      _point(point),
      _distance(distance),
      _done(false)
  {
    const RTreeNode* root = _tree->getRoot();
    _pendingNodes.push_back(root->getId());  //Begin traversal at root

    _populateNext();  //Traverse tree to gather nodes within range
  }

  double DistanceIterator::_calculateDistance(const std::vector<double>& point, const Box& box)
  {
    distCalcs++;
    // calculate a simple euclidean distance in n dimensional space
    assert((int)point.size() == box.getDimensions());
    double c = 0;
    for (unsigned int i = 0; i < point.size(); i++)
    {
      double a = 0;
      assert(box.getUpperBound(i) >= box.getLowerBound(i));
      if (point[i] > box.getUpperBound(i))
      {
        a = point[i] - box.getUpperBound(i);
      }
      else if (point[i] < box.getLowerBound(i))
      {
        a = point[i] - box.getLowerBound(i);
      }
      c += a * a;
    }
    return sqrt(c);
  }

  const Box& DistanceIterator::getBox() const
  {
    return _currentResult.box;
  }

  int DistanceIterator::getId() const
  {
    return _currentResult.id;
  }

  bool DistanceIterator::hasNext()
  {
    _populateNext();
    return !_done;
  }

  bool DistanceIterator::next()
  {
    bool result = false;
    _populateNext();
    if (!_pendingResults.empty())
    {
      _currentResult = _pendingResults.front();
      _pendingResults.pop_front();
      result = true;
    }
    return result;
  }

  void DistanceIterator::_populateNext()
  {
    while (_pendingResults.empty() && !_done)
    {
      if (_pendingNodes.empty()) //If the tree has been completely searched, stop
      {
        _done = true;
      }
      else
      {
        //Get the first node in the list to examine
        const RTreeNode* node = _tree->getNode(_pendingNodes.front());
        nodeHits++;
        _pendingNodes.pop_front(); //Remove node from future consideration
        for (int i = 0; i < node->getChildCount(); i++)
        {

          Box b = node->getChildEnvelope(i).toBox();
          double d = _calculateDistance(_point, b);
          if (d <= _distance)
          {
            if (node->isLeafNode())
            {
              _pendingResults.emplace_back(b, node->getChildUserId(i));
            }
            else
            {
              _pendingNodes.push_front(node->getChildNodeId(i));
            }
          }
        }
      }
    }
  }
}

