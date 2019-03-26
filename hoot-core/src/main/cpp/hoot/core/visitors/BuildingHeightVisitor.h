
#ifndef BUILDING_HEIGHT_VISITOR_H
#define BUILDING_HEIGHT_VISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/info/NumericStatistic.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/info/OperationStatusInfo.h>

namespace hoot
{

class BuildingHeightVisitor : public ConstElementVisitor, public NumericStatistic,
  public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::BuildingHeightVisitor"; }

  BuildingHeightVisitor();

  virtual void visit(const ConstElementPtr& e) override;

  virtual QString getDescription() const { return "Calculates building height statistics"; }

  virtual QString getInitStatusMessage() const
  { return "Calculating building height statistics..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Calculated height statistics for " + QString::number(_numAffected) + " buildings"; }

  virtual double getStat() const { return _totalHeight; }
  virtual double getMin() const { return _minHeight; }
  virtual double getMax() const { return _maxHeight; }
  virtual double getAverage() const { return _totalHeight / _numHeightsProcessed; }

private:

  BuildingCriterion _crit;

  // in meters
  double _totalHeight;
  double _numHeightsProcessed;
  double _minHeight;
  double _maxHeight;

  double _getHeight(const ConstElementPtr& e) const;
};

}

#endif // BUILDING_HEIGHT_VISITOR_H
