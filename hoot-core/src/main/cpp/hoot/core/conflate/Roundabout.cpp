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
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveWayOp.h>
#include <hoot/core/ops/RemoveNodeOp.h>
#include <hoot/core/util/ElementConverter.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/LineString.h>
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/algorithms/linearreference/LocationOfPoint.h>

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

  if (_status == Status::Unknown1)
    _otherStatus = Status::Unknown2;
  else
    _otherStatus = Status::Unknown1;
}

void Roundabout::setRoundaboutCenter (NodePtr pNode)
{
  _pCenterNode = pNode;
}

NodePtr Roundabout::getNewCenter(OsmMapPtr pMap)
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

  // Calculate and set center
  rnd->setRoundaboutCenter(rnd->getNewCenter(pMap));

  return rnd;
}

namespace // Anonymous
{
  geos::geom::Coordinate getCentroid(ConstOsmMapPtr pMap, WayPtr pWay)
  {
    geos::geom::Envelope env = pWay->getEnvelopeInternal(pMap);
    geos::geom::Coordinate center(0.0, 0.0);
    env.centre(center);
    return center;
  }
}

void Roundabout::handleCrossingWays(boost::shared_ptr<OsmMap> pMap)
{
  // Get a center point
  NodePtr pCenterNode = getNewCenter(pMap);
  pCenterNode->setStatus(_otherStatus);

  // Get our roundabout's envelope
  geos::geom::Envelope rndEnv = _roundaboutWay->getEnvelopeInternal(pMap);

  // Find ways that intersect the envelope
  std::vector<long> intersectIds = pMap->getIndex().findWays(rndEnv);

  // Calculate intersection points of crossing ways
  ElementConverter converter(pMap);
  GeomPtr pRndGeo = converter.convertToGeometry(_roundaboutWay);
  for (size_t i = 0; i < intersectIds.size(); i++)
  {
    WayPtr pWay = pMap->getWay(intersectIds[i]);

    if (pWay->getStatus() == _status)
      continue; // Bail if this is the ref data

    GeomPtr pWayGeo = converter.convertToGeometry(pWay);
    if (pRndGeo->intersects(pWayGeo.get()))
    {
      // Make list of waylocations
      std::vector<WayLocation> splitPoints;
      geos::geom::Geometry * pIntersect = pRndGeo->intersection(pWayGeo.get());
      geos::geom::CoordinateSequence *pCoords = pIntersect->getCoordinates();

      // We are only interested in ways that intersect the geometry once or
      // twice. More than that is situation we are not prepared to handle.
      size_t numIntersects = pCoords->getSize();
      if (numIntersects > 0 && numIntersects < 3)
      {
        for (size_t j = 0; j < pCoords->getSize(); j++)
        {
          // Get intersection
          geos::geom::Coordinate coord = pCoords->getAt(j);

          WayLocation w = LocationOfPoint::locate(pMap, pWay, coord);
          splitPoints.push_back(w);
        }

        // Sort way locations really quick
        std::sort(splitPoints.begin(), splitPoints.end());

        // Get split ways
        WaySplitter splitter(pMap, pWay);
        std::vector<WayPtr> newWays = splitter.createSplits(splitPoints);

        // Now what? Need to throw away the "interior" splits, and replace
        // with wheel spokes.
        bool replace = false;
        for (size_t j = 0; j < newWays.size(); j++)
        {
          if (newWays[j])
          {
            geos::geom::Coordinate midpoint = getCentroid(pMap, newWays[j]);

            // If the midpoint of the split way is outside of our roundabout
            // geometry, we want to keep it. Otherwise, let it disappear.
            if (!rndEnv.contains(midpoint))
            {
              pMap->addWay(newWays[j]);

              // Now make connector way
              WayPtr pWay(new Way(_otherStatus,
                                  pMap->createNextWayId(),
                                  15));
              pWay->addNode(pCenterNode->getId());
              pWay->setTag("highway", "unclassified");
              pWay->setTag("hoot:special", "roundabout_connector");

              // Take the new way. Whichever is closest, first node or last,
              // connect it to our center point.
              NodePtr pFirstNode = pMap->getNode(newWays[j]->getFirstNodeId());
              NodePtr pLastNode = pMap->getNode(newWays[j]->getLastNodeId());

              double firstD = pFirstNode->toCoordinate().distance(pCenterNode->toCoordinate());
              double lastD = pLastNode->toCoordinate().distance(pCenterNode->toCoordinate());

              // Connect to center node
              if (firstD < lastD)
              {
                pWay->addNode(pFirstNode->getId());
              }
              else
              {
                pWay->addNode(pLastNode->getId());
              }
              pMap->addWay(pWay);
              replace = true;
            }
          }
        }

        // Remove the original way if it's been split
        if (newWays.size() > 0 && replace)
        {
          // Remove pWay
          RemoveWayOp::removeWayFully(pMap, pWay->getId());
          pMap->addNode(pCenterNode);
        }
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
  _pCenterNode = getNewCenter(pMap);

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
