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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "WayCleaner.h"

#include <hoot/core/util/Log.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

unsigned int WayCleaner::logWarnCount = 0;

WayCleaner::WayCleaner()
{
}

void WayCleaner::visit(const boost::shared_ptr<Element>& e)
{
  WayPtr way = boost::dynamic_pointer_cast<Way>(e);
  const vector<long> nodeIds = way->getNodeIds();

  if (_isZeroLengthWay(way, *_map))
  {
    throw HootException("Cannot clean zero length way.");
  }

  QList<long> modifiedNodeIds = QVector<long>::fromStdVector(nodeIds).toList();
  QList<long> nodeIdsTemp;
  QList<Coordinate> coords;
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    bool found = false;
    if (nodeIdsTemp.contains(nodeIds[i]))
    {
      //the only duplicated nodes allowed are the first and last for a closed way
      if (i == (nodeIds.size() - 1) && nodeIds[0] == nodeIds[i])
      {
      }
      else
      {
        found = true;
      }
    }
    else
    {
      nodeIdsTemp.append(nodeIds[i]);
    }

    const Coordinate coord = _map->getNode(nodeIds[i])->toCoordinate();
    if (coords.contains(coord))
    {
      //the only duplicated coords allowed are the first and last for a closed way, if the node ID's
      //match
      if (i == (nodeIds.size() - 1) && nodeIds[0] == nodeIds[i])
      {
      }
      else
      {
        found = true;
      }
    }
    else
    {
      coords.append(coord);
    }

    if (found)
    {
      modifiedNodeIds.removeAt(i);
    }
  }

  way->setNodes(modifiedNodeIds.toVector().toStdVector());

  _numAffected++;
}

bool WayCleaner::_hasDuplicateCoords(ConstWayPtr way, const OsmMap& map,
                                     const bool logDetails)
{
  const vector<long> nodeIds = way->getNodeIds();

  const unsigned int logWarnMessageLimit = ConfigOptions().getLogWarnMessageLimit();

  if (nodeIds.size() == 2 &&
      map.getNode(nodeIds.at(0))->toCoordinate() == map.getNode(nodeIds.at(1))->toCoordinate())
  {
    if (logDetails)
    {
      if (logWarnCount < logWarnMessageLimit)
      {
        LOG_WARN(
          "Duplicate coordinate " << map.getNode(nodeIds.at(0))->toCoordinate() <<
          " for node with ID:'s " << nodeIds[0] << " and " << nodeIds[1] <<
          " found at indexes 0 and 1; For way with ID: " << way->getElementId().getId());
        LOG_VART(nodeIds);
      }
      else if (logWarnCount == logWarnMessageLimit)
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    return true;
  }

  bool found = false;
  QList<Coordinate> coords;
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    const Coordinate coord = map.getNode(nodeIds[i])->toCoordinate();
    if (coords.contains(coord))
    {
      //the only duplicated coords allowed are the first and last for a closed way, if the node ID's
      //match
      if (i == (nodeIds.size() - 1) && nodeIds[0] == nodeIds[i])
      {
        found = false;
      }
      else
      {
        found = true;
      }
      if (found)
      {
        if (logDetails)
        {
          if (logWarnCount < logWarnMessageLimit)
          {
            LOG_WARN(
              "Duplicate coord " << map.getNode(nodeIds[i])->toCoordinate() <<
              " for node with ID: " << nodeIds[i] << " found at index: " << i <<
              " For way with ID: " << way->getElementId().getId());
            LOG_VART(nodeIds);
          }
          else if (logWarnCount == logWarnMessageLimit)
          {
            LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
          }
          logWarnCount++;
        }
        return found;
      }
    }
    else
    {
      coords.append(coord);
    }
  }
  return found;
}

bool WayCleaner::_hasDuplicateNodes(ConstWayPtr way, const bool logDetails)
{
  const vector<long> nodeIds = way->getNodeIds();

  const unsigned int logWarnMessageLimit = ConfigOptions().getLogWarnMessageLimit();

  if (nodeIds.size() == 2 && nodeIds.at(0) == nodeIds.at(1))
  {
    if (logDetails)
    {
      if (logWarnCount < logWarnMessageLimit)
      {
        LOG_WARN(
          "Duplicate node with ID: " << nodeIds[0] << " found at indexes 0 and 1; " <<
          "For way with ID: " << way->getElementId().getId());
        LOG_VART(nodeIds);
      }
      else if (logWarnCount == logWarnMessageLimit)
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    return true;
  }

  bool found = false;
  QList<long> nodeIdsTemp;
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    if (nodeIdsTemp.contains(nodeIds[i]))
    {
      //the only duplicated nodes allowed are the first and last for a closed way
      if (i == (nodeIds.size() - 1) && nodeIds[0] == nodeIds[i])
      {
        found = false;
      }
      else
      {
        found = true;
      }
      if (found)
      {
        if (logDetails)
        {
          if (logWarnCount < logWarnMessageLimit)
          {
            LOG_WARN(
              "Duplicate node with ID: " << nodeIds[i] << " found at index: " << i <<
              " For way with ID: " << way->getElementId().getId());
            LOG_VART(nodeIds);
          }
          else if (logWarnCount == logWarnMessageLimit)
          {
            LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
          }
          logWarnCount++;
        }
        return found;
      }
    }
    else
    {
      nodeIdsTemp.append(nodeIds[i]);
    }
  }
  return found;
}

bool WayCleaner::_isZeroLengthWay(ConstWayPtr way, const OsmMap& map)
{
  return way->getNodeCount() == 2 && (_hasDuplicateNodes(way) || _hasDuplicateCoords(way, map));
}

void WayCleaner::cleanWay(WayPtr way, const OsmMapPtr& map)
{
  WayCleaner wayCleaner;
  wayCleaner.setOsmMap(map.get());
  wayCleaner.visit(way);
}

ConstWayPtr WayCleaner::cleanWay(ConstWayPtr way, const OsmMapPtr& map)
{
  WayPtr cleanedWay(new Way(*way.get()));
  WayCleaner::cleanWay(cleanedWay, map);
  return cleanedWay;
}

vector<ConstWayPtr> WayCleaner::cleanWays(const vector<ConstWayPtr>& ways,
                                          const OsmMapPtr& map)
{
  vector<ConstWayPtr> cleanedWays;
  for (vector<ConstWayPtr>::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    ConstWayPtr way = *it;
    if (!_isZeroLengthWay(way, *map.get()))
    {
      cleanedWays.push_back(cleanWay(way, map));
    }
  }
  return cleanedWays;
}

}
