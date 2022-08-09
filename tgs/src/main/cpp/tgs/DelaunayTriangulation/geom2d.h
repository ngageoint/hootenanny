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
 * @copyright Copyright (C) 2015, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef GEOM2D_H
#define GEOM2D_H

#include <math.h>
#include <iostream>

#ifndef ABS
#define ABS(a)	((a) >= 0 ? (a) : -(a))
#endif

#ifndef MAX
#define MAX(a, b)	((a) >= (b) ? (a) : (b))
#define MIN(a, b)	((a) <= (b) ? (a) : (b))
#endif

#ifndef TRUE
#define FALSE 0
#define TRUE  1
#endif

#define EPS 1e-6

using Real = double;

namespace Tgs
{

class Vector2d
{
public:
  Real x, y;
  Vector2d()
  {
    x = 0;
    y = 0;
  }
  Vector2d(Real a, Real b)
  {
    x = a;
    y = b;
  }
  Real norm() const;
  void normalize();
  Vector2d operator+(const Vector2d&) const;
  Vector2d operator-(const Vector2d&) const;
  friend Vector2d operator*(Real, const Vector2d&);
  friend Real dot(const Vector2d&, const Vector2d&);
  friend std::istream& operator>>(std::istream&, Vector2d&);
  friend std::ostream& operator<<(std::ostream&, const Vector2d&);
};

class Point2d
{
public:
  Real x, y;
  Point2d()
  {
    x = 0;
    y = 0;
  }
  Point2d(Real a, Real b)
  {
    x = a;
    y = b;
  }

  Real distance(const Point2d& p) const
  {
    Real dx = x - p.x;
    Real dy = y - p.y;
    return sqrt(dx * dx + dy * dy);
  }

  Point2d operator+(const Vector2d&) const;
  Vector2d operator-(const Point2d&) const;
  int operator==(const Point2d&) const;
  bool operator<(const Point2d& p) const
  {
    if (x < p.x)
      return true;
    else if (x == p.x)
      return y < p.y;
    return false;
  }
  friend std::istream& operator>>(std::istream&, Point2d&);
  friend std::ostream& operator<<(std::ostream&, const Point2d&);
};

class Line
{
public:
  Line() : a(0.0), b(0.0), c(0.0) { }
  Line(const Point2d&, const Point2d&);
  Real eval(const Point2d&) const;
  int classify(const Point2d&) const;
private:
  Real a, b, c;
};

// Vector2d:

inline Real Vector2d::norm() const
{
  return sqrt(x * x + y * y);
}

inline void Vector2d::normalize()
{
  Real len;

  if ((len = sqrt(x * x + y * y)) == 0.0)
    std::cerr << "Vector2d::normalize: Division by 0\n";
  else
  {
    x /= len;
    y /= len;
  }
}

inline Vector2d Vector2d::operator+(const Vector2d& v) const
{
  return Vector2d(x + v.x, y + v.y);
}

inline Vector2d Vector2d::operator-(const Vector2d& v) const
{
  return Vector2d(x - v.x, y - v.y);
}

inline Vector2d operator*(Real c, const Vector2d& v)
{
  return Vector2d(c * v.x, c * v.y);
}

inline Real dot(const Vector2d& u, const Vector2d& v)
{
  return u.x * v.x + u.y * v.y;
}

inline std::ostream& operator<<(std::ostream& os, const Vector2d& v)
{
  os << '(' << v.x << ", " << v.y << ')';
  return os;
}

inline std::istream& operator>>(std::istream& is, Vector2d& v)
{
  is >> v.x >> v.y;
  return is;
}

// Point2d:

inline Point2d Point2d::operator+(const Vector2d& v) const
{
  return Point2d(x + v.x, y + v.y);
}

inline Vector2d Point2d::operator-(const Point2d& p) const
{
  return Vector2d(x - p.x, y - p.y);
}

inline int Point2d::operator==(const Point2d& p) const
{
  return ((*this - p).norm() < EPS);
}

inline std::istream& operator>>(std::istream& is, Point2d& p)
{
  is >> p.x >> p.y;
  return is;
}

inline std::ostream& operator<<(std::ostream& os, const Point2d& p)
{
  os << '(' << p.x << ", " << p.y << ')';
  return os;
}

// Line:

/** Computes the normalized line equation through the points p and q. */
inline Line::Line(const Point2d& p, const Point2d& q)
{
  Vector2d t = q - p;
  Real len = t.norm();
  a =   t.y / len;
  b = - t.x / len;
  c = -(a * p.x + b * p.y);
}

/** Plugs point p into the line equation. */
inline Real Line::eval(const Point2d& p) const
{
  return (a * p.x + b * p.y + c);
}

/** Returns -1, 0, or 1, if p is to the left of, on, or right of the line, respectively. */
inline int Line::classify(const Point2d& p) const
{
  Real d = eval(p);
  return (d < -EPS) ? -1 : (d > EPS ? 1 : 0);
}

}

#endif
