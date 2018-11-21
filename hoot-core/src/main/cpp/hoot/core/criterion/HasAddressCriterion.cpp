
#include "HasAddressCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, HasAddressCriterion)

HasAddressCriterion::HasAddressCriterion()
{
}

void HasAddressCriterion::setConfiguration(const Settings& conf)
{
  _addressParser.setConfiguration(conf);
}

bool HasAddressCriterion::isSatisfied(const boost::shared_ptr<const Element>& e) const
{
  return _addressParser.hasAddress(e);
}

}

