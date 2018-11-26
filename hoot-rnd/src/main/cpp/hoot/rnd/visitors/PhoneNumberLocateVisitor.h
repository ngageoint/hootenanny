
#ifndef PHONE_NUMBER_LOCATE_VISITOR_H
#define PHONE_NUMBER_LOCATE_VISITOR_H

// Hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/rnd/conflate/phone/PhoneNumberLocator.h>

namespace hoot
{

/**
 *
 */
class PhoneNumberLocateVisitor : public ElementVisitor
{
public:

  PhoneNumberLocateVisitor();

  static std::string className() { return "hoot::PhoneNumberLocateVisitor"; }

  virtual void visit(const ElementPtr& e);

  virtual QString getDescription() const
  { return "Determines admin level locations for elements based on phone number tags"; }

private:

  PhoneNumberLocator _phoneNumberLocator;
};

}

#endif // PHONE_NUMBER_LOCATE_VISITOR_H
