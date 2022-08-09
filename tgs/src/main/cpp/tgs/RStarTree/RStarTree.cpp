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

#include "RStarTree.h"

// Standard
#include <cassert>
#include <cfloat>
#include <cmath>
#include <exception>
#include <iostream>

// tgs
#include <tgs/StreamUtils.h>
#include <tgs/TgsException.h>
#include <tgs/RStarTree/PageStore.h>
#include <tgs/RStarTree/RTreeNode.h>

using namespace std;

namespace Tgs
{

RStarTree::RStarTree(const std::shared_ptr<PageStore>& ps, int dimensions)
  : _store(dimensions, ps)
{
  const RTreeNode* root;

  if (ps->getPageCount() > 0)
  {
    _header = ps->getPage(0);
    _headerStruct = reinterpret_cast<Header*>(_header->getData());
    _dimensions = _headerStruct->dimensions;
    _rootId = _headerStruct->rootId;
    _height = _headerStruct->height;
    root = _getRoot();
  }
  else
  {
    _header = ps->createPage();
    _headerStruct = reinterpret_cast<Header*>(_header->getData());
    _headerStruct->dimensions = dimensions;
    _headerStruct->height = 0;
    _height = _headerStruct->height;
    assert(_header->getId() == 0);
    root = _store.createNode();
    _setRootId(root->getId());
    _header->setDirty();
  }

  assert((int)sizeof(Header) < ps->getPageSize());

  // Beckmann et. al. found that an m of 40% typically yields the best results.
  _mPercent = .4;
  _M = root->getMaxChildCount();
  _m = (int)(_M * _mPercent);
  _p = .3;
  _dimensions = dimensions;
}

void RStarTree::_addChild(RTreeNode* parent, const Box& b, int id)
{
  if (id < 0)
    parent->addNodeChild(_getNode(-id));
  else
    parent->addUserChild(b, id);
}

int RStarTree::_calculateNodeLevel(const RTreeNode* node) const
{
  int level = 0;
  while (node->getParentId() >= 0)
  {
    level++;
    node = _store.getNode(node->getParentId());
  }
  return level;
}

void RStarTree::_chooseSplitAxis(BoxVector& boxes) const
{
  double S = -1;
  int result = -1;
  for (int i = 0; i < _dimensions; i++)
  {
    _sortOnDimension(boxes, i);
    double thisS = 0;

    for (int k = 1; k <= _M - 2 * _m + 2; k++)
    {
      Box g1(_dimensions);
      Box g2(_dimensions);

      unsigned int j = 0;
      for (; j < (unsigned int)((_m - 1) + k); j++)
        g1.expand(boxes[j].box);

      for (; j < boxes.size(); j++)
        g2.expand(boxes[j].box);

      thisS += g1.calculateOverlap(g2);
    }
    if (thisS < S || i == 0)
    {
      S = thisS;
      result = i;
    }
  }

  if (result != _dimensions - 1)
    _sortOnDimension(boxes, result);
}

int RStarTree::_chooseSplitIndex(const BoxVector& boxes) const
{
  double minOverlap = 0;
  double minArea = 0;
  int result = 0;

  int t = _M - 2 * _m + 2;
  for (int k = 1; k <= t; k++)
  {
    Box g1(_dimensions);
    Box g2(_dimensions);

    int j = 0; 
    for (; j < (_m - 1) + k; j++)
      g1.expand(boxes[j].box);

    for (; j < (int)boxes.size(); j++)
      g2.expand(boxes[j].box);

    double overlap = g1.calculateOverlap(g2);
    double area = g1.calculateVolume() + g2.calculateVolume();
    if (k == 1 || (overlap < minOverlap || (_isClose(overlap, minOverlap) && area < minArea)))
    {
      result = (_m - 1) + k;
      minOverlap = overlap;
      minArea = area;
    }
  }
  return result;
}

RTreeNode* RStarTree::_chooseSubTree(const Box& b, int level)
{
  // Set N to be the root
  RTreeNode* N = _getRoot();

  int currentLevel = _getHeight();
  assert(level <= _getHeight());

  while (currentLevel > level)
  {
    assert(currentLevel != 0 && N->isLeafNode() == false);
    int id;

    const RTreeNode* firstChild = _store.getNode(N ->getChildNodeId(0));
    // if the childpointers in N point to leaves
    if (firstChild->isLeafNode()) // determine the minimum overlap cost, using this method appears to increase insert time by about 17x
      id = _findLeastOverlapEnlargement(N , b);
    else // choose the entry in N whose rectangle needs lease area enlargement to include the new data rectangle.
      id = _findLeastEnlargement(N , b);

    N  = _store.getNode(id);
    currentLevel--;
  }

  // return the best node for insertion.
  return N ;
}

int RStarTree::_findLeastEnlargement(const RTreeNode* node, const Box& b) const
{
  double minExpansion = DBL_MAX;
  int minId = -1;
  for (int i = 0; i < node->getChildCount(); i++)
  {
    double expansion = node->getChildEnvelope(i).calculateExpansion(b);
    if (expansion < minExpansion)
    {
      minExpansion = expansion;
      minId = node->getChildNodeId(i);
    }
  }
  return minId;
}

int RStarTree::_findLeastOverlapEnlargement(const RTreeNode* node, const Box& b) const
{
  double minOverlapExpansion = DBL_MAX;
  int minId = -1;
  for (int i = 0; i < node->getChildCount(); i++)
  {
    double overlapExpansion = 0.0;
    Box preB = node->getChildEnvelope(i).toBox();
    Box postB = node->getChildEnvelope(i).toBox().expand(b);
    for (int j = 0; j < node->getChildCount(); j++)
    {
      if (j == i)
        continue;
      const BoxInternalData& bid = node->getChildEnvelope(j);
      double pre = bid.calculateOverlap(preB);
      double post = bid.calculateOverlap(postB);
      overlapExpansion += post - pre;
    }
    if (overlapExpansion < minOverlapExpansion)
    {
      minOverlapExpansion = overlapExpansion;
      minId = node->getChildNodeId(i);
    }
  }
  return minId;
}

int RStarTree::_getHeight() const
{
  return _height;
}

RTreeNode* RStarTree::_getNode(int id)
{
  RTreeNode* node = _store.getNode(id);
  return node;
}

RTreeNode* RStarTree::_getRoot()
{
  return _store.getNode(_rootId);
}

void RStarTree::insert(const Box& b, int userId)
{
  if (userId < 0 || b.isValid() == false)
    throw Exception("Internal Error: Box or id are not valid.");
  _overflowedLevels.clear();
  _insert(b, userId, 0);
}

void RStarTree::_insert(const Box& b, int id, int level)
{
  // cout << "Inserting " << id << endl;
  // I1 Invoke ChooseSubtree, with the level as a parameter, to find an appropriate node N, in which
  // to place the new entry.
  RTreeNode* N = _chooseSubTree(b, level);

  // I2 If N has less than M entries, accomodate E in N.
  if (N->getChildCount() < N->getMaxChildCount())
    _addChild(N, b, id);
  else  // I2 If N has M entries, invoke OverflowTreatement with the level of N as a parameter.
  {
    // splits 'node' into 'N' and 'newNode'. These nodes contain everything from N
    RTreeNode* newNode = nullptr;
    int a = _overflowTreatment(N, newNode, level);
    
    // I3 If OverflowTreatment was called and a split was performed, propagate OverflowTreatment
    // upwards if necessary.
    if (a == Split)
    {
      // I3 if overflow caused root split, create a new root
      if (N->getParentId() == -1)
      {
        RTreeNode* newRoot = _store.createNode();
        _setRootId(newRoot->getId());
        _setHeight(_getHeight() + 1);
        newRoot->addNodeChild(N);
        newRoot->addNodeChild(newNode);
      }
      else
        _insert(newNode->calculateEnvelope(), -newNode->getId(), level + 1);
    }
    _insert(b, id, level);
  }

  // I4 Adjust all covering rectangles in the insertion path such that they are minimum bounding
  // boxes enclosing their children rectangles.
  _updateBounds(N);
}

bool RStarTree::_isClose(const double a, const double b) const
{ 
  return fabs(a - b) < 1e-6; 
}

int RStarTree::_overflowTreatment(RTreeNode* node, RTreeNode*& newNode, int level)
{
  // OT1 If the level is not the root level and this is the first call of OverflowTreatment in
  // the given level during the insertion of one data rectangle, then
  int result;
  if (level != 0 && _overflowedLevels.find(level) == _overflowedLevels.end())
  {
    // OT1 Invoke ReInsert
    _overflowedLevels.insert(level);
    _reinsert(node, level);
    result = Reinsert;
  }
  else
  {
    // OT1 Invoke Split
    _split(node, newNode);
    result = Split;
  }
  return result;
}

class Child
{
public:
  Box b;
  int id;

  Child() : id(0) { }

  Child(int id_, const BoxInternalData& b_) : b(b_.toBox()), id(id_) { }
};

using DistancePair = std::pair<double, int>;

class CompareDistancePairs
{
public:
  bool operator() (const DistancePair& a, const DistancePair& b) const
  {
    return a.first < b.first;
  }
};

void RStarTree::_reinsert(RTreeNode* node, int level)
{
  // compute distances of child node's centroids to envelope's centroids
  std::vector<DistancePair> distances;
  distances.resize(node->getChildCount());
  Box envelope = node->calculateEnvelope();
  for (int i = 0; i < node->getChildCount(); i++)
  {
    const BoxInternalData& b = node->getChildEnvelope(i);
    distances[i].first = b.calculateCentroidDistance(envelope);
    distances[i].second = i;
  }
  
  // sort entries in decreasing order
  sort(distances.begin(), distances.end(), CompareDistancePairs());

  std::vector<int> removalIndexes;
  std::vector<Child> removedChildren;
  removalIndexes.resize((int)(node->getChildCount() * _p + 0.5));
  removedChildren.resize(removalIndexes.size());
  // remove the first _p * node->getChildCount() entries
  for (unsigned int i = 0; i < removalIndexes.size(); i++)
  {
    int index = distances[i].second;
    removalIndexes[i] = index;
    removedChildren[i] = Child(node->getChildId(index), node->getChildEnvelope(index));
  }
  node->removeChildren(removalIndexes);

  _updateBounds(node);

  // invoke insert for each of the removed children.
  for (const auto& child : removedChildren)
    _insert(child.b, child.id, level);
}

void RStarTree::_setHeight(int height)
{
  _height = height;
  _headerStruct->height = _height;
  _header->setDirty();
}

void RStarTree::_setRootId(int id)
{
  _rootId = id;
  _headerStruct->rootId = id;
  _header->setDirty();
}

class BoxComparor
{
public:
  BoxComparor(int dimension)
  {
    _dimension = dimension;
  }

  bool operator()(const RStarTree::BoxPair& a, const RStarTree::BoxPair& b) const
  {
    // sort on lower bound then upper bound
    return (a.box.getLowerBound(_dimension) < b.box.getLowerBound(_dimension) ||
           (a.box.getLowerBound(_dimension) == b.box.getLowerBound(_dimension) &&
            a.box.getUpperBound(_dimension) < b.box.getUpperBound(_dimension)));
  }
private:
  int _dimension;
};

void RStarTree::_sortOnDimension(BoxVector& boxes, int dim)
{
  BoxComparor bc(dim);

  sort(boxes.begin(), boxes.end(), bc);
}

void RStarTree::_split(RTreeNode* node, RTreeNode*& newNode)
{
  assert(node->getChildCount() > 1);
  BoxVector boxes;
  boxes.reserve(node->getChildCount());
  bool leaf = node->isLeafNode();
  for (int i = 0; i < node->getChildCount(); i++)
  {
    if (leaf)
      boxes.emplace_back(node->getChildEnvelope(i), node->getChildUserId(i));
    else
      boxes.emplace_back(node->getChildEnvelope(i), node->getChildNodeId(i));
  }

  int splitIndex = _splitBoxes(boxes);
  newNode = _store.createNode();
  int parentId = node->getParentId();
  if (leaf)
  {
    std::vector<std::pair<Box, int>> tmp;
    tmp.reserve(boxes.size());
    // we need to copy the interesting data before we clear the node b/c boxes contains pointers
    // to BoxInternal which references node. More complicated than I'd like, but it is more
    // efficient.
    for (unsigned int i = 0; i < boxes.size(); i++)
      tmp.emplace_back(boxes[i].box.toBox(), boxes[i].id);

    node->clear();
    node->setParentId(parentId);
    // cout << "  splitting parent: " << node->getParentId() << endl;
    for (int i = 0; i < (int)boxes.size(); i++)
    {
      if (i < splitIndex)
        node->addUserChild(tmp[i].first, tmp[i].second);
      else
        newNode->addUserChild(tmp[i].first, tmp[i].second);
    }
  }
  else
  {
    node->clear();
    node->setParentId(parentId);
    // cout << "  splitting parent: " << node->getParentId() << endl;
    for (int i = 0; i < (int)boxes.size(); i++)
    {
      if (i < splitIndex)
        node->addNodeChild(_store.getNode(boxes[i].id));
      else
        newNode->addNodeChild(_store.getNode(boxes[i].id));
    }
  }
  assert(node->getChildCount() > 0 && newNode->getChildCount() > 0);
}

int RStarTree::_splitBoxes(BoxVector& boxes)
{
  _chooseSplitAxis(boxes);
  unsigned int splitIndex = _chooseSplitIndex(boxes);
  return splitIndex;
}

bool RStarTree::sanityCheck() const
{
  return _sanityCheck(getRoot());
}

bool RStarTree::_sanityCheck(const RTreeNode* node) const
{
  int count = node->getChildCount();
  int maxCount = node->getMaxChildCount();
  if (count < 0 || count > maxCount)
    return false;

  for (int i = 0; i < count; i++)
  {
    if (!node->isLeafNode())
    {
      const RTreeNode* child = getNode(node->getChildNodeId(i));
      if (_sanityCheck(child) == false)
        return false;
    }
    else
    {
      RTreeNode* n = const_cast<RTreeNode*>(node);
      if (n->getChildId(i) < 0)
        return false;
    }
  }
  return true;
}

void RStarTree::_updateBounds(RTreeNode* node)
{
  int parentId = node->getParentId();
  // propagate the increased bounding box up the nodes
  while (parentId > 0)
  {
    int childId = node->getId();
    Box childBox = node->calculateEnvelope();
    node = _store.getNode(parentId);
    int childIndex = node->convertChildIdToIndex(childId);
    node->updateChildEnvelope(childIndex, childBox);
    parentId = node->getParentId();
  }
}

}
