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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "AlphaShape.h"

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/GeometryConverter.h>
#include <hoot/core/util/GeometryUtils.h>

// GEOS
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Polygon.h>
#include <geos/util/IllegalArgumentException.h>
using namespace geos::geom;

// Qt
#include <QString>

// Standard
#include <stdlib.h>
#include <limits>
using namespace std;

// TGS
#include <tgs/DelaunayTriangulation/DelaunayTriangulation.h>
#include <tgs/DisjointSet/DisjointSet.h>
#include <tgs/RStarTree/HilbertCurve.h>
#include <tgs/Statistics/Random.h>
using namespace Tgs;

namespace hoot
{

class FaceGroup
{
public:

  FaceGroup(int id)
  {
    _id = id;
  }

  void addChild(int child)
  {
    _children.insert(child);
  }

  int getId() { return _id; }

private:
  int _id;


  set<int> _children;
};

class ComparePolygon
{
public:

  ComparePolygon(Envelope e)
  {
    _curve.reset(new HilbertCurve(2, 8));
    _e = e;
    _size = (1 << 8) - 1;
  }

  ComparePolygon(const ComparePolygon& other)
  {
    _curve = other._curve;
    _e = other._e;
    _size = other._size;
  }

  bool operator()(const shared_ptr<Geometry>& p1, const shared_ptr<Geometry>& p2)
  {
    const Envelope* e1 = p1->getEnvelopeInternal();
    const Envelope* e2 = p2->getEnvelopeInternal();
    double x1 = (e1->getMinX() + e1->getMaxX()) / 2.0;
    double x2 = (e2->getMinX() + e2->getMaxX()) / 2.0;
    double y1 = (e1->getMinY() + e1->getMaxY()) / 2.0;
    double y2 = (e2->getMinY() + e2->getMaxY()) / 2.0;

    int i1[2], i2[2];
    i1[0] = (int)((x1 - _e.getMinX()) / (_e.getWidth()) * _size);
    i1[1] = (int)((y1 - _e.getMinY()) / (_e.getHeight()) * _size);
    i2[0] = (int)((x2 - _e.getMinX()) / (_e.getWidth()) * _size);
    i2[1] = (int)((y2 - _e.getMinY()) / (_e.getHeight()) * _size);

    bool result;
    int h1 = _curve->encode(i1);
    int h2 = _curve->encode(i2);
    if (h1 == h2)
    {
      result = p1 < p2;
    }
    else
    {
      result = h1 < h2;
    }

    return result;
  }

private:

  ComparePolygon();
  ComparePolygon& operator=(ComparePolygon& other);

  Envelope _e;
  shared_ptr<HilbertCurve> _curve;
  double _size;
};

inline double distance(double x1, double x2, double y1, double y2)
{
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

AlphaShape::AlphaShape(double alpha)
{
  _alpha = alpha;
  _dt.reset(new Tgs::DelaunayTriangulation);
}

shared_ptr<Way> AlphaShape::_addFaceAsWay(const Face* face, shared_ptr<OsmMap> map)
{
  Edge e = face->getEdge(0);
  e.getOriginX();
  shared_ptr<Node> start(new Node(Status::Unknown1, map->createNextNodeId(), e.getOriginX(), e.getOriginY(),
    -1));
  map->addNode(start);
//  shared_ptr<Way> way(new Way(Unknown1, map->createNextWayId(), -1));
    shared_ptr<Way> way;
//  way->addNode(start->getId());

  for (int i = 2; i < 6; i+=2)
  {
    e = face->getEdge(i);
    shared_ptr<Node> n(new Node(Status::Unknown1, map->createNextNodeId(), e.getOriginX(),
      e.getOriginY(), -1));
    map->addNode(n);
//    way->addNode(n->getId());
  }
//  way->addNode(start->getId());
//  map->addWay(way);

//  if (_isInside(*face))
//  {
//    way->setTag("area", "yes");
//  }

  return way;
}

shared_ptr<Polygon> AlphaShape::_convertFaceToPolygon(const Face& face) const
{
  shared_ptr<Polygon> result;
  CoordinateSequence* cs = GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->
                           create(4, 2);
  LinearRing* lr;

  Coordinate c;
  c.z = 0.0;
  Edge e;
  int p = 0;
  for (int j = 0; j < 6; j += 2)
  {
    e = face.getEdge(j);
    c.x = e.getOriginX();
    c.y = e.getOriginY();
    cs->setAt(c, p++);
  }
  e = face.getEdge(0);
  c.x = e.getOriginX();
  c.y = e.getOriginY();
  cs->setAt(c, p++);

  lr = GeometryFactory::getDefaultInstance()->createLinearRing(cs);
  std::vector<Geometry*>* holes = new std::vector<Geometry*>();
  result.reset(GeometryFactory::getDefaultInstance()->createPolygon(lr,
    holes));

  return result;
}

bool AlphaShape::_isBoundary(const Edge& e) const
{
  bool result = false;
  if (_isTooLong(e) == false)
  {
    Edge leftCcw = e.getLeftNext();
    Edge leftCw = e.getLeftPrevious();
    bool leftOpen = false;

    if (_isTooLong(leftCcw) || _isTooLong(leftCw))
    {
      leftOpen = true;
    }

    Edge rightCcw = e.getRightNext();
    Edge rightCw = e.getRightPrevious();
    bool rightOpen = false;

    if (_isTooLong(rightCcw) || _isTooLong(rightCw))
    {
      rightOpen = true;
    }

    if ((rightOpen == true || leftOpen == true) && leftOpen != rightOpen)
    {
      result = true;
    }
  }

  return result;
}

bool AlphaShape::_isInside(const Face& face) const
{
  bool result = true;
  for (int i = 0; i < 6; i++)
  {
    if (_isTooLong(face.getEdge(i)))
    {
      result = false;
    }
  }

  return result;
}

bool AlphaShape::_isOutsideEdge(const Tgs::Edge& e) const
{
  if (_outsidePoint.find(pair<double,double>(e.getOriginX(), e.getOriginY())) !=
        _outsidePoint.end() &&
      _outsidePoint.find(pair<double,double>(e.getDestinationX(), e.getDestinationY())) !=
        _outsidePoint.end())
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool AlphaShape::_isTooLong(const Edge& e) const
{
  return distance(e.getOriginX(), e.getDestinationX(), e.getOriginY(), e.getDestinationY()) >
      _alpha;
}

void AlphaShape::insert(const vector< pair<double, double> >& points)
{
  LOG_DEBUG("Inserting points.");

  double minX = numeric_limits<double>::max();
  double maxX = -minX;
  double minY = numeric_limits<double>::max();
  double maxY = -minY;

  for (size_t i = 0; i < points.size(); i++)
  {
    minX = min(points[i].first, minX);
    maxX = max(points[i].first, maxX);
    minY = min(points[i].second, minY);
    maxY = max(points[i].second, maxY);
  }

  double size = max(_alpha, max(maxX - minX, maxY - minY));
  double midX = (maxX + minX) / 2.0;
  double midY = (maxY + minY) / 2.0;

  vector< pair<double, double> > randomized;
  randomized.reserve(points.size() + 3);
  randomized.push_back(pair<double, double>(midX - size * 5, midY - size * 3));
  randomized.push_back(pair<double, double>(midX + size * 5, midY - size * 3));
  randomized.push_back(pair<double, double>(midX, midY + size * 5));
  _outsidePoint.insert(randomized[0]);
  _outsidePoint.insert(randomized[1]);
  _outsidePoint.insert(randomized[2]);

  for (size_t i = 0; i < points.size(); i++)
  {
    randomized.push_back(points[i]);
  }

  // randomize the input. By randomizing we take the complexity from O(n^2) to O(n lg(n))
  for (size_t i = 0; i < points.size() * 3; i++)
  {
    int i1 = Tgs::Random::instance()->generateInt(points.size()) + 3;
    int i2 = Tgs::Random::instance()->generateInt(points.size()) + 3;
    swap(randomized[i1], randomized[i2]);
  }

  for (size_t i = 0; i < randomized.size(); i++)
  {
    if (i % 10000 == 0 && Log::getInstance().getLevel() <= Log::Trace)
    {
      cout << "Progress: " << i << " of " << randomized.size() - 1 << "          \r";
      cout << flush;
    }
    _dt->insert(randomized[i].first, randomized[i].second);
  }
  if (Log::getInstance().getLevel() <= Log::Trace)
  {
    cout << endl;
  }
}

shared_ptr<OsmMap> AlphaShape::toOsmMap()
{
  shared_ptr<OsmMap> result(new OsmMap());

  GeometryConverter(result).convertGeometryToElement(toGeometry().get(), Status::Unknown1, -1);

  const RelationMap& rm = result->getRelationMap();
  for (RelationMap::const_iterator it = rm.begin(); it != rm.end(); ++it)
  {
    Relation* r = result->getRelation(it->first).get();
    r->setTag("area", "yes");
  }

  return result;
}

shared_ptr<Geometry> AlphaShape::toGeometry()
{
  LOG_DEBUG("Traversing faces");
  // create a vector of all faces
  vector< shared_ptr<Geometry> > tmp, tmp2;
  Envelope e;
  double preUnionArea = 0.0;
  int i = 0;
  for (FaceIterator fi = _dt->getFaceIterator(); fi != _dt->getFaceEnd(); fi++)
  {
    const Face& f = *fi;
    i++;
    if (_isInside(f))
    {
      shared_ptr<Polygon> p = _convertFaceToPolygon(f);
      tmp.push_back(p);
      e.expandToInclude(p->getEnvelopeInternal());
      preUnionArea += p->getArea();
    }
  }
  LOG_DEBUG("Area: " << (long)preUnionArea);

  // if the result is an empty geometry.
  if (tmp.size() == 0)
  {
    return shared_ptr<Geometry>(GeometryFactory::getDefaultInstance()->createEmptyGeometry());
  }

  LOG_DEBUG("Joining faces");
  // while there is more than one geometry.
  while (tmp.size() > 1)
  {
    LOG_DEBUG("Sorting size: " << tmp.size());
    // sort polygons using the hilbert value. This increases the chances that nearby polygons will
    // be merged early and speed up the union process.
    ComparePolygon compare(e);
    sort(tmp.begin(), tmp.end(), compare);

    LOG_DEBUG("Remaining pieces: " << tmp.size());
    tmp2.resize(0);
    tmp2.reserve(tmp.size() / 2 + 1);
    // merge pairs at a time. This makes the join faster.
    for (size_t i = 0; i < tmp.size() - 1; i += 2)
    {
      shared_ptr<Geometry> g;
      // sometimes GEOS gives results that are incorrect. In those cases we try cleaning the
      // geometries and attempting it again.
      bool cleanAndRetry = false;
      try
      {
        double area = tmp[i]->getArea() + tmp[i + 1]->getArea();
        g.reset(tmp[i]->Union(tmp[i + 1].get()));
        if (g->isEmpty() || fabs(g->getArea() - area) > 0.1)
        {
          cleanAndRetry = true;
        }
      }
      catch (geos::util::GEOSException& e)
      {
        LOG_DEBUG("Topology error. Attempting to fix it: " << e.what());
        cleanAndRetry = true;
      }

      if (cleanAndRetry)
      {
        tmp[i].reset(GeometryUtils::validateGeometry(tmp[i].get()));
        tmp[i + 1].reset(GeometryUtils::validateGeometry(tmp[i + 1].get()));
        try
        {
          g.reset(tmp[i]->Union(tmp[i + 1].get()));
        }
        // if the cleaning didn't fix the problem.
        catch(geos::util::GEOSException& e)
        {
          // report an error.
          QString error = "Error unioning two geometries. " + QString(e.what()) + "\n" +
              "geom1: " + QString::fromStdString(tmp[i]->toString()) + "\n" +
              "geom2: " + QString::fromStdString(tmp[i + 1]->toString());
          throw HootException(error);
        }
      }

      tmp2.push_back(g);
    }
    // if there are an odd number of entries, just add the last one
    if (tmp.size() % 2 == 1)
    {
      tmp2.push_back(tmp[tmp.size() - 1]);
    }

    tmp = tmp2;
  }

  shared_ptr<Geometry> result;

  LOG_DEBUG("Converting geometry to map.");
  if (tmp.size() == 1)
  {
    result = tmp[0];
  }
  else
  {
    result.reset(GeometryFactory::getDefaultInstance()->createEmptyGeometry());
  }

  result.reset(GeometryUtils::validateGeometry(result.get()));

  // I've never seen this happen when the cleaning step above is used, but a check can't hurt.
  // We still carry on with a warning even though the output may not be correct.
  if (fabs(preUnionArea - result->getArea()) > 1)
  {
    LOG_WARN("Area after union is inconsistent. GEOS error? pre union: " << (long)preUnionArea <<
      " post union: " << result->getArea());
  }

  return result;
}

QString AlphaShape::toString()
{
  QString result;
  Edge start = _dt->getStartingEdge();
  Edge e = start;

  do
  {
    result += QString::fromStdString(e.toString());
    e = e.getLeftNext();
  } while (e != start);

  return result;
}

}
