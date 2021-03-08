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

#include "IntersectionIterator.h"

// Standard Includes
#include <cassert>
#include <cmath>
#include <iomanip>

#include <tgs/RStarTree/RStarTree.h>
#include <tgs/RStarTree/RTreeNode.h>

namespace Tgs
{
  IntersectionIterator::IntersectionIterator(const RStarTree* tree,
                                             const std::vector<double>& minBounds,
                                             const std::vector<double>& maxBounds)
    : _tree(tree),
      _minBounds(minBounds),
      _maxBounds(maxBounds),
      _done(false)
  {
    const RTreeNode* root = _tree->getRoot();
    _pendingNodes.push_back(root->getId());  //Begin traversal at root

    _populateNext();  //Traverse tree to gather nodes within range
  }

  bool IntersectionIterator::_determineIntesection(const Box& box)
  {
    // calculate a simple euclidean distance in n dimensional space
    assert((int)_maxBounds.size() == box.getDimensions());

    for (unsigned int i = 0; i < _maxBounds.size(); i++)
    {
      assert(box.getUpperBound(i) >= box.getLowerBound(i));

      if (_minBounds[i] > box.getUpperBound(i) || box.getLowerBound(i) > _maxBounds[i])
      {
        return false;
      }
    }
    return true;
  }

  const Box& IntersectionIterator::getBox() const
  {
    return _currentResult.box;
  }

  int IntersectionIterator::getId() const
  {
    return _currentResult.id;
  }

  bool IntersectionIterator::hasNext()
  {
    _populateNext();
    return !_done;
  }

  bool IntersectionIterator::next()
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

  void IntersectionIterator::_populateNext()
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
        _pendingNodes.pop_front(); //Remove node from future consideration
        for (int i = 0; i < node->getChildCount(); i++)
        {
          Box b = node->getChildEnvelope(i).toBox();
          bool intersects = _determineIntesection(b);
          if (intersects)
          {
            if (node->isLeafNode())
            {
              _pendingResults.push_back(Result(b, node->getChildUserId(i)));
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

