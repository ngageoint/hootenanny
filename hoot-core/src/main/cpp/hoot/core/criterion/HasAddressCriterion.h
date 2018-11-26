
#ifndef HAS_ADDRESS_CRITERION_H
#define HAS_ADDRESS_CRITERION_H

#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/conflate/address/AddressParser.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Identifies features with addresses
 */
class HasAddressCriterion : public ElementCriterion, public Configurable
{
public:

  static std::string className() { return "hoot::HasAddressCriterion"; }

  HasAddressCriterion();

  virtual void setConfiguration(const Settings& conf);

  virtual bool isSatisfied(const boost::shared_ptr<const Element>& e) const;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new HasAddressCriterion()); }

  virtual QString getDescription() const { return "Identifies features with addresses"; }

private:

  AddressParser _addressParser;
};

}
#endif // HAS_ADDRESS_CRITERION_H
