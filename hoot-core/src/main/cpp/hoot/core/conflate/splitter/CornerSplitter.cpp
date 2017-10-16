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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "CornerSplitter.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/OsmMap.h>

// Qt
#include <QDebug>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, CornerSplitter)

CornerSplitter::CornerSplitter()
{

}

CornerSplitter::CornerSplitter(boost::shared_ptr<OsmMap> map)
{
  _map = map;
}

void CornerSplitter::splitCorners(boost::shared_ptr<OsmMap> map)
{
  CornerSplitter splitter(map);
  return splitter.splitCorners();
}

void CornerSplitter::splitCorners()
{
  // Get a list of ways in the map
  for (WayMap::const_iterator it = _map->getWays().begin(); it != _map->getWays().end(); ++it)
  {
    _todoWays.push_back(it->first);
  }

  // Traverse each way, looking for corners to split
  // Splitting a way will usually result in adding a new way to _todoWays
  for (size_t i = 0; i < _todoWays.size(); i++)
  {
    ConstWayPtr pWay = _map->getWay(_todoWays[i]);
    size_t nodeCount = pWay->getNodeCount();

    // If the way has just two nodes, there are no corners
    if (nodeCount > 2)
    {
      for (size_t nodeIdx = 1; nodeIdx < nodeCount-1; nodeIdx++)
      {
        WayLocation prev(_map, pWay, nodeIdx-1, 0.0);
        WayLocation current(_map, pWay, nodeIdx, 0.0);
        WayLocation next(_map, pWay, nodeIdx+1, 0.0);

        // Calculate headings
        double h1 = atan2(current.getCoordinate().y - prev.getCoordinate().y,
                          current.getCoordinate().x - prev.getCoordinate().x);
        double h2 = atan2(next.getCoordinate().y - current.getCoordinate().y,
                          next.getCoordinate().x - current.getCoordinate().x);

        double threshold = toRadians(45.0);
        double delta = fabs(h2-h1);


        // If we make enough of a turn, split the way
        if (delta > threshold)
        {
          _splitWay(pWay->getId(), nodeIdx, pWay->getNodeId(nodeIdx));
        }
      }
    }
  }
}

void CornerSplitter::_splitWay(long wayId, long nodeIdx, long nodeId)
{
  boost::shared_ptr<Way> pWay = _map->getWay(wayId);
  if (!pWay)
  {
    LOG_TRACE("way at " << wayId << " does not exist.");
    return;
  }

  LOG_TRACE("Splitting way: " << pWay->getElementId() << " at node: " <<
            ElementId(ElementType::Node, nodeId));

  // split the way and remove it from the map
  WayLocation wayLoc(_map, pWay, nodeIdx, 0.0);
  vector< boost::shared_ptr<Way> > splits = WaySplitter::split(_map, pWay, wayLoc);

  // Process splits
  if (splits.size() > 1)
  {
    LOG_VART(pWay->getElementId());
    LOG_VART(pWay->getStatus());
    LOG_VART(splits[0]->getElementId());

    const ElementId splitWayId = pWay->getElementId();

    QList<ElementPtr> newWays;
    foreach (const boost::shared_ptr<Way>& w, splits)
    {
      newWays.append(w);
      _todoWays.push_back(w->getId());
    }

    // Make sure any ways that are part of relations continue to be part of those relations after
    // they're split.
    _map->replace(pWay, newWays);

    if (ConfigOptions().getPreserveUnknown1ElementIdWhenModifyingFeatures() &&
        pWay->getStatus() == Status::Unknown1)
    {
      // see similar notes in HighwaySnapMerger::_mergePair
      LOG_TRACE("Setting unknown1 " << pWay->getElementId().getId() << " on " <<
                splits[0]->getElementId() << "...");
      ElementPtr newWaySegment(_map->getElement(splits[0]->getElementId())->clone());
      newWaySegment->setId(pWay->getElementId().getId());
      _map->replace(_map->getElement(splits[0]->getElementId()), newWaySegment);
    }

    LOG_VART(_map->containsElement(splitWayId));
  }
}

void CornerSplitter::apply(boost::shared_ptr<OsmMap> &map)
{
  splitCorners(map);
}

}
