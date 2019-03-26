
#include "NodesPerWayVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, NodesPerWayVisitor)

NodesPerWayVisitor::NodesPerWayVisitor()
{
}

void NodesPerWayVisitor::visit(const ConstElementPtr& /*e*/)
{

}

}
