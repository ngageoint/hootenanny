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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__NORMAL_ESTIMATOR_H__
#define __TGS__NORMAL_ESTIMATOR_H__

// Standard Includes
#include <queue>

#include <tgs/HashMap.h>
#include <tgs/TgsExport.h>
#include <tgs/SpinImage/Points.h>

namespace Tgs
{

class HilbertRTree;
class KnnIteratorNd;
class NormalEstimatorTest;
class PointCloud;

/**
 * Uses the algorithm described in [2], although [1] is the prior work and is much easier to
 * read.
 *
 * [1] "Surface Reconstruction from Unorganized Points"
 * (http://citeseer.ist.psu.edu/hoppe94surface.html)
 * [2] Mitra, Nguyen and Guibas "Estimating Surface Normals in Noisy Point Cloud Data"
 * http://graphics.stanford.edu/%7Eniloy/research/normal_est/paper_docs/normal_estimation_ijcga_04.pdf
 */
class TGS_EXPORT NormalEstimator
{
public:

  NormalEstimator();

  virtual ~NormalEstimator() {}

  void calculateNormals(PointCloud& pc);

  /**
   * Sets the maximum k value to evaluate (kThreshold in [2]). Defaults to 15.
   */
  int getKThreshold() const { return _k; }

  void setKThreshold(int k) { _k = k; }

protected:

  class VertexOrder
  {
  public:
    int id;
    double cost;
    int parent;

    VertexOrder(int id, double cost, int parent)
    {
      this->id = id;
      this->cost = cost;
      this->parent = parent;
    }
  };

  struct LessVertexOrder
    : public std::binary_function<int, int, bool>
  {
  public:
    bool operator()(const VertexOrder& _Left, const VertexOrder& _Right) const
    {
      return (_Left.cost > _Right.cost);
    }
  };

  // white box testing
  friend class NormalEstimatorTest;

  std::shared_ptr<Tgs::HilbertRTree> _rtree;
  PointCloud* _pc;
  std::vector<int> _nearestNeighbors;
  std::vector<double> _nnDistances;
  std::shared_ptr<KnnIteratorNd> _it;
  std::vector<double> _nnTmp;
  int _k;

  /// this is set to true for a point if its children have been traversed.
  std::vector<bool> _processed;

  using RiemannianGraph = std::vector<HashSet<int>>;

  RiemannianGraph _edges;

  using MstQueue = std::priority_queue<VertexOrder, std::vector<VertexOrder>, LessVertexOrder>;

  MstQueue _mstQueue;

  /**
   * Calculates the distance to each neighbor and averages the result.
   */
  double _calculateAverageDistanceToNeighbors(int k);

  /**
   * Calculate the cost in the Riemannian of traveling from one point to another. This is
   * the cost as defined by [1] section 3.3
   */
  double _calculateCost(int idx1, int idx2) const;

  void _calculateNearestNeighbors(int pointIndex);

  /**
   * Calculates the third principal component as the normal and returns as 'normal'
   */
  void _calculatePrincipalComponent(const std::vector<int>& neighbors, int k, Point3d& normal);

  /**
   * Calculates the tangent plane based solely on the k nearest neighbors. In the end the 'real'
   * tangent plane may be the reflection of the returned normal. See [1] section 3.2
   */
  Point3d _calculateTangentPlane(int pointIndex);

  void _populateRiemannianGraph(int pointIndex);

  void _processMst();

  void _loadPointCloud(const PointCloud& pc);
};

}

#endif
