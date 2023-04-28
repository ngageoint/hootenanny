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

#include "NodeToWayMap.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/ConfigOptions.h>

using namespace std;

namespace hoot
{

int NodeToWayMap::logWarnCount = 0;

NodeToWayMap::NodeToWayMap(const OsmMap& m)
{
  const WayMap& ways = m.getWays();
  for (auto it = ways.begin(); it != ways.end(); ++it)
    addWay(it->second);
}

void NodeToWayMap::addWay(const std::shared_ptr<const Way>& w)
{
  LOG_TRACE("Adding way: " << w->getElementId() << " with nodes: "<<  w->getNodeIds() << "...");
  const std::vector<long>& nodes = w->getNodeIds();
  for (auto node_id : nodes)
    (*this)[node_id].insert(w->getId());
}

const set<long>& NodeToWayMap::getWaysByNode(long nid) const
{
  auto it = this->find(nid);
  if (it == this->end())
    return _emptySet;
  else
    return it->second;
}

void NodeToWayMap::removeWay(const std::shared_ptr<const Way>& w)
{
  LOG_TRACE("Removing way from node to way map: " << w->getElementId() << "...");
  const std::vector<long>& nodes = w->getNodeIds();
  for (auto node_id : nodes)
  {
    set<long>& s = (*this)[node_id];
    s.erase(w->getId());
    // if we just removed the last node, then remove the entry from the map.
    if (s.empty())
      erase(node_id);
  }
}

bool NodeToWayMap::validate(const OsmMap& m)
{
  bool result = true;
  // verify that the map contains everything that it should.
#ifndef NDEBUG
  const WayMap& ways = m.getWays();
  for (auto it = ways.begin(); it != ways.end(); ++it)
  {
    const std::shared_ptr<Way>& w = it->second;
    assert(w->getId() != 0);
    const std::vector<long>& nodes = w->getNodeIds();
    for (auto node_id : nodes)
    {
      const set<long>& s = (*this)[node_id];
      assert(s.find(w->getId()) != s.end());
    }
  }
#endif

  // verify that the map doesn't contain anything it shouldn't.
  for (auto it = begin(); it != end(); ++it)
  {
    long nid = it->first;
    const set<long>& s = it->second;
    // this assumption causes problems when a OsmMap is incomplete (think map reduce)
    //assert(map.containsNode(nid) || s.size() == 0);
    for (auto way_id : s)
    {
      assert(way_id != 0);
      if (m.containsWay(way_id) == false)
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN(QString("Map does not contain way: %1 ref by node: %2").arg(way_id).arg(nid));
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
        result = false;
      }
    }
  }
  return result;
}

}
