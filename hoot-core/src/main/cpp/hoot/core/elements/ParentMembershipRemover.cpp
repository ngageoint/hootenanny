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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "ParentMembershipRemover.h"

// Hoot
#include <hoot/core/elements/RelationMemberUtils.h>
#include <hoot/core/elements/WayUtils.h>

namespace hoot
{

int ParentMembershipRemover::removeMemberships(
  const ElementId& elementId, const ConstOsmMapPtr& map)
{
  LOG_TRACE("Removing parent memberships for " << elementId << "...");

  int numRemoved = 0;

  std::vector<RelationPtr> relations = RelationMemberUtils::getContainingRelations(elementId, map);
  LOG_VART(relations.size());
  for (std::vector<RelationPtr>::const_iterator itr = relations.begin(); itr != relations.end();
       ++itr)
  {
    RelationPtr relation = *itr;
    LOG_VART(relation->contains(elementId));
    if (relation->contains(elementId))
    {
      relation->removeElement(elementId);
      LOG_VART(relation->contains(elementId));
      numRemoved++;
    }
  }

  if (elementId.getType() == ElementType::Node)
  {
    std::vector<WayPtr> ways = WayUtils::getContainingWays(elementId.getId(), map);
    LOG_VART(ways.size());
    for (std::vector<WayPtr>::const_iterator itr = ways.begin(); itr != ways.end(); ++itr)
    {
      WayPtr way = *itr;
      LOG_VART(way->containsNodeId(elementId.getId()));
      if (way->containsNodeId(elementId.getId()))
      {
        way->removeNode(elementId.getId());
        LOG_VART(way->containsNodeId(elementId.getId()));
        numRemoved++;
      }
    }
  }

  return numRemoved;
}

}
