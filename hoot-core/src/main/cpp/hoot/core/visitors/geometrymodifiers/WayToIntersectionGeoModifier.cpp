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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "WayToIntersectionGeoModifier.h"

// Hoot
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveWayByEid.h>
#include <hoot/core/util/Factory.h>

// Standard
#include <algorithm>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(GeometryModifierAction, WayToIntersectionGeoModifier)

bool WayToIntersectionGeoModifier::processElement(const ElementPtr& pElement, OsmMap* pMap)
{
  // only process ways
  if (pElement->getElementType() != ElementType::Way)
    return false;
  const WayPtr pMyWay = std::dynamic_pointer_cast<Way>(pElement);

  // find envelope of nodes
  std::shared_ptr<Envelope> pEnv(pMyWay->getEnvelope(pMap->shared_from_this()));

  // find intersecting ways
  vector<long> intersectIds = pMap->getIndex().findWays(*pEnv);

  long myNodeCount = pMyWay->getNodeCount();
  const vector<long>& myNodeIds = pMyWay->getNodeIds();
  vector<IntersectionInfo> allIntersections;

  // find actual intersection of specific way segments
  for (long intersId : intersectIds)
  {
    WayPtr pIntersWay = pMap->getWay(intersId);

    long interNodeCount = pIntersWay->getNodeCount();
    const vector<long>& interNodeIds = pIntersWay->getNodeIds();

    for (int myNodeIx = 0; myNodeIx < myNodeCount - 1; ++myNodeIx)
    {
      long p1Id = myNodeIds[myNodeIx];
      long p2Id = myNodeIds[myNodeIx + 1];
      CoordinateExt myP1(pMap->getNode(p1Id)->toCoordinate());
      CoordinateExt myP2(pMap->getNode(p2Id)->toCoordinate());

      for (int interNodeIx = 0; interNodeIx < interNodeCount - 1; ++interNodeIx)
      {
        long i1Id = interNodeIds[interNodeIx];
        long i2Id = interNodeIds[interNodeIx + 1];

        // don't check if the segments share any nodes
        if (i1Id == p1Id || i1Id == p2Id || i2Id == p1Id || i2Id == p2Id)
          continue;

        CoordinateExt interP1(pMap->getNode(i1Id)->toCoordinate());
        CoordinateExt interP2(pMap->getNode(i2Id)->toCoordinate());

        std::shared_ptr<CoordinateExt> pIntersectionPoint = CoordinateExt::lineSegementsIntersect(myP1, myP2, interP1, interP2);

        if (pIntersectionPoint)
        {
          IntersectionInfo intersection = { *pIntersectionPoint, p1Id, p2Id };
          allIntersections.push_back(intersection);
        }
      }
    }
  }

  if (!allIntersections.empty())
    processIntersections(pMap, pMyWay, allIntersections);

  return true;
}

void WayToIntersectionGeoModifier::processIntersections(OsmMap* pMap, const WayPtr pWay, const vector<IntersectionInfo>& inters) const
{
  for (IntersectionInfo intersInfo : inters)
  {
    // create new node with tags from original way
    NodePtr pNode = std::make_shared<Node>(Status::Unknown1, pMap->createNextNodeId(), intersInfo.intersectionPoint);
    pNode->setTags(pWay->getTags());
    pMap->addNode(pNode);
  }

  // merge original node ids into an attached way if either end node is attached to another way
  const std::shared_ptr<NodeToWayMap>& n2w = pMap->getIndex().getNodeToWayMap();
  const vector<long>& nodesToAttach = pWay->getNodeIds();
  bool attached = assignToAdjacentWay(pMap, n2w, pWay->getId(), nodesToAttach);

  if (!attached)   // if not at the beginning...
  {
    //  Copy the node id vector so it can be reversed
    vector<long> reverseNodes = pWay->getNodeIds();
    reverse(reverseNodes.begin(), reverseNodes.end());
    attached = assignToAdjacentWay(pMap, n2w, pWay->getId(), reverseNodes); // ...try the end
  }

  // remove original way
  if (attached)
  {
    RemoveWayByEid removeOp(pWay->getId());
    OsmMapPtr mapPtr = pMap->shared_from_this();
    removeOp.apply(mapPtr);
  }
}

bool WayToIntersectionGeoModifier::assignToAdjacentWay(OsmMap* pMap, const std::shared_ptr<NodeToWayMap>& n2w, long myWayId, const vector<long>& nodesToAttach) const
{
  long nodeId = nodesToAttach[0];
  const set<long>& wayIds = n2w->getWaysByNode(nodeId);

  if (!wayIds.empty())
  {
    for (long wayId : wayIds)
    {
      if (myWayId != wayId)
      {
        const WayPtr pWay = pMap->getWay(wayId);
        const vector<long>& wayNodes = pWay->getNodeIds();

        if (wayNodes.front() == nodeId)
        {
          // insert ids at front
          for (size_t i = 1; i < nodesToAttach.size(); i++)
            pWay->insertNode(0, nodesToAttach[i]);
          return true;
        }
        else if (wayNodes.back() == nodeId)
        {
          // insert ids at the end
          for (size_t i = 1; i < nodesToAttach.size(); i++)
            pWay->addNode(nodesToAttach[i]);
          return true;
        }
      }
    }
  }
  return false;
}

}
