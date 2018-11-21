
#include "NormalizePhoneNumbersVisitor.h"

// Hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, NormalizePhoneNumbersVisitor)

NormalizePhoneNumbersVisitor::NormalizePhoneNumbersVisitor()
{
}

void NormalizePhoneNumbersVisitor::setConfiguration(const Settings& conf)
{
  _phoneNumberNormalizer.setConfiguration(conf);
}

void NormalizePhoneNumbersVisitor::visit(const ElementPtr& e)
{
  _phoneNumberNormalizer.normalizePhoneNumbers(e);
}

}
