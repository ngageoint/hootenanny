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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RemoveEmptyRelationsOp.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/ops/RemoveRelationOp.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RemoveEmptyRelationsOp)

RemoveEmptyRelationsOp::RemoveEmptyRelationsOp()
{
}

void RemoveEmptyRelationsOp::apply(OsmMapPtr& map)
{
  //MapProjector::projectToPlanar(map);
  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    _removeLeafRelationIfEmpty(map, it->second);
  }
}

void RemoveEmptyRelationsOp::_removeLeafRelationIfEmpty(OsmMapPtr& map, RelationPtr relation)
{
  if (relation->getMembers().size() > 0)
  {
    LOG_TRACE(
      "Relation not empty: " << relation->getId() << " child count: " <<
      relation->getMembers().size() << " children: " << relation->getMembers());
    const std::vector<RelationData::Entry> members = relation->getMembers();
    for (size_t i = 0; i < members.size(); i++)
    {
      ElementPtr member = map->getElement(members[i].getElementId());
      if (member->getElementType() == ElementType::Relation)
      {
        _removeLeafRelationIfEmpty(map, map->getRelation(member->getId()));
      }
    }
  }
  else
  {
    if (map->containsRelation(relation->getId()))
    {
      LOG_TRACE("Removing empty relation: " << relation->getId());
      RemoveRelationOp::removeRelation(map/*->shared_from_this()*/, relation->getId());
    }
  }
}

}
