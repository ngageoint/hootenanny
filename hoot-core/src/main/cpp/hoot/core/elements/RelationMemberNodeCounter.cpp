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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "RelationMemberNodeCounter.h"

// hoot


namespace hoot
{

int RelationMemberNodeCounter::numNodes(const ConstRelationPtr& relation) const
{
  int count = 0;
  const std::vector<RelationData::Entry> members = relation->getMembers();
  for (size_t i = 0; i < members.size(); i++)
  {
    const RelationData::Entry member = members[i];
    const ElementType type = member.getElementId().getType();
    if (type == ElementType::Node)
    {
      count++;
    }
    else if (type == ElementType::Way)
    {
      ConstWayPtr way = _map->getWay(member.getElementId().getId());
      if (way)
      {
        count += way->getNodeCount();
      }
    }
    else if (type == ElementType::Relation)
    {
      ConstRelationPtr r = _map->getRelation(member.getElementId().getId());
      if (r)
      {
        count += numNodes(r);
      }
    }
  }
  return count;
}

}
