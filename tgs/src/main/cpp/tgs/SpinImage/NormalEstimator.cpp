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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */

#include "NormalEstimator.h"

// Standard Includes
#include <algorithm>
#include <iostream>

// Newmat Includes
#include <newmatap.h>
#include <newmatio.h>

#include "../RStarTree/KnnIteratorNd.h"
#include "../RStarTree/MemoryPageStore.h"
#include "../RStarTree/HilbertRTree.h"

using namespace std;

namespace Tgs
{
  NormalEstimator::NormalEstimator()
  {
    _k = 15;
  }

  double NormalEstimator::_calculateAverageDistanceToNeighbors(int k)
  {
    double sum = 0.0;
    for (int i = 0; i < k; i++)
    {
      sum += _nnDistances[i];
    }
    return sum / k;
  }


  double NormalEstimator::_calculateCost(int idx1, int idx2) const
  {
    PointCloud& pc = *_pc;
    const Point3d& p1 = pc.getNormals()[idx1];
    const Point3d& p2 = pc.getNormals()[idx2];
    return 1 - fabs(dotProduct(p1, p2));
  }

  void NormalEstimator::calculateNormals(PointCloud& pc)
  {
    // init
    _edges.clear();
    _pc = &pc;

    // load point cloud into RTree
    _loadPointCloud(pc);

    // The point w/ the max z value becomes the initial plane when traversing the minimum
    // spanning tree (MST)
    double maxZ = -1e300;
    int maxZId = -1;
    // calculate the preliminary normals. We may have to reflect them.
    pc.getNormals().resize(0);
    pc.getNormals().reserve(pc.getPoints().size());
    const std::vector<CloudPoint>& points = pc.getPoints();
    _edges.resize(pc.getPoints().size());
    for (unsigned int i = 0; i < points.size(); i++)
    {
      Point3d normal = _calculateTangentPlane(i);
      pc.getNormals().push_back(normal);
      // add this point and all its neighbors to the Riemannian graph
      _populateRiemannianGraph(i);

      if (points[i].z > maxZ)
      {
        maxZ = points[i].z;
        maxZId = i;
      }
    }
    ////
    // handling reflections is very expensive and doesn't seem to work well w/ sparse data
    // so I've removed this step for now. -JRS
    ////
//     // make the assumption that the topmost point is pointing uppish
//     _processed.resize(points.size(), false);
//     _mstQueue.push(VertexOrder(maxZId, 0, -1));
//     if (pc.getNormals()[maxZId].p3 < 0.0)
//     {
//       pc.getNormals()[maxZId].p1 *= -1.0;
//       pc.getNormals()[maxZId].p2 *= -1.0;
//       pc.getNormals()[maxZId].p3 *= -1.0;
//     }
//     else if (pc.getNormals()[maxZId].p3 == 0.0)
//     {
//       /// @todo if this becomes a real problem, we can try the min z, max x, min x, ...
//       throw Exception("The uppermost point has no z component in the normal. Failing.");
//     }
//
//     // this is done to make sure all normals are pointed "out"
//     _processMst();

    bool island = false;
    for (unsigned int i = 0; i < _processed.size(); i++)
    {
      if (_processed[i] == false)
      {
//         cout << "no process " << i << endl;
        island = true;
      }
    }
    if (island)
    {
      cout << "  One or more islands were found. Increasing k may fix this, or you need to look "
        "at the data." << endl;
    }
  }

  void NormalEstimator::_calculateNearestNeighbors(int pointIndex)
  {
    PointCloud& pc = *_pc;
    const CloudPoint& cp = pc.getPoints()[pointIndex];
    _nnTmp[0] = cp.x;
    _nnTmp[1] = cp.y;
    _nnTmp[2] = cp.z;
    _it->reset(_nnTmp);
    _nearestNeighbors.resize(0);
    _nearestNeighbors.reserve(getKThreshold());
    _nnDistances.resize(0);
    _nnDistances.reserve(getKThreshold());
    while (_it->next() && (int)_nearestNeighbors.size() < getKThreshold())
    {
      _nearestNeighbors.push_back(_it->getId());
      _nnDistances.push_back(_it->getDistance());
    }
  }

  void NormalEstimator::_calculatePrincipalComponent(const std::vector<int>& neighbors, int k,
    Point3d& normal)
  {
    PointCloud& pc = *_pc;

    // if we don't have k neighbors reduce the size.
    k = std::min((int)neighbors.size(), k);

    Matrix dataMat(3, k);
    //std::cout << "Rows " << dataMat.nrows() << std::endl;
    Matrix deviates(3, k);
    SymmetricMatrix covar(3);
    DiagonalMatrix eigenValues(3);
    Matrix eigenVectors;
    ColumnVector means(3);
    means = 0.0;
    RowVector h(k);
    h = 1.0;

    for(int i = 0; i < k; i++)
    {
      const CloudPoint& cp = pc.getPoints()[neighbors[i]];
      //std::cout << cp << endl;

      dataMat.element(0, i) = cp.x;
      means.element(0)+= cp.x / (float)k;
      dataMat.element(1, i) = cp.y;
      means.element(1)+= cp.y / (float)k;
      dataMat.element(2, i) = cp.z;
      means.element(2)+= cp.z / (float)k;
    }

    //std::cout << "Means " << means.element(0) << " " << means.element(1) << " " << means.element(2) << std::endl;

    deviates = dataMat - (means * h);
    covar << (1.0/(float)k) * (deviates * deviates.t());
    //std::cout << covar << std::endl;
    Jacobi(covar, eigenValues, eigenVectors);

    eigenVectors = eigenVectors.i();

    // get the third principal component as the normal
    normal.p1 = eigenVectors.element(0, 0);
    normal.p2 = eigenVectors.element(0, 1);
    normal.p3 = eigenVectors.element(0, 2);
  }

  Point3d NormalEstimator::_calculateTangentPlane(int pointIndex)
  {
//    PointCloud& pc = *_pc;
    _calculateNearestNeighbors(pointIndex);
//     ////
//     // See [2] Algorithm 1 for details.
//     ////
//
//     // [2] uses a max count of 10 in their example.
//     const int MAX_COUNT = 10;
//     const int k0 = 15;
//
//     // standard deviation of the noise in the point cloud. I'm guessing 1cm for our data.
//     double sigmaN = .01;
//
//     // [2] uses these in their example. These values may need tweaking
//     double d1 = 1;
//     double d2 = 4;
//     double epsilon = .1;
//
//     int k = MAX(k0, _nearestNeighbors.size());
//     for (int count = 0; count < MAX_COUNT; count++)
//     {
//       double rOld = _nnDistances[k];
//       double rho = k / (PI * rOld * rOld);
//       double d = _calculateDistanceToBestPlane(k);
//       double mu = _calculateAverageDistanceToNeighbors(k);
//       double kappa = 2 * d / (mu * mu);
//       // [2] equation 5
//       double rNew = pow(
//         (1 / kappa) *
//         (d1 * sigmaN / sqrt(epsilon * rho) + d2 * sigmaN * sigmaN)
//         , 1/3);
//       k = ceil(PI * rho * rNew * rNew);
//       if (k > getKThreshold())
//       {
//         break;
//       }
//     }

    Point3d result;
    _calculatePrincipalComponent(_nearestNeighbors, getKThreshold(), result);

    return result;
  }

  void NormalEstimator::_populateRiemannianGraph(int index)
  {
    for (unsigned int i = 0; i < _nearestNeighbors.size(); i++)
    {
      int nId = _nearestNeighbors[i];
      if (nId != index)
      {
        // insert an edge that goes in both directions
        _edges[index].insert(nId);
        _edges[nId].insert(index);
      }
    }
  }

  void NormalEstimator::_processMst()
  {
    PointCloud& pc = *_pc;

    // using Djikstra's to calculate the MST
    while (_mstQueue.size() > 0)
    {
      VertexOrder vo = _mstQueue.top();
      _mstQueue.pop();

      // if we've already processed the point, no need to do it again
      if (_processed[vo.id] == true)
      {
        continue;
      }
      _processed[vo.id] = true;

      if (vo.parent >= 0)
      {
        Point3d& childNormal = pc.getNormals()[vo.id];
        const Point3d& parentNormal = pc.getNormals()[vo.parent];
        if (dotProduct(parentNormal, childNormal) < 0.0)
        {
          childNormal.reflect();
        }
      }

      // go through all our neighbors
      const HashSet<int>& neighbors = _edges[vo.id];
      for (HashSet<int>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
      {
        if (_processed[*it] == false)
        {
          // calculate the cost to reach this node and push it onto the queue for processing
          // later
          double costDelta = _calculateCost(vo.id, *it);
          _mstQueue.push(VertexOrder(*it, vo.cost + costDelta, vo.id));
        }
      }
    }
  }

  void NormalEstimator::_loadPointCloud(const PointCloud& pc)
  {
    const std::vector<CloudPoint>& points = pc.getPoints();

    boost::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(256));
    _rtree.reset(new HilbertRTree(mps, 3));
    _nnTmp.resize(3);
    _it.reset(new KnnIteratorNd(_rtree.get(), _nnTmp));

    std::vector<Box> boxes;
    std::vector<int> fids;
    boxes.reserve(points.size());
    fids.reserve(points.size());
    for (unsigned int i = 0; i < points.size(); i++)
    {
      Box b(3);
      b.setBounds(0, points[i].x, points[i].x);
      b.setBounds(1, points[i].y, points[i].y);
      b.setBounds(2, points[i].z, points[i].z);
      boxes.push_back(b);
      fids.push_back(i);
    }

    _rtree->bulkInsert(boxes, fids);
  }
}
