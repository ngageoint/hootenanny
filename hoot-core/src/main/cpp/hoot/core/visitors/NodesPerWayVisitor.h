
#ifndef NODES_PER_WAY_VISITOR_H
#define NODES_PER_WAY_VISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/info/NumericStatistic.h>

namespace hoot
{

/**
 *
 */
class NodesPerWayVisitor : public ConstElementVisitor, public NumericStatistic
{
public:

  static std::string className() { return "hoot::NodesPerWayVisitor"; }

  NodesPerWayVisitor();

  virtual void visit(const ConstElementPtr& e) override;

  virtual QString getDescription() const { return ""; }

  // TODO
  virtual double getStat() const { return 0.0; }
  virtual double getMin() const { return 0.0; }
  virtual double getMax() const { return 0.0; }
  virtual double getAverage() const { return 0.0; }
};

}

#endif // NODES_PER_WAY_VISITOR_H
