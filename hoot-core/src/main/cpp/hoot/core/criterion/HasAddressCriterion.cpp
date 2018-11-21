
#include "HasAddressCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/AddressParser.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, HasAddressCriterion)

bool HasAddressCriterion::isSatisfied(const boost::shared_ptr<const Element>& e) const
{
  return AddressParser::hasAddress(*e);
}

}

