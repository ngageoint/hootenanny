
#ifndef SNAP_UNCONNECTED_WAYS
#define SNAP_UNCONNECTED_WAYS

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * This class is set up to generically snap an unconnected way endpoint node to another way, using
 * the input criteria to determine which types of ways to snap.  As of 3/15/19, this has only ever
 * been tested with roads, however.
 *
 * The main impetus of this class was to make the output of Differential Conflation using the
 * Network Algorithm better by cleaning up unconnected roads.  However, this could be used as a
 * cleanup op after any conflation.  Future efforts, however if possible, should focus on trying to
 * fix the lack of snapping in the conflation routines themselves if possible rather than using this
 * as a cleanup utility.
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
  ElementCriterionPtr _wayToSnapCriterion;
  ElementCriterionPtr _wayToSnapToCriterion;
  ElementCriterionPtr _wayNodeToSnapToCriterion;

  QList<long> _snappedRoadNodes;
  QList<long> _snappedToRoadNodes;

  int _taskStatusUpdateInterval;

  // Specifying the element as input here allows for optionally using its CE in the search radius
  // calculation.
  Meters _getSearchRadius(const boost::shared_ptr<const Element>& e) const;
};

}

#endif // SNAP_UNCONNECTED_WAYS
