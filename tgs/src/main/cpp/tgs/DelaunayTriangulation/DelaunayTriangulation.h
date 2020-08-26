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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef DELAUNAYTRIANGULATION_H
#define DELAUNAYTRIANGULATION_H

/**
 * @copyright (Text Below)
 * @author Dani Lischinski
 * This code was sourced from Graphics Gems IV and heavily modified to suite our needs.
 * http://www.karlchenofhell.org/cppswp/lischinski.pdf
 * http://tog.acm.org/resources/GraphicsGems/
 *
 * EULA: The Graphics Gems code is copyright-protected. In other words, you cannot claim the text
 * of the code as your own and resell it. Using the code is permitted in any program, product, or
 * library, non-commercial or commercial. Giving credit is not required, though is a nice gesture.
 * The code comes as-is, and if there are any flaws or problems with any Gems code, nobody involved
 * with Gems - authors, editors, publishers, or webmasters - are to be held responsible. Basically,
 * don't be a jerk, and remember that anything free comes with no guarantee.
 */

// Standard Include
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <tgs/DelaunayTriangulation/geom2d.h>

namespace Tgs
{

class InternalEdge;

/**
 * This class is so small that it can be copied and stored willy nilly instead of a pointer.
 */
class Edge
{
public:

  Edge() { _ie = 0; }
  Edge(InternalEdge* ie);
  Edge(const Edge& e) : _ie(e._ie) {}
  Edge(Edge& e) : _ie(e._ie) {}
  Edge& operator=(const Edge& e) { _ie = e._ie; return *this; }
  bool operator==(const Edge& e) const
  {
    if (_ie == e._ie)
    {
      return true;
    }
    if (getOriginX() == e.getOriginX() &&
        getOriginY() == e.getOriginY() &&
        getDestinationX() == e.getDestinationX() &&
        getDestinationY() == e.getDestinationY())
    {
      return true;
    }
    return false;
  }
  bool operator!=(const Edge& e) const { return !operator==(e); }
  bool operator<(const Edge& e) const;
  bool operator>(const Edge& e) const;

  InternalEdge* getInternalEdge() const { return _ie; }

  const Point2d& getOrigin() const;
  const Point2d& getDestination() const;

  double getOriginX() const;
  double getOriginY() const;
  double getDestinationX() const;
  double getDestinationY() const;

  const Edge getOriginNext() const;
  const Edge getOriginPrevious() const;
  const Edge getDestinationNext() const;
  const Edge getDestinationPrevious() const;

  const Edge getLeftNext() const;
  const Edge getLeftPrevious() const;

  const Edge getReverse() const;

  const Edge getRightNext() const;
  const Edge getRightPrevious() const;

  double getLength() const;

  std::string toString() const;

private:
  mutable InternalEdge* _ie;
};

class QuadEdge;
class Subdivision;

/**
 * Returns twice the area of the oriented triangle (a, b, c), i.e., the
 * area is positive if the triangle is oriented counterclockwise.
 */
inline double TriArea2(const Point2d & a, const Point2d & b, const Point2d & c)
{
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

/**
 * A triangle in the delaunay triangulation
 */
class Face
{
public:
  Face() { _id = -1; }

  Face(Face& other);
  Face(const Face& other);

  Face(Edge start);

  virtual ~Face();

  /**
   * Returns true if p is completely contained by this face.
   */
  bool contains(const Point2d& p) const;

  /**
   * Returns true if p is contained by or touches the edge of the face.
   */
  bool overlaps(const Point2d& p) const;

  Edge getEdge(int i) const { return _edges[i]; }

  bool operator==(const Face& other) const;
  bool operator<(const Face& other) const;
  Face& operator=(const Face& other);

  std::string toString() const;

private:
  int _id;
  // a face is made up of six edges. A set of three in each direction
  Edge _edges[6];
};

class EdgeIterator
{
public:

  EdgeIterator(const std::set<QuadEdge*>& edges);

  EdgeIterator() { _atEnd = true; }

  Edge operator*();
  Edge operator++(int);
  Edge& operator++();
  bool operator==(const EdgeIterator& other) const { return _atEnd == other._atEnd; }
  bool operator!=(const EdgeIterator& other) const { return !operator==(other); }
  EdgeIterator end() { EdgeIterator result; result._atEnd = true; return result; }

private:

  Edge _e;
  std::list<InternalEdge*> _todo;
  const std::set<QuadEdge *>* _edges;
  std::set<QuadEdge*>::const_iterator _it;

  bool _atEnd;
};

struct FaceCompare
{
  bool operator() (const Face* lhs, const Face* rhs) const
  {
    return *lhs < *rhs;
  }
};

class FaceIterator
{
public:

  FaceIterator(EdgeIterator it, EdgeIterator end);
  FaceIterator(const FaceIterator& from);

  FaceIterator() { _atEnd = true; }
  virtual ~FaceIterator();

  const Face& operator*();
  Face operator++(int);
  Face& operator++();
  bool operator==(const FaceIterator& other) const { return _atEnd == other._atEnd; }
  bool operator!=(const FaceIterator& other) const { return !operator==(other); }

private:

  Face* _f;
  EdgeIterator _it;
  EdgeIterator _end;
  std::set<Face*, FaceCompare> _done;

  bool _atEnd;
};

/**
 *
 * 1. Incremental Delaunay Triangulation, Dani Lischinski
 *    http://data.karlchenofhell.org/cppswp/lischinski.pdf
 */
class DelaunayTriangulation
{
public:

  DelaunayTriangulation();

  virtual ~DelaunayTriangulation();

  /**
   * Returns a face that contains this point. If the point lies on an vertex or edge then you will
   * arbitrarily get one of the faces that contain this point.
   */
  Face findContainingFace(double x, double y) const;

  /**
   * Returns the "starting" edge. Your window into the triangulation. The triangulation is
   * built while points are inserted so this should return very quickly. Any inserts that occur
   * will invalidate all previous edge pointers.
   */
  const Edge getStartingEdge() const;

  const EdgeIterator& getEdgeEnd() const { return _edgeEnd; }

  EdgeIterator getEdgeIterator() const;

  const FaceIterator& getFaceEnd() const { return _faceEnd; }

  FaceIterator getFaceIterator() const;

  const std::vector<Face>& getFaces();

  /**
   * Inserts a single point into the triangulation. If the data is inserted in random order the
   * performance will be O(n lg(n)). Sorted order inserts may result in O(n^2). Where n is the
   * number of points.
   */
  void insert(double x, double y);

private:

  Subdivision* _subdivision;
  double _x[3], _y[3];
  int _pointCount;
  std::vector<Face> _faces;
  std::map<Edge, std::pair<int, int>> _edgeToFace;

  EdgeIterator _edgeEnd;
  FaceIterator _faceEnd;

  void _addFaces(Edge e);
};

}

#endif // DELAUNAYTRIANGULATION_H
