
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
#include <hoot/core/util/MapProjector.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

// GEOS
#include <geos/geom/Coordinate.h>

#include <float.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, SnapUnconnectedWays)

SnapUnconnectedWays::SnapUnconnectedWays() :
_snapToExistingWayNodes(true),
_maxNodeReuseDistance(0.5),
_maxSnapDistance(5.0),
_snapToWayDiscretizationSpacing(1.0),
_addCeToSearchDistance(false),
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

  _snapToExistingWayNodes = ConfigOptions().getSnapUnconnectedWaysUseExistingWayNodes();
  if (_snapToExistingWayNodes)
  {
    _maxNodeReuseDistance = confOpts.getSnapUnconnectedWaysExistingWayNodeTolerance();
    if (_maxNodeReuseDistance <= 0.0)
    {
      throw IllegalArgumentException(
        "Invalid " + ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey() + " value: "
        + QString::number(_maxNodeReuseDistance) + ". The value must be greater than 0.0.");
    }
  }
  else
  {
    // Setting it to zero, if it wasn't specified, allows for skipping way node reuse completely.
    _maxNodeReuseDistance = 0.0;
  }
  _maxSnapDistance = confOpts.getSnapUnconnectedWaysSnapTolerance();
  if (_maxSnapDistance <= 0.0)
  {
    throw IllegalArgumentException(
      "Invalid " + ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey() + " value: "
      + QString::number(_maxSnapDistance) + ". The value must be greater than 0.0.");
  }
  _addCeToSearchDistance = confOpts.getSnapUnconnectedWaysAddCircularErrorToSearchRadius();
  _snapToWayDiscretizationSpacing  = confOpts.getSnapUnconnectedWaysDiscretizationSpacing();
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

void SnapUnconnectedWays::apply(OsmMapPtr& map)
{
  _map = map;
  _numAffected = 0;
  _snappedWayNodeIds.clear();

  // need to be in planar for this
  MapProjector::projectToPlanar(_map);

  // create feature crits for filtering what things we snap and snap to (will default to just
  // plain ways and way nodes)
  ElementCriterionPtr wayToSnapCrit =
    _createFeatureCriterion(_wayToSnapCriterionClassName, _snapWayStatus);
  ElementCriterionPtr wayToSnapToCrit =
    _createFeatureCriterion(_wayToSnapToCriterionClassName, _snapToWayStatus);
  ElementCriterionPtr wayNodeToSnapToCrit;
  if (_snapToExistingWayNodes)
  {
    wayNodeToSnapToCrit =
      _createFeatureCriterion(_wayNodeToSnapToCriterionClassName, _snapToWayStatus);
  }

  // create needed geospatial indexes for surrounding feature searches; if the way node reuse option
  // was turned off, then no need to index way nodes.
  if (wayNodeToSnapToCrit)
  {
    _createFeatureIndex(
      wayNodeToSnapToCrit, _snapToWayNodeIndex, _snapToWayNodeIndexToEid, ElementType::Node);
  }
  _createFeatureIndex(
    wayToSnapToCrit, _snapToWayIndex, _snapToWayIndexToEid, ElementType::Way);

  const WayMap ways = _map->getWays();
  LOG_VARD(ways.size());
  long waysProcessed = 0;
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const ConstWayPtr& wayToSnap = wayItr->second;
    LOG_VARD(wayToSnap->getElementId());

    // find unconnected endpoints on the way, if the way satisfies the specified crit
    const std::set<long> unconnectedEndNodeIds =
      _getUnconnectedEndNodeIds(wayToSnap, wayToSnapCrit);
    LOG_VARD(unconnectedEndNodeIds.size());
    for (std::set<long>::const_iterator unconnectedEndNodeIdItr = unconnectedEndNodeIds.begin();
         unconnectedEndNodeIdItr != unconnectedEndNodeIds.end(); ++unconnectedEndNodeIdItr)
    {
      // for each unconnected endpoint
      const long unconnectedEndNodeId = *unconnectedEndNodeIdItr;
      LOG_VARD(_snappedWayNodeIds.contains(unconnectedEndNodeId));
      // don't snap the same node more than once
      if (!_snappedWayNodeIds.contains(unconnectedEndNodeId))
      {
        NodePtr unconnectedEndNode = _map->getNode(unconnectedEndNodeId);

        // Try to find the nearest way node satisifying the specifying way node criteria that
        // is within the max reuse distance (if one was specified) and snap the unconnected way
        // node.  This reuse of existing way nodes on way being snapped to is done in order to cut
        // back on the number of new way nodes being added.
        bool snapOccurred = false;
        if (_snapToExistingWayNodes)
        {
          snapOccurred = _snapUnconnectedNodeToWayNode(unconnectedEndNode);
        }

        if (!snapOccurred)
        {
          // Otherwise if we weren't able to snap to a nearby way node, we're going to try to find
          // a nearby way and snap onto the closest location on it.
          /*snapOccurred =*/ _snapUnconnectedNodeToWay(unconnectedEndNode);
        }

        if (snapOccurred && _numAffected % _taskStatusUpdateInterval == 0)
        {
          PROGRESS_INFO(
            "Snapped " << StringUtils::formatLargeNumber(_numAffected) << " way nodes.");
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

ElementCriterionPtr SnapUnconnectedWays::_createFeatureCriterion(
  const QString criterionClassName, const Status& status)
{
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

void SnapUnconnectedWays::_createFeatureIndex(ElementCriterionPtr featureCrit,
                                              boost::shared_ptr<Tgs::HilbertRTree>& featureIndex,
                                              std::deque<ElementId>& featureIndexToEid,
                                              const ElementType& elementType)
{
  LOG_DEBUG("Creating feature index of type: " << elementType << "...");

  // TODO: tune these indexes? - see #3054
  boost::shared_ptr<Tgs::MemoryPageStore> mps(new Tgs::MemoryPageStore(728));
  featureIndex.reset(new Tgs::HilbertRTree(mps, 2));
  boost::shared_ptr<IndexElementsVisitor> spatialIndexer;
  if (elementType == ElementType::Node)
  {
    spatialIndexer.reset(
      new IndexElementsVisitor(
        featureIndex, featureIndexToEid, featureCrit,
        boost::bind(&SnapUnconnectedWays::_getWayNodeSearchRadius, this, _1), _map));
  }
  else
  {
    spatialIndexer.reset(
      new IndexElementsVisitor(
        featureIndex, featureIndexToEid, featureCrit,
        boost::bind(&SnapUnconnectedWays::_getWaySearchRadius, this, _1), _map));
  }
  LOG_DEBUG(spatialIndexer->getInitStatusMessage());
  if (elementType == ElementType::Node)
  {
    _map->visitNodesRo(*spatialIndexer);
  }
  else
  {
    _map->visitWaysRo(*spatialIndexer);
  }
  spatialIndexer->finalizeIndex();
  LOG_DEBUG(spatialIndexer->getCompletedStatusMessage());
  LOG_VARD(featureIndexToEid.size());
  LOG_VARD(_map->getIndex().getElementToRelationMap()->size());
}

std::set<long> SnapUnconnectedWays::_getUnconnectedEndNodeIds(const ConstWayPtr& way,
                                                           const ElementCriterionPtr& wayCrit) const
{
  LOG_DEBUG("Getting unconnected end nodes for: " << way->getElementId() << "...");
  std::set<long> unconnectedEndNodeIds;

  // get the way's nodes

  // grab the way's endpoints
  LOG_VARD(way->getNodeIds());
  const long firstEndNodeId = way->getFirstNodeId();
  LOG_VARD(firstEndNodeId);
  const long secondEndNodeId = way->getLastNodeId();
  LOG_VARD(secondEndNodeId);

  // filter the connected ways to each endpoint down by the feature crit
  const std::set<long> filteredWaysContainingFirstEndNode =
    OsmUtils::getContainingWayIdsByNodeId(firstEndNodeId, _map, wayCrit);
  LOG_VARD(filteredWaysContainingFirstEndNode);
  const std::set<long> filteredWaysContainingSecondEndNode =
    OsmUtils::getContainingWayIdsByNodeId(secondEndNodeId, _map, wayCrit);
  LOG_VARD(filteredWaysContainingSecondEndNode);

  // If only one way is connected to a node, then that node is an unconnected end way node.
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

Meters SnapUnconnectedWays::_getWaySearchRadius(const ConstElementPtr& e) const
{
  return _addCeToSearchDistance ? _maxSnapDistance + e->getCircularError() : _maxSnapDistance;
}

Meters SnapUnconnectedWays::_getWayNodeSearchRadius(const ConstElementPtr& e) const
{
  return
    _addCeToSearchDistance ? _maxNodeReuseDistance + e->getCircularError() : _maxNodeReuseDistance;
}

std::set<ElementId> SnapUnconnectedWays::_getNearbyFeaturesToSnapTo(
  const ConstNodePtr& node, const ElementType& elementType) const
{
  LOG_DEBUG("Retrieving nearby features to snap to for: " << node->getElementId() << "...");

  // The neighbors do not seem to be returned sorted by distance...hasn't seemed to matter in the
  // output quality so far but may need some attention later.

  boost::shared_ptr<geos::geom::Envelope> env(node->getEnvelope(_map));
  if (elementType == ElementType::Node)
  {
    env->expandBy(_getWayNodeSearchRadius(node));
    return
      IndexElementsVisitor::findNeighbors(
        *env, _snapToWayNodeIndex, _snapToWayNodeIndexToEid, _map);
  }
  else
  {
    env->expandBy(_getWaySearchRadius(node));
    return IndexElementsVisitor::findNeighbors(*env, _snapToWayIndex, _snapToWayIndexToEid, _map);
  }
}

int SnapUnconnectedWays::_getNodeToSnapWayInsertIndex(NodePtr nodeToSnap,
                                                      const ConstWayPtr& wayToSnapTo) const
{
  LOG_DEBUG(
    "Calculating way snap insert index for: " << nodeToSnap->getElementId() << " going into: " <<
    wayToSnapTo->getElementId() << "...");

  // TODO: Is there a better way to do all of this this using WayLocation?

  // find the closest way node on snap target way to our node being snapped
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
    // For ways of length > 2, try both the way node before and after the closest way node to see
    // which is closer to it.
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

    // determine the second closest way node to our node being snapped
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

  // Our way node snap index is either right after or before the closest way node.
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

  // Find all way nodes near the input way node that don't belong to the same way the input way
  // node belongs to.
  const std::set<ElementId> wayNodesToSnapTo =
    _getNearbyFeaturesToSnapTo(nodeToSnap, ElementType::Node);
  LOG_VARD(wayNodesToSnapTo);

  // For each way node neighbor, let's try snapping to the first one that we can.
  for (std::set<ElementId>::const_iterator wayNodesToSnapToItr = wayNodesToSnapTo.begin();
       wayNodesToSnapToItr != wayNodesToSnapTo.end(); ++wayNodesToSnapToItr)
  {
    const long wayNodeToSnapToId = (*wayNodesToSnapToItr).getId();
    LOG_VARD(wayNodeToSnapToId);

    // Don't try to snap a way node to itself.
    if (wayNodeToSnapToId != nodeToSnap->getId())
    {
      NodePtr wayNodeToSnapTo = _map->getNode(wayNodeToSnapToId);
      LOG_VARD(wayNodeToSnapTo->getId());

      // get all ways that a contain the neighbor
      const std::set<long>& waysContainingWayNodesToSnapTo =
        _map->getIndex().getNodeToWayMap()->getWaysByNode(wayNodeToSnapToId);
      LOG_VARD(waysContainingWayNodesToSnapTo);

      // get all ways that contain our input node
      // Note that these don't seem to be sorted by distance.
      const std::set<long>& waysContainingNodeToSnap =
        _map->getIndex().getNodeToWayMap()->getWaysByNode(nodeToSnap->getId());
      LOG_VARD(waysContainingNodeToSnap);

      // Check that there is no overlap between the two way groups, so that we don't try to snap
      // the input way node to a way its already on.
      std::set<long> commonNodesBetweenWayGroups;
      std::set_intersection(
        waysContainingNodeToSnap.begin(),
        waysContainingNodeToSnap.end(),
        waysContainingWayNodesToSnapTo.begin(),
        waysContainingWayNodesToSnapTo.end(),
        std::inserter(commonNodesBetweenWayGroups, commonNodesBetweenWayGroups.begin()));
      LOG_VARD(commonNodesBetweenWayGroups);
      if (commonNodesBetweenWayGroups.size() == 0 /*&&
          // I don't think this distance check is necessary...leaving here disabled for the time
          // being just in case. See similar check in _snapUnconnectedNodeToWay
          Distance::euclidean(wayNodeToSnapTo->toCoordinate(), nodeToSnap->toCoordinate()) <=
            _maxNodeReuseDistance*/)
      {
        // Snap the input way node to the nearest other way node we found.

        LOG_DEBUG(
          "Snapping way node: " << nodeToSnap->getId() << " to way node: " << wayNodeToSnapToId);

        // merge tags
        wayNodeToSnapTo->setTags(
          TagMergerFactory::mergeTags(
            wayNodeToSnapTo->getTags(), nodeToSnap->getTags(), ElementType::Node));
        // Add the optional custom tag for tracking purposes.
        if (!_snappedRoadsTagKey.isEmpty())
        {
          wayNodeToSnapTo->getTags().set(_snappedRoadsTagKey, "yes");
        }

        // Replace the snapped node with the node we snapped it to.
        ReplaceElementOp elementReplacer(
          nodeToSnap->getElementId(), wayNodeToSnapTo->getElementId(), true);
        elementReplacer.apply(_map);

        _snappedWayNodeIds.append(nodeToSnap->getId());
        _numAffected++;

        // Don't snap the node more than once.
        return true;
      }
    }
  }

  return false;
}

bool SnapUnconnectedWays::_snapUnconnectedNodeToWay(NodePtr nodeToSnap)
{
  LOG_DEBUG("Attempting to snap unconnected node: " << nodeToSnap->getId() << " to a way...");

  // get nearby ways
  const std::set<ElementId> waysToSnapTo =
    _getNearbyFeaturesToSnapTo(nodeToSnap, ElementType::Way);
  LOG_VARD(waysToSnapTo);
  for (std::set<ElementId>::const_iterator waysToSnapToItr = waysToSnapTo.begin();
       waysToSnapToItr != waysToSnapTo.end(); ++waysToSnapToItr)
  {
    // for each neighbor way
    WayPtr wayToSnapTo = _map->getWay((*waysToSnapToItr).getId());
    LOG_VARD(wayToSnapTo);
    // grab its way nodes
    const std::vector<long>& wayNodeIdsToSnapTo = wayToSnapTo->getNodeIds();
    LOG_VARD(wayNodeIdsToSnapTo);

    // Make sure the way we're trying to snap to doesn't already contain the node we're trying to
    // snap.
    const bool wayToSnapToContainsNodeToSnap =
      std::find(
        wayNodeIdsToSnapTo.begin(), wayNodeIdsToSnapTo.end(), nodeToSnap->getId()) !=
      wayNodeIdsToSnapTo.end();
    LOG_VARD(wayToSnapToContainsNodeToSnap);
    if (!wayToSnapToContainsNodeToSnap)
    {
      // Find the closest coord on the neighboring way to our input node by discretizing it.
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
        // Now, figure out where on the target way to insert our node being snapped.
        const int nodeToSnapInsertIndex = _getNodeToSnapWayInsertIndex(nodeToSnap, wayToSnapTo);
        LOG_VARD(nodeToSnapInsertIndex);

        // snap the node to the way

        if (Log::getInstance().getLevel() <= Log::Debug)
        {
          // This conversion is expensive, so do it during debugging only.
          const geos::geom::Coordinate wgs84Coord =
            MapProjector::project(
              closestWayToSnapToCoord, _map->getProjection(),
              MapProjector::createWgs84Projection());
          LOG_DEBUG(
            "Snapping way node: " << nodeToSnap->getId() << " to coord: " <<
            closestWayToSnapToCoord.toString() << " (wgs84: " << wgs84Coord.toString() <<
            ") and inserting at way node index: " << nodeToSnapInsertIndex);
        }
        // move the snapped node to the closest way coord
        nodeToSnap->setX(closestWayToSnapToCoord.x);
        nodeToSnap->setY(closestWayToSnapToCoord.y);
        // Add the optional custom tag for tracking purposes.
        if (!_snappedRoadsTagKey.isEmpty())
        {
          nodeToSnap->getTags().set(_snappedRoadsTagKey, "yes");
        }
        //_map->addNode(nodeToSnap);  // TODO: Is this necessary?
        _snappedWayNodeIds.append(nodeToSnap->getId());

        // add the snapped node as a way node on the target way
        QList<long> wayNodeIdsToSnapToList =
          QList<long>::fromVector(QVector<long>::fromStdVector(wayNodeIdsToSnapTo));
        LOG_VARD(wayNodeIdsToSnapToList);
        wayNodeIdsToSnapToList.insert(nodeToSnapInsertIndex, nodeToSnap->getId());
        LOG_VARD(wayNodeIdsToSnapToList);
        wayToSnapTo->setNodes(wayNodeIdsToSnapToList.toVector().toStdVector());
        LOG_VARD(wayToSnapTo->getNodeIds());

        _numAffected++;

        // Don't snap the node more than once.
        return true;
      }
    }
  }

  return false;
}

}
