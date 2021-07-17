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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "HighwayCornerSplitter.h"

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
#include <hoot/core/util/Factory.h>

#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>

// Qt
#include <QDebug>
#include <QTextStream>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, HighwayCornerSplitter)

HighwayCornerSplitter::HighwayCornerSplitter()
  : _cornerThreshold(ConfigOptions().getHighwayCornerSplitterThreshold()),
    _splitRounded(ConfigOptions().getHighwayCornerSplitterRoundedSplit()),
    _roundedThreshold(ConfigOptions().getHighwayCornerSplitterRoundedThreshold()),
    _roundedMaxNodeCount(ConfigOptions().getHighwayCornerSplitterRoundedMaxNodeCount())
{
}

HighwayCornerSplitter::HighwayCornerSplitter(const std::shared_ptr<OsmMap>& map)
  : _map(map),
    _cornerThreshold(ConfigOptions().getHighwayCornerSplitterThreshold()),
    _splitRounded(ConfigOptions().getHighwayCornerSplitterRoundedSplit()),
    _roundedThreshold(ConfigOptions().getHighwayCornerSplitterRoundedThreshold()),
    _roundedMaxNodeCount(ConfigOptions().getHighwayCornerSplitterRoundedMaxNodeCount())
{
}

void HighwayCornerSplitter::splitCorners(const std::shared_ptr<OsmMap>& map)
{
  HighwayCornerSplitter splitter(map);
  return splitter.splitCorners();
}

void HighwayCornerSplitter::splitCorners()
{
  _numAffected = 0;

  // Get a list of ways (that look like roads) in the map
  HighwayCriterion highwayCrit(_map);
  for (WayMap::const_iterator it = _map->getWays().begin(); it != _map->getWays().end(); ++it)
  {
    if (highwayCrit.isSatisfied(it->second))
    {
      _todoWays.push_back(it->first);
    }
  }

  double threshold = toRadians(_cornerThreshold);
  // Traverse each way, looking for corners to split
  // Splitting a way will usually result in adding a new way to _todoWays
  for (size_t i = 0; i < _todoWays.size(); i++)
  {
    ConstWayPtr pWay = _map->getWay(_todoWays[i]);
    size_t nodeCount = pWay->getNodeCount();
    // If the way has just two nodes, there are no corners
    if (nodeCount > 2)
    {
      // Look until we find a split, or get to the end
      bool split = false;
      for (size_t nodeIdx = 1; nodeIdx < nodeCount - 1 && !split; nodeIdx++)
      {
        WayLocation prev(_map, pWay, nodeIdx - 1, 0.0);
        WayLocation current(_map, pWay, nodeIdx, 0.0);
        WayLocation next(_map, pWay, nodeIdx + 1, 0.0);

        // Calculate headings
        const double twopi = M_PI*2.0;
        double h1 = atan2(current.getCoordinate().y - prev.getCoordinate().y,
                          current.getCoordinate().x - prev.getCoordinate().x);
        double h2 = atan2(next.getCoordinate().y - current.getCoordinate().y,
                          next.getCoordinate().x - current.getCoordinate().x);

        double delta = fabs(h2-h1);

        if (delta > M_PI)
          delta = twopi - delta;

        // If we make enough of a turn, split the way
        if (delta > threshold)
        {
          LOG_TRACE("splitting way with delta: " << delta);
          split = _splitWay(pWay->getId(), nodeIdx, pWay->getNodeId(nodeIdx));
        }
      }
    }
  }
  //  Try out the rounded corner splitting algorithm if desired
  if (_splitRounded)
    _splitRoundedCorners();
}

void HighwayCornerSplitter::_splitRoundedCorners()
{
  _todoWays.clear();
  //  Get a list of ways (that look like roads) in the map
  HighwayCriterion highwayCriterion(_map);
  for (WayMap::const_iterator it = _map->getWays().begin(); it != _map->getWays().end(); ++it)
  {
    if (highwayCriterion.isSatisfied(it->second))
      _todoWays.push_back(it->first);
  }
  //  Use the threashold from the configuration for rounded corners
  const double threshold = toRadians(_roundedThreshold);
  //  Display trace
  bool trace = Log::getInstance().getLevel() == Log::Trace;
  //  Traverse each way, looking for corners to split
  //  Splitting a way will usually result in adding a new way to _todoWays
  for (size_t i = 0; i < _todoWays.size(); i++)
  {
    ConstWayPtr pWay = _map->getWay(_todoWays[i]);
    size_t nodeCount = pWay->getNodeCount();

    // If the way has less than 4 nodes it can't be rounded
    if (nodeCount >= 4)
    {
      //  Precalculate the headings between points
      QVector<double> headings;
      QVector<double> distances;
      for (size_t nodeIdx = 0; nodeIdx < nodeCount - 1; ++nodeIdx)
      {
        //  Get the current and next way locations
        WayLocation current(_map, pWay, nodeIdx, 0.0);
        WayLocation next(_map, pWay, nodeIdx + 1, 0.0);
        //  Calculate and store the heading from point A to point B
        double heading = atan2(next.getCoordinate().y - current.getCoordinate().y,
                               next.getCoordinate().x - current.getCoordinate().x);
        headings.push_back(heading);
        //  Maybe we can do something with the distance, maybe not
        double distance = Distance::euclidean(next.getCoordinate(), current.getCoordinate());
        distances.push_back(distance);
      }
      int max_start_index = 0;
      double max_total_delta = 0.0;
      //  Iterate the headings `max_node_count` at a time to find the largest heading delta and use
      // that section as the curve
      for (int start_index = 0; start_index < headings.size() - 2; ++start_index)
      {
        double total = 0.0;
        for (int j = 0; j < _roundedMaxNodeCount && start_index + j + 1 < headings.size(); ++j)
        {
          double delta = headings[start_index + j + 1] - headings[start_index + j];
          total += delta;
        }
        //  Save the highest heading delta and the start index of that subline
        if (fabs(total) > fabs(max_total_delta))
        {
          max_start_index = start_index;
          max_total_delta = total;
        }
      }
      //  Make the split if the heading delta exceeds the threashold
      if (fabs(max_total_delta) > threshold)
      {
        //  Find the middle of the threshold to split at
        double mid_point = max_total_delta / 2.0;
        double total = 0.0;
        for (int index = max_start_index + 1;
             index < max_start_index + _roundedMaxNodeCount && index < headings.size(); ++index)
        {
          double delta = headings[index] - headings[index - 1];
          total += delta;
          //  Split the way once the cumulative heading delta exceeds the midpoint delta value
          if (fabs(total) > fabs(mid_point))
          {
            _splitWay(pWay->getId(), index + 1, pWay->getNodeId(index + 1), false);
            break;
          }
        }
      }
      //  Only build up the tables and display them in trace mode
      if (trace)
      {
        QString buffer;
        QTextStream ts(&buffer);
        for (int j = 0; j < headings.size(); ++j)
          ts << QString().setNum(headings[j], 'f') << "\t| " <<
                QString().setNum(distances[j], 'f') << "\n";
        //  Output a bunch of stuff here to help develop the algorithm
        LOG_TRACE("\nWay: " << pWay->getTags().getName() <<
                  "\nHeadings\t| Distances" <<
                  "\n" << ts.readAll()
                 );
      }
    }
  }
}

bool HighwayCornerSplitter::_splitWay(long wayId, long nodeIdx, long nodeId, bool sharpCorner)
{
  WayPtr pWay = _map->getWay(wayId);
  if (!pWay)
  {
    LOG_TRACE("way at " << wayId << " does not exist.");
    return false;
  }
  //  For sharp corners, some small
  if (sharpCorner)
  {
    GeometryFactory::unique_ptr factory = GeometryFactory::create();
    //  Check the previous segment to ensure it is larger than the circular error before splitting
    if (nodeIdx == 1)
    {
      std::shared_ptr<LineString> ls = ElementToGeometryConverter(_map).convertToLineString(pWay);
      CoordinateArraySequence* subline = new CoordinateArraySequence();
      subline->add(0, ls->getCoordinateN(nodeIdx), true);
      subline->add(1, ls->getCoordinateN(nodeIdx - 1), true);
      // GeometryFactory takes ownership of these input parameters.
      std::shared_ptr<LineString> sub(factory->createLineString(subline));
      if (sub->getLength() <= pWay->getCircularError())
        return false;
    }
    //  Check the next segment to ensure it is larger than the circular error before splitting
    if (nodeIdx == (long)(pWay->getNodeCount() - 2))
    {
      std::shared_ptr<LineString> ls = ElementToGeometryConverter(_map).convertToLineString(pWay);
      CoordinateArraySequence* subline = new CoordinateArraySequence();
      subline->add(0, ls->getCoordinateN(nodeIdx), true);
      subline->add(1, ls->getCoordinateN(nodeIdx + 1), true);
      // GeometryFactory takes ownership of these input parameters.
      std::shared_ptr<LineString> sub(factory->createLineString(subline));
      if (sub->getLength() <= pWay->getCircularError())
        return false;
    }
  }
  LOG_TRACE("Splitting way: " << pWay->getElementId() << " at node: " <<
            ElementId(ElementType::Node, nodeId));

  // split the way and remove it from the map
  WayLocation wayLoc(_map, pWay, nodeIdx, 0.0);
  vector<WayPtr> splits = WaySplitter::split(_map, pWay, wayLoc);

  // Process splits
  if (splits.size() > 1)
  {
    LOG_VART(pWay->getElementId());
    LOG_VART(pWay->getStatus());
    LOG_VART(splits[0]->getElementId());

    const ElementId splitWayId = pWay->getElementId();

    // Make sure any ways that are part of relations continue to be part of those relations after
    // they're split.
    QList<ElementPtr> newWays;
    foreach (const WayPtr& w, splits)
      newWays.append(w);

    _map->replace(pWay, newWays);

    // Need to process the "right-hand-side" of the split, looking for more corners
    _todoWays.push_back(splits[1]->getId());

    LOG_VART(_map->containsElement(splitWayId));

    //  Split was successful
    _numAffected++;
    return true;
  }
  //  No splits were made
  return false;
}

void HighwayCornerSplitter::apply(std::shared_ptr<OsmMap> &map)
{
  splitCorners(map);
}

void HighwayCornerSplitter::setConfiguration(const Settings& conf)
{
  ConfigOptions options(conf);
  _cornerThreshold = options.getHighwayCornerSplitterThreshold();
  _splitRounded = options.getHighwayCornerSplitterRoundedSplit();
  _roundedThreshold = options.getHighwayCornerSplitterRoundedThreshold();
  _roundedMaxNodeCount = options.getHighwayCornerSplitterRoundedMaxNodeCount();
}

QStringList HighwayCornerSplitter::getCriteria() const
{
  return QStringList(HighwayCriterion::className());
}

}
