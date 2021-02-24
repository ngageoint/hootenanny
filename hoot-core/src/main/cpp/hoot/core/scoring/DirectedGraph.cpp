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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "DirectedGraph.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/elements/Way.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/OneWayCriterion.h>

namespace hoot
{

DirectedGraph::DirectedGraph()
{
}

void DirectedGraph::addEdge(long from, long to, double weight)
{
  _edges.insert(from, Edge(from, to, weight));
}

double DirectedGraph::determineCost(const std::shared_ptr<Way>& way)
{
  QString highway = way->getTags()["highway"];
  // reasonable default for an unknown "highway"
  double mph = 25;

  // TODO: move to config
  if (highway == "motorway")
    mph = 65;
  else if (highway == "trunk")
    mph = 45;
  else if (highway == "primary")
    mph = 30;
  else if (highway == "secondary")
    mph = 25;
  else if (highway == "tertiary")
    mph = 22;
  else if (highway == "residential" || highway == "unclassified")
    mph = 15;
  else if (highway == "living_street")
    mph = 10;
  else if (highway == "track")
    mph = 10;
  else if (highway == "path")
    mph = 5;

  return _mphToSecondsPerMeter(mph);
}

void DirectedGraph::deriveEdges(const std::shared_ptr<const OsmMap>& map)
{
  const WayMap& ways = map->getWays();

  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const std::shared_ptr<Way>& way = it->second;
    LOG_VART(way.get());
    LOG_VART(way->getNodeIds().size());
    double cost = determineCost(way);
    LOG_VART(cost);
    double length = ElementToGeometryConverter(map).convertToLineString(way)->getLength();
    LOG_VART(length);
    if (length == 0)
    {
      continue;
    }

    long nStart = way->getNodeId(0);
    long nEnd = way->getNodeId(way->getNodeCount() - 1);
    if (cost >= 0)
    {
      if (isOneWay(way))
      {
        addEdge(nStart, nEnd, cost * length);
      }
      else
      {
        addEdge(nStart, nEnd, cost * length);
        addEdge(nEnd, nStart, cost * length);
      }
    }
  }
}

bool DirectedGraph::isOneWay(const std::shared_ptr<Way>& way)
{
  return OneWayCriterion().isSatisfied(way);
}

}
