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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "Roundabout.h"
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveWayOp.h>
#include <hoot/core/ops/RemoveNodeOp.h>
#include <hoot/core/util/ElementConverter.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/CoordinateSequence.h>

namespace hoot
{

typedef boost::shared_ptr<geos::geom::Geometry> GeomPtr;

Roundabout::Roundabout():
  _status(Status::Invalid)
{
  // Blank
}

void Roundabout::setRoundaboutWay (WayPtr pWay)
{
  _roundaboutWay = pWay;
  _status = pWay->getStatus();
}

NodePtr Roundabout::getCenter(OsmMapPtr pMap)
{
  double lat = 0;
  double lon = 0;
  double count = 0;

  for (size_t i = 0; i < _roundaboutNodes.size(); i++)
  {
    ConstNodePtr pNode = _roundaboutNodes[i];
    lon += pNode->getX();
    lat += pNode->getY();
  }

  count = _roundaboutNodes.size();
  lat = lat / count;
  lon = lon / count;

  NodePtr pNewNode(new Node(_status,
                   pMap->createNextNodeId(),
                   lon, lat, 15));
  pNewNode->setTag("hoot:special", "RoundaboutCenter");

  return pNewNode;
}

RoundaboutPtr Roundabout::makeRoundabout (const boost::shared_ptr<OsmMap> &pMap,
                                          WayPtr pWay)
{
  RoundaboutPtr rnd(new Roundabout);

  // Add the way to the roundabout
  rnd->setRoundaboutWay(pWay);

  // Get all the nodes from the way
  const std::vector<long> nodeIds = pWay->getNodeIds();
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    rnd->addRoundaboutNode(pMap->getNode(nodeIds[i]));
  }

  return rnd;
}

void Roundabout::connectCrossingWays(boost::shared_ptr<OsmMap> pMap)
{
  // TODO: Finish implementing this. The new nodes need to be inserted into the
  // proper location within their ways, and the chunk of way inside the
  // roundabout needs to be removed.
  return;

  // Get our envelope
  geos::geom::Envelope env = _roundaboutWay->getEnvelopeInternal(pMap);

  // Find intersecting ways
  std::vector<long> intersectIds = pMap->getIndex().findWays(env);

  // Find intersecting points
  ElementConverter converter(pMap);
  GeomPtr pRndGeo = converter.convertToGeometry(_roundaboutWay);
  for (size_t i = 0; i < intersectIds.size(); i++)
  {
    WayPtr pWay = pMap->getWay(intersectIds[i]);
    GeomPtr pWayGeo = converter.convertToGeometry(pWay);

    if (pRndGeo->intersects(pWayGeo.get()))
    {
      geos::geom::Geometry * pIntersect = pRndGeo->intersection(pWayGeo.get());
      geos::geom::CoordinateSequence *pCoords = pIntersect->getCoordinates();
      for (size_t j = 0; j < pCoords->getSize(); j++)
      {
        geos::geom::Coordinate coord = pCoords->getAt(j);

        // Make a node
        NodePtr pNode(new Node(_status, pMap->createNextNodeId(), coord, 15));
        pMap->addNode(pNode);

        // Add to both ways
        _roundaboutWay->addNode(pNode->getId());
        pWay->addNode(pNode->getId());
      }
    }
  }
}

/* Get all the nodes in the roundabout way.
 * Iterate through them, and see if they belong to another way.
 * If they do, keep them.
 *
 * Remove the roundabout from the map (way + leftover nodes)
 *
 * Iterate through the nodes that were kept, and connect them to
 * the centerpoint with temp ways.
 */
void Roundabout::removeRoundabout(boost::shared_ptr<OsmMap> pMap)
{
  // First, take care of ways that may cross the roundabout, but not connect
  connectCrossingWays(pMap);

  // Find our connecting nodes & extra nodes.
  std::set<long> connectingNodeIDs;
  std::set<long> extraNodeIDs;
  for (size_t i = 0; i < _roundaboutNodes.size(); i++)
  {
    long nodeId = _roundaboutNodes[i]->getId();
    if (pMap->getIndex().getNodeToWayMap()->getWaysByNode(nodeId).size() > 1)
    {
      connectingNodeIDs.insert(_roundaboutNodes[i]->getId());
    }
    else
    {
      extraNodeIDs.insert(_roundaboutNodes[i]->getId());
    }
  }

  // Find our center coord...
  _pCenterNode = getCenter(pMap);

  // Remove roundabout way & extra nodes
  RemoveWayOp::removeWayFully(pMap, _roundaboutWay->getId());
  for (std::set<long>::iterator it = extraNodeIDs.begin();
       it != extraNodeIDs.end(); ++it)
  {
    RemoveNodeOp::removeNode(pMap, *it);
  }

  // Add center node
  pMap->addNode(_pCenterNode);

  // Connect it up
  for (std::set<long>::iterator it = connectingNodeIDs.begin();
       it != connectingNodeIDs.end(); ++it)
  {
    WayPtr pWay(new Way(_status,
                        pMap->createNextWayId(),
                        15));
    pWay->addNode(_pCenterNode->getId());
    pWay->addNode(*it);
    pWay->setTag("highway", "unclassified");

    // Add special hoot tag
    pWay->setTag("hoot:special", "roundabout_connector");

    pMap->addWay(pWay);
    _tempWays.push_back(pWay);
  }
}


/*
 * Go through our nodes... if they are still there, check location.
 * If they are in the same place, fine. Otherwise, add nodes back as new.
 *
 * Then put the original way back. Modify the nodes it contains, to make
 * sure its correct
 *
 * See if center node is still there, if so, use it to get the ways that need
 * to connect to the roundabout.
 *
 * MAYBE: our roundabout nodes might need to be copies, so they don't get moved
 * around during conflation & merging
 *
 */
void Roundabout::replaceRoundabout(boost::shared_ptr<OsmMap> pMap)
{
  // Re-add roundabout from the ref dataset, but not secondary dataset
  if(_status == Status::Unknown1)
  {
    std::vector<ConstNodePtr> wayNodes;
    for (size_t i = 0; i < _roundaboutNodes.size(); i++)
    {
      bool found = false;
      ConstNodePtr thisNode = _roundaboutNodes[i];
      long nodeId = thisNode->getId();
      if (pMap->getNodes().end() != pMap->getNodes().find(nodeId))
      {
        ConstNodePtr otherNode = pMap->getNodes().find(nodeId)->second;

        // If nodes differ by more than circular error, add the node as new
        if (thisNode->toCoordinate().distance(otherNode->toCoordinate()) > thisNode->getCircularError())
        {
          NodePtr pNewNode(new Node(*thisNode));
          pNewNode->setId(pMap->createNextNodeId());
          pMap->addNode(pNewNode);
          wayNodes.push_back(pNewNode);
          found = true;
        }
      }

      // If not found, we need to add it back to the map
      if (!found)
      {
        NodePtr pNewNode(new Node(*(_roundaboutNodes[i])));
        pMap->addNode(pNewNode);
        wayNodes.push_back(pNewNode);
      }
    }

    // All our nodes should be there, now lets add the way back
    WayPtr pNewWay(new Way(*_roundaboutWay));

    // Make sure our nodes are set correctly
    std::vector<long> nodeIds;
    for (size_t i = 0; i < wayNodes.size(); i++)
      nodeIds.push_back(wayNodes[i]->getId());
    pNewWay->setNodes(nodeIds);
    pMap->addWay(pNewWay);
  }

  // Need to remove temp parts no matter what
  // Delete temp ways we added
  for (size_t i = 0; i < _tempWays.size(); i++)
    RemoveWayOp::removeWayFully(pMap, _tempWays[i]->getId());

  // Remove center node if no other ways are using it
  if (pMap->getIndex().getNodeToWayMap()->getWaysByNode(_pCenterNode->getId()).size() < 1)
    RemoveNodeOp::removeNodeFully(pMap, _pCenterNode->getId());
}

} // namespace
