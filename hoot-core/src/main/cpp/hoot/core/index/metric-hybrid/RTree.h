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
 * @copyright Copyright (C) 2015, 2017, 2018, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef HOOT_HYBRID_RTREE_H
#define HOOT_HYBRID_RTREE_H

// hoot
#include <hoot/core/index/metric-hybrid/Node.h>
#include <hoot/core/index/metric-hybrid/SubTreeCallback.h>

// Standard
#include <assert.h>
#include <math.h>

// TGS
#include <tgs/HashMap.h>
#include <tgs/RStarTree/HilbertCurve.h>

namespace hoot
{

namespace hybrid
{

template <class KeyType, class DataType>
class RNode : public Node<KeyType, DataType>
{
public:

  RNode() = default;

  virtual ~RNode()
  {
    for (size_t i = 0; i < _children.size(); ++i)
    {
      delete _children[i];
    }
    _children.clear();
  }

  void addChild(Node<KeyType, DataType>* n, const geos::geom::Envelope& e)
  {
    if (n != nullptr)
    {
      _children.push_back(n);
      _childEnvelope.push_back(e);
    }
  }

  virtual const Node<KeyType, DataType>* getChild(size_t i) const { return _children[i]; }

  virtual size_t getChildCount() const { return _children.size(); }

  const geos::geom::Envelope& getChildEnvelope(size_t i) const { return _childEnvelope[i]; }

  virtual QString toString(QString indent = QString()) const
  {
    std::stringstream ss;
    ss << indent << "RNode: " << std::endl;
    for (size_t i = 0; i < _children.size(); ++i)
    {
      ss << indent << "  " << _childEnvelope[i].toString() << std::endl;
      ss << _children[i]->toString(indent + "  ");
    }
    return QString::fromUtf8(ss.str().data());
  }

private:
  std::vector<Node<KeyType, DataType>*> _children;
  std::vector<geos::geom::Envelope> _childEnvelope;
};

/**
 * The layer understands how to build and query a RTree. By breaking it out why can create hybrid
 * trees that use RTree constructs for some layers, and other constructs for other layers.
 */
template <class KeyType, class DataType>
class RTreeLayer : public SubTreeCallback<KeyType, DataType>
{
public:

  class Order
  {
  public:
    Order(std::vector<int>& hilbertValue) : _hilbertValue(hilbertValue) { }

    bool operator() (size_t o1, size_t o2)
    {
      return _hilbertValue[o1] < _hilbertValue[o2];
    }

    std::vector<int>& _hilbertValue;
  };

  RTreeLayer(int childCount = 2, int bucketSize = 1, int depth = -1)
  {
    _childCount = childCount;
    _bucketSize = bucketSize;
    _callback = nullptr;
    _depth = depth;
    _distanceCount = 0;
    _keys = nullptr;
    _values = nullptr;
    _order = nullptr;
  }
  virtual ~RTreeLayer() = default;

  virtual Leaf<KeyType, DataType>* buildLeaf(size_t start, size_t end)
  {
    const std::vector<KeyType>& keys = *_keys;
    const std::vector<DataType>& values = *_values;
    std::vector<size_t>& order = *_order;

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
      return nullptr;
    }

    const std::vector<KeyType>& keys = *_keys;
    std::vector<size_t>& order = *_order;

    // if we've reached the bottom then create a leaf node.
    if (depth >= _depth && (int)(end - start) <= _bucketSize)
    {
      return buildLeaf(start, end);
    }
    // if we aren't at the bottom yet
    else
    {
      RNode<KeyType, DataType>* result = new RNode<KeyType, DataType>();

      // order the entries by Hilbert value.
      Order o(_hilbertValues);
      sort(order.begin() + start, order.begin() + end, o);

      int childCount = std::min(end - start, (size_t)_childCount);
      int elementCount = end - start;
      // evenly distribute the elements among our children and build the sub-trees.
      for (int i = 0; i < childCount; ++i)
      {
        size_t iStart = start + elementCount * i / childCount;
        size_t iEnd = start + elementCount * (i + 1) / childCount;

        geos::geom::Envelope e;
        for (size_t j = iStart; j < iEnd; ++j)
        {
          e.expandToInclude(&(keys[order[j]].getEnvelope()));
        }

        Node<KeyType, DataType>* child = _callback->buildTree(iStart, iEnd, depth + 1);
        result->addChild(child, e);
      }

      return result;
    }
  }

  double distance(const geos::geom::Coordinate& c, const geos::geom::Envelope& e) const
  {
    geos::geom::Envelope e2(c.x, c.x, c.y, c.y);
    _distanceCount++;
    return e.distance(e2);
  }

  /**
   * Should be called after the last buildTree call to clean up temporary values.
   */
  void endBuild()
  {
    _hilbertValues.clear();
    _keys = nullptr;
    _values = nullptr;
    _order = nullptr;
  }

  /**
   * Recursively search the tree for any keys that are within radius distance of c.
   */
  void find(const Node<KeyType, DataType>* n, int depth, std::set<DataType>& result) const
  {
    const Leaf<KeyType, DataType>* l = dynamic_cast<const Leaf<KeyType, DataType>*>(n);

    if (l != nullptr)
    {
      _callback->findLeaf(l, result);
    }
    else
    {
      const RNode<KeyType, DataType>* rn = dynamic_cast<const RNode<KeyType, DataType>*>(n);
      assert(rn != nullptr);

      for (size_t i = 0; i < rn->getChildCount(); ++i)
      {
        double d = distance(_queryC, rn->getChildEnvelope(i));
        if (d <= _queryRadius)
        {
          const Node<KeyType, DataType>* child = rn->getChild(i);
          _callback->find(child, depth + 1, result);
        }
      }
    }
  }

  virtual void findLeaf(const Leaf<KeyType, DataType>* leaf, std::set<DataType>& result) const
  {
    for (size_t i = 0; i < leaf->getSize(); ++i)
    {
      const std::pair<KeyType, DataType>& p = leaf->get(i);
      if (distance(_queryC, p.first.getEnvelope()) <= _queryRadius)
      {
        result.insert(p.second);
      }
    }
  }

  int getDepth() const { return _depth; }

  const geos::geom::Coordinate& getQueryCoordinate() const { return _queryC; }

  double getQueryRadius() const { return _queryRadius; }

  int resetDistanceCount()
  {
    int result = _distanceCount;
    _distanceCount = 0;
    return result;
  }

  /**
   * This must be called before buildTree is called the first time. In the simplest case _callback
   * is set with this object's pointer.
   */
  void setCallback(SubTreeCallback<KeyType, DataType>* callback)
  {
    _callback = callback;
  }

  /**
   * Sets the keys and values that will be used to build the layers. The keys and values should
   * not change through the build process.
   * @param order This is the ordering of the entries as they're inserted into the tree. The order
   *  is shared among any layers that may be building the tree. The start/end values in buildTree
   *  reference start/end values in this order. The order contains values from 0 to n-1, initially
   *  sorted.
   */
  void setKeysValues(const std::vector<KeyType>& keys, const std::vector<DataType>& values,
                     std::vector<size_t>& order)
  {
    _keys = &keys;
    _values = &values;
    _order = &order;
    _hilbertValues.resize(_keys->size());

    geos::geom::Envelope bounds;
    if (keys.size() > 0)
    {
      bounds = keys[0].getEnvelope();
    }
    for (size_t i = 1; i < _keys->size(); ++i)
    {
      bounds.expandToInclude(&keys[i].getEnvelope());
    }

    // calculate the hibert values of each key.
    int hcoord[2];
    geos::geom::Coordinate center;
    Tgs::HilbertCurve curve(2, 8);
    for (size_t i = 0; i < _keys->size(); ++i)
    {
      const geos::geom::Envelope& e = keys[i].getEnvelope();
      e.centre(center);
      hcoord[0] = round((center.x - bounds.getMinX()) / (bounds.getMaxX() - bounds.getMinX())
                        * 0xFF);
      hcoord[1] = round((center.y - bounds.getMinY()) / (bounds.getMaxY() - bounds.getMinY())
                        * 0xFF);

      _hilbertValues[i] = curve.encode(hcoord);
    }
  }

  void setQuery(const geos::geom::Coordinate& c, double radius) const
  {
    _queryC = c;
    _queryRadius = radius;
  }

private:

  mutable geos::geom::Coordinate _queryC;
  mutable double _queryRadius;

  int _childCount;
  int _bucketSize;
  SubTreeCallback<KeyType, DataType>* _callback;
  int _depth;
  mutable int _distanceCount;
  const std::vector<KeyType>* _keys;
  const std::vector<DataType>* _values;
  std::vector<int> _hilbertValues;
  std::vector<size_t>* _order;
};


/**
 * Implements a R-Tree with significant deviations from the original paper. This is built from
 * the top down, one layer at a time and only supports a bulk insert (no updates or deletes). This
 * is only really useful for hybrid indexes. If you want a much better R-Tree check out the
 * implementation in TGS.
 *
 * The KeyType class must support the following method:
 *
 *   const Envelope& getEnvelope() const;
 *
 * 1. http://en.wikipedia.org/wiki/R-tree
 */
template <class KeyType, class DataType>
class RTree
{
public:
  /**
   * @param childCount - The number of child nodes a non-leaf node has.
   * @param bucketSize - The number of data elements stored at a leaf node.
   * @param depth - The minimum depth of the tree. The tree is guaranteed to be at least this deep
   *        This isn't typically useful, but can be useful with hybrid trees.
   */
  RTree(int childCount = 2, int bucketSize = 1, int depth = -1) :
    _layer(childCount, bucketSize, depth)
  {
    _root = nullptr;
  }

  virtual ~RTree() { delete _root; }

  /**
   * Clear any old data and build a new index with the specified keys and values.
   */
  void buildIndex(const std::vector<KeyType>& keys, const std::vector<DataType>& values)
  {
    assert(keys.size() == values.size());
    delete _root;
    // As the tree is built we re-order
    std::vector<size_t> order;
    order.resize(keys.size());
    for (size_t i = 0; i < order.size(); ++i)
    {
      order[i] = i;
    }

    _layer.setCallback(&_layer);
    _layer.setKeysValues(keys, values, order);
    _root = _layer.buildTree(0, keys.size(), 0);
    _layer.endBuild();
  }

  /**
   * Returns all bounding boxes that are within radius of c.
   */
  std::set<DataType> find(const geos::geom::Coordinate& c, double radius) const
  {
    std::set<DataType> result;
    _layer.setQuery(c, radius);
    _layer.find(_root, 0, result);
    return result;
  }

  int resetDistanceCount()
  {
    return _layer.resetDistanceCount();
  }

  QString toString() const
  {
    return _root->toString();
  }

private:

  Node<KeyType, DataType>* _root;
  RTreeLayer<KeyType, DataType> _layer;
};

}

}

#endif // HOOT_HYBRID_RTREE_H
