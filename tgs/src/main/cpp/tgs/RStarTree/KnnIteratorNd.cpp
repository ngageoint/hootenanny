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
 * @copyright Copyright (C) 2015, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "KnnIteratorNd.h"

// Standard Includes
#include <queue>
#include <vector>

// Standard Includes
#include <limits.h>
#include <list>
#include <queue>
#include <set>

#include <tgs/TgsException.h>

using namespace Tgs;

static inline double min(double a, double b) { return a < b ? a : b; }
static inline double max(double a, double b) { return a > b ? a : b; }
static inline double sqr(double a) { return a * a; }

KnnIteratorNd::KnnIteratorNd(const RStarTree* tree, const std::vector<double>& point,
  const Box& searchRegion)
{
  _searchTree = tree;

  assert(_searchTree->getRoot() != NULL);

  _knnBounds = searchRegion;
  reset(point);
}

KnnIteratorNd::~KnnIteratorNd()
{
  // clean up our little memory mess
  while (_nodeDistanceAllocationPool.size() > 0)
  {
    NodeDistance* nd = _nodeDistanceAllocationPool.back();
    _nodeDistanceAllocationPool.pop_back();
    delete [] nd;
  }
}

/**
* Calculate the next nearest neighbor, an id of -1 is returned if there are no more features.
*/
void KnnIteratorNd::_calculateNextNn()
{
  double distance = -1;
  int id = RESERVED_ID;
  double tmpDistance;
  int tmpId;

  while (_knnSearchQueue.size() > 0 || _knnLeafHeap.size() > 0)
  {
    // get the next most likely area to search (minimum distance first)
    NodeDistance* nd = NULL;
    const RTreeNode * currNode = NULL;
    if (_knnSearchQueue.size() != 0)
    {
      nd = _knnSearchQueue.top();
      currNode = _searchTree->getNode(nd->id);
    }

    LeafDistance ld;
    // get rid of any leaves on our heap that have the same fids as results we've returned.
    if (_knnLeafHeap.size() > 0)
    {
      ld = _knnLeafHeap.top();
      while (_knnLeafHeap.size() > 0 && _knnReturnedFids.find(ld.fid) != _knnReturnedFids.end())
      {       
        _knnLeafHeap.pop();  
        if (_knnLeafHeap.size() > 0)
          ld = _knnLeafHeap.top();
      }

      if (_knnSearchQueue.size() == 0 && _knnLeafHeap.size() == 0)
      {
        // if we're all out of features
        break;
      }
    }

    // if we already have some leaf distances calculated, check to see if they're closer than
    // the node
    if (_knnLeafHeap.size() > 0)
    {
      if (nd == NULL || (ld.distance <= nd->minPossibleDistance && currNode != NULL && !currNode->isLeafNode()))
      {
        // if the leaf is closer than the next most likely node, return the result
        _knnLeafHeap.pop();
        distance = ld.distance;
        id = ld.fid;
        break;
      }
    }

    if (nd == NULL)
    {
      throw Exception("Internal Error: RTree::calculateNextNn This state should not occur.");
    }

    ////
    // Process this nd
    ////

    _knnSearchQueue.pop();
    
    if (currNode != NULL)
    {
      if (currNode->isLeafNode())
      {
        for (int i=0; i<currNode->getChildCount(); i++)
        {
          const BoxInternalData& b = currNode->getChildEnvelope(i);
          tmpId = currNode->getChildUserId(i);
          if (_knnReturnedFids.find(tmpId) == _knnReturnedFids.end())
          {
            if (_knnBounds.isValid() == false || _knnBounds.calculateOverlap(b) > 0)
            {
              tmpDistance = _calculateDistance(_knnPoint, b);
              _knnLeafHeap.push(LeafDistance(tmpDistance, tmpId));
            }
          }
        }
      }
      else
      {
        for (int i=0; i<currNode->getChildCount(); i++)
        {
          const BoxInternalData& b = currNode->getChildEnvelope(i);
          tmpDistance = _calculateDistance(_knnPoint, b);
          tmpId = currNode->getChildNodeId(i);
          if (_knnBounds.isValid() == false || _knnBounds.calculateOverlap(b) > 0)
          {
            _knnSearchQueue.push(_createNodeDistance(tmpDistance, tmpId));
          }
        }
      }
    }
    
    _releaseNodeDistance(nd);
  }

  _knnDistance = sqrt(distance);
  _knnId = id;
}

// /**
// * Calculate the next nearest neighbor, an id of -1 is returned if there are no more features.
// */
// void KnnIteratorNd::_calculateNextNn()
// {
//   double distance = -1;
//   int id = RESERVED_ID;
// 
//   while (_knnSearchQueue.size() > 0)
//   {
//     // get the next most likely area to search (minimum distance first)
//     NodeDistance* nd = NULL;
//     const RTreeNode * currNode;
//     if (_knnSearchQueue.size() != 0)
//     {
//       nd = _knnSearchQueue.top();
//       currNode = _searchTree->getNode(nd->id);
//       _nodesVisited++;
//     }
// 
//     if (_knnSearchQueue.size() == 0)
//     {
//       // if we're all out of features
//       break;
//     }
// 
//     // if we already have some leaf distances calculated, check to see if they're closer than
//     // the node
//     if (distance >= 0)
//     {
//       if (nd == NULL || (distance <= nd->minPossibleDistance && !currNode->isLeafNode()))
//       {
//         break;
//       }
//     }
// 
//     if (nd == NULL)
//     {
//       throw Exception("Internal Error: RTree::calculateNextNn This state should not occur.");
//     }
// 
//     ////
//     // Process this nd
//     ////
// 
//     _knnSearchQueue.pop();
// 
//     if (currNode->isLeafNode())
//     {
//       for (int i=0; i<currNode->getChildCount(); i++)
//       {
//         const BoxInternalData& b = currNode->getChildEnvelope(i);
//         double tmpDistance;
//         int tmpId = currNode->getChildUserId(i);
//         if (_knnBounds.isValid() == false || _knnBounds.calculateOverlap(b) > 0)
//         {
//           tmpDistance = distPtToLine(_knnPoint.x, _knnPoint.y, b.getLowerBoundRaw(0), b.getLowerBoundRaw(1), b.getUpperBoundRaw(0), b.getUpperBoundRaw(1));
//           if (distance < 0.0 || tmpDistance < distance || (tmpDistance == distance && tmpId > id))
//           {
//             distance = tmpDistance;
//             id = tmpId;
//           }
//         }
//       }
//     }
//     else
//     {
//       for (int i=0; i<currNode->getChildCount(); i++)
//       {
//         const BoxInternalData& b = currNode->getChildEnvelope(i);
//         double d = _calculateDistance(_knnPoint, b);
//         int childId = currNode->getChildNodeId(i);
//         if (_knnBounds.isValid() == false || _knnBounds.calculateOverlap(b) > 0)
//         {
//           _knnSearchQueue.push(_createNodeDistance(d, childId));
//         }      
//       }
//     }
// 
//     _releaseNodeDistance(nd);
//   }
// 
// 
//   _knnDistance = sqrt(distance);
//   _knnId = id;
// }
// 

double KnnIteratorNd::_calculateDistance(const std::vector<double>& point, 
  const BoxInternalData& box) const
{
  assert((int)point.size() == box.getDimensions());
  _tmp.resize(point.size());
  for (unsigned int i = 0; i < point.size(); i++)
  {
    if (point[i] < box.getLowerBound(i))
    {
      _tmp[i] = box.getLowerBound(i);
    }
    else if (point[i] > box.getUpperBound(i))
    {
      _tmp[i] = box.getUpperBound(i);
    }
    else
    {
      _tmp[i] = point[i];
    }
  }
  return euclideanDistance(point, _tmp);
}

KnnIteratorNd::NodeDistance* KnnIteratorNd::_createNodeDistance(double minPossibleDistance, int nodeId)
{
  if (_nodeDistancePool.size() == 0)
  {
    NodeDistance* allocation = new NodeDistance[ALLOCATION_SIZE];
    _nodeDistanceAllocationPool.push_back(allocation);
    for (int i = ALLOCATION_SIZE - 1; i >= 0; i--)
    {
      _nodeDistancePool.push_back(&(allocation[i]));
    }
  }

  NodeDistance* nd = _nodeDistancePool.back();
  _nodeDistancePool.pop_back();

  nd->minPossibleDistance = minPossibleDistance;
  nd->id = nodeId;
  return nd;
}

void KnnIteratorNd::_releaseNodeDistance(NodeDistance* nd)
{
  _nodeDistancePool.push_back(nd);
}

const Box& KnnIteratorNd::getBox() const
{
  throw Tgs::Exception("Not Implemented.");
}

bool KnnIteratorNd::next()
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

bool KnnIteratorNd::hasNext() 
{
  _calculateNextNn();
  return (getId() != RESERVED_ID);
}

void KnnIteratorNd::reset(const std::vector<double>& point)
{
  _knnPoint = point;
  _knnReturnedFids.clear();
  while (_knnSearchQueue.empty() == false)
  {
    _releaseNodeDistance(_knnSearchQueue.top());
    _knnSearchQueue.pop();
  }
  while (_knnLeafHeap.empty() == false)
  {
    _knnLeafHeap.pop();
  }
  _knnSearchQueue.push(_createNodeDistance(-1, _searchTree->getRoot()->getId()));
}
