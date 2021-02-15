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

#include "ConnectedRelationMemberFinder.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/SharedWayNodeFinder.h>

namespace hoot
{

bool ConnectedRelationMemberFinder::haveConnectedWayMembers(
  const ConstRelationPtr& relation1, const ConstRelationPtr& relation2) const
{
  if (relation1 && relation2)
  {
    std::set<ElementId> relation1WayMembers = relation1->getMemberIds(ElementType::Way);
    std::set<ElementId> relation2WayMembers = relation2->getMemberIds(ElementType::Way);
    for (std::set<ElementId>::const_iterator it = relation1WayMembers.begin();
         it != relation1WayMembers.end(); ++it)
    {
      const ElementId elementId1 = *it;
      for (std::set<ElementId>::const_iterator it2 = relation2WayMembers.begin();
           it2 != relation2WayMembers.end(); ++it2)
      {
        const ElementId elementId2 = *it2;
        if (elementId1 != elementId2)
        {
          ConstWayPtr way1 = _map->getWay(elementId1);
          ConstWayPtr way2 = _map->getWay(elementId1);
          if (way1 && way2)
          {
            if (SharedWayNodeFinder::waysShareEndNode(way1, way2, true))
            {
              LOG_TRACE("Found shared end node.");
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}

}
