
#ifndef SNAP_UNCONNECTED_ROADS
#define SNAP_UNCONNECTED_ROADS

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/info/OperationStatusInfo.h>

namespace hoot
{

class SnapUnconnectedRoads : public OsmMapOperation, public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::SnapUnconnectedRoads"; }

  SnapUnconnectedRoads();

  virtual void apply(OsmMapPtr& map);

  virtual std::string getClassName() const { return className(); }

  virtual QString getInitStatusMessage() const
  { return "Snapping unconnected roads to nearest road..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Snapped " + QString::number(_numAffected) + " roads."; }

  virtual QString getDescription() const
  { return "Snaps unconnected road end points to the nearest road node."; }

private:

  Meters _getSearchRadius(const boost::shared_ptr<const Element>& e) const;
};

}

#endif // SNAP_UNCONNECTED_ROADS
