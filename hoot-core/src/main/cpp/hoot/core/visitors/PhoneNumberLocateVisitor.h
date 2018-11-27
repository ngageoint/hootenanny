
#ifndef PHONE_NUMBER_LOCATE_VISITOR_H
#define PHONE_NUMBER_LOCATE_VISITOR_H

// Hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/conflate/phone/PhoneNumberLocator.h>
#include <hoot/core/conflate/phone/PhoneNumberParser.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 *
 */
class PhoneNumberLocateVisitor : public ElementVisitor, public Configurable
{
public:

  PhoneNumberLocateVisitor();

  static std::string className() { return "hoot::PhoneNumberLocateVisitor"; }

  virtual void setConfiguration(const Settings& conf);

  virtual void visit(const ElementPtr& e);

  virtual QString getDescription() const
  { return "Determines admin level locations for elements based on phone number tags"; }

private:

  friend class PhoneNumberLocateVisitorTest;

  PhoneNumberParser _phoneNumberParser;
  PhoneNumberLocator _phoneNumberLocator;
};

}

#endif // PHONE_NUMBER_LOCATE_VISITOR_H
