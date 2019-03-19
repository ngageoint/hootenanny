
#ifndef SNAP_UNCONNECTED_WAYS
#define SNAP_UNCONNECTED_WAYS

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
 * This class is set up to snap an unconnected way endpoint node to another way, using the element
 * input criteria to determine which types of ways to snap.  As of 3/15/19, this has only ever
 * been tested with roads, however.
 *
 * The main impetus of this class was to specifically make the road output of Differential
 * Conflation using the Network Algorithm (with rubber sheeting as a pre conflate op) better by
 * cleaning up unconnected roads.  However, this could also be used as a cleanup op after other
 * types of conflation.  Future efforts, however if possible, should focus on trying to fix the
 * lack of snapping in the conflation routines themselves if possible rather than using this as an
 * after the fact cleanup utility.
 */
class SnapUnconnectedWays : public OsmMapOperation, public OperationStatusInfo, public Configurable
{
public:

  static std::string className() { return "hoot::SnapUnconnectedWays"; }

  SnapUnconnectedWays();

  virtual void apply(OsmMapPtr& map);

  virtual std::string getClassName() const { return className(); }

  virtual QString getInitStatusMessage() const
  { return "Snapping unconnected ways to nearest way..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Snapped " + QString::number(_numAffected) + " ways."; }

  virtual QString getDescription() const
  { return "Snaps unconnected way endpoints to the nearest way."; }

  virtual void setConfiguration(const Settings& conf);

private:

  double _maxNodeReuseDistance;
  double _maxSnapDistance;
  QString _snappedRoadsTagKey;
  QString _wayToSnapToCriterionClassName;
  QString _wayToSnapCriterionClassName;
  QString _wayNodeToSnapToCriterionClassName;
  Status _snapWayStatus;
  Status _snapToWayStatus;

  OsmMapPtr _map;

  boost::shared_ptr<Tgs::HilbertRTree> _snapToWayNodeIndex;
  std::deque<ElementId> _snapToWayNodeIndexToEid;
  boost::shared_ptr<Tgs::HilbertRTree> _snapToWayIndex;
  std::deque<ElementId> _snapToWayIndexToEid;

  // TODO: rename to ids
  QList<long> _snappedWayNodes;
  QList<long> _snappedToWayNodes;

  int _taskStatusUpdateInterval;

  Settings _conf;

  // Specifying the element as input here allows for optionally using its CE in the search radius
  // calculation.
  Meters _getWaySearchRadius(const boost::shared_ptr<const Element>& e) const;
  Meters _getWayNodeSearchRadius(const boost::shared_ptr<const Element>& e) const;

  ElementCriterionPtr _createFeatureCriterion(const QString criterionClassName,
                                              const Status& status);
  void _createFeatureIndex(ElementCriterionPtr featureCrit,
                           boost::shared_ptr<Tgs::HilbertRTree> index,
                           std::deque<ElementId>& indexToEid, const ElementType& elementType);

  std::set<long> _getUnconnectedWayEndNodeIds(const ConstWayPtr& way,
                                              const ElementCriterionPtr& wayCrit) const;
  std::set<ElementId> _getNearbyFeaturesToSnapTo(const ConstNodePtr& unconnectedWayEndNode,
                                                 const ElementType& elementType) const;
  int _getSnappedNodeInsertIndex(NodePtr unconnectedWayEndNode, const long closestSnapToWayNodeId,
                                 const ConstWayPtr& snapToWay) const;

  bool _snapUnconnectedNodeToWayNode(NodePtr unconnectedWayEndNode);
  bool _snapUnconnectedNodeToWay(NodePtr unconnectedWayEndNode);
};

}

#endif // SNAP_UNCONNECTED_WAYS
