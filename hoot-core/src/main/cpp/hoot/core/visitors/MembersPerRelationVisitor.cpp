
#include "MembersPerRelationVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, MembersPerRelationVisitor)

MembersPerRelationVisitor::MembersPerRelationVisitor()
{
}

void MembersPerRelationVisitor::visit(const ConstElementPtr& /*e*/)
{

}

}
