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
 * @copyright Copyright (C) 2015, 2017, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS_POINTS_H__
#define __TGS_POINTS_H__

//STD Includes
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>

namespace Tgs
{
  struct Point3d
  {
    Point3d()
    {
      p1 = p2 = p3 = 0;
    }

    Point3d(double _p1, double _p2, double _p3)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
    }

    void normalize()
    {
      double sum = sqrt(p1 * p1 + p2 * p2 + p3 * p3);
      p1 /= sum;
      p2 /= sum;
      p3 /= sum;
    }

    void reflect()
    {
      p1 = -p1;
      p2 = -p2;
      p3 = -p3;
    }

    const Point3d& operator*(double v)
    {
      p1 *= v;
      p2 *= v;
      p3 *= v;
      return *this;
    }

    double p1, p2, p3;
  };

  /**
  *  Holds a single point of data from the point cloud files
  *
  *  May extend in the future to read the time information as well
  */
  class CloudPoint
  {
  public:
    double x, y, z;
    double r, g, b, i;

    CloudPoint() {}

    CloudPoint(const CloudPoint& cp)
    {
      *this = cp;
    }

    CloudPoint(double x, double y, double z)
    {
      this->x = x;
      this->y = y;
      this->z = z;
    }

    CloudPoint& operator=(const CloudPoint& cp)
    {
      x = cp.x;
      y = cp.y;
      z = cp.z;
      r = cp.r;
      g = cp.g;
      b = cp.b;
      i = cp.i;
      return *this;
    }

    Point3d toPoint() const
    {
      return Point3d(x, y, z);
    }

  };

  inline std::ostream& operator<<(std::ostream& s, const CloudPoint& p)
  {
    s << "x: " << p.x << " y: " << p.y << " z: " << p.z;
    return s;
  }


  inline std::ostream& operator<<(std::ostream& s, const Point3d& p)
  {
    s << "x: " << p.p1 << " y: " << p.p2 << " z: " << p.p3;
    return s;
  }

  inline Point3d operator-(const Point3d& p1, const Point3d& p2)
  {
    Point3d result;
    result.p1 = p1.p1 - p2.p1;
    result.p2 = p1.p2 - p2.p2;
    result.p3 = p1.p3 - p2.p3;
    return result;
  }

  inline double dotProduct(const Point3d& p1, const Point3d& p2)
  {
    return p1.p1 * p2.p1 + p1.p2 * p2.p2 + p1.p3 * p2.p3;
  }

  inline double magnitude(const Point3d& p)
  {
    return sqrt(p.p1 * p.p1 + p.p2 * p.p2 + p.p3 * p.p3);
  }

  class PointCloud
  {
  public:
    PointCloud() {}

    void addPoint(const CloudPoint& cp) { _points.push_back(cp); }

    void clear() { _normals.clear(); _points.clear(); }

    const std::vector<Point3d>& getNormals() const { return _normals; }

    std::vector<Point3d>& getNormals() { return _normals; }

    const std::vector<CloudPoint>& getPoints() const { return _points; }

    void load(const std::string& fn)
    {
      std::fstream ins(fn.c_str(), std::fstream::in);

      if(ins.is_open())
      {
        while(!ins.eof())
        {
          char inLine[512];
          Tgs::CloudPoint cp;
          ins.getline(inLine, 512);
          std::stringstream ss;
          ss << inLine;
          ss >> cp.x >> cp.y >> cp.z >> cp.i >> cp.r >> cp.g >> cp.b;
          addPoint(cp);
        }
      }
      else
      {
        std::cout << "Unable to open " << fn << std::endl;
      }
    }

    void setNormals(const std::vector<Point3d>& normals) { _normals = normals; }

    void setPoints(const std::vector<CloudPoint>& points) { _points = points; }

  private:
    std::vector<CloudPoint> _points;
    std::vector<Point3d> _normals;
  };

  class Point2d
  {
  public:
    Point2d() {}

    Point2d(double p1, double p2) { this->p1 = p1; this->p2 = p2; }

    double p1, p2;
  };

  inline std::ostream& operator<<(std::ostream& s, const Point2d& p)
  {
    s << "x: " << p.p1 << " y: " << p.p2;
    return s;
  }

  class PointNd
  {
  public:
    std::vector<double> pt;
  };

} // End namespace
#endif
