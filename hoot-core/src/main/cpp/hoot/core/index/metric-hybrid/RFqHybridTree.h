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
 * @copyright Copyright (C) 2015, 2017, 2018 Maxar (http://www.maxar.com/)
 */
#ifndef RFQHYBRIDTREE_H
#define RFQHYBRIDTREE_H

#include <hoot/core/index/metric-hybrid/FqTree.h>
#include <hoot/core/index/metric-hybrid/RTree.h>

// boost
#include <boost/pool/object_pool.hpp>

namespace hoot
{

namespace hybrid
{

/**
 * Implements a hybrid tree using both Fq-Tree and R-Tree where the layers type change for each
 * layer in the tree.
 *
 * The KeyType class must support the following method:
 *
 *   const Envelope& getEnvelope() const;
 *
 *   <element type> getMetricElement() const;
 *
 * Where DistanceFunction supports:
 *
 *   int operator() (<element type>, <element type>);
 *
 * The <element type> does not need to by known by the tree.
 *
 * @optimize This data structure has not been through any real optimization steps. I tuned the
 * structure a bit, but it really needs to go through the profiler and hand tune the data
 * structures for size. Each node is quite wasteful. I did some work with the FqNodes to bring down
 * the memory usage by removing vectors, but using boost pools may also help out.
 *
 */
template <class KeyType, class DataType, class DistanceFunction>
class RFqHybridTree : public SubTreeCallback<KeyType, DataType>
{
public:
  /**
   * fqDepth should always be greater than or equal to rDepth.
   * @param bucketSize - The max number of data elements stored at a leaf node.
   * @param rChildCount - The max number of child nodes a RNode has.
   * @param rDepth - The depth that the RNodes will be populated.
   * @param fqDepth - The minimum depth that the FqNodes will be populated. fqDepth must always be
   *  >= rDepth.
   *
   * The default parameters were determined while testing against a node only data set with 95k
   * nodes and about 800k names.
   */
  RFqHybridTree(int bucketSize = 1, int rChildCount = 10, int rDepth = 5, int fqDepth = 27) :
    // set the depth to the max of the two so we don't create leaves prematurely.
    _rLayer(rChildCount, bucketSize, std::max(rDepth, fqDepth)),
    // set the depth to the max of the two so we don't create leaves prematurely.
    _fqLayer(bucketSize, std::max(rDepth, fqDepth))
  {
    _rDepth = rDepth;
    _fqDepth = fqDepth;
    assert(fqDepth >= rDepth);
    _rLayer.setCallback(this);
    _fqLayer.setCallback(this);
    _root = 0;
  }

  virtual ~RFqHybridTree() { delete _root; }

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

    _rLayer.setKeysValues(keys, values, order);
    _fqLayer.setKeysValues(keys, values, order);
    _root = _rLayer.buildTree(0, keys.size(), 0);
    _rLayer.endBuild();
    _fqLayer.endBuild();
  }

  /**
   * This method is used internally and should not be called directly.
   */
  virtual Node<KeyType, DataType>* buildTree(size_t start, size_t end, int depth)
  {
    if (_isRLayer(depth))
    {
      return _rLayer.buildTree(start, end, depth);
    }
    else
    {
      return _fqLayer.buildTree(start, end, depth);
    }
  }

  /**
   * This method is used internally and should not be called directly.
   */
  virtual void find(const Node<KeyType, DataType>* n, int depth, std::set<DataType>& result) const
  {
    if (_isRLayer(depth))
    {
      _rLayer.find(n, depth, result);
    }
    else
    {
      _fqLayer.find(n, depth, result);
    }
  }

  /**
   * Returns all bounding boxes that are within radius of c.
   */
  std::set<DataType> find(const geos::geom::Coordinate& c, double radius, const KeyType& k, int D) const
  {
    std::set<DataType> result;
    _rLayer.setQuery(c, radius);
    _queryTerm = k;
    _fqLayer.setQuery(k, D);
    _rLayer.find(_root, 0, result);
    return result;
  }

  virtual void findLeaf(const Leaf<KeyType, DataType>* leaf, std::set<DataType>& result) const
  {
    for (size_t i = 0; i < leaf->getSize(); ++i)
    {
      const std::pair<KeyType, DataType>& p = leaf->get(i);
      if (_rLayer.distance(_rLayer.getQueryCoordinate(), p.first.getEnvelope()) <=
          _rLayer.getQueryRadius())
      {
        int d = _df(_queryTerm.getMetricElement(), p.first.getMetricElement());
        if (d <= _fqLayer.getQueryD())
        {
          result.insert(p.second);
        }
      }
    }
  }

  int resetDistanceCount()
  {
    return _rLayer.resetDistanceCount();
  }

  QString toString() const
  {
    QString result;
    if (_root)
    {
      result = _root->toString();
    }
    return result;
  }

protected:

  Node<KeyType, DataType>* _root;

  mutable KeyType _queryTerm;
  DistanceFunction _df;
  RTreeLayer<KeyType, DataType> _rLayer;
  FqTreeLayer<KeyType, DataType, DistanceFunction> _fqLayer;
  int _rDepth;
  int _fqDepth;

  /**
   * Returns true if the layer at this depth is a RLayer.
   */
  bool _isRLayer(int depth) const
  {
    bool result = true;
    // this is the standard case, we haven't exceeded either depth, then alternate layering
    if (depth < _rDepth && depth < _fqDepth)
    {
      result = depth % 2 == 0;
    }
    else  // we've exceeded one depth or the other
    {
      // if the depths are equal then continue alternating.
      if (_fqDepth == _rDepth)
      {
        result = depth % 2 == 0;
      }
      // if one layer has a higher depth then that layer is used for all remaining layers.
      else if (_fqDepth > _rDepth)
      {
        result = false;
      }
      else
      {
        result = true;
      }
    }
    return result;
  }
};

}

}

#endif // RFQHYBRIDTREE_H
