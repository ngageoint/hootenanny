
#ifndef BRIDGE_CRITERION_H
#define BRIDGE_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * Identifies bridges
 */
class BridgeCriterion : public ElementCriterion
{
public:

  static std::string className() { return "hoot::BridgeCriterion"; }

  BridgeCriterion();

  virtual bool isSatisfied(const ConstElementPtr& e) const;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new BridgeCriterion()); }

  virtual QString getDescription() const { return "Identifies bridges"; }
};

}

#endif // BRIDGE_CRITERION_H
