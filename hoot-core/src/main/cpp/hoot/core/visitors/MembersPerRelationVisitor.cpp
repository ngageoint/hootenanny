/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "MembersPerRelationVisitor.h"

// hoot
#include <hoot/core/elements/Relation.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, MembersPerRelationVisitor)

MembersPerRelationVisitor::MembersPerRelationVisitor()
  : _totalMembers(0),
    _minMembersPerRelation(0),
    _maxMembersPerRelation(0)
{
}

void MembersPerRelationVisitor::visit(const ConstElementPtr& e)
{
  if (_crit.isSatisfied(e))
  {
    ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(e);
    const int numMembers = relation->getMembers().size();
    _totalMembers += numMembers;
    if (_minMembersPerRelation == 0 || numMembers < _minMembersPerRelation)
      _minMembersPerRelation = numMembers;
    if (numMembers > _maxMembersPerRelation)
      _maxMembersPerRelation = numMembers;
    _numAffected++;
  }
}

}
