
#include "BuildingHeightVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, BuildingHeightVisitor)

BuildingHeightVisitor::BuildingHeightVisitor()
{
}

void BuildingHeightVisitor::visit(const ConstElementPtr& /*e*/)
{

}

}
