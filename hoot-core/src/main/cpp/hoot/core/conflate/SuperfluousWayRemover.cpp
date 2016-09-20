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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "SuperfluousWayRemover.h"

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveWayOp.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, SuperfluousWayRemover)

SuperfluousWayRemover::SuperfluousWayRemover()
{

}

SuperfluousWayRemover::SuperfluousWayRemover(shared_ptr<OsmMap> map)
{
  _inputMap = map;
}


void SuperfluousWayRemover::removeWays(shared_ptr<OsmMap> map)
{
  SuperfluousWayRemover swr(map);
  return swr.removeWays();
}

void SuperfluousWayRemover::removeWays()
{
  LOG_INFO("Removing superfluous ways...");

  shared_ptr<ElementToRelationMap> e2r = _inputMap->getIndex().getElementToRelationMap();

  // make a copy of the ways to avoid issues when removing.
  const WayMap ways = _inputMap->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const shared_ptr<const Way>& w = it->second;

    bool same = true;
    long firstId;
    const vector<long>& nodeIds = w->getNodeIds();
    if (nodeIds.size() > 0)
    {
      firstId = nodeIds[0];

      for (size_t i = 1; i < nodeIds.size(); i++)
      {
        if (nodeIds[i] != firstId)
        {
          same = false;
        }
      }
    }

    bool inRelation = e2r->getRelationByElement(w).size() > 0;

    // if all the nodes in a way are the same or there are zero nodes
    if ((same || w->getTags().size() == 0) && !inRelation)
    {
      RemoveWayOp::removeWayFully(_inputMap, w->getId());
    }
  }
}

void SuperfluousWayRemover::apply(shared_ptr<OsmMap>& map)
{
  removeWays(map);
}

}
