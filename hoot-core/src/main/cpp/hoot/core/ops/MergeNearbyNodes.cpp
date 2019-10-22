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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "MergeNearbyNodes.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/index/ClosePointHash.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/elements/OsmUtils.h>

// Qt
#include <QTime>

// Standard
#include <iostream>

// TGS
#include <tgs/StreamUtils.h>
#include <tgs/RStarTree/HilbertRTree.h>

using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, MergeNearbyNodes)

double calcDistanceSquared(const NodePtr& n1, const NodePtr& n2)
{
  double dx = n1->getX() - n2->getX();
  double dy = n1->getY() - n2->getY();
  return dx * dx + dy * dy;
}

MergeNearbyNodes::MergeNearbyNodes(Meters distance)
{
  _distance = distance;
  if (_distance < 0.0)
  {
    _distance = ConfigOptions().getMergeNearbyNodesDistance();
    if (_distance <= 0.0)
    {
      throw IllegalArgumentException(
        "Nearby node merging distance must be greater than zero. Distance specified: " +
        QString::number(_distance));
    }
  }
}

void MergeNearbyNodes::apply(std::shared_ptr<OsmMap>& map)
{
  QTime time;
  time.start();

  std::shared_ptr<OsmMap> wgs84;
  std::shared_ptr<OsmMap> planar;

  if (MapProjector::isGeographic(map))
  {
    wgs84 = map;
    planar.reset(new OsmMap(map));
    MapProjector::projectToPlanar(planar);
  }
  else
  {
    planar = map;
    // if we need to check for bounds containment
    if (_bounds.isNull() == false)
    {
      wgs84.reset(new OsmMap(map));
      MapProjector::projectToWgs84(wgs84);
    }
  }

  ClosePointHash cph(_distance);

  int startNodeCount = 0;
  const NodeMap& nodes = planar->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    const NodePtr& n = it->second;
    cph.addPoint(n->getX(), n->getY(), n->getId());
    startNodeCount++;

    if (startNodeCount % 100000 == 0)
    {
      PROGRESS_INFO(
        "\tInitialized " << StringUtils::formatLargeNumber(startNodeCount) << " nodes / " <<
        StringUtils::formatLargeNumber(nodes.size()) << " for merging.");
    }
  }

  double distanceSquared = _distance * _distance;

  int processedCount = 0;
  cph.resetIterator();
  while (cph.next())
  {
    const std::vector<long>& v = cph.getMatch();

    for (size_t i = 0; i < v.size(); i++)
    {
      if (!map->containsNode(v[i])) continue;

      for (size_t j = 0; j < v.size(); j++)
      {
        bool replace = false;
        double calcdDistanceSquared = -1.0;
        if (v[i] != v[j] && map->containsNode(v[j]))
        {
          const NodePtr& n1 = planar->getNode(v[i]);
          const NodePtr& n2 = planar->getNode(v[j]);

          if (n1->getStatus() == n2->getStatus())
          {
            calcdDistanceSquared = calcDistanceSquared(n1, n2);
            if (distanceSquared > calcdDistanceSquared)
            {
              // if the geographic bounds are not specified.
              if (_bounds.isNull() == true)
              {
                replace = true;
              }
              // if the geographic bounds are specified, then make sure both points are inside.
              else
              {
                const NodePtr& g1 = wgs84->getNode(v[i]);
                const NodePtr& g2 = wgs84->getNode(v[j]);
                if (_bounds.contains(g1->getX(), g1->getY()) &&
                    _bounds.contains(g2->getX(), g2->getY()))
                {
                  replace = true;
                }
              }

              if (replace)
              {
                LOG_TRACE(
                  "Merging nodes: " << ElementId(ElementType::Node, v[j]) << " and " <<
                  ElementId(ElementType::Node, v[i]) << "...");
                map->replaceNode(v[j], v[i]);
                _numAffected++;
              }
            }
          }
        }

        // custom logging for debugging purposes
        if (Log::getInstance().getLevel() <= Log::Trace)
        {
          if (calcdDistanceSquared != -1.0)
          {
            _logMergeResult(
              v[i], v[j], map, replace, std::sqrt(distanceSquared),
              std::sqrt(calcdDistanceSquared));
          }
          else
          {
            _logMergeResult(v[i], v[j], map, replace);
          }
        }
      }
    }

    processedCount++;
    if (processedCount % 10000 == 0)
    {
      PROGRESS_INFO(
        "\tMerged " << StringUtils::formatLargeNumber(_numAffected) << " node groups from " <<
        StringUtils::formatLargeNumber(startNodeCount) << " total nodes.");
    }
  }
}

bool MergeNearbyNodes::_passesLogMergeFilter(const long nodeId1, const long nodeId2,
                                             OsmMapPtr& map) const
{
  // can add various filtering criteria here for debugging purposes...

  QStringList kvps;
  kvps.append("OBJECTID=168008");
  kvps.append("OBJECTID=76174");

  std::set<ElementId> wayIdsOwning1;
  const std::set<long> waysOwning1 = OsmUtils::getContainingWayIdsByNodeId(nodeId1, map);
  for (std::set<long>::const_iterator it = waysOwning1.begin(); it != waysOwning1.end(); ++it)
  {
    wayIdsOwning1.insert(ElementId(ElementType::Way, *it));
  }
  if (OsmUtils::anyElementsHaveAnyKvp(kvps, wayIdsOwning1, map))
  {
    return true;
  }

  std::set<ElementId> wayIdsOwning2;
  const std::set<long> waysOwning2 = OsmUtils::getContainingWayIdsByNodeId(nodeId2, map);
  for (std::set<long>::const_iterator it = waysOwning2.begin(); it != waysOwning2.end(); ++it)
  {
    wayIdsOwning2.insert(ElementId(ElementType::Way, *it));
  }
  if (OsmUtils::anyElementsHaveAnyKvp(kvps, wayIdsOwning2, map))
  {
    return true;
  }

  return false;
}

void MergeNearbyNodes::_logMergeResult(const long nodeId1, const long nodeId2, OsmMapPtr& map,
                                       const bool replaced, const double distance,
                                       const double calcdDistance)
{
  if (_passesLogMergeFilter(nodeId1, nodeId2, map))
  {
    QString msg = "merging nodes: ";
    if (!replaced)
    {
      msg.prepend("not ");
    }
    msg += QString::number(nodeId1) + " and " + QString::number(nodeId2);
    if (calcdDistance != -1.0)
    {
      msg +=
        " at a distance of: " + QString::number(calcdDistance) +
        " where the distance threshold is: " + QString::number(distance);
    }
    msg += "...";
    LOG_TRACE(msg);
    LOG_TRACE(
      "Node " << nodeId1 << " belongs to ways: " <<
      OsmUtils::getContainingWayIdsByNodeId(nodeId1, map));
    LOG_TRACE(
      "Node " << nodeId2 << " belongs to ways: " <<
      OsmUtils::getContainingWayIdsByNodeId(nodeId2, map));
    LOG_VART(OsmUtils::nodesAreContainedInTheSameWay(nodeId1, nodeId2, map));
  }
}

void MergeNearbyNodes::mergeNodes(std::shared_ptr<OsmMap> map, Meters distance)
{
  MergeNearbyNodes mnn(distance);
  mnn.apply(map);
}

void MergeNearbyNodes::readObject(QDataStream& is)
{
  is >> _distance;
}

void MergeNearbyNodes::writeObject(QDataStream& os) const
{
  os << _distance;
}

}
