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
#include "HighwayNodeCriterion.h"

// hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, HighwayNodeCriterion)

HighwayNodeCriterion::HighwayNodeCriterion()
{
}

bool HighwayNodeCriterion::isSatisfied(const ConstElementPtr& e) const
{
  HighwayCriterion highwayCrit;
  if (e->getElementType() == ElementType::Node)
  {
    boost::shared_ptr<NodeToWayMap> nodeToWayMap = _map->getIndex().getNodeToWayMap();
    LOG_VART(nodeToWayMap->size());
    const std::set<long>& containingWays = nodeToWayMap->getWaysByNode(e->getId());
    LOG_VART(containingWays);
    for (std::set<long>::const_iterator containingWaysItr = containingWays.begin();
         containingWaysItr != containingWays.end(); ++containingWaysItr)
    {
      if (highwayCrit.isSatisfied(_map->getWay(*containingWaysItr)))
      {
        LOG_TRACE("Road node: " << e->getElementId() << " found in way: " << *containingWaysItr);
        return true;
      }
    }
  }
  return false;
}

}
