
#ifndef BUILDING_LEVELS_VISITOR_H
#define BUILDING_LEVELS_VISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/info/NumericStatistic.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/criterion/BuildingCriterion.h>

namespace hoot
{

/**
 *
 */
class BuildingLevelsVisitor : public ConstElementVisitor, public NumericStatistic
{
public:

  static std::string className() { return "hoot::BuildingLevelsVisitor"; }

  BuildingLevelsVisitor();

  virtual void visit(const ConstElementPtr& e) override;

  virtual QString getDescription() const { return "Calculates building level statistics"; }

  virtual QString getInitStatusMessage() const
  { return "Calculating building level statistics..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Calculated level statistics for " + QString::number(_numAffected) + " buildings"; }


  virtual double getStat() const { return _totalLevels; }
  virtual double getMin() const { return _minLevels; }
  virtual double getMax() const { return _maxLevels; }
  virtual double getAverage() const { return _totalLevels / _numLevelsProcessed; }

private:

  BuildingCriterion _crit;

  // in meters
  double _totalLevels;
  double _numLevelsProcessed;
  double _minLevels;
  double _maxLevels;

  int _getNumLevels(const ConstElementPtr& e) const;
};

}

#endif // BUILDING_LEVELS_VISITOR_H
