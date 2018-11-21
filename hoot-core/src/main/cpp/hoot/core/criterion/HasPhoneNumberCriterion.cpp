
#include "HasPhoneNumberCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, HasPhoneNumberCriterion)

bool HasPhoneNumberCriterion::isSatisfied(const boost::shared_ptr<const Element>& e) const
{
  return _phoneNumberParser.hasPhoneNumber(e);
}

}

