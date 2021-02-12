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
 * @copyright Copyright (C) 2018 Maxar (http://www.maxar.com/)
 */

#include "InWayNodeCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, InWayNodeCriterion)

InWayNodeCriterion::InWayNodeCriterion(const OsmMap& map, const std::vector<long>& wayIds)
{
  for (size_t i = 0; i < wayIds.size(); i++)
  {
    ConstWayPtr w = map.getWay(wayIds[i]);
    const std::vector<long>& nids = w->getNodeIds();

    _nids.insert(nids.begin(), nids.end());
  }
}

bool InWayNodeCriterion::isSatisfied(const ConstElementPtr& e) const
{
  return _nids.find(e->getId()) != _nids.end();
}

}
