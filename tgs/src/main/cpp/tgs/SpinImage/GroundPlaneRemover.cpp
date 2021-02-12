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

#include "GroundPlaneRemover.h"

// Standard Includes
#include <algorithm>
#include <iostream>
using namespace std;

namespace Tgs
{

  bool GroundPlaneRemover::_areNeighborsVertical(int id) const
  {
    std::vector<Point3d>& normals = _pc->getNormals();
    const HashSet<int>& neighbors = _edges[id];
    for (HashSet<int>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
    {
      const Point3d& p = normals[*it];
      if (_isVertical(p) == false)
      {
        return false;
      }
    }
    return true;
  }

  bool GroundPlaneRemover::_isVertical(const Point3d& p) const
  {
    static const Point3d vertical(0, 0, 1);
    return fabs(dotProduct(p, vertical)) >= .90;
  }

  void GroundPlaneRemover::removeGroundPlane(PointCloud& pc)
  {
    Point3d vertical(0, 0, 1);

    calculateNormals(pc);

    std::vector<Point3d>& normals = pc.getNormals();
    const std::vector<CloudPoint>& points = pc.getPoints();
    double minZ = 1e300;
    double maxZ = -1e300;
    for (unsigned int i = 0; i < points.size(); i++)
    {
      minZ = std::min(points[i].z, minZ);
      maxZ = std::max(points[i].z, maxZ);
    }

    std::vector<CloudPoint> newPoints;
    newPoints.reserve(points.size());
    std::vector<Point3d> newNormals;
    newNormals.reserve(points.size());

    for (unsigned int i = 0; i < points.size(); i++)
    {
      // if the point is 10cm above the bottom or
      // it isn't pointing vertically or
      // it's neighbors aren't pointing vertically
      if (points[i].z >= minZ + .20 ||
        _isVertical(normals[i]) == false ||
        _areNeighborsVertical(i) == false)
      {
        newPoints.push_back(points[i]);
        newNormals.push_back(normals[i]);
      }
    }

    pc.setPoints(newPoints);
    pc.setNormals(newNormals);
  }
}
