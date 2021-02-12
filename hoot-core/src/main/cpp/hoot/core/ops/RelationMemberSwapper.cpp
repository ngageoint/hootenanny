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
 * @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)
 */

#include "RelationMemberSwapper.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/index/OsmMapIndex.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RelationMemberSwapper)

RelationMemberSwapper::RelationMemberSwapper() :
_includeReviewRelations(true)
{
}

RelationMemberSwapper::RelationMemberSwapper(
  const ElementId& idToReplace, const ElementId& idToReplaceWith,
  const bool includeReviewRelations) :
_idToReplace(idToReplace),
_idToReplaceWith(idToReplaceWith),
_includeReviewRelations(includeReviewRelations)
{
}

void RelationMemberSwapper::apply(const OsmMapPtr& map)
{
  LOG_TRACE(
    "Adding " << _idToReplaceWith << " to all relations " << _idToReplace << " was a member of " <<
    "and removing " << _idToReplace << " from them...")

  _numAffected = 0;

  const std::set<long> owningRelationIds =
    map->getIndex().getElementToRelationMap()->getRelationByElement(_idToReplace);
  for (std::set<long>::const_iterator it = owningRelationIds.begin(); it != owningRelationIds.end();
       ++it)
  {
    RelationPtr owningRelation = map->getRelation(*it);
    if (owningRelation &&
        (!_includeReviewRelations || owningRelation->getType() != MetadataTags::RelationReview()))
    {
      owningRelation->replaceElement(_idToReplace, _idToReplaceWith);
      _numAffected++;
    }
  }
}

void RelationMemberSwapper::swap(
  const ElementId& idToReplace, const ElementId& idToReplaceWith, const OsmMapPtr& map,
  const bool includeReviewRelations)
{
  RelationMemberSwapper(idToReplace, idToReplaceWith, includeReviewRelations).apply(map);
}

}
