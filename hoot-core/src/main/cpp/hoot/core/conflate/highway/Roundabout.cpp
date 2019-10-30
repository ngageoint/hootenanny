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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "Roundabout.h"
#include <hoot/core/algorithms/linearreference/LocationOfPoint.h>
#include <hoot/core/algorithms/splitter/WaySplitter.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/ops/RemoveWayByEid.h>
#include <hoot/core/ops/UnconnectedWaySnapper.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>
#include <hoot/core/elements/OsmUtils.h>

#include <geos/geom/Geometry.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/LineString.h>

using geos::geom::CoordinateSequence;

namespace hoot
{

typedef std::shared_ptr<geos::geom::Geometry> GeomPtr;

Roundabout::Roundabout()
  :  _status(Status::Invalid),
    _overrideStatus(false)
{
}

void Roundabout::setRoundaboutWay(WayPtr pWay)
{
  _roundaboutWay = pWay;
  _status = pWay->getStatus();

  if (_status == Status::Unknown1)
    _otherStatus = Status::Unknown2;
  else
    _otherStatus = Status::Unknown1;
}

void Roundabout::setRoundaboutCenter(NodePtr pNode)
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
  pNewNode->setTag(MetadataTags::HootSpecial(), MetadataTags::RoundaboutCenter());

  return pNewNode;
}

RoundaboutPtr Roundabout::makeRoundabout(const OsmMapPtr& pMap, WayPtr pWay)
{
  RoundaboutPtr rnd(new Roundabout());

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

  LOG_TRACE("Created roundabout: " << rnd->toDetailedString(pMap));
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

void Roundabout::handleCrossingWays(OsmMapPtr pMap)
{
  LOG_TRACE("Handling crossing ways...");

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
      GeomPtr pIntersect(pRndGeo->intersection(pWayGeo.get()));
      std::shared_ptr<CoordinateSequence> pCoords(pIntersect->getCoordinates());

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
              // Add the ways outside of the to connect back up if needed
              _connectingWays.push_back(newWays[j]);

              // Now make connector way
              WayPtr pWay(new Way(_otherStatus,
                                  pMap->createNextWayId(),
                                  15));
              pWay->addNode(pCenterNode->getId());
              pWay->setTag("highway", "unclassified");
              pWay->setTag(MetadataTags::HootSpecial(), MetadataTags::RoundaboutConnector());
              //  Also add in the connector ways to later remove
              LOG_TRACE("Adding temp way: " << pWay->getId() << "...");
              _tempWays.push_back(pWay);

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
          LOG_TRACE("Removing original way: " << pWay->getElementId() << "...");
          RemoveWayByEid::removeWayFully(pMap, pWay->getId());
          pMap->addNode(pCenterNode);
        }
      }
    }
  }
}

void Roundabout::removeRoundabout(OsmMapPtr pMap)
{
  /* Get all the nodes in the roundabout way.
   * Iterate through them, and see if they belong to another way.
   * If they do, keep them.
   *
   * Remove the roundabout from the map (way + leftover nodes)
   *
   * Iterate through the nodes that were kept, and connect them to
   * the centerpoint with temp ways.
   */

  //LOG_TRACE("Removing roundabout: " << _roundaboutWay->getElementId() << "...");
  LOG_TRACE("Removing roundabout: " << toDetailedString(pMap) << "...");

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
  LOG_VART(connectingNodeIDs.size());
  LOG_VART(extraNodeIDs.size());

  // Find our center coord...
  if (!_pCenterNode)
    _pCenterNode = getNewCenter(pMap);

  // Remove roundabout way & extra nodes
  LOG_TRACE("Removing roundabout way: " << _roundaboutWay->getId() << "...");
  RemoveWayByEid::removeWayFully(pMap, _roundaboutWay->getId());
  for (std::set<long>::iterator it = extraNodeIDs.begin(); it != extraNodeIDs.end(); ++it)
  {
    LOG_TRACE("Removing extra node with ID: " << *it << "...");
    // There may be something off with the map index, as I found situation where one of these extra
    // nodes was still in use. So, changed the removal to only if unused here.
    RemoveNodeByEid::removeNode(pMap, *it, true);
  }

  // Add center node
  LOG_VART(_pCenterNode);
  pMap->addNode(_pCenterNode);

  // Connect it up
  for (std::set<long>::iterator it = connectingNodeIDs.begin(); it != connectingNodeIDs.end(); ++it)
  {
    WayPtr pWay(new Way(_status, pMap->createNextWayId(), 15));
    pWay->addNode(_pCenterNode->getId());
    pWay->addNode(*it);
    LOG_VART(pWay->getNodeIds());
    pWay->setTag("highway", "unclassified");

    // Add special hoot tag
    pWay->setTag(MetadataTags::HootSpecial(), MetadataTags::RoundaboutConnector());

    pMap->addWay(pWay);
    _tempWays.push_back(pWay);
  }
  LOG_VART(_tempWays.size());
}

void Roundabout::replaceRoundabout(OsmMapPtr pMap)
{
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
   */

  //LOG_TRACE("Replacing roundabout: " << _roundaboutWay->getElementId() << "...");
  LOG_TRACE("Replacing roundabout: " << toDetailedString(pMap) << "...");

  // Re-add roundabout from the ref dataset or the secondary dataset if it has no match in the
  // reference
  if (_status == Status::Unknown1 || _overrideStatus)
  {
    std::vector<ConstNodePtr> wayNodes;
    for (size_t i = 0; i < _roundaboutNodes.size(); i++)
    {
      bool found = false;
      ConstNodePtr thisNode = _roundaboutNodes[i];
      long nodeId = thisNode->getId();
      LOG_VART(nodeId);
      if (pMap->getNodes().end() != pMap->getNodes().find(nodeId))
      {
        ConstNodePtr otherNode = pMap->getNodes().find(nodeId)->second;
        LOG_VART(otherNode->getId());

        // If nodes differ by more than circular error, add the node as new
        LOG_VART(thisNode->toCoordinate().distance(otherNode->toCoordinate()));
        LOG_VART(thisNode->getCircularError());
        if (thisNode->toCoordinate().distance(otherNode->toCoordinate()) >
            thisNode->getCircularError())
        {
          NodePtr pNewNode(new Node(*thisNode));
          pNewNode->setId(pMap->createNextNodeId());
          LOG_TRACE("Adding found node: " << pNewNode->getId() << "...");
          pMap->addNode(pNewNode);
          wayNodes.push_back(pNewNode);
          found = true;
        }
      }

      // If not found, we need to add it back to the map
      if (!found)
      {
        NodePtr pNewNode(new Node(*(_roundaboutNodes[i])));
        LOG_TRACE("Adding not found node: " << pNewNode->getId() << "...");
        pMap->addNode(pNewNode);
        wayNodes.push_back(pNewNode);
      }
    }

    // All our nodes should be there, now lets add the way back
    WayPtr pRoundabout(new Way(*_roundaboutWay));

    // Make sure our nodes are set correctly
    std::vector<long> nodeIds;
    for (size_t i = 0; i < wayNodes.size(); i++)
      nodeIds.push_back(wayNodes[i]->getId());
    pRoundabout->setNodes(nodeIds);
    LOG_VART(pRoundabout->getNodeIds());
    pMap->addWay(pRoundabout);

    OsmUtils::logElementDetail(pRoundabout, pMap);

    //  Convert the roundabout to a geometry for distance checking later
    ElementConverter converter(pMap);
    std::shared_ptr<geos::geom::Geometry> geometry = converter.convertToGeometry(pRoundabout);
    //  Check all of the connecting ways (if they exist) for an endpoint on or near the roundabout
    for (size_t i = 0; i < _connectingWays.size(); ++i)
    {
      WayPtr way = _connectingWays[i];
      bool foundValidWay = pMap->containsWay(way->getId());
      // If the way doesn't exist anymore check for ways with its ID as the parent ID before
      // ignoring it
      if (!foundValidWay)
      {
        //  Check the endpoints against the roundabout
        const std::vector<long> ids = way->getNodeIds();
        NodePtr node1 = pMap->getNode(ids[0]);
        NodePtr node2 = pMap->getNode(ids[ids.size() - 1]);
        //  Validate the endpoints
        if (!node1 || !node2)
          continue;
        std::shared_ptr<geos::geom::Geometry> ep1 = converter.convertToGeometry(ConstNodePtr(node1));
        std::shared_ptr<geos::geom::Geometry> ep2 = converter.convertToGeometry(ConstNodePtr(node2));
        //  Use the distance to find the right end to use
        NodePtr endpoint;
        if (geometry->distance(ep1.get()) < geometry->distance(ep2.get()))
          endpoint = node1;
        else
          endpoint = node2;
        // If the way doesn't exist anymore because of splitting, find the ways with the right
        // endpoint
        std::vector<long> waysWithNode =
          ElementIdsVisitor::findWaysByNode(pMap, endpoint->getId());
        if (waysWithNode.size() < 1)
          continue;

        //  Find the way that contains the correct node endpoint but isn't a 'hoot:special' node
        for (size_t index = 0; index < waysWithNode.size(); ++index)
        {
          WayPtr w = pMap->getWay(waysWithNode[index]);
          if (w && !w->getTags().contains(MetadataTags::HootSpecial()))
          {
            way = w;
            foundValidWay = true;
            break;
          }
        }
      }
      //  No valid way was found, ignore it
      if (!foundValidWay)
        continue;
      //  If the way only contains one node, ignore it
      const std::vector<long>& nodes = way->getNodeIds();
      if (nodes.size() < 2)
        continue;
      ConstNodePtr endpoint1 = pMap->getNode(nodes[0]);
      ConstNodePtr endpoint2 = pMap->getNode(nodes[nodes.size() - 1]);
      //  Validate the endpoints
      if (!endpoint1 || !endpoint2)
        continue;
      //  Check if either of the endpoints are already part of the roundabout
      if (pRoundabout->containsNodeId(endpoint1->getId()) ||
          pRoundabout->containsNodeId(endpoint2->getId()))
        continue;
      //  Snap the closest
      UnconnectedWaySnapper::snapClosestEndpointToWay(pMap, way, pRoundabout);
    }

    // Need to remove temp parts no matter what; delete temp ways we added
    for (size_t i = 0; i < _tempWays.size(); i++)
    {
      ConstWayPtr tempWay = _tempWays[i];
      LOG_TRACE("Removing temp way: " << tempWay->getElementId() << "...");
      RemoveWayByEid::removeWayFully(pMap, tempWay->getId());
    }

    // Remove center node if no other ways are using it
    if (pMap->getIndex().getNodeToWayMap()->getWaysByNode(_pCenterNode->getId()).size() < 1)
    {
      LOG_TRACE("Removing center node: " << _pCenterNode->getElementId() << "...");
      RemoveNodeByEid::removeNodeFully(pMap, _pCenterNode->getId());
    }
  }
}

QString Roundabout::toString() const
{
  return
    QString(
      "Way: %1, Status: %2, Other Status: %3, Original nodes size: %4, Current nodes size: %5: "
      "Center node: %6, Temp ways size: %7, Connecting ways size: %8, Override status: %9")
      .arg(_roundaboutWay->getId())
      .arg(_status.toString())
      .arg(_otherStatus.toString())
      .arg(QString::number(_roundaboutNodes.size()))
      .arg(QString::number(_roundaboutWay->getNodeIds().size()))
      .arg(_pCenterNode->getId())
      .arg(QString::number(_tempWays.size()))
      .arg(QString::number(_connectingWays.size()))
      .arg(_overrideStatus);
}

QString Roundabout::toDetailedString(OsmMapPtr map) const
{
  QString str = toString();

  str += ", Original nodes size: " + QString::number(_roundaboutNodes.size());
  if (_roundaboutWay)
  {
    str += ", Current nodes size: " + QString::number(_roundaboutWay->getNodeIds().size());
  }

  bool nodeIdsMatch = false;
  const std::vector<ConstNodePtr> originalNodes = _roundaboutNodes;
  const std::vector<long> originalNodeIds = OsmUtils::nodesToNodeIds(originalNodes);
  if (_roundaboutWay)
  {
    nodeIdsMatch = (originalNodeIds == _roundaboutWay->getNodeIds());
  }
  if (nodeIdsMatch)
  {
    str += ", original and current node IDs match";
  }
  else
  {
    str +=
      ", original and current node IDs do not match, original nodes: " + getOriginalNodesString();
    str += "; current nodes: " + getCurrentNodesString(map);
  }

  if (nodeIdsMatch)
  {
    const std::vector<ConstNodePtr> currentNodes =
      OsmUtils::nodeIdsToNodes(_roundaboutWay->getNodeIds(), map);
    if (OsmUtils::nodeCoordsMatch(originalNodes, currentNodes))
    {
      str += ", original and current node coordinates match.";
    }
    else
    {
      str +=
        ", original and current node coordinates do not match. original node coords: " +
        OsmUtils::nodeCoordsToString(originalNodes) + ", current node coords: " +
        OsmUtils::nodeCoordsToString(currentNodes);
    }
  }

  return str;
}

QString Roundabout::getOriginalNodesString() const
{
  QString str;
  for (size_t i = 0; i < _roundaboutNodes.size(); i++)
  {
    ConstNodePtr node = _roundaboutNodes[i];
    if (node)
    {
      str += QString::number(node->getId()) + ",";
    }
    else
    {
      str += "null node,";
    }
  }
  str.chop(1);
  return str;
}

QString Roundabout::getCurrentNodesString(OsmMapPtr map) const
{
  QString str;
  const std::vector<long> nodeIds = _roundaboutWay->getNodeIds();
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    ConstNodePtr node = map->getNode(nodeIds[i]);
    if (node)
    {
      str += QString::number(node->getId()) + ", ";
    }
    else
    {
      str += "ID: " + QString::number(nodeIds[i]) + " not found, ";
    }
  }
  str.chop(1);
  return str;
}

}
