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

#ifndef UNCONNECTED_WAY_SNAPPER
#define UNCONNECTED_WAY_SNAPPER

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>

// Tgs
#include <tgs/RStarTree/HilbertRTree.h>

namespace hoot
{

/**
 * This class is set up to snap an unconnected way endpoint node to another way, using custom
 * element input criteria to determine which types of ways should be snapped and what type of ways
 * they should be snapped to.  However, as of 3/21/19, this has only ever been tested with roads.
 *
 * The main impetus in creating this class was to make the road output of Differential Conflation
 * using the Network Algorithm (with rubber sheeting as a pre conflate op) better by snapping
 * unconnected secondary roads to the nearest road in the reference dataset.  However, this op
 * could also be used as a cleanup op after other types of conflation.  Future efforts, if possible,
 * should focus on trying to fix the lack of snapping in the conflation routines themselves rather
 * than relying on this as a cleanup utility.
 *
 * *Possible* future enhancements:
 *
 * - If the ways are very parallel, snapping them may not make sense.  I've only seen bad snapped
 * roads like this in cropped data so far so may not end up being relevant.
 * - If there ends up being a way node fairly close to the selected snap point on the way and that
 * way node was skipped over due to being outside of the way snap threshold, it still might make
 * sense to snap to it instead.  Have only seen one instance of this so far...
 */
class UnconnectedWaySnapper : public OsmMapOperation, public OperationStatusInfo,
  public Configurable
{
public:

  static std::string className() { return "hoot::UnconnectedWaySnapper"; }

  UnconnectedWaySnapper();

  /**
   * @see OsmMapOperation
   */
  virtual void apply(OsmMapPtr& map);

  /**
   * @see OperationStatusInfo
   */
  virtual QString getInitStatusMessage() const
  { return "Snapping unconnected ways to the nearest way..."; }

  /**
   * @see OperationStatusInfo
   */
  virtual QString getCompletedStatusMessage() const
  { return "Snapped " + QString::number(_numAffected) + " unconnected ways."; }

  /**
   * @see OperationStatusInfo
   */
  virtual QString getDescription() const
  { return "Snaps unconnected ways to the nearest way."; }

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  long getNumSnappedToWays() const { return _numSnappedToWays; }
  long getNumSnappedToWayNodes() const { return _numSnappedToWayNodes; }

  void setSnapToExistingWayNodes(bool snap) { _snapToExistingWayNodes = snap; }
  void setMaxNodeReuseDistance(double distance);
  void setMaxSnapDistance(double distance);
  void setWayDiscretizationSpacing(double spacing);
  void setAddCeToSearchDistance(bool add) { _addCeToSearchDistance = add; }
  void setMarkSnappedNodes(bool mark) { _markSnappedNodes = mark; }
  void setWayToSnapToCriterionClassName(QString name);
  void setWayToSnapCriterionClassName(QString name);
  void setWayNodeToSnapToCriterionClassName(QString name);
  void setSnapWayStatus(const Status& status) { _snapWayStatus = status; }
  void setSnapToWayStatus(const Status& status) { _snapToWayStatus = status; }

  /**
   * @brief snapClosestEndpointToWay Finds the closest endpont on 'disconnected' and snaps it to
   *   the closest node in 'connectTo'
   * @param map Map containing ways
   * @param disconnected Disconnected way that needs to be connected
   * @param connectTo Way to connect the disconnected way to
   * @returns True if the ways were successfully snapped together
   */
  static bool snapClosestEndpointToWay(OsmMapPtr map, WayPtr disconnected, WayPtr connectTo);

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

  // allow for optionally tagging the snapped node; useful for debugging
  bool _markSnappedNodes;

  // the feature criterion to be used for way snap target candidates
  QString _wayToSnapToCriterionClassName;
  // the feature criterion to be used for way snap source candidates
  QString _wayToSnapCriterionClassName;
  // the feature criterion to be used for way snap target candidates
  QString _wayNodeToSnapToCriterionClassName;
  // the status criterion to be used for the snap source way
  Status _snapWayStatus;
  // the status criterion to be used for the snap target way or way node
  Status _snapToWayStatus;

  // feature indexes used for way nodes being snapped to
  boost::shared_ptr<Tgs::HilbertRTree> _snapToWayNodeIndex;
  std::deque<ElementId> _snapToWayNodeIndexToEid;

  // feature indexes used for ways being snapped to
  boost::shared_ptr<Tgs::HilbertRTree> _snapToWayIndex;
  std::deque<ElementId> _snapToWayIndexToEid;

  // keep track of the way nodes that are snapped
  QList<long> _snappedWayNodeIds;
  long _numSnappedToWays;
  long _numSnappedToWayNodes;

  int _taskStatusUpdateInterval;
  OsmMapPtr _map;
  Settings _conf;

  /*
   * The radius around the end node to look for ways to snap to.
   */
  Meters _getWaySearchRadius(const ConstElementPtr& e) const;

  /*
   * The radius around the end node to look for way nodes to snap to.
   */
  Meters _getWayNodeSearchRadius(const ConstElementPtr& e) const;

  /*
   * Creates the criterion used to determine via filtering which features we want to snap or snap to
   *
   * @param criterionClassName the name of a hoot ElementCriterion class
   * @param status a hoot status; either Unknown1 or Unknown2
   * @return an element criterion
   */
  ElementCriterionPtr _createFeatureCriterion(const QString criterionClassName,
                                              const Status& status);
  /*
   * Creates an index needed when searching for features to snap to
   *
   * @param featureCrit the element criterion for the feature type being indexed
   * @param featureIndex a pointer to the geospatial index being created
   * @param featureIndexToEid a pointer to the element ID index being created
   * @param elementType the element type of the criterion class; either Way or Node
   */
  void _createFeatureIndex(ElementCriterionPtr featureCrit,
                           boost::shared_ptr<Tgs::HilbertRTree>& featureIndex,
                           std::deque<ElementId>& featureIndexToEid,
                           const ElementType& elementType);

  /*
   * Identifies unconnected way nodes
   *
   * @param way the way to examine
   * @param wayCrit an optional element criterion to restrict the types of ways being examined
   * @return a collection of node IDs
   */
  std::set<long> _getUnconnectedEndNodeIds(const ConstWayPtr& way,
                                           const ElementCriterionPtr& wayCrit =
                                             ElementCriterionPtr()) const;
  /*
   * Return feature candidates to snap to
   *
   * @param node the node attempting snapping
   * @param elementType the element type of the feature being snapped to; either Way or Node
   * @return a collection of element IDs
   */
  std::set<ElementId> _getNearbyFeaturesToSnapTo(const ConstNodePtr& node,
                                                 const ElementType& elementType) const;
  /*
   * Determines where in a way to snap an unconnected end node
   *
   * @param nodeToSnap the node being snapped
   * @param snapToWay the way being snapped to
   * @return a way node index
   */
  int _getNodeToSnapWayInsertIndex(NodePtr nodeToSnap, const ConstWayPtr& snapToWay) const;

  /*
   * Attempts to snap an unconnected way end node to another way node
   *
   * @param nodeToSnap the node to attempt to snap
   * @return true if the node was snapped; false otherwise
   */
  bool _snapUnconnectedNodeToWayNode(NodePtr nodeToSnap);

  /*
   * Attempts to snap an unconnected way end node to another way
   *
   * @param nodeToSnap the node to attempt to snap
   * @return true if the node was snapped; false otherwise
   */
  bool _snapUnconnectedNodeToWay(NodePtr nodeToSnap);

  /**
   * @brief _snapUnconnectedNodeToWay Snap a particular node into a way at its closest intersecting point
   * @param nodeToSnap Node to snap/add into the way
   * @param wayToSnapTo Way to connect/add the node into
   * @return True if successful
   */
  bool _snapUnconnectedNodeToWay(NodePtr nodeToSnap, WayPtr wayToSnapTo);

  /**
   * @brief snapClosestEndpointToWay Finds the closest endpont on 'disconnected' and snaps it to
   *   the closest node in 'connectTo'
   * @param disconnected Disconnected way that needs to be connected
   * @param connectTo Way to connect the disconnected way to
   * @return True if successful
   */
  bool _snapClosestEndpointToWay(WayPtr disconnected, WayPtr connectTo);
};

}

#endif // UNCONNECTED_WAY_SNAPPER
