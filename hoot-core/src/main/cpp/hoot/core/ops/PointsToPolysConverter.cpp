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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PointsToPolysConverter.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/elements/OsmUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, PointsToPolysConverter)

PointsToPolysConverter::PointsToPolysConverter(const double bufferSize) :
_bufferSize(bufferSize)
{
}

void PointsToPolysConverter::apply(OsmMapPtr& map)
{
  _numAffected = 0;
  _numProcessed = 0;
  _nodeIdsConverted.clear();

  MapProjector::projectToPlanar(map);

  LOG_DEBUG("Adding polys...");
  // Make a copy of the nodes here, so as we add in polys with nodes, we don't process those as
  // well.
  NodeMap nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    const long nodeId = it->first;
    LOG_VART(nodeId);
    // not allowing child nodes to be converted (could/should we?)
    LOG_VART(OsmUtils::isChild(ElementId(ElementType::Node, nodeId), map));
    if (!OsmUtils::isChild(ElementId(ElementType::Node, nodeId), map))
    {
      _addPolyFromPoint(it->second, map);
      _numAffected++;
    }
    _numProcessed++;
  }

  LOG_DEBUG("Removing points...");
  LOG_VART(_nodeIdsConverted.size());
  for (QList<long>::const_iterator it = _nodeIdsConverted.begin(); it != _nodeIdsConverted.end();
       ++it)
  {
    RemoveNodeByEid::removeNodeFully(map, *it);
  }
}

void PointsToPolysConverter::_addPolyFromPoint(const ConstNodePtr& point, const OsmMapPtr& map)
{
  // TODO: any way to consolidate some of this with  GeometryUtils::createMapFromBounds?

  geos::geom::Envelope polyBounds(point->toCoordinate());
  polyBounds.expandBy(_bufferSize);

  NodePtr lowerLeft(
    new Node(
      point->getStatus(),
      map->createNextNodeId(),
      geos::geom::Coordinate(polyBounds.getMinX(), polyBounds.getMinY())));
  map->addNode(lowerLeft);
  NodePtr upperLeft(
    new Node(
      point->getStatus(),
      map->createNextNodeId(),
      geos::geom::Coordinate(polyBounds.getMinX(), polyBounds.getMaxY())));
  map->addNode(upperLeft);
  NodePtr upperRight(
    new Node(
      point->getStatus(),
      map->createNextNodeId(),
      geos::geom::Coordinate(polyBounds.getMaxX(), polyBounds.getMaxY())));
  map->addNode(upperRight);
  NodePtr lowerRight(
    new Node(
      point->getStatus(),
      map->createNextNodeId(),
      geos::geom::Coordinate(polyBounds.getMaxX(), polyBounds.getMinY())));
  map->addNode(lowerRight);
  _nodeIdsConverted.append(point->getId());

  WayPtr poly(new Way(point->getStatus(), map->createNextWayId()));
  poly->addNode(lowerLeft->getId());
  poly->addNode(upperLeft->getId());
  poly->addNode(upperRight->getId());
  poly->addNode(lowerRight->getId());
  poly->addNode(lowerLeft->getId());
  LOG_VART(poly);

  map->addWay(poly);
}

}
