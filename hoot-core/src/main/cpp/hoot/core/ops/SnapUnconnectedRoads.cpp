
#include "SnapUnconnectedRoads.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/visitors/IndexElementsVisitor.h>
//#include <hoot/core/criterion/HighwayNodeCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/algorithms/WayDiscretizer.h>
#include <hoot/core/algorithms/Distance.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>
#include <tgs/RStarTree/HilbertRTree.h>

// GEOS
#include <geos/geom/Coordinate.h>

#include <float.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, SnapUnconnectedRoads)

SnapUnconnectedRoads::SnapUnconnectedRoads()
{
}

Meters SnapUnconnectedRoads::_getSearchRadius(const boost::shared_ptr<const Element>& /*e*/) const
{
  return /*e->getCircularError()*/5.0;  // TODO: make configurable
}

void SnapUnconnectedRoads::apply(OsmMapPtr& map)
{  
  _numAffected = 0;

  const WayMap ways = map->getWays();   //copy
  LOG_VARD(ways.size());
  boost::shared_ptr<NodeToWayMap> nodeToWayMap = map->getIndex().getNodeToWayMap(); //copy
  LOG_VARD(nodeToWayMap->size());

  // TODO: tune these indexes? - see #3054
  // TODO: make these index creations into a generic method?

  // TODO: Given the disabled code below, we're snapping to the closest part of the closest road.
  // We may be better snapping to a road node on that road instead to avoid creating extra road
  // nodes, but only if it is within a very small tolerance of the selected road part.

  // TODO: optionally, flag snapped road nodes for review?

//  // snap to road way node index
//  boost::shared_ptr<Tgs::MemoryPageStore> mps1(new Tgs::MemoryPageStore(728));
//  boost::shared_ptr<Tgs::HilbertRTree> snapToRoadNodeIndex(new Tgs::HilbertRTree(mps1, 2));
//  // TODO: make the status part of these crits configurable?
//  boost::shared_ptr<HighwayNodeCriterion> highwayNodeCrit(new HighwayNodeCriterion());
//  highwayNodeCrit->setOsmMap(map.get());
//  boost::shared_ptr<ChainCriterion> roadNodeToSnapToCrit(
//    new ChainCriterion(new StatusCriterion(Status::Unknown1), highwayNodeCrit));
//  std::deque<ElementId> snapToRoadNodeIndexToEid;
//  IndexElementsVisitor v1(
//    snapToRoadNodeIndex, snapToRoadNodeIndexToEid, roadNodeToSnapToCrit,
//    boost::bind(&SnapUnconnectedRoads::_getSearchRadius, this, _1), map);
//  LOG_DEBUG(v1.getInitStatusMessage());
//  map->visitNodesRo(v1);
//  v1.finalizeIndex();
//  LOG_DEBUG(v1.getCompletedStatusMessage());
//  LOG_VARD(snapToRoadNodeIndexToEid.size());

  // snap to road way index
  boost::shared_ptr<Tgs::MemoryPageStore> mps2(new Tgs::MemoryPageStore(728));
  boost::shared_ptr<Tgs::HilbertRTree> snapToRoadIndex(new Tgs::HilbertRTree(mps2, 2));
  // TODO: make the status part of these crits configurable?
  boost::shared_ptr<HighwayCriterion> highwayCrit(new HighwayCriterion());
  boost::shared_ptr<ChainCriterion> roadToSnapToCrit(
    new ChainCriterion(new StatusCriterion(Status::Unknown1), highwayCrit));
  std::deque<ElementId> snapToRoadIndexToEid;
  IndexElementsVisitor v2(
    snapToRoadIndex, snapToRoadIndexToEid, roadToSnapToCrit,
    boost::bind(&SnapUnconnectedRoads::_getSearchRadius, this, _1), map);
  LOG_DEBUG(v2.getInitStatusMessage());  // TODO: add crit to this status message?
  map->visitWaysRo(v2);
  v2.finalizeIndex();
  LOG_DEBUG(v2.getCompletedStatusMessage());
  LOG_VARD(snapToRoadIndexToEid.size());

  boost::shared_ptr<ChainCriterion> roadToSnapCrit(
    new ChainCriterion(new HighwayCriterion(), new StatusCriterion(Status::Unknown2)));
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const ConstWayPtr& way = wayItr->second;
    LOG_VARD(way->getElementId());

    // for all roads
    if (roadToSnapCrit->isSatisfied(way))
    {
      // if the road's end node isn't shared with another road

      // get the road's nodes
      const std::vector<long>& roadNodeIds = way->getNodeIds();
      LOG_VARD(roadNodeIds);

      // find all ways that contain both end points
      const long firstRoadEndNodeId = roadNodeIds.at(0);
      LOG_VARD(firstRoadEndNodeId);
      // TODO: add feature filtering to getWaysByNode?
      const std::set<long>& waysContainingFirstEndNode =
        /*nodeToWayMap*/map->getIndex().getNodeToWayMap()->getWaysByNode(firstRoadEndNodeId);
      LOG_VARD(waysContainingFirstEndNode);
      const long secondRoadEndNodeId = roadNodeIds.at(roadNodeIds.size() - 1);
      LOG_VARD(secondRoadEndNodeId);
      const std::set<long>& waysContainingSecondEndNode =
        /*nodeToWayMap*/map->getIndex().getNodeToWayMap()->getWaysByNode(secondRoadEndNodeId);
      LOG_VARD(waysContainingFirstEndNode);

      // filter the connected ways down to roads
      std::set<long> roadsContainingFirstEndNode;
      for (std::set<long>::const_iterator containingWaysItr1 = waysContainingFirstEndNode.begin();
           containingWaysItr1 != waysContainingFirstEndNode.end(); ++containingWaysItr1)
      {
        const long containingWayId = *containingWaysItr1;
        if (roadToSnapCrit->isSatisfied(map->getWay(containingWayId)))
        {
          roadsContainingFirstEndNode.insert(containingWayId);
        }
      }
      LOG_VARD(roadsContainingFirstEndNode);
      std::set<long> roadsContainingSecondEndNode;
      for (std::set<long>::const_iterator containingWaysItr2 = waysContainingSecondEndNode.begin();
           containingWaysItr2 != waysContainingSecondEndNode.end(); ++containingWaysItr2)
      {
        const long containingWayId = *containingWaysItr2;
        if (roadToSnapCrit->isSatisfied(map->getWay(containingWayId)))
        {
          roadsContainingSecondEndNode.insert(containingWayId);
        }
      }
      LOG_VARD(roadsContainingSecondEndNode);

      // If only one road is connected to a node, then then that node is an unconnected end road
      // node.
      std::set<long> unconnectedRoadIds;
      if (roadsContainingFirstEndNode.size() == 1)
      {
        unconnectedRoadIds.insert(firstRoadEndNodeId);
      }
      if (roadsContainingSecondEndNode.size() == 1)
      {
        unconnectedRoadIds.insert(secondRoadEndNodeId);
      }
      LOG_VARD(unconnectedRoadIds);

      for (std::set<long>::const_iterator unconnectedRoadIdItr = unconnectedRoadIds.begin();
           unconnectedRoadIdItr != unconnectedRoadIds.end(); ++unconnectedRoadIdItr)
      {
          const long unconnectedRoadId = *unconnectedRoadIdItr;
          LOG_VARD(_snappedRoadNodes.contains(unconnectedRoadId));
          if (/*unconnectedRoadId != 0 && */!_snappedRoadNodes.contains(unconnectedRoadId))
          {
    //        // Find all road nodes near this unconnected node that don't belong to the same way the
    //        // unconnected road node is already on.
    //        NodePtr roadNode = map->getNode(unconnectedRoadId);
    //        assert(roadNode->getStatus() == Status::Unknown2);
    //        LOG_VARD(roadNode->getId());
    //        assert(roadNode->getId() == unconnectedRoadId);
    //        boost::shared_ptr<geos::geom::Envelope> env(roadNode->getEnvelope(map));
    //        env->expandBy(_getSearchRadius(roadNode));
    //        // TODO: These are not sorted by distance.
    //        const std::set<ElementId> neighbors =
    //          IndexElementsVisitor::findNeighbors(
    //            *env, snapToRoadNodeIndex, snapToRoadNodeIndexToEid, map);
    //        LOG_VARD(neighbors);

    //        if (neighbors.size() > 0)
    //        {
    //          // If there were any road node neighbors, let's try snapping to the first one that we can.
    //          // This avoid creating any unnecessary new road nodes.

    //          // TODO: Are these already sorted by distance?
    //          const std::set<long>& waysContainingUnconnectedRoadNode =
    //            /*nodeToWayMap*/map->getIndex().getNodeToWayMap()->getWaysByNode(unconnectedRoadId);
    //          LOG_VARD(waysContainingUnconnectedRoadNode);

    //          for (std::set<ElementId>::const_iterator neighborsItr = neighbors.begin();
    //               neighborsItr != neighbors.end(); ++neighborsItr)
    //          {
    //            const long neighborId = (*neighborsItr).getId();
    //            LOG_VARD(neighborId);

    //            // Don't try to snap a road to itself.
    //            if (neighborId != unconnectedRoadId)
    //            {
    //              const std::set<long> waysContainingNeighbor =
    //                /*nodeToWayMap*/map->getIndex().getNodeToWayMap()->getWaysByNode(neighborId);
    //              LOG_VARD(waysContainingNeighbor);
    //              // TODO: check way for status = 1?

    //              std::set<long> intersection;
    //              std::set_intersection(
    //                waysContainingUnconnectedRoadNode.begin(), waysContainingUnconnectedRoadNode.end(),
    //                waysContainingNeighbor.begin(), waysContainingNeighbor.end(),
    //                std::inserter(intersection, intersection.begin()));
    //              LOG_VARD(intersection);
    //              // Don't try to snap a way to itself.
    //              if (intersection.size() == 0) // if they don't contain any of the same values
    //              {
    //                // snap the unconnected road node to the nearest road node
    //                NodePtr neighborRoadNode = map->getNode(neighborId);
    //                assert(neighborRoadNode->getId() == neighborId);
    //                assert(neighborRoadNode->getStatus() == Status::Unknown1);
    //                LOG_VARD(neighborRoadNode->getId());
    //                LOG_VARD(_snappedToRoadNodes.contains(neighborRoadNode->getId()));
    //                if (neighborRoadNode->getId() != roadNode->getId() /*&&
    //                    // I don't think this is necessary.
    //                    !_snappedToRoadNodes.contains(neighborRoadNode->getId())*/)
    //                {
    //                  LOG_DEBUG(
    //                    "Snapping road node: " << roadNode->getElementId() << " to road node: " <<
    //                    neighborRoadNode->getElementId());
    //                  roadNode->setX(neighborRoadNode->getX());
    //                  roadNode->setY(neighborRoadNode->getY());
    //                  _snappedToRoadNodes.append(neighborRoadNode->getId());
    //                  _snappedRoadNodes.append(roadNode->getId());
    //                  _numAffected++;
    //                }
    //              }
    //            }
    //          }
    //        }
            //else
            //{
              // If no nearby road nodes were found, we're going to try to find nearby roads, then
              // snap into to the closest location on the neighboring road.

              NodePtr roadNode = map->getNode(unconnectedRoadId);
              assert(roadNode->getStatus() == Status::Unknown2);
              LOG_VARD(roadNode->getId());
              assert(roadNode->getId() == unconnectedRoadId);
              boost::shared_ptr<geos::geom::Envelope> env(roadNode->getEnvelope(map));
              env->expandBy(_getSearchRadius(roadNode));
              // TODO: These are not sorted by distance.
              const std::set<ElementId> neighbors =
                IndexElementsVisitor::findNeighbors(*env, snapToRoadIndex, snapToRoadIndexToEid, map);
              LOG_VARD(neighbors);

              for (std::set<ElementId>::const_iterator neighborsItr = neighbors.begin();
                   neighborsItr != neighbors.end(); ++neighborsItr)
              {
                // We're arbitrarily snapping to the first neighbor road (for now?).

                const long neighborId = (*neighborsItr).getId();
                LOG_VARD(neighborId);
                WayPtr roadToSnapTo = map->getWay(neighborId);
                assert(highwayCrit->isSatisfied(way));
                assert(roadToSnapTo->getId() == neighborId);
                LOG_VARD(roadToSnapTo);
                const std::vector<long>& roadToSnapToNodeIds = roadToSnapTo->getNodeIds();
                LOG_VARD(roadToSnapToNodeIds);

                LOG_VARD(
                  std::find(
                    roadToSnapToNodeIds.begin(), roadToSnapToNodeIds.end(), unconnectedRoadId) ==
                    roadToSnapToNodeIds.end());
                if (std::find(
                      roadToSnapToNodeIds.begin(), roadToSnapToNodeIds.end(), unconnectedRoadId) ==
                      roadToSnapToNodeIds.end())
                {
                  WayDiscretizer wd1(map, roadToSnapTo);
                  std::vector<geos::geom::Coordinate> cs1;
                  wd1.discretize(1.0, cs1);
                  geos::geom::Coordinate closestCoordinate;
                  double shortestDistance = DBL_MAX;
                  cs1.push_back(map->getNode(roadToSnapTo->getNodeId(0))->toCoordinate());
                  cs1.push_back(
                    map->getNode(
                      roadToSnapTo->getNodeId(roadToSnapTo->getNodeIds().size() - 1))->toCoordinate());
                  LOG_VARD(cs1);
                  for (size_t i = 0; i < cs1.size(); i++)
                  {
                    geos::geom::Coordinate coord = cs1[i];
                    LOG_VARD(coord);
                    const double distanceToRoadNode = coord.distance(roadNode->toCoordinate());
                    LOG_VARD(distanceToRoadNode);
                    // TODO: This *may* be able to be optimized to skip out once the distance values
                    // start getting larger.
                    if (distanceToRoadNode < shortestDistance)
                    {
                      closestCoordinate = coord;
                      shortestDistance = distanceToRoadNode;
                    }
                  }
                  LOG_VARD(shortestDistance);

                  // This check of less than the allowed snap distance should not be necessary.  For
                  // some reason I don't understand yet, neighbors are being returned at longer
                  // distances than they should be.
                  if (/*shortestDistance != DBL_MAX &&*/ shortestDistance <= 5.0)
                  {
                    LOG_DEBUG(
                      "Snapping road node: " << roadNode->getElementId() << " to coord: " <<
                      closestCoordinate.toString());
                    roadNode->setX(closestCoordinate.x);
                    roadNode->setY(closestCoordinate.y);
                    map->addNode(roadNode);
                    _snappedRoadNodes.append(roadNode->getId());

                    // Find the closest node to our new node on the snap to road.
                    // TODO: Is there a better way to do this using WayLocation?
                    shortestDistance = DBL_MAX;
                    long closestNodeId = 0;
                    for (size_t i = 0; i < roadToSnapToNodeIds.size(); i++)
                    {
                      boost::shared_ptr<Node> snapToRoadNode = map->getNode(roadToSnapToNodeIds[i]);
                      LOG_VARD(snapToRoadNode);
                      const double distanceToSnapToRoadNode =
                        Distance::euclidean(roadNode->toCoordinate(), snapToRoadNode->toCoordinate());
                      LOG_VARD(distanceToSnapToRoadNode);
                      if (distanceToSnapToRoadNode < shortestDistance)
                      {
                        shortestDistance = distanceToSnapToRoadNode;
                        closestNodeId = snapToRoadNode->getId();
                      }
                    }
                    LOG_VARD(shortestDistance);
                    LOG_VARD(closestNodeId);

                    // Now, figure out if the way road node after the closest node is closer to our
                    // new node or way node before is closer.  That tells us where to add our new way
                    // node to the way.
                    const int closestWayNodeIndex = roadToSnapTo->getNodeIndex(closestNodeId);
                    LOG_VARD(closestWayNodeIndex);
                    int otherWayNodeIndex = -1;
                    if (closestWayNodeIndex == 0)
                    {
                      assert(roadToSnapToNodeIds.size() >= 2);
                      otherWayNodeIndex = 1;
                    }
                    else if (closestWayNodeIndex == (int)(roadToSnapToNodeIds.size() - 1))
                    {
                      assert(roadToSnapToNodeIds.size() >= 2);
                      otherWayNodeIndex = roadToSnapToNodeIds.size() - 2;
                    }
                    else
                    {
                      // try both the one before and after to see which is closer
                      const int nodeBeforeIndex = closestWayNodeIndex - 1;
                      LOG_VARD(nodeBeforeIndex);
                      const long nodeBeforeId = roadToSnapTo->getNodeId(nodeBeforeIndex);
                      LOG_VARD(nodeBeforeId);
                      const int nodeAfterIndex = closestWayNodeIndex + 1;
                      LOG_VARD(nodeAfterIndex);
                      const long nodeAfterId = roadToSnapTo->getNodeId(nodeAfterIndex);
                      LOG_VARD(nodeAfterId);

                      assert(map->containsNode(nodeBeforeId));
                      assert(map->containsNode(nodeAfterId));
                      //LOG_VARD(map->containsNode(nodeBeforeId));
                      //LOG_VARD(map->containsNode(nodeAfterId));

                      LOG_VARD(
                        Distance::euclidean(
                          map->getNode(nodeBeforeId)->toCoordinate(), roadNode->toCoordinate()));
                      LOG_VARD(
                        Distance::euclidean(
                          map->getNode(nodeAfterId)->toCoordinate(), roadNode->toCoordinate()));
                      if (Distance::euclidean(
                            map->getNode(nodeBeforeId)->toCoordinate(), roadNode->toCoordinate()) <
                          Distance::euclidean(
                            map->getNode(nodeAfterId)->toCoordinate(), roadNode->toCoordinate()))
                      {
                        otherWayNodeIndex = nodeBeforeIndex;
                      }
                      else
                      {
                        otherWayNodeIndex = nodeAfterIndex;
                      }
                    }
                    LOG_VARD(otherWayNodeIndex);

                    int insertIndex = -1;
                    assert(otherWayNodeIndex != closestWayNodeIndex);
                    if (otherWayNodeIndex > closestWayNodeIndex)
                    {
                      insertIndex = closestWayNodeIndex + 1;
                    }
                    else
                    {
                      insertIndex = closestWayNodeIndex - 1;
                    }
                    LOG_VARD(insertIndex);

                    // Add the new snapped node as a way node on the snapped to road.
                    QList<long> newRoadToSnapToNodeIds =
                      QList<long>::fromVector(QVector<long>::fromStdVector(roadToSnapToNodeIds));
                    LOG_VARD(newRoadToSnapToNodeIds);
                    newRoadToSnapToNodeIds.insert(insertIndex, roadNode->getId());
                    LOG_VARD(newRoadToSnapToNodeIds);
                    roadToSnapTo->setNodes(newRoadToSnapToNodeIds.toVector().toStdVector());
                    LOG_VARD(roadToSnapTo->getNodeIds());

                    _numAffected++;

                    // Kick out of the neighbors loop, as we don't want to snap the road more than once.
                    break;
                  }
                }
              }
            //}
          }
      }
    }
  }
}

}
