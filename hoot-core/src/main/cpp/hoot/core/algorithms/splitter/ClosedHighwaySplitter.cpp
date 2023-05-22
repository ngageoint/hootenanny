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
 * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 */

#include "ClosedHighwaySplitter.h"

// Hoot
#include <hoot/core/algorithms/Distance.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/algorithms/splitter/WaySplitter.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

// Geos
//#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>

// Qt
#include <QTextStream>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, ClosedHighwaySplitter)

void ClosedHighwaySplitter::splitClosedHighways(const std::shared_ptr<OsmMap>& map)
{
  ClosedHighwaySplitter splitter(map);
  return splitter.splitClosedHighways();
}

void ClosedHighwaySplitter::splitClosedHighways()
{
  _numAffected = 0;

  ElementToGeometryConverter converter(_map->shared_from_this());
  // Get a list of all highways in the map
  HighwayCriterion highwayCrit(_map);
  const WayMap& ways =  _map->getWays();
  vector<long> ids;
  for (auto it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = _map->getWay(it->first);
    if (!way)
      continue;
    if (!highwayCrit.isSatisfied(way))
      continue;
    int nodeCount = static_cast<int>(way->getNodeCount());
    // If the way has less than 4 nodes then nothing can be done here
    if (nodeCount < 4)
      continue;
    //  Check for closed way
    if (way->getFirstNodeId() != way->getLastNodeId())
      continue;
    ids.push_back(way->getId());
  }
  //  Iterate all of the closed highways looking to split them
  for (auto id : ids)
  {
    WayPtr way = _map->getWay(id);
    if (!way)
      continue;
    //  Find the closest node to the "middle" of the way and split at that node
    Meters mid = converter.convertToLineString(way)->getLength() / 2.0;
    Meters total = 0;

    int split = -1;
    Coordinate start = _map->getNode(way->getFirstNodeId())->toCoordinate();
    int nodeCount = static_cast<int>(way->getNodeCount());
    //  Iterate all of the way segments
    for (int s = 0, e = 1; e < nodeCount; ++s, ++e)
    {
      Coordinate next = _map->getNode(way->getNodeId(e))->toCoordinate();
      Meters len = next.distance(start);
      //  Check the length of this segment with the total length so far against the mid threshold
      if (total + len > mid)
      {
        //  Find the closer of the two nodes, start or end as the split point
        if (len - total > mid / 2.0)
          split = e;
        else
          split = s;
        break;
      }
      else
      {
        //  Move to the next way segment
        start = next;
        total += len;
      }
    }
    //  Split the way at the split location WaySplitter::split will insert the new way into the map
    //  and update the original way
    if (split != -1)
      WaySplitter::split(_map, way, WayLocation(_map, way, split, 0.0));
  }
}

void ClosedHighwaySplitter::apply(std::shared_ptr<OsmMap> &map)
{
  _map = map;
  splitClosedHighways();
}

QStringList ClosedHighwaySplitter::getCriteria() const
{
  return QStringList(HighwayCriterion::className());
}

}
