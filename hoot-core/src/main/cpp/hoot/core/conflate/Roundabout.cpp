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

Roundabout::Roundabout()
{
  // Blank
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

  NodePtr pNewNode(new Node(Status::Unknown1,
                   pMap->createNextNodeId(),
                   lon, lat, 15));
  pNewNode->setTag("hoot", "RoundaboutCenter");

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

// FIND WAYS THAT CROSS THE ROUNDABOUT AND INJECT AN INTERSECTION
void Roundabout::handleCrossingWays(boost::shared_ptr<OsmMap> pMap)
{
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
        NodePtr pNode(new Node(Status::Unknown1, pMap->createNextNodeId(), coord, 15));
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
 * Delete the roundabout (way + leftover nodes)
 *
 * Iterate through the nodes that were kept, and connect them to
 * the centerpoint with temp ways.
 *
 */
void Roundabout::removeRoundabout(boost::shared_ptr<OsmMap> pMap)
{
  // First, take care of ways that may cross the roundabout, but not connect
  handleCrossingWays(pMap);

  // Find our connecting nodes & extra nodes.
  std::vector<ConstNodePtr> connectingNodes;
  std::vector<ConstNodePtr> extraNodes;
  for (size_t i = 0; i < _roundaboutNodes.size(); i++)
  {
    long nodeId = _roundaboutNodes[i]->getId();
    if (pMap->getIndex().getNodeToWayMap()->getWaysByNode(nodeId).size() > 1)
    {
      connectingNodes.push_back(_roundaboutNodes[i]);
    }
    else
    {
      extraNodes.push_back(_roundaboutNodes[i]);
    }
  }

  // Find our center coord...
  _pCenterNode = getCenter(pMap);

  // Remove roundabout way & extra nodes
  RemoveWayOp::removeWayFully(pMap, _roundaboutWay->getId());
  for (size_t i = 0; i < extraNodes.size(); i++)
  {
    RemoveNodeOp::removeNode(pMap, extraNodes[i]->getId());
  }

  // Add center node
  pMap->addNode(_pCenterNode);

  // Connect it up
  for (size_t j = 0; j < connectingNodes.size(); j++)
  {
    WayPtr pWay(new Way(Status::Unknown1,
                        pMap->createNextWayId(),
                        15));
    pWay->addNode(_pCenterNode->getId());
    pWay->addNode(connectingNodes[j]->getId());
    pWay->setTag("highway", "unclassified");

    // Add special hoot tag
    pWay->setTag("hoot", "roundabout_connector");

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
 * If not - draw a circle, and find ways that intersect it?
 *
 * MAYBE TODO: our roundabout nodes might need to be copies, so they don't get moved around
 * during conflation & merging
 *
 */
void Roundabout::replaceRoundabout(boost::shared_ptr<OsmMap> pMap)
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

  // Now need to somehow reconnect our ways
  // Find where we cross our roundabout way
  // Maybe just start with the ways we added (the temp ways)

  // Delete temp ways we added
  for (size_t i = 0; i < _tempWays.size(); i++)
    RemoveWayOp::removeWayFully(pMap, _tempWays[i]->getId());

  // Remove center node if no other ways are using it
  // RemoveNodeOp::removeNode(pMap, extraNodes[i]->getId());
  if (pMap->getIndex().getNodeToWayMap()->getWaysByNode(_pCenterNode->getId()).size() < 1)
    RemoveNodeOp::removeNodeFully(pMap, _pCenterNode->getId());



}

}
