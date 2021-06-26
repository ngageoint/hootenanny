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
 * @copyright Copyright (C) 2015, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __TGC_KNN_ITERATOR_H__
#define __TGC_KNN_ITERATOR_H__

// Standard Includes
#include <climits>
#include <cmath>
#include <list>
#include <queue>
#include <set>

#include <tgs/HashMap.h>
#include <tgs/TgsExport.h>
#include <tgs/RStarTree/Iterator.h>
#include <tgs/RStarTree/RStarTree.h>

namespace Tgs
{

class TGS_EXPORT KnnIterator : public Iterator
{
public:

  /**
  * The distance and the feature id. This is used in the priority queue while searching the tree.
  */
  class LeafDistance
  {
  public:

    LeafDistance() : distance(0.0), fid(0) { }

    LeafDistance(double dist, int fid)
    {
      this->distance = dist;
      this->fid = fid;
    }

    double distance;
    int fid;
  };

  struct LessLeafDistance
    : public std::binary_function<KnnIterator::LeafDistance, KnnIterator::LeafDistance, bool>
  {
  public:

    bool operator()(const KnnIterator::LeafDistance& _Left,
      const KnnIterator::LeafDistance& _Right) const
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

    NodeDistance() = default;

    NodeDistance(double dist, int nodeId)
    {
      minPossibleDistance = dist;
      id = nodeId;
    }

    double minPossibleDistance;
    int id;
  };

  struct LessNodeDistance
    : public std::binary_function<KnnIterator::NodeDistance*, KnnIterator::NodeDistance*, bool>
  {
  public:

    bool operator()(KnnIterator::NodeDistance*& _Left,
                    KnnIterator::NodeDistance*& _Right) const
    {
      return (_Left->minPossibleDistance > _Right->minPossibleDistance);
    }
  };

  KnnIterator(const RStarTree* tree, const double x, const double y,
    const Box& searchRegion = Box());
  virtual ~KnnIterator();

  const Box& getBox() const override;
  int getId() const { return _knnId; }
  virtual double getDistance() const { return _knnDistance; }
  bool hasNext() override;
  bool next() override;

  virtual void reset(const double x, const double y);

  static double euclideanDistance(double x1, double y1, double x2, double y2);
  static double distPtToLine(double x, double y, double s1, double t1, double s2, double t2);

protected:

  enum
  {
    RESERVED_ID = -1,
    MAX_NODE_DISTANCE_POOL_SIZE = 1000
  };

  /**
   * Returns the distance squared from the source to the specified id. The bounding box of the
   * id is given for convenience.
   */
  virtual double _calculateDistance(const BoxInternalData& box, int id) const;

  /**
   * Returns the distance squared from the source to the specified box.
   */
  virtual double _calculateDistance(const BoxInternalData& box) const;

  void _calculateNextNn();

private:

  const RStarTree* _searchTree;

  /// More efficient than a bunch of news.
  std::vector<NodeDistance*> _nodeDistancePool;
  // allocate in chunks rather than bits and bytes
  static const int ALLOCATION_SIZE = 64;
  std::vector<NodeDistance*> _nodeDistanceAllocationPool;

  std::priority_queue<NodeDistance*, std::vector<NodeDistance*>, LessNodeDistance> _knnSearchQueue;
  std::priority_queue<LeafDistance, std::vector<LeafDistance>, LessLeafDistance> _knnLeafHeap;
  HashSet<int> _knnReturnedFids;

  double _knnX;
  double _knnY;
  Box _knnBounds;
  double _knnDistance;
  int _knnId;
  Box _itemBounds;

  NodeDistance* _createNodeDistance(double minPossibleDistance, int nodeId);

  void _releaseNodeDistance(NodeDistance* nd);
};

inline double KnnIterator::euclideanDistance(double x1, double y1, double x2, double y2)
{
  return /*sqrt*/( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) );
}

inline double KnnIterator::distPtToLine(double x, double y, double s1, double t1,
  double s2, double t2)
{
  double deltaX = s2 - s1;
  double deltaY = t2 - t1;
  double deltaX2 = x - s1;
  double deltaY2 = y - t1;
  double denominator = deltaX * deltaX + deltaY * deltaY;
  double numerator = deltaX2 * deltaX + deltaY2 * deltaY;
  double u = -1;

  // degenerated line case, i.e. the line is a point
  if (fabs(denominator) < 1.0e-06)
    u = 0;
  else
    u = numerator / denominator;

  // collinear case and line is not degenerated
  if (u != 0.0 &&
     fabs(deltaX - deltaX2) < 1.0e-06 &&
     fabs(deltaY - deltaY2) < 1.0e-06)
  {
    u = deltaX2 / deltaX;
    // case when point lies on the line segment
    if (u >= 0 && u <= 1)
      return 0;
  }

  // perpendicular bisector is on the line segment
  if (u >= 0 && u <= 1)
  {
    // calc point on the line
    return euclideanDistance(x, y, s1 + deltaX*u, t1 + deltaY*u);
  }
  // perpendicular bisector is not on the line segment, pick the closest end point.
  else
  {
    // use endpoints
    double d1 = euclideanDistance(x, y, s1, t1);
    double d2 = euclideanDistance(x, y, s2, t2);
    if (d1 < d2)
      return d1;
    else
      return d2;
  }
}

}

#endif
