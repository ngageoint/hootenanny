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
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/geometry/GeometryToElementConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/util/StringUtils.h>

// GEOS
#include <geos/geom/GeometryFactory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

AlphaShapeGenerator::AlphaShapeGenerator(const double alpha, const double buffer)
  : _alpha(alpha),
    _buffer(buffer),
    _manuallyCoverSmallPointClusters(true)
{
  LOG_VART(_alpha);
  LOG_VART(_buffer);
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
  GeometryToElementConverter(result).convertGeometryToElement(cutterShape.get(), Status::Invalid, -1);
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

  // create a complex geometry representing the alpha shape

  std::shared_ptr<Geometry> cutterShape;

  AlphaShape alphaShape(_alpha);
  alphaShape.insert(points);
  try
  {
    cutterShape = alphaShape.toGeometry();
  }
  catch (const IllegalArgumentException& /*e*/)
  {
    LOG_DEBUG(
      "Failed to generate alpha shape geometry with alpha value of: " <<
      StringUtils::formatLargeNumber(_alpha) << ".");
  }
  if (!cutterShape)
  {
    cutterShape.reset(GeometryFactory::getDefaultInstance()->createEmptyGeometry());
  }
  cutterShape.reset(cutterShape->buffer(_buffer));

  // See _coverStragglers description. This is an add-on behavior that is separate from the Alpha
  // Shape algorithm itself.
  if (_manuallyCoverSmallPointClusters)
  {
    _coverStragglers(cutterShape, inputMap);
  }

  return cutterShape;
}

void AlphaShapeGenerator::_coverStragglers(std::shared_ptr<Geometry>& geometry,
                                           const ConstOsmMapPtr& map)
{
  LOG_DEBUG("Covering stragglers...");

  // Pretty simple...go through and find any point that wasn't covered by the Alpha Shape and draw
  // a buffer around it. This can get very slow for fairly large linear datasets. May need a new alg
  // here, if possible, for better performance.
  const NodeMap& nodes = map->getNodes();
  vector<GeometryPtr> stragglers;
  int processed = 0;
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    NodePtr node = it->second;
    GeometryPtr point(
      GeometryFactory::getDefaultInstance()->createPoint(
        geos::geom::Coordinate(node->getX(), node->getY())));
    if (!geometry->contains(point.get()))
    {
      LOG_TRACE(
        "Point " << point->toString() << " not covered by alpha shape. Buffering and adding it...");
      point.reset(point->buffer(_buffer));
      stragglers.push_back(point);
    }
    processed++;
  }

  LOG_VARD(processed);


  if (stragglers.size() > 0)
  {
    LOG_DEBUG("Adding " << stragglers.size() << " point stragglers to alpha shape.");
    //  Merge the stragglers geometries
    GeometryPtr merged = GeometryUtils::mergeGeometries(stragglers, *geometry->getEnvelopeInternal());
    //  Join the original geometry with the stragglers geometry
    geometry.reset(geometry->Union(merged.get()));
  }
  LOG_VART(geometry->getArea());
}

}
