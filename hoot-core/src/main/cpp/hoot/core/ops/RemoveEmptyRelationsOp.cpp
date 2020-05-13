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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RemoveEmptyRelationsOp.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/RemoveRelationByEid.h>

// Qt
#include <QList>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RemoveEmptyRelationsOp)

RemoveEmptyRelationsOp::RemoveEmptyRelationsOp()
{
}

void RemoveEmptyRelationsOp::apply(OsmMapPtr& map)
{
  //Deleting twice in opposite directions catches child relations that are encountered after their
  //parent on the first pass.  This only works for child relations nested one deep.  At this point,
  //further empty relation nesting then one level hasn't been encountered.  If it is encountered,
  //then this will need to be reworked to delete empty relations recursively without breaking the
  //map.
  _deleteEmptyRelations(map, false);
  _deleteEmptyRelations(map, true);
}

void RemoveEmptyRelationsOp::_deleteEmptyRelations(const OsmMapPtr& map, const bool reverseOrder)
{
  const RelationMap& relations = map->getRelations();

  QList<long> relationIds;
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    RelationPtr relation = it->second;
    const long relationId = relation->getId();
    const long membersSize = relation->getMembers().size();
    if (membersSize == 0)
    {
      LOG_TRACE("Removing empty relation: " << relationId);
      if (relation->getTags().contains("name"))
      {
        LOG_TRACE("Empty relation name: " << relation->getTags().get("name"));
      }
      relationIds.append(relationId);
    }
    else
    {
      LOG_TRACE(
        "Relation " << relation->getElementId() << " has " << membersSize <<
        " members. Members: " << relation->getMembers());
    }
  }
  if (!reverseOrder)
  {
    qSort(relationIds);
  }
  else
  {
    qSort(relationIds.end(), relationIds.begin());
  }
  LOG_VART(relationIds);

  for (QList<long>::const_iterator it = relationIds.begin(); it != relationIds.end(); ++it)
  {
    RemoveRelationByEid::removeRelation(map, *it);
    _numAffected++;
  }
}

}
