
#include "AddressCountVisitor.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/AddressParser.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, AddressCountVisitor)

AddressCountVisitor::AddressCountVisitor() :
_count(0)
{
}

void AddressCountVisitor::visit(const boost::shared_ptr<Element>& e)
{
  _count += AddressParser::numAddresses(*e);
}

}
