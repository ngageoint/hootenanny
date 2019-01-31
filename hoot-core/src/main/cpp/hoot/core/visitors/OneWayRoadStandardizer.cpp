
#include "OneWayRoadStandardizer.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/ReversedRoadCriterion.h>
#include <hoot/core/elements/Way.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, OneWayRoadStandardizer)

OneWayRoadStandardizer::OneWayRoadStandardizer()
{
}

void OneWayRoadStandardizer::visit(const boost::shared_ptr<Element>& e)
{
  if (ReversedRoadCriterion().isSatisfied(e))
  {
    WayPtr road = boost::dynamic_pointer_cast<Way>(e);
    road->reverseOrder();
    road->getTags().set("oneway", "yes");
    _numAffected++;
  }
}

}
