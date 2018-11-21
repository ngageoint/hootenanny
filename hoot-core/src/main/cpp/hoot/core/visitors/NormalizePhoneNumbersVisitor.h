
#ifndef NORMALIZE_PHONE_NUMBERS_VISITOR_H
#define NORMALIZE_PHONE_NUMBERS_VISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/algorithms/PhoneNumberNormalizer.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Normalizes element phone numbers
 */
class NormalizePhoneNumbersVisitor : public ElementVisitor, public Configurable
{
public:

  static std::string className() { return "hoot::NormalizePhoneNumbersVisitor"; }

  NormalizePhoneNumbersVisitor();

  virtual void setConfiguration(const Settings& conf);

  virtual void visit(const ElementPtr& e);

  virtual QString getDescription() const { return "Normalizes element phone numbers"; }

private:

  PhoneNumberNormalizer _phoneNumberNormalizer;
};

}

#endif // NORMALIZE_PHONE_NUMBERS_VISITOR_H
