
#include "MembersPerRelationVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Relation.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, MembersPerRelationVisitor)

MembersPerRelationVisitor::MembersPerRelationVisitor() :
_totalMembers(0),
_numRelations(0),
_minMembersPerRelation(INT_MAX),
_maxMembersPerRelation(0)
{
}

void MembersPerRelationVisitor::visit(const ConstElementPtr& e)
{
  if (_crit.isSatisfied(e))
  {
    ConstRelationPtr relation = boost::dynamic_pointer_cast<const Relation>(e);
    const int numMembers = relation->getMembers().size();
    _totalMembers += numMembers;
    if (numMembers < _minMembersPerRelation)
    {
      _minMembersPerRelation = numMembers;
    }
    if (numMembers > _maxMembersPerRelation)
    {
      _maxMembersPerRelation = numMembers;
    }
    _numAffected++;
  }
}

}
