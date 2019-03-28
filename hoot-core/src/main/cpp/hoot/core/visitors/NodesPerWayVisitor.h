
#ifndef NODES_PER_WAY_VISITOR_H
#define NODES_PER_WAY_VISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/info/NumericStatistic.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/criterion/WayCriterion.h>

namespace hoot
{

class NodesPerWayVisitor : public ConstElementVisitor, public NumericStatistic,
  public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::NodesPerWayVisitor"; }

  NodesPerWayVisitor();

  virtual void visit(const ConstElementPtr& e) override;

  virtual QString getDescription() const { return "Calculates way node statistics"; }

  virtual QString getInitStatusMessage() const
  { return "Calculating way node statistics..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Calculated node statistics for " + QString::number(_numAffected) + " ways"; }

  virtual long numWithStat() const { return _numAffected; }
  virtual double getStat() const { return _totalWayNodes; }
  virtual double getMin() const { return _minNodesPerWay; }
  virtual double getMax() const { return _maxNodesPerWay; }
  virtual double getAverage() const
  {
    const double average = _numAffected == 0 ? 0.0 : _totalWayNodes / _numAffected;
    return average;
  }

private:

  WayCriterion _crit;

  int _totalWayNodes;
  int _minNodesPerWay;
  int _maxNodesPerWay;
};

}

#endif // NODES_PER_WAY_VISITOR_H
