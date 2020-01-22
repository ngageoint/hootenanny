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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "HilbertRTree.h"

// Standard Includes
#include <cassert>
#include <cfloat>
#include <cmath>
#include <exception>
#include <iostream>
using namespace std;

// Tgs
#include <tgs/RStarTree/HilbertCurve.h>
#include <tgs/RStarTree/PageStore.h>
#include <tgs/RStarTree/RTreeNode.h>
#include <tgs/Statistics/Random.h>

using namespace Tgs;

HilbertRTree::HilbertRTree(const std::shared_ptr<PageStore>& ps, int dimensions) :
  RStarTree(ps, dimensions)
{
  _hilbertCurve = new HilbertCurve(dimensions, 8);
}

HilbertRTree::~HilbertRTree()
{
  delete _hilbertCurve;
}

void HilbertRTree::bulkInsert(const std::vector<Box>& boxes, const std::vector<int>& fids)
{
  //std::cout << "Child count: " << getRoot()->getChildCount() << std::endl;
  //std::cout << "boxes size: " << boxes.size() << std::endl;
  //std::cout << "fids size: " << fids.size() << std::endl;
  assert(getRoot()->getChildCount() == 0);
  assert(boxes.size() == fids.size());
  std::vector<UserBoxHolder> sortedBoxes;
  _calculateHilbertValues(boxes, fids, sortedBoxes);

  std::vector<int> childNodes;
  _createLeafNodes(sortedBoxes, childNodes);
  _setHeight(0);
  //std::cout << "Child nodes count: " << childNodes.size() << std::endl;

  //std::cout << "Creating parent nodes..." << std::endl;
  while (childNodes.size() != 1)
  {
    std::vector<int> parentNodes;
    _createParentNodes(childNodes, parentNodes);
    childNodes = parentNodes;
    _setHeight(_getHeight() + 1);
  }
  _setRootId(childNodes[0]);
}

void HilbertRTree::_calculateHilbertValues(const std::vector<Box>& boxes,
  const std::vector<int>& fids, std::vector<UserBoxHolder>& hilbertBoxes)
{
  //std::cout << "Calculating Hilbert values..." << std::endl;

  assert(hilbertBoxes.size() == 0);
  HilbertCurve hilbertCurve(_dimensions, 16);

  hilbertBoxes.reserve(boxes.size());
  Box bounds(_dimensions);
  for (unsigned int i = 0; i < boxes.size(); i++)
  {
    bounds.expand(boxes[i]);
  }

  double boundsWidth[Box::MAX_DIMENSIONS];
  for (int i = 0; i < bounds.getDimensions(); i++)
  {
    boundsWidth[i] = bounds.getUpperBoundRaw(i) - bounds.getLowerBoundRaw(i);
  }

  int point[Box::MAX_DIMENSIONS];
  for (unsigned int i = 0; i < boxes.size(); i++)
  {
    assert(fids[i] >= 0);

    const Box& box = boxes[i];
    for (int j = 0; j < box.getDimensions(); j++)
    {
      double v = (box.getLowerBoundRaw(j) + box.getUpperBoundRaw(j)) / 2.0;
      if (boundsWidth[j] != 0)
      {
        point[j] = int(((1 << ORDER) - 1) * ((v - bounds.getLowerBoundRaw(j)) / boundsWidth[j]));
      }
      else
      {
        point[j] = 0;
      }
      assert(point[j] >= 0 && point[j] < (1 << ORDER));
    }

    hilbertBoxes.push_back(UserBoxHolder(boxes[i], fids[i], _hilbertCurve->encode(point)));
  }
  sort(hilbertBoxes.begin(), hilbertBoxes.end());
}

double HilbertRTree::_calculateArea(int parentId)
{
  const RTreeNode* parent = _getNode(parentId);
  double result = 0.0;
  for (int i = 0; i < parent->getChildCount(); i++)
  {
    result += parent->getChildEnvelope(i).calculateVolume();
  }
  if (parent->isLeafNode() == false)
  {
    for (int i = 0; i < parent->getChildCount(); i++)
    {
      result += _calculateArea(parent->getChildNodeId(i));
    }
  }

  return result;
}

double HilbertRTree::_calculateOverlap(int parentId)
{
  const RTreeNode* parent = _getNode(parentId);
  std::vector<double> tmp;
  double result = _calculatePairwiseOverlap(parentId, tmp);
  if (parent->isLeafNode() == false)
  {
    for (int i = 0; i < parent->getChildCount(); i++)
    {
      result += _calculateOverlap(parent->getChildNodeId(i));
    }
  }

  return result;
}

double HilbertRTree::_calculatePairwiseOverlap(int parentId, std::vector<double>& overlaps)
{
  double result = 0.0;
  const RTreeNode* parent = _getNode(parentId);
  overlaps.resize(parent->getChildCount());
  for (int i = 0; i < parent->getChildCount(); i++)
  {
    overlaps[i] = 0.0;
    for (int j = 0; j < parent->getChildCount(); j++)
    {
      if (i != j)
      {
        // TODO: remove toBox and implement overlap calculation for BoxInternal
        double o = parent->getChildEnvelope(j).calculateOverlap(
          parent->getChildEnvelope(i).toBox());
        overlaps[i] += o;
        result += o;
      }
    }
  }
  return result;
}

int HilbertRTree::_chooseWeightedChild(const std::vector<double>& weights)
{
  assert(weights.size() > 0);
  int result = -1;
  double sum = 0.0;
  for (unsigned int i = 0; i < weights.size(); i++)
  {
    sum += weights[i];
  }
  if (sum == 0.0)
  {
    return Tgs::Random::instance()->generateInt(weights.size());
  }
  double r = Tgs::Random::instance()->generateUniform() * sum;
  double incr = 0.0;
  for (unsigned int i = 0; i < weights.size(); i++)
  {
    incr += weights[i];
    if (r <= incr)
      return i;
  }
  assert(result != -1);
  return result;
}

void HilbertRTree::_createLeafNodes(const std::vector<UserBoxHolder>& hilbertBoxes,
  std::vector<int>& result)
{
  //std::cout << "                                                            Creating leaf nodes..." << std::endl;

  int maxChildCount = getRoot()->getMaxChildCount();
  //std::cout << "maxChildCount: " << maxChildCount << std::endl;
  result.reserve((int)ceil((float)hilbertBoxes.size() / (float)maxChildCount));
  RTreeNode* node = _getRoot();
  //std::cout << "node == 0: " << (node == 0) << std::endl;
  //std::cout << "node id: " << node->getId() << std::endl;
  //std::cout << "node child count: " << node->getChildCount() << std::endl;
  result.push_back(node->getId());

  //int numProcessed = 0;

  for (unsigned int i = 0; i < hilbertBoxes.size(); i++)
  {
    if (node->getChildCount() == maxChildCount)
    {
      node = _store.createNode();
      //std::cout << "node == 0: " << (node == 0) << std::endl;
      //std::cout << "node id: " << node->getId() << std::endl;
      result.push_back(node->getId());
    }
    node->addUserChild(*hilbertBoxes[i].box, hilbertBoxes[i].fid);

//    numProcessed++;
//    if (numProcessed % 100000 == 0)
//    {
//      std::cout << "                              Created " << numProcessed << " / " <<
//        hilbertBoxes.size() << " leaf nodes." << std::endl;
//    }
  }
}

void HilbertRTree::_createParentNodes(const std::vector<int>& childNodes,
  std::vector<int>& result)
{
  int maxChildCount = getRoot()->getMaxChildCount();
  //std::cout << "maxChildCount: " << maxChildCount << std::endl;
  result.reserve((int)ceil((float)childNodes.size() / (float)maxChildCount));
  RTreeNode* node = _store.createNode();
  //std::cout << "node == 0: " << (node == 0) << std::endl;
  //std::cout << "node id: " << node->getId() << std::endl;
  //std::cout << "node child count: " << node->getChildCount() << std::endl;
  result.push_back(node->getId());

  //int numProcessed = 0;

  for (unsigned int i = 0; i < childNodes.size(); i++)
  {
    if (node->getChildCount() == maxChildCount)
    {
      node = _store.createNode();
//      std::cout << "node == 0: " << (node == 0) << std::endl;
//      std::cout << "node id: " << node->getId() << std::endl;
      result.push_back(node->getId());
    }
    node->addNodeChild(_getNode(childNodes[i]));

//    numProcessed++;
//    if (numProcessed % 1000 == 0)
//    {
//      std::cout << "                                                            Created " <<
//        numProcessed << " / " << childNodes.size() << " parent nodes." << std::endl;
//    }
  }
}

void HilbertRTree::greedyShuffle()
{
  _shuffleSize = _getRoot()->getMaxChildCount();
//   double startingOverlap = _calculateOverlap(_getRootId());
  for (int i = 0; i < 10; i++)
  {
    _greedyShuffle(_getRootId());
  }
}

void HilbertRTree::_greedyShuffle(int parentId)
{
  std::vector<double> tmp;
  RTreeNode* parent = _getNode(parentId);
  // if we're a leaf, no shuffling is possible, return the overlap
  if (parent->isLeafNode())
  {
    return;
  }

  // calculate the overlaps for each child, this tells us which ones need more balancing.
  std::vector<double> overlaps;
  for (int i = 0; i < parent->getChildCount(); i++)
  {
    double overlap = _calculatePairwiseOverlap(parent->getChildNodeId(i), tmp);
    overlaps.push_back(overlap);
  }
  // randomly choose child based on overlap weight
  for (int i = 0; i < _shuffleSize; i++)
  {
    // recursively call greedyShuffle on children
    int childId = parent->getChildNodeId(_chooseWeightedChild(overlaps));
    _greedyShuffle(childId);
  }

  // swap children two levels down
  if (parent->getChildCount() >= 2)
  {
    // randomly choose child based on overlap weight
    for (int i = 0; i < _shuffleSize; i++)
    {
      _swapGrandChildNodes(parentId, overlaps);
    }
  }
}

class BoxHolder
{
public:

  HilbertRTree::BoxPair boxPair;
  int hilbertValue;

  BoxHolder(const BoxHolder& bh) :
    boxPair(bh.boxPair)
  {
    hilbertValue = bh.hilbertValue;
  }

  BoxHolder(const HilbertRTree::BoxPair& bp, int hv) :
    boxPair(bp)
  {
    hilbertValue = hv;
  }

  bool operator<(const BoxHolder& b)
  {
    return hilbertValue < b.hilbertValue;
  }

  BoxHolder& operator=(const BoxHolder& bh)
  {
    if (this != &bh)
    {
      boxPair = bh.boxPair;
      hilbertValue = bh.hilbertValue;
    }
    return *this;
  }
};

int HilbertRTree::_splitBoxes(BoxVector& boxes)
{
  Box bounds(_dimensions);
  for (unsigned int i = 0; i < boxes.size(); i++)
  {
    bounds.expand(boxes[i].box);
  }
  double boundsWidth[Box::MAX_DIMENSIONS];
  for (int i = 0; i < bounds.getDimensions(); i++)
  {
    boundsWidth[i] = bounds.getUpperBoundRaw(i) - bounds.getLowerBoundRaw(i);
  }

  std::vector<BoxHolder> hilbertBoxes;
  int point[Box::MAX_DIMENSIONS];
  hilbertBoxes.reserve(boxes.size());
  for (unsigned int i = 0; i < boxes.size(); i++)
  {
    BoxInternalData& box = boxes[i].box;
    for (int j = 0; j < box.getDimensions(); j++)
    {
      double v = (box.getLowerBoundRaw(j) + box.getUpperBoundRaw(j)) / 2.0;
      if (boundsWidth[j] == 0)
      {
        point[j] = 0;
      }
      else
      {
        point[j] = int(((1 << ORDER) - 1) * ((v - bounds.getLowerBoundRaw(j)) / boundsWidth[j]));
      }
      assert(point[j] >= 0 && point[j] < (1 << ORDER));
    }

    hilbertBoxes.push_back(BoxHolder(boxes[i], _hilbertCurve->encode(point)));
  }
  //sort(hilbertBoxes.begin(), hilbertBoxes.end());
  bool swap = false;
  do
  {
    swap = false;
    for (unsigned int i = 0; i < hilbertBoxes.size() - 1; i++)
    {
      if (hilbertBoxes[i + 1] < hilbertBoxes[i])
      {
        BoxHolder tmp = hilbertBoxes[i];
        hilbertBoxes[i] = hilbertBoxes[i + 1];
        hilbertBoxes[i + 1] = tmp;
        swap = true;
      }
    }
  } while (swap);

  for (unsigned int i = 0; i < boxes.size(); i++)
  {
    boxes[i] = hilbertBoxes[i].boxPair;
  }

  return _chooseSplitIndex(boxes);
}

double HilbertRTree::_sum(const std::vector<double>& v)
{
  double result = 0.0;
  for (unsigned int i = 0; i < v.size(); i++)
  {
    result += v[i];
  }
  return result;
}

double HilbertRTree::_swapGrandChildNodes(int parentId, const std::vector<double>& overlaps)
{
  RTreeNode* parent = _getNode(parentId);
  assert(parent->isLeafNode() == false);
  assert(parent->getChildCount() >= 2);

  // if there are no overlaps, there is nothing to do.
  if (_sum(overlaps) == 0.0)
  {
    return 0.0;
  }

  // pick a child to swap
  int childIndex = _chooseWeightedChild(overlaps);
  int childId = parent->getChildNodeId(childIndex);
  RTreeNode* child = _getNode(childId);
  int grandChildOffset = Tgs::Random::instance()->generateInt(child->getChildCount());

  std::vector<double> tmp = overlaps;
  tmp[childIndex] = 0.0;
  // pick a different child to swap
  int otherChildIndex = _chooseWeightedChild(tmp);
  int otherChildId = parent->getChildNodeId(otherChildIndex);
  RTreeNode* otherChild = _getNode(otherChildId);
  int otherGrandChildOffset = Tgs::Random::instance()->generateInt(otherChild->getChildCount());
  int otherGrandChildId = otherChild->getChildId(otherGrandChildOffset);
  Box otherGrandChildBox = otherChild->getChildEnvelope(otherGrandChildOffset).toBox();

  if (otherChildId == childId)
  {
    return 0.0;
  }

  double startOverlap = _calculatePairwiseOverlap(parentId, tmp) +
    _calculatePairwiseOverlap(childId, tmp) +
    _calculatePairwiseOverlap(otherChildId, tmp);

//   std::cout << "Swapping(" << parentId << ") " << childId << ":" << childIndex << " " <<
//     otherChildId << ":" << otherChildIndex << std::endl;

  double startVolume = child->calculateEnvelope().calculateVolume() +
    otherChild->calculateEnvelope().calculateVolume();
//   double startPerimeter = child->calculateEnvelope().calculatePerimeter() +
//     otherChild->calculateEnvelope().calculatePerimeter();

  // swap the children.
  otherChild->updateChild(otherGrandChildOffset, child->getChildId(grandChildOffset),
    child->getChildEnvelope(grandChildOffset).toBox());
  if (otherChild->isLeafNode() == false)
  {
    RTreeNode* otherGrandChild = _getNode(otherChild->getChildNodeId(otherGrandChildOffset));
//     std::cout << "Parent1 " << otherGrandChild->getParentId() << std::endl;
    otherGrandChild->setParentId(otherChildId);
//     std::cout << "Parent1 " << otherGrandChild->getParentId() << std::endl;
  }

  child->updateChild(grandChildOffset, otherGrandChildId, otherGrandChildBox);
  if (child->isLeafNode() == false)
  {
    RTreeNode* grandChild = _getNode(child->getChildNodeId(grandChildOffset));
//     std::cout << "Parent2 " << grandChild->getParentId() << std::endl;
    grandChild->setParentId(childId);
//     std::cout << "Parent2 " << grandChild->getParentId() << std::endl;
  }
  _updateBounds(child);
  _updateBounds(otherChild);

  double endVolume = child->calculateEnvelope().calculateVolume() +
    otherChild->calculateEnvelope().calculateVolume();
//   double endPerimeter = child->calculateEnvelope().calculatePerimeter() +
//     otherChild->calculateEnvelope().calculatePerimeter();

  double endOverlap = _calculatePairwiseOverlap(parentId, tmp) +
    _calculatePairwiseOverlap(childId, tmp) +
    _calculatePairwiseOverlap(otherChildId, tmp);

  double result;
  //std::cout << startOverlap << " " << endOverlap << "\t";
//   if (startOverlap * startPerimeter * startVolume >
//     endOverlap * endPerimeter * endVolume)
  if (startVolume > endVolume)
  {
    result = startOverlap - endOverlap;
    //std::cout << ".";
  }
  else
  {
    // swap the children back.
    child->updateChild(grandChildOffset, otherChild->getChildId(otherGrandChildOffset),
      otherChild->getChildEnvelope(otherGrandChildOffset).toBox());
    if (child->isLeafNode() == false)
    {
      RTreeNode* grandChild = _getNode(child->getChildNodeId(grandChildOffset));
//       std::cout << " Parent1 " << grandChild->getParentId() << std::endl;
      assert(grandChild->getParentId() == otherChildId);
//       std::cout << childId << std::endl;
      grandChild->setParentId(childId);
//       std::cout << " Parent1 " << grandChild->getParentId() << std::endl;
    }
    otherChild->updateChild(otherGrandChildOffset, otherGrandChildId, otherGrandChildBox);
    if (child->isLeafNode() == false)
    {
      RTreeNode* otherGrandChild = _getNode(otherChild->getChildNodeId(otherGrandChildOffset));
      assert(otherGrandChild->getParentId() == childId);
//       std::cout << " Parent2 " << otherGrandChild->getParentId() << std::endl;
      otherGrandChild->setParentId(otherChildId);
//       std::cout << " Parent2 " << otherGrandChild->getParentId() << std::endl;
    }

    _updateBounds(child);
    _updateBounds(otherChild);

    result = 0.0;
  }

  return result;
}
