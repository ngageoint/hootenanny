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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ImmediatelyConnectedOutOfBoundsWayTagger.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, ImmediatelyConnectedOutOfBoundsWayTagger)

ImmediatelyConnectedOutOfBoundsWayTagger::ImmediatelyConnectedOutOfBoundsWayTagger() :
_strictBounds(true)
{
  _boundsChecker.setMustCompletelyContain(_strictBounds);
}

ImmediatelyConnectedOutOfBoundsWayTagger::ImmediatelyConnectedOutOfBoundsWayTagger(
  const bool strictBounds) :
_strictBounds(strictBounds)
{
  _boundsChecker.setMustCompletelyContain(_strictBounds);
}

void ImmediatelyConnectedOutOfBoundsWayTagger::apply(OsmMapPtr& map)
{  
  _numAffected = 0;
  _numProcessed = 0;
  std::set<long> directlyConnectedWayIds;
  _boundsChecker.setOsmMap(map.get());

  WayMap ways = map->getWays();
  std::shared_ptr<NodeToWayMap> nodeToWayMap = map->getIndex().getNodeToWayMap();
  for (WayMap::iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    WayPtr way = wayItr->second;
    LOG_TRACE("Examining source way: " << way->getElementId() << "...");

    // if the way hasn't already been tagged and falls within the bounds
    LOG_VART(directlyConnectedWayIds.find(way->getId()) == directlyConnectedWayIds.end());
    LOG_VART(_boundsChecker.isSatisfied(way));
    if (directlyConnectedWayIds.find(way->getId()) == directlyConnectedWayIds.end() &&
        _boundsChecker.isSatisfied(way))
    {
      // for each node in the way
      const std::vector<long> wayNodeIds = way->getNodeIds();
      LOG_VART(wayNodeIds.size());

      for (std::vector<long>::const_iterator wayNodeIdItr = wayNodeIds.begin();
           wayNodeIdItr != wayNodeIds.end(); ++wayNodeIdItr)
      {
        LOG_VART(*wayNodeIdItr);

        // find all ways directly connected it
        std::set<long> idsOfWaysContainingWayNode = nodeToWayMap->getWaysByNode(*wayNodeIdItr);
        LOG_VART(idsOfWaysContainingWayNode.size());
        for (std::set<long>::const_iterator connectedWayIdItr = idsOfWaysContainingWayNode.begin();
             connectedWayIdItr != idsOfWaysContainingWayNode.end(); ++connectedWayIdItr)
        {
          const long connectedWayId = *connectedWayIdItr;
          LOG_VART(connectedWayId);

          // if we haven't already tagged the connected way
          LOG_VART(directlyConnectedWayIds.find(connectedWayId) == directlyConnectedWayIds.end());
          if (directlyConnectedWayIds.find(connectedWayId) == directlyConnectedWayIds.end())
          {
            WayPtr connectedWay = map->getWay(connectedWayId);
            if (!connectedWay)
            {
              continue;
            }

            // and its *not* within the bounds
            LOG_VART(_boundsChecker.isSatisfied(connectedWay));
            if (!_boundsChecker.isSatisfied(connectedWay))
            {
              // then tag it
              LOG_TRACE("Tagging OOB connected way: " << connectedWay->getElementId());
              connectedWay->getTags().set(MetadataTags::HootConnectedWayOutsideBounds(), "yes");
              directlyConnectedWayIds.insert(connectedWayId);
              _numAffected++;
            }
            else
            {
               LOG_TRACE(
                 "Skipping connected way: " << connectedWay->getElementId() << " that didn't " <<
                 "pass criterion...");
            }
          }
          else
          {
            LOG_TRACE(
              "Skipping connected way: " << ElementId(ElementType::Way, connectedWayId) <<
              " that was already tagged...");
          }
        }
      }
    }
    else
    {
      LOG_TRACE(
        "Skipping source way: " << way->getElementId() << " that either didn't satisfy " <<
        "the criterion or was already tagged...");
    }

    _numProcessed++;
  }
}

}
