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

#include "AlphaShapeGenerator.h"

// Hoot
#include <hoot/core/algorithms/alpha-shape/AlphaShape.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/GeometryConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/StringUtils.h>

// GEOS
#include <geos/geom/GeometryFactory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

AlphaShapeGenerator::AlphaShapeGenerator(const double alpha, const double buffer) :
_alpha(alpha),
_buffer(buffer),
_retryOnTooSmallInitialAlpha(true),
_maxTries(1)
{
  LOG_VART(_alpha);
  LOG_VART(_buffer);
  if (_retryOnTooSmallInitialAlpha)
  {
    // have not seen an instance yet where more than two tries are needed to complete successfully;
    // in all other cases, no solution could be found no matter how many tries were attempted
    _maxTries = 2;
  }
}

OsmMapPtr AlphaShapeGenerator::generateMap(OsmMapPtr inputMap)
{
  OsmMapWriterFactory::writeDebugMap(inputMap, "alpha-shape-input-map");

  std::shared_ptr<Geometry> cutterShape = generateGeometry(inputMap);
  if (cutterShape->getArea() == 0.0)
  {
    // would rather this be thrown than a warning logged, as the warning may go unoticed by
    // clients who are expecting the alpha shape to be generated
    throw HootException("Alpha Shape area is zero. Try increasing the buffer size and/or alpha.");
  }
  OsmMapWriterFactory::writeDebugMap(cutterShape, inputMap->getProjection(), "cutter-shape-map");

  OsmMapPtr result;
  result.reset(new OsmMap(inputMap->getProjection()));
  result->appendSource(inputMap->getSource());
  // add the resulting alpha shape for debugging.
  GeometryConverter(result).convertGeometryToElement(cutterShape.get(), Status::Invalid, -1);
  OsmMapWriterFactory::writeDebugMap(result, "alpha-shape-result-map");

  const RelationMap& rm = result->getRelations();
  for (RelationMap::const_iterator it = rm.begin(); it != rm.end(); ++it)
  {
    Relation* r = result->getRelation(it->first).get();
    r->setTag("area", "yes");
  }

  LOG_VART(MapProjector::toWkt(result->getProjection()));
  OsmMapWriterFactory::writeDebugMap(result, "alpha-shape-result-map");

  return result;
}

std::shared_ptr<Geometry> AlphaShapeGenerator::generateGeometry(OsmMapPtr inputMap)
{
  MapProjector::projectToPlanar(inputMap);
  LOG_VART(MapProjector::toWkt(inputMap->getProjection()));

  // put all the nodes into a vector of points
  std::vector<std::pair<double, double>> points;
  points.reserve(inputMap->getNodes().size());
  const NodeMap& nodes = inputMap->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    pair<double, double> p;
    p.first = (it->second)->getX();
    p.second = (it->second)->getY();
    points.push_back(p);
  }
  LOG_VART(points.size());
  _inputPoints = points;

  // create a complex geometry representing the alpha shape

  std::shared_ptr<Geometry> cutterShape;

  int numTries = 0;
  while (numTries < _maxTries)
  {
    LOG_DEBUG(
      "Generating alpha shape geometry with alpha: " << _alpha << "; attempt " << (numTries + 1) <<
      " / " << _maxTries << "...");

    AlphaShape alphaShape(_alpha);
    alphaShape.insert(points);
    try
    {
      cutterShape = alphaShape.toGeometry();
    }
    catch (const IllegalArgumentException& e)
    {
      if (_retryOnTooSmallInitialAlpha && e.getWhat().startsWith("Longest face edge of size"))
      {
        const double longestFaceEdge = alphaShape.getLongestFaceEdge();
        LOG_INFO(
          "Failed to generate alpha shape geometry with alpha value of: " <<
          StringUtils::formatLargeNumber(_alpha) <<
          ". Attempting to generate the shape again using the longest face edge size of: " <<
          StringUtils::formatLargeNumber(longestFaceEdge) << " for alpha...");
        _alpha = longestFaceEdge;
      }
      else
      {
        break;
      }
    }
    numTries++;
  }
  if (!cutterShape)
  {
    cutterShape.reset(GeometryFactory::getDefaultInstance()->createEmptyGeometry());
  }
  cutterShape.reset(cutterShape->buffer(_buffer));
  //OsmMapWriterFactory::writeDebugMap(cutterShape, inputMap->getProjection(), "cutter-shape-map");

  // TODO
  _coverStragglers(cutterShape);
  //OsmMapWriterFactory::writeDebugMap(geometry, spatRef, "alpha-shape-after-covering-stragglers");

  return cutterShape;
}

void AlphaShapeGenerator::_coverStragglers(std::shared_ptr<Geometry>& geometry)
{
  LOG_DEBUG("Covering stragglers...");

  int addedPointCtr = 0;
  LOG_VARD(_inputPoints.size());
  for (std::vector<std::pair<double, double>>::const_iterator itr = _inputPoints.begin();
       itr != _inputPoints.end(); ++itr)
  {
    std::pair<double, double> rawPoint = *itr;
    LOG_VART(rawPoint.first);
    LOG_VART(rawPoint.second);
    std::shared_ptr<geos::geom::Geometry> point(
      GeometryFactory::getDefaultInstance()->createPoint(
        geos::geom::Coordinate(rawPoint.first, rawPoint.second)));
    if (!geometry->touches(point.get()) && !geometry->contains(point.get()))
    {
      LOG_TRACE(
        "Point " << point->toString() << " not covered by alpha shape. Buffering and adding it...");
      point.reset(point->buffer(_buffer));
      geometry.reset(geometry->Union(point.get()));
      addedPointCtr++;
      LOG_VART(geometry->getArea());
    }
  }

  LOG_DEBUG("Added " << addedPointCtr << " point stragglers to alpha shape.");
}

}
