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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "AlphaShape.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/GeometryConverter.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/StringUtils.h>

// GEOS
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/util/IllegalArgumentException.h>
#include <geos/geom/Point.h>
using namespace geos::geom;

// Qt
#include <QString>

// Standard
#include <stdlib.h>
#include <limits>
#include <queue>
using namespace std;

// TGS
#include <tgs/DelaunayTriangulation/DelaunayTriangulation.h>
#include <tgs/DisjointSet/DisjointSet.h>
#include <tgs/RStarTree/HilbertCurve.h>
#include <tgs/Statistics/Random.h>
using namespace Tgs;

namespace hoot
{

int AlphaShape::logWarnCount = 0;

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

  bool operator()(const GeometryPtr& p1, const GeometryPtr& p2)
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
  std::shared_ptr<HilbertCurve> _curve;
  double _size;
};

AlphaShape::AlphaShape(double alpha)
  : _alpha(alpha),
    _longestFaceEdge(0.0),
    _sizeX(0.0),
    _sizeY(0.0)
{
  LOG_VARD(_alpha);
  _pDelauneyTriangles.reset(new Tgs::DelaunayTriangulation);
}

GeometryPtr AlphaShape::_convertFaceToPolygon(const Face& face) const
{
  GeometryPtr result;
  CoordinateSequence* cs =
    GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->create(4, 2);
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
  result.reset(GeometryFactory::getDefaultInstance()->createPolygon(lr, holes));

  return result;
}

bool AlphaShape::_isBoundary(const double alpha, const Edge& e) const
{
  bool result = false;
  if (_isTooLong(alpha, e) == false)
  {
    Edge leftCcw = e.getLeftNext();
    Edge leftCw = e.getLeftPrevious();
    bool leftOpen = false;

    if (_isTooLong(alpha, leftCcw) || _isTooLong(alpha, leftCw))
    {
      leftOpen = true;
    }

    Edge rightCcw = e.getRightNext();
    Edge rightCw = e.getRightPrevious();
    bool rightOpen = false;

    if (_isTooLong(alpha, rightCcw) || _isTooLong(alpha, rightCw))
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

bool AlphaShape::_isInside(const double alpha, const Face& face) const
{
  bool result = true;
  for (int i = 0; i < 6; i++)
  {
    const bool edgeTooLong = _isTooLong(alpha, face.getEdge(i));
    LOG_VART(edgeTooLong);
    if (edgeTooLong)
      result = false;
  }
  return result;
}

bool AlphaShape::_isTooLong(const double alpha, const Edge& e) const
{
  const double edgeDistance = e.getLength();
  LOG_VART(edgeDistance);
  if (edgeDistance > _longestFaceEdge)
    _longestFaceEdge = edgeDistance;
  return edgeDistance > alpha;
}

void AlphaShape::insert(const vector<pair<double, double>>& points)
{
  LOG_DEBUG("Inserting " << points.size() << " points...");

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

  _sizeX = maxX - minX;
  _sizeY = maxY - minY;
  double size = max(_alpha, max(maxX - minX, maxY - minY));
  double midX = (maxX + minX) / 2.0;
  double midY = (maxY + minY) / 2.0;

  vector<pair<double, double>> randomized;
  randomized.reserve(points.size() + 3);
  randomized.push_back(pair<double, double>(midX - size * 5, midY - size * 3));
  randomized.push_back(pair<double, double>(midX + size * 5, midY - size * 3));
  randomized.push_back(pair<double, double>(midX, midY + size * 5));
  _outsidePoint.insert(randomized[0]);
  _outsidePoint.insert(randomized[1]);
  _outsidePoint.insert(randomized[2]);

  for (size_t i = 0; i < points.size(); i++)
    randomized.push_back(points[i]);

  // By randomizing the input we take the complexity from O(n^2) to O(n lg(n)).
  for (size_t i = 0; i < points.size() * 3; i++)
  {
    int i1 = Tgs::Random::instance()->generateInt(points.size()) + 3;
    int i2 = Tgs::Random::instance()->generateInt(points.size()) + 3;
    swap(randomized[i1], randomized[i2]);
  }
  for (size_t i = 0; i < randomized.size(); i++)
  {
    if (i % 10000 == 0)
    {
      PROGRESS_TRACE("Progress: " << i << " of " << randomized.size() - 1 << "          ");
    }
    _pDelauneyTriangles->insert(randomized[i].first, randomized[i].second);
  }
  LOG_TRACE("Progress: " << randomized.size() - 1 << " of " << randomized.size() - 1 << "          ");
  LOG_VARD(_pDelauneyTriangles->getFaces().size());
}

OsmMapPtr AlphaShape::_toOsmMap()
{
  OsmMapPtr result(new OsmMap());
  GeometryConverter(result).convertGeometryToElement(toGeometry().get(), Status::Unknown1, -1);
  const RelationMap& rm = result->getRelations();
  for (RelationMap::const_iterator it = rm.begin(); it != rm.end(); ++it)
  {
    Relation* r = result->getRelation(it->first).get();
    r->setTag("area", "yes");
  }
  return result;
}

double AlphaShape::_collectValidFaces(const double alpha, std::vector<GeometryPtr>& faces, Envelope& e)
{
  double preUnionArea = 0.0;
  for (FaceIterator fi = _pDelauneyTriangles->getFaceIterator();
       fi != _pDelauneyTriangles->getFaceEnd(); fi++)
  {
    const Face& f = *fi;
    if (_isInside(alpha, f))
    {
      GeometryPtr p = _convertFaceToPolygon(f);
      LOG_VART(p->getArea());
      faces.push_back(p);
      e.expandToInclude(p->getEnvelopeInternal());
      preUnionArea += p->getArea();
    }
  }
  return preUnionArea;
}

GeometryPtr AlphaShape::toGeometry()
{
  LOG_DEBUG("Traversing " << _pDelauneyTriangles->getFaces().size() << " faces...");

  // create a vector of all faces
  vector<GeometryPtr> faces, temp;
  Envelope e;
  double preUnionArea = 0.0;
  double alpha = -1;

  bool calculateAlpha = _alpha < 0.0;
  std::vector<double> alpha_options;

  if (calculateAlpha)
  {
    //  Get the list of all possible alpha values (edge lengths) if alpha needs to be calculated
    std::set<long> alpha_values;
    size_t faceCount = _pDelauneyTriangles->getFaces().size();
    //  Whole number scale for deduplication
    double scale = 1.0;
    if (_sizeX < 10 || _sizeY < 10)
      scale = 100.0;
    else if (_sizeX < 100 || _sizeY < 100)
      scale = 10.0;
    //  Iterate all of the edges to get the alpha value candidates
    for (EdgeIterator it = _pDelauneyTriangles->getEdgeIterator(); it != _pDelauneyTriangles->getEdgeEnd(); ++it)
    {
      //  Multiply by scale and convert it to a whole number for deduplication in the set
      long length = (*it).getLength() * scale;
      if (length > 0 && length > _alpha)
        alpha_values.insert(length);
    }
    //  Get a list of possible alpha values to test
    for (std::set<long>::iterator it = alpha_values.begin(); it != alpha_values.end(); ++it)
      alpha_options.push_back((*it) / scale);
    //  Iterate the alpha values searching for one that uses at least
    //  90% of the Delauney triangle faces
    while (alpha_options.size() > 0 && faceCount * 0.9 >= faces.size())
    {
      //  Clear out any previous faces that may exist
      faces.clear();
      e.init();
      //  Get the next alpha to try
      alpha = alpha_options[0];
      alpha_options.erase(alpha_options.begin());
      preUnionArea = _collectValidFaces(alpha, faces, e);
    }
    LOG_VARD(e);
    LOG_DEBUG("Area: " << (long)preUnionArea);
    LOG_VARD(faces.size());

    // if the result is an empty geometry
    if (faces.size() == 0)
    {
      throw IllegalArgumentException(
        "Unable to find alpha value to create alpha shape.");
    }
    //  Choose the alpha value
    LOG_INFO("No alpha value supplied; generated alpha value: " << alpha);
    _alpha = alpha;
  }
  else
  {
    //  Only check two alpha values, the given alpha and the longest face edge
    alpha_options.push_back(_alpha);
    alpha_options.push_back(getLongestFaceEdge());
    //  Iterate both options for the alpha value
    while (faces.size() < 1 && alpha_options.size() > 0)
    {
      //  Clear out any previous faces that may exist
      faces.clear();
      e.init();
      //  Get the next alpha to try
      alpha = alpha_options[0];
      alpha_options.erase(alpha_options.begin());
      preUnionArea = _collectValidFaces(alpha, faces, e);
    }
    LOG_VARD(e);
    LOG_DEBUG("Area: " << (long)preUnionArea);
    LOG_VARD(faces.size());

    // if the result is an empty geometry
    if (faces.size() == 0)
    {
      throw IllegalArgumentException(
        "Unable to create alpha shape with alpha value of: " + QString::number(_alpha) + ".");
    }
  }

  LOG_DEBUG("Joining " << StringUtils::formatLargeNumber(faces.size()) << " faces...");

  // while there is more than one geometry
  while (faces.size() > 1)
  {
    LOG_TRACE("Sorting size: " << faces.size());
    // Sort polygons using the Hilbert value. This increases the chances that nearby polygons will
    // be merged early and speed up the union process.
    ComparePolygon compare(e);
    sort(faces.begin(), faces.end(), compare);

    LOG_TRACE("Remaining pieces: " << faces.size());
    temp.resize(0);
    temp.reserve(faces.size() / 2 + 1);
    // Merge pairs at a time. This makes the join faster.
    for (size_t i = 0; i < faces.size() - 1; i += 2)
    {
      GeometryPtr g;
      // Sometimes GEOS gives results that are incorrect. In those cases we try cleaning the
      // geometries and attempting it again.
      bool cleanAndRetry = false;
      try
      {
        double area = faces[i]->getArea() + faces[i + 1]->getArea();
        g.reset(faces[i]->Union(faces[i + 1].get()));
        if (g->isEmpty() || fabs(g->getArea() - area) > 0.1)
        {
          cleanAndRetry = true;
        }
      }
      catch (const geos::util::GEOSException& e)
      {
        LOG_TRACE("Topology error. Attempting to fix it: " << e.what());
        cleanAndRetry = true;
      }

      if (cleanAndRetry)
      {
        faces[i].reset(GeometryUtils::validateGeometry(faces[i].get()));
        faces[i + 1].reset(GeometryUtils::validateGeometry(faces[i + 1].get()));
        try
        {
          g.reset(faces[i]->Union(faces[i + 1].get()));
        }
        // if the cleaning didn't fix the problem
        catch(const geos::util::GEOSException& e)
        {
          // report an error
          QString error = "Error unioning two geometries. " + QString(e.what()) + "\n" +
              "geom1: " + QString::fromStdString(faces[i]->toString()) + "\n" +
              "geom2: " + QString::fromStdString(faces[i + 1]->toString());
          throw HootException(error);
        }
      }

      temp.push_back(g);
    }
    // If there are an odd number of entries, just add the last one.
    if (faces.size() % 2 == 1)
    {
      temp.push_back(faces[faces.size() - 1]);
    }

    faces = temp;
  }

  LOG_DEBUG("Creating output geometry...");

  GeometryPtr result;
  if (faces.size() == 1)
  {
    result = faces[0];
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
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN(
        "Area after union is inconsistent. GEOS error? pre union: " << (long)preUnionArea <<
        " post union: " << result->getArea());
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }

  return result;
}

QString AlphaShape::toString()
{
  QString result;
  Edge start = _pDelauneyTriangles->getStartingEdge();
  Edge e = start;

  do
  {
    result += QString::fromStdString(e.toString());
    e = e.getLeftNext();
  } while (e != start);

  return result;
}

}
