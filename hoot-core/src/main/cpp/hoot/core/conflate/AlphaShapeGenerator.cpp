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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "AlphaShapeGenerator.h"

// Hoot
#include <hoot/core/algorithms/AlphaShape.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/util/GeometryConverter.h>

namespace hoot
{

AlphaShapeGenerator::AlphaShapeGenerator(const double alpha, const double buffer) :
_alpha(alpha),
_buffer(buffer)
{
}

OsmMapPtr AlphaShapeGenerator::generateMap(OsmMapPtr inputMap)
{
  shared_ptr<Geometry> cutterShape = generateGeometry(inputMap);
  if (cutterShape->getArea() == 0.0)
  {
    LOG_WARN("Alpha Shape area is zero. Try increasing the buffer size and/or alpha.");
  }

  shared_ptr<OsmMap> result;

  result.reset(new OsmMap(inputMap->getProjection()));
  // add the resulting alpha shape for debugging.
  GeometryConverter(result).convertGeometryToElement(cutterShape.get(), Status::Invalid, -1);

  const RelationMap& rm = result->getRelationMap();
  for (RelationMap::const_iterator it = rm.begin(); it != rm.end(); ++it)
  {
    Relation* r = result->getRelation(it->first).get();
    r->setTag("area", "yes");
  }

  return result;
}

shared_ptr<Geometry> AlphaShapeGenerator::generateGeometry(OsmMapPtr inputMap)
{
  MapProjector::projectToPlanar(inputMap);

  // put all the nodes into a vector of points.
  std::vector< std::pair<double, double> > points;
  points.reserve(inputMap->getNodeMap().size());
  const NodeMap& nodes = inputMap->getNodeMap();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    pair<double, double> p;
    p.first = (it->second)->getX();
    p.second = (it->second)->getY();
    points.push_back(p);
  }

  // create a complex geometry representing the alpha shape
  shared_ptr<Geometry> cutterShape;
  {
    AlphaShape alphaShape(_alpha);
    alphaShape.insert(points);
    cutterShape = alphaShape.toGeometry();
    cutterShape.reset(cutterShape->buffer(_buffer));
  }

  return cutterShape;
}

}
