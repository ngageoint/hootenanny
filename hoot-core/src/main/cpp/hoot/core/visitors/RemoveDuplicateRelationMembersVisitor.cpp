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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RemoveDuplicateRelationMembersVisitor.h"

// Hoot
#include <hoot/core/elements/Relation.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QVector>
#include <QList>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, RemoveDuplicateRelationMembersVisitor)

RemoveDuplicateRelationMembersVisitor::RemoveDuplicateRelationMembersVisitor()
{
}

void RemoveDuplicateRelationMembersVisitor::visit(const ElementPtr& e)
{
  if (e->getElementType() == ElementType::Relation)
  {
    RelationPtr relation = boost::dynamic_pointer_cast<Relation>(e);
    const std::vector<RelationData::Entry>& members = relation->getMembers();
    LOG_VART(members.size());
    if (members.size() > 1)
    {
      // We want to retain the ordering of the members here, so won't use a set.
      QList<RelationData::Entry> parsedMembers;
      for (size_t i = 0; i < members.size(); i++)
      {
        const RelationData::Entry& member = members[i];
        if (!parsedMembers.contains(member))
        {
          parsedMembers.append(member);
        }
      }
      relation->setMembers(parsedMembers.toVector().toStdVector());
    }
  }
}

}
