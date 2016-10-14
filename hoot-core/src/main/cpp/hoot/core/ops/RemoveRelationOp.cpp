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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RemoveRelationOp.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/util/Validate.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RemoveRelationOp)

RemoveRelationOp::RemoveRelationOp()
{
}

RemoveRelationOp::RemoveRelationOp(long rId):
  _rIdToRemove(rId)
{
}

void RemoveRelationOp::apply(shared_ptr<OsmMap>& map)
{
  if (map->_relations.find(_rIdToRemove) != map->_relations.end())
  {
    // determine if this relation is a part of any other relations
    // make a copy of the rids in case the index gets changed.
    const set<long> rids = map->_index->getElementToRelationMap()->
      getRelationByElement(ElementId::relation(_rIdToRemove));

    // remove this relation from all other parent relations.
    for (set<long>::const_iterator it = rids.begin(); it != rids.end(); ++it)
    {
      map->getRelation(*it)->removeElement(ElementId::relation(_rIdToRemove));
    }

    map->_index->removeRelation(map->getRelation(_rIdToRemove));
    map->_relations.erase(_rIdToRemove);
    VALIDATE(map->validate());
  }
}

} // end namespace hoot

