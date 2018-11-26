
#include "PhoneNumberLocateVisitor.h"

#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

PhoneNumberLocateVisitor::PhoneNumberLocateVisitor()
{

}

void PhoneNumberLocateVisitor::visit(const ElementPtr& e)
{
  QList<ElementPhoneNumber> phoneNumbers = _phoneNumberParser.parsePhoneNumbers(e);
  for (QList<ElementPhoneNumber>::const_iterator phoneNumberItr = phoneNumbers.constBegin();
       phoneNumberItr != phoneNumbers.constEnd(); ++phoneNumberItr)
  {
    ElementPhoneNumber phoneNumber = *phoneNumberItr;
    const QString locationDescription =
      _phoneNumberLocator.getLocationDescription(phoneNumber.tagValue);
    if (!locationDescription.isEmpty())
    {
      e->getTags().set(phoneNumber.tagKey + ":description", locationDescription);
    }
  }
}

}
