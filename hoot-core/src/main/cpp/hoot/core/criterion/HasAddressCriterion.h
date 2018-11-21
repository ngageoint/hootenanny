
#ifndef HAS_ADDRESS_CRITERION_H
#define HAS_ADDRESS_CRITERION_H

#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * Identifies features with addresses
 */
class HasAddressCriterion : public ElementCriterion
{
public:

  static std::string className() { return "hoot::LinearCriterion"; }

  HasAddressCriterion() {}

  virtual bool isSatisfied(const boost::shared_ptr<const Element>& e) const;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new HasAddressCriterion()); }

  virtual QString getDescription() const { return "Identifies features with addresses"; }
};

}
#endif // HAS_ADDRESS_CRITERION_H
