
#include "SnapUnconnectedWays.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/visitors/IndexElementsVisitor.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/algorithms/Distance.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/ops/ReplaceElementOp.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

// GEOS
#include <geos/geom/Coordinate.h>

#include <float.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, SnapUnconnectedWays)

SnapUnconnectedWays::SnapUnconnectedWays() :
_maxNodeReuseDistance(0.5),
_maxSnapDistance(5.0),
_snappedRoadsTagKey("")
{
  setConfiguration(conf());
}

void SnapUnconnectedWays::setConfiguration(const Settings& conf)
{
  ConfigOptions confOpts = ConfigOptions(conf);

  if (confOpts.getSnapUnconnectedWaysReuseWayNodes())
  {
    _maxNodeReuseDistance = confOpts.getSnapUnconnectedWaysWayNodeReuseTolerance();
    if (_maxNodeReuseDistance < 0.0)
    {
      throw IllegalArgumentException(
        "Invalid " + ConfigOptions::getSnapUnconnectedWaysWayNodeReuseToleranceKey() + " value: "
        + QString::number(_maxNodeReuseDistance) +
        ". The value must be greater than or equal to 0.0.");
    }
  }
  else
  {
    _maxNodeReuseDistance = 0.0;
  }
  _maxSnapDistance = confOpts.getSnapUnconnectedWaysSnapTolerance();
  if (_maxSnapDistance < 0.0)
  {
    throw IllegalArgumentException(
      "Invalid " + ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey() + " value: "
      + QString::number(_maxSnapDistance) + ". The value must be greater than or equal to 0.0.");
  }
  _snappedRoadsTagKey = confOpts.getSnapUnconnectedWaysTagKey();

  _snapWayStatus = Status::fromString(confOpts.getSnapUnconnectedWaysSnapWayStatus());
  _snapToWayStatus = Status::Unknown1;
  if (_snapWayStatus == Status::Unknown1)
  {
    _snapToWayStatus = Status::Unknown2;
  }

  _wayToSnapCriterionClassName = confOpts.getSnapUnconnectedWaysSnapWayCriterion().trimmed();
  if (_wayToSnapCriterionClassName.isEmpty())
  {
    _wayToSnapCriterionClassName = "hoot::WayCriterion";
  }
  _wayToSnapToCriterionClassName = confOpts.getSnapUnconnectedWaysSnapToWayCriterion().trimmed();
  if (_wayToSnapToCriterionClassName.isEmpty())
  {
    _wayToSnapToCriterionClassName = "hoot::WayCriterion";
  }
  if (_maxNodeReuseDistance != 0.0)
  {
    _wayNodeToSnapToCriterionClassName =
      confOpts.getSnapUnconnectedWaysSnapToWayNodeCriterion().trimmed();
    if (_wayNodeToSnapToCriterionClassName.isEmpty())
    {
      _wayNodeToSnapToCriterionClassName = "hoot::WayNodeCriterion";
    }
  }

  _conf = conf;
}

ElementCriterionPtr SnapUnconnectedWays::_createFeatureCriterion(
  const QString criterionClassName, const Status& status)
{
  ElementCriterionPtr outputCrit;
  const QString critClass = criterionClassName.trimmed();
  if (!critClass.isEmpty())
  {
    ElementCriterionPtr critTemp(
      Factory::getInstance().constructObject<ElementCriterion>(critClass));
    Configurable* configurable = dynamic_cast<Configurable*>(critTemp.get());
    if (configurable != 0)
    {
      configurable->setConfiguration(_conf);
    }
    ConstOsmMapConsumer* mapConsumer = dynamic_cast<ConstOsmMapConsumer*>(critTemp.get());
    if (mapConsumer != 0)
    {
      mapConsumer->setOsmMap(_map.get());
    }
    outputCrit.reset(new ChainCriterion(new StatusCriterion(status), critTemp));
  }
  return outputCrit;
}

Meters SnapUnconnectedWays::_getWaySearchRadius(const boost::shared_ptr<const Element>& /*e*/) const
{
  return _maxSnapDistance;
}

Meters SnapUnconnectedWays::_getWayNodeSearchRadius(
  const boost::shared_ptr<const Element>& /*e*/) const
{
  return _maxNodeReuseDistance;
}

std::set<long> SnapUnconnectedWays::_getUnconnectedWayEndNodeIds(const ConstWayPtr& way,
                                                           const ElementCriterionPtr& wayCrit) const
{
  std::set<long> unconnectedWayEndNodeIds;

  // get the way's nodes
  const std::vector<long>& wayNodeIds = way->getNodeIds();
  LOG_VARD(wayNodeIds);

  // grab the endpoints
  const long firstWayEndNodeId = wayNodeIds.at(0);
  LOG_VARD(firstWayEndNodeId);
  const long secondWayEndNodeId = wayNodeIds.at(wayNodeIds.size() - 1);
  LOG_VARD(secondWayEndNodeId);

  // filter the connected ways to each endpoint down by crit
  const std::set<long> filteredWaysContainingFirstEndNode =
    OsmUtils::getContainingWayIdsByNodeId(firstWayEndNodeId, _map, wayCrit);
  const std::set<long> filteredWaysContainingSecondEndNode =
    OsmUtils::getContainingWayIdsByNodeId(secondWayEndNodeId, _map, wayCrit);

  // If only one way is connected to a node, then then that node is an unconnected end way
  // node.
  if (filteredWaysContainingFirstEndNode.size() == 1)
  {
    unconnectedWayEndNodeIds.insert(firstWayEndNodeId);
  }
  if (filteredWaysContainingSecondEndNode.size() == 1)
  {
    unconnectedWayEndNodeIds.insert(secondWayEndNodeId);
  }
  LOG_VARD(unconnectedWayEndNodeIds);

  return unconnectedWayEndNodeIds;
}

void SnapUnconnectedWays::_createFeatureIndex(ElementCriterionPtr featureCrit,
                                              boost::shared_ptr<Tgs::HilbertRTree> index,
                                              std::deque<ElementId>& indexToEid,
                                              const ElementType& elementType)
{
  // TODO: tune these indexes? - see #3054
  boost::shared_ptr<Tgs::MemoryPageStore> mps(new Tgs::MemoryPageStore(728));
  index.reset(new Tgs::HilbertRTree(mps, 2));
  boost::shared_ptr<IndexElementsVisitor> indexer;
  if (elementType == ElementType::Node)
  {
    indexer.reset(
      new IndexElementsVisitor(
        index, indexToEid, featureCrit,
        boost::bind(&SnapUnconnectedWays::_getWayNodeSearchRadius, this, _1), _map));
  }
  else
  {
    indexer.reset(
      new IndexElementsVisitor(
        index, indexToEid, featureCrit,
        boost::bind(&SnapUnconnectedWays::_getWaySearchRadius, this, _1), _map));
  }
  LOG_DEBUG(indexer->getInitStatusMessage());
  if (elementType == ElementType::Node)
  {
    _map->visitNodesRo(*indexer);
  }
  else
  {
    _map->visitWaysRo(*indexer);
  }
  indexer->finalizeIndex();
  LOG_DEBUG(indexer->getCompletedStatusMessage());
  LOG_VARD(indexToEid.size());
}

std::set<ElementId> SnapUnconnectedWays::_getNearbyFeaturesToSnapTo(
  const ConstNodePtr& unconnectedWayEndNode, const ElementType& elementType) const
{
  LOG_VARD(unconnectedWayEndNode->getId());
  boost::shared_ptr<geos::geom::Envelope> env(unconnectedWayEndNode->getEnvelope(_map));
  if (elementType == ElementType::Node)
  {
    env->expandBy(_getWayNodeSearchRadius(unconnectedWayEndNode));
    // TODO: These are not sorted by distance.
    return
      IndexElementsVisitor::findNeighbors(*env, _snapToWayNodeIndex, _snapToWayNodeIndexToEid, _map);
  }
  else
  {
    env->expandBy(_getWaySearchRadius(unconnectedWayEndNode));
    // TODO: These are not sorted by distance.
    return IndexElementsVisitor::findNeighbors(*env, _snapToWayIndex, _snapToWayIndexToEid, _map);
  }
}

int SnapUnconnectedWays::_getSnappedNodeInsertIndex(NodePtr unconnectedWayEndNode,
                                                    const long closestSnapToWayNodeId,
                                                    const ConstWayPtr& snapToWay) const
{
  const std::vector<long>& snapToWayNodeIds = snapToWay->getNodeIds();
  const int indexOfClosestSnapToWayNodeId =
    snapToWay->getNodeIndex(closestSnapToWayNodeId);
  LOG_VARD(indexOfClosestSnapToWayNodeId);

  int indexOfSecondClosestSnapToWayNodeId = -1;
  if (indexOfClosestSnapToWayNodeId == 0)
  {
    assert(snapToWayNodeIds.size() >= 2);
    indexOfSecondClosestSnapToWayNodeId = 1;
  }
  else if (indexOfClosestSnapToWayNodeId == (int)(snapToWayNodeIds.size() - 1))
  {
    assert(neighborWayNodeIds.size() >= 2);
    indexOfSecondClosestSnapToWayNodeId = snapToWayNodeIds.size() - 2;
  }
  else
  {
    // try both the one before and after the closest node to see which is closer
    const int nodeBeforeIndex = indexOfClosestSnapToWayNodeId - 1;
    LOG_VARD(nodeBeforeIndex);
    const long nodeBeforeId = snapToWay->getNodeId(nodeBeforeIndex);
    LOG_VARD(nodeBeforeId);
    const int nodeAfterIndex = indexOfClosestSnapToWayNodeId + 1;
    LOG_VARD(nodeAfterIndex);
    const long nodeAfterId = snapToWay->getNodeId(nodeAfterIndex);
    LOG_VARD(nodeAfterId);
    assert(map->containsNode(nodeBeforeId));
    assert(map->containsNode(nodeAfterId));

    const double distFromBeforeNodeToUnconnectedEndNode =
      Distance::euclidean(
        _map->getNode(nodeBeforeId)->toCoordinate(), unconnectedWayEndNode->toCoordinate());
    const double distFromAfterNodeToUnconnectedEndNode =
      Distance::euclidean(
        _map->getNode(nodeAfterId)->toCoordinate(), unconnectedWayEndNode->toCoordinate());
    if (distFromBeforeNodeToUnconnectedEndNode < distFromAfterNodeToUnconnectedEndNode)
    {
      indexOfSecondClosestSnapToWayNodeId = nodeBeforeIndex;
    }
    else
    {
      indexOfSecondClosestSnapToWayNodeId = nodeAfterIndex;
    }
  }
  LOG_VARD(indexOfSecondClosestSnapToWayNodeId);

  int snappedNodeInsertIndex = -1;
  assert(indexOfSecondClosestSnapToWayNodeId != indexOfClosestSnapToWayNodeId);
  if (indexOfSecondClosestSnapToWayNodeId > indexOfClosestSnapToWayNodeId)
  {
    snappedNodeInsertIndex = indexOfClosestSnapToWayNodeId + 1;
  }
  else
  {
    snappedNodeInsertIndex = indexOfClosestSnapToWayNodeId - 1;
  }

  return snappedNodeInsertIndex;
}

bool SnapUnconnectedWays::_snapUnconnectedNodeToWayNode(NodePtr unconnectedWayEndNode)
{
  // find all way nodes near this unconnected node that don't belong to the same way
  // the unconnected way node is already on
  const std::set<ElementId> snapToNeighborWayNodes =
    _getNearbyFeaturesToSnapTo(unconnectedWayEndNode, ElementType::Node);
  LOG_VARD(snapToNeighborWayNodes);
  if (snapToNeighborWayNodes.size() > 0)
  {
    // If there were any way node neighbors, let's try snapping to the first one that
    // we can. This avoids creating any unnecessary new way nodes on the way being
    // snapped to.
    for (std::set<ElementId>::const_iterator snapToNeighborWayNodesItr
           = snapToNeighborWayNodes.begin();
         snapToNeighborWayNodesItr != snapToNeighborWayNodes.end(); ++snapToNeighborWayNodesItr)
    {
      const long snapToWayNodeNeighborId = (*snapToNeighborWayNodesItr).getId();
      LOG_VARD(snapToWayNodeNeighborId);

      // Don't try to snap a way node to itself.
      if (snapToWayNodeNeighborId != unconnectedWayEndNode->getId())
      {
        const std::set<long> waysContainingSnapToWayNodeNeighbor =
          /*nodeToWayMap*/_map->getIndex().getNodeToWayMap()->getWaysByNode(snapToWayNodeNeighborId);
        LOG_VARD(waysContainingSnapToWayNodeNeighbor);
        // TODO: also check way for the correct status here?

        // TODO: These aren't sorted by distance.
        const std::set<long>& waysContainingUnconnectedWayEndNode =
          /*nodeToWayMap*/_map->getIndex().getNodeToWayMap()->getWaysByNode(
            unconnectedWayEndNode->getId());
        LOG_VARD(waysContainingUnconnectedWayEndNode);

        std::set<long> commonNodesBetweenWayGroups;
        std::set_intersection(
          waysContainingUnconnectedWayEndNode.begin(),
          waysContainingUnconnectedWayEndNode.end(),
          waysContainingSnapToWayNodeNeighbor.begin(),
          waysContainingSnapToWayNodeNeighbor.end(),
          std::inserter(commonNodesBetweenWayGroups, commonNodesBetweenWayGroups.begin()));
        LOG_VARD(commonNodesBetweenWayGroups);
        // Don't try to snap a way to itself.
        if (commonNodesBetweenWayGroups.size() == 0)
        {
          LOG_DEBUG(
            "Snapping way node: " << unconnectedWayEndNode->getId() << " to way node: " <<
            snapToWayNodeNeighborId);

          // snap the unconnected road node to the nearest road node
          NodePtr snapToNeighborWayNode = _map->getNode(snapToWayNodeNeighborId);
          assert(snapToNeighborWayNode->getId() == snapToNeighborWayNode->getId());
          LOG_VARD(snapToNeighborWayNode->getId());
          LOG_VARD(_snappedToWayNodes.contains(snapToNeighborWayNode->getId()));

          const Tags mergedTags =
            TagMergerFactory::mergeTags(
              snapToNeighborWayNode->getTags(), unconnectedWayEndNode->getTags(), ElementType::Node);
          snapToNeighborWayNode->setTags(mergedTags);
          if (!_snappedRoadsTagKey.isEmpty())
          {
            snapToNeighborWayNode->getTags().set(_snappedRoadsTagKey, "yes");
          }

          ReplaceElementOp elementReplacer(
            unconnectedWayEndNode->getElementId(), snapToNeighborWayNode->getElementId(), true);
          elementReplacer.apply(_map);

          _snappedToWayNodes.append(snapToNeighborWayNode->getId());
          _snappedWayNodes.append(unconnectedWayEndNode->getId());
          _numAffected++;

          return true;
        }
      }
    }
  }

  return false;
}

bool SnapUnconnectedWays::_snapUnconnectedNodeToWay(NodePtr unconnectedWayEndNode)
{
  LOG_VARD(unconnectedWayEndNode->getId());

  const std::set<ElementId> neighborWays =
    _getNearbyFeaturesToSnapTo(unconnectedWayEndNode, ElementType::Way);
  LOG_VARD(neighborWays);
  for (std::set<ElementId>::const_iterator neighborWaysItr = neighborWays.begin();
       neighborWaysItr != neighborWays.end(); ++neighborWaysItr)
  {
    // We're arbitrarily snapping to the first neighbor road (for now?).

    WayPtr neighborWay = _map->getWay((*neighborWaysItr).getId());
    LOG_VARD(neighborWay);
    const std::vector<long>& neighborWayNodeIds = neighborWay->getNodeIds();
    LOG_VARD(neighborWayNodeIds);

    const bool neighborWayContainsUnconnectedWayNode =
      std::find(
        neighborWayNodeIds.begin(), neighborWayNodeIds.end(), unconnectedWayEndNode->getId()) ==
      neighborWayNodeIds.end();
    LOG_VARD(neighborWayContainsUnconnectedWayNode);
    if (!neighborWayContainsUnconnectedWayNode)
    {
      double shortestDistanceFromUnconnectedEndNodeToNeighborWayCoord = DBL_MAX;
      const geos::geom::Coordinate closestNeighborWayCoordinateToUnconnectedWayEndNode =
        OsmUtils::closestWayCoordToNode(unconnectedWayEndNode, neighborWay,
          shortestDistanceFromUnconnectedEndNodeToNeighborWayCoord, 1.0, _map);
      LOG_VARD(closestNeighborWayCoordinateToUnconnectedWayEndNode);
      LOG_VARD(shortestDistanceFromUnconnectedEndNodeToNeighborWayCoord);

      // This check of less than the allowed snap distance should not be necessary.  For
      // some reason I don't understand yet, neighbors are being returned at longer
      // distances than they should be.
      if (/*shortestDistance != DBL_MAX &&*/
          shortestDistanceFromUnconnectedEndNodeToNeighborWayCoord <= _maxSnapDistance)
      {
        // Find the closest node to our new node on the snap to road.
        // TODO: Is there a better way to do this using WayLocation?
        const long closestNeighborWayNodeIdToUnconnectedEndNode =
          OsmUtils::closestWayNodeIdToNode(unconnectedWayEndNode, neighborWay, _map);
        LOG_VARD(closestNeighborWayNodeIdToUnconnectedEndNode);

        // Now, figure out if the way node after the closest node is closer to our new node or if
        // the way node before it is closer.  That tells us which two existing way nodes to stick
        // our new way node on the way being snapped to.
        const int snappedNodeInsertIndex =
          _getSnappedNodeInsertIndex(
            unconnectedWayEndNode, closestNeighborWayNodeIdToUnconnectedEndNode, neighborWay);
        LOG_VARD(snappedNodeInsertIndex);

        LOG_DEBUG(
          "Snapping way node: " << unconnectedWayEndNode->getId() << " to coord: " <<
          closestNeighborWayCoordinateToUnconnectedWayEndNode.toString() <<
          " and inserting at way node index: " << snappedNodeInsertIndex);
        unconnectedWayEndNode->setX(closestNeighborWayCoordinateToUnconnectedWayEndNode.x);
        unconnectedWayEndNode->setY(closestNeighborWayCoordinateToUnconnectedWayEndNode.y);
        if (!_snappedRoadsTagKey.isEmpty())
        {
          unconnectedWayEndNode->getTags().set(_snappedRoadsTagKey, "yes");
        }
        _map->addNode(unconnectedWayEndNode);
        _snappedWayNodes.append(unconnectedWayEndNode->getId());

        // Add the new snapped node as a way node on the snapped to road.
        const std::vector<long>& neighborWayNodeIds = neighborWay->getNodeIds();
        LOG_VARD(neighborWayNodeIds);
        QList<long> wayToSnapToNodeIds =
          QList<long>::fromVector(QVector<long>::fromStdVector(neighborWayNodeIds));
        LOG_VARD(wayToSnapToNodeIds);
        wayToSnapToNodeIds.insert(snappedNodeInsertIndex, unconnectedWayEndNode->getId());
        LOG_VARD(wayToSnapToNodeIds);
        neighborWay->setNodes(wayToSnapToNodeIds.toVector().toStdVector());
        LOG_VARD(neighborWay->getNodeIds());

        _numAffected++;

        // Kick out of the neighbors loop, as we don't want to snap the road more than
        // once.
        return true;
      }
    }
  }

  return false;
}

void SnapUnconnectedWays::apply(OsmMapPtr& map)
{  
  _map = map;
  _numAffected = 0;

  // create feature crits for filtering
  ElementCriterionPtr wayToSnapCriterion =
    _createFeatureCriterion(_wayToSnapCriterionClassName, _snapWayStatus);
  ElementCriterionPtr wayToSnapToCriterion =
    _createFeatureCriterion(_wayToSnapToCriterionClassName, _snapToWayStatus);
  ElementCriterionPtr wayNodeToSnapToCriterion =
    _createFeatureCriterion(_wayNodeToSnapToCriterionClassName, _snapToWayStatus);

  // create needed geospatial indexes for surrounding feature searches
  _createFeatureIndex(
    wayNodeToSnapToCriterion, _snapToWayNodeIndex, _snapToWayNodeIndexToEid, ElementType::Node);
  _createFeatureIndex(
    wayToSnapToCriterion, _snapToWayIndex, _snapToWayIndexToEid, ElementType::Way);

  const WayMap ways = _map->getWays();   //copy
  LOG_VARD(ways.size());
  //boost::shared_ptr<NodeToWayMap> nodeToWayMap = _map->getIndex().getNodeToWayMap(); //copy
  //LOG_VARD(nodeToWayMap->size());

  long waysProcessed = 0;
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const ConstWayPtr& way = wayItr->second;
    LOG_VARD(way->getElementId());

    // for all ways passing the snap way crit
    if (wayToSnapCriterion->isSatisfied(way))
    {
      // find any endpoints on the way not connected to any other way
      const std::set<long> unconnectedWayEndNodeIds =
        _getUnconnectedWayEndNodeIds(way, wayToSnapCriterion);
      for (std::set<long>::const_iterator unconnectedWayEndNodeIdItr =
             unconnectedWayEndNodeIds.begin();
           unconnectedWayEndNodeIdItr != unconnectedWayEndNodeIds.end();
           ++unconnectedWayEndNodeIdItr)
      {
          const long unconnectedWayEndNodeId = *unconnectedWayEndNodeIdItr;
          LOG_VARD(_snappedWayNodes.contains(unconnectedWayEndNodeId));
          // don't snap the same node more than once
          if (!_snappedWayNodes.contains(unconnectedWayEndNodeId))
          {
            NodePtr unconnectedWayEndNode = map->getNode(unconnectedWayEndNodeId);
            assert(unconnectedWayEndNode->getId() == unconnectedWayEndNodeId);
            // try to find the nearest way node to snap to that is within the reuse distance, if
            // one was specified
            bool waySnapOccurred = false;
            if (_maxNodeReuseDistance > 0.0 && wayNodeToSnapToCriterion)
            {
              waySnapOccurred = _snapUnconnectedNodeToWayNode(unconnectedWayEndNode);
            }

            if (!waySnapOccurred)
            {
              // If no nearby way nodes were found, we're going to try to find nearby ways, then
              // snap into the closest location on those neighboring ways.
              /*waySnapOccurred =*/ _snapUnconnectedNodeToWay(unconnectedWayEndNode);
            }
          }
      }
    }

    if (_numAffected % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Snapped " << StringUtils::formatLargeNumber(_numAffected) << " way nodes.");
    }

    waysProcessed++;
    if (waysProcessed % (_taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "Processed " << StringUtils::formatLargeNumber(waysProcessed) << " / " <<
        StringUtils::formatLargeNumber(ways.size()) << " ways for way snapping.");
    }
  }
}

}
