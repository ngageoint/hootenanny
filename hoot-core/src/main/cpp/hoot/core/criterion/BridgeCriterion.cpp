
#include "BridgeCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, BridgeCriterion)

BridgeCriterion::BridgeCriterion()
{
}

bool BridgeCriterion::isSatisfied(const ConstElementPtr& e) const
{
  return !e->getTags().isFalse("bridge") && e->getTags().contains("bridge");
}

}
