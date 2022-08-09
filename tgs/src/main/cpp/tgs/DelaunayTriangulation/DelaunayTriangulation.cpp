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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "DelaunayTriangulation.h"

// Standard
#include <algorithm>
#include <sstream>

// Tgs
#include <tgs/HashMap.h>
#include <tgs/TgsException.h>

using namespace std;

namespace Tgs
{

/// An internal check for robustness so we don't get caught in an endless loop
const int MAX_LOOPS = 100000;

class InternalEdge
{
  friend class QuadEdge;
  friend void Splice(InternalEdge *, InternalEdge *);
private:
  int num;
  InternalEdge *next;
  Point2d *data;
public:
  InternalEdge()
    : next(nullptr),
      data(nullptr)
  {
  }

  InternalEdge *Rot();
  InternalEdge *invRot();
  InternalEdge *Sym();
  InternalEdge *Onext();
  InternalEdge *Oprev();
  InternalEdge *Dnext();
  InternalEdge *Dprev();
  InternalEdge *Lnext();
  InternalEdge *Lprev();
  InternalEdge *Rnext();
  InternalEdge *Rprev();
  Point2d *Org();
  Point2d *Dest();
  const Point2d & Org2d() const;
  const Point2d & Dest2d() const;
  void EndPoints(Point2d *, Point2d *);
  QuadEdge *Qedge()
  {
    return reinterpret_cast<QuadEdge *>(this - num);
  }

};

inline std::ostream& operator<<(std::ostream& os, const InternalEdge& e)
{
  os << e.Org2d() << " -> " << e.Dest2d();
  return os;
}

Edge::Edge(InternalEdge* ie) { _ie = ie; }

const Point2d& Edge::getOrigin() const { return _ie->Org2d(); }
const Point2d& Edge::getDestination() const { return _ie->Dest2d(); }

double Edge::getOriginX() const { return _ie->Org2d().x; }
double Edge::getOriginY() const { return _ie->Org2d().y; }
double Edge::getDestinationX() const { return _ie->Dest2d().x; }
double Edge::getDestinationY() const { return _ie->Dest2d().y; }

Edge Edge::getOriginNext() const { return _ie->Onext(); }
Edge Edge::getOriginPrevious() const { return _ie->Oprev(); }
Edge Edge::getDestinationNext() const { return _ie->Dnext(); }
Edge Edge::getDestinationPrevious() const { return _ie->Dprev(); }

Edge Edge::getLeftNext() const { return _ie->Lnext(); }
Edge Edge::getLeftPrevious() const { return _ie->Lprev(); }

Edge Edge::getReverse() const { return _ie->Sym(); }

Edge Edge::getRightNext() const { return _ie->Rnext(); }
Edge Edge::getRightPrevious() const { return _ie->Rprev(); }

inline double distance(double x1, double x2, double y1, double y2)
{
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double Edge::getLength() const { return distance(getOriginX(), getDestinationX(), getOriginY(), getDestinationY()); }

bool Edge::operator<(const Edge& e) const
{
  if (operator==(e))
    return false;
  else if (getOriginX() < e.getOriginX())
    return true;
  else if (getOriginX() == e.getOriginX())
  {
    if (getOriginY() < e.getOriginY())
      return true;
    else if (getOriginY() == e.getOriginY())
    {
      if (getDestinationX() < e.getDestinationX())
        return true;
      else if (getDestinationX() == e.getDestinationX())
      {
        if (getDestinationY() < e.getDestinationY())
          return true;
        else if (getDestinationY() == e.getDestinationY())
          throw Tgs::Exception("Unexpected path.");
      }
    }
  }
  return false;
}

bool Edge::operator>(const Edge& e) const
{
  return !(*this < e || *this == e);
}


std::string Edge::toString() const
{
  std::stringstream strm;

  if (_ie == nullptr)
    strm << "(null)" << endl;
  else
  {
    strm << "origin: " << getOriginX() << ", " << getOriginY() << endl;
    strm << "dest:   " << getDestinationX() << ", " << getDestinationY() << endl;
  }

  return strm.str();
}

class Subdivision
{
public:

  Subdivision(const Point2d &, const Point2d &, const Point2d &);
  virtual ~Subdivision();
  void InsertSite(const Point2d &);
  InternalEdge *Connect(InternalEdge * a, InternalEdge * b);
  void DeleteEdge(InternalEdge * e);
  void Draw();
  InternalEdge* MakeEdge();
  Point2d* MakePoint(const Point2d& pt);

  InternalEdge* getStartingEdge() const { return startingEdge; }

  const set<QuadEdge*>& getEdges() const { return _edges; }

  InternalEdge *Locate(const Point2d &);

private:

  InternalEdge * startingEdge;

  set<QuadEdge*> _edges;
  vector<Point2d*> _data;
};

class QuadEdge
{
public:

  QuadEdge();
  // should only be used by MakeEdge
  InternalEdge e[4];

  InternalEdge& getInternalEdge(int i) { return e[i]; }
};

inline QuadEdge::QuadEdge()
{
  e[0].num = 0;
  e[1].num = 1;
  e[2].num = 2;
  e[3].num = 3;
  e[0].next = &(e[0]);
  e[1].next = &(e[3]);
  e[2].next = &(e[2]);
  e[3].next = &(e[1]);
}

/************************* Edge Algebra *************************************/
// Return the dual of the current edge, directed from its right to its left.
inline InternalEdge *InternalEdge::Rot()
{
  return (num < 3) ? this + 1 : this - 3;
}

// Return the dual of the current edge, directed from its left to its right.
// .1 Incremental Delaunay Triangulation } 7
inline InternalEdge *InternalEdge::invRot()
{
  return (num > 0) ? this - 1 : this + 3;
}

// Return the edge from the destination to the origin of the current edge.
inline InternalEdge *InternalEdge::Sym()
{
  return (num < 2) ? this + 2 : this - 2;
}

// Return the next ccw edge around (from) the origin of the current edge.
inline InternalEdge *InternalEdge::Onext()
{
  return next;
}

// Return the next cw edge around (from) the origin of the current edge.
inline InternalEdge *InternalEdge::Oprev()
{
  return Rot()->Onext()->Rot();
}

// Return the next ccw edge around (into) the destination of the current edge.
inline InternalEdge *InternalEdge::Dnext()
{
  return Sym()->Onext()->Sym();
}

// Return the next cw edge around (into) the destination of the current edge.
inline InternalEdge *InternalEdge::Dprev()
{
  return invRot()->Onext()->invRot();
}

// Return the ccw edge around the left face following the current edge.
inline InternalEdge *InternalEdge::Lnext()
{
  return invRot()->Onext()->Rot();
}

// Return the ccw edge around the left face before the current edge.
inline InternalEdge *InternalEdge::Lprev()
{
  return Onext()->Sym();
}

// Return the edge around the right face ccw following the current edge.
inline InternalEdge *InternalEdge::Rnext()
{
  return Rot()->Onext()->invRot();
}

// Return the edge around the right face ccw before the current edge.
inline InternalEdge *InternalEdge::Rprev()
{
  return Sym()->Onext();
}

/************** Access to data pointers *************************************/
inline Point2d *InternalEdge::Org()
{
  return data;
}

inline Point2d *InternalEdge::Dest()
{
  return Sym()->data;
}

inline const Point2d & InternalEdge::Org2d() const
{
  return *data;
}
inline const Point2d & InternalEdge::Dest2d() const
{
  return (num < 2) ? *((this + 2)->data) : *((this - 2)->data);
}
inline void InternalEdge::EndPoints(Point2d * or1, Point2d * de)
{
  data = or1;
  Sym()->data = de;
}

/*********************** Basic Topological Operators ************************/
InternalEdge *Subdivision::MakeEdge()
{
  QuadEdge *ql = new QuadEdge;
  _edges.insert(ql);
  return ql->e;
}

Point2d* Subdivision::MakePoint(const Point2d& pt)
{
  Point2d* point = new Point2d(pt);
  _data.push_back(point);
  return point;
}

void Splice(InternalEdge * a, InternalEdge * b)
{
  // This operator affects the two edge rings around the origins of a and b,
  // and, independently, the two edge rings around the left faces of a and b.
  // In each case, (i) if the two rings are distinct, Splice will combine
  // them into one; (ii) if the two are the same ring, Splice will break it
  // into two separate pieces.
  // Thus, Splice can be used both to attach the two edges together, and
  // to break them apart. See Guibas and Stolfi (1985) p.96 for more details
  // and illustrations.

  InternalEdge *alpha = a->Onext()->Rot();
  InternalEdge *beta = b->Onext()->Rot();
  InternalEdge *t1 = b->Onext();
  InternalEdge *t2 = a->Onext();
  InternalEdge *t3 = beta->Onext();
  InternalEdge *t4 = alpha->Onext();
  a->next = t1;
  b->next = t2;
  alpha->next = t3;
  beta->next = t4;
}

void Subdivision::DeleteEdge(InternalEdge * e)
{
  Splice(e, e->Oprev());
  Splice(e->Sym(), e->Sym()->Oprev());
  _edges.erase(e->Qedge());
  delete e->Qedge();
}

/************* Topological Operations for Delaunay Diagrams *****************/
Subdivision::Subdivision(const Point2d & a, const Point2d & b, const Point2d & c)
// Initialize a subdivision to the triangle defined by the points a, b, c.
{
  Point2d* da = MakePoint(a);
  Point2d* db = MakePoint(b);
  Point2d* dc = MakePoint(c);
  InternalEdge *ea = MakeEdge();
  ea->EndPoints(da, db);
  InternalEdge *eb = MakeEdge();
  Splice(ea->Sym(), eb);
  eb->EndPoints(db, dc);
  InternalEdge *ec = MakeEdge();
  Splice(eb->Sym(), ec);
  ec->EndPoints(dc, da);
  Splice(ec->Sym(), ea);
  startingEdge = ea;
}

Subdivision::~Subdivision()
{
  for (auto edge : _edges)//set<QuadEdge*>::iterator it = _edges.begin(); it != _edges.end(); ++it)
    delete edge;
  _edges.clear();
  for (auto data : _data)//vector<Point2d*>::iterator it = _data.begin(); it != _data.end(); ++it)
    delete data;
  _data.clear();
}

InternalEdge *Subdivision::Connect(InternalEdge * a, InternalEdge * b)
{
  // Add a new edge e connecting the destination of a to the
  // origin of b, in such a way that all three have the same
  // left face after the connection is complete.
  // Additionally, the data pointers of the new edge are set.
  InternalEdge *e = MakeEdge();
  Splice(e, a->Lnext());
  Splice(e->Sym(), b);
  e->EndPoints(a->Dest(), b->Org());
  return e;
}

void Swap(InternalEdge * e)
{
  // Essentially turns edge e counterclockwise inside its enclosing
  // quadrilateral. The data pointers are modified accordingly.
  InternalEdge *a = e->Oprev();
  InternalEdge *b = e->Sym()->Oprev();
  Splice(e, a);
  Splice(e->Sym(), b);
  Splice(e, a->Lnext());
  Splice(e->Sym(), b->Lnext());
  e->EndPoints(a->Dest(), b->Dest());
}

int InCircle(const Point2d & a, const Point2d & b, const Point2d & c, const Point2d & d)
{
  // Returns TRUE if the point d is inside the circle defined by the
  // points a, b, c. See Guibas and Stolfi (1985) p.107.
  return (a.x * a.x + a.y * a.y) * TriArea2(b, c, d) - (b.x * b.x + b.y * b.y) * TriArea2(a, c, d) +
         (c.x * c.x + c.y * c.y) * TriArea2(a, b, d) - (d.x * d.x + d.y * d.y) * TriArea2(a, b, c) >
         0;
}

int ccw(const Point2d & a, const Point2d & b, const Point2d & c)
{
  // Returns TRUE if the points a, b, c are in a counterclockwise order
  return (TriArea2(a, b, c) > 0);
}

int RightOf(const Point2d & x, const InternalEdge * e)
{
  return ccw(x, e->Dest2d(), e->Org2d());
}

int LeftOf(const Point2d & x, const InternalEdge * e)
{
  return ccw(x, e->Org2d(), e->Dest2d());
}

int OnEdge(const Point2d & x, const InternalEdge * e)
{
  // A predicate that determines if the point x is on the edge e.
  // The point is considered on if it is in the EPS-neighborhood
  // of the edge.
  Real t1, t2, t3;
  t1 = (x - e->Org2d()).norm();
  t2 = (x - e->Dest2d()).norm();
  if (t1 < EPS || t2 < EPS)
    return TRUE;

  t3 = (e->Org2d() - e->Dest2d()).norm();
  if (t1 > t3 || t2 > t3)
    return FALSE;

  Line line(e->Org2d(), e->Dest2d());
  return (fabs(line.eval(x)) < EPS);
}

/************* An Incremental Algorithm for the Construction of *************/
/************************ Delaunay Diagrams *********************************/

// Returns an edge e, s.t. either x is on e, or e is an edge of
// a triangle containing x. The search starts from startingEdge
// and proceeds in the general direction of x. Based on the
// pseudocode in Guibas and Stolfi (1985) p.121.
InternalEdge *Subdivision::Locate(const Point2d & x)
{
  InternalEdge *e = startingEdge;
  int looping = 0;
  while (TRUE)
  {
    if (x == e->Org2d() || x == e->Dest2d())
    {
      if (looping > MAX_LOOPS)
        cout << "on point" << endl;
      return e;
    }
    else if (RightOf(x, e))
    {
      if (looping > MAX_LOOPS)
        cout << "right of " << *e << endl;
      e = e->Sym();
    }
    else if (!RightOf(x, e->Onext()))
    {
      if (looping > MAX_LOOPS)
      {
        cout << "!RightOf(x, e->Onext()) " << *e->Onext() << endl;
        cout << TriArea2(x, e->Onext()->Dest2d(), e->Onext()->Org2d()) << endl;
      }
      e = e->Onext();
    }
    else if (!RightOf(x, e->Dprev()))
    {
      if (looping > MAX_LOOPS)
        cout << "!RightOf(x, e->Dprev()) " << *e->Dprev() << endl;
      e = e->Dprev();
    }
    else
    {
      if (looping > MAX_LOOPS)
        cout << "found face" << endl;
      return e;
    }
    looping++;
    if (looping > MAX_LOOPS)
      cout << Edge(e).toString() << endl;
    if (looping > MAX_LOOPS + 20)
    {
      cout << endl;
      cout << "Point: " << x.x  << ", " << x.y << endl;
      throw Exception("Looping too many times in Delaunay triangulation");
    }
  }
}

// Inserts a new point into a subdivision representing a Delaunay
// triangulation, and fixes the affected edges so that the result
// is still a Delaunay triangulation. This is based on the
// pseudocode from Guibas and Stolfi (1985) p.120, with slight
// modifications and a bug fix.
void Subdivision::InsertSite(const Point2d & x)
{
  InternalEdge *e = Locate(x);
  if ((x == e->Org2d()) || (x == e->Dest2d()))	// point is already in
    return;
  else if (OnEdge(x, e))
  {
    e = e->Oprev();
    DeleteEdge(e->Onext());
  }

  int looping = 0;

  // Connect the new point to the vertices of the containing
  // triangle (or quadrilateral, if the new point fell on an
  // existing edge.)
  InternalEdge *base = MakeEdge();
  base->EndPoints(e->Org(), MakePoint(x));
  Splice(base, e);
  startingEdge = base;
  do
  {
    base = Connect(e, base->Sym());
    e = base->Oprev();
    looping++;
    if (looping > MAX_LOOPS)
      throw Exception("Looping too many times in Delaunay triangulation");
  }
  while (e->Lnext() != startingEdge);

  looping = 0;
  // Examine suspect edges to ensure that the Delaunay condition is satisfied.
  do
  {
    const InternalEdge *t = e->Oprev();
    if (RightOf(t->Dest2d(), e) && InCircle(e->Org2d(), t->Dest2d(), e->Dest2d(), x))
    {
      Swap(e);
      e = e->Oprev();
    }
    else if (e->Onext() == startingEdge)  // no more suspect edges
      return;
    else  // pop a suspect edge
      e = e->Onext()->Lprev();
    looping++;
    if (looping > MAX_LOOPS)
      throw Exception("Looping too many times in Delaunay triangulation");
  }
  while (TRUE);
}

EdgeIterator::EdgeIterator(const set<QuadEdge *> &edges)
{
  _edges = &edges;
  _it = _edges->begin();
  _atEnd = false;
  operator++(0);
}

Edge EdgeIterator::operator*() const
{
  return _e;
}

Edge EdgeIterator::operator++(int)
{
  Edge tmp(_e);
  operator++();
  return tmp;
}

Edge& EdgeIterator::operator++()
{
  while (_todo.empty() && _it != _edges->end())
  {
    QuadEdge* qe = *_it;
    ++_it;
    for (int i = 0; i < 4; i++)
    {
      if (qe->getInternalEdge(i).Org() != nullptr)
        _todo.push_back(&qe->getInternalEdge(i));
    }
  }

  if (_todo.empty() && _it == _edges->end())
  {
    _atEnd = true;
    return _e;
  }

  _e = Edge(*_todo.begin());
  _todo.pop_front();

  return _e;
}

Face::Face(const Face& other)
{
  *this = other;
}

Face::Face(Edge start)
{
  _id = -1;

  int i = 0;
  Edge e = start;
  _edges[i++] = e;
  e = e.getLeftNext();
  _edges[i++] = e;
  e = e.getLeftNext();
  _edges[i++] = e;

  e = start.getReverse();
  _edges[i++] = e;
  e = e.getRightNext();
  _edges[i++] = e;
  e = e.getRightNext();
  _edges[i++] = e;

  std::sort(_edges, _edges + 6);
}

Face::~Face()
{
  _id = -2;
  for (int i = 0; i < 6; i++)
    _edges[i] = Edge();
}

bool sameSide(const Point2d& p1, const Point2d& p2, const Point2d& a, const Point2d& b)
{
  return ccw(p1, a, b) == ccw(p2, a, b);
}

bool Face::contains(const Point2d& p) const
{
  //cout << toString() << endl;
  // taken from http://www.blackpawn.com/texts/pointinpoly/
  Point2d a(_edges[0].getOriginX(), _edges[0].getOriginY());
  Point2d b(_edges[2].getOriginX(), _edges[2].getOriginY());
  Point2d c(_edges[4].getOriginX(), _edges[4].getOriginY());
  return sameSide(p, a, b, c) && sameSide(p, b, a, c) && sameSide(p, c, a, b);
}

bool Face::operator==(const Face& other) const
{
  for (int i = 0; i < 6; i++)
  {
    if (_edges[i] != other._edges[i])
      return false;
  }
  return true;
}

bool Face::operator<(const Face& other) const
{
  for (int i = 0; i < 6; i++)
  {
    if (_edges[i] < other._edges[i])
      return true;
    else if (_edges[i] > other._edges[i])
      return false;
  }
  return false;
}

Face& Face::operator=(const Face& other)
{
  if (this != &other)
  {
    _id = other._id;
    for (int i = 0; i < 6; i++)
      _edges[i] = other._edges[i];
  }
  return *this;
}

bool Face::overlaps(const Point2d& p) const
{
  if (contains(p))
    return true;

  for (size_t i = 0; i < 6; i+=2)
  {
    if (OnEdge(p, _edges[i].getInternalEdge()))
      return true;
  }

  return false;
}

std::string Face::toString() const
{
  std::stringstream strm;
  strm << "Face:";
  for (int i = 0; i < 6; i+=2)
    strm << " " << _edges[i].getOriginX() << "," << _edges[i].getOriginY();

  return strm.str();
}

FaceIterator::FaceIterator(const FaceIterator& from)
  : _it(from._it),
    _end(from._end),
    _done(from._done),
    _atEnd(from._atEnd)
{
  _f = new Face(*_it);
}

FaceIterator::FaceIterator(const EdgeIterator& it, const EdgeIterator& end)
  : _f(new Face(*it)),
    _it(it),
    _end(end),
    _atEnd(false)
{
  _done.insert(_f);
}

FaceIterator::~FaceIterator()
{
  for (auto ptr : _done)
    delete ptr;
}

const Face& FaceIterator::operator*() const
{
  return *_f;
}

Face FaceIterator::operator ++(int)
{
  Face tmp(*_f);
  operator++();
  return tmp;
}

Face& FaceIterator::operator++()
{
  if (_it == _end)
    _atEnd = true;
  else
  {
    bool alreadyDone;
    do
    {
      _f = new Face(*_it);
      ++_it;

      alreadyDone = _done.find(_f) != _done.end();
      if (alreadyDone)
        delete _f;
    } while (alreadyDone && _it != _end);

    if (!alreadyDone)
      _done.insert(_f);
    if (_it == _end)
      _atEnd = true;
  }
  return *_f;
}

DelaunayTriangulation::DelaunayTriangulation()
  : _subdivision(nullptr),
    _x{0.0, 0.0, 0.0},
    _y{0.0, 0.0, 0.0},
    _pointCount(0)
{
}

DelaunayTriangulation::~DelaunayTriangulation()
{
  delete _subdivision;
}

Face DelaunayTriangulation::findContainingFace(double x, double y) const
{
  Point2d p(x, y);
  Edge e1 = _subdivision->Locate(p);
  Edge e2 = e1.getReverse();

  Face f1(e1);
  if (f1.overlaps(p))
    return f1;
  Face f2(e2);
  if (f2.overlaps(p))
    return f2;
  else
    throw Exception("Unable to find containing face.");
}

EdgeIterator DelaunayTriangulation::getEdgeIterator() const
{
  return EdgeIterator(_subdivision->getEdges());
}

const vector<Face>& DelaunayTriangulation::getFaces()
{
  if (_faces.empty())
  {
    for (auto fi = getFaceIterator(); fi != getFaceEnd(); ++fi)
      _faces.push_back(*fi);
  }
  return _faces;
}

FaceIterator DelaunayTriangulation::getFaceIterator() const
{
  return FaceIterator(getEdgeIterator(), getEdgeEnd());
}

Edge DelaunayTriangulation::getStartingEdge() const
{
  if (_pointCount < 3)
    throw Exception("You must add at least three points to create a triangulation.");
  return Edge(_subdivision->getStartingEdge());
}

void DelaunayTriangulation::insert(double x, double y)
{
  if (_pointCount < 3)
  {
    _x[_pointCount] = x;
    _y[_pointCount] = y;

    if (_pointCount == 2)
    {
      Point2d a(_x[0], _y[0]);
      Point2d b(_x[1], _y[1]);
      Point2d c(_x[2], _y[2]);

      _subdivision = new Subdivision(a, b, c);
    }
  }
  else
  {
    Point2d p(x, y);
    _subdivision->InsertSite(p);
  }
  _pointCount++;
}

}
