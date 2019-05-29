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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "UnconnectedWaySnapper.h"

// hoot
#include <hoot/core/algorithms/Distance.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/IndexElementsVisitor.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

// GEOS
#include <geos/geom/Coordinate.h>

// Standard
#include <cfloat>
#include <functional>
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, UnconnectedWaySnapper)

UnconnectedWaySnapper::UnconnectedWaySnapper() :
_snapToExistingWayNodes(true),
_maxNodeReuseDistance(0.5),
_maxSnapDistance(5.0),
_snapToWayDiscretizationSpacing(1.0),
_addCeToSearchDistance(false),
_markSnappedNodes(false),
_wayToSnapToCriterionClassName("hoot::WayCriterion"),
_wayToSnapCriterionClassName("hoot::WayCriterion"),
_wayNodeToSnapToCriterionClassName("hoot::WayNodeCriterion"),
_snapWayStatus(Status::Unknown2),
_snapToWayStatus(Status::Unknown1),
_numSnappedToWays(0),
_numSnappedToWayNodes(0),
_taskStatusUpdateInterval(1000)
{
}

void UnconnectedWaySnapper::setConfiguration(const Settings& conf)
{
  ConfigOptions confOpts = ConfigOptions(conf);

  _taskStatusUpdateInterval = confOpts.getTaskStatusUpdateInterval();

  setSnapToExistingWayNodes(confOpts.getSnapUnconnectedWaysUseExistingWayNodes());
  if (_snapToExistingWayNodes)
  {
    setMaxNodeReuseDistance(confOpts.getSnapUnconnectedWaysExistingWayNodeTolerance());
  }
  else
  {
    // Setting it to zero, if it wasn't specified, allows for skipping way node reuse completely.
    _maxNodeReuseDistance = 0.0;
  }
  setMaxSnapDistance(confOpts.getSnapUnconnectedWaysSnapTolerance());
  setAddCeToSearchDistance(confOpts.getSnapUnconnectedWaysAddCircularErrorToSearchRadius());
  setWayDiscretizationSpacing(confOpts.getSnapUnconnectedWaysDiscretizationSpacing());
  setMarkSnappedNodes(confOpts.getSnapUnconnectedWaysMarkSnappedNodes());

  setSnapWayStatus(Status::fromString(confOpts.getSnapUnconnectedWaysSnapWayStatus()));
  setSnapToWayStatus(Status::fromString(confOpts.getSnapUnconnectedWaysSnapToWayStatus()));

  setWayToSnapCriterionClassName(confOpts.getSnapUnconnectedWaysSnapWayCriterion().trimmed());
  setWayToSnapToCriterionClassName(confOpts.getSnapUnconnectedWaysSnapToWayCriterion().trimmed());
  if (_snapToExistingWayNodes)
  {
    setWayNodeToSnapToCriterionClassName(
      confOpts.getSnapUnconnectedWaysSnapToWayNodeCriterion().trimmed());
  }

  _conf = conf;
}

void UnconnectedWaySnapper::setMaxNodeReuseDistance(double distance)
{
  if (distance <= 0.0)
  {
    throw IllegalArgumentException(
      "Invalid " + ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey() + " value: "
      + QString::number(distance) + ". The value must be greater than 0.0.");
  }
  _maxNodeReuseDistance = distance;
}
void UnconnectedWaySnapper::setMaxSnapDistance(double distance)
{
  if (distance <= 0.0)
  {
    throw IllegalArgumentException(
      "Invalid " + ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey() + " value: "
      + QString::number(distance) + ". The value must be greater than 0.0.");
  }
  _maxSnapDistance = distance;
}
void UnconnectedWaySnapper::setWayDiscretizationSpacing(double spacing)
{
  if (spacing <= 0.0)
  {
    throw IllegalArgumentException(
      "Invalid " + ConfigOptions::getSnapUnconnectedWaysDiscretizationSpacingKey() + " value: "
      + QString::number(spacing) + ". The value must be greater than 0.0.");
  }
  _snapToWayDiscretizationSpacing = spacing;
}

void UnconnectedWaySnapper::setWayToSnapToCriterionClassName(const QString& name)
{
  if (name.trimmed().isEmpty())
  {
    LOG_WARN(
      "No snap to way criterion specified for the Unconnected Way Snapper.  " <<
      "Defaulting to: hoot::WayCriterion.");
    _wayToSnapToCriterionClassName =
      ConfigOptions::getSnapUnconnectedWaysSnapToWayCriterionDefaultValue();
  }
  else
  {
    _wayToSnapToCriterionClassName = name.trimmed();
  }
}
void UnconnectedWaySnapper::setWayToSnapCriterionClassName(const QString& name)
{
  if (name.trimmed().isEmpty())
  {
    LOG_WARN(
      "No snap way criterion specified for the Unconnected Way Snapper.  " <<
      "Defaulting to: hoot::WayCriterion.");
    _wayToSnapCriterionClassName =
      ConfigOptions::getSnapUnconnectedWaysSnapWayCriterionDefaultValue();
  }
  else
  {
    _wayToSnapCriterionClassName = name.trimmed();
  }
}
void UnconnectedWaySnapper::setWayNodeToSnapToCriterionClassName(const QString& name)
{
  if (name.trimmed().isEmpty())
  {
    LOG_WARN(
      "No snap to way node criterion specified for the Unconnected Way Snapper.  " <<
      "Defaulting to: hoot::WayNodeCriterion.");
    _wayNodeToSnapToCriterionClassName =
      ConfigOptions::getSnapUnconnectedWaysSnapToWayNodeCriterionDefaultValue();
  }
  else
  {
    _wayNodeToSnapToCriterionClassName = name.trimmed();
  }
}

void UnconnectedWaySnapper::apply(OsmMapPtr& map)
{
  _map = map;
  _numAffected = 0;
  _numSnappedToWays = 0;
  _numSnappedToWayNodes = 0;
  _snappedWayNodeIds.clear();

  // need to be in planar for all of this
  MapProjector::projectToPlanar(_map);

  // create feature crits for filtering what things we snap and snap to (will default to just
  // plain ways and way nodes if nothing was specified)
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
  LOG_VART(ways.size());
  long waysProcessed = 0;
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const ConstWayPtr& wayToSnap = wayItr->second;
    LOG_VART(wayToSnap->getElementId());

    // find unconnected endpoints on the way, if the way satisfies the specified crit
    const std::set<long> unconnectedEndNodeIds =
      _getUnconnectedEndNodeIds(wayToSnap, wayToSnapCrit);
    for (std::set<long>::const_iterator unconnectedEndNodeIdItr = unconnectedEndNodeIds.begin();
         unconnectedEndNodeIdItr != unconnectedEndNodeIds.end(); ++unconnectedEndNodeIdItr)
    {
      // for each unconnected endpoint
      const long unconnectedEndNodeId = *unconnectedEndNodeIdItr;
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
          // If we weren't able to snap to a nearby way node or the snapping directly to way nodes
          // option was turned off, we're going to try to find a nearby way and snap onto the
          // closest location on it.
          /*snapOccurred =*/ _snapUnconnectedNodeToWay(unconnectedEndNode);
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

  LOG_DEBUG(_numSnappedToWays << " ways snapped to the closest point on other ways");
  LOG_DEBUG(_numSnappedToWayNodes << " ways snapped directly to way nodes");
}

ElementCriterionPtr UnconnectedWaySnapper::_createFeatureCriterion(const QString& criterionClassName, const Status& status)
{
  const QString critClass = criterionClassName.trimmed();
  if (!critClass.isEmpty())
  {
    LOG_TRACE(
      "Creating feature filtering criterion: " << criterionClassName << ", status: " <<
      status.toString() << "...");
    ElementCriterionPtr critTemp(
      Factory::getInstance().constructObject<ElementCriterion>(critClass));
    std::shared_ptr<Configurable> configurable =
      std::dynamic_pointer_cast<Configurable>(critTemp);
    if (configurable)
    {
      configurable->setConfiguration(_conf);
    }
    std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
      std::dynamic_pointer_cast<ConstOsmMapConsumer>(critTemp);
    if (mapConsumer)
    {
      mapConsumer->setOsmMap(_map.get());
    }
    return
      std::shared_ptr<ChainCriterion>(new ChainCriterion(new StatusCriterion(status), critTemp));
  }
  return ElementCriterionPtr();
}

void UnconnectedWaySnapper::_createFeatureIndex(const ElementCriterionPtr& featureCrit,
                                                std::shared_ptr<Tgs::HilbertRTree>& featureIndex,
                                                std::deque<ElementId>& featureIndexToEid,
                                                const ElementType& elementType)
{
  LOG_TRACE("Creating feature index of type: " << elementType << "...");

  // TODO: tune these indexes? - see #3054
  std::shared_ptr<Tgs::MemoryPageStore> mps(new Tgs::MemoryPageStore(728));
  featureIndex.reset(new Tgs::HilbertRTree(mps, 2));
  std::shared_ptr<IndexElementsVisitor> spatialIndexer;
  if (elementType == ElementType::Node)
  {
    spatialIndexer.reset(
      new IndexElementsVisitor(
        featureIndex, featureIndexToEid, featureCrit,
        std::bind(&UnconnectedWaySnapper::_getWayNodeSearchRadius, this, placeholders::_1), _map));
  }
  else
  {
    spatialIndexer.reset(
      new IndexElementsVisitor(
        featureIndex, featureIndexToEid, featureCrit,
        std::bind(&UnconnectedWaySnapper::_getWaySearchRadius, this, placeholders::_1), _map));
  }
  LOG_TRACE(spatialIndexer->getInitStatusMessage());
  if (elementType == ElementType::Node)
  {
    _map->visitNodesRo(*spatialIndexer);
  }
  else
  {
    _map->visitWaysRo(*spatialIndexer);
  }
  spatialIndexer->finalizeIndex();
  LOG_TRACE(spatialIndexer->getCompletedStatusMessage());
  LOG_VART(featureIndexToEid.size());
  LOG_VART(_map->getIndex().getElementToRelationMap()->size());
}

std::set<long> UnconnectedWaySnapper::_getUnconnectedEndNodeIds(
  const ConstWayPtr& way, const ElementCriterionPtr& wayCrit) const
{
  LOG_TRACE("Getting unconnected end nodes for: " << way->getElementId() << "...");
  std::set<long> unconnectedEndNodeIds;

  // grab the way's endpoints
  const long firstEndNodeId = way->getFirstNodeId();
  const long secondEndNodeId = way->getLastNodeId();
  LOG_VART(firstEndNodeId);
  LOG_VART(secondEndNodeId);

  // filter all the ways containing each endpoint down by the feature crit
  const std::set<long> filteredWaysContainingFirstEndNode =
    OsmUtils::getContainingWayIdsByNodeId(firstEndNodeId, _map, wayCrit);
  const std::set<long> filteredWaysContainingSecondEndNode =
    OsmUtils::getContainingWayIdsByNodeId(secondEndNodeId, _map, wayCrit);

  // If only one way is connected to the end node, then we'll call that node an unconnected end
  // way node.
  if (filteredWaysContainingFirstEndNode.size() == 1)
  {
    unconnectedEndNodeIds.insert(firstEndNodeId);
  }
  if (filteredWaysContainingSecondEndNode.size() == 1)
  {
    unconnectedEndNodeIds.insert(secondEndNodeId);
  }
  LOG_VART(unconnectedEndNodeIds);

  return unconnectedEndNodeIds;
}

Meters UnconnectedWaySnapper::_getWaySearchRadius(const ConstElementPtr& e) const
{
  return _addCeToSearchDistance ? _maxSnapDistance + e->getCircularError() : _maxSnapDistance;
}

Meters UnconnectedWaySnapper::_getWayNodeSearchRadius(const ConstElementPtr& e) const
{
  return
    _addCeToSearchDistance ? _maxNodeReuseDistance + e->getCircularError() : _maxNodeReuseDistance;
}

std::set<ElementId> UnconnectedWaySnapper::_getNearbyFeaturesToSnapTo(
  const ConstNodePtr& node, const ElementType& elementType) const
{
  LOG_TRACE("Retrieving nearby features to snap to for: " << node->getElementId() << "...");

  // The neighbors do not seem to be returned sorted by distance...hasn't seemed to matter in the
  // output quality so far but may need some attention later.

  std::set<ElementId> neighborIds;
  std::shared_ptr<geos::geom::Envelope> env(node->getEnvelope(_map));
  if (elementType == ElementType::Node)
  {
    env->expandBy(_getWayNodeSearchRadius(node));
    neighborIds =
      IndexElementsVisitor::findNeighbors(
        *env, _snapToWayNodeIndex, _snapToWayNodeIndexToEid, _map);
  }
  else
  {
    env->expandBy(_getWaySearchRadius(node));
    neighborIds =
      IndexElementsVisitor::findNeighbors(*env, _snapToWayIndex, _snapToWayIndexToEid, _map);
  }
  LOG_VART(neighborIds);
  return neighborIds;
}

int UnconnectedWaySnapper::_getNodeToSnapWayInsertIndex(const NodePtr& nodeToSnap, const ConstWayPtr& wayToSnapTo) const
{
  LOG_TRACE(
    "Calculating way snap insert index for: " << nodeToSnap->getElementId() << " going into: " <<
    wayToSnapTo->getElementId() << "...");

  // find the closest way node on snap target way to our node being snapped
  const std::vector<long>& wayToSnapToNodeIds = wayToSnapTo->getNodeIds();
  const int indexOfClosestWayNodeId =
    wayToSnapTo->getNodeIndex(OsmUtils::closestWayNodeIdToNode(nodeToSnap, wayToSnapTo, _map));
  LOG_VART(wayToSnapToNodeIds);
  LOG_VART(indexOfClosestWayNodeId);

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
    const long wayNodeBeforeId = wayToSnapTo->getNodeId(wayNodeBeforeIndex);
    const int wayNodeAfterIndex = indexOfClosestWayNodeId + 1;
    const long wayNodeAfterId = wayToSnapTo->getNodeId(wayNodeAfterIndex);
    LOG_VART(wayNodeBeforeIndex);
    LOG_VART(wayNodeBeforeId);
    LOG_VART(wayNodeAfterIndex);
    LOG_VART(wayNodeAfterId);

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
  LOG_VART(indexOfSecondClosestWayNodeId);

  //  Our way node snap index either takes the place of the nearest, pushing it to the right, or
  //  it is right after the closest way node.
  int nodeToSnapInsertIndex = indexOfClosestWayNodeId;
  assert(indexOfSecondClosestWayNodeId != indexOfClosestWayNodeId);
  if (indexOfSecondClosestWayNodeId > indexOfClosestWayNodeId)
  {
    nodeToSnapInsertIndex++;
  }

  LOG_VART(nodeToSnapInsertIndex);
  return nodeToSnapInsertIndex;
}

bool UnconnectedWaySnapper::_snapUnconnectedNodeToWayNode(const NodePtr& nodeToSnap)
{
  LOG_TRACE("Attempting to snap unconnected node: " << nodeToSnap->getId() << " to a way node...");

  // Find all way nodes near the input way node that don't belong to the same way the input way
  // node belongs to.
  const std::set<ElementId> wayNodesToSnapTo =
    _getNearbyFeaturesToSnapTo(nodeToSnap, ElementType::Node);
  // For each way node neighbor, let's try snapping to the first one that we can.
  for (std::set<ElementId>::const_iterator wayNodesToSnapToItr = wayNodesToSnapTo.begin();
       wayNodesToSnapToItr != wayNodesToSnapTo.end(); ++wayNodesToSnapToItr)
  {
    const long wayNodeToSnapToId = (*wayNodesToSnapToItr).getId();
    LOG_VART(wayNodeToSnapToId);

    // Don't try to snap a way node to itself.
    if (wayNodeToSnapToId != nodeToSnap->getId())
    {
      NodePtr wayNodeToSnapTo = _map->getNode(wayNodeToSnapToId);
      LOG_VART(wayNodeToSnapTo->getId())

      // Compare all the ways that a contain the neighbor and all the ways that contain our input
      // node.  If there's overlap, then we pass b/c we don't want to try to snap the input way
      // node to a way its already on.
      if (!OsmUtils::nodesAreContainedByTheSameWay(wayNodeToSnapToId, nodeToSnap->getId(), _map) /*&&
          // I don't think this distance check is necessary...leaving here disabled for the time
          // being just in case. See similar check in _snapUnconnectedNodeToWay
          Distance::euclidean(wayNodeToSnapTo->toCoordinate(), nodeToSnap->toCoordinate()) <=
            _maxNodeReuseDistance*/)
      {
        // Snap the input way node to the nearest way node neighbor we found.

        LOG_TRACE(
          "Snapping way node: " << nodeToSnap->getId() << " to way node: " << wayNodeToSnapToId);

        // merge the tags
        wayNodeToSnapTo->setTags(
          TagMergerFactory::mergeTags(
            wayNodeToSnapTo->getTags(), nodeToSnap->getTags(), ElementType::Node));
        // Add the optional custom tag for tracking purposes.
        if (_markSnappedNodes)
        {
          wayNodeToSnapTo->getTags().set(MetadataTags::HootSnappedWayNode(), "snapped_to_way_node");
        }

        // Replace the snapped node with the node we snapped it to.
        ReplaceElementOp elementReplacer(
          nodeToSnap->getElementId(), wayNodeToSnapTo->getElementId(), true);
        elementReplacer.apply(_map);

        _snappedWayNodeIds.append(nodeToSnap->getId());
        _numAffected++;
        _numSnappedToWayNodes++;

        // Don't snap the node more than once.
        return true;
      }
    }
  }

  return false;
}

bool UnconnectedWaySnapper::_snapUnconnectedNodeToWay(const NodePtr& nodeToSnap)
{
  LOG_TRACE("Attempting to snap unconnected node: " << nodeToSnap->getId() << " to a way...");

  // get nearby ways
  const std::set<ElementId> waysToSnapTo =
    _getNearbyFeaturesToSnapTo(nodeToSnap, ElementType::Way);
  for (std::set<ElementId>::const_iterator waysToSnapToItr = waysToSnapTo.begin();
       waysToSnapToItr != waysToSnapTo.end(); ++waysToSnapToItr)
  {
    // for each neighboring way
    WayPtr wayToSnapTo = _map->getWay((*waysToSnapToItr).getId());
    LOG_VART(wayToSnapTo);
    if (_snapUnconnectedNodeToWay(nodeToSnap, wayToSnapTo))
    {
      _snappedWayNodeIds.append(nodeToSnap->getId());
      _numAffected++;
      _numSnappedToWays++;

      // Don't snap the node more than once.
      return true;
    }
  }

  return false;
}

bool UnconnectedWaySnapper::snapClosestEndpointToWay(OsmMapPtr map, const WayPtr& disconnected, const WayPtr& connectTo)
{
  //  Create object for static call
  UnconnectedWaySnapper uws;
  uws._map = map;
  //  Call protected function to snap closest endpoint to way
  return uws._snapClosestEndpointToWay(disconnected, connectTo);
}

bool UnconnectedWaySnapper::_snapClosestEndpointToWay(const WayPtr& disconnected, const WayPtr& connectTo)
{
  //  Validate the parameters
  if (!disconnected || !connectTo || disconnected->getId() == connectTo->getId())
    return false;
  //  Get the endpoint closest to the way to connect to it
  const std::vector<long> nodeIds = disconnected->getNodeIds();
  ElementConverter converter(_map);
  std::shared_ptr<geos::geom::Geometry> geometry = converter.convertToGeometry(connectTo);

  NodePtr endpoint1 = _map->getNode(nodeIds[0]);
  std::shared_ptr<geos::geom::Geometry> ep1 = converter.convertToGeometry(ConstNodePtr(endpoint1));
  NodePtr endpoint2 = _map->getNode(nodeIds[nodeIds.size() - 1]);
  std::shared_ptr<geos::geom::Geometry> ep2 = converter.convertToGeometry(ConstNodePtr(endpoint2));

  NodePtr endpoint;
  if (geometry->distance(ep1.get()) < geometry->distance(ep2.get()))
    endpoint = endpoint1;
  else
    endpoint = endpoint2;

  return _snapUnconnectedNodeToWay(endpoint, connectTo);
}

bool UnconnectedWaySnapper::_snapUnconnectedNodeToWay(const NodePtr& nodeToSnap, const WayPtr& wayToSnapTo)
{
  //  Validate the parameters
  if (!nodeToSnap || !wayToSnapTo)
    return false;

  const std::vector<long>& wayNodeIdsToSnapTo = wayToSnapTo->getNodeIds();
  // Make sure the way we're trying to snap to doesn't already contain the node we're trying to
  // snap to it.
  const bool wayToSnapToContainsNodeToSnap =
    std::find(wayNodeIdsToSnapTo.begin(), wayNodeIdsToSnapTo.end(), nodeToSnap->getId()) !=
    wayNodeIdsToSnapTo.end();
  LOG_VART(wayToSnapToContainsNodeToSnap);
  if (!wayToSnapToContainsNodeToSnap)
  {
    // find the closest coord on the neighboring way to our input node
    double shortestDistanceFromNodeToSnapToWayCoord = DBL_MAX;
    const geos::geom::Coordinate closestWayToSnapToCoord =
      OsmUtils::closestWayCoordToNode(nodeToSnap, wayToSnapTo,
        shortestDistanceFromNodeToSnapToWayCoord, _snapToWayDiscretizationSpacing, _map);

    // This check of the calculated distance being less than the allowed snap distance should not
    // be necessary, but it is for now.  For some reason, neighbors are occasionally being
    // returned at longer distances away than expected.
    if (/*shortestDistance != DBL_MAX &&*/
        shortestDistanceFromNodeToSnapToWayCoord <= _maxSnapDistance)
    {
      // snap the node to the way

      // figure out where on the target way to insert our node being snapped
      const int nodeToSnapInsertIndex = _getNodeToSnapWayInsertIndex(nodeToSnap, wayToSnapTo);

      if (Log::getInstance().getLevel() <= Log::Debug)
      {
        // This projection could be expensive, so do it during debugging only.
        const geos::geom::Coordinate wgs84Coord =
          MapProjector::project(
            closestWayToSnapToCoord, _map->getProjection(),
            MapProjector::createWgs84Projection());
        LOG_TRACE(
          "Snapping way node: " << nodeToSnap->getId() << " to coord: " <<
          closestWayToSnapToCoord.toString() << " (wgs84: " << wgs84Coord.toString() <<
          ") and inserting at index: " << nodeToSnapInsertIndex);
      }

      // move the snapped node to the closest way coord
      nodeToSnap->setX(closestWayToSnapToCoord.x);
      nodeToSnap->setY(closestWayToSnapToCoord.y);

      // Add the optional custom tag for tracking purposes.
      if (_markSnappedNodes)
      {
        nodeToSnap->getTags().set(MetadataTags::HootSnappedWayNode(), "snapped_to_way");
      }

      // add the snapped node as a way node on the target way
      QList<long> wayNodeIdsToSnapToList =
        QList<long>::fromVector(QVector<long>::fromStdVector(wayNodeIdsToSnapTo));
      wayNodeIdsToSnapToList.insert(nodeToSnapInsertIndex, nodeToSnap->getId());
      wayToSnapTo->setNodes(wayNodeIdsToSnapToList.toVector().toStdVector());
      LOG_VART(wayToSnapTo->getNodeIds());

      return true;
    }
  }
  return false;
}

}
