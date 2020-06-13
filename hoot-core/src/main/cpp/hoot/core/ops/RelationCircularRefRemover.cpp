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

#include "RelationCircularRefRemover.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/Relation.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RelationCircularRefRemover)

void RelationCircularRefRemover::apply(OsmMapPtr& map)
{
  RelationMap relations =  map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    RelationPtr relation = it->second;
    const long relationId = relation->getId();
    LOG_VART(relationId);
    const std::vector<RelationData::Entry>& members = relation->getMembers();
    for (size_t i = 0; i < members.size(); i++)
    {
      const RelationData::Entry member = members.at(i);
      if (member.getElementId().getType() == ElementType::Relation)
      {
        const long memberRelationId = member.getElementId().getId();

        LOG_VART(memberRelationId);
        LOG_VART(_relationIdsToRelationMemberIds.size());
        LOG_VART(_relationIdsToRelationMemberIds.values(memberRelationId).size());

        if (relationId == memberRelationId)
        {
          LOG_TRACE(
            "Removing relation that contains itself as a member: " << memberRelationId << "...");
          relation->removeElement(ElementId(ElementType::Relation, memberRelationId));
          _numAffected++;
        }
        else if (_relationIdsToRelationMemberIds.values(memberRelationId).contains(relationId))
        {
          // This could leave some empty relations, but we'll let the empty relation removal
          // op/visitor handle that elsewhere.
          LOG_TRACE(
            "Removing relation member element with ID: " << memberRelationId <<
            " from relation with id: " << relationId << "...");
          relation->removeElement(ElementId(ElementType::Relation, memberRelationId));
          _numAffected++;
        }
        else
        {
          _relationIdsToRelationMemberIds.insert(relationId, memberRelationId);
        }
      }
    }
  }
}

}
