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
#include "WayEndNodeCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/WayUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, WayEndNodeCriterion)

WayEndNodeCriterion::WayEndNodeCriterion(const bool allowShared) :
WayNodeCriterion(),
_allowShared(allowShared)
{

}

WayEndNodeCriterion::WayEndNodeCriterion(ConstOsmMapPtr map, const bool allowShared) :
WayNodeCriterion(map),
_allowShared(allowShared)
{
}

bool WayEndNodeCriterion::isSatisfied(const ConstElementPtr& e) const
{
  if (!e)
  {
    return false;
  }

  // See if its a way node first.
  if (!WayNodeCriterion::isSatisfied(e))
  {
    return false;
  }
  // Passing the WayNodeCriterion check tells us this element is a node.
  assert(e->getElementType() == ElementType::Node);

  // Get all the ways this node belongs to.
  const std::set<long> containingWayIds = WayUtils::getContainingWayIds(e->getId(), _map);
  assert(containingWayIds.size() >= 1);
  // If we're not allowing the way node to be shared, then return false if more than one way
  // contains it.
  if (!_allowShared && containingWayIds.size() > 1)
  {
    return false;
  }

  // Otherwise, return true if this node is the end node of any way that contains it and that way
  // isn't a closed poly.
  for (std::set<long>::const_iterator itr = containingWayIds.begin(); itr != containingWayIds.end();
       ++itr)
  {
    ConstWayPtr way = _map->getWay(*itr);
    if (way && !way->isClosedArea() && way->isEndNode(e->getId()))
    {
      return true;
    }
  }

  return false;
}

}
