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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ElementIdRemapper.h"

//  Hoot
#include <hoot/core/ops/IdSwapOp.h>
#include <hoot/core/conflate/IdSwap.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, ElementIdRemapper)

ElementIdRemapper::ElementIdRemapper() :
_remapNodes(true),
_remapWays(true),
_remapRelations(true)
{
}

void ElementIdRemapper::apply(OsmMapPtr& map)
{
  if (_remapNodes)
  {
    NodeMap allNodes = map->getNodes();
    for (NodeMap::iterator it = allNodes.begin(); it != allNodes.end(); ++it)
    {
      NodePtr node = it->second;
      map->getIdSwap()->add(
        node->getElementId(), ElementId(ElementType::Node, _idGen->createNodeId()));
    }
  }

  if (_remapWays)
  {
    WayMap allWays = map->getWays();
    for (WayMap::iterator it = allWays.begin(); it != allWays.end(); ++it)
    {
      WayPtr way = it->second;
      map->getIdSwap()->add(
        way->getElementId(), ElementId(ElementType::Way, _idGen->createWayId()));
    }
  }

  if (_remapRelations)
  {
    RelationMap allRelations = map->getRelations();
    for (RelationMap::iterator it = allRelations.begin(); it != allRelations.end(); ++it)
    {
      RelationPtr relation = it->second;
      map->getIdSwap()->add(
        relation->getElementId(), ElementId(ElementType::Relation, _idGen->createRelationId()));
    }
  }

  IdSwapOp().apply(map);
}

}
