
#include "NormalizePhoneNumbersVisitor.h"

// Hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, NormalizePhoneNumbersVisitor)

NormalizePhoneNumbersVisitor::NormalizePhoneNumbersVisitor()
{
}

void NormalizePhoneNumbersVisitor::visit(const ElementPtr& e)
{
  _phoneNumberParser.normalizePhoneNumbers(e);
}

}
