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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef FQTREE_H
#define FQTREE_H

// hoot
#include <hoot/core/util/Log.h>

// Standard
#include <assert.h>
#include <vector>

// TGS
#include <tgs/HashMap.h>
#include <tgs/Statistics/Random.h>

#include "Node.h"
#include "SubTreeCallback.h"

namespace hoot
{
using namespace std;

namespace hybrid
{

#define FQ_NODE_SIZE 2

template <class KeyType, class DataType>
class FqNode : public Node<KeyType, DataType>
{
public:

  FqNode()
  {
    for (size_t i = 0; i < FQ_NODE_SIZE; ++i)
    {
      _children[i] = 0;
      _childDistance[i] = 0;
    }
    _next = 0;
  }

  virtual ~FqNode()
  {
    for (size_t i = 0; i < FQ_NODE_SIZE; ++i)
    {
      delete _children[i];
    }
    delete _next;
  }

  void addChild(Node<KeyType, DataType>* n, short d)
  {
    if (n != 0)
    {
      if (_next != 0)
      {
        _next->addChild(n, d);
      }
      else
      {
        bool done = false;
        for (size_t i = 0; i < (FQ_NODE_SIZE) && !done; ++i)
        {
          if (_children[i] == 0)
          {
            _children[i] = n;
            _childDistance[i] = d;
            done = true;
          }
        }

        if (!done)
        {
          _next = new FqNode();
          _next->addChild(n, d);
        }
      }
    }
  }

  virtual Node<KeyType, DataType>* getChild(size_t i) const
  {
    if (i >= FQ_NODE_SIZE)
    {
      return _next->getChild(i - FQ_NODE_SIZE);
    }
    else
    {
      return _children[i];
    }
  }

  virtual short getChildDistance(size_t i) const
  {
    if (i >= FQ_NODE_SIZE)
    {
      return _next->getChildDistance(i - FQ_NODE_SIZE);
    }
    else
    {
      return _childDistance[i];
    }
  }

  virtual size_t getChildCount() const
  {
    size_t result = 0;
    if (_next == 0)
    {
      for (size_t i = 0; i < FQ_NODE_SIZE; ++i)
      {
        if (_children[i] != 0)
        {
          result++;
        }
      }
    }
    else
    {
      result = FQ_NODE_SIZE + _next->getChildCount();
    }
    return result;
  }

  virtual QString toString(QString indent = QString()) const
  {
    std::stringstream ss;
    ss << indent << "FqNode: " << endl;
    size_t c = getChildCount();
    for (size_t i = 0; i < c; ++i)
    {
      ss << indent << "  d: " << getChildDistance(i) << endl;
      ss << getChild(i)->toString(indent + "  ");
    }
    return QString::fromUtf8(ss.str().data());
  }

private:
  // This strange memory structure is here to prevent unnecessary news and to keep the majority
  // of the objects allocated the same size. Switching from vectors to this structure reduced
  // memory usage of the index by about 60% without a substantial difference in speed. Switching
  // to boost pools at some point in the future may further improve memory usage.
  Node<KeyType, DataType>* _children[FQ_NODE_SIZE];
  short _childDistance[FQ_NODE_SIZE];
  FqNode* _next;
};

/**
 * The layer understands how to build and query a RTree. By breaking it out why can create hybrid
 * trees that use RTree constructs for some layers, and other constructs for other layers.
 */
template <class KeyType, class DataType, class DistanceFunction>
class FqTreeLayer : public SubTreeCallback<KeyType, DataType>
{
public:

  FqTreeLayer(int bucketSize = 1, int depth = -1)
  {
    _bucketSize = bucketSize;
    _callback = 0;
    _depth = depth;
    _distanceCount = 0;
  }

  virtual Leaf<KeyType, DataType>* buildLeaf(size_t start, size_t end)
  {
    const vector<KeyType>& keys = *_keys;
    const vector<DataType>& values = *_values;
    vector<size_t>& order = *_order;

    Leaf<KeyType, DataType>* result = new Leaf<KeyType, DataType>();

    for (size_t i = start; i < end; ++i)
    {
      result->addData(keys[order[i]], values[order[i]]);
    }

    return result;
  }

  /**
   * Recursively build a tree based on the keys/values in [start, end). All subtrees are built
   * by calling _callback->buildTree
   */
  virtual Node<KeyType, DataType>* buildTree(size_t start, size_t end, int depth)
  {
    if (end - start == 0)
    {
      return 0;
    }

    while ((int)_q.size() <= depth)
    {
      _q.push_back(_pickKey());
      LOG_TRACE(_q.size() - 1 << ": " << _q[_q.size() - 1]);
    }

    const vector<KeyType>& keys = *_keys;
    vector<size_t>& order = *_order;

    // if we've reached the bottom then create a leaf node.
    if (depth == _depth || (_depth == -1 && (int)(end - start) <= _bucketSize))
    {
      return buildLeaf(start, end);
    }
    // if we aren't at the bottom yet
    else
    {
      for (size_t i = start; i < end; ++i)
      {
        _distanceValues[order[i]] = _df(_q[depth].getMetricElement(),
                                        keys[order[i]].getMetricElement());
      }

      FqNode<KeyType, DataType>* result = new FqNode<KeyType, DataType>();

      Order o;
      o.fq = this;
      sort(order.begin() + start, order.begin() + end, o);

      for (size_t i = start; i < end;)
      {
        size_t iStart = i;
        short d = _distanceValues[order[i]];
        for (++i; i < end && d == _distanceValues[order[i]]; ++i);
        size_t iEnd = i;

        Node<KeyType, DataType>* child = _callback->buildTree(iStart, iEnd, depth + 1);
        result->addChild(child, d);
      }

      return result;
    }
  }

  /**
   * Should be called after the last buildTree call to clean up temporary values.
   */
  void endBuild()
  {
    _distanceValues.clear();
    _keys = 0;
    _values = 0;
    _order = 0;
  }

  /**
   * Recursively search the tree for any keys that are within radius distance of c.
   */
  void find(const Node<KeyType, DataType>* n, int depth, set<DataType>& result) const
  {
    if (_d[depth] == -1)
    {
      _d[depth] = _df(_query.getMetricElement(), _q[depth].getMetricElement());
    }
    int di = _d[depth];

    const FqNode<KeyType, DataType>* fqn = dynamic_cast<const FqNode<KeyType, DataType>*>(n);
    if (fqn != 0)
    {
      for (size_t i = 0; i < fqn->getChildCount(); ++i)
      {
        int d = fqn->getChildDistance(i);
        // if it is within the search range
        if (di - _queryD <= d && d <= di + _queryD)
        {
          _callback->find(fqn->getChild(i), depth + 1, result);
        }
      }
    }
    else
    {
      const Leaf<KeyType, DataType>* leaf = dynamic_cast<const Leaf<KeyType, DataType>*>(n);
      assert(leaf != 0);

      _callback->findLeaf(leaf, result);
    }
  }

  virtual void findLeaf(const Leaf<KeyType, DataType>* leaf, set<DataType>& result) const
  {
    for (size_t i = 0; i < leaf->getSize(); ++i)
    {
      const pair<KeyType, DataType>& p = leaf->get(i);
      int d = _df(_query.getMetricElement(), p.first.getMetricElement());
      if (d <= _queryD)
      {
        result.insert(p.second);
      }
    }
  }

  int getDepth() const { return _depth; }

  int getQueryD() const { return _queryD; }

  int resetDistanceCount()
  {
    int result = _distanceCount;
    _distanceCount = 0;
    return result;
  }

  void setBucketSize(int bucketSize) { _bucketSize = bucketSize; }

  /**
   * This must be called before buildTree is called the first time. In the simplest case _callback
   * is set with this object's pointer.
   */
  void setCallback(SubTreeCallback<KeyType, DataType>* callback)
  {
    _callback = callback;
  }

  void setDepth(int depth) { _depth = depth; }

  /**
   * Sets the keys and values that will be used to build the layers. The keys and values should
   * not change through the build process.
   * @param order This is the ordering of the entries as they're inserted into the tree. The order
   *  is shared among any layers that may be building the tree. The start/end values in buildTree
   *  reference start/end values in this order. The order contains values from 0 to n-1, initially
   *  sorted.
   */
  void setKeysValues(const vector<KeyType>& keys, const vector<DataType>& values,
                     vector<size_t>& order)
  {
    _keys = &keys;
    _values = &values;
    _order = &order;
    _q.clear();
    _distanceValues.resize(_keys->size());
  }

  void setQuery(const KeyType& query, int queryD) const
  {
    _query = query;
    _queryD = queryD;
    _d.resize(_q.size());
    for (size_t i = 0; i < _d.size(); ++i)
    {
      _d[i] = -1;
    }
  }

private:

  class Order
  {
  public:
    bool operator() (size_t p1, size_t p2)
    {
      return fq->_distanceValues[p1] < fq->_distanceValues[p2];
    }

    FqTreeLayer<KeyType, DataType, DistanceFunction>* fq;
  };

  mutable KeyType _query;
  // "D" in [1]
  mutable int _queryD;
  mutable vector<short> _d;

  DistanceFunction _df;
  int _bucketSize;
  SubTreeCallback<KeyType, DataType>* _callback;
  int _depth;
  mutable int _distanceCount;
  /// q as defined in [1].
  vector<KeyType> _q;
  const vector<KeyType>* _keys;
  const vector<DataType>* _values;
  vector<int> _distanceValues;
  vector<size_t>* _order;

  const KeyType& _pickKey() const
  {
    // for now just use a random key as described in the paper.
    const KeyType& result = (*_keys)[Tgs::Random::instance()->generateInt(_keys->size())];

    return result;
  }
};

/**
 * Implements a FQ-Tree with optional fixed height. [1]
 *
 * The KeyType class must support the following method:
 *
 *   <element type> getMetricElement() const;
 *
 * Where DistanceFunction supports:
 *
 *   int operator() (<element type>, <element type>);
 *
 * The <element type> does not need to by known by the tree.
 *
 * 1. Proximity Matching Using Fixed-Queries Trees
 * http://www.researchgate.net/publication/2290984_Proximity_Matching_Using_Fixed-Queries_Trees/file/9fcfd50dd177b3354b.pdf
 */
template <class KeyType, class DataType, class DistanceFunction>
class FqTree
{
public:
  FqTree(int depth = -1)
  {
    _depth = depth;
    _root = 0;
    _bucketSize = 2;
  }

  virtual ~FqTree() { delete _root; }

  void buildIndex(const vector<KeyType>& keys, const vector<DataType>& values)
  {
    assert(keys.size() == values.size());
    delete _root;
    _order.resize(keys.size());
    for (size_t i = 0; i < _order.size(); ++i)
    {
      _order[i] = i;
    }

    _layer.setCallback(&_layer);
    _layer.setKeysValues(keys, values, _order);
    _layer.setDepth(_depth);
    _layer.setBucketSize(_bucketSize);
    _root = _layer.buildTree(0, _order.size(), 0);
    _layer.endBuild();
    _order.clear();
  }

  set<DataType> find(const KeyType& key, int maxDistance) const
  {
    set<DataType> result;
    _layer.setQuery(key, maxDistance);
    _layer.find(_root, 0, result);

    return result;
  }

  void setBucketSize(int s) { _bucketSize = s; }

  void setDepth(int depth) { _depth = depth; }

  QString toString() const { return _root->toString(); }

protected:

  Node<KeyType, DataType>* _root;

  int _bucketSize;
  // As the tree is built we re-order
  vector<size_t> _order;
  int _depth;
  FqTreeLayer<KeyType, DataType, DistanceFunction> _layer;
};

}

}

#endif // FQTREE_H
