
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
_snapToWayDiscretizationSpacing(0.1),
_snappedRoadsTagKey(""),
_wayToSnapToCriterionClassName("hoot::WayCriterion"),
_wayToSnapCriterionClassName("hoot::WayCriterion"),
_wayNodeToSnapToCriterionClassName("hoot::WayNodeCriterion"),
_snapWayStatus(Status::Unknown2),
_snapToWayStatus(Status::Unknown1),
_taskStatusUpdateInterval(1000)
{
  setConfiguration(conf());
}

void SnapUnconnectedWays::setConfiguration(const Settings& conf)
{
  ConfigOptions confOpts = ConfigOptions(conf);

  _taskStatusUpdateInterval = ConfigOptions().getTaskStatusUpdateInterval();

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
  _snapToWayDiscretizationSpacing  = confOpts.getSnapUnconnectedWaysWayDiscretizationSpacing();
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
    LOG_WARN(
      "No snap way criterion specified for the Unconnected Way Snapper.  " <<
      "Defaulting to: hoot::WayCriterion.");
    _wayToSnapCriterionClassName = "hoot::WayCriterion";
  }
  _wayToSnapToCriterionClassName = confOpts.getSnapUnconnectedWaysSnapToWayCriterion().trimmed();
  if (_wayToSnapToCriterionClassName.isEmpty())
  {
    LOG_WARN(
      "No snap to way criterion specified for the Unconnected Way Snapper.  " <<
      "Defaulting to: hoot::WayCriterion.");
    _wayToSnapToCriterionClassName = "hoot::WayCriterion";
  }
  if (_maxNodeReuseDistance != 0.0)
  {
    _wayNodeToSnapToCriterionClassName =
      confOpts.getSnapUnconnectedWaysSnapToWayNodeCriterion().trimmed();
    if (_wayNodeToSnapToCriterionClassName.isEmpty())
    {
      LOG_WARN(
        "No snap to way node criterion specified for the Unconnected Way Snapper.  " <<
        "Defaulting to: hoot::WayNodeCriterion.");
      _wayNodeToSnapToCriterionClassName = "hoot::WayNodeCriterion";
    }
  }

  _conf = conf;
}

ElementCriterionPtr SnapUnconnectedWays::_createFeatureCriterion(
  const QString criterionClassName, const Status& status)
{
  // TODO: We may want to force inheritiance from WayCriterion for snap to ways and from
  // WayNodeCriterion for snap to way nodes.

  const QString critClass = criterionClassName.trimmed();
  if (!critClass.isEmpty())
  {
    LOG_DEBUG(
      "Creating feature filtering criterion: " << criterionClassName << ", status: " <<
      status.toString() << "...");
    ElementCriterionPtr critTemp(
      Factory::getInstance().constructObject<ElementCriterion>(critClass));
    boost::shared_ptr<Configurable> configurable =
      boost::dynamic_pointer_cast<Configurable>(critTemp);
    if (configurable)
    {
      configurable->setConfiguration(_conf);
    }
    boost::shared_ptr<ConstOsmMapConsumer> mapConsumer =
      boost::dynamic_pointer_cast<ConstOsmMapConsumer>(critTemp);
    if (mapConsumer)
    {
      mapConsumer->setOsmMap(_map.get());
    }
    return
      boost::shared_ptr<ChainCriterion>(new ChainCriterion(new StatusCriterion(status), critTemp));
  }
  return ElementCriterionPtr();
}

Meters SnapUnconnectedWays::_getWaySearchRadius(const ConstElementPtr& /*e*/) const
{
  // Specifying the element as input here allows for optionally using its CE in the search radius
  // calculation.
  return _maxSnapDistance;
}

Meters SnapUnconnectedWays::_getWayNodeSearchRadius(const ConstElementPtr& /*e*/) const
{
  return _maxNodeReuseDistance;
}

void SnapUnconnectedWays::_createFeatureIndex(ElementCriterionPtr featureCrit,
                                              boost::shared_ptr<Tgs::HilbertRTree>& index,
                                              std::deque<ElementId>& indexToEid,
                                              const ElementType& elementType)
{
  LOG_DEBUG("Creating feature index of type: " << elementType << "...");

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
  LOG_VARD(_map->getIndex().getElementToRelationMap()->size());
}

std::set<long> SnapUnconnectedWays::_getUnconnectedEndNodeIds(const ConstWayPtr& way,
                                                           const ElementCriterionPtr& wayCrit) const
{
  LOG_DEBUG("Getting unconnected end nodes for: " << way->getElementId() << "...");
  std::set<long> unconnectedEndNodeIds;

  // get the way's nodes
  const std::vector<long>& wayNodeIds = way->getNodeIds();
  LOG_VARD(wayNodeIds);

  // grab the endpoints
  const long firstEndNodeId = wayNodeIds.at(0);
  LOG_VARD(firstEndNodeId);
  const long secondEndNodeId = wayNodeIds.at(wayNodeIds.size() - 1);
  LOG_VARD(secondEndNodeId);

  // filter the connected ways to each endpoint down by crit
  const std::set<long> filteredWaysContainingFirstEndNode =
    OsmUtils::getContainingWayIdsByNodeId(firstEndNodeId, _map, wayCrit);
  LOG_VARD(filteredWaysContainingFirstEndNode);
  const std::set<long> filteredWaysContainingSecondEndNode =
    OsmUtils::getContainingWayIdsByNodeId(secondEndNodeId, _map, wayCrit);
  LOG_VARD(filteredWaysContainingSecondEndNode);

  // If only one way is connected to a node, then then that node is an unconnected end way
  // node.
  if (filteredWaysContainingFirstEndNode.size() == 1)
  {
    unconnectedEndNodeIds.insert(firstEndNodeId);
  }
  if (filteredWaysContainingSecondEndNode.size() == 1)
  {
    unconnectedEndNodeIds.insert(secondEndNodeId);
  }
  LOG_VARD(unconnectedEndNodeIds);

  return unconnectedEndNodeIds;
}

std::set<ElementId> SnapUnconnectedWays::_getNearbyFeaturesToSnapTo(
  const ConstNodePtr& node, const ElementType& elementType) const
{
  LOG_DEBUG("Retrieving nearby features to snap to for: " << node->getElementId() << "...");

  boost::shared_ptr<geos::geom::Envelope> env(node->getEnvelope(_map));
  if (elementType == ElementType::Node)
  {
    env->expandBy(_getWayNodeSearchRadius(node));
    // TODO: These are not sorted by distance.
    return
      IndexElementsVisitor::findNeighbors(
        *env, _snapToWayNodeIndex, _snapToWayNodeIndexToEid, _map);
  }
  else
  {
    env->expandBy(_getWaySearchRadius(node));
    // TODO: These are not sorted by distance.
    return IndexElementsVisitor::findNeighbors(*env, _snapToWayIndex, _snapToWayIndexToEid, _map);
  }
}

int SnapUnconnectedWays::_getNodeToSnapWayInsertIndex(NodePtr nodeToSnap,
                                                      const ConstWayPtr& wayToSnapTo) const
{
  LOG_DEBUG(
    "Calculating way snap insert index for: " << nodeToSnap->getElementId() << " going into: " <<
    wayToSnapTo->getElementId() << "...");

  // Find the closest node to our new node on the snap to road.
  // TODO: Is there a better way to do this using WayLocation?
  const long closestWayNodeId = OsmUtils::closestWayNodeIdToNode(nodeToSnap, wayToSnapTo, _map);
  LOG_VARD(closestWayNodeId);

  const std::vector<long>& wayToSnapToNodeIds = wayToSnapTo->getNodeIds();
  LOG_VARD(wayToSnapToNodeIds);
  const int indexOfClosestWayNodeId = wayToSnapTo->getNodeIndex(closestWayNodeId);
  LOG_VARD(indexOfClosestWayNodeId);

  int indexOfSecondClosestWayNodeId = -1;
  if (indexOfClosestWayNodeId == 0)
  {
    assert(wayToSnapToNodeIds.size() >= 2);
    indexOfSecondClosestWayNodeId = 1;
  }
  else if (indexOfClosestWayNodeId == (int)(wayToSnapToNodeIds.size() - 1))
  {
    assert(wayToSnapToNodeIds.size() >= 2);
    indexOfSecondClosestWayNodeId = wayToSnapToNodeIds.size() - 2;
  }
  else
  {
    // try both the one before and after the closest node to see which is closer
    const int wayNodeBeforeIndex = indexOfClosestWayNodeId - 1;
    LOG_VARD(wayNodeBeforeIndex);
    const long wayNodeBeforeId = wayToSnapTo->getNodeId(wayNodeBeforeIndex);
    LOG_VARD(wayNodeBeforeId);
    const int wayNodeAfterIndex = indexOfClosestWayNodeId + 1;
    LOG_VARD(wayNodeAfterIndex);
    const long wayNodeAfterId = wayToSnapTo->getNodeId(wayNodeAfterIndex);
    LOG_VARD(wayNodeAfterId);
    assert(map->containsNode(wayNodeBeforeId));
    assert(map->containsNode(wayNodeAfterId));

    const double distFromBeforeWayNodeToNodeToSnap =
      Distance::euclidean(
        _map->getNode(wayNodeBeforeId)->toCoordinate(), nodeToSnap->toCoordinate());
    const double distFromAfterWayNodeToNodeToSnap =
      Distance::euclidean(
        _map->getNode(wayNodeAfterId)->toCoordinate(), nodeToSnap->toCoordinate());
    if (distFromBeforeWayNodeToNodeToSnap < distFromAfterWayNodeToNodeToSnap)
    {
      indexOfSecondClosestWayNodeId = wayNodeBeforeIndex;
    }
    else
    {
      indexOfSecondClosestWayNodeId = wayNodeAfterIndex;
    }
  }
  LOG_VARD(indexOfSecondClosestWayNodeId);

  int nodeToSnapInsertIndex = -1;
  assert(indexOfSecondClosestWayNodeId != indexOfClosestWayNodeId);
  if (indexOfSecondClosestWayNodeId > indexOfClosestWayNodeId)
  {
    nodeToSnapInsertIndex = indexOfClosestWayNodeId + 1;
  }
  else
  {
    nodeToSnapInsertIndex = indexOfClosestWayNodeId - 1;
  }

  return nodeToSnapInsertIndex;
}

bool SnapUnconnectedWays::_snapUnconnectedNodeToWayNode(NodePtr nodeToSnap)
{
  LOG_DEBUG("Attempting to snap unconnected node: " << nodeToSnap->getId() << " to a way node...");

  // find all way nodes near this unconnected node that don't belong to the same way
  // the unconnected way node is already on
  const std::set<ElementId> wayNodesToSnapTo =
    _getNearbyFeaturesToSnapTo(nodeToSnap, ElementType::Node);
  LOG_VARD(wayNodesToSnapTo);
  if (wayNodesToSnapTo.size() > 0)
  {
    // If there were any way node neighbors, let's try snapping to the first one that
    // we can. This avoids creating any unnecessary new way nodes on the way being
    // snapped to.
    for (std::set<ElementId>::const_iterator wayNodesToSnapToItr = wayNodesToSnapTo.begin();
         wayNodesToSnapToItr != wayNodesToSnapTo.end(); ++wayNodesToSnapToItr)
    {
      const long wayNodeToSnapToId = (*wayNodesToSnapToItr).getId();
      LOG_VARD(wayNodeToSnapToId);

      // Don't try to snap a way node to itself.
      if (wayNodeToSnapToId != nodeToSnap->getId())
      {
        const std::set<long> waysContainingWayNodesToSnapTo =
          /*nodeToWayMap*/_map->getIndex().getNodeToWayMap()->getWaysByNode(wayNodeToSnapToId);
        LOG_VARD(waysContainingWayNodesToSnapTo);
        // TODO: also check way for the correct status here?

        // TODO: These aren't sorted by distance.
        const std::set<long>& waysContainingNodeToSnap =
          /*nodeToWayMap*/_map->getIndex().getNodeToWayMap()->getWaysByNode(nodeToSnap->getId());
        LOG_VARD(waysContainingNodeToSnap);

        std::set<long> commonNodesBetweenWayGroups;
        std::set_intersection(
          waysContainingNodeToSnap.begin(),
          waysContainingNodeToSnap.end(),
          waysContainingWayNodesToSnapTo.begin(),
          waysContainingWayNodesToSnapTo.end(),
          std::inserter(commonNodesBetweenWayGroups, commonNodesBetweenWayGroups.begin()));
        LOG_VARD(commonNodesBetweenWayGroups);
        // Don't try to snap a way to itself.
        if (commonNodesBetweenWayGroups.size() == 0)
        {
          LOG_DEBUG(
            "Snapping way node: " << nodeToSnap->getId() << " to way node: " << wayNodeToSnapToId);

          // snap the unconnected road node to the nearest road node
          NodePtr wayNodeToSnapTo = _map->getNode(wayNodeToSnapToId);
          LOG_VARD(wayNodeToSnapTo->getId());
          LOG_VARD(_snappedToWayNodeIds.contains(wayNodeToSnapTo->getId()));

          const Tags mergedTags =
            TagMergerFactory::mergeTags(
              wayNodeToSnapTo->getTags(), nodeToSnap->getTags(), ElementType::Node);
          wayNodeToSnapTo->setTags(mergedTags);
          if (!_snappedRoadsTagKey.isEmpty())
          {
            wayNodeToSnapTo->getTags().set(_snappedRoadsTagKey, "yes");
          }

          ReplaceElementOp elementReplacer(
            nodeToSnap->getElementId(), wayNodeToSnapTo->getElementId(), true);
          elementReplacer.apply(_map);

          _snappedToWayNodeIds.append(wayNodeToSnapTo->getId());
          _snappedWayNodeIds.append(nodeToSnap->getId());
          _numAffected++;

          return true;
        }
      }
    }
  }

  return false;
}

bool SnapUnconnectedWays::_snapUnconnectedNodeToWay(NodePtr nodeToSnap)
{
  LOG_DEBUG("Attempting to snap unconnected node: " << nodeToSnap->getId() << " to a way...");

  const std::set<ElementId> waysToSnapTo =
    _getNearbyFeaturesToSnapTo(nodeToSnap, ElementType::Way);
  LOG_VARD(waysToSnapTo);
  for (std::set<ElementId>::const_iterator waysToSnapToItr = waysToSnapTo.begin();
       waysToSnapToItr != waysToSnapTo.end(); ++waysToSnapToItr)
  {
    // We're arbitrarily snapping to the first neighbor road (for now?).

    WayPtr wayToSnapTo = _map->getWay((*waysToSnapToItr).getId());
    LOG_VARD(wayToSnapTo);
    const std::vector<long>& wayNodeIdsToSnapTo = wayToSnapTo->getNodeIds();
    LOG_VARD(wayNodeIdsToSnapTo);

    const bool wayToSnapToContainsNodeToSnap =
      std::find(
        wayNodeIdsToSnapTo.begin(), wayNodeIdsToSnapTo.end(), nodeToSnap->getId()) !=
      wayNodeIdsToSnapTo.end();
    LOG_VARD(wayToSnapToContainsNodeToSnap);
    if (!wayToSnapToContainsNodeToSnap)
    {
      double shortestDistanceFromNodeToSnapToWayCoord = DBL_MAX;
      const geos::geom::Coordinate closestWayToSnapToCoord =
        OsmUtils::closestWayCoordToNode(nodeToSnap, wayToSnapTo,
          shortestDistanceFromNodeToSnapToWayCoord, _snapToWayDiscretizationSpacing, _map);
      LOG_VARD(closestWayToSnapToCoord);
      LOG_VARD(shortestDistanceFromNodeToSnapToWayCoord);

      // This check of less than the allowed snap distance should not be necessary.  For
      // some reason I don't understand yet, neighbors are being returned at longer
      // distances than they should be.
      if (/*shortestDistance != DBL_MAX &&*/
          shortestDistanceFromNodeToSnapToWayCoord <= _maxSnapDistance)
      {
        // Now, figure out if the way node after the closest node is closer to our new node or if
        // the way node before it is closer.  That tells us which two existing way nodes to stick
        // our new way node on the way being snapped to.
        const int nodeToSnapInsertIndex = _getNodeToSnapWayInsertIndex(nodeToSnap, wayToSnapTo);
        LOG_VARD(nodeToSnapInsertIndex);

        LOG_DEBUG(
          "Snapping way node: " << nodeToSnap->getId() << " to coord: " <<
          closestWayToSnapToCoord.toString() << " and inserting at way node index: " <<
          nodeToSnapInsertIndex);
        nodeToSnap->setX(closestWayToSnapToCoord.x);
        nodeToSnap->setY(closestWayToSnapToCoord.y);
        if (!_snappedRoadsTagKey.isEmpty())
        {
          nodeToSnap->getTags().set(_snappedRoadsTagKey, "yes");
        }
        _map->addNode(nodeToSnap);
        _snappedWayNodeIds.append(nodeToSnap->getId());

        // Add the new snapped node as a way node on the snapped to road.;
        QList<long> wayNodeIdsToSnapToList =
          QList<long>::fromVector(QVector<long>::fromStdVector(wayNodeIdsToSnapTo));
        LOG_VARD(wayNodeIdsToSnapToList);
        wayNodeIdsToSnapToList.insert(nodeToSnapInsertIndex, nodeToSnap->getId());
        LOG_VARD(wayNodeIdsToSnapToList);
        wayToSnapTo->setNodes(wayNodeIdsToSnapToList.toVector().toStdVector());
        LOG_VARD(wayToSnapTo->getNodeIds());

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
  _snappedWayNodeIds.clear();
  _snappedToWayNodeIds.clear();

  // create feature crits for filtering what we snap and snap to
  ElementCriterionPtr wayToSnapCrit =
    _createFeatureCriterion(_wayToSnapCriterionClassName, _snapWayStatus);
  ElementCriterionPtr wayToSnapToCrit =
    _createFeatureCriterion(_wayToSnapToCriterionClassName, _snapToWayStatus);
  ElementCriterionPtr wayNodeToSnapToCrit;
  if (_maxNodeReuseDistance != 0.0)
  {
    wayNodeToSnapToCrit =
      _createFeatureCriterion(_wayNodeToSnapToCriterionClassName, _snapToWayStatus);
  }

  // create needed geospatial indexes for surrounding feature searches
  if (wayNodeToSnapToCrit)
  {
    _createFeatureIndex(
      wayNodeToSnapToCrit, _snapToWayNodeIndex, _snapToWayNodeIndexToEid, ElementType::Node);
  }
  _createFeatureIndex(
    wayToSnapToCrit, _snapToWayIndex, _snapToWayIndexToEid, ElementType::Way);

  const WayMap ways = _map->getWays();   //copy
  LOG_VARD(ways.size());
  long waysProcessed = 0;
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const ConstWayPtr& wayToSnap = wayItr->second;
    LOG_VARD(wayToSnap->getElementId());

    // for all ways passing the way to snap crit
    //TODO: is this check needed since its done in _getUnconnectedEndNodeIds?
    if (wayToSnapCrit->isSatisfied(wayToSnap))
    {
      // Find any endpoints on the way not connected to any other way.
      const std::set<long> unconnectedEndNodeIds =
        _getUnconnectedEndNodeIds(wayToSnap, wayToSnapCrit);
      LOG_VARD(unconnectedEndNodeIds.size());
      for (std::set<long>::const_iterator unconnectedEndNodeIdItr = unconnectedEndNodeIds.begin();
           unconnectedEndNodeIdItr != unconnectedEndNodeIds.end(); ++unconnectedEndNodeIdItr)
      {
        const long unconnectedEndNodeId = *unconnectedEndNodeIdItr;
        LOG_VARD(_snappedWayNodeIds.contains(unconnectedEndNodeId));
        // don't snap the same node more than once
        if (!_snappedWayNodeIds.contains(unconnectedEndNodeId))
        {
          NodePtr unconnectedEndNode = _map->getNode(unconnectedEndNodeId);
          // Try to find the nearest way node satisifying the specifying way node criteria that
          // is within the reuse distance, if one was specified, and snap to it in order to cut
          // back on the number of new way nodes being added to way being snapped to.
          bool snapOccurred = false;
          if (_maxNodeReuseDistance > 0.0 && wayNodeToSnapToCrit)
          {
            snapOccurred = _snapUnconnectedNodeToWayNode(unconnectedEndNode);
          }

          if (!snapOccurred)
          {
            // Otherwise if no nearby way nodes were found to snap to, we're going to try to find
            // a nearby way and snap into the closest location on it.
            /*snapOccurred =*/ _snapUnconnectedNodeToWay(unconnectedEndNode);
          }

          if (snapOccurred && _numAffected % _taskStatusUpdateInterval == 0)
          {
            PROGRESS_INFO(
              "Snapped " << StringUtils::formatLargeNumber(_numAffected) << " way nodes.");
          }
        }
      }
    }

    waysProcessed++;
    if (waysProcessed % (_taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "Processed " << StringUtils::formatLargeNumber(waysProcessed) << " / " <<
        StringUtils::formatLargeNumber(ways.size()) << " ways for unconnected snapping.");
    }
  }
}

}
