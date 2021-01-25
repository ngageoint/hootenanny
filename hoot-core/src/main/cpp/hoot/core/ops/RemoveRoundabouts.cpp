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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RemoveRoundabouts.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/algorithms/splitter/WaySplitter.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/criterion/RoundaboutCriterion.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/criterion/HighwayCriterion.h>

// Qt
#include <QDebug>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RemoveRoundabouts)

void RemoveRoundabouts::removeRoundabouts(std::vector<RoundaboutPtr>& removed)
{
  LOG_TRACE("Removing roundabouts...");

  // Get a list of roundabouts in the map
  RoundaboutCriterion roundaboutCrit;
  for (WayMap::const_iterator it = _pMap->getWays().begin(); it != _pMap->getWays().end(); ++it)
  {
    if (roundaboutCrit.isSatisfied(it->second))
    {
      _todoWays.push_back(it->first);
    }
  }
  LOG_VART(_todoWays.size());

  // Make roundabout objects
  for (size_t i = 0; i < _todoWays.size(); i++)
  {
    WayPtr pWay = _pMap->getWay(_todoWays[i]);
    RoundaboutPtr rnd = Roundabout::makeRoundabout(_pMap, pWay);
    removed.push_back(rnd);
  }
  LOG_VART(removed.size());

  //  Exit if there are no roundabouts removed
  if (removed.size() == 0)
    return;

  // Mangle (in a good way) ways that may cross our roundabouts, provided there
  // is no 'sibling' roundabout in the secondary dataset
  QVector<bool> foundSibling(removed.size(), false);
  for (size_t i = 0; i < removed.size() - 1; i++)
  {
    //  If a sibling has already been found for this way, skip it
    if (!foundSibling[i])
    {
      geos::geom::Coordinate c1 = removed[i]->getCenter()->toCoordinate();
      for (size_t j = i + 1; j < removed.size() && !foundSibling[i]; j++)
      {
        geos::geom::Coordinate c2 = removed[j]->getCenter()->toCoordinate();
        double distance = c1.distance(c2);
        //  If the two centers are within the circular error, mark both as siblings
        if (distance <= removed[i]->getCenter()->getCircularError())
          foundSibling[i] = foundSibling[j] = true;
      }

      // If no sibling, do the mangle
      if (!foundSibling[i])
      {
        removed[i]->handleCrossingWays(_pMap);
        removed[i]->overrideRoundabout();
      }
    }

    // This could be very expensive...enable for debugging only.
    //OsmMapWriterFactory::writeDebugMap(
      //_pMap, "after-RemoveRoundabouts-handling-crossing-ways-" + QString::number(i + 1));
  }

  //  Mangle the last way if it doesn't have a sibling
  if (!foundSibling[removed.size() - 1])
  {
    removed[removed.size() - 1]->handleCrossingWays(_pMap);
    removed[removed.size() - 1]->overrideRoundabout();
  }

  OsmMapWriterFactory::writeDebugMap(_pMap, "after-RemoveRoundabouts-handling-crossing-ways");

  // Now remove roundabouts
  for (size_t i = 0; i < removed.size(); i++)
  {
    removed[i]->removeRoundabout(_pMap);
    _numAffected++;

    // This could be very expensive...enable for debugging only.
    //OsmMapWriterFactory::writeDebugMap(
      //_pMap, "after-removing-roundabout-" + QString::number(i + 1));
  }
}

void RemoveRoundabouts::apply(OsmMapPtr& pMap)
{
  _numAffected = 0;
  _pMap = pMap;

  LOG_VART(MapProjector::toWkt(pMap->getProjection()));
  MapProjector::projectToPlanar(_pMap);

  std::vector<RoundaboutPtr> removed;
  removeRoundabouts(removed);
  pMap->setRoundabouts(removed);
}

QStringList RemoveRoundabouts::getCriteria() const
{
  return QStringList(HighwayCriterion::className());
}

}
