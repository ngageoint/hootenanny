
#include "AddressCountVisitor.h"

// Hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, AddressCountVisitor)

AddressCountVisitor::AddressCountVisitor() :
_count(0)
{
}

void AddressCountVisitor::setConfiguration(const Settings& conf)
{
  _addressParser.setConfiguration(conf);
}

void AddressCountVisitor::visit(const boost::shared_ptr<Element>& e)
{
  _count += _addressParser.numAddresses(e);
}

}
