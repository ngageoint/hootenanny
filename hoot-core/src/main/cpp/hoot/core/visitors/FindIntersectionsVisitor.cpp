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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "FindIntersectionsVisitor.h"

#include <hoot/core/conflate/matching/NodeMatcher.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, FindHighwayIntersectionsVisitor)
HOOT_FACTORY_REGISTER(ElementVisitor, FindRailwayIntersectionsVisitor)

void FindIntersectionsVisitor::visit(const ConstElementPtr& e)
{
  std::shared_ptr<NodeToWayMap> n2w = _map->getIndex().getNodeToWayMap();
  long id = e->getId();

  const set<long>& wids = n2w->getWaysByNode(id);
  // create the criterion if needed
  if (!_criterion)
    _criterion = createCriterion(_map);
  // find all ways that are of the criterion type
  set<long> hwids;
  for (auto way_id : wids)
  {
    WayPtr w = _map->getWay(way_id);
    if (_criterion->isSatisfied(w))
      hwids.insert(way_id);
  }

  if (hwids.size() >= 3) // two or more ways intersecting
  {
    // keep it
    _ids.push_back(id);

    // TODO: This and the commented out minAngle/maxAngle sections below are puzzling...changing the
    // tag keys changes the number of nodes in the test output for FindIntersectionsOpTest.
//    _map->getNode(id)->setTag(MetadataTags::HootIntersectionWayCount(), QString::number(hwids.size()));
    _map->getNode(id)->setTag("IntersectionWayCount", QString::number(hwids.size()));

    vector<Radians> angles = NodeMatcher::calculateAngles(_map.get(), id, hwids, 10);
    vector<double> v;
    for (auto angle : angles)
      v.push_back(toDegrees(angle)+180);
    sort(v.begin(), v.end());

    double minAngle = 360.;
    double maxAngle = 0.;
    for (uint i = 0; i < v.size(); i++)
    {
      double a = (i == 0) ? (v[i] + 360 - v[v.size() - 1]) : v[i] - v[i-1];
      if (a < minAngle)
        minAngle = a;
      if (a > maxAngle)
        maxAngle = a;
    }
    //_map->getNode(id)->setTag(MetadataTags::HootIntersectionMinAngle(), QString::number(minAngle));
    //_map->getNode(id)->setTag(MetadataTags::HootIntersectionMaxAngle(), QString::number(maxAngle));
    _map->getNode(id)->setTag("MinAngle", QString::number(minAngle));
    _map->getNode(id)->setTag("MaxAngle", QString::number(maxAngle));
  }
}

ElementCriterionPtr FindHighwayIntersectionsVisitor::createCriterion(ConstOsmMapPtr map)
{
  return std::make_shared<HighwayCriterion>(map);
}

ElementCriterionPtr FindRailwayIntersectionsVisitor::createCriterion(ConstOsmMapPtr /*map*/)
{
  return std::make_shared<RailwayCriterion>();
}

}





