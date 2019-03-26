
#include "TunnelCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, TunnelCriterion)

TunnelCriterion::TunnelCriterion()
{
}

bool TunnelCriterion::isSatisfied(const ConstElementPtr& e) const
{
  const Tags& tags = e->getTags();
  return !tags.isFalse("tunnel") && !tags.get("tunnel").trimmed().isEmpty();
}

}
