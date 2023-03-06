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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */

#include "KnnIterator.h"

// Standard
#include <queue>
#include <vector>
#include <cassert>
#include <climits>
#include <list>
#include <queue>
#include <set>

// tgs
#include <tgs/TgsException.h>
#include <tgs/RStarTree/MemoryPageStore.h>

namespace Tgs
{

static inline double sqr(double a) { return a * a; }

KnnIterator::KnnIterator(const RStarTree* tree, const double x, const double y, const Box& searchRegion)
{
  _searchTree = tree;
  _knnX = x;
  _knnY = y;

  assert(_searchTree->getRoot() != nullptr);

  _knnBounds = searchRegion;
  reset(x, y);
}

KnnIterator::~KnnIterator()
{
  // clean up our little memory mess
  while (!_nodeDistanceAllocationPool.empty())
  {
    NodeDistance* nd = _nodeDistanceAllocationPool.back();
    _nodeDistanceAllocationPool.pop_back();
    delete [] nd;
  }
}

double KnnIterator::_calculateDistance(const BoxInternalData& box, int) const
{
  return distPtToLine(_knnX, _knnY, box.getLowerBoundRaw(0), box.getLowerBoundRaw(1), box.getUpperBoundRaw(0), box.getUpperBoundRaw(1));
}

/**
* Calculate the next nearest neighbor, an id of -1 is returned if there are no more features.
*/
void KnnIterator::_calculateNextNn()
{
  double distance = -1;
  int id = RESERVED_ID;
  double tmpDistance;
  int tmpId;

  while (!_knnSearchQueue.empty() || !_knnLeafHeap.empty())
  {
    // get the next most likely area to search (minimum distance first)
    NodeDistance* nd = nullptr;
    const RTreeNode* currNode = nullptr;
    if (!_knnSearchQueue.empty())
    {
      nd = _knnSearchQueue.top();
      currNode = _searchTree->getNode(nd->id);
    }

    LeafDistance ld;
    // get rid of any leaves on our heap that have the same fids as results we've returned.
    if (!_knnLeafHeap.empty())
    {
      ld = _knnLeafHeap.top();
      while (!_knnLeafHeap.empty() && _knnReturnedFids.find(ld.fid) != _knnReturnedFids.end())
      {       
        _knnLeafHeap.pop();  
        if (!_knnLeafHeap.empty())
          ld = _knnLeafHeap.top();
      }
      // if we're all out of features
      if (_knnSearchQueue.empty() && _knnLeafHeap.empty())
        break;
    }
    // if we already have some leaf distances calculated, check to see if they're closer than
    // the node
    if (!_knnLeafHeap.empty() && (nd == nullptr || (ld.distance <= nd->minPossibleDistance)))
    {
      // if the leaf is closer than the next most likely node, return the result
      _knnLeafHeap.pop();
      distance = ld.distance;
      id = ld.fid;
      break;
    }

    if (nd == nullptr)
      throw Tgs::Exception("Internal Error: RTree::calculateNextNn This state should not occur.");

    ////
    // Process this nd
    ////

    _knnSearchQueue.pop();
    
    if (currNode != nullptr)
    {
      if (currNode->isLeafNode())
      {
        for (int i = 0; i < currNode->getChildCount(); i++)
        {
          const BoxInternalData& b = currNode->getChildEnvelope(i);
          tmpId = currNode->getChildUserId(i);
          if (_knnReturnedFids.find(tmpId) == _knnReturnedFids.end() &&
              (_knnBounds.isValid() == false || _knnBounds.calculateOverlap(b) > 0))
          {
            tmpDistance = _calculateDistance(b, tmpId);
            _knnLeafHeap.push(LeafDistance(tmpDistance, tmpId));
          }
        }
      }
      else
      {
        for (int i = 0; i < currNode->getChildCount(); i++)
        {
          const BoxInternalData& b = currNode->getChildEnvelope(i);
          tmpDistance = _calculateDistance(b);
          tmpId = currNode->getChildNodeId(i);
          if (_knnBounds.isValid() == false || _knnBounds.calculateOverlap(b) > 0)
            _knnSearchQueue.push(_createNodeDistance(tmpDistance, tmpId));
        }
      }
    }

    _releaseNodeDistance(nd);
  }

  _knnDistance = sqrt(distance);
  _knnId = id;
}

double KnnIterator::_calculateDistance(const BoxInternalData& box) const
{
  double x = _knnX;
  double y = _knnY;
  // force 2 dimensions
  double left = box.getLowerBoundRaw(0);
  double right = box.getUpperBoundRaw(0);
  double bottom = box.getLowerBoundRaw(1);
  double top = box.getUpperBoundRaw(1);

  if (x >= left)
  {
    if (x <= right)
    {
      if (y <= top)
      {
        if (y >= bottom)  // inside the box
          return 0;
        else              // directly below the box
          return sqr(bottom - y);
      }
      else                // directly above the box
        return sqr(y - top);
    } 
    else
    {
      if (y < bottom)   // lower right
        return euclideanDistance(x, y, right, bottom);
      else if (y > top) // upper right
        return euclideanDistance(x, y, right, top);
      else              // directly right
        return sqr(x - right);
    }
  }
  else
  {
    if (y < bottom)   // lower left
      return euclideanDistance(x, y, left, bottom);
    else if (y > top) // upper left
      return euclideanDistance(x, y, left, top);
    else              // directly left
      return sqr(left - x);
  }
}

KnnIterator::NodeDistance* KnnIterator::_createNodeDistance(double minPossibleDistance, int nodeId)
{
  if (_nodeDistancePool.empty())
  {
    NodeDistance* allocation = new NodeDistance[ALLOCATION_SIZE];
    _nodeDistanceAllocationPool.push_back(allocation);
    for (int i = ALLOCATION_SIZE - 1; i >= 0; i--)
      _nodeDistancePool.push_back(&(allocation[i]));
  }

  NodeDistance* nd = _nodeDistancePool.back();
  _nodeDistancePool.pop_back();

  nd->minPossibleDistance = minPossibleDistance;
  nd->id = nodeId;
  return nd;
}

void KnnIterator::_releaseNodeDistance(NodeDistance* nd)
{
  _nodeDistancePool.push_back(nd);
}

const Box& KnnIterator::getBox() const
{
  throw Tgs::Exception("Not Implemented.");
}

bool KnnIterator::next()
{  
  bool result = false;
  _calculateNextNn();
  if (getId() != RESERVED_ID)
  {
    _knnReturnedFids.insert(getId());
    result = true;
  }
  return result;
}

bool KnnIterator::hasNext() 
{
  _calculateNextNn();
  return (getId() != RESERVED_ID);
}

void KnnIterator::reset(const double x, const double y)
{
  //_knnPoint = point;
  _knnX = x;
  _knnY = y;
  _knnReturnedFids.clear();
  while (_knnSearchQueue.empty() == false)
  {
    _releaseNodeDistance(_knnSearchQueue.top());
    _knnSearchQueue.pop();
  }
  while (_knnLeafHeap.empty() == false)
    _knnLeafHeap.pop();

  _knnSearchQueue.push(_createNodeDistance(-1, _searchTree->getRoot()->getId()));
}

}
