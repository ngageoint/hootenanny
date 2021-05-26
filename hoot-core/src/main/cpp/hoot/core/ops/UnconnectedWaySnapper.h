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

#ifndef UNCONNECTED_WAY_SNAPPER
#define UNCONNECTED_WAY_SNAPPER

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/conflate/ConflateInfoCacheConsumer.h>

// Tgs
#include <tgs/RStarTree/HilbertRTree.h>

namespace hoot
{

/**
 * This class is set up to snap an unconnected way endpoint node to another way using custom
 * element input criteria to determine which types of ways should be snapped and what type of ways
 * they should be snapped to. The way may be either snapped to a way node on another way or the
 * closest non-node point on it. Snapping to an existing way node is favored in order to reduce the
 * creation of new way nodes.
 *
 * The main impetus in creating this class was to make the road output of Differential Conflation
 * better by snapping unconnected secondary roads to the nearest road in the reference dataset.
 * However, this op can also be used as a cleanup op after other types of conflation. Before using
 * this class in conflation, effort should be made, where possible, to try to fix the lack of
 * feature snapping in the conflation routines themselves rather than relying on this as a cleanup
 * utility.
 *
 * Additionally, this class can be configured to mark any snapped roads as needing review or mark
 * them for review without actually snapping them.
 *
 * *Possible* future enhancements:
 *
 * - If a way is snapped to another way and the ways end up being parallel and overlap, snapping
 * them may not make sense. Have seen bad snaps like that in a couple of datasets so far. This may
 * not be an easy change to make, since the snapped node in question may belong to multiple ways.
 * One manner in which to go about it could be to use CopyMapSubsetOp to make a temp copy of the
 * snap source and target ways, perform the snap, do the parallel/overlap checks, and then back out
 * of the actual snap if needed.
 * - This class doesn't pay attention to the direction of the ways being snapped. Not sure yet
 * if that can be addressed or not. Technically, the way joiner (maybe) run later on could fix the
 * problem.
 */
class UnconnectedWaySnapper : public OsmMapOperation, public Configurable,
  public ConflateInfoCacheConsumer
{
public:

  static QString className() { return "hoot::UnconnectedWaySnapper"; }

  UnconnectedWaySnapper();
  ~UnconnectedWaySnapper() = default;

  /**
   * @see OsmMapOperation
   */
  void apply(OsmMapPtr& map) override;

  /**
   * Finds the closest endpont on 'disconnected' and snaps it to the closest node in 'connectTo'
   *
   * @param map Map containing ways
   * @param disconnected Disconnected way that needs to be connected
   * @param connectTo Way to connect the disconnected way to
   * @returns True if the ways were successfully snapped together
   */
  static bool snapClosestWayEndpointToWay(
    OsmMapPtr map, const WayPtr& disconnected, const WayPtr& connectTo);

  /**
   * @see OperationStatus
   */
  QString getInitStatusMessage() const override
  { return "Snapping unconnected ways to the nearest way..."; }

  /**
   * @see OperationStatus
   */
  QString getCompletedStatusMessage() const override
  { return "Snapped " + QString::number(_numAffected) + " unconnected ways."; }

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  /**
   * @see ApiEntityInfo
   */
  QString getDescription() const override
  { return "Snaps unconnected ways to the nearest way."; }
  /**
   * @see ApiEntityInfo
   */
  QString getName() const override { return className(); }
  /**
   * @see ApiEntityInfo
   */
  QString getClassName() const override { return className(); }

  void setConflateInfoCache(const std::shared_ptr<ConflateInfoCache>& cache) override
  { _conflateInfoCache = cache; }

  long getNumSnappedToWays() const { return _numSnappedToWays; }
  long getNumSnappedToWayNodes() const { return _numSnappedToWayNodes; }

  void setSnapToExistingWayNodes(bool snap) { _snapToExistingWayNodes = snap; }
  void setMaxNodeReuseDistance(double distance);
  void setMaxSnapDistance(double distance);
  void setWayDiscretizationSpacing(double spacing);
  void setAddCeToSearchDistance(bool add) { _addCeToSearchDistance = add; }
  void setMarkSnappedNodes(bool mark) { _markSnappedNodes = mark; }
  void setWayToSnapToCriteria(const QStringList& criteria);
  void setWayToSnapCriteria(const QStringList& criteria);
  void setSnapWayStatuses(const QStringList& statuses);
  void setSnapToWayStatuses(const QStringList& statuses);
  void setMarkSnappedWays(bool mark) { _markSnappedWays = mark; }
  void setReviewSnappedWays(bool review) { _reviewSnappedWays = review; }
  void setMarkOnly(bool markOnly) { _markOnly = markOnly; }
  void setMinTypeMatchScore(double score);
  void setTypeExcludeKvps(const QStringList& kvps) { _typeExcludeKvps = kvps; }

private:

  friend class UnconnectedWaySnapperTest;

  // if true, will attempt to snap nodes to existing way nodes instead of adding them to the way as
  // a new way node
  bool _snapToExistingWayNodes;

  // furthest away a way node can be from a unconnected node for us to consider snapping to it
  double _maxNodeReuseDistance;
  // furthest away a way can be from a unconnected node for us to consider snapping to it
  double _maxSnapDistance;
  // how small we cut up the snap to way when trying to snap to one of its coords
  double _snapToWayDiscretizationSpacing;
  // adds the CE of each individual way node snap candidate to the nearby feature search radius
  bool _addCeToSearchDistance;

  // allow for optionally tagging snapped nodes; useful for debugging
  bool _markSnappedNodes;
  // allow for optionally tagging snapped ways; useful for debugging
  bool _markSnappedWays;
  // mark anything snapped as needing review
  bool _reviewSnappedWays;
  // don't actually snap ways; this allows for marking w/o snapping
  bool _markOnly;

  // the feature criteria to be used for way snap target candidates
  QStringList _wayToSnapToCriteria;
  ElementCriterionPtr _wayToSnapCrit;
  // the feature criteria to be used for way snap source candidates
  QStringList _wayToSnapCriteria;
  ElementCriterionPtr _wayToSnapToCrit;
  // the feature criteria to be used for way snap target candidates
  QStringList _wayNodeToSnapToCriteria;
  ElementCriterionPtr _wayNodeToSnapToCrit;
  // the status criteria to be used for the snap source way
  QStringList _snapWayStatuses;
  // the status criteria to be used for the snap target way or way node
  QStringList _snapToWayStatuses;
  // the feature criteria to use for selected unconnected way nodes to snap
  ElementCriterionPtr _unconnectedWayNodeCrit;

  // feature indexes used for way nodes being snapped to
  std::shared_ptr<Tgs::HilbertRTree> _snapToWayNodeIndex;
  std::deque<ElementId> _snapToWayNodeIndexToEid;

  // feature indexes used for ways being snapped to
  std::shared_ptr<Tgs::HilbertRTree> _snapToWayIndex;
  std::deque<ElementId> _snapToWayIndexToEid;

  // this can prevent very differently typed ways from being snapped to each other; valid range
  // greater than 0 and less than or equal to 1; a value of -1.0 ignores the setting completely
  double _minTypeMatchScore;

  // If a way has any of these tag key/value pairs, it will never be snapped.
  QStringList _typeExcludeKvps;

  // keep track of the way nodes that are snapped
  QList<long> _snappedWayNodeIds;
  long _numSnappedToWays;
  long _numSnappedToWayNodes;
  WayPtr _snappedToWay;

  // Existence of this cache tells us that elements must be individually checked to see that they
  // are conflatable given the current configuration before modifying them.
  std::shared_ptr<ConflateInfoCache> _conflateInfoCache;

  int _taskStatusUpdateInterval;

  OsmMapPtr _map;

  Settings _conf;

  ReviewMarker _reviewMarker;

  /*
   * @see WayJoinerAdvanced
   */
  long _getPid(const ConstWayPtr& way) const;

  /*
   * The radius around the end node to look for ways to snap to.
   */
  Meters _getWaySearchRadius(const ConstElementPtr& e) const;

  /*
   * The radius around the end node to look for way nodes to snap to.
   */
  Meters _getWayNodeSearchRadius(const ConstElementPtr& e) const;

  /*
   * Creates all necessary feature matching criteria
   */
  void _createAllFeatureCriteria();
  /*
   * Creates the matching criteria used to determine via filtering which features we want to snap or
   * snap to, which consists of type and status criteria combined together
   *
   * @param typeCriteria the names of one or more hoot ConflatableElementCriterion classes
   * @param statuses one or more hoot status strings
   * @param isNode if true, indicates we are creating matching criteria for a node instead of a way
   * @return an element criterion
   */
  ElementCriterionPtr _createFeatureCriteria(
    const QStringList& typeCriteria, const QStringList& statuses, const bool isNode = false) const;
  ElementCriterionPtr _getTypeCriteria(
    const QStringList& typeCriteria, const bool isNode = false) const;
  ElementCriterionPtr _getTypeCriterion(
    const QString& typeCriterion, const bool isNode = false) const;
  ElementCriterionPtr _getStatusCriteria(const QStringList& statuses) const;

  /*
   * Creates all necessary feature spatial indices
   */
  void _createFeatureIndexes();
  /*
   * Creates an individual feature spatial index needed when searching for features to snap to
   *
   * @param featureCrit the element criterion for the feature type being indexed
   * @param featureIndex a pointer to the geospatial index being created
   * @param featureIndexToEid a pointer to the element ID index being created
   * @param elementType the element type of the criterion class; either Way or Node
   */
  void _createFeatureIndex(
    const ElementCriterionPtr& featureCrit, std::shared_ptr<Tgs::HilbertRTree>& featureIndex,
    std::deque<ElementId>& featureIndexToEid, const ElementType& elementType);

  /*
   * Finds all unconnected way end nodes and attempts to snap them
   */
  void _snapUnconnectedWayEndNodes(const WayPtr& wayToSnap);
  bool _snapUnconnectedWayEndNode(const NodePtr& unconnectedEndNode, const WayPtr& wayToSnap);

  /*
   * Identifies unconnected way nodes
   *
   * @param way the way to examine
   * @param wayCrit an optional element criterion to restrict the types of ways being examined
   * @return a collection of node IDs
   */
  std::set<long> _getUnconnectedWayEndNodeIds(
    const ConstWayPtr& way, const ElementCriterionPtr& wayCrit = ElementCriterionPtr()) const;
  /*
   * Return feature candidates to snap to
   *
   * @param node the node attempting snapping
   * @param elementType the element type of the feature being snapped to; either Way or Node
   * @return a collection of element IDs
   */
  QList<ElementId> _getNearbyFeaturesToSnapTo(
    const ConstNodePtr& node, const ElementType& elementType) const;

  /*
   * Attempts to snap an unconnected way end node to another way node
   *
   * @param nodeToSnap the node to attempt to snap
   * @param wayToSnapTags optional tags of the way being snapped, which forces a type match
   * requirement between the two ways being snapped based on the value of _minTypeMatchScore
   * @return true if a node was snapped; false otherwise
   */
  bool _snapUnconnectedWayEndNodeToWayNode(const NodePtr& nodeToSnap, const Tags& wayToSnapTags);
  void _snap(const NodePtr& nodeToSnap, const NodePtr& wayNodeToSnapTo);

  /*
   * Attempts to snap an unconnected way end node to another way
   *
   * @param nodeToSnap the node to attempt to snap
   * @param wayToSnapTags optional tags of the way being snapped, which forces a type match
   * requirement between the two ways being snapped based on the value of _minTypeMatchScore
   * @return true if a node was snapped; false otherwise
   */
  bool _snapUnconnectedWayEndNodeToWay(
    const NodePtr& nodeToSnap, const Tags& wayToSnapTags);

  /*
   * Snaps a particular node into a way at its closest intersecting point
   *
   * Note that this method may still end up snapping to a way node like
   * _snapUnconnectedWayEndNodeToWayNode does, if it deems it more efficient.
   *
   * @param nodeToSnap Node to snap/add into the way
   * @param wayToSnapTo Way to connect/add the node into
   * @param snappedToNode true if a way node was snapped to; false if a way coordinate was snapped
   * to
   * @return true if a node was snapped; false otherwise
   */
  bool _snapUnconnectedWayEndNodeToWay(
    const NodePtr& nodeToSnap, const WayPtr& wayToSnapTo, bool& snappedToNode);

  /*
   * Finds the closest endpont on 'disconnected' and snaps it to the closest node in 'connectTo'
   *
   * @param disconnected Disconnected way that needs to be connected
   * @param connectTo Way to connect the disconnected way to
   * @return true if a node was snapped; false otherwise
   */
  bool _snapClosestWayEndpointToWay(const WayPtr& disconnected, const WayPtr& connectTo);

  /*
   * Marks a snapped way with a custom tag
   */
  void _markSnappedWay(const long idOfNodeBeingSnapped, const bool toWayNode);
  /*
   * Marks a snapped way with a review tag
   */
  void _reviewSnappedWay(const long idOfNodeBeingSnapped) const;
};

}

#endif // UNCONNECTED_WAY_SNAPPER
