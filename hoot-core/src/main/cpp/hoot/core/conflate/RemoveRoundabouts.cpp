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

#include "RemoveRoundabouts.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/MapProjector.h>

// Qt
#include <QDebug>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RemoveRoundabouts)

RemoveRoundabouts::RemoveRoundabouts()
{
  // blank
}

void RemoveRoundabouts::removeRoundabouts(std::vector<RoundaboutPtr> &removed)
{
  // Get a list of roundabouts in the map
  for (WayMap::const_iterator it = _pMap->getWays().begin(); it != _pMap->getWays().end(); ++it)
  {
    if (OsmSchema::getInstance().isRoundabout(it->second->getTags(),
                                              it->second->getElementType()))
    {
      _todoWays.push_back(it->first);
    }
  }

  // Make roundabout objects
  for (size_t i = 0; i < _todoWays.size(); i++)
  {
    WayPtr pWay = _pMap->getWay(_todoWays[i]);
    RoundaboutPtr rnd = Roundabout::makeRoundabout(_pMap, pWay);
    removed.push_back(rnd);
  }

  // Mangle (in a good way) ways that may cross our roundabouts, provided there
  // is no 'sibling' roundabout in the secondary dataset
  for (size_t i = 0; i < removed.size(); i++)
  {
    geos::geom::Coordinate c1 = removed[i]->getCenter()->toCoordinate();
    bool hasSibling = false;
    for (size_t j = i+1; j < removed.size() && !hasSibling; j++)
    {
      geos::geom::Coordinate c2 = removed[j]->getCenter()->toCoordinate();
      double distance = c1.distance(c2);
      if (distance <= removed[i]->getCenter()->getCircularError())
        hasSibling = true;
    }

    // If no sibling, do the mangle
    if (!hasSibling)
      removed[i]->handleCrossingWays(_pMap);
  }

  // Now remove roundabouts
  for (size_t i = 0; i < removed.size(); i++)
    removed[i]->removeRoundabout(_pMap);
}

void RemoveRoundabouts::apply(OsmMapPtr &pMap)
{
  _pMap = pMap;
  MapProjector::projectToPlanar(_pMap);
  std::vector<RoundaboutPtr> removed;
  removeRoundabouts(removed);
  pMap->setRoundabouts(removed);
}

}
