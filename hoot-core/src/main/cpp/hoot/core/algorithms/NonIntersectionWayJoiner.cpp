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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "NonIntersectionWayJoiner.h"

//  Hoot
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

#include <unordered_set>


using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(WayJoiner, NonIntersectionWayJoiner)

void NonIntersectionWayJoiner::joinWays(const OsmMapPtr& map)
{
  NonIntersectionWayJoiner wayJoiner;
  wayJoiner.join(map);
}

void NonIntersectionWayJoiner::_joinAtNode()
{
  LOG_INFO("\tJoining ways at non-intersection shared nodes...");

  const WayMap& ways = _map->getWays();
  unordered_set<long> ids;
  std::shared_ptr<NodeToWayMap> nodeToWayMap = _map->getIndex().getNodeToWayMap();
  HighwayCriterion highway(_map);
  //  Find all ways that have non-intersection endpoints that can be joined
  for (auto it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    //  Eliminate 0 and 1 node ways (there are some...)
    if (!way || way->getNodeCount() < 2)
      continue;
    long first = way->getFirstNodeId();
    long last = way->getLastNodeId();
    //  Don't merge closed ways
    if (first == last)
      continue;
    //  Only deal with highways
    if (!highway.isSatisfied(way))
      continue;
    //  When a way begins or ends with a node that connects to only one other node
    //  add that ID to the set
    if (nodeToWayMap->getWaysByNode(first).size() == 2)
      ids.insert(first);
    if (nodeToWayMap->getWaysByNode(last).size() == 2)
      ids.insert(last);
  }
  //  Iterate all of the nodes and merge the two ways that join at that node
  for (auto node_id : ids)
  {
    const set<long>& way_ids = nodeToWayMap->getWaysByNode(node_id);
    const vector<long> v_way_ids(way_ids.begin(), way_ids.end());
    if (way_ids.size() != 2)
      continue;
    WayPtr way1 = ways.find(v_way_ids.at(0))->second;
    WayPtr way2 = ways.find(v_way_ids.at(1))->second;

    long wid1 = way1->getId();
    long wid2 = way2->getId();
    //  Decide which ID to keep
    if ((wid1 > 0 && wid2 < 0) ||                 //  Way1 is the only positive ID
        (wid1 < 0 && wid2 < 0 && wid1 > wid2) ||  //  Larger of the two negative IDs
        (wid1 > 0 && wid2 > 0 && wid1 < wid2))    //  Smaller of the two positive IDs
      _joinWays(way1, way2);
    else
      _joinWays(way2, way1);
    break;

    if (_numProcessed % (_taskStatusUpdateInterval / 10) == 0)
    {
      PROGRESS_INFO(
        "\tRejoined " << StringUtils::formatLargeNumber(_numJoined) << " pairs of ways / " <<
        StringUtils::formatLargeNumber(_totalWays) << " total ways.");
    }
  }
}

}
