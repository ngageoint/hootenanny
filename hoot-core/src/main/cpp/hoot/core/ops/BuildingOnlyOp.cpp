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
#include "BuildingOnlyOp.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/ops/RemoveRelationOp.h>
#include <hoot/core/visitors/RemoveTagVisitor.h>
#include <hoot/core/visitors/ReplaceTagVisitor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/OsmMap.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, BuildingOnlyOp)

BuildingOnlyOp::BuildingOnlyOp()
{
}

void BuildingOnlyOp::apply(boost::shared_ptr<OsmMap>& map)
{
  _map = map;

  // Remove superfluous tags
  RemoveTagVisitor removeVisitor("error:circular");
  removeVisitor.addKey("OBJECTID");
  removeVisitor.addKey("PAGENUMBER");
  removeVisitor.addKey("SHAPE_AREA");
  removeVisitor.addKey("SHAPE_LENG");
  removeVisitor.addKey("hoot:layername");
  map->visitRw(removeVisitor);

  // Change uppercase "BUILDING" tag to lower
  ReplaceTagVisitor replaceVisitor("BUILDING", "yes",
                                   "building", "yes");
  map->visitRw(replaceVisitor);

  // Go through all the relations, find which ones to remove
  std::vector<long> rIdsToRemove;
  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    const boost::shared_ptr<Relation>& r = it->second;

    // Is it a building relation?
    if (r->getType() == MetadataTags::RelationBuilding())
      rIdsToRemove.push_back(r->getId());
  }

  // Now do the removing
  for (size_t i = 0; i < rIdsToRemove.size(); i++)
    RemoveRelationOp::removeRelation(map, rIdsToRemove[i]);
}

} // namespace hoot
