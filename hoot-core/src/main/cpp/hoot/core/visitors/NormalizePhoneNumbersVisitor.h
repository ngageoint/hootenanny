
#ifndef NORMALIZE_PHONE_NUMBERS_VISITOR_H
#define NORMALIZE_PHONE_NUMBERS_VISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/algorithms/PhoneNumberParser.h>

namespace hoot
{

/**
 * Normalizes element phone numbers
 */
class NormalizePhoneNumbersVisitor : public ElementVisitor
{
public:

  static std::string className() { return "hoot::NormalizePhoneNumbersVisitor"; }

  NormalizePhoneNumbersVisitor();

  virtual void visit(const ElementPtr& e);

  virtual QString getDescription() const { return "Normalizes element phone numbers"; }

private:

  PhoneNumberParser _phoneNumberParser;
};

}

#endif // NORMALIZE_PHONE_NUMBERS_VISITOR_H
