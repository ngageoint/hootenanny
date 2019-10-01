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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BuildingWayNodeCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/criterion/BuildingCriterion.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, BuildingWayNodeCriterion)

BuildingWayNodeCriterion::BuildingWayNodeCriterion()
{
}

BuildingWayNodeCriterion::BuildingWayNodeCriterion(ConstOsmMapPtr map) :
_map(map)
{
}

bool BuildingWayNodeCriterion::isSatisfied(const ConstElementPtr& e) const
{
  bool result = false;

  if (!_map)
  {
    throw HootException("You must set the map before calling BuildingWayNodeCriterion");
  }

  if (e->getElementType() == ElementType::Node)
  {
    const set<long> waysContainingNode =
      _map->getIndex().getNodeToWayMap()->getWaysByNode(e->getElementId().getId());
    for (set<long>::const_iterator it = waysContainingNode.begin();
         it != waysContainingNode.end(); ++it)
    {
      const long wayId = *it;
      if (_map->containsElement(ElementId(ElementType::Way, wayId)) &&
          BuildingCriterion().isSatisfied(_map->getWay(wayId)))
      {
        result = true;
        break;
      }
    }
  }

  return result;
}

long BuildingWayNodeCriterion::getMatchingWayId(const ConstElementPtr& e)
{
  long result = 0;

  if (!_map)
  {
    throw HootException("You must set the map before calling BuildingWayNodeCriterion");
  }

  if (e->getElementType() == ElementType::Node)
  {
    const set<long> waysContainingNode =
      _map->getIndex().getNodeToWayMap()->getWaysByNode(e->getElementId().getId());
    for (set<long>::const_iterator it = waysContainingNode.begin(); it != waysContainingNode.end();
         ++it)
    {
      const long wayId = *it;
      if (_map->containsElement(ElementId(ElementType::Way, wayId)) &&
          BuildingCriterion().isSatisfied(_map->getWay(wayId)))
      {
        return wayId;
        break;
      }
    }
  }

  return result;
}

void BuildingWayNodeCriterion::setOsmMap(const OsmMap* map)
{
  _map = map->shared_from_this();
}

ElementCriterionPtr BuildingWayNodeCriterion::clone()
{
  return ElementCriterionPtr(new BuildingWayNodeCriterion(_map));
}

}
