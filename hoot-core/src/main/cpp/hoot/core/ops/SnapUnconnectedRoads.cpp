
#include "SnapUnconnectedRoads.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/visitors/IndexElementsVisitor.h>
#include <hoot/core/criterion/HighwayNodeCriterion.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>
#include <tgs/RStarTree/HilbertRTree.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, SnapUnconnectedRoads)

SnapUnconnectedRoads::SnapUnconnectedRoads()
{
}

Meters SnapUnconnectedRoads::_getSearchRadius(const boost::shared_ptr<const Element>& e) const
{
  return e->getCircularError(); //??
}

void SnapUnconnectedRoads::apply(OsmMapPtr& map)
{  
  _numAffected = 0;

  const WayMap ways = map->getWays();   //copy
  boost::shared_ptr<NodeToWayMap> nodeToWayMap = map->getIndex().getNodeToWayMap(); //copy

  // TODO: tune this? - see #3054
  boost::shared_ptr<Tgs::MemoryPageStore> mps(new Tgs::MemoryPageStore(728));
  boost::shared_ptr<Tgs::HilbertRTree> index(new Tgs::HilbertRTree(mps, 2));
  boost::shared_ptr<HighwayNodeCriterion> crit(new HighwayNodeCriterion());
  std::deque<ElementId> indexToEid;
  IndexElementsVisitor v(index,
                         indexToEid,
                         crit,
                         boost::bind(&SnapUnconnectedRoads::_getSearchRadius, this, _1),
                         map);
  map->visitNodesRo(v);
  v.finalizeIndex();

  HighwayCriterion highwayCrit;
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const ConstWayPtr& way = wayItr->second;
    LOG_VARD(way->getElementId());

    // for all roads
    if (highwayCrit.isSatisfied(way))
    {
      // if the road's end node isn't shared with another road

      // get the roads nodes
      const std::vector<long>& roadNodeIds = way->getNodeIds();
      LOG_VARD(roadNodeIds);

      // find all ways that contain both end points
      const long firstRoadEndNodeId = roadNodeIds.at(0);
      const std::set<long>& waysContainingFirstEndNode =
        nodeToWayMap->getWaysByNode(firstRoadEndNodeId);
      LOG_VARD(waysContainingFirstEndNode);
      const long secondRoadEndNodeId = roadNodeIds.at(roadNodeIds.size() - 1);
      const std::set<long>& waysContainingSecondEndNode =
        nodeToWayMap->getWaysByNode(secondRoadEndNodeId);
      LOG_VARD(waysContainingFirstEndNode);

      // filter the connected ways down to roads
      std::set<long> roadsContainingFirstEndNode;
      for (std::set<long>::const_iterator containingWaysItr1 = waysContainingFirstEndNode.begin();
           containingWaysItr1 != waysContainingFirstEndNode.end(); ++containingWaysItr1)
      {
        if (highwayCrit.isSatisfied(map->getWay(*containingWaysItr1)))
        {
          roadsContainingFirstEndNode.insert(*containingWaysItr1);
        }
      }
      LOG_VARD(roadsContainingFirstEndNode);
      std::set<long> roadsContainingSecondEndNode;
      for (std::set<long>::const_iterator containingWaysItr2 = waysContainingSecondEndNode.begin();
           containingWaysItr2 != waysContainingSecondEndNode.end(); ++containingWaysItr2)
      {
        if (highwayCrit.isSatisfied(map->getWay(*containingWaysItr2)))
        {
          roadsContainingSecondEndNode.insert(*containingWaysItr2);
        }
      }
      LOG_VARD(roadsContainingSecondEndNode);

      // If only one road is connected, then we have one unconnected end road node.
      long unconnectedRoadId = 0;
      if (roadsContainingFirstEndNode.size() == 1 && roadsContainingSecondEndNode.size() == 0)
      {
        unconnectedRoadId = firstRoadEndNodeId;
      }
      else if (roadsContainingFirstEndNode.size() == 0 &&
               roadsContainingSecondEndNode.size() == 1)
      {
        unconnectedRoadId = secondRoadEndNodeId;
      }
      LOG_VARD(unconnectedRoadId);

      if (unconnectedRoadId != 0)
      {
        // find all road nodes near this unconnected node that don't belong to the same way the
        // unconnected road node is already on
        NodePtr roadNode = map->getNode(unconnectedRoadId);
        boost::shared_ptr<geos::geom::Envelope> env(roadNode->getEnvelope(map));
        env->expandBy(_getSearchRadius(roadNode));
        const std::set<ElementId> neighbors =
          IndexElementsVisitor::findNeighbors(*env, index, indexToEid, map);
        LOG_VARD(neighbors);

        // TODO: Are these already sorted by distance?
        const std::set<long>& waysContainingUnconnectedRoadNode =
          nodeToWayMap->getWaysByNode(unconnectedRoadId);
        for (std::set<ElementId>::const_iterator neighborsItr = neighbors.begin();
             neighborsItr != neighbors.end(); ++neighborsItr)
        {
          const std::set<long> waysContainingNeighbor =
            nodeToWayMap->getWaysByNode((*neighborsItr).getId());
          LOG_VARD(waysContainingNeighbor);

          std::set<long> intersection;
          std::set_intersection(
            waysContainingUnconnectedRoadNode.begin(), waysContainingUnconnectedRoadNode.end(),
            waysContainingNeighbor.begin(), waysContainingNeighbor.end(),
            std::inserter(intersection, intersection.begin()));
          LOG_VARD(intersection);
          if (intersection.size() == 0) //if they don't contain any of the same values
          {
            // snap the unconnected road node to the nearest road node
            NodePtr neighborRoadNode = map->getNode((*neighborsItr).getId());
            LOG_VARD(neighborRoadNode);
            roadNode->setX(neighborRoadNode->getX());
            roadNode->setY(neighborRoadNode->getY());
            _numAffected++;
          }
        }
      }
    }
  }
}

}
