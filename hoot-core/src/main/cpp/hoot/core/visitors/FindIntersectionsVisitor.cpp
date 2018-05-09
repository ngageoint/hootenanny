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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "FindIntersectionsVisitor.h"

#include <hoot/core/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/conflate/matching/NodeMatcher.h>
#include <hoot/core/schema/OsmSchema.h>

using namespace std;

namespace hoot
{

void FindIntersectionsVisitor::visit(const ConstElementPtr& e)
{
  boost::shared_ptr<NodeToWayMap> n2w = _map->getIndex().getNodeToWayMap();
  long id = e->getId();

  const set<long>& wids = n2w->getWaysByNode(id);

  // find all ways that are highways (ie roads)
  set<long> hwids;
  for (set<long>::const_iterator it = wids.begin(); it != wids.end(); ++it)
  {
    WayPtr w = _map->getWay(*it);

    if (OsmSchema::getInstance().isLinearHighway(w->getTags(), w->getElementType()))
    {
      hwids.insert(*it);
    }
  }


  if (hwids.size() >= 3) // two or more roads intersecting
  {
    // keep it
    _ids.push_back(id);
    _map->getNode(id)->setTag("IntersectionWayCount", QString::number(hwids.size()));

    vector<Radians> angles = NodeMatcher::calculateAngles(_map, id, hwids, 10);

    vector<double> v;
    for (uint i = 0; i < angles.size(); i++)
    {
      v.push_back(toDegrees(angles[i])+180);
    }
    sort(v.begin(), v.end());

    double minAngle = 360.;
    double maxAngle = 0.;

    for (uint i = 0; i < v.size(); i++)
    {
      double a = (i == 0) ? (v[i] + 360 - v[v.size()-1]) : v[i] - v[i-1];
      if (a < minAngle)
      {
        minAngle = a;
      }
      if (a > maxAngle)
      {
        maxAngle = a;
      }
    }

    _map->getNode(id)->setTag("MinAngle", QString::number(minAngle));
    _map->getNode(id)->setTag("MaxAngle", QString::number(maxAngle));
  }
}

}





