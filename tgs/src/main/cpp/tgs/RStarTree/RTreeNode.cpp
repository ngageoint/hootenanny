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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "RTreeNode.h"

// Standard Includes
#include <algorithm>
#include <assert.h>
#include <math.h>

#include <tgs/TgsException.h>
#include <tgs/RStarTree/RTreeNodeStore.h>

using namespace std;
using namespace Tgs;

BoxInternalData::BoxInternalData(int dimensions, const char* data)
{
  _dimensions = dimensions;
  _data = data;
}

BoxInternalData::BoxInternalData(int dimensions, const char* data, const Box& b)
{
  _dimensions = dimensions;
  _data = data;

  assert(b.getDimensions() == _dimensions);
  for (int i = 0; i < _dimensions; i++)
  {
    setBounds(i, b.getLowerBoundRaw(i), b.getUpperBoundRaw(i));
  }
}

double BoxInternalData::calculateCentroidDistance(const Box& b) const
{
  assert(getDimensions() == b.getDimensions());
  double result = 0.0;
  for (int i = 0; i < getDimensions(); i++)
  {
    double a = (getUpperBound(i) + getLowerBound(i)) / 2.0;
    double a2 = (b.getUpperBound(i) + b.getLowerBound(i)) / 2.0;
    double c = a - a2;
    result += c * c;
  }
  result = sqrt(result);
  return result;
}

double BoxInternalData::calculateExpansion(const Box& b) const
{
  double v1 = calculateVolume();
  double v2 = 1;
  assert(b.getDimensions() == getDimensions());
  for (int i = 0; i < _dimensions; i++)
  {
    v2 *=
      std::max(getUpperBound(i), b.getUpperBound(i)) -
      std::min(getLowerBound(i), b.getLowerBound(i));
  }
  return v2 - v1;
}

double BoxInternalData::calculateOverlap(const Box& b) const
{
  assert(b.getDimensions() == getDimensions());
  Box overlap(_dimensions);
  for (int i = 0; i < _dimensions; i++)
  {
    double ub = std::min(getUpperBound(i), b.getUpperBound(i));
    double lb = std::max(getLowerBound(i), b.getLowerBound(i));
    if (ub - lb <= 0)
    {
      return 0.0;
    }
    overlap.setBounds(i, lb, ub);
  }
  return overlap.calculateVolume();
}

double BoxInternalData::calculateVolume() const
{
  double result = 1;
  for (int i = 0; i < _dimensions; i++)
  {
    result *= getUpperBound(i) - getLowerBound(i);
  }
  return result;
}

int BoxInternalData::getDimensions() const
{
  return _dimensions;
}

void BoxInternalData::setBounds(int d, double lower, double upper) const
{
  double* v = (double*)_data;
  v[d * 2] = lower;
  v[d * 2 + 1] = upper;
}

int BoxInternalData::size(int dimensions)
{
  return sizeof(double) * dimensions * 2;
}

Box BoxInternalData::toBox() const
{
  Box b(_dimensions);
  for (int i = 0; i < _dimensions; i++)
  {         
    b.setBounds(i, getLowerBoundRaw(i), getUpperBoundRaw(i));
  }
  return b;
}
double BoxInternalData::getLowerBound(int d) const
{ 
  const double* v = (const double*)_data;
  return std::min(v[d * 2],v[d * 2 + 1]);
}

double BoxInternalData::getUpperBound(int d) const
{ 
  const double* v = (const double*)_data;
  return std::max(v[d * 2],v[d * 2 + 1]);
}

bool BoxInternalData::isContained(const Box& b) const
{
  bool result = true;
  for (int i = 0; i < _dimensions; i++)
  {
    result = result && (b.getLowerBound(i) >= getLowerBound(i));
    result = result && (b.getUpperBound(i) <= getUpperBound(i));
  }
  return result;
}

RTreeNode::RTreeNode(int dimensions, const std::shared_ptr<Page>& page)
{
  _dimensions = dimensions;
  _pageSp = page;
  _page = page.get();
  _id = page->getId();


  _maxChildCount = (_page->getDataSize() - _getHeaderSize()) / _getChildSize();
  if (_maxChildCount < 2)
  {
    throw Exception("Internal Error: page is not large enough to support two children.");
  }
  
  int* childCount = (int*)_page->getData();
  _getHeader()->childCount = *childCount;
}

void RTreeNode::addChild(const Box& envelope, int id)
{
  assert(getChildCount() < _maxChildCount);
  assert(((isLeafNode() == false || getChildCount() == 0) && id < 0) ||
    (isLeafNode() == true && id >= 0));

  int childIndex = getChildCount();
  BoxInternalData bid(_dimensions, _getChildPtr(childIndex)->getBox(), envelope);
  _getChildPtr(childIndex)->id = id;
  _getHeader()->childCount++;
  _page->setDirty();
}

void RTreeNode::addNodeChild(RTreeNode* node)
{
  addChild(node->calculateEnvelope(), -node->getId());
  node->setParentId(getId());
}

void RTreeNode::addUserChild(const Box& envelope, int id)
{
  addChild(envelope, id);
}

Box RTreeNode::calculateEnvelope() const
{
  Box result(_dimensions);
  for (int i = 0; i < getChildCount(); i++)
  {
    result.expand(getChildEnvelope(i));
  }
  return result;
}

void RTreeNode::clear()
{
  _getHeader()->childCount = 0;
  setParentId(-1);
  _page->setDirty();
}

int RTreeNode::convertChildIdToIndex(int id) const
{
  assert(isLeafNode() == false);
  int result = -1;
  for (int i = 0; i < getChildCount(); i++)
  {
    if (getChildNodeId(i) == id)
    {
      result = i;
      break;
    }
  }
  assert(result >= 0);
  return result;
}

int RTreeNode::getChildCount() const
{
  return _getHeader()->childCount;
}

BoxInternalData RTreeNode::getChildEnvelope(int childIndex) const
{
  assert(childIndex < getChildCount());
  BoxInternalData bid(_dimensions, _getChildPtr(childIndex)->getBox());
  return bid;
}

int RTreeNode::getChildId(int childIndex)
{
  int id = _getChildPtr(childIndex)->id;
  return id;
}

int RTreeNode::getChildNodeId(int childIndex) const
{
  assert(childIndex < getChildCount());
  int id = -_getChildPtr(childIndex)->id;
  assert(id > 0);
  return id;
}

int RTreeNode::getChildUserId(int childIndex) const
{
  assert(childIndex < getChildCount());
  int id = _getChildPtr(childIndex)->id;
  assert(id >= 0);
  return id;
}

RTreeNode::ChildData* RTreeNode::_getChildPtr(int index) 
{
  return reinterpret_cast<ChildData*>(_page->getData() + _getChildSize() * index + _getHeaderSize());
}

const RTreeNode::ChildData* RTreeNode::_getChildPtr(int index) const
{
  return reinterpret_cast<ChildData*>(_page->getData() + _getChildSize() * index + _getHeaderSize());
}

int RTreeNode::_getChildSize() const
{
  return BoxInternalData::size(_dimensions) + sizeof(int);
}

RTreeNode::Header* RTreeNode::_getHeader()
{
  return reinterpret_cast<Header*>(_page->getData());
}

const RTreeNode::Header* RTreeNode::_getHeader() const
{
  return reinterpret_cast<Header*>(_page->getData());
}

int RTreeNode::getParentId() const
{
  return _getHeader()->parentId;
}

bool RTreeNode::isLeafNode() const
{
  bool result = false;
  if (getChildCount() == 0 || _getChildPtr(0)->id >= 0)
  {
    result = true;
  }
  return result;
}

void RTreeNode::removeChild(int index)
{
  assert(index >= 0 && index < getMaxChildCount());
  if (index < getMaxChildCount() - 1)
  {
    memmove(_getChildPtr(index), _getChildPtr(index + 1),
      _getChildSize() * (getChildCount() - index - 1));
  }
  _getHeader()->childCount = getChildCount() - 1;
  _page->setDirty();
}

void RTreeNode::removeChildren(std::vector<int>& children)
{
  /// @optimize This could be made more efficient w/ a little cleverness.
  for (unsigned int i = 0; i < children.size(); i++)
  {
    removeChild(children[i]);
    // we just shifted everything to the left, update the indexes to reflect that.
    for (unsigned int j = i + 1; j < children.size(); j++)
    {
      if (children[j] > children[i])
      {
        children[j]--;
      }
    }
  }
}

void RTreeNode::setParentId(int id)
{
  _getHeader()->parentId = id;
  _page->setDirty();
}

void RTreeNode::updateChild(int childIndex, int id, const Box& b)
{
  assert(childIndex < getChildCount());
  updateChildEnvelope(childIndex, b);
  _getChildPtr(childIndex)->id = id;
}

void RTreeNode::updateChildEnvelope(int index, const Box& envelope)
{
  assert(index < getChildCount());
  BoxInternalData bid(_dimensions, _getChildPtr(index)->getBox(), envelope);
  _page->setDirty();
}
