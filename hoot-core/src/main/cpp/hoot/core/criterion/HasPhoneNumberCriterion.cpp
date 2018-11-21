
#include "HasPhoneNumberCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, HasPhoneNumberCriterion)

HasPhoneNumberCriterion::HasPhoneNumberCriterion()
{
}

void HasPhoneNumberCriterion::setConfiguration(const Settings& conf)
{
  _phoneNumberParser.setConfiguration(conf);
}

bool HasPhoneNumberCriterion::isSatisfied(const boost::shared_ptr<const Element>& e) const
{
  return _phoneNumberParser.hasPhoneNumber(e);
}

}

