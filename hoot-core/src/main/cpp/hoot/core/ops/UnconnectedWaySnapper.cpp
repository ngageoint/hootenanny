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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "UnconnectedWaySnapper.h"

// hoot
#include <hoot/core/algorithms/Distance.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/ConflatableElementCriterion.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/LinearWayNodeCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/SpatialIndexer.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

// Standard
#include <cfloat>
#include <functional>
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, UnconnectedWaySnapper)

UnconnectedWaySnapper::UnconnectedWaySnapper() :
_snapToExistingWayNodes(true),
_favorReferenceWayNode(false),
_maxNodeReuseDistance(0.5),
_maxSnapDistance(5.0),
_snapToWayDiscretizationSpacing(1.0),
_addCeToSearchDistance(false),
_markSnappedNodes(false),
_markSnappedWays(false),
_reviewSnappedWays(false),
_markOnly(false),
_wayToSnapToCriteria(QStringList(LinearCriterion::className())),
_wayToSnapCriteria(QStringList(LinearCriterion::className())),
_wayNodeToSnapToCriteria(QStringList(LinearWayNodeCriterion::className())),
_snapWayStatuses(QStringList(Status(Status::Unknown2).toString())),
_snapToWayStatuses(QStringList(Status(Status::Unknown1).toString())),
_minTypeMatchScore(-1.0),
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
  setFavorReferenceWayNode(confOpts.getSnapUnconnectedWaysFavorReferenceWayNode());
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

  setSnapWayStatuses(confOpts.getSnapUnconnectedWaysSnapWayStatuses());
  setSnapToWayStatuses(confOpts.getSnapUnconnectedWaysSnapToWayStatuses());

  const QStringList globalSnapCriteria = confOpts.getSnapUnconnectedWaysSnapCriteria();

  const QStringList snappedWayCriteria = confOpts.getSnapUnconnectedWaysSnapWayCriteria();
  // If neither of these conditions are true, we'll use the default of snapping ways of all types.
  if (!snappedWayCriteria.isEmpty())
  {
    setWayToSnapCriteria(snappedWayCriteria);
  }
  else if (!globalSnapCriteria.isEmpty())
  {
    setWayToSnapCriteria(globalSnapCriteria);
  }

  const QStringList snapToWayCriteria = confOpts.getSnapUnconnectedWaysSnapToWayCriteria();
  if (!snapToWayCriteria.isEmpty())
  {
    setWayToSnapToCriteria(snapToWayCriteria);
  }
  else if (!globalSnapCriteria.isEmpty())
  {
    setWayToSnapToCriteria(globalSnapCriteria);
  }

  setMarkSnappedNodes(confOpts.getSnapUnconnectedWaysMarkSnappedNodes());
  setMarkSnappedWays(confOpts.getSnapUnconnectedWaysMarkSnappedWays());
  setReviewSnappedWays(confOpts.getSnapUnconnectedWaysReviewSnappedWays());
  setMarkOnly(confOpts.getSnapUnconnectedWaysMarkOnly());

  setMinTypeMatchScore(confOpts.getSnapUnconnectedWaysMinimumTypeMatchScore());

  setTypeExcludeKvps(confOpts.getSnapUnconnectedWaysExcludeTypes());

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

void UnconnectedWaySnapper::setWayToSnapToCriteria(const QStringList& criteria)
{
  QStringList criteriaTemp = criteria;
  StringUtils::removeEmptyStrings(criteriaTemp);
  if (criteriaTemp.isEmpty())
  {
    LOG_DEBUG(
      "No snap to way criteria specified for the Unconnected Way Snapper. Defaulting to: " +
      _wayToSnapToCriteria.join(","));
  }
  else
  {
    _wayToSnapToCriteria = criteria;
  }
  LOG_VART(_wayToSnapToCriteria);
}

void UnconnectedWaySnapper::setWayToSnapCriteria(const QStringList& criteria)
{
  QStringList criteriaTemp = criteria;
  StringUtils::removeEmptyStrings(criteriaTemp);
  if (criteriaTemp.isEmpty())
  {
    LOG_DEBUG(
      "No snap way criteria specified for the Unconnected Way Snapper. Defaulting to: " +
      _wayToSnapCriteria.join(","));
  }
  else
  {
    _wayToSnapCriteria = criteria;
  }
  LOG_VART(_wayToSnapCriteria);
}

void UnconnectedWaySnapper::setSnapWayStatuses(const QStringList& statuses)
{
  _snapWayStatuses = statuses;
  StringUtils::removeEmptyStrings(_snapWayStatuses);
}

void UnconnectedWaySnapper::setSnapToWayStatuses(const QStringList& statuses)
{
  _snapToWayStatuses = statuses;
  StringUtils::removeEmptyStrings(_snapToWayStatuses);
}

void UnconnectedWaySnapper::setMinTypeMatchScore(double score)
{
  if (score != -1.0 && (score <= 0.0 || score > 1.0))
  {
    throw IllegalArgumentException(
      "Minimum type match score must be greater than 0 and less than or equal to 1 (-1 to disable).");
  }
  _minTypeMatchScore = score;
}

void UnconnectedWaySnapper::apply(OsmMapPtr& map)
{
  _map = map;
  _numAffected = 0;
  _numSnappedToWays = 0;
  _numSnappedToWayNodes = 0;
  _snappedWayNodeIds.clear();

  OsmMapWriterFactory::writeDebugMap(map, className(), "before-snapping");

  // need to be in planar for all of this
  MapProjector::projectToPlanar(_map);

  // Create feature crits for filtering what things we snap and snap to (will default to just
  // plain ways and way nodes if nothing was specified).
  _createAllFeatureCriteria();
  // Create needed geospatial indexes for surrounding feature searches.
  _createFeatureIndexes();

  WayMap ways = _map->getWays();
  LOG_DEBUG(
    "Searching for ways to snap out of " << StringUtils::formatLargeNumber(ways.size()) <<
    " total ways...");
  long waysProcessed = 0;
  for (WayMap::iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    WayPtr wayToSnap = wayItr->second;
    _numProcessed++;
    if (!wayToSnap)
    {
      continue;
    }
    LOG_VART(wayToSnap->getElementId());

    // Its possible an additional check like this may need to be made against the way or way node
    // being snapped to. Note that this only kicks in if _conflateInfoCache has been populated which
    // will only happen when this class is instantiated inside the conflate pipeline. Non-conflation
    // way snapping workflows will skip this check.
    if (_conflateInfoCache &&
        !_conflateInfoCache->elementCanBeConflatedByActiveMatcher(wayToSnap, className()))
    {
      LOG_TRACE(
        "Skipping processing of " << wayToSnap->getElementId() << ", as it cannot be conflated by "
        "any actively configured conflate matcher.");
      continue;
    }

    // This allows for type exclusion. It may make more sense to pass these exclude types in as part
    // of the snapping criteria, but will go with this for now.
    LOG_VART(wayToSnap->getTags().hasAnyKvp(_typeExcludeKvps));
    if (wayToSnap->getTags().hasAnyKvp(_typeExcludeKvps))
    {
      LOG_TRACE(
        "Skipping processing of " << wayToSnap->getElementId() <<
        ", as it is of an explicitly excluded type.");
      continue;
    }

    LOG_VART(_wayToSnapCrit->isSatisfied(wayToSnap));
    // Ensure the way has the status we require for snapping.
    if (_wayToSnapCrit->isSatisfied(wayToSnap))
    {
      _snapUnconnectedWayEndNodes(wayToSnap);
    }

    waysProcessed++;
    if (waysProcessed % (_taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "Processed " << StringUtils::formatLargeNumber(waysProcessed) << " of " <<
        StringUtils::formatLargeNumber(ways.size()) << " ways for unconnected snapping.");
    }
  }

  LOG_DEBUG(_numSnappedToWays << " ways snapped to the closest point on other ways");
  LOG_DEBUG(_numSnappedToWayNodes << " ways snapped directly to way nodes");
}

void UnconnectedWaySnapper::_createAllFeatureCriteria()
{
  LOG_DEBUG("Creating way to snap criteria...");
  _wayToSnapCrit = _createFeatureCriteria(_wayToSnapCriteria, _snapWayStatuses);
  LOG_DEBUG("Creating way to snap to criteria...");
  _wayToSnapToCrit = _createFeatureCriteria(_wayToSnapToCriteria, _snapToWayStatuses);
  if (_snapToExistingWayNodes)
  {
   LOG_DEBUG("Creating way node to snap to criteria...");
    _wayNodeToSnapToCrit =
      _createFeatureCriteria(_wayToSnapToCriteria, _snapToWayStatuses, true);
  }
  // This node crit is derived from the corresponding way crit, so don't set isNode=true. Use the
  // way to snap criteria to derive it since we always snap unconnected way end nodes from the way
  // being snapped. The status of what contains an unconnected way node doesn't matter, so we don't
  // filter by status.
  LOG_DEBUG("Creating unconnected way node criteria...");
  _unconnectedWayNodeCrit = _createFeatureCriteria(_wayToSnapCriteria, QStringList());
}

void UnconnectedWaySnapper::_createFeatureIndexes()
{
  // If the way node reuse option was turned off, then no need to index way nodes.
  if (_wayNodeToSnapToCrit)
  {
    LOG_DEBUG("Creating way node to snap to index...");
    _createFeatureIndex(
      _wayNodeToSnapToCrit, _snapToWayNodeIndex, _snapToWayNodeIndexToEid, ElementType::Node);
  }
  LOG_DEBUG("Creating way to snap to index...");
  _createFeatureIndex(
    _wayToSnapToCrit, _snapToWayIndex, _snapToWayIndexToEid, ElementType::Way);
}

long UnconnectedWaySnapper::_getPid(const ConstWayPtr& way) const
{
  LOG_VART(way->hasPid());
  long pid = WayData::PID_EMPTY;
  if (way->hasPid())
  {
    pid = way->getPid();
  }
  else if (way->getTags().contains(MetadataTags::HootSplitParentId()))
  {
    pid = way->getTags()[MetadataTags::HootSplitParentId()].toLong();
  }
  return pid;
}

ElementCriterionPtr UnconnectedWaySnapper::_createFeatureCriteria(
  const QStringList& typeCriteria, const QStringList& statuses, const bool isNode) const
{
  ElementCriterionPtr crit = ElementCriterionPtr();
  if (!typeCriteria.isEmpty())
  {
    const QString type = !isNode ? "way" : "node";
    LOG_TRACE(
      "Creating feature filtering criteria for " << type << ": " << typeCriteria <<
      " having statuses: " << statuses << "...");

    ElementCriterionPtr typeCrit = _getTypeCriteria(typeCriteria, isNode);
    if (!statuses.isEmpty())
    {
      ElementCriterionPtr statusCrit = _getStatusCriteria(statuses);
      // combine our element type and status crits into a single crit
      crit = std::make_shared<ChainCriterion>(statusCrit, typeCrit);
    }
    else
    {
      crit = typeCrit;
    }
  }

  LOG_VART(crit);
  return crit;
}

ElementCriterionPtr UnconnectedWaySnapper::_getTypeCriteria(
  const QStringList& typeCriteria, const bool isNode) const
{
  if (typeCriteria.isEmpty())
  {
    throw IllegalArgumentException("Way snapping type criteria is empty.");
  }

  ElementCriterionPtr typeCrit = ElementCriterionPtr();
  if (typeCriteria.size() == 1)
  {
    typeCrit = _getTypeCriterion(typeCriteria.at(0), isNode);
  }
  else
  {
    std::shared_ptr<OrCriterion> orCrit = std::make_shared<OrCriterion>();
    for (int i = 0; i < typeCriteria.size(); i++)
    {
      orCrit->addCriterion(_getTypeCriterion(typeCriteria.at(i), isNode));
    }
    LOG_VART(orCrit->criteriaSize());
    typeCrit = orCrit;
  }
  LOG_VART(typeCrit);
  return typeCrit;
}

ElementCriterionPtr UnconnectedWaySnapper::_getTypeCriterion(
  const QString& typeCriterion, const bool isNode) const
{
  LOG_VART(typeCriterion);
  LOG_VART(isNode);
  ElementCriterionPtr typeCrit = ElementCriterionPtr();
  if (!typeCriterion.trimmed().isEmpty())
  {
    typeCrit = Factory::getInstance().constructObject<ElementCriterion>(typeCriterion);
    LOG_VART(typeCrit);

    // Requiring that the criteria be ConflatableElementCriterion is a little misleading, as they
    // don't necessary have to be conflatable, but that's the only use case we have so far. Also, it
    // allows us to call getChildCriteria when isNode=true.
    std::shared_ptr<ConflatableElementCriterion> conflatableCrit =
      std::dynamic_pointer_cast<ConflatableElementCriterion>(typeCrit);
    if (!conflatableCrit)
    {
      throw IllegalArgumentException(
        "Only classes inheriting from ConflatableElementCriterion are valid as way snapping "
        "criteria.");
    }
    if (conflatableCrit->getGeometryType() != GeometryTypeCriterion::GeometryType::Line)
    {
      throw IllegalArgumentException(
        "Only classes capable of conflating linear features are valid as way snapping criteria.");
    }

    // If we're creating a node index, get the corresponding way node type from the conflatable
    // crit we just created. Assuming a single criterion returned here, which *should* be true for
    // all linear crits.
    if (isNode)
    {
      typeCrit =
        Factory::getInstance().constructObject<ElementCriterion>(
          conflatableCrit->getChildCriteria().at(0));
    }

    // configure our element criterion, in case it needs it
    std::shared_ptr<Configurable> configurable =
      std::dynamic_pointer_cast<Configurable>(typeCrit);
    if (configurable)
    {
      configurable->setConfiguration(_conf);
    }
    std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
      std::dynamic_pointer_cast<ConstOsmMapConsumer>(typeCrit);
    if (mapConsumer)
    {
      mapConsumer->setOsmMap(_map.get());
    }
  }
  LOG_VART(typeCrit);
  return typeCrit;
}

ElementCriterionPtr UnconnectedWaySnapper::_getStatusCriteria(const QStringList& statuses) const
{
  // create our criterion for the feature status
  ElementCriterionPtr statusCrit = ElementCriterionPtr();
  if (statuses.size() == 1)
  {
    statusCrit = std::make_shared<StatusCriterion>(Status::fromString(statuses.at(0)));
  }
  else
  {
    std::shared_ptr<OrCriterion> orCrit = std::make_shared<OrCriterion>();
    for (int i = 0; i < statuses.size(); i++)
    {
      orCrit->addCriterion(std::make_shared<StatusCriterion>(Status::fromString(statuses.at(i))));
    }
    statusCrit = orCrit;
  }
  LOG_VART(statusCrit);
  return statusCrit;
}

void UnconnectedWaySnapper::_createFeatureIndex(
  const ElementCriterionPtr& featureCrit, std::shared_ptr<Tgs::HilbertRTree>& featureIndex,
  std::deque<ElementId>& featureIndexToEid, const ElementType& elementType)
{
  LOG_DEBUG("Creating feature index of type: " << elementType << "...");

  // tune these indexes? - see #3054
  featureIndex =
    std::make_shared<Tgs::HilbertRTree>(std::make_shared<Tgs::MemoryPageStore>(728), 2);
  std::shared_ptr<SpatialIndexer> spatialIndexer;
  if (elementType == ElementType::Node)
  {
    spatialIndexer =
      std::make_shared<SpatialIndexer>(
        featureIndex, featureIndexToEid, featureCrit,
        std::bind(&UnconnectedWaySnapper::_getWayNodeSearchRadius, this, placeholders::_1), _map);
  }
  else
  {
      spatialIndexer =
        std::make_shared<SpatialIndexer>(
          featureIndex, featureIndexToEid, featureCrit,
          std::bind(&UnconnectedWaySnapper::_getWaySearchRadius, this, placeholders::_1), _map);
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

void UnconnectedWaySnapper::_snapUnconnectedWayEndNodes(const WayPtr& wayToSnap)
{
  // Find unconnected endpoints on the way, if the way satisfies the specified crit.
  const std::set<long> unconnectedEndNodeIds =
    _getUnconnectedWayEndNodeIds(wayToSnap, _unconnectedWayNodeCrit);
  for (std::set<long>::const_iterator unconnectedEndNodeIdItr = unconnectedEndNodeIds.begin();
       unconnectedEndNodeIdItr != unconnectedEndNodeIds.end(); ++unconnectedEndNodeIdItr)
  {
    bool snapOccurred = false;

    // for each unconnected endpoint
    const long unconnectedEndNodeId = *unconnectedEndNodeIdItr;
    // don't snap the same node more than once
    if (!_snappedWayNodeIds.contains(unconnectedEndNodeId))
    {
      NodePtr unconnectedEndNode = _map->getNode(unconnectedEndNodeId);
      LOG_VART(unconnectedEndNode->getTags().hasAnyKvp(_typeExcludeKvps));
      if (unconnectedEndNode->getTags().hasAnyKvp(_typeExcludeKvps))
      {
        LOG_TRACE(
          "Skipping processing of " << unconnectedEndNode->getElementId() <<
          ", as it is of an explicitly excluded type.");
        continue;
      }
      snapOccurred = _snapUnconnectedWayEndNode(unconnectedEndNode, wayToSnap);
    }

    if (!snapOccurred)
    {
      LOG_TRACE(
        "No snap occurred for " << wayToSnap->getElementId() << " with unconnected end node " <<
        ElementId(ElementType::Node, unconnectedEndNodeId));
    }
  }

  if (unconnectedEndNodeIds.empty())
  {
    LOG_TRACE("No unconnected end nodes to snap for " << wayToSnap->getElementId());
  }
}

bool UnconnectedWaySnapper::_snapUnconnectedWayEndNode(
  const NodePtr& unconnectedEndNode, const WayPtr& wayToSnap)
{
  // For way snapping, pass in the tags of the way being snapped during comparison. They will be
  // needed if a minimum type score or a feature type exclude list was specified.

  // Try to find the nearest way node satisfying the specifying way node criteria that is within the
  // max reuse distance (if one was specified) and snap the unconnected way node. The reuse of
  // existing way nodes on way being snapped to is done in order to cut back on the number of new
  // way nodes being added.
  bool snapOccurred = false;
  if (_snapToExistingWayNodes)
  {
    snapOccurred = _snapUnconnectedWayEndNodeToWayNode(unconnectedEndNode, wayToSnap->getTags());
  }

  if (!snapOccurred)
  {
    // If we weren't able to snap to a nearby way node or the snapping directly to way nodes option
    // was turned off, we're going to try to find a nearby way and snap onto the closest location on
    // it.
    snapOccurred = _snapUnconnectedWayEndNodeToWay(unconnectedEndNode, wayToSnap->getTags());
  }

  if (snapOccurred)
  {
    assert(_snappedToWay);
    LOG_TRACE(
      "Snapped " << wayToSnap->getElementId() << " to " << _snappedToWay->getElementId() <<
      " from " << unconnectedEndNode->getElementId());

    // Previously we were retaining the parent ID and status of the way being snapped to but
    // discovered later on that was causing problems with dropped ref ways when generating
    // changesets and writing them back with Diff Conflate (#4977).

    LOG_VART(wayToSnap);
    LOG_VART(_snappedToWay);

    // It seems like the geospatial indices should have to be updated after every snap, but haven't
    // any direct evidence of that being necessary for proper snapping yet. If it is needed, that
    // would likely slow things down a lot.

    if (ConfigOptions().getDebugMapsWriteDetailed())
    {
      OsmMapWriterFactory::writeDebugMap(
        _map, className(),
        "after-snap-" + wayToSnap->getElementId().toString() + "-" +
          unconnectedEndNode->getElementId().toString() + "-" +
          _snappedToWay->getElementId().toString());
    }
  }
  return snapOccurred;
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

std::set<long> UnconnectedWaySnapper::_getUnconnectedWayEndNodeIds(
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
    WayUtils::getContainingWayIds(firstEndNodeId, _map, wayCrit);
  const std::set<long> filteredWaysContainingSecondEndNode =
    WayUtils::getContainingWayIds(secondEndNodeId, _map, wayCrit);
  LOG_VART(filteredWaysContainingFirstEndNode);
  LOG_VART(filteredWaysContainingSecondEndNode);

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

QList<ElementId> UnconnectedWaySnapper::_getNearbyFeaturesToSnapTo(
  const ConstNodePtr& node, const ElementType& elementType) const
{
  LOG_TRACE("Retrieving nearby features to snap to for: " << node->getElementId() << "...");

  QList<ElementId> neighborIds;
  std::shared_ptr<geos::geom::Envelope> env(node->getEnvelope(_map));
  if (elementType == ElementType::Node)
  {
    env->expandBy(_getWayNodeSearchRadius(node));
    // The node neighbors must be sorted by distance to get the best way node snapping results. Not
    // sure yet if they need to be when snapping to ways.
    neighborIds =
      SpatialIndexer::findSortedNodeNeighbors(
        node, *env, _snapToWayNodeIndex, _snapToWayNodeIndexToEid, _map);
  }
  else
  {
    env->expandBy(_getWaySearchRadius(node));
    const std::set<ElementId> neighborIdsSet =
      SpatialIndexer::findNeighbors(
        *env, _snapToWayIndex, _snapToWayIndexToEid, _map, elementType, false);
    for (std::set<ElementId>::const_iterator neighborIdsItr = neighborIdsSet.begin();
         neighborIdsItr != neighborIdsSet.end(); ++neighborIdsItr)
    {
      neighborIds.append(*neighborIdsItr);
    }
  }
  LOG_VART(neighborIds);
  return neighborIds;
}

bool UnconnectedWaySnapper::_snapUnconnectedWayEndNodeToWayNode(
  const NodePtr& nodeToSnap, const Tags& wayToSnapTags)
{
  if (!nodeToSnap)
  {
    return false;
  }

  LOG_TRACE("Attempting to snap unconnected node: " << nodeToSnap->getId() << " to a way node...");

  // Find all way nodes near the input way node that don't belong to the same way the input way
  // node belongs to.
  const QList<ElementId> wayNodesToSnapTo =
    _getNearbyFeaturesToSnapTo(nodeToSnap, ElementType::Node);
  if (wayNodesToSnapTo.empty())
  {
    LOG_TRACE(
      "No nearby way nodes to snap to for " << nodeToSnap->getElementId() <<
      ". Skipping snapping...");
  }

  OsmSchema& schema = OsmSchema::getInstance();

  // For each way node neighbor, let's try snapping to the first one that we can.
  for (QList<ElementId>::const_iterator wayNodesToSnapToItr = wayNodesToSnapTo.begin();
       wayNodesToSnapToItr != wayNodesToSnapTo.end(); ++wayNodesToSnapToItr)
  {
    const long wayNodeToSnapToId = (*wayNodesToSnapToItr).getId();
    LOG_VART(wayNodeToSnapToId);

    // Don't try to snap a way node to itself.
    if (wayNodeToSnapToId != nodeToSnap->getId())
    {
      NodePtr wayNodeToSnapTo = _map->getNode(wayNodeToSnapToId);
      if (!wayNodeToSnapTo)
      {
        LOG_TRACE(
          "Way node to snap to with ID: " << wayNodeToSnapToId <<
          " does not exist in the map. Skipping snap...");
        continue;
      }
      LOG_VART(wayNodeToSnapTo->getId());

      // Compare all the ways that a contain the neighbor and all the ways that contain our input
      // node. If there's overlap, then we pass b/c we don't want to try to snap the input way
      // node to a way its already on.
      if (!WayUtils::nodesAreContainedInTheSameWay(wayNodeToSnapToId, nodeToSnap->getId(), _map))
      {
        // If a minimum type match score was specified, we need to do a type comparison. If the
        // type similarity falls below the configured score, don't snap the ways together.
        if (_minTypeMatchScore != -1.0)
        {
          const std::vector<ConstWayPtr> containingWays =
            WayUtils::getContainingWaysConst(wayNodeToSnapToId, _map);
          LOG_VART(containingWays.size());
          bool typeMatchFound = false;
          for (std::vector<ConstWayPtr>::const_iterator containingWaysItr = containingWays.begin();
               containingWaysItr != containingWays.end(); ++containingWaysItr)
          {   
            ConstWayPtr containingWay = *containingWaysItr;
            LOG_VART(containingWay->getElementId());
            if (schema.explicitTypeMismatch(
                  wayToSnapTags, containingWay->getTags(), _minTypeMatchScore))
            {
              LOG_TRACE(
                "Explicit type mismatch between containing way: " << containingWay->getElementId() <<
                " with type: " << schema.getFirstType(containingWay->getTags(), true) <<
                " and way to snap with type: " << schema.getFirstType(wayToSnapTags, true) <<
                " for minimum match score: " << _minTypeMatchScore << ". Skipping snap.");
            }
            else
            {
              LOG_TRACE(
                "Type match between containing way: " << containingWay->getElementId() <<
                " with type: " << schema.getFirstType(containingWay->getTags(), true) <<
                " and way to snap with type: " << schema.getFirstType(wayToSnapTags, true) <<
                " for minimum match score: " << _minTypeMatchScore << ". Proceeding with snap.");
              typeMatchFound = true;
              break;
            }
          }

          LOG_VART(typeMatchFound);
          if (!typeMatchFound)
          {
            continue;
          }
        }
        else
        {
          LOG_TRACE(
            "Skipping type matching between ways containing way nodes: " <<
            (*wayNodesToSnapToItr).getId() << " and " << nodeToSnap->getId() <<
            ", since no minimum type score set...");
        }

        // Snap the input way node to the nearest way node neighbor we found. If we're trying to
        // keep as many existing reference way nodes as possible, then we'll swap the source/target
        // way node here if necessary.
        if (_favorReferenceWayNode && nodeToSnap->getStatus() == Status::Unknown1 &&
            wayNodeToSnapTo->getStatus() == Status::Unknown2)
        {
          _snap(wayNodeToSnapTo, nodeToSnap);
        }
        else
        {
          _snap(nodeToSnap, wayNodeToSnapTo);
        }

        // Don't snap the node more than once.
        return true;
      }
      else
      {
        LOG_TRACE(
          "Nodes " << wayNodeToSnapTo->getElementId() << " and " << nodeToSnap->getElementId() <<
          " are contained in the same way. Skipping snapping...")
      }
    }
  }

  return false;
}

void UnconnectedWaySnapper::_snap(const NodePtr& nodeToSnap, const NodePtr& wayNodeToSnapTo)
{
  LOG_TRACE(
    "Snapping way node: " << nodeToSnap->getElementId() << " to way node: " <<
    wayNodeToSnapTo->getElementId());

  // Add this optional custom tag for tracking purposes.
  if (_markSnappedNodes)
  {
    wayNodeToSnapTo->getTags().set(MetadataTags::HootSnapped(), "to_way_node_source");
  }
  if (_markSnappedWays)
  {
    _markSnappedWay(nodeToSnap->getId(), true);
  }
  if (_reviewSnappedWays)
  {
    _reviewSnappedWay(nodeToSnap->getId());
  }
  // Get the snapped to way so we can retain the parent id. The size should be equal to 1. Maybe
  // this could be optimized, since we're doing way containing way node checks already above?
  const std::set<long>& waysContainingWayNodeToSnapTo =
    _map->getIndex().getNodeToWayMap()->getWaysByNode(wayNodeToSnapTo->getId());
  LOG_TRACE(waysContainingWayNodeToSnapTo.size());
  assert(waysContainingWayNodeToSnapTo.size() > 0);
  _snappedToWay = _map->getWay(*waysContainingWayNodeToSnapTo.begin());
  _snappedToWay->getTags().set(MetadataTags::HootSnapped(), "to_way_node_target");
  LOG_VART(_snappedToWay);

  // Skip the actual snapping if we're only marking ways that could be snapped.
  if (!_markOnly)
  {
    LOG_TRACE(
      "Replacing " << nodeToSnap->getElementId() << " with " << wayNodeToSnapTo->getElementId());
    // Merge the tags.
    wayNodeToSnapTo->setTags(
      TagMergerFactory::mergeTags(
        wayNodeToSnapTo->getTags(), nodeToSnap->getTags(), ElementType::Node));
    // Replace the snapped node with the node we snapped it to.
    ReplaceElementOp(
      nodeToSnap->getElementId(), wayNodeToSnapTo->getElementId(), true).apply(_map);
  }

  _snappedWayNodeIds.append(nodeToSnap->getId());
  _numAffected++;
  _numSnappedToWayNodes++;
}

bool UnconnectedWaySnapper::_snapUnconnectedWayEndNodeToWay(
  const NodePtr& nodeToSnap, const Tags& wayToSnapTags)
{
  if (!nodeToSnap)
  {
    return false;
  }

  LOG_TRACE("Attempting to snap unconnected node: " << nodeToSnap->getId() << " to a way...");

  OsmSchema& schema = OsmSchema::getInstance();

  // get nearby ways
  const QList<ElementId> waysToSnapTo =
    _getNearbyFeaturesToSnapTo(nodeToSnap, ElementType::Way);
  for (QList<ElementId>::const_iterator waysToSnapToItr = waysToSnapTo.begin();
       waysToSnapToItr != waysToSnapTo.end(); ++waysToSnapToItr)
  {
    // for each neighboring way
    WayPtr wayToSnapTo = _map->getWay((*waysToSnapToItr).getId());
    LOG_VART(wayToSnapTo->getElementId());

    // If the tags of the way being snapped were passed in, we need to do a type comparison.
    // If the type similarity falls below the configured score, don't snap the ways together.
    if (!wayToSnapTags.isEmpty())
    {
      if (_minTypeMatchScore == -1.0)
      {
        LOG_TRACE(
          "Skipping type matching between way to snap to: " << wayToSnapTo->getElementId() <<
          " and way to snap, since no minimum type score set...");
      }
      else if (
        schema.explicitTypeMismatch(wayToSnapTags, wayToSnapTo->getTags(), _minTypeMatchScore))
      {
        LOG_TRACE(
          "Explicit type mismatch between way to snap to: " << wayToSnapTo->getElementId() <<
          " with type: " << schema.getFirstType(wayToSnapTo->getTags(), true) <<
          " and way to snap with type: " << schema.getFirstType(wayToSnapTags, true) <<
          " for minimum match score: " << _minTypeMatchScore << ". Snapping skipped.");
        continue;
      }
      else
      {
        LOG_TRACE(
          "Type match between way to snap to: " << wayToSnapTo->getElementId() <<
          " with type: " << schema.getFirstType(wayToSnapTo->getTags(), true) <<
          " and way to snap with type: " << schema.getFirstType(wayToSnapTags, true) <<
          " for minimum match score: " << _minTypeMatchScore << ". Proceeding with snap...");
      }
    }

    // If the node to snap belongs to a way that has any way nodes in common with the way being
    // snapped to, then skip the snap. These generally manifest themselves as roads with forks,
    // which we don't want being snapped to each other. See the two unnamed highway=service roads in
    // the Southeast corner and East of the road, "Jay Sarno Way" in
    // ServiceChangesetReplacementOutOfSpecMixedRelationsTest as an example. This may need more
    // testing beyond what we already have.
    const bool nodeToSnapContainedByWaySharingNodesWithWayToSnapTo =
      WayUtils::nodeContainedByWaySharingNodesWithAnotherWay(
        nodeToSnap->getId(), wayToSnapTo->getId(), _map);
    if (nodeToSnapContainedByWaySharingNodesWithWayToSnapTo)
    {
      LOG_TRACE(
        "Node to snap: " << nodeToSnap->getElementId() << " belongs to way sharing nodes with the"
        " way to snap to: " << wayToSnapTo->getElementId() << ". Skipping snap for " <<
        nodeToSnap->getElementId() << "...")
      continue;
    }

    // The attempt to snap to a way coord may end up snapping to an existing way node instead, if
    // its deemed more efficient (existing way node very close to the selected coord), so we want
    // to take note of that in order to do bookkeeping here correctly.
    bool snappedToNode = false;
    const bool snapped = _snapUnconnectedWayEndNodeToWay(nodeToSnap, wayToSnapTo, snappedToNode);
    if (snapped)
    {
      if (!snappedToNode)
      {
        _snappedWayNodeIds.append(nodeToSnap->getId());
        _numAffected++;
        _numSnappedToWays++;
      }
      // Don't snap the node more than once.
      return true;
    }
  }

  return false;
}

bool UnconnectedWaySnapper::_snapUnconnectedWayEndNodeToWay(
  const NodePtr& nodeToSnap, const WayPtr& wayToSnapTo, bool& snappedToNode)
{
  //  Validate the parameters
  if (!nodeToSnap || !wayToSnapTo)
    return false;

  LOG_TRACE(
    "Attempting to snap node: " << nodeToSnap->getElementId() << " to way: " <<
    wayToSnapTo->getElementId() << "...")

  const std::vector<long>& wayNodeIdsToSnapTo = wayToSnapTo->getNodeIds();
  // Make sure the way we're trying to snap to doesn't already contain the node we're trying to
  // snap to it.
  const bool wayToSnapToContainsNodeToSnap =
    std::find(wayNodeIdsToSnapTo.begin(), wayNodeIdsToSnapTo.end(), nodeToSnap->getId()) !=
    wayNodeIdsToSnapTo.end();
  LOG_VART(wayToSnapToContainsNodeToSnap);
  if (!wayToSnapToContainsNodeToSnap)
  {
    // Find the closest coord on the neighboring way to our input node.
    double shortestDistanceFromNodeToSnapToWayCoord = DBL_MAX;
    const geos::geom::Coordinate closestWayToSnapToCoord =
      WayUtils::closestWayCoordToNode(nodeToSnap, wayToSnapTo,
        shortestDistanceFromNodeToSnapToWayCoord, _snapToWayDiscretizationSpacing, _map);

    // This check of the calculated distance being less than the allowed snap distance should not
    // be necessary, but it is for now. For some reason, neighbors are occasionally being returned
    // at longer distances away than expected.
    // TODO: This may have been fixed with the addition of distance sorting in
    // SpatialIndexer...check.
    if (shortestDistanceFromNodeToSnapToWayCoord <= _maxSnapDistance)
    {
      // Do a check here to see if the coordinate we've selected to snap to on the way is very close
      // to one of its way nodes. If it is, let's just snap to the way node instead to avoid
      // duplication of way nodes. It may be possible to combine this with the previous step as an
      // optimization.
      NodePtr closestWayNode =
        _map->getNode(WayUtils::closestWayNodeIdToNode(nodeToSnap, wayToSnapTo, _map));
      const geos::geom::Coordinate closestWayCoordinate = closestWayNode->toCoordinate();
      const double distanceBetweenWaySnapCoordAndClosestWayNode =
        Distance::euclidean(closestWayToSnapToCoord, closestWayCoordinate);
      LOG_VART(distanceBetweenWaySnapCoordAndClosestWayNode);
      if (distanceBetweenWaySnapCoordAndClosestWayNode <=
          ConfigOptions().getDuplicateNodeRemoverDistanceThreshold())
      {
        LOG_TRACE("Way has way node very close to node to snap. Snapping to node instead...");
        _snap(nodeToSnap, closestWayNode);
        snappedToNode = true;
        return true;
      }

      // Snap the node to the coord on the way.

      // Figure out where on the target way to insert our node being snapped.
      const long nodeToSnapInsertIndex =
        WayUtils::closestWayNodeInsertIndex(nodeToSnap, wayToSnapTo, _map);
      if (nodeToSnapInsertIndex == -1)
      {
        return false;
      }

      if (Log::getInstance().getLevel() <= Log::Debug)
      {
        // This projection could be expensive, so do it during debugging only.
        const geos::geom::Coordinate wgs84Coord =
          MapProjector::project(
            closestWayToSnapToCoord, _map->getProjection(),
            MapProjector::createWgs84Projection());
        LOG_TRACE(
          "Snapping way node: " << nodeToSnap->getElementId() << " to coord: " <<
          closestWayToSnapToCoord.toString() << " (wgs84: " << wgs84Coord.toString() <<
          ") and inserting at index: " << nodeToSnapInsertIndex);
      }

      // Add this optional custom tag for tracking purposes.
      if (_markSnappedNodes)
      {
        nodeToSnap->getTags().set(MetadataTags::HootSnapped(), "to_way_source");
      }
      if (_markSnappedWays)
      {
        _markSnappedWay(nodeToSnap->getId(), false);
      }
      if (_reviewSnappedWays)
      {
        _reviewSnappedWay(nodeToSnap->getId());
      }

      // Skip the actual snapping if we're only marking ways that could be snapped.
      if (!_markOnly)
      {
        // Move the snapped node to the closest way coord.
        nodeToSnap->setX(closestWayToSnapToCoord.x);
        nodeToSnap->setY(closestWayToSnapToCoord.y);

        // Add the snapped node as a way node on the target way.
        QList<long> wayNodeIdsToSnapToList =
          QList<long>::fromVector(QVector<long>::fromStdVector(wayNodeIdsToSnapTo));
        wayNodeIdsToSnapToList.insert(nodeToSnapInsertIndex, nodeToSnap->getId());
        wayToSnapTo->setNodes(wayNodeIdsToSnapToList.toVector().toStdVector());
        LOG_VART(wayToSnapTo->getNodeIds());
      }
      _snappedToWay = wayToSnapTo;
      _snappedToWay->getTags().set(MetadataTags::HootSnapped(), "to_way_target");
      LOG_VART(_snappedToWay);

      return true;
    }
    else
    {
      LOG_TRACE(
        "Node to snap: " << nodeToSnap->getElementId() << " not within snap distance: " <<
        _maxSnapDistance << " of " << wayToSnapTo->getElementId());
    }
  }
  else
  {
    LOG_TRACE(
      "Way to snap to: " << wayToSnapTo->getElementId() << " contains node to snap: " <<
      nodeToSnap->getElementId())
  }
  return false;
}

bool UnconnectedWaySnapper::snapClosestWayEndpointToWay(
  OsmMapPtr map, const WayPtr& disconnected, const WayPtr& connectTo)
{
  LOG_TRACE(
    "Attempting to snap " << disconnected->getElementId() << " to " << connectTo->getElementId() <<
    "...");

  //  Create object for static call
  UnconnectedWaySnapper uws;
  uws._map = map;
  //  Call protected function to snap closest endpoint to way
  return uws._snapClosestWayEndpointToWay(disconnected, connectTo);
}

bool UnconnectedWaySnapper::_snapClosestWayEndpointToWay(
  const WayPtr& disconnected, const WayPtr& connectTo)
{
  //  Validate the parameters
  if (!disconnected || !connectTo || disconnected->getId() == connectTo->getId())
    return false;
  //  Get the endpoint closest to the way to connect to it
  const std::vector<long> nodeIds = disconnected->getNodeIds();
  ElementToGeometryConverter converter(_map);
  std::shared_ptr<geos::geom::Geometry> geometry = converter.convertToGeometry(connectTo);
  if (!geometry || geometry->isEmpty())
  {
    return false;
  }

  NodePtr endpoint1 = _map->getNode(nodeIds[0]);
  std::shared_ptr<geos::geom::Geometry> ep1 = converter.convertToGeometry(ConstNodePtr(endpoint1));
  NodePtr endpoint2 = _map->getNode(nodeIds[nodeIds.size() - 1]);
  std::shared_ptr<geos::geom::Geometry> ep2 = converter.convertToGeometry(ConstNodePtr(endpoint2));

  NodePtr endpoint;
  if (geometry->distance(ep1.get()) < geometry->distance(ep2.get()))
    endpoint = endpoint1;
  else
    endpoint = endpoint2;

  bool snappedToNode = false;
  return _snapUnconnectedWayEndNodeToWay(endpoint, connectTo, snappedToNode);
}

void UnconnectedWaySnapper::_markSnappedWay(
  const long idOfNodeBeingSnapped, const bool toWayNode) const
{
  std::set<long> owningWayIds =
    WayUtils::getContainingWayIds(idOfNodeBeingSnapped, _map);
  const long owningWayId = *owningWayIds.begin();
  const QString tagVal = toWayNode ? "to_way_node_source" : "to_way_source";
  _map->getWay(owningWayId)->getTags().set(MetadataTags::HootSnapped(), tagVal);
}

void UnconnectedWaySnapper::_reviewSnappedWay(const long idOfNodeBeingSnapped) const
{
  std::set<long> owningWayIds =
    WayUtils::getContainingWayIds(idOfNodeBeingSnapped, _map);
  const long owningWayId = *owningWayIds.begin();
  _reviewMarker.mark(
    _map, _map->getWay(owningWayId), "Potentially snappable unconnected way",
    className(), 1.0);
}

}
