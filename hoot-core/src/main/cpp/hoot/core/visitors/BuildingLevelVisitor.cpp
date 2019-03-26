
#include "BuildingLevelVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, BuildingLevelVisitor)

BuildingLevelVisitor::BuildingLevelVisitor()
{
}

void BuildingLevelVisitor::visit(const ConstElementPtr& /*e*/)
{

}

}
