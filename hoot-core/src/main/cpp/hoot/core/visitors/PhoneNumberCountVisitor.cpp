
#include "PhoneNumberCountVisitor.h"

// Hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, PhoneNumberCountVisitor)

PhoneNumberCountVisitor::PhoneNumberCountVisitor() :
_count(0)
{
}

void PhoneNumberCountVisitor::visit(const ConstElementPtr& e)
{
  _count += _phoneNumberParser.numPhoneNumbers(e);
}

}
