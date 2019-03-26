
#ifndef TUNNEL_CRITERION_H
#define TUNNEL_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * Identifies tunnels
 */
class TunnelCriterion : public ElementCriterion
{
public:

  static std::string className() { return "hoot::TunnelCriterion"; }

  TunnelCriterion();

  virtual bool isSatisfied(const ConstElementPtr& e) const;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new TunnelCriterion()); }

  virtual QString getDescription() const { return "Identifies tunnels"; }
};

}

#endif // BRIDGE_CRITERION_H
