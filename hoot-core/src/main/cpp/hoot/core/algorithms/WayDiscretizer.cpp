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

#include "WayDiscretizer.h"

// GEOS
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/LineString.h>
using namespace geos::geom;

// Hoot
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>

// Standard
#include <iostream>
using namespace std;

#include "Distance.h"

namespace hoot
{

WayDiscretizer::WayDiscretizer(const ConstOsmMapPtr &map, boost::shared_ptr<const Way> way) :
  _map(map)
{
  _way = way;

  // Go through all the nodes
  double l = 0;
  const std::vector<long>& nodeIds = _way->getNodeIds();
  boost::shared_ptr<const Node> lastNode = _map->getNode(nodeIds[0]);
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    // calculate the distance from the last node to this node.
    boost::shared_ptr<const Node> n = _map->getNode(nodeIds[i]);
    double d = Distance::euclidean(*lastNode, *n);

    // add the distance to a running total
    l += d;
    // keep track of the distance from the beginning of the way to this node.
    _lengthNodes.push_back(l);
    lastNode = n;
  }
}

void WayDiscretizer::discretize(double spacing, vector<Coordinate>& result)
{
  double length = _lengthNodes.back();
  double d = 0.0;

  assert(spacing != 0.0);

  do
  {
    result.push_back(interpolate(d));
    d += spacing;
  } while (d <= length);
}

void WayDiscretizer::discretize(double spacing, vector<WayLocation>& result)
{
  Meters wayLength = ElementConverter(_map).convertToLineString(_way)->getLength();

  int count = ceil(wayLength / spacing);
  spacing = wayLength / double(count);

  result.resize(count + 1);

  for (int i = 0; i <= count; i++)
  {
    result[i] = WayLocation(_map, _way, double(i) * spacing);
  }
}

geos::geom::Coordinate WayDiscretizer::interpolate(double d)
{
  geos::geom::Coordinate result;

  const std::vector<long>& nodeIds = _way->getNodeIds();

  if (d <= 0)
  {
    boost::shared_ptr<const Node> n = _map->getNode(nodeIds[0]);
    result.x = n->getX();
    result.y = n->getY();
  }
  else if (d >= _lengthNodes.back())
  {
    boost::shared_ptr<const Node> n = _map->getNode(nodeIds.back());
    result.x = n->getX();
    result.y = n->getY();
  }
  else
  {
    size_t i = 0;
    for (; i < _lengthNodes.size() - 1 && _lengthNodes[i + 1] < d; i++);

    double startD = _lengthNodes[i];
    double endD = _lengthNodes[i + 1];

    boost::shared_ptr<const Node> startN = _map->getNode(nodeIds[i]);
    boost::shared_ptr<const Node> endN = _map->getNode(nodeIds[i + 1]);

    double w = (d - startD) / (endD - startD);

    result.x = startN->getX() * (1 - w) + endN->getX() * w;
    result.y = startN->getY() * (1 - w) + endN->getY() * w;
  }

  return result;
}

}
