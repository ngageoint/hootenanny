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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "DirectedGraph.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/OsmMap.h>

namespace hoot
{

DirectedGraph::DirectedGraph()
{
}

void DirectedGraph::addEdge(long from, long to, double weight)
{
  _edges.insert(from, Edge(from, to, weight));
}

double DirectedGraph::determineCost(boost::shared_ptr<Way> way)
{
  QString highway = way->getTags()["highway"];
  // reasonable default for an unknown "highway"
  double mph = 25;

  if (highway == "motorway")
  {
    mph = 65;
  }
  if (highway == "trunk")
  {
    mph = 45;
  }
  if (highway == "primary")
  {
    mph = 30;
  }
  if (highway == "secondary")
  {
    mph = 25;
  }
  if (highway == "tertiary")
  {
    mph = 22;
  }
  if (highway == "residential" || highway == "unclassified")
  {
    mph = 15;
  }
  if (highway == "living_street")
  {
    mph = 10;
  }
  if (highway == "track")
  {
    mph = 10;
  }
  if (highway == "path")
  {
    mph = 5;
  }

  return _mphToSecondsPerMeter(mph);
}

void DirectedGraph::deriveEdges(boost::shared_ptr<const OsmMap> map)
{
  const WayMap& ways = map->getWays();

  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const boost::shared_ptr<Way>& way = it->second;
    double cost = determineCost(way);
    double length = ElementConverter(map).convertToLineString(way)->getLength();

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

bool DirectedGraph::isOneWay(boost::shared_ptr<Way> way)
{
  return OsmSchema::getInstance().isOneWay(*way);
}

}
