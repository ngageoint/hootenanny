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
 * @copyright Copyright (C) 2016-2023 Maxar (http://www.maxar.com/)
 */
#include "HighwayIntersectionCriterion.h"

// hoot
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, HighwayIntersectionCriterion)

bool HighwayIntersectionCriterion::isSatisfied(const ConstElementPtr& e) const
{
  if (e->getElementType() != ElementType::Node)
    return false;

  std::shared_ptr<NodeToWayMap> n2w = _map->getIndex().getNodeToWayMap();
  long id = e->getId();

  const set<long>& wids = n2w->getWaysByNode(id);

  // find all ways that are highways (ie roads)
  set<long> hwids;
  for (auto way_id : wids)
  {
    ConstWayPtr w = _map->getWay(way_id);
    if (HighwayCriterion(_map).isSatisfied(w))
      hwids.insert(way_id);
  }

  // three or more ways meeting at a node is an intersection
  return (hwids.size() >= 3);
}

}
