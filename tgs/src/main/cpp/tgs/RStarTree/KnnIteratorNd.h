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


#ifndef __TGS__KNN_ITERATOR_ND_H__
#define __TGS__KNN_ITERATOR_ND_H__

// Standard
#include <cassert>
#include <climits>
#include <list>
#include <queue>
#include <set>
#include <math.h>

// tgs
#include <tgs/HashMap.h>
#include <tgs/TgsExport.h>
#include <tgs/RStarTree/Iterator.h>
#include <tgs/RStarTree/RStarTree.h>

class OGRRawPoint;

namespace Tgs
{
  class TGS_EXPORT KnnIteratorNd : public Iterator
  {
  public:

    /**
    * The distance and the feature id. This is used in the priority queue while searching the tree.
    */
    class LeafDistance
    {
    public:
      LeafDistance() : distance(0.0), fid(0) { }

      LeafDistance(double dist, int fid) : distance(dist), fid(fid) { }

      double distance;
      int fid;
    };

    struct LessLeafDistance
      : public std::binary_function<KnnIteratorNd::LeafDistance, KnnIteratorNd::LeafDistance, bool>
    {
    public:
      bool operator()(const KnnIteratorNd::LeafDistance& _Left, 
        const KnnIteratorNd::LeafDistance& _Right) const
      {	
        if (_Left.distance > _Right.distance)
          return true;
        else if (_Left.distance == _Right.distance)
          return _Left.fid > _Right.fid;
        else
          return false;
      }
    };

    /**
    * The distance and the node. This is used in the priority queue while searching the tree.
    */
    class NodeDistance
    {
    public:
      NodeDistance() : minPossibleDistance(0.0), id(0) { }

      NodeDistance(double dist, int nodeId)
      {
        minPossibleDistance = dist;
        id = nodeId;
      }

      double minPossibleDistance;
      int id;
    };

    struct LessNodeDistance
      : public std::binary_function<KnnIteratorNd::NodeDistance*, KnnIteratorNd::NodeDistance*, bool>
    {
    public:
      bool operator()(/*const*/ KnnIteratorNd::NodeDistance*& _Left, 
        /*const*/ KnnIteratorNd::NodeDistance*& _Right) const
      {	
        return (_Left->minPossibleDistance > _Right->minPossibleDistance);
      }
    };

    KnnIteratorNd(const RStarTree* tree, const std::vector<double>& point, 
      const Box& searchRegion = Box());
    ~KnnIteratorNd();

    const Box& getBox() const override;
    int getId() const override { return _knnId; }
    double getDistance() const { return _knnDistance; }
    bool hasNext() override;
    bool next() override;

    void reset(const std::vector<double>& point);

    /**
     * Returns the euclidean distance squared
     */
    static double euclideanDistance(const std::vector<double>& p1, const std::vector<double>& p2);

  private:

    double _calculateDistance(const std::vector<double>& p, const BoxInternalData& box) const;
    void _calculateNextNn();

    const RStarTree* _searchTree;

    enum
    {
      RESERVED_ID = -1,
      MAX_NODE_DISTANCE_POOL_SIZE = 1000
    };

    /// More efficient than a bunch of news.
    std::vector<NodeDistance*> _nodeDistancePool;
    // allocate in chunks rather than bits and bytes
    static const int ALLOCATION_SIZE = 64;
    std::vector<NodeDistance*> _nodeDistanceAllocationPool;

    std::priority_queue<NodeDistance*, std::vector<NodeDistance*>, LessNodeDistance> _knnSearchQueue;
    std::priority_queue<LeafDistance, std::vector<LeafDistance>, LessLeafDistance> _knnLeafHeap;
    HashSet<int> _knnReturnedFids;

    std::vector<double> _knnPoint;
    mutable std::vector<double> _tmp;
    Box _knnBounds;
    double _knnDistance;
    int _knnId;
    Box _itemBounds;

    NodeDistance* _createNodeDistance(double minPossibleDistance, int nodeId);

    void _releaseNodeDistance(NodeDistance* nd);
  };

  inline double KnnIteratorNd::euclideanDistance(const std::vector<double>& p1, 
    const std::vector<double>& p2)
  {
    assert(p1.size() == p2.size());
    double sum = 0.0;
    for (unsigned int i = 0; i < p1.size(); i++)
    {
      double diff = p1[i] - p2[i];
      sum += diff * diff;
    }
    return sum;
  }
}

#endif
