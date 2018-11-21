
#include "NormalizeAddressesVisitor.h"

// Hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, NormalizeAddressesVisitor)

NormalizeAddressesVisitor::NormalizeAddressesVisitor()
{
}

void NormalizeAddressesVisitor::visit(const ElementPtr& /*e*/)
{
  //_addressNormalizer.normalizeAddresses(*e);
  //TODO: finish
}

}
