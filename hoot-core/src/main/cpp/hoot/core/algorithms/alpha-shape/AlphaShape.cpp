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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "AlphaShape.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/GeometryMerger.h>
#include <hoot/core/geometry/GeometryToElementConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>

// GEOS
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Point.h>
#include <geos/util/IllegalArgumentException.h>

// Standard
#include <stdlib.h>
#include <limits>
#include <queue>

// TGS
#include <tgs/DelaunayTriangulation/DelaunayTriangulation.h>
#include <tgs/DisjointSet/DisjointSet.h>
#include <tgs/RStarTree/HilbertCurve.h>
#include <tgs/Statistics/Random.h>

using namespace geos::geom;
using namespace std;
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

  int getId() const { return _id; }

private:

  int _id;
  set<int> _children;
};


AlphaShape::AlphaShape(double alpha)
  : _alpha(alpha),
    _longestFaceEdge(0.0),
    _sizeX(0.0),
    _sizeY(0.0)
{
  LOG_VART(_alpha);
  _pDelauneyTriangles = std::make_shared<Tgs::DelaunayTriangulation>();
}

GeometryPtr AlphaShape::_convertFaceToPolygon(const Face& face) const
{
  GeometryPtr result;
  CoordinateSequence* cs =
    GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->create(4, 2).release();
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
  std::vector<LinearRing*>* holes = new std::vector<LinearRing*>();
  // GeometryFactory takes ownership of these input parameters.
  result.reset(GeometryFactory::getDefaultInstance()->createPolygon(lr, holes));

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
  LOG_DEBUG("Inserting " << StringUtils::formatLargeNumber(points.size()) << " points...");

  double minX = numeric_limits<double>::max();
  double maxX = -minX;
  double minY = numeric_limits<double>::max();
  double maxY = -minY;

  for (const auto& point : points)
  {
    minX = min(point.first, minX);
    maxX = max(point.first, maxX);
    minY = min(point.second, minY);
    maxY = max(point.second, maxY);
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

  for (const auto& point : points)
    randomized.push_back(point);

  // By randomizing the input we take the complexity from O(n^2) to O(n lg(n)).
  int random_max = static_cast<int>(points.size());
  for (size_t i = 0; i < points.size() * 3; i++)
  {
    int i1 = Tgs::Random::instance()->generateInt(random_max) + 3;
    int i2 = Tgs::Random::instance()->generateInt(random_max) + 3;
    swap(randomized[i1], randomized[i2]);
  }
  for (size_t i = 0; i < randomized.size(); i++)
  {
    if (i % ConfigOptions().getTaskStatusUpdateInterval() == 0)
    {
      PROGRESS_INFO(
        "Added " << StringUtils::formatLargeNumber(i) << " of " <<
        StringUtils::formatLargeNumber(randomized.size() - 1) << " points.");
    }
    _pDelauneyTriangles->insert(randomized[i].first, randomized[i].second);
  }
  LOG_VARD(_pDelauneyTriangles->getFaces().size());
  //  Report the final progress
  PROGRESS_INFO(
    "Added " << StringUtils::formatLargeNumber(randomized.size() - 1) << " of " <<
    StringUtils::formatLargeNumber(randomized.size() - 1) << " points.");
}

OsmMapPtr AlphaShape::_toOsmMap()
{
  OsmMapPtr result = std::make_shared<OsmMap>();
  GeometryToElementConverter(result).convertGeometryToElement(toGeometry().get(), Status::Unknown1, -1);
  const RelationMap& rm = result->getRelations();
  for (auto it = rm.begin(); it != rm.end(); ++it)
  {
    Relation* r = result->getRelation(it->first).get();
    r->setTag("area", "yes");
  }
  return result;
}

double AlphaShape::_collectValidFaces(const double alpha, std::vector<GeometryPtr>& faces, Envelope& e) const
{
  double preUnionArea = 0.0;
  for (auto fi = _pDelauneyTriangles->getFaceIterator(); fi != _pDelauneyTriangles->getFaceEnd(); fi++)
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

bool AlphaShape::_searchAlpha(double &alpha, std::vector<GeometryPtr> &faces, Envelope &e, double& area, size_t face_count,
                              const std::vector<double> &alpha_options, size_t min_index, size_t max_index)
{
  bool found = false;
  std::vector<GeometryPtr> temp_faces;
  Envelope temp_env;
  if (max_index >= min_index)
  {
    size_t mid_index = min_index + (max_index - min_index) / 2;
    double temp_alpha = alpha_options[mid_index];
    temp_faces.clear();
    temp_env.init();
    double temp_area = _collectValidFaces(temp_alpha, temp_faces, temp_env);
    bool is_valid = static_cast<double>(face_count) * 0.9 <= static_cast<double>(temp_faces.size());

    if (is_valid)
    {
      //  Save the values found
      alpha = temp_alpha;
      faces = temp_faces;
      e = temp_env;
      area = temp_area;
      //  Find the minimal alpha so work the bottom half of the search space
      found = _searchAlpha(alpha, faces, e, area, face_count, alpha_options, min_index, mid_index - 1);
      //  Nothing below this alpha value was found, this is the one
      if (!found)
        return true;
    }
    else
    {
      //  Tested index wasn't valid so work the top half of the search space
      found = _searchAlpha(alpha, faces, e, area, face_count, alpha_options, mid_index + 1, max_index);
      //  Some alpha value above this index was found
      if (found)
        return true;
    }
  }
  return found;
}

GeometryPtr AlphaShape::toGeometry()
{
  LOG_DEBUG(
    "Traversing " << StringUtils::formatLargeNumber(_pDelauneyTriangles->getFaces().size()) <<
    " faces...");

  // create a vector of all faces
  vector<GeometryPtr> faces;
  Envelope e;
  double preUnionArea = 0.0;
  double alpha = -1.0;

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
    for (auto it = _pDelauneyTriangles->getEdgeIterator(); it != _pDelauneyTriangles->getEdgeEnd(); ++it)
    {
      //  Multiply by scale and convert it to a whole number for deduplication in the set
      long length = static_cast<long>((*it).getLength() * scale);
      if (length > 0 && length > static_cast<long>(_alpha))
        alpha_values.insert(length);
    }
    //  Get a list of possible alpha values to test
    for (auto value : alpha_values)
      alpha_options.push_back(static_cast<double>(value) / scale);
    //  Iterate the alpha values searching for one that uses at least
    //  90% of the Delauney triangle faces
    bool success = _searchAlpha(alpha, faces, e, preUnionArea, faceCount, alpha_options, 0, alpha_options.size() - 1);
    LOG_VARD(e);
    LOG_DEBUG("Area: " << (long)preUnionArea);
    LOG_VARD(faces.size());

    // if the result is an empty geometry
    if (faces.empty() || !success)
      throw IllegalArgumentException("Unable to find alpha value to create alpha shape.");
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
    while (faces.size() < 1 && !alpha_options.empty())
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
    if (faces.empty())
    {
      throw IllegalArgumentException(
        QString("Unable to create alpha shape with alpha value of: %1.").arg(QString::number(_alpha)));
    }
  }

  LOG_DEBUG("Joining " << StringUtils::formatLargeNumber(faces.size()) << " faces...");
  GeometryPtr result = GeometryMerger().mergeGeometries(faces, e);

  LOG_DEBUG("Creating output geometry...");

  if (!result || result->isEmpty())
    result = GeometryFactory::getDefaultInstance()->createEmptyGeometry();

  LOG_DEBUG("Validating output geometry...");
  //  Validate the resulting geometry
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

}
