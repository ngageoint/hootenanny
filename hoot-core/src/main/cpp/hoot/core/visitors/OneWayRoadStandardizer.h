
#ifndef ONE_WAY_ROAD_STANDARDIZER_H
#define ONE_WAY_ROAD_STANDARDIZER_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/info/OperationStatusInfo.h>

namespace hoot
{

/**
 *
 */
class OneWayRoadStandardizer : public ElementVisitor, public OperationStatusInfo
{

public:

  static std::string className() { return "hoot::OneWayRoadStandardizer"; }

  OneWayRoadStandardizer();

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual QString getInitStatusMessage()
  { return "Standardizing one way roads..."; }

  virtual QString getCompletedStatusMessage()
  { return "Standardized " + QString::number(_numAffected) + " one way roads."; }

  virtual QString getDescription() const
  { return ""; }
};

}

#endif // ONE_WAY_ROAD_STANDARDIZER_H
