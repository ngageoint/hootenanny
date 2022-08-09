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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "BuildingOutlineRemoveOp.h"

// geos
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Polygon.h>

// hoot
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/GeometryToElementConverter.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, BuildingOutlineRemoveOp)

void BuildingOutlineRemoveOp::apply(std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;
  _map = map;

  // go through all the relations
  const RelationMap& relations = map->getRelations();
  for (auto it = relations.begin(); it != relations.end(); ++it)
  {
    const std::shared_ptr<Relation>& r = it->second;
    // remove the outline
    if (r->getType() == MetadataTags::RelationBuilding())
      _removeOutline(r);
  }
}

void BuildingOutlineRemoveOp::_removeOutline(const std::shared_ptr<Relation>& building)
{
  const vector<RelationData::Entry> entries = building->getMembers();
  for (const auto& member : entries)
  {
    if (member.getRole() == MetadataTags::RoleOutline())
    {
      building->removeElement(member.getRole(), member.getElementId());
      RecursiveElementRemover(member.getElementId()).apply(_map);
      _numAffected++;
    }
  }
}

QStringList BuildingOutlineRemoveOp::getCriteria() const
{
  return QStringList(BuildingCriterion::className());
}

}
