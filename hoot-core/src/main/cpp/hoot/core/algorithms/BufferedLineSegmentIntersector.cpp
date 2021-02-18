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
 * @copyright Copyright (C) 2015, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */
#include "BufferedLineSegmentIntersector.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// opencv
#include <hoot/core/util/OpenCv.h>

using namespace cv;
using namespace geos::geom;
using namespace std;

namespace hoot
{

class LineSegmentResult
{
public:
  LineSegmentResult(const LineSegment& ls) :
    _c0(Coordinate::getNull()),
    _c1(Coordinate::getNull()),
    _dir(ls.p1.x - ls.p0.x, ls.p1.y - ls.p0.y),
    _ls(ls)
  {
  }

  double d2(const Coordinate& c1, const Coordinate& c2)
  {
    double dx = c1.x - c2.x;
    double dy = c1.y - c2.y;
    return dx * dx + dy * dy;
  }

  void addCoordinate(const Coordinate& c)
  {
    // each time a coordinate is added I'm checking to see if it is closer to the start or end
    // coordinate. While there are faster approaches using a bunch of if statements, this should
    // be very robust and relatively simple.
    if (c.isNull() == false)
    {
      if (_c0.isNull())
      {
        _c0 = c;
      }
      else
      {
        // if this coordinate is closer to the start coordinate
        if (d2(c, _ls.p0) < d2(_c0, _ls.p0))
        {
          _c0 = c;
        }
      }

      if (_c1.isNull())
      {
        _c1 = c;
      }
      else
      {
        // if this coordinate is closer to the end coordinate
        if (d2(c, _ls.p1) < d2(_c1, _ls.p1))
        {
          _c1 = c;
        }
      }
    }
  }

  bool isValid() const { return _c1.isNull() == false && _c0 != _c1; }

  LineSegment toLineSegment() const
  {
    if (_c1.isNull())
    {
      return LineSegment();
    }
    else
    {
      return LineSegment(_c0, _c1);
    }
  }

private:
  Coordinate _c0, _c1;
  Vec2d _dir;
  const LineSegment& _ls;
};

BufferedLineSegmentIntersector::BufferedLineSegmentIntersector()
{
}

inline double sgn(double x)
{
  if (x < 0)
  {
    return -1;
  }
  else
  {
    return 1;
  }
}

bool BufferedLineSegmentIntersector::isWithinLineSegment(const LineSegment& ls, const Coordinate& c)
{
  const Coordinate& a = ls.p0;
  const Coordinate& b = ls.p1;

  // calculate the lenght of the line segment squared
  double dbax = b.x - a.x;
  double dbay = b.y - a.y;
  double squaredLengthBa = dbax * dbax + dbay * dbay;

  double dcax = c.x - a.x;
  double dcay = c.y - a.y;

  double dcbx = c.x - b.x;
  double dcby = c.y - b.y;

  // if the distance from c to both end points is less than the length of the line then it
  // must be between the two endpoints.
  return (dcax * dcax + dcay * dcay) <= squaredLengthBa
      && (dcbx * dcbx + dcby * dcby) <= squaredLengthBa;
}

void BufferedLineSegmentIntersector::circleIntersection(const Coordinate& origin, Meters r,
  const LineSegment& l, Coordinate& p1, Coordinate& p2)
{
  // the algorithm assume the circle is centered at 0,0
  LineSegment c(l.p0.x - origin.x, l.p0.y - origin.y, l.p1.x - origin.x, l.p1.y - origin.y);
  double dx = c.p1.x - c.p0.x;
  double dy = c.p1.y - c.p0.y;
  double dr2 = dx * dx + dy * dy;
  double D = c.p0.x * c.p1.y - c.p1.x * c.p0.y;

  double delta = r * r * dr2 - D * D;

  // if the line doesn't intersect the circle.
  if (delta < 0)
  {
    p1 = Coordinate::getNull();
    p2 = Coordinate::getNull();
  }
  // if there is at least one intersection.
  else
  {
    p1.x = (D * dy + sgn(dy) * dx * sqrt(r * r * dr2 - D * D)) / dr2 + origin.x;
    p1.y = (-D * dx + abs(dy) * sqrt(r * r * dr2 - D * D)) / dr2 + origin.y;
    // if the point isn't on the line segment
    if (!isWithinLineSegment(l, p1))
    {
      p1 = Coordinate::getNull();
    }

    // if this is a tanget (one point in common)
    if (delta == 0)
    {
      p2 = Coordinate::getNull();
    }
    // if this has two intersections
    else
    {
      p2.x = (D * dy - sgn(dy) * dx * sqrt(r * r * dr2 - D * D)) / dr2 + origin.x;
      p2.y = (-D * dx - abs(dy) * sqrt(r * r * dr2 - D * D)) / dr2 + origin.y;
      // if the point isn't on the line segment
      if (!isWithinLineSegment(l, p2))
      {
        p2 = Coordinate::getNull();
      }
    }
  }
}

bool BufferedLineSegmentIntersector::intersect(const LineSegment& a, Meters buffer,
  const LineSegment& b, LineSegment& result)
{
  LineSegmentResult lsr(b);

  // if the start and end of b are both within buffer then return b.
  if (a.distance(b.p0) <= buffer)
  {
    lsr.addCoordinate(b.p0);
  }

  if (a.distance(b.p1) <= buffer)
  {
    lsr.addCoordinate(b.p1);
    if (lsr.isValid())
    {
      result = lsr.toLineSegment();
      return true;
    }
  }

  //skip the line intersection check if a begins and ends on the same point
  if (a.p0 != a.p1)
  {
    // intersect with the two parallel lines

    // calculate the direction of the vector and the direction in 90deg to the right and left.
    Vec2d va(a.p1.x - a.p0.x, a.p1.y - a.p0.y);
    va = va * (1.0 / norm(va));
    Vec2d va90(-va[1], va[0]);

    LineSegment pLeft(a.p0.x + va90[0] * buffer,
      a.p0.y + va90[1] * buffer,
      a.p1.x + va90[0] * buffer,
      a.p1.y + va90[1] * buffer);
    LineSegment pRight(a.p0.x - va90[0] * buffer,
      a.p0.y - va90[1] * buffer,
      a.p1.x - va90[0] * buffer,
      a.p1.y - va90[1] * buffer);

    Coordinate tmp;
    if (pLeft.intersection(b, tmp))
    {
      lsr.addCoordinate(tmp);
      if (lsr.isValid())
      {
        result = lsr.toLineSegment();
        return true;
      }
    }

    if (pRight.intersection(b, tmp))
    {
      lsr.addCoordinate(tmp);
      if (lsr.isValid())
      {
        result = lsr.toLineSegment();
        return true;
      }
    }
  }

  // intersect with the two circles
  {
    Coordinate p0, p1;
    circleIntersection(a.p0, buffer, b, p0, p1);
    lsr.addCoordinate(p0);
    lsr.addCoordinate(p1);

    circleIntersection(a.p1, buffer, b, p0, p1);
    lsr.addCoordinate(p0);
    lsr.addCoordinate(p1);
  }

  result = lsr.toLineSegment();
  return lsr.isValid();
}

}
