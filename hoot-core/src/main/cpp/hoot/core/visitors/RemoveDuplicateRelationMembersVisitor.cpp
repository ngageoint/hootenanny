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
 * @copyright Copyright (C) 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

#include "RemoveDuplicateRelationMembersVisitor.h"

// Hoot
#include <hoot/core/elements/Relation.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QVector>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveDuplicateRelationMembersVisitor)

void RemoveDuplicateRelationMembersVisitor::visit(const ElementPtr& e)
{
  if (!e)
  {
    return;
  }

  if (e->getElementType() == ElementType::Relation)
  {
    RelationPtr relation = std::dynamic_pointer_cast<Relation>(e);
    LOG_VART(relation->getElementId());
    const std::vector<RelationData::Entry>& members = relation->getMembers();
    if (members.size() > 1)
    {
      // use an ordered collection here; entry doesn't have an < implementation, otherwise
      // std::set could be used
      QList<RelationData::Entry> uniqueMembers;
      for (std::vector<RelationData::Entry>::const_iterator it = members.begin();
           it != members.end(); ++it)
      {
        if (!uniqueMembers.contains(*it))
        {
          uniqueMembers.append(*it);
        }
      }

      // no point in updating the members if there were no dupes
      if (uniqueMembers.size() < (int)members.size())
      {
        LOG_TRACE("Updating " << relation->getElementId() << "...");
        _numAffected += (members.size() - uniqueMembers.size());
        relation->setMembers(uniqueMembers.toVector().toStdVector());
      }
    }
  }
}

}
