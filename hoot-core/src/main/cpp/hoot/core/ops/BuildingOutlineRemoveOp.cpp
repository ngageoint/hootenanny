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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BuildingOutlineRemoveOp.h"

// geos
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Polygon.h>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/GeometryConverter.h>
#include <hoot/core/OsmMap.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, BuildingOutlineRemoveOp)

BuildingOutlineRemoveOp::BuildingOutlineRemoveOp()
{
}

void BuildingOutlineRemoveOp::apply(boost::shared_ptr<OsmMap>& map)
{
  _map = map;

  // go through all the relations
  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); it++)
  {
    const boost::shared_ptr<Relation>& r = it->second;
    // add the relation to a building group if appropriate
    if (r->getType() == "building")
    {
      _removeOutline(r);
    }
  }
}

void BuildingOutlineRemoveOp::_removeOutline(const boost::shared_ptr<Relation>& building)
{
  const vector<RelationData::Entry> entries = building->getMembers();
  for (size_t i = 0; i < entries.size(); i++)
  {
    if (entries[i].role == "outline")
    {
      building->removeElement(entries[i].role, entries[i].getElementId());
      RecursiveElementRemover(entries[i].getElementId()).apply(_map);
    }
  }
}

}
