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
 * @copyright Copyright (C) 2015, 2016, 2021 Maxar (http://www.maxar.com/)
 */

#include "SpinImageGenerator.h"

// Standard Includes
#include <iostream>
using namespace std;

#include "NormalEstimator.h"
#include "Points.h"
#include "SpinImage.h"
#include "SpinImageStack.h"

// Tgs
#include <tgs/Statistics/Random.h>

namespace Tgs
{
  SpinImageGenerator::SpinImageGenerator()
  {
    _alphaSize = 10;
    _betaSize = 20;
    _alphaMax = 10.0;
    _betaMax = 10.0;
    _k = -1;
    _relativeAngle = Normal;
  }

  void SpinImageGenerator::generateSingleImage(PointCloud& pc, int index, SpinImage& si)
  {
    _pc = &pc;
    _populateSpinImage(index, &si);
  }

  void SpinImageGenerator::generateStack(PointCloud& pc, SpinImageStack& stack, int maxImageCount)
  {
    _pc = &pc;
    _stack = &stack;

//     double minx = 1e300, miny = 1e300, minz = 1e300;
//     double maxx = -1e300, maxy = -1e300, maxz = -1e300;
    if (maxImageCount <= 0 || maxImageCount >= (int)pc.getPoints().size())
    {
      stack.resize(pc.getPoints().size());

      for (unsigned int i = 0; i < pc.getPoints().size(); i++)
      {
        SpinImage* image = new SpinImage();
        _populateSpinImage(i, image);
        image->setCompareAlgorithm(SpinImage::CompareAllCells);
        stack.addImage(i, image);
      }
    }
    else
    {
      vector<int> rIndex;
      rIndex.reserve(pc.getPoints().size());
      for (unsigned int i = 0; i < pc.getPoints().size(); i++)
      {
        rIndex.push_back(i);
      }
      for (unsigned int i = 0; i < pc.getPoints().size(); i++)
      {
        int r1 = Tgs::Random::instance()->generateInt(rIndex.size());
        int r2 = Tgs::Random::instance()->generateInt(rIndex.size());
        int tmp = rIndex[r1];
        rIndex[r1] = rIndex[r2];
        rIndex[r2] = tmp;
      }

      stack.resize(maxImageCount);
      for (int i = 0; i < maxImageCount; i++)
      {
        SpinImage* image = new SpinImage();
        _populateSpinImage(rIndex[i], image);
        image->setCompareAlgorithm(SpinImage::CompareAllCells);
        stack.addImage(i, image);
      }
    }
  }

  void SpinImageGenerator::_populateSpinImage(int id, SpinImage* image)
  {
    PointCloud& pc = *_pc;

    const std::vector<CloudPoint>& points = pc.getPoints();
    const std::vector<Point3d>& normals = pc.getNormals();

    Point3d p = points[id].toPoint();
    Point3d n;
    if (_relativeAngle == Normal)
    {
      n = normals[id];
    }
    else
    {
      n = Point3d(0, 0, 1);
    }

    image->clear();
    image->resize(_alphaSize, _betaSize);
    image->setBounds(_alphaMax, -_betaMax, _betaMax);
    if (_relativeAngle == Up)
    {
      image->setIgnoreReflections(false);
    }
//     cout << endl;
    for (unsigned int i = 0; i < points.size(); i++)
    {
      if (i != id)
      {
        const Point3d& x = points[i].toPoint();

        double mag = magnitude(x - p);
        double beta = dotProduct(n, x - p);
        double alpha = sqrt(mag * mag - beta * beta);
        image->addValue(alpha, beta);
//         printf("alpha %5.1f beta %5.1f ", alpha, beta);
//         cout << "p: " << p << "\tn: " << n << "\tx: " << x << endl;
      }
    }

  }
}
