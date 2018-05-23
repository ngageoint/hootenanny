
#include "RailwayCriterion.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, RailwayCriterion)

RailwayCriterion::RailwayCriterion()
{
}

bool RailwayCriterion::isSatisfied(const boost::shared_ptr<const Element> &e) const
{
  return OsmSchema::getInstance().isRailway(*e);
}

}
