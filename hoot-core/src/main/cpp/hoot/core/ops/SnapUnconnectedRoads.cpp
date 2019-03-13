
#include "SnapUnconnectedRoads.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/visitors/IndexElementsVisitor.h>
#include <hoot/core/criterion/HighwayNodeCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>
#include <tgs/RStarTree/HilbertRTree.h>

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

  // TODO: tune this? - see #3054
  boost::shared_ptr<Tgs::MemoryPageStore> mps(new Tgs::MemoryPageStore(728));
  boost::shared_ptr<Tgs::HilbertRTree> index(new Tgs::HilbertRTree(mps, 2));
  boost::shared_ptr<HighwayNodeCriterion> highwayNodeCrit(new HighwayNodeCriterion());
  highwayNodeCrit->setOsmMap(map.get());
  boost::shared_ptr<ChainCriterion> roadNodeToSnapToCrit(
    new ChainCriterion(new StatusCriterion(Status::Unknown1), highwayNodeCrit));
  std::deque<ElementId> indexToEid;
  IndexElementsVisitor v(
    index, indexToEid, roadNodeToSnapToCrit,
    boost::bind(&SnapUnconnectedRoads::_getSearchRadius, this, _1), map);
  LOG_DEBUG(v.getInitStatusMessage());
  map->visitNodesRo(v);
  v.finalizeIndex();
  LOG_DEBUG(v.getCompletedStatusMessage());
  LOG_VARD(indexToEid.size());

  //HighwayCriterion highwayCrit;
  boost::shared_ptr<ChainCriterion> roadToSnapCrit(
    new ChainCriterion(new HighwayCriterion(), new StatusCriterion(Status::Unknown2)));
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const ConstWayPtr& way = wayItr->second;
    LOG_VARD(way->getElementId());

    // for all roads
    //if (highwayCrit.isSatisfied(way))
    if (roadToSnapCrit->isSatisfied(way))
    {
      // if the road's end node isn't shared with another road

      // get the roads nodes
      const std::vector<long>& roadNodeIds = way->getNodeIds();
      LOG_VARD(roadNodeIds);

      // find all ways that contain both end points
      const long firstRoadEndNodeId = roadNodeIds.at(0);
      LOG_VARD(firstRoadEndNodeId);
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
        //if (highwayCrit.isSatisfied(map->getWay(containingWayId)))
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
        //if (highwayCrit.isSatisfied(map->getWay(containingWayId)))
        if (roadToSnapCrit->isSatisfied(map->getWay(containingWayId)))
        {
          roadsContainingSecondEndNode.insert(containingWayId);
        }
      }
      LOG_VARD(roadsContainingSecondEndNode);

      // If only one road is connected, then we have one unconnected end road node.
      long unconnectedRoadId = 0;
      if (roadsContainingFirstEndNode.size() == 1 && roadsContainingSecondEndNode.size() > 1)
      {
        unconnectedRoadId = firstRoadEndNodeId;
      }
      else if (roadsContainingFirstEndNode.size() > 1 &&
               roadsContainingSecondEndNode.size() == 1)
      {
        unconnectedRoadId = secondRoadEndNodeId;
      }
      else if (roadsContainingFirstEndNode.size() == 1 &&
               roadsContainingSecondEndNode.size() == 1)
      {
        LOG_DEBUG("Both end road nodes connected to one road (?).  Skipping snap...");
      }
      LOG_VARD(unconnectedRoadId);

      LOG_VARD(_snappedRoadNodes.contains(unconnectedRoadId));
      if (unconnectedRoadId != 0 && !_snappedRoadNodes.contains(unconnectedRoadId))
      {
        // find all road nodes near this unconnected node that don't belong to the same way the
        // unconnected road node is already on
        NodePtr roadNode = map->getNode(unconnectedRoadId);
        assert(roadNode->getStatus() == Status::Unknown2);
        LOG_VARD(roadNode->getId());
        assert(roadNode->getId() == unconnectedRoadId);
        boost::shared_ptr<geos::geom::Envelope> env(roadNode->getEnvelope(map));
        env->expandBy(_getSearchRadius(roadNode));
        const std::set<ElementId> neighbors =
          IndexElementsVisitor::findNeighbors(*env, index, indexToEid, map);
        LOG_VARD(neighbors);

        // TODO: Are these already sorted by distance?
        const std::set<long>& waysContainingUnconnectedRoadNode =
          /*nodeToWayMap*/map->getIndex().getNodeToWayMap()->getWaysByNode(unconnectedRoadId);
        LOG_VARD(waysContainingUnconnectedRoadNode);

        for (std::set<ElementId>::const_iterator neighborsItr = neighbors.begin();
             neighborsItr != neighbors.end(); ++neighborsItr)
        {
          const long neighborId = (*neighborsItr).getId();
          LOG_VARD(neighborId);

          if (neighborId != unconnectedRoadId)
          {
            const std::set<long> waysContainingNeighbor =
              /*nodeToWayMap*/map->getIndex().getNodeToWayMap()->getWaysByNode(neighborId);
            LOG_VARD(waysContainingNeighbor);
            // TODO: check way for status = 1

            std::set<long> intersection;
            std::set_intersection(
              waysContainingUnconnectedRoadNode.begin(), waysContainingUnconnectedRoadNode.end(),
              waysContainingNeighbor.begin(), waysContainingNeighbor.end(),
              std::inserter(intersection, intersection.begin()));
            LOG_VARD(intersection);
            if (intersection.size() == 0) //if they don't contain any of the same values
            {
              // snap the unconnected road node to the nearest road node
              NodePtr neighborRoadNode = map->getNode(neighborId);
              assert(neighborRoadNode->getId() == neighborId);
              assert(neighborRoadNode->getStatus() == Status::Unknown1);
              LOG_VARD(neighborRoadNode->getId());
              LOG_VARD(_snappedToRoadNodes.contains(neighborRoadNode->getId()));
              if (neighborRoadNode->getId() != roadNode->getId() /*&&
                  // I don't think this is necessary.
                  !_snappedToRoadNodes.contains(neighborRoadNode->getId())*/)
              {
                LOG_DEBUG(
                  "Snapping road node: " << roadNode->getElementId() << " to road node: " <<
                  neighborRoadNode->getElementId());
                roadNode->setX(neighborRoadNode->getX());
                roadNode->setY(neighborRoadNode->getY());
                _snappedToRoadNodes.append(neighborRoadNode->getId());
                _snappedRoadNodes.append(roadNode->getId());
                _numAffected++;
              }
            }
          }
        }
      }
    }
  }
}

}
