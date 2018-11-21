
#ifndef HAS_PHONE_NUMBER_CRITERION_H
#define HAS_PHONE_NUMBER_CRITERION_H

#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/algorithms/PhoneNumberParser.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Identifies features with phone numbers
 */
class HasPhoneNumberCriterion : public ElementCriterion, public Configurable
{
public:
  static std::string className() { return "hoot::HasPhoneNumberCriterion"; }

  HasPhoneNumberCriterion();

  virtual void setConfiguration(const Settings& conf);

  virtual bool isSatisfied(const boost::shared_ptr<const Element>& e) const;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new HasPhoneNumberCriterion()); }

  virtual QString getDescription() const { return "Identifies features with phone numbers"; }

private:

  PhoneNumberParser _phoneNumberParser;
};

}
#endif // HAS_PHONE_NUMBER_CRITERION_H
