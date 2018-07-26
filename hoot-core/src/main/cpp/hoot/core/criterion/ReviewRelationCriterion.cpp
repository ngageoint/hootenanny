
#include "ReviewRelationCriterion.h"

// Hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MetadataTags.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, ReviewRelationCriterion)

bool ReviewRelationCriterion::isSatisfied(const ConstElementPtr& e) const
{
  return e->getElementType() == ElementType::Relation &&
         e->getTags().contains(MetadataTags::HootReviewNeeds());
}

}
